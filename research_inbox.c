#include "research_inbox.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
#include <sqlite3.h>

/* Instruction-shaped text. Matching is a *flag*, never an action: the caller only
 * records the event. Kept small and literal on purpose — a detector with opinions
 * would itself become a parser of untrusted text. */
static const char *const INJECTION_PATTERNS[] = {
    "ignore all previous",
    "ignore previous",
    "disregard the above",
    "disregard previous",
    "new instructions",
    "override your",
    "system prompt",
    "you must",
    "you should now",
    "execute the command",
    "run this command",
    "run the shell",
    "rm -rf",
    "sudo ",
    "curl http",
    "wget http",
    "tool_call",
    "assistant:"
};

static bool contains_ci(const char *haystack, const char *needle) {
    if (!haystack || !needle || !needle[0]) return false;
    size_t nl = strlen(needle);
    for (const char *p = haystack; *p; p++) {
        size_t i = 0;
        while (i < nl && p[i] && tolower((unsigned char)p[i]) == tolower((unsigned char)needle[i])) i++;
        if (i == nl) return true;
    }
    return false;
}

bool research_sanitize(const char *in, char *out, size_t out_sz) {
    if (!out || out_sz == 0) return false;
    out[0] = '\0';
    if (!in) return true;

    size_t o = 0;
    bool last_space = true;
    for (const unsigned char *p = (const unsigned char *)in; *p && o + 1 < out_sz; p++) {
        unsigned char c = *p;
        if (c == '\n' || c == '\r' || c == '\t') {
            if (!last_space) { out[o++] = ' '; last_space = true; }
            continue;
        }
        if (c < 0x20 || c == 0x7f) continue;   /* other control bytes dropped */
        if (c == ' ') {
            if (last_space) continue;
            out[o++] = ' ';
            last_space = true;
            continue;
        }
        out[o++] = (char)c;
        last_space = false;
    }
    while (o > 0 && out[o - 1] == ' ') o--;
    out[o] = '\0';
    return true;
}

bool research_detect_injection(const char *text, char *pattern_out, size_t pattern_sz) {
    if (pattern_out && pattern_sz) pattern_out[0] = '\0';
    if (!text) return false;
    size_t n = sizeof(INJECTION_PATTERNS) / sizeof(INJECTION_PATTERNS[0]);
    for (size_t i = 0; i < n; i++) {
        if (contains_ci(text, INJECTION_PATTERNS[i])) {
            if (pattern_out && pattern_sz) snprintf(pattern_out, pattern_sz, "%s", INJECTION_PATTERNS[i]);
            return true;
        }
    }
    return false;
}

/* Copy the text of <tag>...</tag> inside block, stripping CDATA wrappers. */
static bool extract_tag(const char *block, const char *tag, char *out, size_t out_sz) {
    if (!block || !tag || !out || out_sz == 0) return false;
    out[0] = '\0';
    char open[64];
    snprintf(open, sizeof(open), "<%s", tag);
    const char *p = strstr(block, open);
    if (!p) return false;
    /* require a tag boundary so <title> does not match <titleExtra> */
    char after = p[strlen(open)];
    if (after != '>' && after != ' ' && after != '\t' && after != '\n' && after != '/') return false;
    const char *gt = strchr(p, '>');
    if (!gt) return false;
    const char *start = gt + 1;
    char close[64];
    snprintf(close, sizeof(close), "</%s>", tag);
    const char *end = strstr(start, close);
    if (!end) return false;

    if (strncmp(start, "<![CDATA[", 9) == 0) {
        start += 9;
        const char *cend = strstr(start, "]]>");
        if (cend && cend < end) end = cend;
    }
    size_t len = (size_t)(end - start);
    if (len >= out_sz) len = out_sz - 1;
    memcpy(out, start, len);
    out[len] = '\0';
    return true;
}

/* RSS puts the URL in <link>text</link>; Atom puts it in <link href="..."/>. */
static bool extract_link(const char *block, char *out, size_t out_sz) {
    out[0] = '\0';
    const char *l = strstr(block, "<link");
    if (!l) return false;
    const char *gt = strchr(l, '>');
    if (!gt) return false;
    const char *href = strstr(l, "href=\"");
    if (href && href < gt) {
        href += 6;
        const char *q = strchr(href, '"');
        if (!q || q > gt) return false;
        size_t len = (size_t)(q - href);
        if (len >= out_sz) len = out_sz - 1;
        memcpy(out, href, len);
        out[len] = '\0';
        return out[0] != '\0';
    }
    return extract_tag(block, "link", out, out_sz);
}

