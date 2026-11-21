# Almaz — Sovereign Self-Evolving, Self-Healing Autonomous AI Organism & SOTA Dominance Harness

[![Release](https://img.shields.io/badge/Release-v7.3.0-blue.svg)](https://github.com/M4F-S/Almaz/releases/tag/v7.3.0)
[![License](https://img.shields.io/badge/License-Apache_2.0-green.svg)](LICENSE)
[![Language](https://img.shields.io/badge/Language-C99_POSIX-orange.svg)]()
[![Tests](https://img.shields.io/badge/Unit_Tests-66%2F66_Passed_(100%25)-brightgreen.svg)]()
[![Holdouts](https://img.shields.io/badge/Holdout_Battery-65%2F65_Passed_(100%25)-brightgreen.svg)]()
[![Operation_Crucible](https://img.shields.io/badge/Operation_Crucible-100%25_Invariants_Intact-success.svg)]()
[![Memory_Safety](https://img.shields.io/badge/Memory_Safety-0_Leaks_|_ASan_|_UBSan_Certified-success.svg)]()
[![Dual_Transport_MCP](https://img.shields.io/badge/MCP-Dual_HTTP/SSE_+_Stdio-blueviolet.svg)]()
[![Supervisor](https://img.shields.io/badge/Supervisor-almaz--watchdog_Active-success.svg)]()
[![Tamper_Proof](https://img.shields.io/badge/Tamper_Lock-chattr_+i_Immutable-red.svg)]()
[![RFC_8615](https://img.shields.io/badge/Skills_Discovery-RFC_8615_Compliant-success.svg)]()
[![Memory](https://img.shields.io/badge/Memory_Footprint-<12.5MB_Peak_RSS-purple.svg)]()
[![Cold_Start](https://img.shields.io/badge/Cold_Start-<1.8ms-yellow.svg)]()

> **Almaz** is a bare-metal, autopoietic autonomous AI organism and execution harness implemented in pure C99. Functioning as the cognitive twin of [Belya](https://github.com/M4F-S/Belya), Almaz bridges deterministic systems engineering and adaptive computational cognition: a dual-process self-healing supervisor (`almaz-watchdog`), biological memory decay (**FadeMem**), dual-stream tactical and strategic memory routing (**CORAL**), de-linearized semantic fact extraction (**SimpleMem**), an introspective self-model (**SARSI**), an emotional appraisal state machine (**VIGIL**), persistent dual-transport MCP (native HTTP/SSE + stdio POSIX pipes), an interactive ANSI terminal TUI, native stdio LSP integration, speculative shadow worktree exploration, direct inter-agent mesh socket IPC, RFC 8615 skills discovery, trajectory distillation (ShareGPT & Hermes DPO), and kernel-level immutable tamper resistance (`chattr +i`).

---

## Table of Contents
- [Executive Overview](#executive-overview)
- [Comparative Benchmark Matrix](#comparative-benchmark-matrix)
- [Architectural Topology: Dual-Process Symbiosis](#architectural-topology-dual-process-symbiosis)
- [Persistent Dual-Transport MCP Subsystem](#persistent-dual-transport-mcp-subsystem)
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
  - [Pillar 4: Direct Inter-Agent Mesh Socket IPC](#pillar-4-direct-inter-agent-mesh-socket-ipc)
- [Skills Discovery, Trajectory Distillation & Rules Ingestion](#skills-discovery-trajectory-distillation--rules-ingestion)
  - [1. RFC 8615 Agent Skills Discovery (`--export-skills`)](#1-rfc-8615-agent-skills-discovery---export-skills)
  - [2. Multi-Format Trajectory Distillation (ShareGPT & Hermes DPO)](#2-multi-format-trajectory-distillation-sharegpt--hermes-dpo)
  - [3. Universal Rules Auto-Ingestion](#3-universal-rules-auto-ingestion)
  - [4. Closed-Loop Skill Auto-Synthesis](#4-closed-loop-skill-auto-synthesis)
- [Verification Batteries & Quality Scorecard](#verification-batteries--quality-scorecard)
- [Native Tool Suite (46 Built-In Tools + 64 MCP Tool Cap)](#native-tool-suite-46-built-in-tools--64-mcp-tool-cap)
- [Installation & Quick Start](#installation--quick-start)
  - [Prerequisites](#prerequisites)
  - [Building from Source](#building-from-source)
  - [Running the Strict Test Batteries](#running-the-strict-test-batteries)
- [Operating Modes](#operating-modes)
  - [Mode 1: Interactive Terminal TUI](#mode-1-interactive-terminal-tui)
  - [Mode 2: 24/7 Telegram & Omnichannel Daemon](#mode-2-247-telegram--omnichannel-daemon)
  - [Mode 3: Supervised Self-Healing Watchdog](#mode-3-supervised-self-healing-watchdog)
- [Production Deployment & Defense-in-Depth Administration](#production-deployment--defense-in-depth-administration)
- [Relationship with Twin Sibling Belya](#relationship-with-twin-sibling-belya)
- [License & Provenance](#license--provenance)

---

## Executive Overview

Where Belya focuses strictly on deterministic compiler-driven software engineering, Almaz introduces biological cognition and self-healing autopoiesis:
- **Dual-Process Supervision:** An unprivileged worker process executes cognitive loops supervised by `almaz-watchdog`. If the worker exhausts step budgets or crashes, the watchdog captures diagnostics, rolls back to the Last-Known-Good (`.lkg`) binary, and restarts within milliseconds.
- **Dual-Transport Model Context Protocol (MCP):** Connects to remote vector daemons via high-performance non-blocking HTTP/SSE streams (`mcp_http.c`) with zero local process overhead, alongside stdio POSIX pipes for web automation and code tools.
- **FadeMem & CORAL Memory Routing:** Mimics human memory retention curves by decaying low-salience episodic events while consolidating critical architectural discoveries into high-salience semantic knowledge.
- **VIGIL Emotional Dynamics:** Self-calibrating state machine dynamically modulates agent risk-taking, concurrency, and temperature according to cognitive strain and compilation feedback.

---

## Comparative Benchmark Matrix

| Feature | Almaz (Pure C99) | AutoGPT (Python) | MemGPT / Letta (Python) | Devin (Cloud) |
| :--- | :---: | :---: | :---: | :---: |
| **Language & Toolchain** | **Pure C99 (POSIX)** | Python / AsyncIO | Python / FastAPI | Closed SaaS |
| **Active Memory Footprint** | **< 12.5 MB RSS** | ~650 MB | ~850 MB | Cloud SaaS |
| **Cold Start Latency** | **< 1.8 ms** | ~4,200 ms | ~3,800 ms | Web Session |
| **Memory Architecture** | **FadeMem + CORAL + SimpleMem** | Vector DB dump | Hierarchical Context | Proprietary |
| **Self-Healing Mechanism** | **almaz-watchdog (.lkg rollback)** | Exception stacktrace | Restart container | Cloud retry |
| **MCP Transports** | **Dual HTTP/SSE + Stdio Pipes** | Python MCP wrapper | REST APIs | Proprietary |
| **Tamper Resistance** | **Kernel Immutable (`chattr +i`)** | None | Container read-only | Cloud isolation |
| **Compiler Watchdog** | **Pre-Flight Auto-Revert** | None | None | Cloud unit test |

---

## Architectural Topology: Dual-Process Symbiosis

```
                       +----------------------------------+
                       |   almaz-watchdog (Supervisor)    |
                       |  - Health & Heartbeat Check      |
                       |  - Automatic .lkg Rollback       |
                       |  - Immutable File Descriptor Lock|
                       +----------------------------------+
                                        |
                            fork() & supervise restarts
                                        |
                                        v
                       +----------------------------------+
                       |        Almaz C99 Core Daemon     |
                       |  - Dual-Transport MCP Client     |
                       |  - VIGIL Emotional State Machine |
                       |  - FadeMem & CORAL Routing       |
                       |  - SARSI Self-Model Calibration  |
                       +----------------------------------+
                                        |
               +------------------------+------------------------+
               |                                                 |
               v                                                 v
    +-----------------------+                         +-----------------------+
    |  Remote Knowledge MCP |                         | Stdio Subprocess MCP  |
    |  (HTTP/SSE 127.0.0.1) |                         | (Playwright, GitHub)  |
    +-----------------------+                         +-----------------------+
```

---

## Persistent Dual-Transport MCP Subsystem

Almaz incorporates a dual-transport Model Context Protocol (MCP) subsystem, combining zero-overhead HTTP/SSE streaming with POSIX pipe subprocesses:
- **Declarative Configuration (`mcp_servers.json`):**
  ```json
  {
    "mcpServers": {
      "knowledge-http": {
        "url": "http://127.0.0.1:8760/mcp"
      },
      "playwright": {
        "command": "npx",
        "args": ["-y", "@playwright/mcp"]
      },
      "context7": {
        "command": "npx",
        "args": ["-y", "@upstash/context7-mcp"]
      },
      "github": {
        "command": "npx",
        "args": ["-y", "@modelcontextprotocol/server-github"],
        "env": { "GITHUB_PERSONAL_ACCESS_TOKEN": "${GITHUB_TOKEN}" }
      }
    }
  }
  ```
- **Zero-Overhead HTTP/SSE Transport (`mcp_http.c`):** Queries remote vector knowledge daemons over persistent keep-alive HTTP connections without launching local Node.js or Python runtime subprocesses.
- **Stdio Subprocess Transport (`mcp_client.c`):** Spawns Playwright and GitHub MCP servers over bidirectional POSIX pipes with automated `/tmp` caching and non-fatal handshake timeout safety.
- **Strict Registration Caps:** Up to 8 concurrent MCP servers, with tool registration capped at 64 tools.

---

## Core Cognitive Systems & Organs

### 1. SARSI Persistent Self-Model & Introspective Calibration
Almaz tracks its own operational accuracy and lifetime performance across SQLite tables (`self_model`), computing success-to-failure ratios, average token expenditures, and memory growth rates.

### 2. VIGIL Emotional Appraisal Engine
A continuous 3D valence-arousal-dominance state machine dynamically shapes prompt directives:
- **High Strain / Compilation Failure:** Throttles concurrency, increases temperature damping, and engages defensive verification guards.
- **Consistent Success / Low Entropy:** Enables speculative parallel execution and streamlines tool approvals.

### 3. FadeMem Biological Memory Decay & Concept Fusion
Prevents context window saturation through mathematically bounded exponential decay:
$$S(t) = S_0 \cdot e^{-\lambda \cdot \Delta t}$$
Low-salience memories are periodically compressed and pruned into high-density semantic concept clusters.

### 4. CORAL Dual-Stream Memory Routing
Separates immediate episodic turn details (tactical) from enduring architectural invariants and user preferences (strategic).

### 5. SimpleMem De-Linearized Semantic Fact Extraction
Parses conversation streams into subject-predicate-object triples stored in SQLite FTS5, facilitating rapid sub-millisecond keyword and associative recall.

### 6. KnowSelf Situational Memory Gating
Dynamically injects relevant memories and skills into context based on semantic salience matching, preventing context pollution.

### 7. Autonomic Subconscious Idle Loop & Intrinsic Goals
During idle periods without user input, Almaz executes autonomic maintenance cycles:
- Prunes expired timeline records.
- Verifies workspace git repository integrity.
- Syncs self-model metrics and checks system RSS consumption.

### 8. ROMA Recursive Context Firewall
Isolates subagent tool traces and raw intermediate error buffers behind a recursive context firewall, preventing hallucination cascades.

---

## SOTA Dominance Architecture

### Pillar 1: Interactive Terminal TUI & Split Diff Inspector
- Single-keystroke modal approval for all tool mutations (`[Y]` Apply, `[N]` Reject, `[E]` Edit args, `[A]` Always allow, `[D]` Split diff).
- Dual-column side-by-side terminal diff viewer with ANSI syntax highlighting.

### Pillar 2: Native Stdio LSP JSON-RPC Integration
- Connects directly to language servers over stdio pipes for jump-to-definition, reference lookups, and compiler diagnostic ingestion.

### Pillar 3: Speculative Multi-Worktree Candidate Dispatcher
- Simultaneously branches parallel Git worktrees to evaluate minimal, defensive, and idiomatic implementation strategies, atomically merging the best candidate.

### Pillar 4: Direct Inter-Agent Mesh Socket IPC
- High-speed Unix domain socket IPC (`/tmp/belya_almaz_mesh.sock`) enabling Almaz and Belya to pair-program with sub-millisecond consultation latency.

---

## Skills Discovery, Trajectory Distillation & Rules Ingestion

### 1. RFC 8615 Agent Skills Discovery (`--export-skills`)
Exports the complete agent skill catalogue to `.well-known/agent-skills/index.json` following the RFC 8615 standard without opening network ports.

### 2. Multi-Format Trajectory Distillation (ShareGPT & Hermes DPO)
- **ShareGPT:** `./build/almaz --export-sharegpt ./data/almaz_sharegpt.jsonl`
- **Hermes DPO:** `./build/almaz --export-dpo ./data/almaz_dpo.jsonl`

### 3. Universal Rules Auto-Ingestion
Automatically loads instructions from `ALMAZRULES.md`, `BELYARULES.md`, `CLAUDE.md`, `AGENTS.md`, and `.agentrules`.

### 4. Closed-Loop Skill Auto-Synthesis
Distills successful error recoveries into reusable `skills/<name>/SKILL.md` procedural specifications.

---

## Verification Batteries & Quality Scorecard

Almaz passes strict multi-arena verification:

```
=== ALMAZ QUALITY SCORECARD ===
  [✓] Unit Test Suite:         66/66 Modules Passed (100%) + MCP Selftest
  [✓] Hidden Holdout Battery:  65/65 Assertions Passed (100%)
  [✓] Operation Crucible:      100% Invariants Intact (Zero Leaks, Zero Faults)
  [✓] Host Pre-Flight Selfest: 8/8 Invariant Checks Passed (Exit Code 0)
  [✓] AddressSanitizer/UBSan:  Clean Heap, 0 Leaks, 0 Undefined Behavior
  [✓] Memory Footprint:        <12.5MB Baseline Resident Set Size (RSS)
```

1. **Unit Test Suite (`make test`):** 66 test modules covering all C structures, JSON parsers, frontmatter engines, memory decay, and MCP HTTP/stdio transports.
2. **Hidden Holdout Suite (`make holdout`):** 10 scenarios (65 assertions) testing path containment, emotional state clamping, and FadeMem retention bounds.
3. **Operation Crucible (`make test-crucible-asan`):** Adversarial battery testing deep JSON recursion (depth 100/200), byte truncation, SSRF guards, and shell escapes under ASan & UBSan.
4. **Pre-Flight Self-Test Gate (`./build/almaz --selfest`):** Validates active SQLite schema, WAL journal mode, and active state rows.

---

## Native Tool Suite (46 Built-In Tools + 64 MCP Tool Cap)

| Category | Native Tools |
| :--- | :--- |
| **Filesystem & Execution** | `bash`, `read_file`, `write_file`, `edit_file`, `apply_patch`, `patch_file`, `list_dir`, `search_files`, `ripgrep`, `pty_exec` |
| **Harness & Speculation** | `dispatch_worker_task`, `query_ast_graph`, `run_test_cycle`, `generate_repo_map`, `enter_worktree`, `leave_worktree`, `plan_mode` |
| **Language Server & AST** | `lsp_definition`, `lsp_references`, `lsp_query`, `ast_slice` |
| **Cognitive Memory** | `save_memory`, `recall_memory`, `search_memory`, `save_skill`, `search_skills`, `git_checkpoint`, `git_rollback` |
| **Agent Collaboration** | `mesh_collaborate`, `ask_user`, `audit_security_variants` |
| **Network & MCP** | `fetch_url`, `rest_call`, `define_tool`, `synthesize_mcp_server`, `mcp_call` |
| **Dynamic MCP Extensions**| Discovered dynamically from `mcp_servers.json` (Playwright, Context7, Knowledge HTTP, GitHub) up to 64 tools. |

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
# 1. Primary Unit Test Battery (66 test modules + MCP HTTP selftest)
make test

# 2. Strict Memory Safety Audit under AddressSanitizer and UndefinedBehaviorSanitizer
make test_asan

# 3. Hidden Holdout Benchmark Suite (10 scenarios / 65 assertions)
make holdout

# 4. Adversarial Stress Crucible under AddressSanitizer
make test-crucible-asan

# 5. Host Database Self-Test Gate
./build/almaz --selfest
```

---

## Operating Modes

### Mode 1: Interactive Terminal TUI
```bash
./build/almaz
```

### Mode 2: 24/7 Telegram & Omnichannel Daemon
```bash
export TELEGRAM_BOT_TOKEN="<token>"
export TELEGRAM_CHAT_ID="<chat_id>"
./build/almaz --telegram
```

### Mode 3: Supervised Self-Healing Watchdog
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
**Host Environment:** Linux (x86-64 / ARM64) & macOS (Darwin POSIX)  
**Architecture:** Pure C99 POSIX | Release v7.3.0 | SOTA Dominance Architecture
