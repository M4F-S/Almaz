CC = gcc
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    DEAD_STRIP = -Wl,-dead_strip
else
    DEAD_STRIP = -Wl,--gc-sections
endif
OPT_SIZE := $(shell $(CC) -Oz -E -x c /dev/null >/dev/null 2>&1 && echo -Oz || echo -Os)
CFLAGS = -Wall -Wextra -Werror -Wpedantic -Wshadow -Wformat=2 $(OPT_SIZE) -ffunction-sections -fdata-sections -std=c99 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700
LIBS = $(DEAD_STRIP) -lcurl -lsqlite3 -lm

SRCS = timeline_anomaly.c budget_store.c linenoise.c minijson.c minifrontmatter.c mcp_client.c mcp_http.c jev_client.c model_adapter.c belya_agent.c belya_harness.c telegram_adapter.c belya_channel.c health_watcher.c main.c
OBJS = $(SRCS:.c=.o)
BUILD_DIR = build
TARGET = $(BUILD_DIR)/almaz
DEPLOY_BIN = almaz
DEPLOY_SVC = almaz
DEPLOY_RESTART = systemctl restart $(DEPLOY_SVC)

TEST_SRCS = timeline_anomaly.c budget_store.c linenoise.c minijson.c minifrontmatter.c mcp_client.c mcp_http.c jev_client.c model_adapter.c belya_agent.c belya_harness.c telegram_adapter.c belya_channel.c health_watcher.c test_suite.c
TEST_TARGET = almaz_test
MCP_HTTP_TEST_TARGET = build/test_mcp_http

HOLDOUT_SRCS = timeline_anomaly.c budget_store.c linenoise.c minijson.c minifrontmatter.c mcp_client.c jev_client.c model_adapter.c belya_agent.c belya_harness.c telegram_adapter.c belya_channel.c health_watcher.c test_holdout.c
HOLDOUT_TARGET = almaz_holdout

BENCHMARK_SRCS = timeline_anomaly.c budget_store.c linenoise.c minijson.c minifrontmatter.c mcp_client.c mcp_http.c jev_client.c model_adapter.c belya_agent.c belya_harness.c telegram_adapter.c belya_channel.c health_watcher.c benchmark_runner.c
BENCHMARK_TARGET = almaz_benchmark

WATCHDOG_TARGET = almaz-watchdog

all: $(TARGET) $(WATCHDOG_TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

$(WATCHDOG_TARGET): watchdog.c
	$(CC) $(CFLAGS) -o $@ watchdog.c $(DEAD_STRIP) -lcurl

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_TARGET) $(MCP_HTTP_TEST_TARGET)
	./$(TEST_TARGET)
	./$(MCP_HTTP_TEST_TARGET)

$(MCP_HTTP_TEST_TARGET): mcp_http_selftest.c mcp_http.c mcp_client.c minijson.c mcp_http.h mcp_client.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ mcp_http_selftest.c mcp_http.c mcp_client.c minijson.c $(LIBS)

$(TEST_TARGET): $(TEST_SRCS)
	$(CC) $(CFLAGS) -o $@ $(TEST_SRCS) $(LIBS)

holdout: $(HOLDOUT_TARGET)
	./$(HOLDOUT_TARGET)

test-holdout: holdout

$(HOLDOUT_TARGET): $(HOLDOUT_SRCS)
	$(CC) $(CFLAGS) -o $@ $(HOLDOUT_SRCS) $(LIBS)

benchmark: $(BENCHMARK_TARGET) $(TARGET)
	./$(BENCHMARK_TARGET)

arena: benchmark

$(BENCHMARK_TARGET): $(BENCHMARK_SRCS)
	$(CC) $(CFLAGS) -o $@ $(BENCHMARK_SRCS) $(LIBS)

# deploy: publish the built artifact to the path the service actually runs, respecting the
# immutable flag, and restart ONLY when the content really changed (H3/H4).
deploy: $(TARGET)
	@set -e; \
	if [ -e "$(DEPLOY_BIN)" ] && cmp -s "$(TARGET)" "$(DEPLOY_BIN)"; then \
	  echo "[deploy] $(DEPLOY_BIN) already matches $(TARGET) - no restart needed"; \
	else \
	  if [ -e "$(DEPLOY_BIN)" ] && ! chattr -i "$(DEPLOY_BIN)" 2>/dev/null; then \
	    echo "[deploy] REFUSED: cannot clear the immutable flag on $(DEPLOY_BIN)." >&2; \
	    echo "[deploy] Publishing is an OPERATOR action: the service runs without" >&2; \
	    echo "[deploy] CAP_LINUX_IMMUTABLE, so an agent cannot swap the running binary." >&2; \
	    echo "[deploy] Nothing was changed. Run make deploy as root outside the service." >&2; \
	    exit 1; \
	  fi; \
	  echo "[deploy] installing $(TARGET) -> $(DEPLOY_BIN)"; \
	  if [ -e "$(DEPLOY_BIN)" ]; then cp -a "$(DEPLOY_BIN)" "$(DEPLOY_BIN).lkg"; fi; \
	  install -m 0755 "$(TARGET)" "$(DEPLOY_BIN).new"; \
	  mv -f "$(DEPLOY_BIN).new" "$(DEPLOY_BIN)"; \
	  chattr +i "$(DEPLOY_BIN)" 2>/dev/null || true; \
	  $(DEPLOY_RESTART); \
	  if [ -x /usr/local/libexec/almaz/deploy_meta.sh ]; then \
	    /usr/local/libexec/almaz/deploy_meta.sh >/dev/null && echo "[deploy] .deploy_meta regenerated"; \
	  fi; \
	fi

# release: the whole ritual in one command - clean build, test gate, then deploy (H4).
release:
	$(MAKE) clean
	$(MAKE) $(TARGET)
	$(MAKE) test
	$(MAKE) deploy

clean:
	# NOTE: almaz-watchdog is a DEPLOYED artifact (chattr +i, and its absence is a
	# 203/EXEC loop) - clean must never delete it. Use `make deploy` to publish it.
	-rm -rf $(BUILD_DIR)
	-rm -f $(OBJS) $(TEST_TARGET) $(HOLDOUT_TARGET) $(BENCHMARK_TARGET) test_*.sqlite* test_sample.txt bench_mem.sqlite* .belya_history .almaz_history almaz_crashes.log

gdrive:
	@./scripts/sync_to_gdrive.sh

.PHONY: all test holdout test-holdout benchmark arena clean gdrive deploy release
