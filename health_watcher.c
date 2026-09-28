#include "health_watcher.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/statvfs.h>

#define HW_CHECK_INTERVAL 60       /* run() cadence */
#define HW_SQLITE_INTERVAL 600     /* quick_check cadence (10 min) */
#define HW_RETRY_GAP 5             /* seconds between quick_check retries */
#define HW_ALERT_MIN_GAP 300       /* min seconds between any two alerts */
#define HW_DISK_MIN_FREE_PCT 15
#define HW_DISK_HYSTERESIS_PCT 18
#define HW_RSS_HIGH_MB 200
#define HW_RSS_RECOVER_MB 150
#define HW_GW_FAIL_THRESHOLD 5
#define HW_TG_FAIL_THRESHOLD 10
#define HW_TIMELINE_KEEP 2000

void health_watcher_init(HealthWatcher *hw) {
    if (!hw) return;
    memset(hw, 0, sizeof(*hw));
}

long health_watcher_parse_rss_kb(const char *status_text) {
    if (!status_text) return -1;
    const char *marker = strstr(status_text, "VmRSS:");
    if (!marker) return -1;
    marker += strlen("VmRSS:");
    char *end = NULL;
    long val = strtol(marker, &end, 10);
    if (!end || end == marker) return -1;
    while (*end == ' ' || *end == '\t') end++;
    if (strncmp(end, "kB", 2) != 0) return -1;
    return val;
}

int health_watcher_disk_free_pct(const char *path) {
    if (!path) return -1;
    struct statvfs st;
    if (statvfs(path, &st) != 0) return -1;
    if (st.f_blocks == 0) return 100;
    unsigned long long avail = (unsigned long long)st.f_bavail * st.f_frsize;
    unsigned long long total = (unsigned long long)st.f_blocks * st.f_frsize;
    return (int)((avail * 100ULL) / total);
}

static long health_watcher_rss_kb(void) {
    FILE *fp = fopen("/proc/self/status", "r");
    if (!fp) return -1;
    char buf[4096];
    size_t n = fread(buf, 1, sizeof(buf) - 1, fp);
    fclose(fp);
    if (n == 0) return -1;
    buf[n] = '\0';
    return health_watcher_parse_rss_kb(buf);
}

static bool sqlite_quick_check_ok(sqlite3 *db) {
    if (!db) return false;
    sqlite3_stmt *stmt = NULL;
    bool ok = false;
    if (sqlite3_prepare_v2(db, "PRAGMA quick_check;", -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char *txt = sqlite3_column_text(stmt, 0);
            ok = (txt && strcmp((const char *)txt, "ok") == 0);
        }
    }
    sqlite3_finalize(stmt);
    return ok;
}

static bool prune_timeline(sqlite3 *db, int keep) {
    if (!db) return false;
    char sql[192];
    snprintf(sql, sizeof(sql),
             "DELETE FROM agent_timeline WHERE id NOT IN "
             "(SELECT id FROM agent_timeline ORDER BY id DESC LIMIT %d);", keep);
    char *err = NULL;
    int rc = sqlite3_exec(db, sql, NULL, NULL, &err);
    sqlite3_free(err);
    return rc == SQLITE_OK;
}

static bool checkpoint_wal(sqlite3 *db) {
    if (!db) return false;
    return sqlite3_wal_checkpoint_v2(db, NULL, SQLITE_CHECKPOINT_TRUNCATE, NULL, NULL) == SQLITE_OK;
}

static void log_timeline(BelyaAgent *agent, const char *summary) {
    if (!agent) return;
    belya_agent_log_timeline(agent, "health_watcher", summary);
}

/* Observation -> action bridge (Week-3): each health alert transition also
 * enqueues an intrinsic goal so the agent's goal engine can act on it. */
