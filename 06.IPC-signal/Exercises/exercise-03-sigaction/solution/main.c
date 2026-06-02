#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/*
 * Advanced Signal Handler using sa_sigaction
 */
void sigusr1_handler(int signum, siginfo_t *info, void *context) {
    // printf is not safe here, but used for demo purposes.
    printf("\nCaught signal %d (SIGUSR1)\n", signum);
    
    // Check if info is NULL (though it shouldn't be with SA_SIGINFO)
    if (info != NULL) {
        printf("Signal sent from Process ID: %d\n", info->si_pid);
        printf("User ID of sender: %d\n", info->si_uid);
    }
    
    (void)context; // Unused
}

int main() {
    struct sigaction sa;

    printf("My PID: %d\n", getpid());
    printf("Please run: kill -SIGUSR1 %d\n", getpid());

    /*
     * Configure sigaction struct
     */
    sa.sa_sigaction = sigusr1_handler;
    sa.sa_flags = SA_SIGINFO; // Important: Use SA_SIGINFO to get extra info
    sigemptyset(&sa.sa_mask); // Do not block other signals during handler

    // Register handler for SIGUSR1
    if (sigaction(SIGUSR1, &sa, NULL) < 0) {
        perror("sigaction");
        return 1;
    }

    printf("Waiting for SIGUSR1...\n");
    
    // Infinite loop waiting for signals
    while (1) {
        pause(); 
    }

    return 0;
}
