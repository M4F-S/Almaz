# 💎 Almaz Sovereign Software Organism: Project State & Session Handoff

**Repository:** `M4F-S/Almaz`  
**Current Version:** `v2.1.0-phase7` (Competitive Parity & Late-2026 Sovereign Organism)  
**Host Environment:** macOS (Darwin ARM64) & Remote VPS Ubuntu 24.04 (`187.124.2.26`)  
**Deployment Daemon:** `/opt/almaz/almaz` under `almaz.service` (supervised by `almaz-watchdog`)  
**Active Inference Engine:** `deepseek-v4.1-flash` via OpenCode Go Flat-Rate Gateway  
**Memory Architecture:** SQLite 3 with FTS5, Gomaa Scoped Wings, FadeMem biological decay, CORAL dual-stream routing, and WAL Mode (`almaz_memory.sqlite`)  

---

## 🏛️ 1. Architectural Blueprint & Cognitive Organs

Almaz is **not a passive chat assistant**. It is a **continuous, self-evolving, self-healing, self-aware software organism** implemented in pure C99 with direct POSIX, bash, and host execution privileges.

### Core Cognitive Organs & Evolutionary Enhancements
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
5. **FadeMem Biological Memory Decay & Concept Fusion:**
   - Exponential power-law retention decay ($R(t) = e^{-\lambda \Delta t}$) with salience protection.
   - Proactive concept fusion clustering duplicate semantic records into unified nodes.
6. **CORAL Dual-Stream Memory Routing:**
   - Strategic vs Tactical memory isolation: routes architectural invariants and user preferences to strategic memory, ephemeral execution artifacts to tactical memory.
7. **SimpleMem Atomic Fact Extraction:**
   - De-linearizes conversational context into discrete atomic subject-predicate-object tuples indexed in FTS5.
8. **Soft-Constraint Relaxation Engine:**
   - Dynamic threshold adaptation under cognitive pressure or repeated verification failure.
9. **Autonomic Subconscious Idle Loop:**
   - Single-threaded event loop in `telegram_adapter.c` long-polling gap.
   - Conservative threshold: Idle $\ge 30$ minutes (1800s), rate-limited to max 3 cycles / 24h, spaced $\ge 2$ hours apart.
   - Introspection payload: Scans `agent_timeline` for anomalies, executes pending intrinsic goals, syncs `self_model`, logs to timeline.
10. **Intrinsic Goal Engine:**
    - Table: `agent_goals` in SQLite. Deterministic generation of `MAINTENANCE`, `CODE_AUDIT`, and `KNOWLEDGE_CONSOLIDATION` goals.
11. **Timeline Anomaly Detector, Budget Controller & Health Watcher:**
    - Real-time telemetry monitoring, anomalous latency detection, spend velocity limiting, and process health diagnostics.

### Competitive Parity Integration (Phases 0–7)
Almaz seamlessly incorporates all 41 native tools and capabilities from the Competitive Parity Master Plan:
- Phase 0: POSIX.1-2008 Prelude & Twin Sync
- Phase 1: Diff Colorizer & Interactive UX (`ask_user`, `/diff`, `/doctor`, `/cost`)
- Phase 2: Deterministic Lifecycle Hooks & 5-Tier Compactor
- Phase 3: SQLite Task Board & Cron Engine (`task_create`, `task_update`, `task_list`, `task_get`, `cron_create`, `cron_list`, `cron_tick`)
- Phase 4: Plan Mode Sandbox & Isolated Worktrees (`enter_plan_mode`, `exit_plan_mode`, `enter_worktree`, `leave_worktree`)
- Phase 5: POSIX PTY Subshells & Background Task Manager (`manage_task`, `background: true`)
- Phase 6: Streaming Web Retrieval, Notebook Splicer & LSP-over-MCP (`web_search`, `web_fetch`, `edit_notebook`, `lsp_definition`, `lsp_references`)
- Phase 7: Omnichannel Gateway (Discord/Slack/Telegram) & Stdin-Piped Remote Backends (`exec_backend`)

---

## 🔬 2. Verification & Safety Metrics

- **Super Strict Unit Test Suite:** **61/61 tests passed (100%)** under Clang/GCC with AddressSanitizer and UndefinedBehaviorSanitizer.
- **Hidden Holdout Test Suite:** **65/65 Assertions passed (100%)** across holdout scenarios.
- **Multi-Arena Benchmarks:** **36/36 Arenas passed (100%)** including organism self-model and cognitive adaptation arenas.
- **Memory Safety:** 100% verified allocation guards, 0 leaks, 0 undefined behavior, stable at ~12.5 MB RSS.
- **Compiler Cleanliness:** Zero warnings under `-Wall -Wextra -Werror -Wpedantic -O2`.

---

## 🚀 3. Production Deployment Status

- **VPS Host:** `187.124.2.26` (Ubuntu 24.04 LTS, root access).
- **Service Name:** `almaz.service` (`systemd`).
- **Supervision:** `almaz-watchdog` process supervisor.
- **Daily Timer:** `/etc/systemd/system/almaz-daily.timer` (03:00 UTC Darwinian evolution cycle).
- **Target Working Directory:** `/opt/almaz`.
- **Git Branch:** `harden/v0.7.1` tracking `origin/harden/v0.7.1`.

---

## 📋 4. Next Session Objectives & Roadmap

1. **Subconscious Organism Telemetry:**
   - Monitor autonomic idle cycles on the VPS and observe emotional appraisal stabilization under real-world usage.
2. **Darwinian Evolution Verification:**
   - Inspect daily mutation history and novelty gating logs (`mutation_history.json`).
3. **Omnichannel Multi-Gateway Activation:**
   - Integrate Slack and Discord hooks alongside Telegram daemon for multi-user organism interaction.
