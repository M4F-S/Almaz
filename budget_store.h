#ifndef BUDGET_STORE_H
#define BUDGET_STORE_H

#include <stdbool.h>
#include <time.h>

/* Week-4 autonomy: daily budget/cost/time circuit breaker (C99, zero deps).
 * State is persisted to a tiny text file so budget survives daemon restarts.
 * Single-process design (one almaz child at a time via watchdog). */

#define BUDGET_DAY_LEN 16   /* "YYYY-MM-DD" + NUL */

typedef struct {
    long daily_tokens;      /* cap on total tokens/day (0 = disabled) */
    double daily_cost;      /* cap on $/day (0 = disabled) */
    long daily_seconds;     /* cap on cumulative gateway seconds/day (0 = disabled) */
    double in_rate_per_m;   /* $ per 1M input tokens */
    double out_rate_per_m;  /* $ per 1M output tokens */
} BudgetLimits;

typedef struct {
    char day[BUDGET_DAY_LEN];
    long tokens_used;
    long calls;
    double cost_used;
    long runtime_sec;
    int sealed;             /* H1: 1 once any cap has been hit for this day */
} BudgetState;

/* Fill limits from environment (MODEL_BUDGET_DAILY_TOKENS / _COST / _SECONDS,
 * MODEL_INPUT_RATE_PER_M / MODEL_OUTPUT_RATE_PER_M). Safe defaults when unset:
 * 5,000,000 daily tokens, cost/time disabled, rates 0. */
void budget_limits_from_env(BudgetLimits *lim);

/* Set day to UTC today. */
void budget_state_reset_day(BudgetState *bs);

/* Atomic load: returns 0 on success, -1 if missing/unreadable. */
int budget_state_load(const char *path, BudgetState *bs);

/* True when the state belongs to the current UTC day. */
bool budget_state_day_is_today(const BudgetState *bs);

/* Atomic save via temp+rename. */
int budget_state_save(const char *path, const BudgetState *bs);

/* Consume one gateway call. Returns false if the call is NOT allowed
 * (would exceed the daily token/cost/time caps). On acceptance, updates
 * counters (clamping at caps) and sets sealed=true when a cap is hit. */
bool budget_state_consume(BudgetState *bs, const BudgetLimits *lim,
                          long prompt_tokens, long completion_tokens,
                          long cached_tokens, long duration_sec, bool *sealed);

#endif /* BUDGET_STORE_H */