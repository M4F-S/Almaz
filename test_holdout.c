#include "common.h"
#include "minijson.h"
#include "minifrontmatter.h"
#include "belya_agent.h"
#include "belya_harness.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

static int g_holdout_passed = 0;
static int g_holdout_total = 0;

#define HOLDOUT_ASSERT(cond, msg) do { \
    g_holdout_total++; \
    if (!(cond)) { \
        fprintf(stderr, "\033[1;31m[Holdout FAILED]\033[0m %s:%d: %s\n", __FILE__, __LINE__, msg); \
        assert(cond); \
    } else { \
        g_holdout_passed++; \
    } \
} while (0)

static void test_dyn_string_boundary(void) {
    DynString ds = dyn_str_new();
    HOLDOUT_ASSERT(ds.data != NULL, "DynString initial allocation");
    HOLDOUT_ASSERT(ds.len == 0, "DynString initial len 0");
    for (int i = 0; i < 5000; i++) {
        dyn_str_append(&ds, "0123456789ABCDEF");
    }
    HOLDOUT_ASSERT(ds.len == 5000 * 16, "DynString 80KB length check");
    HOLDOUT_ASSERT(ds.cap >= ds.len + 1, "DynString capacity invariant");
    dyn_str_free(&ds);
    HOLDOUT_ASSERT(ds.data == NULL, "DynString post-free pointer NULL");
}

static void test_json_depth_and_escaping(void) {
    const char *json_src = "{\"key\": \"val\\\"with\\nquotes\", \"nested\": {\"level2\": {\"level3\": 42}}, \"arr\": [true, false, null]}";
    JsonValue *root = json_parse(json_src);
    HOLDOUT_ASSERT(root != NULL, "MiniJSON parsed complex object");
    HOLDOUT_ASSERT(root->type == JSON_OBJECT, "MiniJSON root is object");

    JsonValue *nested = json_obj_get(root, "nested");
    HOLDOUT_ASSERT(nested != NULL && nested->type == JSON_OBJECT, "MiniJSON nested object found");
    JsonValue *l2 = json_obj_get(nested, "level2");
    HOLDOUT_ASSERT(l2 != NULL, "MiniJSON level2 found");
    JsonValue *l3 = json_obj_get(l2, "level3");
    HOLDOUT_ASSERT(l3 != NULL && l3->type == JSON_NUMBER && (int)l3->u.number == 42, "MiniJSON level3 is 42");

    char *ser = json_serialize(root);
    HOLDOUT_ASSERT(ser != NULL, "MiniJSON serialization succeeded");
    HOLDOUT_ASSERT(strstr(ser, "42") != NULL, "Serialized contains 42");
    free(ser);
    json_free(root);
}

static void test_frontmatter_delimiters(void) {
    const char *doc = "---\r\nname: test-skill\r\ndescription: A test skill\r\ntriggers:\r\n  - alpha\r\n  - beta\r\n---\r\n# Skill Markdown Content";
    Frontmatter *fm = frontmatter_parse(doc);
    HOLDOUT_ASSERT(fm != NULL, "Frontmatter with CRLF parsed");
    const char *name = frontmatter_get_scalar(fm, "name");
    HOLDOUT_ASSERT(name != NULL && strcmp(name, "test-skill") == 0, "Frontmatter name matches");
    size_t t_count = frontmatter_get_list_count(fm, "triggers");
    HOLDOUT_ASSERT(t_count == 2, "Frontmatter parsed 2 triggers");
    HOLDOUT_ASSERT(strcmp(frontmatter_get_list_item(fm, "triggers", 0), "alpha") == 0, "Frontmatter trigger 0 is alpha");
    HOLDOUT_ASSERT(strcmp(frontmatter_get_list_item(fm, "triggers", 1), "beta") == 0, "Frontmatter trigger 1 is beta");
    frontmatter_free(fm);
}