static void enqueue_health_goal(BelyaAgent *agent, const char *detail) {
    if (!agent || !agent->db) return;
    static unsigned long s_incident_seq = 0; /* uniqueness even within one second */
    sqlite3_stmt *stmt = NULL;
    int recent = 0;
    /* 24h TTL on the dedupe: a pending goal older than a day no longer
       suppresses new remediation goals (Kimi K3 P0-2). */
    if (sqlite3_prepare_v2(agent->db,
                           "SELECT COUNT(*) FROM agent_goals WHERE category='HEALTH_REMEDIATION' AND status='pending' AND created_at >= datetime('now','-1 day');",
                           -1, &stmt, NULL) == SQLITE_OK && sqlite3_step(stmt) == SQLITE_ROW) {
        recent = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    if (recent > 0) return; /* already queued within the last 24h */
    char goal[512];
    /* unique text per incident: almaz_goals_add dedupes by exact goal text,
       so an identical older goal would block a NEW remediation goal forever */
    snprintf(goal, sizeof(goal),
             "Investigate and remediate health event: %s (incident %ld-%lu)",
             detail, (long)time(NULL), ++s_incident_seq);
    almaz_goals_add(agent->db, goal, "HEALTH_REMEDIATION", 80, "generated autonomously by health_watcher");
}

static void send_alert(HealthWatcher *hw, TelegramBot *bot, const char *text) {
    time_t now = time(NULL);
    if (hw->last_alert_time > 0 && (now - hw->last_alert_time) < HW_ALERT_MIN_GAP) {
        return;
    }
    hw->last_alert_time = now;
    if (bot && bot->allowed_chat_id && strlen(bot->allowed_chat_id) > 0) {
        telegram_bot_send_message(bot, bot->allowed_chat_id, text);
    }
}

void health_watcher_run(HealthWatcher *hw, BelyaAgent *agent, ModelGateway *gateway, TelegramBot *bot) {
    if (!hw) return;
    time_t now = time(NULL);
    if (hw->last_check_time > 0 && (now - hw->last_check_time) < HW_CHECK_INTERVAL) {
        return;
    }
    hw->last_check_time = now;
    if (!agent) return;

    char summary[512];

    /* 1. Disk pressure (remediation repeats while breached; log only on transition) */
    int free_pct = health_watcher_disk_free_pct(".");
    if (free_pct >= 0 && free_pct < HW_DISK_MIN_FREE_PCT) {
        bool pruned = prune_timeline(agent->db, HW_TIMELINE_KEEP);
        bool cp = checkpoint_wal(agent->db);
        if (!hw->disk_alerted) {
            snprintf(summary, sizeof(summary),
                     "disk pressure: %d%% free -> pruned timeline (keep %d): pruned=%d checkpoint=%d",
                     free_pct, HW_TIMELINE_KEEP, (int)pruned, (int)cp);
            log_timeline(agent, summary);
            char alert[512];
            snprintf(alert, sizeof(alert),
                     "🩺 *[Almaz Self-Heal]* Disk pressure: %d%% free. Timeline pruned + WAL checkpointed.", free_pct);
            send_alert(hw, bot, alert);
            hw->disk_alerted = 1;
            enqueue_health_goal(agent, "disk pressure low free space");
        }
    } else if (free_pct >= HW_DISK_HYSTERESIS_PCT) {
        if (hw->disk_alerted) {
            hw->disk_alerted = 0;
            log_timeline(agent, "disk pressure recovered");
        }
    }

    /* 2. Memory pressure (remediation once per incident — repeated SQLite
       prunes every 60s change nothing and cost IO; Kimi K3 P1) */
    long rss_kb = health_watcher_rss_kb();
    if (rss_kb > 0 && (rss_kb / 1024) >= HW_RSS_HIGH_MB) {
        if (!hw->mem_alerted) {
            bool pruned = prune_timeline(agent->db, HW_TIMELINE_KEEP);
            bool cp = checkpoint_wal(agent->db);
            snprintf(summary, sizeof(summary),
                     "memory pressure: RSS %ld MB (>=%d MB) -> pruned=%d checkpoint=%d",
                     rss_kb / 1024, HW_RSS_HIGH_MB, (int)pruned, (int)cp);
            log_timeline(agent, summary);
            char alert[512];
            snprintf(alert, sizeof(alert),
                     "🩺 *[Almaz Self-Heal]* Memory pressure: RSS %.0f MB. Timeline pruned + WAL checkpointed.",
                     (double)(rss_kb / 1024));
            send_alert(hw, bot, alert);
            hw->mem_alerted = 1;
            enqueue_health_goal(agent, "memory pressure high RSS");
        }
    } else if (rss_kb > 0 && (rss_kb / 1024) < HW_RSS_RECOVER_MB) {
        if (hw->mem_alerted) {
            hw->mem_alerted = 0;
            log_timeline(agent, "memory pressure recovered");
        }
    }

    /* 3. SQLite integrity (expensive: throttled separately) */
    if (agent->db && (hw->last_sqlite_check_time == 0 ||
                      (now - hw->last_sqlite_check_time) >= HW_SQLITE_INTERVAL)) {
        hw->last_sqlite_check_time = now;
        if (!sqlite_quick_check_ok(agent->db)) {
            /* Retry briefly (transient WAL states can trip quick_check). */
            bool persistent = true;
            for (int i = 0; i < 2; i++) {
                sleep(HW_RETRY_GAP);
                if (sqlite_quick_check_ok(agent->db)) { persistent = false; break; }
            }
            if (persistent && !hw->sqlite_alerted) {
                /* single retained recovery copy — a VACUUM INTO backup can be
                   ~2x the DB; keep only the latest (Kimi K3 P1) */
                unlink("/tmp/almaz_db_recovery.db");
                char sql[320];
                snprintf(sql, sizeof(sql), "VACUUM INTO '/tmp/almaz_db_recovery.db';");
                char *err = NULL;
                int vrc = sqlite3_exec(agent->db, sql, NULL, NULL, &err);
                sqlite3_free(err);
                snprintf(summary, sizeof(summary),
                         "sqlite quick_check FAILED -> vacuum backup attempted (/tmp/almaz_db_recovery.db, vacuum_rc=%d). DB NOT deleted.",
                         vrc);
                log_timeline(agent, summary);
                char alert[640];
                snprintf(alert, sizeof(alert),
                         "🚨 *[Almaz Self-Heal]* SQLite integrity check failed. Backup attempted to `/tmp/almaz_db_recovery.db`. "
                         "Operator may need to restore memory DB from backup.");
                send_alert(hw, bot, alert);
                hw->sqlite_alerted = 1;
                enqueue_health_goal(agent, "sqlite integrity check failure");
            } else if (!persistent) {
                if (hw->sqlite_alerted) {
                    hw->sqlite_alerted = 0;
                    log_timeline(agent, "sqlite integrity recovered");
                }
            }
        } else if (hw->sqlite_alerted) {
            hw->sqlite_alerted = 0;
            log_timeline(agent, "sqlite integrity recovered");
        }
    }

    /* 4. Gateway failure streak */
    if (gateway && gateway->consecutive_failures >= HW_GW_FAIL_THRESHOLD) {
        if (!hw->gw_alerted) {
            snprintf(summary, sizeof(summary),
                     "gateway degraded: %d consecutive failures (endpoint %s)",
                     gateway->consecutive_failures, gateway->endpoint ? gateway->endpoint : "?");
            log_timeline(agent, summary);
            char alert[512];
            snprintf(alert, sizeof(alert),
                     "🚨 *[Almaz Self-Heal]* LLM gateway degraded: %d consecutive failures. Check `%s`.",
                     gateway->consecutive_failures, gateway->endpoint ? gateway->endpoint : "endpoint");
            send_alert(hw, bot, alert);
            hw->gw_alerted = 1;
            enqueue_health_goal(agent, "gateway consecutive failures");
        }
    } else if (gateway && gateway->consecutive_failures < HW_GW_FAIL_THRESHOLD) {
        if (hw->gw_alerted) {
            hw->gw_alerted = 0;
            log_timeline(agent, "gateway recovered");
        }
    }

    /* 5. Telegram poll failure streak */
    if (bot && bot->consecutive_poll_failures >= HW_TG_FAIL_THRESHOLD) {
        if (!hw->tg_alerted) {
            snprintf(summary, sizeof(summary),
                     "telegram poll degraded: %d consecutive getUpdates failures",
                     bot->consecutive_poll_failures);
            log_timeline(agent, summary);
            char alert[512];
            snprintf(alert, sizeof(alert),
                     "🚨 *[Almaz Self-Heal]* Telegram long-poll degraded: %d consecutive getUpdates failures.",
                     bot->consecutive_poll_failures);
            send_alert(hw, bot, alert);
            hw->tg_alerted = 1;
            enqueue_health_goal(agent, "telegram poll failures");
        }
    } else if (bot && bot->consecutive_poll_failures < HW_TG_FAIL_THRESHOLD) {
        if (hw->tg_alerted) {
            hw->tg_alerted = 0;
            log_timeline(agent, "telegram poll recovered");
        }
    }

    /* 6. Daily budget sealed (intentional stop, not a gateway failure) */
    if (gateway && gateway->budget_tripped && budget_state_day_is_today(&gateway->budget_state)) {
        if (!hw->budget_alerted) {
            snprintf(summary, sizeof(summary),
                     "daily budget sealed: %ld tokens, %ld calls used",
                     gateway->budget_state.tokens_used, gateway->budget_state.calls);
            log_timeline(agent, summary);
            char alert[512];
            snprintf(alert, sizeof(alert),
                     "⛔ *[Almaz Budget Breaker]* Daily budget sealed: %ld tokens, %ld calls. "
                     "LLM calls paused until UTC midnight.", gateway->budget_state.tokens_used, gateway->budget_state.calls);
            send_alert(hw, bot, alert);
            hw->budget_alerted = 1;
            enqueue_health_goal(agent, "daily budget sealed");
        }
    } else if (gateway && !gateway->budget_tripped) {
        if (hw->budget_alerted) {
            hw->budget_alerted = 0;
            log_timeline(agent, "daily budget reset");
        }
    }
}