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

SRCS = timeline_anomaly.c budget_store.c linenoise.c minijson.c minifrontmatter.c mcp_client.c jev_client.c model_adapter.c belya_agent.c belya_harness.c telegram_adapter.c belya_channel.c health_watcher.c main.c
OBJS = $(SRCS:.c=.o)
TARGET = almaz

TEST_SRCS = timeline_anomaly.c budget_store.c linenoise.c minijson.c minifrontmatter.c mcp_client.c jev_client.c model_adapter.c belya_agent.c belya_harness.c telegram_adapter.c belya_channel.c health_watcher.c test_suite.c
TEST_TARGET = almaz_test

HOLDOUT_SRCS = timeline_anomaly.c budget_store.c linenoise.c minijson.c minifrontmatter.c mcp_client.c jev_client.c model_adapter.c belya_agent.c belya_harness.c telegram_adapter.c belya_channel.c health_watcher.c test_holdout.c
HOLDOUT_TARGET = almaz_holdout

BENCHMARK_SRCS = timeline_anomaly.c budget_store.c linenoise.c minijson.c minifrontmatter.c mcp_client.c jev_client.c model_adapter.c belya_agent.c belya_harness.c telegram_adapter.c belya_channel.c health_watcher.c benchmark_runner.c
BENCHMARK_TARGET = almaz_benchmark

WATCHDOG_TARGET = almaz-watchdog

all: $(TARGET) $(WATCHDOG_TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

$(WATCHDOG_TARGET): watchdog.c
	$(CC) $(CFLAGS) -o $@ watchdog.c $(DEAD_STRIP) -lcurl

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

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

clean:
	rm -f $(OBJS) $(TARGET) $(TEST_TARGET) $(HOLDOUT_TARGET) $(BENCHMARK_TARGET) $(WATCHDOG_TARGET) test_*.sqlite* test_sample.txt bench_mem.sqlite* .belya_history .almaz_history almaz_crashes.log

gdrive:
	@./scripts/sync_to_gdrive.sh

.PHONY: all test holdout test-holdout benchmark arena clean gdrive
