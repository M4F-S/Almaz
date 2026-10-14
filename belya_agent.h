#ifndef BELYA_AGENT_H
#define BELYA_AGENT_H

#include "model_adapter.h"
#include "jev_client.h"
#include <sqlite3.h>

typedef struct {
    char *role;
    char *content;
    char *tool_call_id;
    ModelParsedToolCall *tool_calls;
    size_t tool_call_count;
} BelyaMessage;

typedef struct {
    char *name;
    char *description;
    JsonValue *parameters_schema;
} BelyaToolSchema;

typedef struct BelyaAgent {
    ModelGateway *gateway;
    JevClient *jev;
    sqlite3 *db;
    BelyaMessage *messages;
    size_t msg_count;
    size_t msg_cap;
    BelyaToolSchema *schemas;
    size_t schema_count;
    size_t max_context_messages;
    size_t max_context_tokens;
    bool has_fts5;
    size_t turn_count;          /* Total steps taken */
    size_t auto_save_interval;  /* Auto-save every N turns (0=disabled) */
    size_t turns_since_save;    /* Turns since last auto-save */
    size_t compaction_percent;  /* Token budget % that triggers compaction (default 80) */
    size_t compaction_keep;     /* Messages to keep after compaction (default 10) */
    size_t total_prompt_tokens;
    size_t total_completion_tokens;
    size_t total_cached_tokens;
    float confidence;           /* Emotional Appraisal: 0.0 to 1.0 (default 0.70) */
    float frustration;          /* Emotional Appraisal: 0.0 to 1.0 (default 0.00) */
    size_t lifetime_turns;      /* Cumulative lifetime turns across all sessions */
    size_t lifetime_successes;  /* Successful tool outcomes across lifetime */
    size_t lifetime_failures;   /* Failed tool outcomes across lifetime */
    size_t lifetime_tool_calls; /* Total tool calls executed across lifetime */
    float recent_form;          /* M2.1/R6(a): measured EWMA of real outcomes, 1.0 = in form */
    size_t recent_form_samples; /* Observations folded into recent_form */
    char db_path[256];
    void (*pre_compact_hook)(void *userdata);
    void *pre_compact_userdata;
    void *user_data;
} BelyaAgent;

// SARSI Persistent Self-Model (Organism Introspection)
char *almaz_agent_get_self_model(BelyaAgent *agent);
bool almaz_agent_update_self_model(BelyaAgent *agent, const char *capabilities, const char *weaknesses, const char *performance_stats);
bool almaz_agent_sync_self_model(BelyaAgent *agent);

// Emotional Appraisal (VIGIL EmoBank State Machine)
void almaz_agent_record_appraisal(BelyaAgent *agent, bool success);

/* M2.1 / R6(a): measured "recent form" — a windowed, evidence-based view of how
   the agent is ACTUALLY doing, deliberately SEPARATE from confidence/frustration
   whose saturating-clamp semantics are asserted by both test_suite.c and the
   hidden holdout (see the reverted R6/EWMA attempt). These are the values the
   escalation gates use; the clamped pair stays exactly as specified. */
void almaz_agent_recent_form_observe(BelyaAgent *agent, float observation);
void almaz_agent_recent_form_decay(BelyaAgent *agent);
float almaz_agent_distress(const BelyaAgent *agent);

BelyaAgent *belya_agent_init(ModelGateway *gw, const char *db_path, const char *system_instructions);
void belya_agent_register_schema(BelyaAgent *agent, const char *name, const char *desc, JsonValue *params);
void belya_agent_add_message(BelyaAgent *agent, const char *role, const char *content);
void belya_agent_add_tool_result(BelyaAgent *agent, const char *tool_call_id, const char *name, const char *result);
void belya_agent_persist_memory(BelyaAgent *agent, const char *topic, const char *content);
void belya_agent_persist_memory_scoped(BelyaAgent *agent, const char *topic, const char *content, const char *wing, const char *room);
char *belya_agent_search_memory(BelyaAgent *agent, const char *query);

// Late-2026 Phase 2: FadeMem Biological Memory & Semantics
double almaz_fademem_retention(time_t now, time_t last_acc, int access_count, double salience, double lambda);
size_t almaz_fademem_prune_sweep(BelyaAgent *agent, double min_retention);
size_t almaz_concept_fusion_sweep(BelyaAgent *agent);

// Late-2026 Phase 2: CORAL Dual-Stream Memory Routing
void almaz_coral_record_tactical(BelyaAgent *agent, const char *task_id, const char *trace_type, const char *summary, const char *details);
bool almaz_coral_promote_strategic(BelyaAgent *agent, const char *rubric_key, const char *failure_mode, const char *trigger_pattern, const char *root_cause, const char *guideline, const char *source_task);
char *almaz_coral_query_strategic(BelyaAgent *agent, const char *pattern);

// Late-2026 Phase 2: SimpleMem De-linearized Semantic Facts
void almaz_simplemem_record_fact(BelyaAgent *agent, const char *entity_key, const char *fact_text, int turn);
char *almaz_simplemem_query_facts(BelyaAgent *agent, const char *entity_key, const char *query);

