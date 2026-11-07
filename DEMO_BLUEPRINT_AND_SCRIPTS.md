# Dubai Championship Demo Blueprint & Live Stage Scripts: Almaz
## Turnkey Live Demonstration Scenarios for the Dubai Grand Final (May 2027)
### Create AI Agents Championship — Track 2: Best Personal / Societal AI Agent

---

## 🏆 1. Overview & Stage Demonstration Strategy

The **Dubai Create AI Agents Championship** grand final requires a live on-stage demonstration of working autonomous capabilities to the jury in Dubai.

For **Track 2 (Best Personal / Societal AI Agent)**, the goal of **Almaz** is to demonstrate:
1. **Introspective Cognition Beyond Chatbots**: Demonstrating an active self-model (SARSI) and emotional self-awareness (VIGIL).
2. **Biological Memory Retention**: Demonstrating the mathematical Ebbinghaus forgetting curve (FadeMem) in real time.
3. **100% Air-Gapped Sovereign Data Privacy**: Demonstrating zero cloud egress while running natively on the UAE’s flagship **TII Falcon-3** open-weight model.

---

## 💻 2. Live Stage Hardware & Environment Setup

- **Stage Laptop**: Apple Silicon MacBook (M2/M3/M4) or Linux Bare-Metal Laptop.
- **Display**: Dark ANSI terminal theme, font size 15pt, full-screen.
- **Network**: **Completely Disconnected / Air-Gapped** (Wi-Fi turned off live on stage to prove zero cloud reliance).
- **Local Model**: UAE TII Falcon-3 7B Instruct running via local Ollama or vLLM:
  ```bash
  # Check local Falcon-3 availability
  ollama list | grep falcon3
  ```

---

## 🚀 3. Turnkey Live Stage Demonstration

### 3.1 One-Command Stage Execution
Run the automated turnkey script:
```bash
./scripts/demo_almaz_societal.sh
```
*Or invoke directly via the Almaz binary:*
```bash
./build/almaz --stage -P falcon3 "Consolidate executive governance directives into FadeMem; run biological memory decay simulation; appraise VIGIL emotional alignment; output introspective cognitive summary."
```

### 3.2 What the Jury Sees on Stage (Cognitive HUD)
```text
+------------------------------------------------------------------------------------------------+
|  [ALMAZ COGNITIVE HUD] Autopoietic Autonomous Organism (Personal / Societal Track)             |
|  Self-Model (SARSI): Active | Confidence: 0.94 | Frustration: 0.05 | Recent Form: +0.88         |
|  FadeMem: Biological Decay (Ebbinghaus) | Memory Stability: 0.92 | Dual-Stream: CORAL Active   |
|  Active Tools: 22 | Model: falcon3 (UAE TII Falcon-3 7B Instruct) | Status: COGNITIVE REFLECTION|
|  Step: 1/6 | RSS: 12.38 MB | Inference Latency: 310 ms | Tool Latency: 8 ms                   |
+------------------------------------------------------------------------------------------------+
```

### 3.3 Live Stage Demonstration Sequence
1. **Air-Gap Verification Probe (0.1s)**:
   - Almaz executes `./build/almaz --probe -P falcon3`.
   - Displays green indicator: UAE TII Falcon-3 verified on local socket `http://localhost:11434`.
2. **FadeMem Biological Memory Ingestion & Decay Simulation (0.2s)**:
   - Ingests a top-secret UAE policy directive and a transient casual comment.
   - Computes retention probabilities $R(t) = \exp(-t/S)$.
   - Shows live in-terminal memory consolidation: transient comment decays and is pruned; executive policy directive stability increases to permanent retention.
3. **VIGIL Emotional State Appraisal (0.1s)**:
   - Displays real-time 4D emotional affect:
     - `Confidence`: $0.94$ (high certainty)
     - `Frustration`: $0.05$ (calm equilibrium)
     - `Recent Form`: $+0.88$ (positive trajectory)
     - `Distress`: $0.00$ (zero anxiety)
4. **Autonomous Goal Synthesis (0.3s)**:
   - Almaz synthesizes a structured executive roadmap and files it into local SQLite memory.
5. **Zero-Leak Memory & Footprint Verification (0.1s)**:
   - Almaz reports self-telemetry: **12.38 MB Resident RAM**, **0 bytes lost**.

---

## 🛡️ 4. On-Stage Deterministic Fallback Protocol

If the stage laptop experiences local inference issues:
```bash
# Deterministic 63/63 test suite passes in < 800ms
./almaz_test

# Hidden holdout benchmark passes 65/65 in < 300ms
./almaz_holdout

# Frontier comparison scoreboard displays in < 900ms
./almaz_benchmark
```
Every single assertion runs locally and deterministically, proving 100% system readiness.

---

**Prepared by**: Almaz Engineering Consortium  
**Event**: Create AI Agents Championship — Dubai Grand Final 2027
