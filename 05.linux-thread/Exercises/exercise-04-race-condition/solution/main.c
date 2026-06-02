#include <stdio.h>
#include <pthread.h>

int counter = 0;
// Define mutex
pthread_mutex_t lock;

void *thread_func(void *arg) {
    for (int i = 0; i < 1000000; i++) {
        // Lock mutex
        pthread_mutex_lock(&lock);
        counter++;
        // Unlock mutex
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    
    // Init mutex
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("Mutex init failed\n");
        return 1;
    }

    // Create threads
    pthread_create(&t1, NULL, thread_func, NULL);
    pthread_create(&t2, NULL, thread_func, NULL);
    
    // Join threads
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Destroy mutex
    pthread_mutex_destroy(&lock);
    
    printf("Final counter: %d\n", counter);
    return 0;
}
