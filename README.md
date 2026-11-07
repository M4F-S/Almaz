# Almaz — Sovereign Self-Evolving, Self-Healing Autonomous AI Organism & SOTA Dominance Harness (Pure C99)

[![Release](https://img.shields.io/badge/Release-v2.5.0--sota--dominance-blue.svg)](https://github.com/M4F-S/Almaz/releases/tag/v2.5.0-sota-dominance)
[![License](https://img.shields.io/badge/License-Apache_2.0-green.svg)](LICENSE)
[![Language](https://img.shields.io/badge/Language-C99_POSIX-orange.svg)]()
[![Tests](https://img.shields.io/badge/Unit_Tests-66%2F66_Passed_(100%25)-brightgreen.svg)]()
[![Holdouts](https://img.shields.io/badge/Holdout_Battery-10%2F10_Passed_(100%25)-brightgreen.svg)]()
[![Operation_Crucible](https://img.shields.io/badge/Operation_Crucible-100%25_Invariants_Intact-success.svg)]()
[![Watchdog](https://img.shields.io/badge/Supervisor-almaz--watchdog_Active-success.svg)]()
[![Tamper_Proof](https://img.shields.io/badge/Tamper_Lock-chattr_+i_Immutable-red.svg)]()
[![Tools](https://img.shields.io/badge/Native_Tools-46_Built--in_Tools-informational.svg)]()
[![Memory](https://img.shields.io/badge/Memory_Footprint-<8.5MB_Peak_RSS-purple.svg)]()
[![Cold_Start](https://img.shields.io/badge/Cold_Start-<1.5ms-yellow.svg)]()

> **Almaz** is a **sovereign self-evolving, self-healing autonomous AI organism** and execution harness implemented in pure C99. Operating as the autopoietic cognitive twin of [Belya](https://github.com/M4F-S/Belya), Almaz elevates deterministic systems engineering into a living computational organism: a dual-process supervisory watchdog (`almaz-watchdog`), biological memory decay (**FadeMem**), dual-stream tactical/strategic memory routing (**CORAL**), de-linearized semantic fact extraction (**SimpleMem**), an introspective self-model (**SARSI**), an emotional appraisal state machine (**VIGIL** protocol), an interactive ANSI terminal TUI, native stdio LSP integration, speculative shadow worktrees, direct inter-agent mesh socket IPC, and kernel-level immutable tamper resistance (`chattr +i`).

---

## Table of Contents
- [The Autopoietic Organism Paradigm](#the-autopoietic-organism-paradigm)
- [Architectural Topology: Dual-Process Symbiosis](#architectural-topology-dual-process-symbiosis)
- [SOTA Dominance Architecture (v2.5.0)](#sota-dominance-architecture-v250)
  - [Pillar 1: Interactive Terminal TUI Dominance](#pillar-1-interactive-terminal-tui-dominance)
  - [Pillar 2: Native Stdio LSP JSON-RPC Integration](#pillar-2-native-stdio-lsp-json-rpc-integration)
  - [Pillar 3: Speculative Multi-Worktree Candidate Dispatcher](#pillar-3-speculative-multi-worktree-candidate-dispatcher)
  - [Pillar 4: Direct Inter-Agent Mesh Protocol (`/tmp/belya_almaz_mesh.sock`)](#pillar-4-direct-inter-agent-mesh-protocol-tmpbelya_almaz_meshsock)
- [Core Cognitive Systems & Organs](#core-cognitive-systems--organs)
  - [1. SARSI Persistent Self-Model & Introspective Calibration](#1-sarsi-persistent-self-model--introspective-calibration)
  - [2. VIGIL Emotional Appraisal System (`confidence` & `frustration`)](#2-vigil-emotional-appraisal-system-confidence--frustration)
  - [3. FadeMem Biological Memory Decay & Concept Fusion](#3-fademem-biological-memory-decay--concept-fusion)
  - [4. CORAL Dual-Stream Memory Routing (Tactical vs. Strategic)](#4-coral-dual-stream-memory-routing-tactical-vs-strategic)
  - [5. SimpleMem De-Linearized Semantic Fact Extraction](#5-simplemem-de-linearized-semantic-fact-extraction)
  - [6. KnowSelf Situational Memory Gating](#6-knowself-situational-memory-gating)
  - [7. Autonomic Subconscious Idle Loop & Intrinsic Goals](#7-autonomic-subconscious-idle-loop--intrinsic-goals)
  - [8. ROMA Recursive Context Firewall](#8-roma-recursive-context-firewall)
- [Frontier Agent Comparison (Almaz vs. Claude Code vs. Hermes Agent)](#frontier-agent-comparison-almaz-vs-claude-code-vs-hermes-agent)
- [Multi-Arena Benchmarks & Verification Batteries](#multi-arena-benchmarks--verification-batteries)
  - [1. Comprehensive 6-Arena Scorecard (36/36 - 100% Passed)](#1-comprehensive-6-arena-scorecard-3636---100-passed)
  - [2. Unit Test Suite (66/66 Passed)](#2-unit-test-suite-6666-passed)
  - [3. Hidden Holdout Benchmark Suite (10/10 Passed)](#3-hidden-holdout-benchmark-suite-1010-passed)
  - [4. Operation Crucible Adversarial Stress Battery](#4-operation-crucible-adversarial-stress-battery)
  - [5. Host Pre-Flight Self-Test Gate (`--selfest`)](#5-host-pre-flight-self-test-gate---selfest)
- [Native Tool Suite (46 Built-In Tools)](#native-tool-suite-46-built-in-tools)
- [Installation & Quick Start](#installation--quick-start)
  - [Prerequisites](#prerequisites)
  - [Building from Source](#building-from-source)
  - [Running the Strict Test Batteries](#running-the-strict-test-batteries)
- [Operating Modes](#operating-modes)
  - [Mode 1: Interactive Terminal CLI](#mode-1-interactive-terminal-cli)
  - [Mode 2: 24/7 VPS Telegram Bot Daemon](#mode-2-247-vps-telegram-bot-daemon)
  - [Mode 3: Self-Healing Watchdog Mode](#mode-3-self-healing-watchdog-mode)
- [Production Deployment & Defense-in-Depth Administration](#production-deployment--defense-in-depth-administration)
- [Relationship with Twin Sibling Belya](#relationship-with-twin-sibling-belya)
- [License & Provenance](#license--provenance)

---

## The Autopoietic Organism Paradigm

Most AI coding agents are static state machines: they run, execute a tool call, and terminate. When an exception or corruption occurs, they fail catastrophically.

Almaz is designed as an **autopoietic organism**—a self-maintaining, homeostatic entity:
- **Introspective Proprioception:** Tracks its own internal state, memory access counts, latency, and RSS footprint.
- **Emotional Modulation (VIGIL):** Regulates exploration vs. exploitation based on dynamic frustration and confidence metrics.
- **Continuous Memory Maintenance (FadeMem):** Background subconscious threads prune dead context and fuse related concepts through biological decay curves.
- **Tamper-Proof Self-Protection:** Secures its core runtime against external interference or rogue hallucinations using kernel-level immutable attributes (`chattr +i`).

---

## Architectural Topology: Dual-Process Symbiosis

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

## SOTA Dominance Architecture (v2.5.0)

### Pillar 1: Interactive Terminal TUI Dominance
- **Raw-Mode Single-Keystroke Modal (`tui_interactive_approve`)**:
  - `[Y]` Apply immediately.
  - `[N]` Reject tool execution safely.
  - `[E]` Inline argument edit mode.
  - `[A]` Session-persistent whitelist.
  - `[D]` Launch instant side-by-side terminal diff viewer.
- **Split Side-by-Side Diff Inspector (`tui_render_side_by_side_diff`)**: Formats unified diffs into aligned dual columns with ANSI colored insertions/deletions and box-drawing borders.
- **Live Token & RSS HUD (`tui_render_token_hud`)**: Displays upstream model, prompt/completion tokens, cache hit %, latency (ms), and active process RSS.

### Pillar 2: Native Stdio LSP JSON-RPC Integration
- Pure C99 stdio JSON-RPC 2.0 client communicating over bidirectional pipes with language servers (`clangd`, `pyright`, `gopls`, `rust-analyzer`, `typescript-language-server`).
- Extracted diagnostics feed into `belya_harness_preflight_check()`, enabling instant zero-guesswork auto-healing before files are written to disk.

### Pillar 3: Speculative Multi-Worktree Candidate Dispatcher
- Explores 1–3 candidate strategies simultaneously in isolated shadow git worktrees:
  - *Surgical Minimal* (least diff churn)
  - *Defensive & Robust* (strict validation, explicit bounds)
  - *Idiomatic Pattern* (standard conventions)
- Automatically evaluates each candidate under AddressSanitizer and unit tests:
  $$\text{Score} = (\text{Tests Passed} \times 10000) - (\text{Diff Churn} \times 2)$$
- Merges the winning candidate cleanly to main workspace and silently discards losers with zero noise.

### Pillar 4: Direct Inter-Agent Mesh Protocol (`/tmp/belya_almaz_mesh.sock`)
- Unix domain socket IPC (`AF_UNIX`) allowing Almaz and Belya to pair-program as a unified duo.
- Automatic leader/client role detection, NDJSON streaming, and sub-millisecond consultation latency.

---

## Core Cognitive Systems & Organs

### 1. SARSI Persistent Self-Model & Introspective Calibration
Stored in SQLite table `self_model`:
- Tracks lifetime turn count, active goals, calibration confidence ($0.0 - 1.0$), and distress levels.
- Automatically adjusts decision temperature and verification thresholds based on past execution history.

### 2. VIGIL Emotional Appraisal System (`confidence` & `frustration`)
Mathematical emotional state machine updating after every turn:
- Successes increment confidence and lower frustration.
- Repeated failures trigger the **Metacognitive Circuit Breaker**, escalating strategy from quick fixes to root-cause refactoring or user consultation (`ask_user`).

### 3. FadeMem Biological Memory Decay & Concept Fusion
Models human memory retention via the Ebbinghaus forgetting curve:
$$R(t) = e^{-\frac{t}{S}}$$
- Memories with high recall frequency and emotional salience are permanently consolidated.
- Stale, low-salience memories undergo subconscious decay and are pruned or fused into high-level concept abstractions during idle cycles.

### 4. CORAL Dual-Stream Memory Routing (Tactical vs. Strategic)
Routes incoming observations into two distinct cognitive tiers:
- **Stream A (Tactical / Immediate):** High-detail, ephemeral execution traces kept in working memory.
- **Stream B (Strategic / Permanent):** Architectural invariant rules, user preferences, and project patterns saved into durable FTS5 storage.

### 5. SimpleMem De-Linearized Semantic Fact Extraction
Extracts atomic subject-predicate-object triples from unstructured conversational turns, allowing semantic recall without polluting token context.

### 6. KnowSelf Situational Memory Gating
Prevents context contamination by filtering out irrelevant historical memories unless situational triggers match current workspace files and tasks.

### 7. Autonomic Subconscious Idle Loop & Intrinsic Goals
When idle, Almaz enters a subconscious cognition cycle:
- Audits active database integrity (`sqlite_integrity_check_state`).
- Evaluates progress on background intrinsic goals (`agent_goals`).
- Synthesizes recurring multi-step workflows into disk skills (`.belya/skills/*/SKILL.md`).

### 8. ROMA Recursive Context Firewall
Strictly isolates child sub-agent outputs, preventing raw stdout spills from degrading parent context reasoning.

---

## Frontier Agent Comparison (Almaz vs. Claude Code vs. Hermes Agent)

| Metric / Dimension | Almaz (C99 Bare-Metal) | Claude Code (Node/TS) | Hermes Agent (Python) |
| :--- | :---: | :---: | :---: |
| **Cognitive Paradigm** | **Autopoietic Organism** | Tool-Calling State Machine | Self-Improving Skill Agent |
| **Cold Boot Time** | **<1.5 ms** | ~850 ms | ~1,200 ms |
| **Baseline RSS Memory** | **<8.5 MB** | 350 MB – 850 MB | 450 MB – 1.8 GB |
| **Memory Decay & Consolidation** | **FadeMem Biological Math** | None (FIFO / Truncate) | None (FIFO / Vector DB) |
| **Emotional Homeostasis** | **VIGIL State Machine** | None | None |
| **Tamper Proofing** | **`chattr +i` Kernel Seal** | None | None |
| **Supervisory Architecture** | **Dual-Process Watchdog** | Single Process | Single Process / Systemd |
| **Multi-Worktree Exploration** | **Speculative Candidate Dispatch** | Single Worktree | Sequential Execution |
| **Native Stdio LSP Client** | **Built-in Pure C99 JSON-RPC** | Plugin / External Extension | Tool-Call Workaround |
| **Inter-Agent Mesh IPC** | **Unix Socket (`AF_UNIX`)** | None | HTTP Broker |

---

## Multi-Arena Benchmarks & Verification Batteries

Almaz undergoes continuous multi-layer verification:

```
=== ALMAZ VERIFICATION SUMMARY ===
  [✓] Unit Test Battery:       66/66 Passed (100%)
  [✓] Hidden Holdout Battery:  10/10 Passed (100%)
  [✓] Operation Crucible:      100% Invariants Intact (Zero Leaks, Zero Faults)
  [✓] Host Pre-Flight Selfest: 8/8 System Checks Passed (Exit Code 0)
  [✓] AddressSanitizer/UBSan:  0 Leaks, 0 Faults, Clean Heap
```

### 1. Comprehensive 6-Arena Scorecard (36/36 - 100% Passed)
Validates tools, cognitive organs, dual-tier failover, and PTY subshell execution.

### 2. Unit Test Suite (66/66 Passed)
Strict unit verification suite testing all memory structures, MiniJSON, frontmatter, AST navigation, TUI diff rendering, LSP client, and speculative worktrees.

### 3. Hidden Holdout Benchmark Suite (10/10 Passed)
Ten adversarial regression holdouts verifying frontmatter bounds, path jailing, emotional appraisal clamping, and FadeMem retention math under stress.

### 4. Operation Crucible Adversarial Stress Battery
Adversarial fuzzing battery testing deep JSON recursion (depth 100/200), byte-by-byte truncation, SSRF network exploits, format string injections, and ripgrep command escapes with **100% invariants intact and zero memory leaks**.

### 5. Host Pre-Flight Self-Test Gate (`--selfest`)
Executed by `/usr/local/libexec/almaz/safety_check.sh` on every service startup:
- Validates SQLite schema integrity and WAL journal mode.
- Confirms active row presence in `self_model`, `agent_goals`, and `session_messages`.
- Exits with status `0` only when all invariants pass.

---

## Native Tool Suite (46 Built-In Tools)

Almaz includes all 46 native tools from the SOTA Dominance Harness:
- **Core Filesystem & Execution:** `bash`, `read_file`, `write_file`, `edit_file`, `apply_patch`, `patch_file`, `list_dir`, `search_files`, `ripgrep`, `pty_exec`.
- **Super Agent Harness & Isolation:** `dispatch_worker_task`, `query_ast_graph`, `run_test_cycle`, `generate_repo_map`, `enter_worktree`, `leave_worktree`, `plan_mode`.
- **Language Server & AST:** `lsp_definition`, `lsp_references`, `lsp_query`, `ast_slice`.
- **Inter-Agent Collaboration:** `mesh_collaborate`, `ask_user`, `audit_security_variants`.
- **Cognitive Memory & Scoping:** `save_memory`, `recall_memory`, `search_memory`, `save_skill`, `search_skills`, `git_checkpoint`, `git_rollback`.
- **Network & Dynamic Extensions:** `fetch_url`, `rest_call`, `define_tool`, `synthesize_mcp_server`, `mcp_call`.

---

## Installation & Quick Start

### Prerequisites
- GCC >= 9.0 or Clang >= 11.0 (C99 standard support)
- POSIX libraries (`libcurl`, `sqlite3`, `pthread`, `m`)
- Git >= 2.25

On Debian / Ubuntu:
```bash
sudo apt-get update && sudo apt-get install -y build-essential libcurl4-openssl-dev libsqlite3-dev git
```

### Building from Source
```bash
git clone https://github.com/M4F-S/Almaz.git
cd Almaz
make all
```
Generates `build/almaz` (the organism core) and `almaz-watchdog` (the supervisor).

### Running the Strict Test Batteries
```bash
# 1. Run the primary 66-test verification battery
make test

# 2. Run the 10-test hidden holdout suite
make holdout

# 3. Run Operation Crucible (Adversarial stress and attack penetration suite)
make test-crucible

# 4. Verify under AddressSanitizer and UndefinedBehaviorSanitizer
make test-asan

# 5. Verify the self-test gate against the active database
./build/almaz --selfest
```

---

## Operating Modes

### Mode 1: Interactive Terminal CLI
```bash
./build/almaz
```

### Mode 2: 24/7 VPS Telegram Bot Daemon
```bash
./build/almaz --telegram
```

### Mode 3: Self-Healing Watchdog Mode
Run Almaz under the dedicated external supervisor:
```bash
./almaz-watchdog --telegram
```

---

## Production Deployment & Defense-in-Depth Administration

Almaz runs in production as a supervised systemd service with immutable kernel attributes.

### Systemd Service Configuration (`/etc/systemd/system/almaz.service`)
```ini
[Unit]
Description=Almaz Sovereign Self-Evolving AI Organism & Telegram Daemon (Pure C99)
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

### Tamper-Resistance Administration
To update or deploy the binary, immutable flags must be unlocked by the operator:
```bash
# Unlock immutable attributes
chattr -i /opt/almaz/almaz /opt/almaz/almaz-watchdog /opt/almaz/almaz.lkg

# Deploy new binary
make deploy

# Re-lock immutable attributes
chattr +i /opt/almaz/almaz /opt/almaz/almaz-watchdog /opt/almaz/almaz.lkg

# Check status and provenance metadata
cat /opt/almaz/.deploy_meta
systemctl status almaz.service
```

---

## Relationship with Twin Sibling Belya

Almaz and [Belya](https://github.com/M4F-S/Belya) are independent, sovereign C99 codebases:
- **Belya (The Deterministic Core):** Engineered for predictable software engineering, unit test generation, and enterprise multi-agent company orchestration.
- **Almaz (The Cognitive Organism):** Engineered for autopoietic survival, emotional modulation, biological memory decay, and automated crash recovery.
- **Inter-Agent Mesh:** Both communicate directly over `/tmp/belya_almaz_mesh.sock` to form an autonomous pair-programming duo.

---

## License & Provenance

Licensed under the **Apache License, Version 2.0**. See the [LICENSE](LICENSE) file for terms and conditions.

**Maintained by:** [M4F-S](https://github.com/M4F-S)  
**Production Deployment:** `srv1412364` (`187.124.2.26`)  
**Architecture:** Pure C99 POSIX | SOTA Dominance Release v2.5.0 (Evolution 4.0)
