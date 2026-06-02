#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

volatile sig_atomic_t child_exit_count = 0;

/*
 * SIGCHLD handler - reaps all terminated children
 */
void sigchld_handler(int signum) {
    pid_t pid;
    int status;

    /* Loop to reap ALL children that have terminated.
     * Multiple children may terminate at the same time,
     * but SIGCHLD may be coalesced into one delivery. */
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        child_exit_count++;
        printf("[Parent] Reaped child PID %d (exit status: %d)\n",
               pid, WEXITSTATUS(status));
    }

    (void)signum;
}

int main() {
    struct sigaction sa;

    printf("Parent PID: %d\n", getpid());

    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NOCLDSTOP | SA_RESTART;

    if (sigaction(SIGCHLD, &sa, NULL) < 0) {
        perror("sigaction SIGCHLD");
        return 1;
    }

    /* Fork 3 children with different lifetimes */
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            printf("[Child %d] PID %d starting, will exit in %d seconds\n",
                   i + 1, getpid(), (i + 1) * 2);
            sleep((i + 1) * 2);
            printf("[Child %d] PID %d exiting now\n", i + 1, getpid());
            exit((i + 1) * 10);

        } else if (pid > 0) {
            printf("[Parent] Forked child %d with PID %d\n", i + 1, pid);
        } else {
            perror("fork");
            return 1;
        }
    }

    /* Parent waits until all children are reaped */
    printf("[Parent] Waiting for all children to finish...\n");
    while (child_exit_count < 3) {
        printf("[Parent] Working... (reaped %d/3 children)\n",
               child_exit_count);
        sleep(1);
    }

    printf("[Parent] All children reaped. No zombies! Total reaped: %d\n",
           child_exit_count);
    return 0;
}
