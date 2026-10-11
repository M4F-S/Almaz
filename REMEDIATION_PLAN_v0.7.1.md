# Almaz Remediation Plan — v0.7.1 (Hardening)

**Anchored to HEAD:** `af84c39`
**Branch:** `harden/v0.7.1`
**Author:** Almaz (self-review)
**Rule:** Every task begins with `read_file` of the live code at current HEAD. Line
numbers below are a map, not a contract — re-verify before editing.

---

## Gate Definition (applies to every task)

A task is **DONE** only when ALL hold:

1. `make clean && make` compiles clean with `-Wall -Wextra -Werror -std=c99`.
2. `make test` → **41/41** pass under ASan+UBSan.
3. `make holdout` → **52/52** pass under ASan+UBSan.
4. No new `almaz_crashes.log` entries.
5. Change committed atomically; branch tagged on completion.

---

## P0 — Correctness & Memory Safety

### R1 — Eliminate unchecked `strdup()` (daemon-crash class)
**Verified evidence:** `grep -rn "= strdup(" *.c | wc -l` → **115** sites across 10 files:
`belya_agent.c, belya_harness.c, benchmark_runner.c, jev_client.c, linenoise.c,
mcp_client.c, minifrontmatter.c, minijson.c, model_adapter.c, telegram_adapter.c`.
No `xstrdup` helper exists anywhere (`grep -rn xstrdup *.c *.h` → empty).

**Root risk:** OOM → `strdup` returns `NULL` → `strcmp(NULL,…)`/`%s` → SIGSEGV in a
supervised daemon. Recovery restarts the process, but the crash is avoidable.

**Tasks:**
- **R1.1** Add `xstrdup()` to a shared header (candidate: `minijson.h` or a new
  `xalloc.h`). Contract: on `NULL` input return `NULL`; on alloc failure, log to
  stderr and `abort()` (fail-fast in a daemon that must never dereference `NULL`).
  Alternative: return `NULL` and require call-site checks — **decide before coding**,
  document the choice.
- **R1.2** Mechanically replace `= strdup(` → `= xstrdup(` **file-by-file**, one
  commit per file, recompiling after each. Do NOT batch all 115 in one edit.
- **R1.3** For call sites where the duplicated string is later dereferenced without a
  check, add an explicit guard or rely on `abort()` semantics — audit each file.
- **R1.4** Verify count: post-change `grep -rn "= strdup(" *.c` must be **0** (except
  the definition inside `xstrdup`).

**Risk:** LOW mechanically, MEDIUM by volume (115 sites). **Mitigation:** file-scoped
commits + per-file compile.

---

### R3 — Transaction-wrap `almaz_agent_update_self_model`
**Verified evidence:** `belya_agent.c:2223`. Body does:
`UPDATE self_model SET active=0 WHERE active=1;` then `INSERT … active=1;`
with **no `BEGIN`/`COMMIT`/`ROLLBACK`**.

**Root risk:** interruption between the two statements leaves **all rows `active=0`**
→ self-model has no active row → downstream reads degrade.

**Tasks:**
- **R3.1** Wrap the `UPDATE`+`INSERT` in `BEGIN IMMEDIATE; … COMMIT;` with
  `ROLLBACK` on any failure path.
- **R3.2** Add an **init-time self-heal**: on DB open, if no row has `active=1`,
  reactivate the latest `version` (`UPDATE self_model SET active=1 WHERE version =
  (SELECT MAX(version) FROM self_model);`).
- **R3.3** Test: inject a failure between UPDATE and INSERT; assert self-heal
  restores exactly one active row.

**Risk:** LOW. **Gate:** all three above.

---

### R2 — MCP discovery must respect the tool whitelist
**Verified evidence:** `belya_harness.c:2249` `belya_harness_connect_mcp`. Inside the
discovery loop it sets `h->tools[h->tool_count].security = PERM_ALLOW;`
**unconditionally** — it never consults `h->tools_whitelist` (field set at
`belya_harness.c:1909`, helper `belya_harness_is_tool_whitelisted` at `:1923`).
Contrast: the native registration path at `:2222` **does** check
`if (!tools_whitelist) { … }`.

**Root risk:** a discovered MCP tool bypasses the allowlist and auto-executes.

**Resolved default (verified `:2222`):** when `tools_whitelist == NULL`, the harness is
**unrestricted** (it even loads custom tools in that branch). So the MCP gate must
apply **only when a whitelist IS set** — preserving unrestricted-mode behavior.

**Tasks:**
- **R2.1** In the MCP loop, if `h->tools_whitelist` is non-empty, gate registration
  on `belya_harness_is_tool_whitelisted(h->tools_whitelist, t_name)`.
