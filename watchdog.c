#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <curl/curl.h>

static volatile sig_atomic_t g_keep_running = 1;
static pid_t g_child_pid = 0;

static void watchdog_sig_handler(int sig) {
    g_keep_running = 0;
    if (g_child_pid > 0) {
        kill(g_child_pid, sig);
    }
}

static void log_crash_event(int exit_code, int term_sig) {
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

static void notify_telegram_crash(const char *bot_token, const char *chat_id, int exit_code, int term_sig, int restart_count) {
    if (!bot_token || !chat_id || strlen(bot_token) == 0 || strlen(chat_id) == 0) return;

    CURL *curl = curl_easy_init();
    if (!curl) return;

    char url[512];
    snprintf(url, sizeof(url), "https://api.telegram.org/bot%s/sendMessage", bot_token);

    char reason[128];
    if (term_sig > 0) {
        snprintf(reason, sizeof(reason), "Killed by signal %d (%s)", term_sig, strsignal(term_sig));
    } else {
        snprintf(reason, sizeof(reason), "Exit code %d", exit_code);
    }

    char body[1024];
    snprintf(body, sizeof(body),
             "{\"chat_id\":\"%s\",\"text\":\"⚠️ *[Almaz Sibling Watchdog Alert]*\\n"
             "Primary organism process terminated unexpectedly.\\n\\n"
             "• *Reason:* `%s`\\n"
             "• *Restarts:* `%d`\\n"
             "• *Action:* Automatic self-healing restart triggered.\",\"parse_mode\":\"Markdown\"}",
             chat_id, reason, restart_count);

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

int main(int argc, char **argv) {
    signal(SIGINT, watchdog_sig_handler);
    signal(SIGTERM, watchdog_sig_handler);

    printf("\033[1;36m=== Almaz Sibling Watchdog (VIGIL Runtime) Online ===\033[0m\n");
    printf("[Watchdog] Monitoring target binary: ./almaz\n");

    int restart_count = 0;
    int backoff_sec = 1;
    time_t last_crash_time = 0;

    const char *tg_token = getenv("TELEGRAM_BOT_TOKEN");
    const char *tg_chat = getenv("TELEGRAM_CHAT_ID");

    char **child_argv = calloc(argc + 1, sizeof(char *));
    if (!child_argv) {
        fprintf(stderr, "[Watchdog Fatal] Memory allocation failure for child arguments\n");
        return 1;
    }
    child_argv[0] = "./almaz";
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
            // Child process
            execv("./almaz", child_argv);
            perror("[Watchdog] execv ./almaz failed");
            _exit(127);
        }

        // Parent process
        g_child_pid = pid;
        int status = 0;
        pid_t waited = waitpid(pid, &status, 0);
        g_child_pid = 0;

        if (!g_keep_running) {
            printf("\n[Watchdog] Termination signal received. Exiting.\n");
            break;
        }

        if (waited == pid) {
            if (WIFEXITED(status)) {
                int exit_code = WEXITSTATUS(status);
                if (exit_code == 0) {
                    printf("[Watchdog] Almaz daemon terminated cleanly (exit 0). Exiting watchdog.\n");
                    break;
                } else {
                    restart_count++;
                    time_t now = time(NULL);
                    if (last_crash_time > 0 && (now - last_crash_time) > 300) {
                        backoff_sec = 1; // Reset backoff if previous crash was over 5 mins ago
                    }
                    last_crash_time = now;

                    printf("\033[1;31m[Watchdog Alert] Almaz exited with error code %d (restart #%d).\033[0m\n", exit_code, restart_count);
                    log_crash_event(exit_code, 0);
                    notify_telegram_crash(tg_token, tg_chat, exit_code, 0, restart_count);

                    printf("[Watchdog] Backing off %d seconds before respawning...\n", backoff_sec);
                    sleep(backoff_sec);
                    backoff_sec = (backoff_sec < 60) ? (backoff_sec * 2) : 60;
                }
            } else if (WIFSIGNALED(status)) {
                int term_sig = WTERMSIG(status);
                restart_count++;
                time_t now = time(NULL);
                if (last_crash_time > 0 && (now - last_crash_time) > 300) {
                    backoff_sec = 1;
                }
                last_crash_time = now;

                printf("\033[1;31m[Watchdog Alert] Almaz killed by signal %d (%s) (restart #%d).\033[0m\n",
                       term_sig, strsignal(term_sig), restart_count);
                log_crash_event(0, term_sig);
                notify_telegram_crash(tg_token, tg_chat, 0, term_sig, restart_count);

                printf("[Watchdog] Backing off %d seconds before respawning...\n", backoff_sec);
                sleep(backoff_sec);
                backoff_sec = (backoff_sec < 60) ? (backoff_sec * 2) : 60;
            }
        }
    }

    free(child_argv);
    return 0;
}
