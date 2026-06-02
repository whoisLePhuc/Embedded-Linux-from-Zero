#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

// Struct to store thread info
typedef struct {
    int thread_id;
    char *message;
} ThreadInfo;

void *thread_func(void *arg) {
    // 1. Cast arg to ThreadInfo*
    ThreadInfo *info = (ThreadInfo *)arg;
    
    // 2. Print starting message with ID and message
    printf("Thread %d: Starting... %s\n", info->thread_id, info->message);
    
    // 3. Sleep random time (1-3s)
    int sleep_time = 1 + rand() % 3;
    sleep(sleep_time);
    
    // 4. Print finished message
    printf("Thread %d: Finished\n", info->thread_id);
    
    return NULL;
}

int main() {
    pthread_t threads[3];
    ThreadInfo infos[3];
    
    // Seed random number generator
    srand(time(NULL));

    // Prepare data for threads
    infos[0].thread_id = 1;
    infos[0].message = "Hello from thread 1";
    
    infos[1].thread_id = 2;
    infos[1].message = "Greetings from thread 2";
    
    infos[2].thread_id = 3;
    infos[2].message = "Thread 3 says hi!";

    // Create 3 threads
    for (int i = 0; i < 3; i++) {
        if (pthread_create(&threads[i], NULL, thread_func, &infos[i]) != 0) {
            perror("pthread_create failed");
            return 1;
        }
    }
    
    // Join 3 threads
    for (int i = 0; i < 3; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join failed");
            return 1;
        }
    }

    printf("Main thread: All threads finished\n");
    return 0;
}