static void test_path_jailing(void) {
    char root[4096];
    if (!getcwd(root, sizeof(root))) {
        strncpy(root, ".", sizeof(root) - 1);
        root[sizeof(root) - 1] = '\0';
    }

    HOLDOUT_ASSERT(!is_path_jailed("/etc/passwd", root, false), "Jailing blocks /etc/passwd");
    HOLDOUT_ASSERT(!is_path_jailed("../outside_jail", root, false), "Jailing blocks relative traversal outside root");
    HOLDOUT_ASSERT(is_path_jailed("test_holdout.c", root, false), "Jailing allows local file");
}

static void test_self_model_lifecycle(void) {
    const char *test_db = "test_holdout_self_model.sqlite";
    unlink(test_db);

    BelyaAgent *agent = belya_agent_init(NULL, test_db, "Holdout agent");
    HOLDOUT_ASSERT(agent != NULL, "Agent initialized for self-model holdout");

    char *sm1 = almaz_agent_get_self_model(agent);
    HOLDOUT_ASSERT(sm1 != NULL, "Self-model initial retrieval succeeded");
    HOLDOUT_ASSERT(strstr(sm1, "v1") != NULL, "Self-model initial version is v1");
    free(sm1);

    bool upd = almaz_agent_update_self_model(agent, "[\"dynamic patch generation\"]", "[\"long latency\"]", "{\"turns\": 12, \"success_rate\": 0.95}");
    HOLDOUT_ASSERT(upd == true, "Self-model update succeeded");

    char *sm2 = almaz_agent_get_self_model(agent);
    HOLDOUT_ASSERT(sm2 != NULL, "Self-model updated retrieval succeeded");
    HOLDOUT_ASSERT(strstr(sm2, "v2") != NULL, "Self-model updated version is v2");
    HOLDOUT_ASSERT(strstr(sm2, "dynamic patch generation") != NULL, "Self-model updated capability recorded");
    free(sm2);

    belya_agent_free(agent);
    unlink(test_db);
}

static void test_emotional_appraisal_clamping(void) {
    BelyaAgent agent;
    memset(&agent, 0, sizeof(agent));
    agent.confidence = 0.50f;
    agent.frustration = 0.00f;

    // Run 30 consecutive failures
    for (int i = 0; i < 30; i++) {
        almaz_agent_record_appraisal(&agent, false);
    }
    HOLDOUT_ASSERT(agent.confidence >= 0.0f, "Confidence does not underflow below 0.0");
    HOLDOUT_ASSERT(agent.frustration <= 1.0f, "Frustration does not overflow above 1.0");
    HOLDOUT_ASSERT(agent.confidence == 0.0f, "Confidence clamped at 0.0 on repeated failure");
    HOLDOUT_ASSERT(agent.frustration == 1.0f, "Frustration clamped at 1.0 on repeated failure");

    // Run 30 consecutive successes
    for (int i = 0; i < 30; i++) {
        almaz_agent_record_appraisal(&agent, true);
    }
    HOLDOUT_ASSERT(agent.confidence <= 1.0f, "Confidence does not overflow above 1.0");
    HOLDOUT_ASSERT(agent.frustration >= 0.0f, "Frustration does not underflow below 0.0");
    HOLDOUT_ASSERT(agent.confidence == 1.0f, "Confidence clamped at 1.0 on repeated success");
    HOLDOUT_ASSERT(agent.frustration == 0.0f, "Frustration clamped at 0.0 on repeated success");
}

static void test_memory_gating_precision(void) {
    HOLDOUT_ASSERT(almaz_should_retrieve_memory("Do you remember our discussion yesterday?"), "Gating triggers on 'remember'");
    HOLDOUT_ASSERT(almaz_should_retrieve_memory("What was the previous decision on database schema?"), "Gating triggers on 'previous'");
    HOLDOUT_ASSERT(almaz_should_retrieve_memory("Explain your self-evolution architecture"), "Gating triggers on 'evolution'");
    HOLDOUT_ASSERT(almaz_should_retrieve_memory("Recall the API keys configuration"), "Gating triggers on 'recall'");

    HOLDOUT_ASSERT(!almaz_should_retrieve_memory("hello"), "Gating ignores greeting 'hello'");
    HOLDOUT_ASSERT(!almaz_should_retrieve_memory("good morning"), "Gating ignores greeting 'good morning'");
    HOLDOUT_ASSERT(!almaz_should_retrieve_memory("compile main.c with gcc"), "Gating ignores direct compile command");
    HOLDOUT_ASSERT(!almaz_should_retrieve_memory("cat /tmp/test.txt"), "Gating ignores direct cat command");
}

