#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int buffer = 0; // 0: Empty, 1: Full
pthread_mutex_t lock;
pthread_cond_t cond;

void *producer(void *arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&lock);
        
        // Wait while buffer is full
        while (buffer == 1) {
            pthread_cond_wait(&cond, &lock);
        }
        
        // Produce
        buffer = 1;
        printf("Produced data\n");
        
        // Signal consumer
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&lock);
        
        sleep(1);
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&lock);
        
        // Wait while buffer is empty
        while (buffer == 0) {
            pthread_cond_wait(&cond, &lock);
        }
        
        // Consume
        buffer = 0;
        printf("Consumed data\n");
        
        // Signal producer
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&lock);
        
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&t1, NULL, producer, NULL);
    pthread_create(&t2, NULL, consumer, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    return 0;
}
