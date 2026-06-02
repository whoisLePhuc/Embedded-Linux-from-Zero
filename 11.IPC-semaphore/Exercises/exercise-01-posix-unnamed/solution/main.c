#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_LOOPS 1000000

int counter = 0;
sem_t sem;

void *thread_func(void *arg) {
    (void)arg;
    for (int i = 0; i < NUM_LOOPS; i++) {
        // Wait (P) - Enter Critical Section
        sem_wait(&sem);
        
        counter++;
        
        // Post (V) - Exit Critical Section
        sem_post(&sem);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    // Init unnamed semaphore, not shared between processes (0), initial value 1
    if (sem_init(&sem, 0, 1) != 0) {
        perror("sem_init");
        exit(1);
    }

    printf("Starting threads with Semaphore protection...\n");

    pthread_create(&t1, NULL, thread_func, NULL);
    pthread_create(&t2, NULL, thread_func, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Expected counter: %d\n", NUM_LOOPS * 2);
    printf("Actual counter:   %d\n", counter);

    sem_destroy(&sem);

    return 0;
}
