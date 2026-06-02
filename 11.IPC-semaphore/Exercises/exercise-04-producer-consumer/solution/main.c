#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define BUFFER_SIZE 5
#define NUM_ITEMS   10
#define NUM_PROD    2
#define NUM_CONS    2

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t sem_empty;
sem_t sem_full;
sem_t mutex;

void *producer(void *arg) {
    int id = *(int *)arg;

    for (int i = 0; i < NUM_ITEMS; i++) {
        int value = rand() % 100;

        sem_wait(&sem_empty);  // Wait for empty slot
        sem_wait(&mutex);      // Lock

        buffer[in] = value;
        printf("Producer %d: produced %d at slot %d\n", id, value, in);
        in = (in + 1) % BUFFER_SIZE;

        sem_post(&mutex);      // Unlock
        sem_post(&sem_full);   // Signal item available

        sleep(rand() % 2);     // Simulate variable work speed
    }

    return NULL;
}

void *consumer(void *arg) {
    int id = *(int *)arg;

    for (int i = 0; i < NUM_ITEMS; i++) {
        sem_wait(&sem_full);   // Wait for item
        sem_wait(&mutex);      // Lock

        int value = buffer[out];
        printf("Consumer %d: consumed %d from slot %d\n", id, value, out);
        out = (out + 1) % BUFFER_SIZE;

        sem_post(&mutex);      // Unlock
        sem_post(&sem_empty);  // Signal empty slot

        sleep(rand() % 2);     // Simulate variable work speed
    }

    return NULL;
}

int main() {
    pthread_t producers[NUM_PROD], consumers[NUM_CONS];
    int prod_ids[NUM_PROD], cons_ids[NUM_CONS];

    srand(time(NULL));

    // Initialize semaphores
    sem_init(&sem_empty, 0, BUFFER_SIZE);
    sem_init(&sem_full, 0, 0);
    sem_init(&mutex, 0, 1);

    printf("Starting Producer-Consumer demo (%d producers, %d consumers)\n",
           NUM_PROD, NUM_CONS);

    // Create producer threads
    for (int i = 0; i < NUM_PROD; i++) {
        prod_ids[i] = i + 1;
        pthread_create(&producers[i], NULL, producer, &prod_ids[i]);
    }

    // Create consumer threads
    for (int i = 0; i < NUM_CONS; i++) {
        cons_ids[i] = i + 1;
        pthread_create(&consumers[i], NULL, consumer, &cons_ids[i]);
    }

    // Wait for producers
    for (int i = 0; i < NUM_PROD; i++) {
        pthread_join(producers[i], NULL);
    }

    // Wait for consumers
    for (int i = 0; i < NUM_CONS; i++) {
        pthread_join(consumers[i], NULL);
    }

    // Cleanup
    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);
    sem_destroy(&mutex);

    printf("All threads completed.\n");
    return 0;
}
