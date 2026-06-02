#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

// Signal handler to prevent zombie (used in demo_with_handler)
void sigchld_handler(int sig) {
    (void)sig;  // Unused
    // Reap all terminated children
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void demo_zombie_creation() {
    printf("=== Demo 1: Creating Zombie Process ===\n");
    printf("Parent PID: %d\n\n", getpid());
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child process
        printf("Child (PID %d): I'm exiting now...\n", getpid());
        exit(42);
    }
    
    // Parent process - intentionally NOT calling wait()
    printf("Parent: Child PID is %d\n", pid);
    printf("Parent: Sleeping 20 seconds WITHOUT calling wait()...\n");
    printf("\n>>> In another terminal, run: ps aux | grep Z\n");
    printf(">>> You should see the child as zombie: <defunct>\n\n");
    
    sleep(20);
    
    printf("Parent: Now calling wait() to reap zombie...\n");
    int status;
    waitpid(pid, &status, 0);
    printf("Parent: Child reaped! Exit status was: %d\n", WEXITSTATUS(status));
    printf(">>> Zombie is gone now. Check ps again!\n\n");
}

void demo_prevent_zombie_with_wait() {
    printf("=== Demo 2: Preventing Zombie with wait() ===\n");
    printf("Parent PID: %d\n\n", getpid());
    
    pid_t pid = fork();
    
    if (pid == 0) {
        printf("Child (PID %d): Working for 2 seconds...\n", getpid());
        sleep(2);
        printf("Child: Exiting now.\n");
        exit(0);
    }
    
    printf("Parent: Waiting for child...\n");
    int status;
    wait(&status);
    printf("Parent: Child reaped immediately. No zombie!\n\n");
}

void demo_prevent_zombie_with_signal() {
    printf("=== Demo 3: Preventing Zombie with SIGCHLD Handler ===\n");
    printf("Parent PID: %d\n\n", getpid());
    
    // Install signal handler
    signal(SIGCHLD, sigchld_handler);
    
    // Create multiple children
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            printf("Child %d (PID %d): Working for %d seconds...\n", 
                   i, getpid(), i + 1);
            sleep(i + 1);
            printf("Child %d: Exiting.\n", i);
            exit(i);
        }
        printf("Parent: Created child %d (PID %d)\n", i, pid);
    }
    
    printf("\nParent: Doing other work while children run...\n");
    printf(">>> Children will be reaped automatically by signal handler\n\n");
    
    sleep(5);
    printf("Parent: All done. No zombies created!\n\n");
}

void demo_prevent_zombie_with_ignore() {
    printf("=== Demo 4: Preventing Zombie with SIG_IGN ===\n");
    printf("Parent PID: %d\n\n", getpid());
    
    // Ignore SIGCHLD - kernel auto-reaps children
    signal(SIGCHLD, SIG_IGN);
    
    pid_t pid = fork();
    
    if (pid == 0) {
        printf("Child (PID %d): Exiting immediately.\n", getpid());
        exit(0);
    }
    
    printf("Parent: Created child %d, sleeping 3 seconds...\n", pid);
    printf(">>> Child exits but won't become zombie (SIG_IGN)\n\n");
    
    sleep(3);
    printf("Parent: Done. No zombie was created!\n");
    printf("(Note: We lose the ability to get exit status)\n\n");
}

int main() {
    printf("======================================\n");
    printf("     Zombie Process Demo\n");
    printf("======================================\n\n");
    
    printf("Choose a demo:\n");
    printf("1. Create zombie (observe for 20 seconds)\n");
    printf("2. Prevent zombie with wait()\n");
    printf("3. Prevent zombie with SIGCHLD handler\n");
    printf("4. Prevent zombie with SIG_IGN\n");
    printf("\nEnter choice (1-4): ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("\n");
    
    switch (choice) {
        case 1:
            demo_zombie_creation();
            break;
        case 2:
            demo_prevent_zombie_with_wait();
            break;
        case 3:
            demo_prevent_zombie_with_signal();
            break;
        case 4:
            demo_prevent_zombie_with_ignore();
            break;
        default:
            printf("Invalid choice\n");
            return 1;
    }
    
    return 0;
}
