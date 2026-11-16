# Almaz — Sovereign Self-Evolving, Self-Healing Autonomous AI Organism & SOTA Dominance Harness

[![Release](https://img.shields.io/badge/Release-v7.2.0-blue.svg)](https://github.com/M4F-S/Almaz/releases/tag/v7.2.0)
[![License](https://img.shields.io/badge/License-Apache_2.0-green.svg)](LICENSE)
[![Language](https://img.shields.io/badge/Language-C99_POSIX-orange.svg)]()
[![Tests](https://img.shields.io/badge/Unit_Tests-66%2F66_Passed_(100%25)-brightgreen.svg)]()
[![Holdouts](https://img.shields.io/badge/Holdout_Battery-10%2F10_Passed_(100%25)-brightgreen.svg)]()
[![Operation_Crucible](https://img.shields.io/badge/Operation_Crucible-100%25_Invariants_Intact-success.svg)]()
[![Memory_Safety](https://img.shields.io/badge/Memory_Safety-0_Leaks_ASan_UBSan_Clean-success.svg)]()
[![Supervisor](https://img.shields.io/badge/Supervisor-almaz--watchdog_Active-success.svg)]()
[![Tamper_Proof](https://img.shields.io/badge/Tamper_Lock-chattr_+i_Immutable-red.svg)]()
[![RFC_8615](https://img.shields.io/badge/Skills_Discovery-RFC_8615_Compliant-success.svg)]()
[![Memory](https://img.shields.io/badge/Memory_Footprint-<8.5MB_Peak_RSS-purple.svg)]()
[![Cold_Start](https://img.shields.io/badge/Cold_Start-<1.5ms-yellow.svg)]()

> **Almaz** is a bare-metal, autopoietic autonomous AI organism and execution harness implemented in pure C99. Functioning as the cognitive twin of [Belya](https://github.com/M4F-S/Belya), Almaz bridges deterministic systems engineering and adaptive computational cognition: a dual-process self-healing supervisor (`almaz-watchdog`), biological memory decay (**FadeMem**), dual-stream tactical and strategic memory routing (**CORAL**), de-linearized semantic fact extraction (**SimpleMem**), an introspective self-model (**SARSI**), an emotional appraisal state machine (**VIGIL**), an interactive ANSI terminal TUI, native stdio LSP integration, speculative shadow worktree exploration, direct inter-agent mesh socket IPC, RFC 8615 skills discovery, trajectory distillation (ShareGPT & Hermes DPO), and kernel-level immutable tamper resistance (`chattr +i`).

---

## Table of Contents
- [Executive Overview](#executive-overview)
- [Comparative Benchmark Matrix](#comparative-benchmark-matrix)
- [Architectural Topology: Dual-Process Symbiosis](#architectural-topology-dual-process-symbiosis)
- [Core Cognitive Systems & Organs](#core-cognitive-systems--organs)
  - [1. SARSI Persistent Self-Model & Introspective Calibration](#1-sarsi-persistent-self-model--introspective-calibration)
  - [2. VIGIL Emotional Appraisal Engine](#2-vigil-emotional-appraisal-engine)
  - [3. FadeMem Biological Memory Decay & Concept Fusion](#3-fademem-biological-memory-decay--concept-fusion)
  - [4. CORAL Dual-Stream Memory Routing](#4-coral-dual-stream-memory-routing)
  - [5. SimpleMem De-Linearized Semantic Fact Extraction](#5-simplemem-de-linearized-semantic-fact-extraction)
  - [6. KnowSelf Situational Memory Gating](#6-knowself-situational-memory-gating)
  - [7. Autonomic Subconscious Idle Loop & Intrinsic Goals](#7-autonomic-subconscious-idle-loop--intrinsic-goals)
  - [8. ROMA Recursive Context Firewall](#8-roma-recursive-context-firewall)
- [SOTA Dominance Architecture](#sota-dominance-architecture)
  - [Pillar 1: Interactive Terminal TUI & Split Diff Inspector](#pillar-1-interactive-terminal-tui--split-diff-inspector)
  - [Pillar 2: Native Stdio LSP JSON-RPC Integration](#pillar-2-native-stdio-lsp-json-rpc-integration)
  - [Pillar 3: Speculative Multi-Worktree Candidate Dispatcher](#pillar-3-speculative-multi-worktree-candidate-dispatcher)
  - [Pillar 4: Direct Inter-Agent Mesh Socket IPC (`/tmp/belya_almaz_mesh.sock`)](#pillar-4-direct-inter-agent-mesh-socket-ipc-tmpbelya_almaz_meshsock)
- [Skills Discovery, Trajectory Distillation & Rules Ingestion](#skills-discovery-trajectory-distillation--rules-ingestion)
  - [1. RFC 8615 Agent Skills Discovery (`--export-skills`)](#1-rfc-8615-agent-skills-discovery---export-skills)
  - [2. Multi-Format Trajectory Distillation (ShareGPT & Hermes DPO)](#2-multi-format-trajectory-distillation-sharegpt--hermes-dpo)
  - [3. Universal Rules Auto-Ingestion](#3-universal-rules-auto-ingestion)
  - [4. Closed-Loop Skill Auto-Synthesis](#4-closed-loop-skill-auto-synthesis)
- [Verification Batteries & Quality Scorecard](#verification-batteries--quality-scorecard)
- [Native Tool Suite (46 Built-In Tools)](#native-tool-suite-46-built-in-tools)
- [Installation & Quick Start](#installation--quick-start)
  - [Prerequisites](#prerequisites)
  - [Building from Source](#building-from-source)
  - [Running the Strict Test Batteries](#running-the-strict-test-batteries)
- [Operating Modes](#operating-modes)
  - [Mode 1: Interactive Terminal TUI](#mode-1-interactive-terminal-tui)
  - [Mode 2: 24/7 VPS Telegram & Omnichannel Daemon](#mode-2-247-vps-telegram--omnichannel-daemon)
  - [Mode 3: Supervised Self-Healing Watchdog](#mode-3-supervised-self-healing-watchdog)
- [Production Deployment & Defense-in-Depth Administration](#production-deployment--defense-in-depth-administration)
- [Relationship with Twin Sibling Belya](#relationship-with-twin-sibling-belya)
- [License & Provenance](#license--provenance)

---

## Executive Overview

Conventional AI agent frameworks suffer from fundamental architectural bottlenecks:
1. **Runtime Overhead:** Heavy language engines (Node.js, Python) demand 350 MB to 1.8 GB RSS, incur garbage collection pauses, and introduce deep dependency vulnerability surfaces.
2. **Crash Fragility:** If an unhandled exception or heap corruption occurs, the process terminates and in-flight session context is irrevocably lost.
3. **Agent Amnesia:** Context windows are finite; naive FIFO truncation discards architectural decisions, while indiscriminate stuffing degrades retrieval needle-in-a-haystack performance.
4. **Static Tool Execution:** Tool calls are treated as isolated RPC invocations without emotional modulation, proprioceptive self-reflection, or speculative verification.

Almaz resolves these challenges via an **autopoietic organism architecture** implemented in pure C99:
- **Bare-Metal Footprint:** Cold boots in `<1.5ms` with a baseline footprint under `8.5MB RSS`.
- **Dual-Process Supervision:** An out-of-process watchdog (`almaz-watchdog`) tracks heartbeat metrics, intercepts faults, and automatically rolls back to a Last-Known-Good (`.lkg`) binary.
- **Biological Memory Dynamics:** Implements human memory decay curves (FadeMem) to prune obsolete tactical logs while consolidating salient strategic insights.
- **Homeostatic Regulation:** Dynamic emotional state tracking (VIGIL) modulates risk tolerance, triggering a metacognitive circuit breaker when persistent failures are detected.
- **Speculative Shadow Verification:** Spawns parallel git worktrees to test candidate patches under compiler check and unit tests before merging to the working tree.
- **Immutable Defense-in-Depth:** Core production binaries are locked with kernel immutable attributes (`chattr +i`), preventing unauthorized runtime tampering.

---

## Comparative Benchmark Matrix

| Dimension / Metric | Almaz (Pure C99) | Belya (Pure C99) | Claude Code (Node/TS) | Hermes Agent (Python) |
| :--- | :---: | :---: | :---: | :---: |
| **Cognitive Paradigm** | **Autopoietic Organism** | **Deterministic Engineer** | Tool-Calling State Machine | Self-Improving Skill Agent |
| **Cold Boot Latency** | **<1.5 ms** | **<1.5 ms** | ~850 ms | ~1,200 ms |
| **Peak RSS Memory** | **<8.5 MB** | **<8.0 MB** | 350 MB – 850 MB | 450 MB – 1.8 GB |
| **Crash Recovery** | **Watchdog + LKG Fallback** | Process / Container | Process Exit | Container Restart |
| **Memory Architecture** | **FadeMem (Decay + Consolidation)** | Vector + AST Graph | FIFO / Truncation | Vector Index (Mem0) |
| **Emotional Homeostasis** | **VIGIL (Confidence / Frustration)** | Deterministic VTable | None | None |
| **Multi-Worktree Exploration**| **Parallel Speculative Branching** | **Parallel Speculative Branching** | Single Worktree | Sequential Execution |
| **Native Stdio LSP Client** | **Pure C99 JSON-RPC** | **Pure C99 JSON-RPC** | Extension / Plugin | External Tool Workaround |
| **Inter-Agent Mesh IPC** | **Unix Socket (`AF_UNIX`)** | **Unix Socket (`AF_UNIX`)** | None | HTTP Broker |
| **Trajectory Distillation** | **OpenAI + ShareGPT + Hermes DPO** | **OpenAI + ShareGPT + Hermes DPO** | None (Proprietary Logs) | Hermes Atropos |
| **Tamper Protection** | **Kernel Immutable (`chattr +i`)** | Kernel Hardening | User Space | User Space |

---

## Architectural Topology: Dual-Process Symbiosis

Almaz operates as a symbiotic dual-process supervisor pair to ensure zero downtime in production:

```
                     ┌──────────────────────────────────────────────┐
                     │            Host Systemd Supervisor           │
                     └──────────────────────┬───────────────────────┘
                                            │
                                            ▼
                     ┌──────────────────────────────────────────────┐
                     │               almaz-watchdog                 │
                     │          (Self-Healing Supervisor)           │
                     │ • Fork/Exec & Process Heartbeat Monitoring   │
                     │ • LKG Fallback Rollback on Fault Loop        │
                     │ • Crash Signature Diagnostic Logging         │
                     └──────────────────────┬───────────────────────┘
                                            │ fork() & monitor
                                            ▼
                     ┌──────────────────────────────────────────────┐
                     │                    almaz                     │
                     │         (Sovereign Cognitive Organism)       │
                     ├──────────────────────────────────────────────┤
                     │ • SARSI Self-Model   • VIGIL Emotional State │
                     │ • FadeMem Retention  • CORAL Dual Routing    │
                     │ • SimpleMem Facts    • Autonomic Subconscious│
                     │ • Stdio LSP Engine   • Terminal Raw TUI HUD  │
                     │ • Speculative Shadow • Inter-Agent Mesh IPC  │
                     └──────────────────────┬───────────────────────┘
                                            │
                                            ▼
                     ┌──────────────────────────────────────────────┐
                     │          Inter-Agent Mesh Socket             │
                     │         /tmp/belya_almaz_mesh.sock           │
                     │     Direct IPC Pair Programming with Belya   │
                     └──────────────────────────────────────────────┘
```

---

## Core Cognitive Systems & Organs

### 1. SARSI Persistent Self-Model & Introspective Calibration
Stored in SQLite table `self_model`:
- **Proprioceptive Monitoring:** Tracks lifetime turn count, active goals, calibration confidence ($0.0 - 1.0$), latency averages, and RSS memory consumption.
- **Dynamic Calibration:** Adjusts generation temperature and verification stringency based on historical task success rates.

### 2. VIGIL Emotional Appraisal Engine
Mathematical emotional state machine tracking internal confidence and frustration metrics:
- Successes increment confidence and decay frustration.
- Repeated tool or compilation failures drive frustration upward.
- When frustration crosses the critical threshold, the **Metacognitive Circuit Breaker** trips: Almaz halts blind retries, generates a root-cause diagnostic hypothesis, and switches to deep plan-mode analysis or requests operator clarification (`ask_user`).

### 3. FadeMem Biological Memory Decay & Concept Fusion
Models human long-term and short-term memory dynamics via an exponential retention curve:
$$R(t) = e^{-\frac{t}{S}}$$
Where $S$ denotes memory strength (frequency of access $\times$ emotional salience).
- High-salience memories are permanently consolidated into core knowledge.
- Low-salience, ephemeral execution traces decay over time and are pruned or abstracted during subconscious idle cycles.

### 4. CORAL Dual-Stream Memory Routing
Routes incoming observations and agent reflections into two distinct cognitive tiers:
- **Stream A (Tactical / Immediate):** High-resolution, ephemeral execution logs retained in working memory during an active task.
- **Stream B (Strategic / Permanent):** Invariant architectural constraints, user directives, and project idioms indexed into durable SQLite FTS5 storage.

### 5. SimpleMem De-Linearized Semantic Fact Extraction
Extracts atomic `(subject, predicate, object)` relational triples from conversational turns and code reviews. Enables zero-cost factual recall without consuming valuable context tokens.

### 6. KnowSelf Situational Memory Gating
Protects prompt context by filtering out historical memory entries unless situational embeddings and file path anchors match the active working directory.

### 7. Autonomic Subconscious Idle Loop & Intrinsic Goals
During idle periods (when waiting for user input or batch schedules), Almaz triggers subconscious maintenance:
- Runs database integrity audits (`sqlite_integrity_check_state`).
- Advances background intrinsic goals registered in `agent_goals`.
- Automatically distills repeated multi-step workflows into disk skills (`skills/<name>/SKILL.md`).

### 8. ROMA Recursive Context Firewall
Strictly isolates child sub-agent executions. Output buffers from nested workers are filtered, compressed, and validated before returning to the parent orchestrator, preventing context flooding.

---

## SOTA Dominance Architecture

### Pillar 1: Interactive Terminal TUI & Split Diff Inspector
- **Raw-Mode Single-Keystroke Approval (`tui_interactive_approve`)**:
  - `[Y]` Apply tool execution immediately.
  - `[N]` Safely reject execution.
  - `[E]` Inline argument edit mode.
  - `[A]` Whitelist tool for session duration.
  - `[D]` Launch interactive terminal split-diff inspector.
- **Split Side-by-Side Diff Inspector (`tui_render_side_by_side_diff`)**: Formats unified diffs into aligned dual columns with ANSI syntax coloring, line numbers, and box-drawing borders.
- **Live Token & RSS HUD (`tui_render_token_hud`)**: Displays upstream model, prompt/completion token count, context cache utilization %, execution latency, and resident memory (RSS).

### Pillar 2: Native Stdio LSP JSON-RPC Integration
- Zero-dependency, pure C99 stdio JSON-RPC 2.0 client communicating over bidirectional pipes with language servers:
  - C/C++: `clangd`
  - Python: `pyright` / `pylsp`
  - Go: `gopls`
  - Rust: `rust-analyzer`
  - TypeScript: `typescript-language-server`
- Language server diagnostics feed directly into pre-flight safety gates, catching semantic errors before edits are committed to disk.

### Pillar 3: Speculative Multi-Worktree Candidate Dispatcher
Explores up to 3 candidate solutions simultaneously across isolated git worktrees:
1. *Candidate A (Surgical Minimal):* Focuses on the smallest viable diff.
2. *Candidate B (Defensive & Robust):* Adds comprehensive boundary assertions and error checks.
3. *Candidate C (Idiomatic Pattern):* Aligns with project architectural idioms.

All candidates are compiled and tested in parallel under AddressSanitizer:
$$\text{Score} = (\text{Passed Tests} \times 10,000) - (\text{Diff Churn} \times 2)$$
The candidate with the highest composite score is merged cleanly into the main repository, and speculative branches are pruned.

### Pillar 4: Direct Inter-Agent Mesh Socket IPC (`/tmp/belya_almaz_mesh.sock`)
- Unix domain socket IPC (`AF_UNIX`) allowing Almaz and Belya to pair-program as a unified pair.
- Non-blocking NDJSON message streaming with sub-millisecond local consultation latency.
- Automatic leader/worker negotiation prevents deadlocks and redundant execution.

---

## Skills Discovery, Trajectory Distillation & Rules Ingestion

### 1. RFC 8615 Agent Skills Discovery (`--export-skills`)
Exports the complete agent skill catalogue to `.well-known/agent-skills/index.json` following the RFC 8615 standard. Enables zero-port, filesystem-level skill discovery for peer agents without exposing network services.

```bash
./build/almaz --export-skills
# Output: ✓ Successfully exported N skills to .well-known/agent-skills/index.json (RFC 8615)
```

### 2. Multi-Format Trajectory Distillation (ShareGPT & Hermes DPO)
Almaz exports training datasets directly from its session memory for local model fine-tuning:
- **ShareGPT Multi-Turn Conversational Tuning:**
  ```bash
  ./build/almaz --export-sharegpt ./data/almaz_sharegpt.jsonl
  ```
- **Hermes Direct Preference Optimization (DPO):** Extracts failed tool invocations paired with successful recoveries to generate `(prompt, chosen, rejected)` training triplets:
  ```bash
  ./build/almaz --export-dpo ./data/almaz_dpo.jsonl
  ```

### 3. Universal Rules Auto-Ingestion
On initialization, Almaz scans the working repository and automatically ingests instructions from standard rule anchors in strict priority order:
1. `ALMAZRULES.md`
2. `Almaz.md`
3. `BELYARULES.md`
4. `Belya.md`
5. `CLAUDE.md`
6. `AGENTS.md`
7. `.agentrules`

### 4. Closed-Loop Skill Auto-Synthesis
When Almaz resolves a complex multi-step error or develops a novel troubleshooting pipeline, `belya_agent_reflect_and_distill()` synthesizes the workflow into a structured `skills/<name>/SKILL.md` specification on disk, eliminating agent amnesia across sessions.

---

## Verification Batteries & Quality Scorecard

Almaz passes strict multi-arena verification:

```
=== ALMAZ QUALITY SCORECARD ===
  [✓] Unit Test Suite:         66/66 Passed (100%)
  [✓] Hidden Holdout Battery:  10/10 Passed (100%)
  [✓] Operation Crucible:      100% Invariants Intact (Zero Leaks, Zero Faults)
  [✓] Host Pre-Flight Selfest: 8/8 Invariant Checks Passed (Exit Code 0)
  [✓] AddressSanitizer/UBSan:  Clean Heap, 0 Leaks, 0 Undefined Behavior
  [✓] Memory Footprint:        <8.5MB Peak Resident Set Size (RSS)
```

1. **Unit Test Suite (`make test`):** 66 tests verifying memory structures, JSON parsing, frontmatter parsing, AST traversal, diff rendering, LSP clients, and worktree isolation.
2. **Hidden Holdout Suite (`make holdout`):** 10 adversarial tests covering path escape containment, frontmatter buffer bounds, emotional state clamping, and FadeMem retention math.
3. **Operation Crucible (`make test-crucible-asan`):** Adversarial battery subjecting Almaz to deep JSON recursion (depth 100/200), byte truncation, SSRF attempts, format string injections, and shell escapes under AddressSanitizer.
4. **Pre-Flight Self-Test Gate (`./build/almaz --selfest`):** Validates active SQLite schema, WAL journal mode, and active state rows before execution commences.

---

## Native Tool Suite (46 Built-In Tools)

| Category | Native Tools |
| :--- | :--- |
| **Filesystem & Execution** | `bash`, `read_file`, `write_file`, `edit_file`, `apply_patch`, `patch_file`, `list_dir`, `search_files`, `ripgrep`, `pty_exec` |
| **Harness & Speculation** | `dispatch_worker_task`, `query_ast_graph`, `run_test_cycle`, `generate_repo_map`, `enter_worktree`, `leave_worktree`, `plan_mode` |
| **Language Server & AST** | `lsp_definition`, `lsp_references`, `lsp_query`, `ast_slice` |
| **Cognitive Memory** | `save_memory`, `recall_memory`, `search_memory`, `save_skill`, `search_skills`, `git_checkpoint`, `git_rollback` |
| **Agent Collaboration** | `mesh_collaborate`, `ask_user`, `audit_security_variants` |
| **Network & MCP** | `fetch_url`, `rest_call`, `define_tool`, `synthesize_mcp_server`, `mcp_call` |

---

## Installation & Quick Start

### Prerequisites
- GCC >= 9.0 or Clang >= 11.0 (C99 standard support)
- POSIX build dependencies: `libcurl`, `sqlite3`, `pthread`, `m`
- Git >= 2.25

On Debian / Ubuntu:
```bash
sudo apt-get update && sudo apt-get install -y build-essential libcurl4-openssl-dev libsqlite3-dev git
```

On macOS:
```bash
brew install curl sqlite
```

### Building from Source
```bash
git clone https://github.com/M4F-S/Almaz.git
cd Almaz
make all
```
Builds the primary binary `build/almaz` and the supervisor binary `almaz-watchdog`.

### Running the Strict Test Batteries
```bash
# 1. Primary Unit Test Battery (66 tests)
make test

# 2. Hidden Holdout Benchmark Suite (10 tests)
make holdout

# 3. Adversarial Stress Battery under AddressSanitizer
make test-crucible-asan

# 4. Host Database Self-Test Gate
./build/almaz --selfest
```

---

## Operating Modes

### Mode 1: Interactive Terminal TUI
Launches interactive terminal mode with ANSI raw-mode keybindings, live token HUD, and split diff inspector:
```bash
./build/almaz
```

### Mode 2: 24/7 VPS Telegram & Omnichannel Daemon
Runs as an omnichannel daemon with continuous polling and session persistence:
```bash
export TELEGRAM_BOT_TOKEN="<token>"
export TELEGRAM_CHAT_ID="<chat_id>"
./build/almaz --telegram
```

### Mode 3: Supervised Self-Healing Watchdog
Executes under the `almaz-watchdog` supervisor, automatically recovering from faults and rolling back to Last-Known-Good (`.lkg`) binaries:
```bash
./almaz-watchdog --telegram
```

---

## Production Deployment & Defense-in-Depth Administration

Almaz runs on production servers under systemd supervision with kernel-level tamper resistance.

### Systemd Service Definition (`/etc/systemd/system/almaz.service`)
```ini
[Unit]
Description=Almaz Sovereign Self-Evolving AI Organism Daemon (Pure C99)
After=network.target

[Service]
Type=simple
User=root
WorkingDirectory=/opt/almaz
ExecStartPre=/usr/local/libexec/almaz/safety_check.sh
ExecStart=/opt/almaz/almaz-watchdog --telegram
Restart=always
RestartSec=5s
LimitNOFILE=65536
CapabilityBoundingSet=~CAP_LINUX_IMMUTABLE

[Install]
WantedBy=multi-user.target
```

### Defense-in-Depth Administration
To update or deploy new binaries, immutable attributes must be temporarily unlocked:
```bash
# Unlock immutable attributes
chattr -i /opt/almaz/almaz /opt/almaz/almaz-watchdog /opt/almaz/almaz.lkg

# Atomically deploy new build
make deploy

# Re-lock immutable attributes
chattr +i /opt/almaz/almaz /opt/almaz/almaz-watchdog /opt/almaz/almaz.lkg

# Verify deployment status and provenance
cat /opt/almaz/.deploy_meta
systemctl status almaz.service
```

---

## Relationship with Twin Sibling Belya

Almaz and [Belya](https://github.com/M4F-S/Belya) are independent, sovereign C99 codebases designed to function as an autonomous pair-programming duo:
- **Belya (The Deterministic Engineer):** Specializes in high-velocity software engineering, rigorous dual VTable enforcement, AST dependency slicing, and enterprise multi-agent company orchestration.
- **Almaz (The Cognitive Organism):** Specializes in introspective self-modeling, emotional state modulation, biological memory decay (FadeMem), and supervised fault recovery.
- **Direct Mesh IPC:** Both instances communicate over `/tmp/belya_almaz_mesh.sock` using non-blocking Unix socket IPC, combining deterministic software engineering with autopoietic cognitive resilience.

---

## License & Provenance

Licensed under the **Apache License, Version 2.0**. See the [LICENSE](LICENSE) file for complete details.

**Maintained by:** [M4F-S](https://github.com/M4F-S)  
**Production Deployment:** `srv1412364` (`187.124.2.26`)  
**Architecture:** Pure C99 POSIX | Release v7.2.0 | SOTA Dominance Architecture
