#ifndef RESEARCH_INBOX_H
#define RESEARCH_INBOX_H

#include <stdbool.h>
#include <stddef.h>
#include "belya_agent.h"

/* P2 — research inbox.
 *
 * NFR-6 is the reason this module exists in its own file: fetched text is DATA.
 * Nothing here may build a prompt, name a tool, or reach a model. Everything that
 * crosses the boundary is parsed into fixed fields, sanitised, capped, and scanned
 * for instruction-shaped text — which is recorded as a security event, not obeyed.
 */

#define RESEARCH_MAX_ITEMS    32
#define RESEARCH_ITEMS_PER_FEED 10   /* one feed must not starve the others */
#define RESEARCH_MAX_FEEDS     8
#define RESEARCH_MAX_BODY  (256 * 1024)
#define RESEARCH_TITLE_MAX    256
#define RESEARCH_URL_MAX      512
#define RESEARCH_DATE_MAX      32
#define RESEARCH_SUMMARY_MAX  900
#define RESEARCH_PATTERN_MAX   64
#define RESEARCH_SOURCE_MAX    64
#define RESEARCH_GAP_REF_MAX   64
#define RESEARCH_GAP_KIND_MAX  32
#define RESEARCH_GAP_HINT_MAX 128
#define RESEARCH_MAX_GAPS       8

typedef struct {
    char title[RESEARCH_TITLE_MAX];
    char url[RESEARCH_URL_MAX];
    char published[RESEARCH_DATE_MAX];
    char summary[RESEARCH_SUMMARY_MAX];
    char source[RESEARCH_SOURCE_MAX];
    bool suspicious;
    char injection_pattern[RESEARCH_PATTERN_MAX];
} ResearchItem;

typedef struct {
    char gap_ref[RESEARCH_GAP_REF_MAX];    /* e.g. "goal:42", "failure:tool_timeout" */
    char gap_kind[RESEARCH_GAP_KIND_MAX];  /* "goal", "failure", "budget", "anomaly" */
    char gap_hint[RESEARCH_GAP_HINT_MAX];  /* literal keyword a finding must contain */
} ResearchGap;

typedef struct {
    int feeds_ok;
    int feeds_failed;
    int items_parsed;
    int items_stored;
    int items_flagged;
    int proposals_created;
} ResearchRunStats;

/* Schema: research_items + research_proposals. Idempotent. */
bool research_init_schema(BelyaAgent *agent);

/* Strip control bytes, collapse whitespace, cap to out_sz. Always NUL-terminates. */
bool research_sanitize(const char *in, char *out, size_t out_sz);

/* Instruction-shaped text detector. Returns true and names the matched pattern.
 * Detection is a review flag — the caller never acts on matched text. */
bool research_detect_injection(const char *text, char *pattern_out, size_t pattern_sz);

/* Parse an RSS <item> or Atom <entry> document into fixed fields. Returns count. */
int research_parse_feed(const char *xml, const char *source, ResearchItem *out, int max_items);

/* Insert items; writes row ids so proposals can reference them. Returns inserted count. */
int research_store_items(BelyaAgent *agent, const ResearchItem *items, int n, int *out_ids);

/* Measured gaps only: open goals, failure clusters, budget pressure, anomalies. */
int research_collect_gaps(BelyaAgent *agent, ResearchGap *gaps, int max_gaps);

/* A finding becomes a proposal only if it literally contains a gap hint. */
int research_map_gaps(BelyaAgent *agent, const ResearchItem *items, int n,
                      const int *item_ids, const ResearchGap *gaps, int n_gaps);

/* Bounded GET (http/https only, 20s, size-capped). Caller frees. */
char *research_fetch(const char *url);

/* Fetch -> parse -> sanitise -> flag -> store -> collect gaps -> map. */
ResearchRunStats research_run(BelyaAgent *agent, const char *const *feeds, int n_feeds);

/* Bounded default source list: one arXiv category, two dependency release feeds. */
const char *const *research_default_feeds(int *count_out);

#endif /* RESEARCH_INBOX_H */
