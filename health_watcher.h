#ifndef HEALTH_WATCHER_H
#define HEALTH_WATCHER_H

#include <time.h>
#include "belya_agent.h"
#include "model_adapter.h"
#include "telegram_adapter.h"

/* In-daemon health watcher (Week-2 autonomy).
 * Single-threaded: call health_watcher_run() periodically from the main loop.
 * Detects + remediates (not just reports):
 *   - SQLite corruption (quick_check)  -> backup + alert
 *   - Disk pressure (statvfs)          -> prune timeline + checkpoint + alert
 *   - Memory pressure (/proc/self RSS) -> prune + checkpoint + alert
 *   - Gateway failure streak           -> alert
 *   - Telegram poll failure streak     -> alert
 * All actions logged to agent_timeline so the self-model sees them.
 * Alerts are rate-limited (one per condition until it recovers). */

typedef struct {
    time_t last_check_time;
    time_t last_sqlite_check_time;
    time_t last_alert_time;
    int disk_alerted;
    int sqlite_alerted;
    int mem_alerted;
    int gw_alerted;
    int tg_alerted;
} HealthWatcher;

void health_watcher_init(HealthWatcher *hw);
void health_watcher_run(HealthWatcher *hw, BelyaAgent *agent, ModelGateway *gateway, TelegramBot *bot);

/* Pure helper exposed for tests. */
long health_watcher_parse_rss_kb(const char *status_text);
int health_watcher_disk_free_pct(const char *path);

#endif /* HEALTH_WATCHER_H */