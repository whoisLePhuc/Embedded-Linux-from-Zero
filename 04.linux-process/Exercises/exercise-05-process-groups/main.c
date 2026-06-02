#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_CHILDREN 3

/* TODO: Declare volatile sig_atomic_t flag for signal handling */

/* TODO: Implement signal handler */

/* TODO: Implement function to print process info (PID, PPID, PGID, SID) */

int main(void) {
    pid_t children[NUM_CHILDREN] = {0};

    /* TODO:
     * 1. Parent creates new session with setsid()
     * 2. Fork 3 children:
     *    - Child 1: stays in parent's process group (setpgid to parent's PID)
     *    - Child 2: creates its own process group (setpgid to its own PID)
     *    - Child 3: creates its own process group
     * 3. Each child registers a signal handler and pauses waiting for signal
     * 4. Parent sleeps briefly then demonstrates killpg():
     *    - Send SIGTERM to Child 2's group, only Child 2 should respond
     *    - Send SIGTERM to Child 3's group, only Child 3 should respond
     *    - Send SIGTERM to parent's group, Child 1 AND parent should respond
     * 5. Parent waits for all children
     */

    return 0;
}