void belya_agent_compact_history(BelyaAgent *agent, size_t keep_recent);
void belya_agent_compact_history_middle_out(BelyaAgent *agent, size_t keep_recent);
void belya_agent_clear_history(BelyaAgent *agent);

#define SYSTEM_PROMPT_DYNAMIC_BOUNDARY "\n\n=== Dynamic Session State ===\n"

// 2026 Competitive Parity Phase 2: 5-Tier Compactor & Lossless Archival
typedef enum {
    COMPACT_TIER_1_MICRO = 1,
    COMPACT_TIER_2_TRUNCATE_TOOLS,
    COMPACT_TIER_3_MIDDLE_OUT,
    COMPACT_TIER_4_FTS5_EVICT,
    COMPACT_TIER_5_EMERGENCY
} CompactTier;

void belya_agent_compact_5tier(BelyaAgent *agent, CompactTier tier, size_t keep_recent);

typedef void (*BelyaPreCompactHookFn)(void *userdata);
void belya_agent_set_pre_compact_hook(BelyaAgent *agent, BelyaPreCompactHookFn fn, void *userdata);

// Token Budgeting & Estimation
size_t belya_agent_total_tokens(const BelyaAgent *agent);
double belya_get_current_rss_mb(void);

// Gomaa Memory Timeline
void belya_agent_log_timeline(BelyaAgent *agent, const char *event_type, const char *summary);
char *belya_agent_get_timeline(BelyaAgent *agent, int limit);

// Auto-Save Configuration
void belya_agent_set_auto_save_interval(BelyaAgent *agent, size_t interval);

// Session Management
bool belya_agent_save_session(BelyaAgent *agent, const char *session_id, const char *title);
bool belya_agent_load_session(BelyaAgent *agent, const char *session_id);
char *belya_agent_list_sessions(BelyaAgent *agent);

// Skill Auto-Distillation & Trajectory Reflection
char *belya_agent_reflect_and_distill(BelyaAgent *agent);

// Skill Curation & Progressive Disclosure (Procedural Memory)
bool belya_agent_save_skill(BelyaAgent *agent, const char *name, const char *trigger, const char *desc, const char *instructions);
size_t belya_agent_load_disk_skills(BelyaAgent *agent, const char *skills_dir);
size_t belya_agent_load_rule_packs(BelyaAgent *agent, const char *rules_dir, DynString *out_rules);
char *belya_agent_search_skills(BelyaAgent *agent, const char *query);
char *belya_agent_get_skills_manifest(BelyaAgent *agent);
char *belya_agent_match_skill_for_prompt(BelyaAgent *agent, const char *user_prompt);

// Git Checkpointing & Instant Rollback
bool belya_agent_create_checkpoint(BelyaAgent *agent, const char *label);
bool belya_agent_rollback_to_checkpoint(BelyaAgent *agent, const char *checkpoint_id);
char *belya_agent_list_checkpoints(BelyaAgent *agent);

// Trajectory Exporter (OpenAI Fine-Tune JSONL Format)
bool belya_agent_export_trajectory(BelyaAgent *agent, const char *session_id, const char *out_path);

// Historical Conversation Memory Search
char *belya_agent_search_conversations(BelyaAgent *agent, const char *query);

// Declarative Agent Manifest Protocol (Belya Agency)
typedef struct {
    char *name;
    char *role;
    char *tools;          /* Comma-separated whitelist of allowed tools */
    char *model;
    int max_turns;
    char *instructions;
} BelyaAgentManifest;

size_t belya_agent_load_manifests(BelyaAgent *agent, const char *agents_dir);
bool belya_agent_save_manifest(BelyaAgent *agent, const char *name, const char *role, const char *tools, const char *model, int max_turns, const char *instructions);
BelyaAgentManifest *belya_agent_get_manifest(BelyaAgent *agent, const char *name);
BelyaAgentManifest *belya_agent_route_manifest(BelyaAgent *agent, const char *query);
void belya_agent_manifest_free(BelyaAgentManifest *manifest);
char *belya_agent_list_manifests(BelyaAgent *agent);

// Phase 3: Relational Task Board, Cron Engine & Memory Dream Sweep
int64_t belya_agent_task_create(BelyaAgent *agent, const char *subject, const char *description, const char *dependencies, const char *assigned_agent);
bool belya_agent_task_update(BelyaAgent *agent, int64_t task_id, const char *status, const char *description);
char *belya_agent_task_list(BelyaAgent *agent, const char *status_filter);
char *belya_agent_task_get(BelyaAgent *agent, int64_t task_id);

int64_t belya_agent_cron_create(BelyaAgent *agent, const char *name, const char *schedule_cron, const char *prompt);
char *belya_agent_cron_list(BelyaAgent *agent);
int belya_agent_cron_tick(BelyaAgent *agent, DynString *out_logs);

char *belya_agent_dream_sweep(BelyaAgent *agent);

ModelGatewayResponse belya_agent_step(BelyaAgent *agent);
ModelGatewayResponse belya_agent_step_forced_text(BelyaAgent *agent, const char *instruction);
void belya_agent_free(BelyaAgent *agent);

#endif
