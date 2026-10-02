#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <curl/curl.h>

static volatile sig_atomic_t g_keep_running = 1;
static pid_t g_child_pid = 0;
static time_t g_last_crash_notify_ts = 0; /* rate limit Telegram crash spam */

#define DEPLOY_STATE_FILE ".deploy_state"
#define CRASH_WINDOW_SEC 60
#define CRASH_THRESHOLD 3
#define STATE_VAL_MAX 250

static char g_active_bin[STATE_VAL_MAX + 8] = "./almaz";
static char g_lkg_bin[STATE_VAL_MAX + 8] = "./almaz";
static char g_staging_bin[STATE_VAL_MAX + 8] = "./almaz.B";

static void trim_crlf(char *s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r' || s[n - 1] == ' ' || s[n - 1] == '\t')) {
        s[--n] = '\0';
    }
}

static void load_deploy_state(void) {
    FILE *fp = fopen(DEPLOY_STATE_FILE, "r");
    if (!fp) return;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "ACTIVE=", 7) == 0) {
            char v[STATE_VAL_MAX + 1];
            snprintf(v, sizeof(v), "%.*s", STATE_VAL_MAX, line + 7);
            trim_crlf(v);
            if (v[0] != '\0' && !strchr(v, '/') && !strstr(v, "..")) snprintf(g_active_bin, sizeof(g_active_bin), "./%s", v);
        } else if (strncmp(line, "LKG=", 4) == 0) {
            char v[STATE_VAL_MAX + 1];
            snprintf(v, sizeof(v), "%.*s", STATE_VAL_MAX, line + 4);
            trim_crlf(v);
            /* validate: no path separators, no '..' — never exec an
               arbitrary path from a file a compromised evolution could write */
            if (v[0] != '\0' && !strchr(v, '/') && !strstr(v, "..")) snprintf(g_lkg_bin, sizeof(g_lkg_bin), "./%s", v);
        } else if (strncmp(line, "STAGING=", 8) == 0) {
            char v[STATE_VAL_MAX + 1];
            snprintf(v, sizeof(v), "%.*s", STATE_VAL_MAX, line + 8);
            trim_crlf(v);
            if (v[0] != '\0' && !strchr(v, '/') && !strstr(v, "..")) snprintf(g_staging_bin, sizeof(g_staging_bin), "./%s", v);
        }
    }
    fclose(fp);
}

static void write_deploy_state(void) {
    FILE *fp = fopen(DEPLOY_STATE_FILE, "w");
    if (!fp) return;
    fprintf(fp, "ACTIVE=%s\nLKG=%s\nSTAGING=%s\n",
            g_active_bin + 2, g_lkg_bin + 2, g_staging_bin + 2);
    fflush(fp);
    fclose(fp);
}

static void watchdog_sig_handler(int sig) {
    g_keep_running = 0;
    if (g_child_pid > 0) {
        kill(g_child_pid, sig);
    }
}

static void log_crash_event(int exit_code, int term_sig) {
    /* bounded log: truncate at 1 MB (Kimi K3 P1: unbounded crash log growth) */
    struct stat st;
    if (stat("almaz_crashes.log", &st) == 0 && st.st_size > 1048576) {
        rename("almaz_crashes.log", "almaz_crashes.log.1");
    }
    FILE *fp = fopen("almaz_crashes.log", "a");
    if (!fp) return;
    time_t now = time(NULL);
    char tbuf[64];
    strftime(tbuf, sizeof(tbuf), "%Y-%m-%d %H:%M:%S UTC", gmtime(&now));
    if (term_sig > 0) {
        fprintf(fp, "[%s] CRASH: Almaz killed by signal %d (%s)\n", tbuf, term_sig, strsignal(term_sig));
    } else {
        fprintf(fp, "[%s] CRASH: Almaz exited with status code %d\n", tbuf, exit_code);
    }
    fclose(fp);
}

static void notify_telegram(const char *bot_token, const char *chat_id, const char *text) {
    if (!bot_token || !chat_id || strlen(bot_token) == 0 || strlen(chat_id) == 0) return;
    CURL *curl = curl_easy_init();
    if (!curl) return;
    char url[512];
    snprintf(url, sizeof(url), "https://api.telegram.org/bot%s/sendMessage", bot_token);
    char body[2048];
    snprintf(body, sizeof(body),
             "{\"chat_id\":\"%s\",\"text\":\"%s\",\"parse_mode\":\"Markdown\"}", chat_id, text);
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}

static void notify_telegram_crash(const char *bot_token, const char *chat_id, int exit_code, int term_sig, int restart_count) {
    char reason[128];
    if (term_sig > 0) {
        snprintf(reason, sizeof(reason), "Killed by signal %d (%s)", term_sig, strsignal(term_sig));
    } else {
        snprintf(reason, sizeof(reason), "Exit code %d", exit_code);
    }
    char text[1400];
    snprintf(text, sizeof(text),
             "⚠️ *[Almaz Sibling Watchdog Alert]*\\n"
             "Primary organism process terminated unexpectedly.\\n\\n"
             "• *Reason:* `%s`\\n"
             "• *Restarts:* `%d`\\n"
             "• *Action:* Automatic self-healing restart triggered.",
             reason, restart_count);
    notify_telegram(bot_token, chat_id, text);
}