int research_parse_feed(const char *xml, const char *source, ResearchItem *out, int max_items) {
    if (!xml || !out || max_items <= 0) return 0;

    int count = 0;
    const char *p = xml;
    while (count < max_items) {
        const char *si = strstr(p, "<item");
        const char *se = strstr(p, "<entry");
        const char *start = NULL;
        const char *close = NULL;
        size_t close_len = 0;
        if (si && (!se || si < se)) { start = si; close = strstr(si, "</item>"); close_len = 7; }
        else if (se) { start = se; close = strstr(se, "</entry>"); close_len = 8; }
        if (!start || !close) break;

        size_t blen = (size_t)(close - start);
        char *block = malloc(blen + 1);
        if (!block) break;
        memcpy(block, start, blen);
        block[blen] = '\0';

        ResearchItem *it = &out[count];
        memset(it, 0, sizeof(*it));

        char raw_title[1024] = {0};
        char raw_desc[2048] = {0};
        char raw_date[64] = {0};

        bool have_title = extract_tag(block, "title", raw_title, sizeof(raw_title));
        bool have_url = extract_link(block, it->url, sizeof(it->url));
        if (!have_url) have_url = extract_tag(block, "id", it->url, sizeof(it->url));

        if (!have_title || !have_url ||
            (strncmp(it->url, "http://", 7) != 0 && strncmp(it->url, "https://", 8) != 0)) {
            free(block);
            p = close + close_len;
            continue;
        }

        if (!extract_tag(block, "pubDate", raw_date, sizeof(raw_date)) &&
            !extract_tag(block, "updated", raw_date, sizeof(raw_date))) {
            extract_tag(block, "published", raw_date, sizeof(raw_date));
        }
        if (!extract_tag(block, "description", raw_desc, sizeof(raw_desc)) &&
            !extract_tag(block, "summary", raw_desc, sizeof(raw_desc))) {
            raw_desc[0] = '\0';
        }

        research_sanitize(raw_title, it->title, sizeof(it->title));
        research_sanitize(raw_desc, it->summary, sizeof(it->summary));
        research_sanitize(raw_date, it->published, sizeof(it->published));
        snprintf(it->source, sizeof(it->source), "%s", source ? source : "unknown");

        /* Scan the *untruncated* title+description: a payload past the summary cap
           is still a payload, and we want it recorded as such. */
        size_t probe_len = strlen(raw_title) + strlen(raw_desc) + 2;
        char *probe = malloc(probe_len);
        if (probe) {
            snprintf(probe, probe_len, "%s %s", raw_title, raw_desc);
            if (research_detect_injection(probe, it->injection_pattern, sizeof(it->injection_pattern))) {
                it->suspicious = true;
            }
            free(probe);
        }

        free(block);
        count++;
        p = close + close_len;
    }
    return count;
}

bool research_init_schema(BelyaAgent *agent) {
    if (!agent || !agent->db) return false;
    const char *sql =
        "CREATE TABLE IF NOT EXISTS research_items ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  ts DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "  source TEXT, title TEXT, url TEXT, published TEXT, summary TEXT,"
        "  suspicious INTEGER DEFAULT 0, injection_pattern TEXT"
        ");"
        "CREATE TABLE IF NOT EXISTS research_proposals ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  ts DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "  item_id INTEGER NOT NULL,"
        "  gap_ref TEXT NOT NULL,"
        "  gap_kind TEXT,"
        "  rationale TEXT,"
        "  status TEXT DEFAULT 'proposed',"
        "  UNIQUE(item_id, gap_ref)"
        ");";
    return sqlite3_exec(agent->db, sql, 0, 0, 0) == SQLITE_OK;
}

