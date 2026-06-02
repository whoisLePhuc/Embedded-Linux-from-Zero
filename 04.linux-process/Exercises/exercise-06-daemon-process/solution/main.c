#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

#define LOG_FILE "/tmp/daemon_timestamp.log"

volatile sig_atomic_t running = 1;
volatile sig_atomic_t reload = 0;

void signal_handler(int sig) {
    if (sig == SIGTERM) {
        running = 0;
    } else if (sig == SIGHUP) {
        reload = 1;
    }
}

void daemonize(void) {
    pid_t pid;

    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    if (setsid() < 0) {
        perror("setsid");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    if (chdir("/") < 0) {
        perror("chdir");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < sysconf(_SC_OPEN_MAX); i++) {
        close(i);
    }

    open("/dev/null", O_RDWR);
    dup(0);
    dup(0);
}

int main(void) {
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGHUP, &sa, NULL);

    daemonize();

    FILE *log = fopen(LOG_FILE, "a");
    if (!log) {
        exit(EXIT_FAILURE);
    }

    while (running) {
        if (reload) {
            fclose(log);
            log = fopen(LOG_FILE, "a");
            if (!log) break;
            reload = 0;
        }

        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char buf[64];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);

        fprintf(log, "[%s] Daemon running (PID: %d)\n", buf, getpid());
        fflush(log);

        sleep(5);
    }

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
    fprintf(log, "[%s] Daemon shutting down\n", buf);
    fflush(log);

    fclose(log);
    return 0;
}