int main(int argc, char **argv) {
    signal(SIGINT, watchdog_sig_handler);
    signal(SIGTERM, watchdog_sig_handler);

    load_deploy_state();

    printf("\033[1;36m=== Almaz Sibling Watchdog (VIGIL Runtime) Online ===\033[0m\n");
    printf("[Watchdog] Active binary: %s | LKG: %s | Staging: %s\n", g_active_bin, g_lkg_bin, g_staging_bin);

    int restart_count = 0;
    int backoff_sec = 1;
    time_t last_crash_time = 0;
    time_t crash_ring[CRASH_THRESHOLD] = {0};
    int crash_count = 0;

    const char *tg_token = getenv("TELEGRAM_BOT_TOKEN");
    const char *tg_chat = getenv("TELEGRAM_CHAT_ID");

    char **child_argv = calloc(argc + 1, sizeof(char *));
    if (!child_argv) {
        fprintf(stderr, "[Watchdog Fatal] Memory allocation failure for child arguments\n");
        return 1;
    }
    child_argv[0] = g_active_bin;
    for (int i = 1; i < argc; i++) {
        child_argv[i] = argv[i];
    }
    child_argv[argc] = NULL;

    while (g_keep_running) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("[Watchdog] fork failed");
            sleep(5);
            continue;
        }

        if (pid == 0) {
            execv(g_active_bin, child_argv);
            perror("[Watchdog] execv failed");
            _exit(127);
        }

        g_child_pid = pid;
        int status = 0;
        pid_t waited = waitpid(pid, &status, 0);
        g_child_pid = 0;

        if (!g_keep_running) {
            printf("\n[Watchdog] Termination signal received. Exiting.\n");
            break;
        }

        if (waited != pid) {
            continue;
        }

        int failed = 0;
        int exit_code = 0;
        int term_sig = 0;
        if (WIFEXITED(status)) {
            exit_code = WEXITSTATUS(status);
            if (exit_code == 0) {
                printf("[Watchdog] Almaz daemon terminated cleanly (exit 0). Exiting watchdog.\n");
                break;
            }
            failed = 1;
        } else if (WIFSIGNALED(status)) {
            term_sig = WTERMSIG(status);
            failed = 1;
        }
        if (!failed) continue;

        restart_count++;
        time_t now = time(NULL);
        if (last_crash_time > 0 && (now - last_crash_time) > 300) {
            backoff_sec = 1;
            crash_count = 0;
        }
        last_crash_time = now;

        printf("\033[1;31m[Watchdog Alert] Almaz terminated (exit %d, sig %d) (restart #%d).\033[0m\n",
               exit_code, term_sig, restart_count);
        log_crash_event(exit_code, term_sig);
        /* rate-limit Telegram crash spam: at most one alert per 5 min */
        if (now - g_last_crash_notify_ts >= 300) {
            notify_telegram_crash(tg_token, tg_chat, exit_code, term_sig, restart_count);
            g_last_crash_notify_ts = now;
        }

        // Crash-loop circuit breaker: CRASH_THRESHOLD crashes within CRASH_WINDOW_SEC
        if (crash_count == CRASH_THRESHOLD) {
            memmove(crash_ring, crash_ring + 1, (size_t)(CRASH_THRESHOLD - 1) * sizeof(time_t));
            crash_count--;
        }
        crash_ring[crash_count++] = now;

        if (crash_count == CRASH_THRESHOLD && (now - crash_ring[0]) <= CRASH_WINDOW_SEC) {
            printf("\033[1;35m[Watchdog] Crash-loop detected (%d crashes within %ds). "
                   "Falling back to LKG binary: %s\033[0m\n",
                   CRASH_THRESHOLD, CRASH_WINDOW_SEC, g_lkg_bin);
            char alert[1600];
            snprintf(alert, sizeof(alert),
                     "🚨 *[Almaz Crash-Loop Breaker]*\\n"
                     "Primary organism crashed %d times in %ds.\\n"
                     "• *Action:* Falling back to last-known-good binary `%s`.\\n"
                     "• *Backoff:* 300s.",
                     CRASH_THRESHOLD, CRASH_WINDOW_SEC, g_lkg_bin);
            notify_telegram(tg_token, tg_chat, alert);

            if (strcmp(g_active_bin, g_lkg_bin) != 0) {
                snprintf(g_active_bin, sizeof(g_active_bin), "%s", g_lkg_bin);
                write_deploy_state();
                printf("[Watchdog] Active slot updated to %s\n", g_active_bin);
            }
            backoff_sec = 300;
            crash_count = 0;
        }

        printf("[Watchdog] Backing off %d seconds before respawning...\n", backoff_sec);
        sleep(backoff_sec);
        /* Keep breaker backoff (>=300s) until the child proves stable; a
           fresh 300s survival resets it in the crash handler above. */
        if (backoff_sec < 300) {
            backoff_sec = (backoff_sec < 60) ? (backoff_sec * 2) : 60;
        }
    }

    free(child_argv);
    return 0;
}