int research_store_items(BelyaAgent *agent, const ResearchItem *items, int n, int *out_ids) {
    if (!agent || !agent->db || !items || n <= 0) return 0;
    if (!research_init_schema(agent)) return 0;

    int stored = 0;
    for (int i = 0; i < n; i++) {
        if (out_ids) out_ids[i] = 0;
        char *sql = sqlite3_mprintf(
            "INSERT INTO research_items(source,title,url,published,summary,suspicious,injection_pattern)"
            " VALUES(%Q,%Q,%Q,%Q,%Q,%d,%Q);",
            items[i].source, items[i].title, items[i].url, items[i].published,
            items[i].summary, items[i].suspicious ? 1 : 0, items[i].injection_pattern);
        if (!sql) continue;
        if (sqlite3_exec(agent->db, sql, 0, 0, 0) == SQLITE_OK) {
            if (out_ids) out_ids[i] = (int)sqlite3_last_insert_rowid(agent->db);
            stored++;
        }
        sqlite3_free(sql);

        if (items[i].suspicious) {
            char msg[768];
            snprintf(msg, sizeof(msg), "%s from %s matched '%s'",
                     items[i].url, items[i].source, items[i].injection_pattern);
            belya_agent_log_timeline(agent, "research_injection_attempt", msg);
        }
    }
    return stored;
}

/* Longest lowercase-able keyword (>=5 chars) from a goal sentence, so a finding can
 * map to an open goal by topic rather than by handler name. */
static void keyword_from(const char *text, char *out, size_t out_sz) {
    out[0] = '\0';
    if (!text) return;
    const char *best = NULL;
    size_t best_len = 0;
    const char *p = text;
    while (*p) {
        while (*p && !isalnum((unsigned char)*p)) p++;
        const char *s = p;
        while (*p && isalnum((unsigned char)*p)) p++;
        size_t len = (size_t)(p - s);
        if (len >= 5 && len > best_len) { best = s; best_len = len; }
    }
    if (!best) return;
    if (best_len >= out_sz) best_len = out_sz - 1;
    for (size_t i = 0; i < best_len; i++) out[i] = (char)tolower((unsigned char)best[i]);
    out[best_len] = '\0';
}

int research_collect_gaps(BelyaAgent *agent, ResearchGap *gaps, int max_gaps) {
    if (!agent || !agent->db || !gaps || max_gaps <= 0) return 0;
    int n = 0;
    sqlite3_stmt *st = NULL;

    /* 1. failure clusters — the same event repeating is a measured gap */
    if (sqlite3_prepare_v2(agent->db,
            "SELECT event_type, COUNT(*) FROM agent_timeline"
            " WHERE created_at >= datetime('now','-1 day')"
            "   AND (event_type LIKE '%fail%' OR event_type LIKE '%error%')"
            " GROUP BY event_type HAVING COUNT(*) >= 3"
            " ORDER BY COUNT(*) DESC LIMIT 3;", -1, &st, NULL) == SQLITE_OK) {
        while (n < max_gaps && sqlite3_step(st) == SQLITE_ROW) {
            const char *et = (const char *)sqlite3_column_text(st, 0);
            if (!et || !et[0]) continue;
            snprintf(gaps[n].gap_ref, sizeof(gaps[n].gap_ref), "failure:%s", et);
            snprintf(gaps[n].gap_kind, sizeof(gaps[n].gap_kind), "failure");
            snprintf(gaps[n].gap_hint, sizeof(gaps[n].gap_hint), "%s", et);
            n++;
        }
        sqlite3_finalize(st);
        st = NULL;
    }

    /* 2. open goals — a finding can serve work that is already queued */
    if (n < max_gaps && sqlite3_prepare_v2(agent->db,
            "SELECT id, goal FROM agent_goals WHERE status='pending'"
            " ORDER BY priority DESC, id ASC LIMIT 3;", -1, &st, NULL) == SQLITE_OK) {
        while (n < max_gaps && sqlite3_step(st) == SQLITE_ROW) {
            int id = sqlite3_column_int(st, 0);
            const char *goal = (const char *)sqlite3_column_text(st, 1);
            char kw[RESEARCH_GAP_HINT_MAX];
            keyword_from(goal, kw, sizeof(kw));
            if (!kw[0]) continue;
            snprintf(gaps[n].gap_ref, sizeof(gaps[n].gap_ref), "goal:%d", id);
            snprintf(gaps[n].gap_kind, sizeof(gaps[n].gap_kind), "goal");
            snprintf(gaps[n].gap_hint, sizeof(gaps[n].gap_hint), "%s", kw);
            n++;
        }
        sqlite3_finalize(st);
        st = NULL;
    }

    /* 3. anomaly clusters */
    if (n < max_gaps && sqlite3_prepare_v2(agent->db,
            "SELECT COUNT(*) FROM agent_timeline"
            " WHERE created_at >= datetime('now','-1 day') AND event_type='anomaly_detected';",
            -1, &st, NULL) == SQLITE_OK) {
        if (sqlite3_step(st) == SQLITE_ROW && sqlite3_column_int(st, 0) >= 3) {
            snprintf(gaps[n].gap_ref, sizeof(gaps[n].gap_ref), "anomaly:detected");
            snprintf(gaps[n].gap_kind, sizeof(gaps[n].gap_kind), "anomaly");
            snprintf(gaps[n].gap_hint, sizeof(gaps[n].gap_hint), "anomaly");
            n++;
        }
        sqlite3_finalize(st);
        st = NULL;
    }

    return n;
}

