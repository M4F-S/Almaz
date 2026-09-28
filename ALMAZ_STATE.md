# 💎 Almaz Sovereign Software Organism: Project State & Session Handoff

**Repository:** `M4F-S/Almaz`  
**Current Version:** `v0.2.0-organism`  
**Host Environment:** macOS (Darwin ARM64) & Remote VPS Ubuntu 24.04 (`187.124.2.26`)  
**Deployment Daemon:** `/opt/almaz/almaz` under `almaz.service` (supervised by `almaz-watchdog`)  
**Active Inference Engine:** `deepseek-v4.1-flash` via OpenCode Go Flat-Rate Gateway  
**Memory Architecture:** SQLite 3 with FTS5, Gomaa Scoped Wings, and WAL Mode (`almaz_memory.sqlite`)  

---

## 🏛️ 1. Architectural Blueprint & Philosophy

Almaz is **not a passive chat assistant**. It is a **continuous, self-evolving, self-healing, self-aware software organism** implemented in pure C99 with direct POSIX, bash, and host execution privileges.

### Core Cognitive Organs
1. **Proprioception & Internal Telemetry (Zone 3b):**
   - Injected ephemerally into every reasoning turn:
     - Host OS, PID, Active RSS Memory (MB)
     - Session Turn vs Lifetime Cumulative Turns
     - Lifetime Tool Invocations & Tool Success Rate %
     - Emotional State: Confidence (0.00–1.00) & Frustration (0.00–1.00)
     - Dynamic Affective Warning if Frustration $\ge 0.60$
2. **Persistent Closed-Loop Self-Model (SARSI):**
   - Table: `self_model` in SQLite (`id`, `version`, `capabilities`, `weaknesses`, `performance_stats`, `active`, `created_at`).
   - Dynamic synchronization via `almaz_agent_sync_self_model()` across every session turn and after each daily Darwinian mutation.
3. **Emotional Appraisal State Machine (VIGIL EmoBank):**
   - Dynamically clamped on every tool observation:
     - Tool Success: Confidence $+0.05$, Frustration $-0.10$
     - Tool Failure: Confidence $-0.15$, Frustration $+0.20$
4. **KnowSelf Situational Memory Gating:**
   - Evaluates input context deterministically to decide when to retrieve FTS5 associative memory, eliminating context bloat on trivial shell commands.
5. **Autonomic Subconscious Idle Loop:**
   - Single-threaded event loop in `telegram_adapter.c` long-polling gap.
   - Conservative threshold: Idle $\ge 30$ minutes (1800s), rate-limited to max 3 cycles / 24h, spaced $\ge 2$ hours apart.
   - Introspection payload: Scans `agent_timeline` for anomalies, executes pending intrinsic goals, syncs `self_model`, logs to timeline. Silent by default (no Telegram spam unless RSS $> 500$ MB).
6. **Intrinsic Goal Engine:**
   - Table: `agent_goals` in SQLite.
   - Deterministic generator (`almaz_goals_generate_deterministic`):
     - `MAINTENANCE`: Timeline pruning, self-model refresh, database maintenance.
     - `CODE_AUDIT`: Git working tree cleanliness (`git status --porcelain`).
     - `KNOWLEDGE_CONSOLIDATION`: Distills recurring failure heuristics into memory.

---

## 🔬 2. Verification & Safety Metrics

- **Memory Safety:** 100% verified allocation guards (**63/63 dynamic allocations checked for NULL**).
- **String Safety:** 0 unbounded legacy string calls (`gets`, `strcpy`, `strcat` eliminated; bounded `strncpy` and bounded `memcpy` enforced).
- **Sanitizer Cleanliness:**
  - **38/38 Unit Tests Passed (100%)** under Clang/GCC AddressSanitizer and UndefinedBehaviorSanitizer.
  - **52/52 Hidden Holdout Assertions Passed (100%)** under AddressSanitizer and UndefinedBehaviorSanitizer.
- **Stability:** `almaz_crashes.log` is clean (**0 crashes recorded**). Memory footprint stable at ~15 MB RSS.

---

## 🚀 3. Production Deployment Status

- **VPS Host:** `187.124.2.26` (root, ssh key `~/.ssh/id_vps`).
- **Path:** `/opt/almaz`
- **Systemd Unit:** `/etc/systemd/system/almaz.service` (running `almaz-watchdog --telegram`).
- **Daily Timer:** `/etc/systemd/system/almaz-daily.timer` (triggers `almaz-daily.service` at 03:00 UTC).
- **Telegram Bot Commands:**
  - `/status`: Live organism telemetry (Lifetime turns, RSS, tool success %, cache economics).
  - `/goals`: Intrinsic goal queue and completion summaries.
  - `/cycle`: Triggers immediate manual autonomic subconscious cognition cycle.
  - `/selfmodel`: Outputs active SARSI self-model JSON record.
  - `/timeline`: Recent Gomaa timeline events.
  - `/checkpoint` / `/rollback`: Git and SQLite transactional state management.

---

## 📋 4. Immediate Backlog for New Almaz Session

1. **Novelty Gating in Darwinian Evolution Pool:**
   - In `tools/evolution_supervisor.py`, add SHA-256 fingerprinting of rejected patches in `mutation_history.json`.
   - Disallow re-testing previously failed phenotypes.
2. **Active Self-Healing Rule & Skill Synthesis:**
   - When recurring errors appear $\ge 3\times$ in `agent_timeline`, automatically author a preventative rule in `rules/` or pattern in `TROUBLESHOOTING.md`.
3. **Autonomic SQLite WAL Truncation:**
   - Add `PRAGMA wal_checkpoint(TRUNCATE)` to the autonomic maintenance cycle so journal files never accumulate disk bloat.
4. **Bash Command Parser Upgrade:**
   - Upgrade `tool_bash` prefix/substring matching to full tokenized verb validation to block command chaining via `;`, `&&`, `|`.
5. **MCP Security Integration:**
   - Enforce tool whitelist and permission prompts on tools discovered via MCP servers.