static void test_token_estimator_limits(void) {
    HOLDOUT_ASSERT(count_estimated_tokens("") == 0, "Token count of empty string is 0");
    size_t hw_tok = count_estimated_tokens("hello world");
    HOLDOUT_ASSERT(hw_tok >= 2 && hw_tok <= 3, "Token count of 2 words is within expected range (2-3)");
    char long_buf[10001];
    memset(long_buf, 'a', 10000);
    long_buf[10000] = '\0';
    size_t tok = count_estimated_tokens(long_buf);
    HOLDOUT_ASSERT(tok >= 2500, "10000 chars estimation is at least 2500 tokens");
}

static void test_circuit_breaker_resilience(void) {
    BelyaHarness h;
    memset(&h, 0, sizeof(h));

    char *alert = NULL;
    // 1st failure
    bool t1 = belya_harness_record_tool_observation(&h, "read_file", "{\"path\":\"nonexistent\"}", "Error: file not found", &alert);
    HOLDOUT_ASSERT(!t1, "1st failure does not trip breaker");
    if (alert) { free(alert); alert = NULL; }

    // 2nd failure
    bool t2 = belya_harness_record_tool_observation(&h, "read_file", "{\"path\":\"nonexistent\"}", "Error: file not found", &alert);
    HOLDOUT_ASSERT(!t2, "2nd failure does not trip breaker");
    if (alert) { free(alert); alert = NULL; }

    // 3rd failure (trips)
    bool t3 = belya_harness_record_tool_observation(&h, "read_file", "{\"path\":\"nonexistent\"}", "Error: file not found", &alert);
    HOLDOUT_ASSERT(t3, "3rd consecutive failure trips circuit breaker");
    HOLDOUT_ASSERT(alert != NULL, "Circuit breaker produces intervention message");
    if (alert) { free(alert); alert = NULL; }

    // Succeeded tool resets breaker
    bool t4 = belya_harness_record_tool_observation(&h, "read_file", "{\"path\":\"real.c\"}", "int main() {}", &alert);
    HOLDOUT_ASSERT(!t4, "Success does not trip breaker");
    HOLDOUT_ASSERT(h.consecutive_tool_failures == 0, "Breaker consecutive failures reset to 0 on success");
    if (alert) { free(alert); alert = NULL; }
}

int main(void) {
    printf("\n================ Running Almaz Hidden Holdout Benchmark Suite ================\n");
    test_dyn_string_boundary();
    printf("  [Holdout 1/9] DynString Boundaries: PASSED\n");

    test_json_depth_and_escaping();
    printf("  [Holdout 2/9] MiniJSON Depth & Escaping: PASSED\n");

    test_frontmatter_delimiters();
    printf("  [Holdout 3/9] YAML Frontmatter CRLF Delimiters: PASSED\n");

    test_path_jailing();
    printf("  [Holdout 4/9] Workspace Path Jailing: PASSED\n");

    test_self_model_lifecycle();
    printf("  [Holdout 5/9] SARSI Self-Model Lifecycle: PASSED\n");

    test_emotional_appraisal_clamping();
    printf("  [Holdout 6/9] Emotional Appraisal Dynamic Clamping: PASSED\n");

    test_memory_gating_precision();
    printf("  [Holdout 7/9] KnowSelf Memory Gating Precision: PASSED\n");

    test_token_estimator_limits();
    printf("  [Holdout 8/9] Token Estimator Limits: PASSED\n");

    test_circuit_breaker_resilience();
    printf("  [Holdout 9/9] Metacognitive Circuit Breaker Resilience: PASSED\n");

    printf("================ Holdout Benchmark Passed: %d/%d (100%%) ================\n\n", g_holdout_passed, g_holdout_total);
    return 0;
}