- **R2.2** Policy: **skip** non-whitelisted tools (preferred) vs register as
  `PERM_DENY`/`PERM_ASK`. Document the choice.
- **R2.3** Test: connect a stub MCP server exposing one whitelisted + one
  non-whitelisted tool; assert only the whitelisted one becomes `PERM_ALLOW`.

**Risk:** LOW–MED. Default-whitelist question **RESOLVED** (unrestricted when NULL).

---

## P1 — Consistency & Robustness

### R4 — Unify `success_rate` semantics
**Verified evidence:** two divergent formulas:
- `belya_agent.c:1907` and `:2125`:
  `success_rate = lifetime_successes / lifetime_tool_calls * 100.0`
- `belya_agent.c:2253`:
  `success_rate = lifetime_successes / lifetime_turns`  (and scale `1.0` vs `100.0`)

**Root risk:** telemetry reports two different numbers for the "same" metric.

**Tasks:**
- **R4.1** Extract one helper
  `static double almaz_success_rate(const BelyaAgent *a)` returning a **0.0–1.0
  ratio of tool successes to tool calls** (single source of truth).
- **R4.2** Route `:1907`, `:2125`, `:2253` through it.
- **R4.3** Test: assert all three emit identical values for a fixed counter state.

**Risk:** LOW. **Decision required:** pick `tool_calls` denominator (recommended) vs
`turns`; document rationale.

---

### R5 — SQLite single-writer discipline + `busy_timeout`
**Verified evidence:** `grep -rn "busy_timeout"` → **empty**. WAL set at
`belya_agent.c:89` (`PRAGMA journal_mode=WAL;`). Multiple writers share one DB:
daemon (`belya_agent.c`), watchdog, health watcher (`health_watcher.c:65`),
harness (`belya_harness.c:3171`).

**Root risk:** concurrent writes → `SQLITE_BUSY` → dropped writes / degraded
self-model sync.

**Open-site scope (verified):** only **two** true open sites exist —
`belya_agent.c:87` (`sqlite3_open`, the primary writer) and `main.c:15`
(`sqlite3_open_v2` READONLY self-test). `health_watcher.c` and `belya_harness.c`
**borrow** `sqlite3 *db` from `agent->db` (no own open). So `busy_timeout` on the
primary writer covers all real writers.

**Tasks:**
- **R5.1** Set `PRAGMA busy_timeout=5000;` immediately after `sqlite3_open` at
  `belya_agent.c:87`. (`main.c` is READONLY — optional.)
- **R5.2** Audit each writer for transaction discipline; ensure short transactions.
- **R5.3** Stress test: N concurrent writers for 10s; assert zero `SQLITE_BUSY`
  surfaced as failure and zero `quick_check` errors.

**Risk:** MED. **Gate:** stress test must pass, not just unit tests.

---

## P2 — Self-Model Accuracy

### R6 — Confidence calibration from a rolling window
**Verified evidence:** affect clamped per tool outcome; `lifetime_successes`
accumulates forever → Confidence drifts toward 1.0 over a long life, losing
sensitivity to recent failures.

**Tasks:**
- **R6.1** Add a rolling window (e.g. last N=20 tool outcomes) in the affect state.
- **R6.2** Blend: `confidence = clamp(0.5*window_rate + 0.5*lifetime_rate)`.
  Define exact weights before coding.
- **R6.3** Test: feed 19 successes + 1 failure vs a stale all-success lifetime;
  assert confidence reacts to the recent failure.

**Risk:** LOW–MED (changes observable telemetry). **Decision required:** window size
and weights.

---

## Sequencing & Commits

Order (each a separate atomic commit on `harden/v0.7.1`):

1. `R1.1` xstrdup helper
2. `R1.2` per-file strdup migration (one commit per file)
3. `R3` self-model transaction + self-heal
4. `R2` MCP whitelist gate
5. `R4` success_rate helper
6. `R5` busy_timeout + writer audit
7. `R6` confidence calibration

Tag `v0.7.1-harden` only after **41/41 + 52/52 + stress test** all green and pushed.

---

## Retracted (do NOT implement)

- **~~P2.1 bash token allowlist~~** — **already implemented** in `9fef7fd`
  (metacharacter rejection + exact `argv[0]` `strcmp`; asserted at
  `test_suite.c:1775`). Removing from scope.
- **~~D1 calloc / D5 skill store~~** — previously retracted; not in scope.

---

## Standing Self-Critique

This plan exists because my prior version was built on a **stale snapshot**
(`9fef7fd`) and cited drifted line numbers. Every task above re-anchors to
`af84c39` and re-verifies before editing. If any line number here is wrong at
edit time, **trust the code, not this document.**
