#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_CHILDREN 3

volatile sig_atomic_t sig_received = 0;

void signal_handler(int sig) {
    (void)sig;
    sig_received = 1;
}

void print_info(const char *label, pid_t pid) {
    pid_t pgid = getpgid(pid == 0 ? 0 : pid);
    pid_t sid = getsid(pid == 0 ? 0 : pid);
    printf("%-10s (PID: %-5d, PPID: %-5d, PGID: %-5d, SID: %-5d)\n",
           label, pid, getppid(), pgid, sid);
}

int main(void) {
    pid_t children[NUM_CHILDREN] = {0};

    signal(SIGTERM, signal_handler);

    setsid();

    printf("=== Process Tree ===\n");
    print_info("Parent", getpid());

    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            signal(SIGTERM, signal_handler);

            if (i == 0) {
                setpgid(0, getppid());
            } else {
                setpgid(0, 0);
            }

            printf("  ├── ");
            char label[32];
            snprintf(label, sizeof(label), "Child %d", i + 1);
            print_info(label, 0);

            while (!sig_received) {
                pause();
            }

            printf("Child %d (PID %d): received SIGTERM, exiting\n",
                   i + 1, getpid());
            exit(0);
        } else if (pid > 0) {
            children[i] = pid;
            if (i > 0) {
                setpgid(pid, pid);
            } else {
                setpgid(pid, getpid());
            }
        } else {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    sleep(1);
    printf("\n=== Group Signaling ===\n");

    printf("Parent sending SIGTERM to Child 2's group (PGID: %d)...\n",
           children[1]);
    killpg(children[1], SIGTERM);
    waitpid(children[1], NULL, 0);
    printf("Parent: Child 2 terminated\n\n");

    printf("Parent sending SIGTERM to Child 3's group (PGID: %d)...\n",
           children[2]);
    killpg(children[2], SIGTERM);
    waitpid(children[2], NULL, 0);
    printf("Parent: Child 3 terminated\n\n");

    printf("Parent sending SIGTERM to parent's group (PGID: %d)...\n",
           getpgrp());
    killpg(getpgrp(), SIGTERM);

    waitpid(children[0], NULL, 0);
    printf("Parent: Child 1 terminated\n");

    printf("Parent: all children done, exiting\n");

    return 0;
}
