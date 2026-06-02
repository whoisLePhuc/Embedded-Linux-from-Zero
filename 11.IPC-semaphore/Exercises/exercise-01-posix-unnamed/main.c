#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_LOOPS 1000000

int counter = 0;
/* TODO: Khai báo semaphore */
// sem_t sem;

void *thread_func(void *arg) {
    (void)arg; // Unused
    for (int i = 0; i < NUM_LOOPS; i++) {
        // Critical Section Start
        /* TODO: sem_wait(&sem); */
        
        counter++;
        
        // Critical Section End
        /* TODO: sem_post(&sem); */
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    /* TODO: Init semaphore với value = 1 */
    // sem_init(&sem, 0, 1);

    printf("Starting threads...\n");

    pthread_create(&t1, NULL, thread_func, NULL);
    pthread_create(&t2, NULL, thread_func, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Expected counter: %d\n", NUM_LOOPS * 2);
    printf("Actual counter:   %d\n", counter);

    /* TODO: Destroy semaphore */
    // sem_destroy(&sem);

    return 0;
}
