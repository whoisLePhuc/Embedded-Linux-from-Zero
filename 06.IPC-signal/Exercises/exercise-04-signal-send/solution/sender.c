#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <receiver_pid>\n", argv[0]);
        return 1;
    }

    pid_t receiver_pid = (pid_t)atoi(argv[1]);
    printf("Sender PID: %d\n", getpid());
    printf("Target PID: %d\n", receiver_pid);

    printf("\n1. Sending SIGUSR1 via kill()...\n");
    if (kill(receiver_pid, SIGUSR1) < 0) {
        perror("kill");
        return 1;
    }
    sleep(1);

    printf("\n2. Sending SIGUSR2 with data via sigqueue()...\n");
    union sigval value;
    value.sival_int = 42;
    if (sigqueue(receiver_pid, SIGUSR2, value) < 0) {
        perror("sigqueue");
        return 1;
    }
    sleep(1);

    printf("\n3. Sending SIGTERM to self via raise()...\n");
    sleep(1);
    raise(SIGTERM);

    printf("This line should NOT appear.\n");
    return 0;
}
