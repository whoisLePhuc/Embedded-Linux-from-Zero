#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 4
#define ARRAY_SIZE  16

int data[ARRAY_SIZE];
pthread_barrier_t barrier;

typedef struct {
    int id;
    int start;
    int end;
} ThreadRange;

void *worker(void *arg) {
    ThreadRange *range = (ThreadRange *)arg;
    int id = range->id;

    /* Phase 1: Initialize chunk */
    for (int i = range->start; i < range->end; i++) {
        data[i] = id * 10 + (i - range->start);
    }
    printf("[Thread %d] Phase 1: data[%d..%d] initialized\n",
           id, range->start, range->end - 1);

    pthread_barrier_wait(&barrier);

    /* Phase 2: Transform chunk */
    for (int i = range->start; i < range->end; i++) {
        data[i] += id + 1;
    }
    printf("[Thread %d] Phase 2: transformed chunk\n", id);

    pthread_barrier_wait(&barrier);

    /* Phase 3: Read and print entire array */
    printf("[Thread %d] Full array: {", id);
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d%s", data[i], (i < ARRAY_SIZE - 1) ? ", " : "");
    }
    printf("}\n");

    return NULL;
}

int main(void) {
    pthread_t threads[NUM_THREADS];
    ThreadRange ranges[NUM_THREADS];
    int chunk_size = ARRAY_SIZE / NUM_THREADS;

    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    for (int i = 0; i < NUM_THREADS; i++) {
        ranges[i].id = i;
        ranges[i].start = i * chunk_size;
        ranges[i].end = (i == NUM_THREADS - 1) ? ARRAY_SIZE
                                               : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, worker, &ranges[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\nFinal array: ");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    pthread_barrier_destroy(&barrier);

    return 0;
}
