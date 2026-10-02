#include "budget_store.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void budget_limits_from_env(BudgetLimits *lim) {
    if (!lim) return;
    memset(lim, 0, sizeof(*lim));
    const char *tk = getenv("MODEL_BUDGET_DAILY_TOKENS");
    lim->daily_tokens = tk && strlen(tk) > 0 ? (long)atol(tk) : 5000000L;
    const char *cs = getenv("MODEL_BUDGET_DAILY_COST");
    lim->daily_cost = cs && strlen(cs) > 0 ? atof(cs) : 0.0;
    const char *tm = getenv("MODEL_BUDGET_DAILY_SECONDS");
    lim->daily_seconds = tm && strlen(tm) > 0 ? (long)atol(tm) : 0L;
    const char *inr = getenv("MODEL_INPUT_RATE_PER_M");
    lim->in_rate_per_m = inr && strlen(inr) > 0 ? atof(inr) : 0.0;
    const char *outr = getenv("MODEL_OUTPUT_RATE_PER_M");
    lim->out_rate_per_m = outr && strlen(outr) > 0 ? atof(outr) : 0.0;
}

void budget_state_reset_day(BudgetState *bs) {
    if (!bs) return;
    time_t now = time(NULL);
    struct tm tm_utc;
    gmtime_r(&now, &tm_utc);
    memset(bs->day, 0, BUDGET_DAY_LEN);
    int yr = tm_utc.tm_year + 1900;
    if (yr < 0 || yr > 9999) yr = 0;
    char full[64];
    snprintf(full, sizeof(full), "%04d-%02d-%02d", yr, tm_utc.tm_mon + 1, tm_utc.tm_mday);
    snprintf(bs->day, BUDGET_DAY_LEN, "%.*s", BUDGET_DAY_LEN - 1, full);
    bs->tokens_used = 0;
    bs->calls = 0;
    bs->cost_used = 0.0;
    bs->runtime_sec = 0;
    bs->sealed = 0;
}

static bool day_matches(const BudgetState *bs) {
    return budget_state_day_is_today(bs);
}

bool budget_state_day_is_today(const BudgetState *bs) {
    if (!bs || bs->day[0] == '\0') return false;
    time_t now = time(NULL);
    struct tm tm_utc;
    gmtime_r(&now, &tm_utc);
    int yr = tm_utc.tm_year + 1900;
    if (yr < 0 || yr > 9999) yr = 0;
    char today[64];
    snprintf(today, sizeof(today), "%04d-%02d-%02d", yr, tm_utc.tm_mon + 1, tm_utc.tm_mday);
    return strncmp(bs->day, today, BUDGET_DAY_LEN - 1) == 0;
}

int budget_state_load(const char *path, BudgetState *bs) {
    if (!path || !bs) return -1;
    FILE *fp = fopen(path, "r");
    if (!fp) return -1;
    char line[128];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "DAY=", 4) == 0) {
            strncpy(bs->day, line + 4, BUDGET_DAY_LEN - 1);
            bs->day[BUDGET_DAY_LEN - 1] = '\0';
            size_t n = strlen(bs->day);
            while (n > 0 && (bs->day[n - 1] == '\n' || bs->day[n - 1] == '\r')) bs->day[--n] = '\0';
        } else if (strncmp(line, "TOKENS=", 7) == 0) {
            bs->tokens_used = atol(line + 7);
        } else if (strncmp(line, "CALLS=", 6) == 0) {
            bs->calls = atol(line + 6);
        } else if (strncmp(line, "COST=", 5) == 0) {
            bs->cost_used = atof(line + 5);
        } else if (strncmp(line, "RUNTIME=", 8) == 0) {
            bs->runtime_sec = atol(line + 8);
        } else if (strncmp(line, "SEALED=", 7) == 0) {
            bs->sealed = atoi(line + 7) ? 1 : 0;
        }
    }
    fclose(fp);
    /* Roll over if the file belongs to a previous UTC day. */
    if (!day_matches(bs)) {
        budget_state_reset_day(bs);
    }
    return 0;
}

int budget_state_save(const char *path, const BudgetState *bs) {
    if (!path || !bs) return -1;
    char tmp[384];
    snprintf(tmp, sizeof(tmp), "%s.tmp", path);
    FILE *fp = fopen(tmp, "w");
    if (!fp) return -1;
    fprintf(fp, "DAY=%s\nTOKENS=%ld\nCALLS=%ld\nCOST=%.6f\nRUNTIME=%ld\nSEALED=%d\n",
            bs->day, bs->tokens_used, bs->calls, bs->cost_used, bs->runtime_sec, bs->sealed ? 1 : 0);
    fflush(fp);
    fclose(fp);
    return rename(tmp, path) == 0 ? 0 : -1;
}

bool budget_state_consume(BudgetState *bs, const BudgetLimits *lim,
                          long prompt_tokens, long completion_tokens,
                          long cached_tokens, long duration_sec, bool *sealed) {
    if (!bs || !lim) return false;
    if (sealed) *sealed = false;
    if (!day_matches(bs)) budget_state_reset_day(bs);

    long costed_in = prompt_tokens - (cached_tokens > prompt_tokens ? prompt_tokens : cached_tokens);
    long costed_out = completion_tokens;
    double cost = (double)costed_in / 1e6 * lim->in_rate_per_m +
                  (double)costed_out / 1e6 * lim->out_rate_per_m;

    bool would_exceed = false;
    if (lim->daily_tokens > 0 && bs->tokens_used + prompt_tokens + completion_tokens > lim->daily_tokens) {
        would_exceed = true;
    }
    if (lim->daily_cost > 0 && bs->cost_used + cost > lim->daily_cost) {
        would_exceed = true;
    }
    if (lim->daily_seconds > 0 && bs->runtime_sec + duration_sec > lim->daily_seconds) {
        would_exceed = true;
    }

    if (would_exceed) {
        /* H1: the call has ALREADY been paid for — record its usage, then
           seal. The old code returned false without counting it, so a
           restart reopened the budget for one more call every time. */
        bs->tokens_used += prompt_tokens + completion_tokens;
        bs->calls += 1;
        bs->cost_used += cost;
        bs->runtime_sec += duration_sec;
        bs->sealed = 1;
        if (sealed) *sealed = true;
        return false;
    }

    bs->tokens_used += prompt_tokens + completion_tokens;
    bs->calls += 1;
    bs->cost_used += cost;
    bs->runtime_sec += duration_sec;

    if (lim->daily_tokens > 0 && bs->tokens_used >= lim->daily_tokens) { bs->sealed = 1; if (sealed) *sealed = true; }
    if (lim->daily_cost > 0 && bs->cost_used >= lim->daily_cost) { bs->sealed = 1; if (sealed) *sealed = true; }
    if (lim->daily_seconds > 0 && bs->runtime_sec >= lim->daily_seconds) { bs->sealed = 1; if (sealed) *sealed = true; }
    return true;
}