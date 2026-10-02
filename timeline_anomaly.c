#include "timeline_anomaly.h"
#include "belya_harness.h" /* almaz_goals_add */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ANOMALY_WINDOW_DAYS 7
#define ANOMALY_MIN_COUNT 5      /* too few events -> noise */
#define ANOMALY_SIGMA 3.0
#define ANOMALY_DAYS_FULL 6      /* M7: six full calendar days of history, zero-filled */
#define ANOMALY_MAX_TYPES 64
#define ANOMALY_MAX_DAYS 64

static int cmp_double(const void *a, const void *b) {
    double x = *(const double *)a;
    double y = *(const double *)b;
    return (x > y) - (x < y);
}

/* Already logged an anomaly for this event_type today? (prevents feedback
 * loops: the anomaly scan reads the very table it writes into.) */
static bool anomaly_logged_today(sqlite3 *db, const char *type) {
    if (!db || !type) return false;
    sqlite3_stmt *stmt = NULL;
    const char *sql =
        "SELECT COUNT(*) FROM agent_timeline "
        "WHERE event_type='anomaly_detected' "
        "AND date(created_at) = date('now') "
        "AND summary LIKE 'anomaly_detected: event_type=' || ? || ' %';";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return false;
    /* H5: the summary writer truncates the type to %.20s — the dedupe must
       match the same truncation or it never fires for long event types. */
    char tbuf[21];
    snprintf(tbuf, sizeof(tbuf), "%.20s", type ? type : "");
    sqlite3_bind_text(stmt, 1, tbuf, -1, SQLITE_STATIC);
    bool logged = sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0;
    sqlite3_finalize(stmt);
    return logged;
}

/* Goal dedupe WITH a 24h TTL: a pending goal older than a day no longer
 * suppresses new remediation goals (Kimi K3 P0-2). */
static bool pending_goal_recent(sqlite3 *db) {
    if (!db) return false;
    sqlite3_stmt *stmt = NULL;
    const char *sql =
        "SELECT COUNT(*) FROM agent_goals "
        "WHERE category='ANOMALY_REMEDIATION' AND status='pending' "
        "AND created_at >= datetime('now', '-1 day');";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return false;
    bool recent = sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0;
    sqlite3_finalize(stmt);
    return recent;
}

int timeline_anomaly_scan(BelyaAgent *agent) {
    if (!agent || !agent->db) return -1;

    /* UTC date for the anomaly goal text (day=X diagnostic in the goal). */
    char today[16];
    time_t now = time(NULL);
    struct tm tm_utc;
    gmtime_r(&now, &tm_utc);
    int yr = tm_utc.tm_year + 1900;
    if (yr < 0 || yr > 9999) yr = 0;
    char today_full[64];
    snprintf(today_full, sizeof(today_full), "%04d-%02d-%02d", yr, tm_utc.tm_mon + 1, tm_utc.tm_mday);
    snprintf(today, sizeof(today), "%.*s", (int)sizeof(today) - 1, today_full);

    sqlite3_stmt *stmt = NULL;
    /* M7: use calendar-day offsets so event-less days are included as zeros
       (previous -7d window made missing days absent -> inflated mean/sigma). */
    const char *sql =
        "SELECT event_type, "
        "       CAST(julianday(date(created_at)) - julianday(date('now')) AS INTEGER) AS day_off, "
        "       COUNT(*) "
        "FROM agent_timeline "
        "WHERE date(created_at) >= date('now', '-6 days') "
        "GROUP BY event_type, day_off;";
    if (sqlite3_prepare_v2(agent->db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }

    char types[ANOMALY_MAX_TYPES][96];
    memset(types, 0, sizeof(types));
    double history[ANOMALY_MAX_TYPES][ANOMALY_MAX_DAYS];
    int hist_n[ANOMALY_MAX_TYPES] = {0};
    double todays[ANOMALY_MAX_TYPES] = {0};
    int ntypes = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *type = (const char *)sqlite3_column_text(stmt, 0);
        int day_off = sqlite3_column_int(stmt, 1);
        int c = sqlite3_column_int(stmt, 2);
        if (!type || c <= 0) continue;
        int idx = -1;
        for (int i = 0; i < ntypes; i++) {
            if (types[i][0] != '\0' && strcmp(types[i], type) == 0) { idx = i; break; }
        }
        if (idx < 0 && ntypes < ANOMALY_MAX_TYPES) {
            idx = ntypes++;
            strncpy(types[idx], type, 95);
            types[idx][95] = '\0';
            /* M7: pre-fill six full calendar days with zeros so event-less
               days actually count toward mean/sigma. */
            for (int k = 0; k < ANOMALY_DAYS_FULL; k++) history[idx][k] = 0.0;
            hist_n[idx] = ANOMALY_DAYS_FULL;
        }
        if (idx < 0) continue;
        if (day_off == 0) {
            todays[idx] = (double)c;
        } else if (day_off >= -ANOMALY_DAYS_FULL && day_off <= -1) {
            history[idx][-day_off - 1] = (double)c;  // -1 -> slot 0 (yesterday), -6 -> slot 5
        }
    }
    sqlite3_finalize(stmt);

    int anomalies = 0;
    for (int i = 0; i < ntypes; i++) {
        int n = hist_n[i];
        double today_count = todays[i];
        if (n < 2 || today_count <= 0) continue;
        double sorted[ANOMALY_MAX_DAYS];
        memcpy(sorted, history[i], (size_t)n * sizeof(double));
        qsort(sorted, (size_t)n, sizeof(double), cmp_double);
        double sum = 0.0;
        for (int j = 0; j < n; j++) sum += sorted[j];
        double mean = sum / (double)n;
        double var = 0.0;
        for (int j = 0; j < n; j++) {
            double d = sorted[j] - mean;
            var += d * d;
        }
        double sigma = (n > 1) ? sqrt(var / (double)(n - 1)) : 0.0;

        bool anomalous = sigma > 0.0 && today_count >= ANOMALY_MIN_COUNT &&
                         today_count > mean + ANOMALY_SIGMA * sigma;
        if (!anomalous) continue;
        if (anomaly_logged_today(agent->db, types[i])) continue;

        char summary[256];
        snprintf(summary, sizeof(summary),
                 "anomaly_detected: event_type=%.20s today=%d (7d mean=%.1f sigma=%.1f threshold=%.1f)",
                 types[i], (int)today_count, mean, sigma, mean + ANOMALY_SIGMA * sigma);
        belya_agent_log_timeline(agent, "anomaly_detected", summary);

        if (!pending_goal_recent(agent->db)) {
            char goal[320];
            snprintf(goal, sizeof(goal),
                     "Investigate timeline anomaly: event_type=%.20s (today=%d > mean+3σ, day=%s)",
                     types[i], (int)today_count, today);
            almaz_goals_add(agent->db, goal, "ANOMALY_REMEDIATION", 70, "generated by timeline_anomaly_scan");
        }
        anomalies++;
    }

    return anomalies;
}