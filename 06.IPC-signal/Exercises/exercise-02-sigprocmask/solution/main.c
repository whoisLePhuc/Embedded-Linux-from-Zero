#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main() {
    sigset_t new_set, old_set, pending_set;

    printf("My PID: %d\n", getpid());

    // 1. Initialize signal set and add SIGINT
    sigemptyset(&new_set);
    sigaddset(&new_set, SIGINT);

    // 2. Block SIGINT
    printf("Blocking SIGINT for 5 seconds. Try pressing Ctrl+C now...\n");
    if (sigprocmask(SIG_BLOCK, &new_set, &old_set) < 0) {
        perror("sigprocmask block");
        return 1;
    }

    // 3. Sleep simulating critical work
    sleep(5);

    // 4. Check for pending signals
    sigemptyset(&pending_set);
    if (sigpending(&pending_set) == 0) {
        if (sigismember(&pending_set, SIGINT)) {
            printf("\nSignal SIGINT is pending!\n");
        }
    } else {
        perror("sigpending");
    }

    // 5. Unblock SIGINT (Restore old mask)
    printf("Unblocking SIGINT...\n");
    // Note: If SIGINT is pending, it will be delivered immediately HERE.
    // The default action for SIGINT is terminate, so the program will exit here if Ctrl+C was pressed.
    if (sigprocmask(SIG_SETMASK, &old_set, NULL) < 0) {
        perror("sigprocmask restore");
        return 1;
    }

    printf("Program finished normally.\n");
    return 0;
}