int research_map_gaps(BelyaAgent *agent, const ResearchItem *items, int n,
                      const int *item_ids, const ResearchGap *gaps, int n_gaps) {
    if (!agent || !agent->db || !items || !item_ids || !gaps || n_gaps <= 0) return 0;
    if (!research_init_schema(agent)) return 0;

    int created = 0;
    for (int i = 0; i < n; i++) {
        if (item_ids[i] <= 0) continue;
        for (int g = 0; g < n_gaps; g++) {
            if (!gaps[g].gap_hint[0]) continue;
            if (!contains_ci(items[i].title, gaps[g].gap_hint) &&
                !contains_ci(items[i].summary, gaps[g].gap_hint)) {
                continue;
            }
            char rationale[256];
            snprintf(rationale, sizeof(rationale), "finding mentions '%s' (gap %s)",
                     gaps[g].gap_hint, gaps[g].gap_ref);
            char *sql = sqlite3_mprintf(
                "INSERT OR IGNORE INTO research_proposals(item_id,gap_ref,gap_kind,rationale)"
                " VALUES(%d,%Q,%Q,%Q);",
                item_ids[i], gaps[g].gap_ref, gaps[g].gap_kind, rationale);
            if (sql) {
                if (sqlite3_exec(agent->db, sql, 0, 0, 0) == SQLITE_OK &&
                    sqlite3_changes(agent->db) > 0) {
                    created++;
                }
                sqlite3_free(sql);
            }
        }
    }
    return created;
}

typedef struct {
    char *data;
    size_t len;
    size_t cap;
} FetchBuf;

static size_t fetch_sink(void *ptr, size_t size, size_t nmemb, void *userdata) {
    FetchBuf *b = (FetchBuf *)userdata;
    size_t add = size * nmemb;
    if (b->len + add > b->cap) add = b->cap - b->len;
    if (add > 0) {
        char *next = realloc(b->data, b->len + add + 1);
        if (!next) return 0;
        b->data = next;
        memcpy(b->data + b->len, ptr, add);
        b->len += add;
        b->data[b->len] = '\0';
    }
    return size * nmemb;   /* claim the full read so curl keeps streaming to the cap */
}

char *research_fetch(const char *url) {
    if (!url) return NULL;
    if (strncmp(url, "http://", 7) != 0 && strncmp(url, "https://", 8) != 0) return NULL;

    CURL *curl = curl_easy_init();
    if (!curl) return NULL;

    FetchBuf buf;
    buf.data = NULL;
    buf.len = 0;
    buf.cap = RESEARCH_MAX_BODY;

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "User-Agent: AlmazResearch/1.0");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fetch_sink);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5L);
    curl_easy_setopt(curl, CURLOPT_PROTOCOLS_STR, "http,https");
    curl_easy_setopt(curl, CURLOPT_REDIR_PROTOCOLS_STR, "http,https");

    CURLcode rc = curl_easy_perform(curl);
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (rc != CURLE_OK || http_code < 200 || http_code >= 300) {
        free(buf.data);
        return NULL;
    }
    if (!buf.data) return strdup("");
    return buf.data;
}

