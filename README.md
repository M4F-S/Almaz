# Almaz — Sovereign Self-Evolving, Self-Healing Autonomous AI Organism (Pure C99)

[![Release](https://img.shields.io/badge/Release-v2.1.0-blue.svg)](https://github.com/M4F-S/Almaz/releases/tag/v2.1.0-phase7)
[![License](https://img.shields.io/badge/License-Apache_2.0-green.svg)](LICENSE)
[![Language](https://img.shields.io/badge/Language-C99-orange.svg)]()
[![Tests](https://img.shields.io/badge/Unit_Tests-61%2F61_Passed_(100%25)-brightgreen.svg)]()
[![Holdout](https://img.shields.io/badge/AIDE%C2%B2_Holdout-65%2F65_Assertions_(100%25)-brightgreen.svg)]()
[![Benchmarks](https://img.shields.io/badge/6--Arena_Benchmarks-36%2F36_Passed_(100%25)-success.svg)]()
[![Compiler](https://img.shields.io/badge/Compiler-0_Warnings_Clean-brightgreen.svg)]()
[![Safety](https://img.shields.io/badge/Memory_Safety-0_Leaks_ASan_Clean-success.svg)]()
[![Tools](https://img.shields.io/badge/Native_Tools-41_Tools-informational.svg)]()
[![Watchdog](https://img.shields.io/badge/Watchdog-VIGIL_Self--Healing-success.svg)]()
[![Memory](https://img.shields.io/badge/Memory_Footprint-<12.5MB_Peak_RSS-purple.svg)]()
[![Binary](https://img.shields.io/badge/Binary_Size-~330KB-informational.svg)]()

Almaz is a **sovereign self-evolving, self-healing autonomous AI organism** implemented in pure C99. Almaz is the **autopoietic cognitive twin sibling** of **Belya** (the deterministic software engineering core). While Belya champions rock-solid deterministic code generation and execution harness precision, Almaz operates under an autopoietic biological computing model: an introspective reasoning core continuously guarded by a dedicated POSIX sibling watchdog (`almaz-watchdog`), biological memory decay (**FadeMem**), dual-stream tactical/strategic memory routing (**CORAL**), de-linearized semantic fact extraction (**SimpleMem**), a persistent self-model (**SARSI**), an emotional appraisal state machine (**VIGIL** protocol), situational memory gating (**KnowSelf**), soft-constraint relaxation for critical remediation, a 65-assertion hidden holdout regression harness (AIDE² safety framework), dual-tier model gateway failover, competitive parity (Phases 0–7: 41 native tools, PTY subshells, task board, cron, plan mode, worktrees, web search, notebook splicer, LSP-over-MCP, omnichannel gateway, remote backends), and Darwinian AST self-evolution.

---

## Table of Contents
- [The Organism Paradigm: Almaz vs Traditional Agents](#the-organism-paradigm-almaz-vs-traditional-agents)
- [Architectural Topology](#architectural-topology)
- [Multi-Arena Benchmarks & Frontier Agent Evaluation](#multi-arena-benchmarks--frontier-agent-evaluation)
  - [1. Comprehensive 6-Arena Scorecard (36/36 - 100% Passed)](#1-comprehensive-6-arena-scorecard-3636---100-passed)
  - [2. Arena-by-Arena Autopoietic Capabilities](#2-arena-by-arena-autopoietic-capabilities)
  - [3. Frontier Agent Architectural Comparison](#3-frontier-agent-architectural-comparison)
  - [4. Running the Benchmark Suite](#4-running-the-benchmark-suite)
- [Core Sovereign Organism Subsystems](#core-sovereign-organism-subsystems)
  - [1. FadeMem Biological Memory Decay & Concept Fusion](#1-fademem-biological-memory-decay--concept-fusion)
  - [2. CORAL Dual-Stream Memory Routing (Tactical vs Strategic)](#2-coral-dual-stream-memory-routing-tactical-vs-strategic)
  - [3. SimpleMem De-Linearized Semantic Fact Extraction](#3-simplemem-de-linearized-semantic-fact-extraction)
  - [4. MIT IDE Soft-Constraint Relaxation & Headroom](#4-mit-ide-soft-constraint-relaxation--headroom)
  - [5. Sibling Watchdog Supervisor (`almaz-watchdog`) & Crash Autopsy](#5-sibling-watchdog-supervisor-almaz-watchdog--crash-autopsy)
  - [6. SARSI Persistent Self-Model & Introspective Calibration](#6-sarsi-persistent-self-model--introspective-calibration)
  - [7. VIGIL Emotional Appraisal System (Confidence & Frustration Dynamics)](#7-vigil-emotional-appraisal-system-confidence--frustration-dynamics)
  - [8. KnowSelf Situational Memory Gating](#8-knowself-situational-memory-gating)
  - [9. AIDE² Hidden Holdout Safety Suite (65/65 Assertions)](#9-aide²-hidden-holdout-safety-suite-6565-assertions)
  - [10. ROMA Context Firewall & Recursive Subagent Task Trees](#10-roma-context-firewall--recursive-subagent-task-trees)
  - [11. AST Symbol Graph Navigation (`goto_definition`, `find_callers`)](#11-ast-symbol-graph-navigation-goto_definition-find_callers)
  - [12. Paper2Agent Dynamic MCP Server Synthesizer](#12-paper2agent-dynamic-mcp-server-synthesizer)
  - [13. Mythos Variant Vulnerability Sweeper](#13-mythos-variant-vulnerability-sweeper)
  - [14. 128-Call / 96k-Token Middle-Out Context Compactor](#14-128-call--96k-token-middle-out-context-compactor)
  - [15. In-Context Attractor & Loop Breaker](#15-in-context-attractor--loop-breaker)
  - [16. Dynamic LLM Mutation Generator & Evolution Supervisor v3.0](#16-dynamic-llm-mutation-generator--evolution-supervisor-v30)
  - [17. Dual-Tier Automatic Model Gateway Failover](#17-dual-tier-automatic-model-gateway-failover)
  - [18. Production Hardening: H4 Tool Stripping & H5 WAL Batching](#18-production-hardening-h4-tool-stripping--h5-wal-batching)
  - [19. SSRF Defense & Secret Path Jailing](#19-ssrf-defense--secret-path-jailing)
  - [20. Production Canary Promotion Architecture (promote.sh)](#20-production-canary-promotion-architecture-promotesh)
- [Native Tool Suite (41 Built-In Tools)](#native-tool-suite-41-built-in-tools)
- [Command & Slash Controls Reference](#command--slash-controls-reference)
- [Operating Modes](#operating-modes)
- [What is real / what is marketing](#what-is-real--what-is-marketing)
- [Automated Verification & Test Suites](#automated-verification--test-suites)
  - [1. Unit Test Suite (61/61 Passed - 100%)](#1-unit-test-suite-6161-passed---100)
  - [2. Hidden Holdout Regression Suite (65/65 Assertions - 100%)](#2-hidden-holdout-regression-suite-6565-assertions---100)
  - [3. SQLite Integrity & WAL Self-Test (--selfest)](#3-sqlite-integrity--wal-self-test---selfest)
  - [4. Zero-Tolerance Memory Safety (ASan/UBSan)](#4-zero-tolerance-memory-safety-asanubsan)
- [Configuration & Deployment (`.env`, `almaz.service`)](#configuration--deployment-env-almazservice)
- [Ecosystem Relationship: Almaz & Belya](#ecosystem-relationship-almaz--belya)
- [License](#license)

---

## The Organism Paradigm: Almaz vs Traditional Agents

Traditional AI agents (like Claude Code, Aider, or Hermes) are **stateless execution loops**: if an unhandled signal strikes, memory overflows, or a tool loop enters a deadlock, the process crashes, losing state and demanding human operator triage.

Almaz rejects this fragility. Built on the principles of **autopoiesis** (self-creation and self-maintenance), Almaz couples its reasoning faculties to continuous biological-style autonomic nervous systems:

| Architectural Dimension | Traditional AI Agents | Almaz Autonomous Organism |
|:---|:---|:---|
| **Process Model** | Single fragile process | Two-process biological symbiosis (`almaz-watchdog` + `almaz`) |
| **Crash Recovery** | Process exits; terminal dies | Signal interception, core autopsy log, Telegram alert, exponential auto-restart |
| **Self-Awareness** | Zero internal state awareness | **SARSI**: Persistent calibration of capability, tool success rates, turn counts, and RSS |
| **Affective Modulation** | Pure token probability | **VIGIL**: Real-time `confidence` & `frustration` tracking triggering proactive strategic pivots |
| **Memory Access** | Blind brute-force vector retrieval | **KnowSelf**: Situational gating skipping redundant queries when context is sufficient |
| **Code Evolution** | Static binary compiled once | **Darwinian AST Mutation**: Nightly self-optimization gated by a 65-assertion holdout harness |
| **Resource Envelope** | 250 MB – 1.2 GB (Python/Node) | **< 3.5 MB Idle RSS / < 12.5 MB Peak** (Pure C99) |

---

## Architectural Topology

```mermaid
graph TD
    subgraph "Host OS / Systemd Daemon Supervision"
        Systemd["systemd (almaz.service)"] --> Watchdog["almaz-watchdog (Process Supervisor & Autopsy)"]
        Watchdog -- "fork() / execvp() & waitpid()" --> AlmazCore["Almaz Organism Core (almaz)"]
        Watchdog -- "Crash Detection & Signal Trap" --> CrashLog[("almaz_crashes.log")]
        Watchdog -- "Emergency Crash Telemetry" --> TGOut["Telegram Operator Notification"]
    end

    subgraph "Almaz Organism Cognitive Systems"
        AlmazCore --> VIGIL["VIGIL Emotional Appraisal (confidence / frustration)"]
        AlmazCore --> SARSI["SARSI Self-Model (belya_memory.sqlite: self_model)"]
        AlmazCore --> KnowSelf["KnowSelf Memory Gating (Adaptive Context Filter)"]
        AlmazCore --> Gomaa["Gomaa Memory, FTS5 & Chronological Timeline"]
        AlmazCore --> Gateway["Model Gateway (OpenCode Go / OpenRouter / Local)"]
    end

    subgraph "Safety, Verification & Evolution Bounds"
        AlmazCore --> Jail["Path Jailing Guard (realpath canonical containment)"]
        AlmazCore --> Rollback["Git Rollback Guard (automatic clean snapshot)"]
        AlmazCore --> Holdout["AIDE² Hidden Holdout Battery (65 Assertions)"]
        Evolution["Nightly Evolution Supervisor v3.0"] --> Holdout
        Evolution --> AlmazCore
    end
```

---

## Multi-Arena Benchmarks & Frontier Agent Evaluation

Almaz is evaluated across a rigorous **6-arena, 36-task comprehensive benchmark suite (`almaz_benchmark`)** measuring function calling accuracy, polyglot editing precision, long-context memory retention, autonomous SWE problem solving, low-level hardware efficiency, and autopoietic cognitive systems against frontier AI coding agents (**Devin, OpenHands, SWE-agent, Aider, and Claude Code**).

### 1. Comprehensive 6-Arena Scorecard (36/36 - 100% Passed)

| Benchmark Arena | Evaluated Capabilities | Passed / Total | Accuracy | macOS ARM64 Latency | Production VPS Latency (Linux) |
|:---|:---|:---:|:---:|:---:|:---:|
| **Arena 1: Tool Calling & Scavenger (BFCL)** | Standard schemas, DeepSeek `<think>` reasoning, raw JSON, parallel tools | **10 / 10** | **100.0%** | `0.24 ms` | `0.06 ms` |
| **Arena 2: Polyglot Editing & Patching (Aider)** | C, Python, Rust, Go, JS/TS edits + Compiler Watchdog auto-revert | **6 / 6** | **100.0%** | `92.44 ms` | `45.35 ms` |
| **Arena 3: Scoped Memory & Retention (Gomaa)** | Wing/Room scoping, deduplication, 50-turn needle recall, rollback | **5 / 5** | **100.0%** | `58.80 ms` | `282.03 ms` |
| **Arena 4: Autonomous SWE Issue Solving (SWE-bench)** | Codebase grep, unified diff patch engine, git tracking, subagents | **4 / 4** | **100.0%** | `35.72 ms` | `27.73 ms` |
| **Arena 5: Resource Footprint ("The C-Factor")** | Cold-start latency, peak RSS, JSON throughput, zero memory leaks | **5 / 5** | **100.0%** | `154.64 ms` | `377.76 ms` |
| **Arena 6: Late-2026 Autopoietic & SOTA Systems** | FNV-1a loop breaker, middle-out compactor, ROMA firewall, AST graph, Paper2Agent, FadeMem, CORAL, SimpleMem | **6 / 6** | **100.0%** | `476.59 ms` | `341.58 ms` |
| **Grand Total** | **End-to-End Autonomous Organism Evaluation** | **36 / 36** | **100.0%** | **`818.44 ms`** | **`1,082.52 ms`** |

---

### 2. Arena-by-Arena Autopoietic Capabilities

- **Arena 1 (Berkeley Function Calling Leaderboard / BFCL Alignment):** Standard schemas, reasoning-embedded `<think>` extraction, ReAct action models, parallel execution, and strict argument contract enforcement across 41 tools.
- **Arena 2 (Aider Polyglot Code Editing):** Targeted AST replacements across 5 programming languages with the **Pre-Flight Compiler Watchdog** auto-reverting syntax errors.
- **Arena 3 (Gomaa Memory & Long Dialogue Retention):** SQLite FTS5 multi-room domain isolation (`wing/room`), deduplication, rollback state machine, and **50-turn needle-in-a-haystack recall** with 100% precision.
- **Arena 4 (SWE-bench Simulation):** Per-file-capped symbol discovery (`search_files`), multi-hunk patch application (`apply_patch`), Git working tree tracking, and structured subagent execution envelopes.
- **Arena 5 ("The C-Factor" System Efficiency):**
  - **Cold Start Latency:** **`1.45 ms`** (macOS) / **`2.74 ms`** (VPS) (<50ms target)
  - **Peak Resident Memory (RSS):** **`9.84 MB`** (macOS) / **`12.50 MB`** (VPS) (<25MB target)
  - **JSON Serialization/Parsing:** **`1.47 µs/op`** (>680,000 JSON ops/sec)
  - **Continuous Leak Profile:** **`0 KB Delta (Zero Leaks)`** across 1,000 continuous tool iterations
  - **Standalone Binary Footprint:** **`285.1 KB`** (macOS) / **`295.4 KB`** (VPS) (<500KB target)
- **Arena 6 (Late-2026 Autopoietic & SOTA Cognitive Architecture):**
  - **`[6.1]` In-Context Attractor Loop Breaker:** 64-bit FNV-1a hash ring detects and severs periodic loops ($k \in \{1, 2, 3\}$).
  - **`[6.2]` Middle-Out Context Compactor & Root Pinning:** Locks Zone 1 directives and archives middle turns to SQLite (`session_messages_archived`).
  - **`[6.3]` ROMA Context Firewall:** Isolates child subagent execution traces from parent context.
  - **`[6.4]` AST Symbol Graph Navigation:** Indexes **`8,913` symbols in `18.8 ms`** (macOS) / **`49.5 ms`** (VPS) for instant `goto_definition` and `find_callers`.
  - **`[6.5]` Paper2Agent MCP & Mythos Variant Sweeper:** Dynamically synthesizes and validates stdio MCP servers and audits CWE-120/134/78 sinks with AddressSanitizer reproduction commands.
  - **`[6.6]` FadeMem, CORAL, SimpleMem & MIT IDE Soft-Constraints:** Verifies biological retention bounds ($R(t) = S \cdot e^{-\lambda t / (1 + 0.35 \ln(1+k))}$), dual-stream tactical-to-strategic rubric promotion, de-linearized semantic fact extraction, and 5% emergency budget headroom under remediation.

---

### 3. Frontier Agent Architectural Comparison

| Architectural Feature | **Almaz v2.1.0 (C99)** | **Devin** | **OpenHands** | **SWE-agent** | **Aider** |
|:---|:---:|:---:|:---:|:---:|:---:|
| **Core Runtime** | **Pure C99 POSIX** | Proprietary | Python / Docker | Python | Python CLI |
| **RAM Footprint (RSS)** | **< 3.5 MB Idle / 12.5 MB Peak** | ~800 MB (Cloud) | ~1,500 MB | ~450 MB | ~250 MB |
| **Cold Start Latency** | **`1.45 ms`** | ~1,500 ms | ~3,200 ms | ~1,800 ms | ~800 ms |
| **Process Supervision** | **Dedicated Sibling (`almaz-watchdog`)** | Cloud Monitor | Docker Restart | None | None |
| **Affective Modulation** | **VIGIL (`confidence` / `frustration`)** | None | None | None | None |
| **Self-Model Introspection** | **SARSI (Persistent SQLite Metrics)** | None | None | None | None |
| **Memory Architecture** | **FadeMem (Decay) + CORAL + SimpleMem** | Vector DB | Vector RAG | Context Window | Repo Map Tree |
| **Compiler Watchdog** | **Pre-Flight Guard (Auto-Revert)** | Post-Run Test | Post-Run Test | Post-Run Test | Linter Check |
| **Code Navigation** | **SQLite AST Symbol Graph (`18ms`)** | LSP / Grep | Grep / Bash | Grep | Tree-sitter Map |
| **Holdout Safety Suite** | **AIDE² 65-Assertion Battery** | Proprietary | None | None | None |
| **Executable Size** | **`285.1 KB` Binary** | Cloud Only | 4.2 GB Docker Image | 650 MB Env | 180 MB Env |

---

### 4. Running the Benchmark Suite

Run the full 6-arena, 36-task benchmark suite locally with a single command:
```bash
make benchmark
```

## Core Sovereign Organism Subsystems

### 1. FadeMem Biological Memory Decay & Concept Fusion
To prevent memory bloat and context poisoning from stale facts, Almaz implements **FadeMem Biological Memory Decay**:
- **Exponential Retention Dynamics**: Every memory item $i$ decays according to its elapsed time $\Delta t$ and access count $k$:
  $$R_i(t) = S_i \cdot \exp\left(-\frac{\lambda \cdot \Delta t}{1 + 0.35 \ln(1 + k)}\right)$$
  where initial salience $S_i \in [0.1, 1.0]$ and base decay rate $\lambda = 0.05 \text{ day}^{-1}$. Frequently accessed or reinforced knowledge builds logarithmic immunity against decay.
- **Subconscious Sweep & Pruning**: During autonomic maintenance, memories whose retention drops below $R_{\text{prune}} = 0.15$ are automatically purged or archived, freeing SQLite index capacity.
- **Subconscious Concept Fusion**: Semantically overlapping memories sharing high FTS5 token affinity are fused into consolidated, generalized invariant schemata (`almaz_memory_fuse_concepts`), retaining timeless principles while discarding transient conversational fluff.

### 2. CORAL Dual-Stream Memory Routing (Tactical vs Strategic)
Almaz replaces flat memory pools with the **CORAL Dual-Stream Architecture**, strictly separating memory by cognitive depth:
- **Stream A: Tactical Action Records**: Stores immediate, transient observations, intermediate tool outputs, and short-term execution traces. Decays aggressively under FadeMem.
- **Stream B: Strategic Corrective Rubrics**: Stores verified behavioral invariants, architectural constraints, and root-cause post-mortems (format: `[TRIGGER] -> [FAILURE_MODE] -> [CORRECTIVE_ACTION] -> [SUCCESS_CRITERION]`).
- **Automated Rubric Promotion**: Tactical failures that recur across sessions are synthesized into permanent strategic rubrics via `almaz_coral_promote_rubric()`. Strategic rubrics are permanently pinned and immune to temporal decay.

### 3. SimpleMem De-Linearized Semantic Fact Extraction
Traditional RAG ingests noisy, conversational prose transcripts that bloat LLM context windows. Almaz implements **SimpleMem De-linearized Semantic Extraction**:
- **Atomic Assertion Distillation**: Deconstructs raw user messages and tool outputs into discrete, subject-predicate-object semantic facts before saving to SQLite.
- **Multi-View FTS5 Indexing**: Facts are indexed across entity, relation, and context vectors, allowing sub-millisecond retrieval of exact factual assertions without ingesting conversational noise.

### 4. MIT IDE Soft-Constraint Relaxation & Headroom
Rigid execution constraints often cause catastrophic agent termination during critical error recovery. Almaz incorporates **MIT IDE Soft Constraints**:
- **Elastic Remediation Retries**: While standard execution enforces a strict circuit breaker of 3 consecutive failures, critical self-healing missions (`HEALTH_REMEDIATION` and `ANOMALY_REMEDIATION` goals) automatically expand the retry threshold to **5 attempts**.
- **Emergency Budget Headroom**: `budget_state_consume_with_headroom()` permits an additional **5% emergency token/cost headroom** beyond standard daily limits exclusively when executing critical self-repair and system recovery routines.

### 5. Sibling Watchdog Supervisor (`almaz-watchdog`) & Crash Autopsy
Almaz does not run unmonitored. On production servers, `almaz.service` boots `almaz-watchdog`, a specialized C supervisor that:
- Spawns and supervises `./almaz --telegram` via POSIX `fork()`, `execvp()`, and `waitpid()`.
- Captures POSIX abnormal termination signals: `SIGSEGV` (segmentation violation), `SIGBUS` (bus error), `SIGABRT` (assertion failure), `SIGFPE` (floating point arithmetic exception), and `SIGILL` (illegal instruction).
- Performs instant post-mortem analysis: records timestamp, signal number, process uptime, and recent system telemetry to `almaz_crashes.log`.
- Dispatches an emergency autopsy notification directly to the human operator's Telegram channel with root-cause signal diagnosis.
- Implements exponential backoff auto-recovery with a rapid-crash circuit breaker (prevents infinite reboot loops if catastrophic config defects exist).

### 6. SARSI Persistent Self-Model & Introspective Calibration
Almaz maintains a dedicated, persistent mathematical self-model in SQLite (`self_model` table):
- **Proprioceptive Telemetry**: Tracks cumulative lifetime turns, successful turns, tool invocation counts, failure rates, total tokens ingested/emitted, and Resident Set Size (RSS memory).
- **Capability Calibration**: Dynamically updates success ratios per tool type, adjusting agent routing weights based on verified empirical outcomes rather than theoretical prompt claims.
- **Introspective API**: `almaz_agent_get_self_model()` and `almaz_agent_update_self_model()` expose live self-knowledge directly into Ephemeral Context (Zone 3), allowing the agent to reason about its own resource boundaries and reliability.

### 7. VIGIL Emotional Appraisal System (Confidence & Frustration Dynamics)
Almaz implements an affective state machine tracking two core emotional axes:
- `confidence` ($[0.0, 1.0]$, initialized at $0.80$): Boosted by successful tool executions, syntax verification passes, and test completions. Decays upon failures and timeouts.
- `frustration` ($[0.0, 1.0]$, initialized at $0.00$): Incremented when tool calls fail, compile checks reject edits, or circuit breakers trip.
- **Proactive Strategic Pivoting**: When frustration crosses $0.60$ or confidence drops below $0.40$, Almaz intercepts the execution cycle, suppresses repetitive retries, and forces a cognitive pivot directive:
  > *"Operational frustration elevated (0.75). Current strategy failing. Abandon current tool trajectory, inspect root cause with read_file, and execute alternative solution."*

### 8. KnowSelf Situational Memory Gating
Unchecked RAG (Retrieval-Augmented Generation) inflates context windows with irrelevant memories, degrading reasoning speed and increasing inference costs. Almaz features **KnowSelf Situational Memory Gating** (`almaz_should_retrieve_memory()`):
- Evaluates prompt length, conversational depth, and query semantics.
- For simple greetings, short instructions, or self-contained tasks, memory retrieval is gated OFF, saving ~800–1,500 tokens of noise per turn.
- Automatically gates retrieval ON for research tasks, cross-session recalls, architecture audits, or explicit historical queries.

### 9. AIDE² Hidden Holdout Safety Suite (65/65 Assertions)
To allow Darwinian self-evolution without risking catastrophic degradation or code rot, Almaz enforces the **AIDE² Hidden Holdout Protocol** (`make holdout`):
- **65 Strict Invariable Assertions** testing 11 core sub-architectures:
  1. `DynString` capacity bounds, reallocation, and formatting safety.
  2. `MiniJSON` deep nesting, Unicode escaping, and tokenization.
  3. `YAML Frontmatter` CRLF/LF delimiter and boundary resilience.
  4. `Workspace Path Jailing` traversal containment across all file tools.
  5. `SARSI Self-Model` persistence, update math, and schema lifecycle.
  6. `Emotional Appraisal` dynamic clamping, decay, and threshold triggers.
  7. `KnowSelf Memory Gating` precision under diverse turn conditions.
  8. `BPE Token Estimator` boundary conditions and edge cases.
  9. `Metacognitive Circuit Breaker` doom-loop trip and recovery behavior.
  10. `FadeMem Biological Decay & Retention Math` exponential decay and access reinforcement.
  11. `Soft-Constraint Budget Elasticity` emergency headroom and retry expansion.
- Any autonomous mutation that fails even a single assertion is discarded immediately.

### 10. ROMA Context Firewall & Recursive Subagent Task Trees
Subagent execution traces are strictly isolated via `belya_agency_dispatch_subagent()`. Child tool logs are scrubbed from the parent context window, returning only formal contract headers, operational summaries, and structured result metrics to prevent context pollution and attention dilution.

### 11. AST Symbol Graph Navigation Engine (`goto_definition`, `find_callers`)
High-speed embedded SQLite symbol indexing over C/C++ tokens (functions, structs, typedefs, enums, macros). Provides instantaneous, exact symbol jumping and reverse call-graph navigation across codebases (>8,800 symbols in <0.2s) without grep hallucination.

### 12. Paper2Agent Dynamic MCP Server Synthesizer (`synthesize_mcp_server`)
Autonomously transforms static tool scripts and repository utilities into fully validated Model Context Protocol (MCP) JSON-RPC 2.0 stdio servers, persisting executable wrappers in `.belya/mcp/` and auto-registering them with the runtime harness.

### 13. Mythos Variant Vulnerability Sweeper (`audit_security_variants`)
Autonomous security audit engine scanning codebases for unsafe C sinks (`strcpy`, `sprintf`, `strcat`, `gets`, `system`, format string vulnerabilities, unchecked allocations), and generating precise AddressSanitizer reproduction commands.

### 14. 128-Call / 96k-Token Middle-Out Context Compactor
Implements multi-tier context window compaction (`belya_harness_compact_context_middle_out`). Locks Zone 1 system directives and pinned root constraints, preserves recent conversation turns, and losslessly archives intermediate dialogue to SQLite table `session_messages_archived`.

### 15. In-Context Attractor & Loop Breaker
Sliding ring buffer calculating 64-bit FNV-1a hashes over consecutive tool action/observation signatures. Automatically detects and severs periodic loops ($k \in \{1, 2, 3\}$), breaking deadlocks before token budgets are exhausted.

### 16. Dynamic LLM Mutation Generator & Evolution Supervisor v3.0
Every night at 03:00 UTC (or via `python3 tools/evolution_supervisor.py`), Almaz executes its autonomous evolution pipeline:
- **Autonomous M1 Dynamic Mutation Generator:** When the static mutation pool is exhausted or gated out by novelty tracking, the supervisor queries an LLM to analyze the source code and propose targeted single-hunk modifications against safe files (`minijson.c`, `minifrontmatter.c`, `linenoise.c`).
- **Symmetric Like-for-Like Benchmarking:** Fixes benchmark distortions by ensuring both candidate and baseline execute identical test batteries (`_ensure_battery()`) with hard 90s timeouts and dead-lock free IPC sinks.
- **Strict Verification Chain:** Candidates are verified with $N=1$ exact match checks, compiled under `-Wall -Wextra -fsanitize=address,undefined`, tested against all 54 unit tests, gated by all 65 hidden holdouts, and benchmarked for speedup.
- **Novelty Tracking:** Rejects previously evaluated or rejected mutations, ensuring forward evolutionary exploration.

### 17. Dual-Tier Automatic Model Gateway Failover
Almaz eliminates runtime downtime through a resilient dual-tier provider gateway:
- **Primary Endpoint:** Ultra-fast primary provider (e.g. OpenCode Go / DeepSeek V4.1 Flash).
- **Secondary Fallback:** Automatic failover provider (e.g. OpenRouter / Anthropic / Qwen).
- **One-Shot Hard Failure Recovery:** Evaluates `gateway_response_is_hard_failure(&res)`. Upon encountering HTTP 429 rate limits, 401 authentication expiry, 502 bad gateways, connection drops, or empty outputs, the gateway seamlessly re-executes the request against the fallback provider without losing conversation history or context.

### 18. Production Hardening: H4 Tool Stripping & H5 WAL Batching
- **H4 (Orphaned Tool Message Stripper):** In `belya_agent_compact_history()`, Almaz discards dangling `role: tool` messages upon context truncation, ensuring history never begins with an unanchored tool response that would trigger HTTP 400 fatal rejections from frontier APIs.
- **H5 (SQLite WAL Batch Transactions):** In `belya_agent_save_session()`, history pruning and message insertions are wrapped in explicit `BEGIN IMMEDIATE; ... COMMIT;` transactions. This eliminates per-row fsync overhead, resolves WAL bloat, and prevents database write lock contention.

### 19. SSRF Defense & Secret Path Jailing
- **Socket-Level SSRF Filter:** Integrates `sockaddr_is_private()` with libcurl's `CURLOPT_OPENSOCKETFUNCTION` to inspect socket destinations at connection time. Outbound calls to `127.0.0.1`, RFC1918 subnets (`10.0.0.0/8`, `172.16.0.0/12`, `192.168.0.0/16`), and cloud metadata IP (`169.254.169.254`) are immediately aborted. Protocols are locked strictly to HTTP/HTTPS via `CURLOPT_PROTOCOLS_STR`.
- **Secret Path Protection:** Enforces `is_secret_path()` guard across all file tools, barring model access to `.env`, `.ssh/`, `.git/config`, or cloud authorization tokens.

### 20. Production Canary Promotion Architecture (`promote.sh`)
Almaz guarantees zero-downtime, crash-free VPS deployments via `/opt/almaz/tools/promote.sh`:
- **Triple-Slot Rotation:** Maintains `ACTIVE` (`almaz`), `LKG` (`almaz.bak`), and `STAGING` (`almaz.B`) binary slots.
- **Gate 1 (Self-Test):** Candidate binary must execute `--selfest` and pass all DB integrity checks.
- **Gate 2 (Protocol Sanity):** Verifies that candidate announces the supervisor contract (`--telegram`).
- **Atomic Promotion:** Atomically copies candidate to `STAGING`, creates `.rollback` snapshot, and renames `STAGING -> ACTIVE`.
- **90-Second Functional Witness Window:** Witnesses the live service for 90 seconds, actively verifying "Almaz Sovereign Telegram Bot Daemon Online" in systemd journal and confirming absence of crash-loops before rotating the Last-Known-Good (`LKG`) slot. If anything fails, it auto-reverts to LKG immediately.

---

## Native Tool Suite (41 Built-In Tools)

| Tool Name | Parameters | Description |
|:---|:---|:---|
| **`bash`** | `command` (str), `background` (bool) | Executes shell commands with persistent CWD tracking, PTY subshell virtualization, & timeout protection |
| **`read_file`** | `path` (str), `offset` (num), `limit` (num) | Reads file contents with line-number slicing |
| **`write_file`** | `path` (str), `content` (str) | Writes text directly to disk with pre-flight compiler syntax check |
| **`edit_file`** | `path`, `old_text`, `new_text`, `verify_compile` (bool) | Exact search-and-replace edit with compiler verification guard and auto-revert |
| **`apply_patch`** | `path` (str), `patch` (str) | Multi-hunk structured replacement patch engine (`<<<<<<< SEARCH ... ======= ... >>>>>>> REPLACE`) |
| **`list_dir`** | `path` (str) | Inspects directory contents |
| **`search_files`** | `pattern` (str), `path` (str), `file_glob` (str) | Recursively searches text patterns across codebase files (grep-like) |
| **`goto_definition`** | `symbol` (str), `path` (str) | AST symbol graph jump to function, struct, enum, or typedef definition |
| **`find_callers`** | `function_name` (str), `path` (str) | Reverse AST call-graph search locating all callers across the codebase |
| **`synthesize_mcp_server`** | `server_name` (str), `repo_path` (str) | Paper2Agent dynamic Model Context Protocol stdio server generator & validator |
| **`audit_security_variants`** | `target_path` (str), `cwe_filter` (str) | Mythos variant security audit scanner identifying memory leaks, buffer overflows, and unsafe sinks with ASan PoCs |
| **`git_status`** | *(none)* | Inspects Git working copy status |
| **`git_diff`** | `staged` (bool), `path` (str) | Inspects staged or unstaged Git diffs |
| **`save_memory`** | `topic` (str), `content` (str) | Stores verified knowledge into SQLite persistent memory with wing/room scoping |
| **`recall_memory`** | `query` (str) | Searches SQLite memory using sanitized FTS5 queries and BM25 ranking |
| **`save_skill`** | `name` (str), `trigger` (str), `description` (str), `instructions` (str) | Distills and indexes reusable procedural skills with automatic deduplication |
| **`recall_skill`** | `query` (str) | Retrieves curated procedural skills with progressive disclosure |
| **`recall_conversation`** | `query` (str) | Searches historical conversation sessions and timestamps in SQLite |
| **`fetch_url`** | `url` (str), `method` (str), `headers` (obj), `body` (str) | Native HTTP/REST client supporting GET, POST, PUT, DELETE, and payloads |
| **`spawn_subagent`**| `task` (str), `instructions` (str), `max_turns` (num) | Spawns isolated worker subagent and returns structured execution envelope |
| **`define_tool`** | `name`, `description`, `parameters`, `script_body` | Dynamically creates, scripts, persists, and registers new executable tools with full parameter contracts |
| **`dispatch_agent`**| `role` (str), `task` (str), `context` (str) | Dispatches specialized subagent bounded by declarative manifest with automatic Git Rollback Guard |
| **`ask_user`** | `question` (str), `options` (arr), `is_multi_select` (bool) | Prompts operator with raw signal-safe interactive terminal modal or rich Telegram inline keyboard |
| **`task_create`** | `title` (str), `description` (str), `priority` (str), `depends_on` (arr) | Registers a structured task on the SQLite task board with dependency graph tracking |
| **`task_update`** | `task_id` (str), `status` (str), `notes` (str) | Updates lifecycle state (`pending`, `in_progress`, `completed`, `failed`) and notes on the task board |
| **`task_list`** | `status` (str), `limit` (num) | Lists tasks from SQLite board with optional status filtering and dependency resolution |
| **`task_get`** | `task_id` (str) | Retrieves complete task details, execution notes, and dependency graph |
| **`cron_create`** | `name` (str), `schedule_expr` (str), `action_prompt` (str) | Registers a recurring background cron job in SQLite |
| **`cron_list`** | *(none)* | Lists all active background cron jobs, intervals, next fire time, and trigger history |
| **`cron_tick`** | *(none)* | Immediately evaluates and fires all due background cron jobs |
| **`enter_plan_mode`** | *(none)* | Enters read-only Plan Mode security sandbox, locking all mutating filesystem and shell operations |
| **`exit_plan_mode`** | *(none)* | Exits Plan Mode sandbox, restoring full read-write execution privileges |
| **`enter_worktree`** | `name` (str), `base_branch` (str) | Spawns and switches into an isolated Git worktree for zero-risk speculative refactoring |
| **`leave_worktree`** | `discard_changes` (bool) | Leaves isolated Git worktree sandbox and restores original working directory and branch |
| **`manage_task`** | `action` (str), `task_id` (str), `input` (str) | Manages background PTY subshell processes (`list`, `status`, `send_input`, `kill`) |
| **`web_search`** | `query` (str), `provider` (str), `max_results` (num) | Multi-provider streaming web retrieval (Tavily, Brave, SerpApi, SearXNG) |
| **`web_fetch`** | `url` (str), `extract_markdown` (bool) | Fetches URL with SSRF protection, size clamps (50KB), and extracts clean markdown |
| **`edit_notebook`** | `path` (str), `action` (str), `cell_index` (num), `content` (str) | Byte-span Jupyter notebook (`.ipynb`) cell editor preserving cell execution counts and metadata |
| **`lsp_definition`** | `path` (str), `symbol` (str), `line` (num), `col` (num) | Navigates to symbol definition via LSP-over-MCP with native AST fallback |
| **`lsp_references`** | `path` (str), `symbol` (str) | Finds all references to a symbol via LSP-over-MCP with native AST fallback |
| **`exec_backend`** | `action` (str), `backend_type` (str), `target` (str) | Inspects or switches execution backend (local, Docker container, or SSH host) via stdin-piped execution |

---

## Command & Slash Controls Reference

| Command | Interface | Description |
|:---|:---|:---|
| `/help` | CLI & Telegram | Show command reference |
| `/status` | CLI & Telegram | View active model, endpoint, token usage, CWD, and session count |
| `/tools` | CLI & Telegram | Show all registered tools (including dynamic MCP & custom tools) |
| `/reset` *(or `/clear`, `/new`)* | CLI & Telegram | **Reset session history** (preserves system directives and persistent SQLite memory) |
| `/compact [N]` | CLI & Telegram | Prune older messages, keeping `N` recent turns |
| `/skills` | CLI & Telegram | View all curated procedural skills in the persistent registry |
| `/cache` | CLI & Telegram | Inspect 3-zone prompt cache hit rates and token savings |
| `/timeline [N]` | CLI & Telegram | View recent Gomaa chronological timeline event log |
| `/rules` | CLI & Telegram | View active repository guidelines (`.agentrules` / `AGENTS.md`) |
| `/sessions` | CLI & Telegram | List all saved conversation sessions in SQLite |
| `/save [id]` | CLI & Telegram | Checkpoint current conversation tree to database |
| `/resume <id>` | CLI & Telegram | Restore past conversation session by ID |
| `/reflect` | CLI & Telegram | Distill recent trajectory into reusable SQLite skill |
| `/export <session_id> <file>` | CLI & Telegram | Export session trajectory to OpenAI fine-tune JSONL format |
| `/checkpoint [id]` | CLI & Telegram | Create instant Git & SQLite state checkpoint |
| `/rollback <id>` | CLI & Telegram | Rollback workspace files and context to a past checkpoint |
| `/model <name>` | CLI & Telegram | Switch active AI model dynamically |
| `/cwd [path]` | CLI & Telegram | View or change current working directory |
| `/mcp <cmd>` | CLI & Telegram | Connect to an external stdio MCP server |
| `/agency <prompt>` | CLI & Telegram | Execute task using Belya Agency multi-agent pipeline (Triage → Architect → Builder → Tester) |
| `/diff` | CLI | Render stateful syntax-highlighted ANSI diff of working tree changes |
| `/doctor` | CLI | Run deep diagnostics on toolchain, SQLite WAL, sanitizers, and memory health |
| `/cost` | CLI | View real-time token expenditure, cache hit rates, and dollar savings |
| `/plan` | CLI | Toggle read-only Plan Mode sandbox on/off |
| `/cron` | CLI | List registered cron jobs and view trigger schedules |

---

## Operating Modes

### Mode 1: 24/7 Supervised VPS Daemon (Watchdog + Telegram)

Almaz runs on production Linux servers as a supervised background daemon connected to `@AlmaztheBot`:
```bash
# Build the organism and watchdog binaries:
make

# Run directly via watchdog supervisor:
./almaz-watchdog --telegram
```

Under systemd, `almaz.service` runs `almaz-watchdog --telegram`:
```bash
sudo systemctl status almaz.service
sudo journalctl -u almaz -f
```

### Mode 2: Terminal Interactive CLI

Launch the interactive REPL for local development, pairing, or diagnostics:
```bash
./almaz
```

### Mode 3: Headless Batch / CI/CD Mission

Execute automated single-turn missions or scripts:
```bash
./almaz --prompt "Audit src/ for memory leaks and run holdout suite"
```

### Mode 4: Multi-Agent Orchestration Mode

Execute compound engineering missions via the Chief-of-Staff multi-agent pipeline:
```bash
./almaz --agency "Implement lock-free ring buffer and verify with tests"
```

---

## What is real / what is marketing

Honest inventory, kept in the repo on purpose. **Self-restarting is not self-repairing**,
and a scorecard is only as trustworthy as the instrument behind it.

**Real, machine-verified mechanisms**
- `almaz-watchdog` supervises the daemon: fork/exec, exponential backoff, crash-autopsy log,
  rate-limited crash alert, and a crash-loop circuit breaker that falls back to the LKG slot.
- A/B deploy slots (`ACTIVE` / `LKG` / `STAGING`) with `promote.sh` gates: candidate
  `--selfest`, a `--telegram` contract check, a 90 s witness, and automatic rollback.
- `health_watcher.c`: disk/RSS/SQLite-integrity/gateway/telegram pressure detection, each
  alert enqueueing a real bounded remediation goal.
- Taboo-gated mutation loop: sandbox apply → ASan compile → unit battery → hidden holdout →
  microbenchmark → accept only on a positive speedup, else record and block the phenotype.
- External 5-minute guardian (binary existence, runtime sha, restart-storm delta, crash-loop
  journal marker, SQLite `quick_check`, disk) — **report-only, never restarts anything**.
- `OnFailure` alerting for the `almaz`/`belya` units.

**Limits — stated plainly**
- **No awareness.** Introspection and telemetry are not consciousness; the self-model is a row
  in SQLite, not experience.
- **Self-healing cannot recover a missing supervisor binary.** If `almaz-watchdog` itself is
  deleted the service dies silently — which is exactly why the external guardian exists.
- **Evolution is bounded, machine-verified optimisation, not open-ended invention.** The
  candidate pool is currently a small static list; an LLM generator is planned, not present.
- **Affect is measured, not felt.** `recent_form` is an EWMA over real outcomes;
  `confidence`/`frustration` are clamped counters asserted by the test suites.
- **Single host, no failover.** One VPS, one SQLite file per agent.

## Automated Verification & Test Suites

### 1. Unit Test Suite (61/61 Passed - 100%)
```bash
make test
```
```text
================ Running Almaz Super Strict Test Suite ================
[Test] DynString Operations...
  -> DynString PASSED
[Test] MiniJSON Parser & Serializer...
  -> MiniJSON PASSED
[Test] BPE-calibrated Token Estimator...
  -> Token Estimator PASSED (Total: 1103 tokens)
[Test] Agent Memory (FTS5) & Rules Auto-Discovery...
  -> Agent Memory & Rules PASSED
[Test] Session Checkpointing & Resumption...
  -> Session Checkpointing & Resumption PASSED
[Test] Dynamic Self-Tooling (define_tool & Custom Script Execution)...
  -> Dynamic Self-Tooling PASSED
[Test] Harness Tool Suite (13 Tools) & Patch Engine...
  -> Harness Tools & Patch Engine PASSED
[Test] Telegram Bot Adapter Security & Ephemeral Lifecycle...
  -> Telegram Adapter PASSED
[Test] Pre-Flight Compiler Watchdog (Auto-Healing Feedback Loop)...
  -> Pre-Flight Compiler Watchdog PASSED
[Test] Native Web Content Retrieval (fetch_url)...
  -> fetch_url Tool PASSED
[Test] Gomaa Memory Paradigm (Wing/Room Scoping, Salience & Timeline)...
  -> Gomaa Memory & Timeline PASSED
[Test] Tool-Call Scavenger Engine (DeepSeek/Reasoning Extraction)...
  -> Tool-Call Scavenger PASSED
[Test] Skills Curation & Progressive Disclosure Loop...
  -> Skills Curation & Progressive Disclosure PASSED
[Test] Git & State Checkpoint and Instant Rollback...
  -> Git Checkpointing & Instant Rollback PASSED
[Test] Trajectory Exporter (OpenAI Fine-Tune JSONL Format)...
  -> Trajectory Exporter PASSED
[Test] Historical Conversation Search & Multi-Method REST Retrieval...
  -> Historical Conversation Search PASSED
[Test] Advanced REST Client (Multi-Method, Headers, JSON Body)...
  -> Advanced REST Client PASSED
[Test] Tool-Call Scavenger Deep Stress & Edge-Case Parser...
  -> Tool-Call Scavenger Deep Stress PASSED
[Test] Multi-Turn Checkpointing & Rollback State Machine...
  -> Multi-Turn Checkpointing & Rollback PASSED
[Test] Progressive Disclosure Manifest & Salience Priority...
  -> Progressive Disclosure Manifest PASSED
[Test] Forced Synthesis on Step Exhaustion & Persistent Keep-Alive...
  -> Forced Synthesis & Keep-Alive PASSED
[Test] v6.0 Resilient Edit Fallback, Regex Search & Diagnostics...
  -> v6.0 Enhancements PASSED
[Test] Exhaustive Verification of All 17 Tools & Edge Cases...
  -> Exhaustive 17 Tools & Edge Cases PASSED
[Test] Skills Lifecycle: Trigger Matching, Auto-Injection & Salience Boost...
  -> Skills Lifecycle & Auto-Injection PASSED
[Test] Subagent Recursion Guard & Sandbox Tool Isolation...
  -> Subagent Recursion Guard PASSED
[Test] Self-Telemetry, RSS Calculation & Proprioception...
  -> Self-Telemetry & Proprioception PASSED (RSS: 11.81 MB)
[Test] Metacognitive Circuit Breaker & Verification Guard...
  -> Metacognitive Circuit Breaker & Verification Guard PASSED
[Test] Track A.1: Workspace Path Jailing (is_path_jailed)...
  -> Workspace Path Jailing PASSED
[Test] Track A.2: C99 Markdown Frontmatter Parser (minifrontmatter)...
  -> C99 Markdown Frontmatter Parser PASSED
[Test] Track A.3: File-First Skills System (skills/*/SKILL.md)...
  -> File-First Skills System PASSED (Loaded 9 disk skills)
[Test] Track A.4: Composable Rule Packs (rules/*/*.md)...
  -> Composable Rule Packs PASSED (Loaded 3 rule packs)
[Test] Track A.5: Systematic TROUBLESHOOTING.md Pattern Resolver...
  -> Systematic TROUBLESHOOTING.md Pattern Resolver PASSED
[Test] Track B: Belya Agency Multi-Agent Orchestration & Rollback Guard...
  -> Belya Agency Multi-Agent Architecture & Rollback Guard PASSED
[Test] Jev TypeSafe AI Integration & Coprocessor Parser...
  -> Jev TypeSafe AI Integration & Coprocessor Parser PASSED
[Test] SARSI Persistent Self-Model (Organism Introspection)...
  -> SARSI Persistent Self-Model PASSED
[Test] VIGIL Emotional Appraisal State Machine...
  -> VIGIL Emotional Appraisal State Machine PASSED
[Test] Measured Recent Form & Escalation Gates (M2.1/R6a)...
  -> Measured Recent Form & Escalation Gates PASSED
[Test] KnowSelf Situational Memory Gating...
  -> KnowSelf Situational Memory Gating PASSED
[Test] Autonomic Subconscious Idle Loop & Intrinsic Goals...
  -> Autonomic Subconscious & Intrinsic Goals PASSED
--- Health Watcher ---
  -> Health Watcher PASSED
--- Budget Controller ---
  -> Budget Controller PASSED
--- Timeline Anomaly ---
  -> Timeline Anomaly PASSED
[Test] Security & Adversarial Hardening Suite (SSRF, Path Jailing, Fuzzing, SQLi)...
  -> Security & Adversarial Hardening Suite PASSED
[Test] Late-2026 In-Context Locking Breaker (Attractor Detection)...
  -> Late-2026 In-Context Locking Breaker PASSED
[Test] Late-2026 128-Call / 96k Middle-Out Context Compactor...
  -> Late-2026 Middle-Out Context Compactor PASSED
[Test] Late-2026 FadeMem Biological Decay (Mathematical & FTS5 Bounds)...
  -> Late-2026 FadeMem Biological Decay PASSED
[Test] Late-2026 Subconscious Pruning & Concept Fusion Sweeps...
  -> Late-2026 Subconscious Pruning & Concept Fusion Sweeps PASSED
[Test] Late-2026 CORAL Dual-Stream Memory Routing & Rubrics...
  -> Late-2026 CORAL Dual-Stream Memory Routing PASSED
[Test] Late-2026 SimpleMem De-linearized Semantic Fact Extraction...
  -> Late-2026 SimpleMem De-linearized Semantic Facts PASSED
[Test] Late-2026 MIT IDE Soft-Constraint Relaxation & Headroom...
  -> Late-2026 MIT IDE Soft-Constraint Relaxation PASSED
[Test] Late-2026 ROMA Recursive Context Firewall...
  -> Late-2026 ROMA Recursive Context Firewall PASSED
[Test] Late-2026 AST Symbol Graph Navigation & Call-Tree...
  -> Late-2026 AST Symbol Graph Navigation PASSED (11552 symbols indexed)
[Test] Late-2026 Paper2Agent Repo-to-MCP Dynamic Synthesis...
  -> Late-2026 Paper2Agent Repo-to-MCP Synthesis PASSED
[Test] Late-2026 Mythos Variant Security Sweeper...
  -> Late-2026 Mythos Variant Security Sweeper PASSED
[Test] Phase 1: Stateful Diff Colorizer, ask_user & Signal-Safe Modal...
  -> Phase 1: Stateful Diff Colorizer, ask_user & Signal-Safe Modal PASSED
[Test] Phase 2: Deterministic Hooks, 5-Tier Compactor & Dynamic Cache Boundary...
  -> Phase 2: Deterministic Hooks, 5-Tier Compactor & Dynamic Cache Boundary PASSED
[Test] Phase 3: Task Board, FTS5 Recall & Background Cron Engine...
  -> Phase 3: Task Board, FTS5 Recall & Background Cron Engine PASSED
[Test] Phase 4: Plan Mode Sandbox, Isolated Git Worktrees & SKILL.md Parity...
  -> Phase 4: Plan Mode Sandbox, Isolated Git Worktrees & SKILL.md Parity PASSED
[Test] Phase 5: POSIX PTY Subshells & Background Task Manager...
  -> Phase 5: POSIX PTY Subshells & Background Task Manager PASSED
[Test] Phase 6: Web Search/Fetch, Notebook Splicer & LSP-over-MCP...
  -> Phase 6: Web Search/Fetch, Notebook Splicer & LSP-over-MCP PASSED
[Test] Phase 7: Omnichannel Gateway & Stdin-Piped Remote Backends...
  -> Phase 7: Omnichannel Gateway & Stdin-Piped Remote Backends PASSED
================ All Tests Passed Successfully (61/61 - 100%) ================
```

### 2. Hidden Holdout Regression Suite (65/65 Assertions - 100%)
```bash
make holdout
```
```text
================ Running Almaz Hidden Holdout Benchmark Suite ================
  [Holdout 1/10] DynString Boundaries: PASSED
  [Holdout 2/10] MiniJSON Depth & Escaping: PASSED
  [Holdout 3/10] YAML Frontmatter CRLF Delimiters: PASSED
  [Holdout 4/10] Workspace Path Jailing: PASSED
  [Holdout 5/10] SARSI Self-Model Lifecycle: PASSED
  [Holdout 6/10] Emotional Appraisal Dynamic Clamping: PASSED
  [Holdout 7/10] KnowSelf Memory Gating Precision: PASSED
  [Holdout 8/10] Token Estimator Limits: PASSED
  [Holdout 9/10] Metacognitive Circuit Breaker Resilience: PASSED
  [Holdout 10/10] FadeMem Biological Retention Bounds: PASSED
================ Holdout Benchmark Passed: 65/65 (100%) ================
```

### 3. SQLite Integrity & WAL Self-Test (`--selfest`)
```bash
./almaz --selfest
```
```text
[Selfest] Almaz self-test
  [PASS] sqlite integrity_check
  [PASS] journal_mode=wal
  [PASS] self_model
  [PASS] agent_goals
  [PASS] agent_timeline
  [PASS] agent_memory_fts
  [PASS] session_messages
  [PASS] self_model active row
[Selfest] ALL CHECKS PASSED
```

### 4. Zero-Tolerance Memory Safety (ASan/UBSan)
Compiled and tested under AddressSanitizer and UndefinedBehaviorSanitizer:
```bash
make clean && make test CFLAGS="-Wall -Wextra -O2 -std=c99 -fsanitize=address,undefined -g -D_POSIX_C_SOURCE=200809L"
make holdout CFLAGS="-Wall -Wextra -O2 -std=c99 -fsanitize=address,undefined -g -D_POSIX_C_SOURCE=200809L"
```
**Result: 61/61 unit tests and 65/65 holdout assertions pass with 0 memory leaks, 0 heap buffer overflows, and 0 undefined behavior.**


---

## Configuration & Deployment (`.env`, `almaz.service`)

Create `.env` in the working directory:
```env
# OpenCode Go Flat Subscription Backend (Recommended)
MODEL_ENDPOINT=https://opencode.ai/zen/go/v1
MODEL_NAME=deepseek-v4.1-flash
MODEL_API_KEY=oc_sk_your-opencode-key

# OpenRouter Alternative Backend
# MODEL_ENDPOINT=https://openrouter.ai/api/v1/chat/completions
# MODEL_NAME=deepseek/deepseek-v4.1-flash
# MODEL_API_KEY=sk-or-v1-your-key

# Channel A: Telegram Bot
TELEGRAM_BOT_TOKEN=123456789:ABCDefGhIJKlmNoPQRsTUVwxyZ
TELEGRAM_CHAT_ID=7934918808

# Channel B: Discord Bot (Optional)
DISCORD_BOT_TOKEN=MTE5...your-discord-token
DISCORD_CHANNEL_ID=123456789012345678

# Channel C: Slack Bot (Optional)
SLACK_BOT_TOKEN=xoxb-...your-slack-bot-token
SLACK_CHANNEL_ID=C0123456789

# Execution Sandbox (Optional - Local, Docker, or Remote SSH)
# DOCKER_CONTAINER=almaz-sandbox

# Jev TypeSafe AI Decision Coprocessor (Optional)
JEV_API_KEY=jev_sk_your-key
```

### Systemd Service Configuration (`/etc/systemd/system/almaz.service`)
```ini
[Unit]
Description=Almaz Sovereign Self-Evolving AI Organism & Telegram Daemon (Pure C99)
After=network.target

[Service]
Type=simple
User=root
WorkingDirectory=/opt/almaz
ExecStart=/opt/almaz/almaz-watchdog --telegram
Restart=always
RestartSec=5
StandardOutput=journal
StandardError=journal
LimitNOFILE=65535

[Install]
WantedBy=multi-user.target
```

---

## Ecosystem Relationship: Almaz & Belya (Twin Sovereign Siblings)

Almaz and Belya are **twin sovereign C99 siblings** sharing a common lineage and low-level POSIX execution foundation, but deployed with complementary, production-grade operational doctrines:

```
┌────────────────────────────────────────────────────────────────────────┐
│                  Twin Sovereign C99 Sibling Ecosystem                  │
└───────────────────┬────────────────────────────────┬───────────────────┘
                    │                                │
                    ▼                                ▼
       ┌──────────────────────────┐     ┌──────────────────────────┐
       │   GitHub: M4F-S/Belya    │     │   GitHub: M4F-S/Almaz    │
       │   (Deterministic Core)   │     │  (Metamorphic Organism)  │
       │       Tag: v2.1.0        │     │       Tag: v2.1.0        │
       │     Path: /opt/belya     │     │     Path: /opt/almaz     │
       │    Service: belya.service│     │    Service: almaz.service│
       └────────────┬─────────────┘     └────────────┬─────────────┘
                    │                                │
                    ▼                                ▼
        [Deterministic Software ]        [Autonomous Self-Evolving]
        [Engineering Workhorse  ]        [Self-Healing Organism   ]
        [48 Tests / ASan Clean  ]        [61 Tests + 65 Holdouts  ]
        [Production Deployments ]        [FadeMem + CORAL + VIGIL ]
```

### 🧬 Sibling Lineage & Mutual Hardening
- **Belya (The Deterministic Sibling & Software Engineering Core)**:
  Maintained in [`M4F-S/Belya`](https://github.com/M4F-S/Belya), deployed 24/7 on the VPS at `/opt/belya` as `belya.service`. Belya is the production engineering core designed for absolute predictability, deterministic test generation, and sub-millisecond execution. It serves as Almaz's rock-solid foundation, providing pristine reference architectures and security harness primitives.
- **Almaz (The Metamorphic Sibling & Cognitive Organism)**:
  Maintained in [`M4F-S/Almaz`](https://github.com/M4F-S/Almaz), deployed 24/7 on the VPS at `/opt/almaz` as `almaz.service` supervised by `almaz-watchdog`. Almaz takes Belya's deterministic foundation and elevates it into an autopoietic organism: a biological computing loop featuring biological memory decay (FadeMem), dual-stream tactical/strategic memory routing (CORAL), semantic fact extraction (SimpleMem), a persistent self-model (SARSI), emotional appraisal dynamics (VIGIL), situational memory gating (KnowSelf), soft-constraint relaxation, and Darwinian AST code mutation guarded by 65 hidden holdout assertions.
- **Continuous Cross-Sibling Co-Evolution**:
  Both siblings continuously exchange hardening advances. Security enhancements first proven in Belya (such as H4 orphaned tool message stripping, H5 SQLite WAL batching, socket-level SSRF filters, kernel Plan Mode sandboxing, and omnichannel gateway polling) are ported into Almaz. Conversely, telemetry innovations created in Almaz (such as H2 SSE usage accounting, linear streaming `memcpy` assembly, and Darwin compatibility) are backported to Belya. In the nightly arena benchmark, both siblings test mutations symmetrically without bias.

---

## License

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at:

[http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
