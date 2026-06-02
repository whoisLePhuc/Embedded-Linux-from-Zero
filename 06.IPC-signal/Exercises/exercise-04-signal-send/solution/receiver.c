#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/*
 * Signal handler with siginfo_t to get sender info
 */
void signal_handler(int signum, siginfo_t *info, void *context) {
    printf("\n--- Received signal ---\n");
    printf("Signal number: %d", signum);

    if (signum == SIGUSR1)
        printf(" (SIGUSR1)\n");
    else if (signum == SIGUSR2)
        printf(" (SIGUSR2)\n");
    else
        printf("\n");

    printf("Sender PID   : %d\n", info->si_pid);

    if (signum == SIGUSR2) {
        printf("Data value   : %d\n", info->si_value.sival_int);
    }

    fflush(stdout);
    (void)context;
}

int main() {
    struct sigaction sa;

    printf("Receiver PID: %d\n", getpid());

    sa.sa_sigaction = signal_handler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) < 0) {
        perror("sigaction SIGUSR1");
        return 1;
    }

    if (sigaction(SIGUSR2, &sa, NULL) < 0) {
        perror("sigaction SIGUSR2");
        return 1;
    }

    printf("Waiting for signals...\n");

    while (1) {
        pause();
    }

    return 0;
}
