#define _DEFAULT_SOURCE  // For usleep
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define NUM_WORKERS 4

// Simulated task for workers
void worker_task(int worker_id) {
    // Seed random with worker-specific value
    srand(time(NULL) + worker_id);
    
    // Random work duration 1-5 seconds
    int work_time = (rand() % 5) + 1;
    
    printf("Worker %d (PID %d): Starting task, will take %d seconds\n",
           worker_id, getpid(), work_time);
    
    sleep(work_time);
    
    printf("Worker %d (PID %d): Task completed!\n", worker_id, getpid());
}

int main() {
    pid_t workers[NUM_WORKERS];
    int completed[NUM_WORKERS] = {0};
    int running = 0;
    
    printf("======================================\n");
    printf("     Process Pool Demo\n");
    printf("======================================\n\n");
    printf("Parent PID: %d\n", getpid());
    printf("Creating %d worker processes...\n\n", NUM_WORKERS);
    
    // Create worker processes
    for (int i = 0; i < NUM_WORKERS; i++) {
        workers[i] = fork();
        
        if (workers[i] < 0) {
            perror("fork failed");
            exit(1);
        }
        
        if (workers[i] == 0) {
            // Child process: do work and exit
            worker_task(i);
            exit(i);  // Exit with worker ID as status
        }
        
        // Parent: record worker PID
        printf("Parent: Created worker %d (PID %d)\n", i, workers[i]);
        running++;
    }
    
    printf("\nParent: All workers started. Monitoring...\n");
    printf("=========================================\n\n");
    
    // Monitor workers using non-blocking wait
    while (running > 0) {
        for (int i = 0; i < NUM_WORKERS; i++) {
            if (workers[i] > 0 && !completed[i]) {
                int status;
                pid_t result = waitpid(workers[i], &status, WNOHANG);
                
                if (result == workers[i]) {
                    // Worker finished
                    completed[i] = 1;
                    running--;
                    
                    if (WIFEXITED(status)) {
                        printf(">> Worker %d (PID %d) finished with exit code %d\n",
                               i, workers[i], WEXITSTATUS(status));
                    } else if (WIFSIGNALED(status)) {
                        printf(">> Worker %d (PID %d) killed by signal %d\n",
                               i, workers[i], WTERMSIG(status));
                    }
                    
                    printf("   Remaining workers: %d\n\n", running);
                }
                else if (result == -1) {
                    // Error
                    perror("waitpid");
                    completed[i] = 1;
                    running--;
                }
            }
        }
        
        // Small delay to avoid busy waiting
        usleep(100000);  // 100ms
    }
    
    printf("=========================================\n");
    printf("All workers completed!\n");
    printf("Parent: Exiting.\n");
    
    return 0;
}