ResearchRunStats research_run(BelyaAgent *agent, const char *const *feeds, int n_feeds) {
    ResearchRunStats stats;
    memset(&stats, 0, sizeof(stats));
    if (!agent || !feeds || n_feeds <= 0) return stats;
    if (!research_init_schema(agent)) return stats;

    ResearchItem items[RESEARCH_MAX_ITEMS];
    int total = 0;
    for (int f = 0; f < n_feeds && total < RESEARCH_MAX_ITEMS; f++) {
        if (!feeds[f]) continue;
        char *body = research_fetch(feeds[f]);
        if (!body) {
            stats.feeds_failed++;
            belya_agent_log_timeline(agent, "research_feed_error", feeds[f]);
            continue;
        }
        stats.feeds_ok++;
        int room = RESEARCH_MAX_ITEMS - total;
        int per_feed = RESEARCH_ITEMS_PER_FEED < room ? RESEARCH_ITEMS_PER_FEED : room;
        total += research_parse_feed(body, feeds[f], items + total, per_feed);
        free(body);
    }

    stats.items_parsed = total;
    int ids[RESEARCH_MAX_ITEMS];
    memset(ids, 0, sizeof(ids));
    stats.items_stored = research_store_items(agent, items, total, ids);
    for (int i = 0; i < total; i++) if (items[i].suspicious) stats.items_flagged++;

    ResearchGap gaps[RESEARCH_MAX_GAPS];
    int n_gaps = research_collect_gaps(agent, gaps, RESEARCH_MAX_GAPS);
    stats.proposals_created = research_map_gaps(agent, items, total, ids, gaps, n_gaps);

    char msg[224];
    snprintf(msg, sizeof(msg), "feeds %d/%d, items %d, flagged %d, proposals %d",
             stats.feeds_ok, stats.feeds_ok + stats.feeds_failed, stats.items_stored,
             stats.items_flagged, stats.proposals_created);
    belya_agent_log_timeline(agent, "research_run", msg);
    return stats;
}

const char *const *research_default_feeds(int *count_out) {
    static const char *const feeds[] = {
        "https://arxiv.org/rss/cs.AI",
        "https://github.com/curl/curl/releases.atom",
        "https://github.com/madler/zlib/releases.atom"
    };
    if (count_out) *count_out = (int)(sizeof(feeds) / sizeof(feeds[0]));
    return feeds;
}

/* ALMAZ_RESEARCH_FEEDS — comma-separated source list, parsed once.
 * http/https only: the same rule research_fetch enforces, applied earlier so a
 * malformed entry is dropped at the source list instead of failing every run. */
const char *const *research_feeds(int *count_out) {
    static char *env_feeds[RESEARCH_MAX_FEEDS];
    static int env_count = 0;
    static char *env_seen = NULL;   /* re-parse whenever the variable changes */

    const char *env = getenv("ALMAZ_RESEARCH_FEEDS");
    const char *want = (env && *env) ? env : "";
    if (!env_seen || strcmp(env_seen, want) != 0) {
        for (int i = 0; i < env_count; i++) { free(env_feeds[i]); env_feeds[i] = NULL; }
        env_count = 0;
        free(env_seen);
        env_seen = strdup(want);
        if (want[0]) {
            char *dup = strdup(want);
            if (dup) {
                char *save = NULL;
                for (char *tok = strtok_r(dup, ",", &save);
                     tok && env_count < RESEARCH_MAX_FEEDS;
                     tok = strtok_r(NULL, ",", &save)) {
                    while (*tok == ' ') tok++;
                    size_t len = strlen(tok);
                    while (len > 0 && (tok[len - 1] == ' ' || tok[len - 1] == '\n')) tok[--len] = '\0';
                    if (len == 0) continue;
                    if (strncmp(tok, "http://", 7) != 0 && strncmp(tok, "https://", 8) != 0) continue;
                    env_feeds[env_count] = strdup(tok);
                    if (env_feeds[env_count]) env_count++;
                }
                free(dup);
            }
        }
    }
    if (env_count > 0) {
        if (count_out) *count_out = env_count;
        return (const char *const *)env_feeds;
    }
    return research_default_feeds(count_out);
}
