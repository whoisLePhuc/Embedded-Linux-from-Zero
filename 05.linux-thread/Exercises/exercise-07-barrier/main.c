#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 4
#define ARRAY_SIZE  16

int data[ARRAY_SIZE];

/* TODO: Declare barrier */

typedef struct {
    int id;
    int start;
    int end;
} ThreadRange;

/*
 * TODO: Define worker thread function
 *
 * Phase 1 (Initialize):
 *   for i in [start, end):
 *       data[i] = thread_id * 10 + (i - start)
 *   Print: "[Thread X] Phase 1: data[start..end-1] initialized"
 *
 * Barrier: pthread_barrier_wait(&barrier)
 *
 * Phase 2 (Transform):
 *   for i in [start, end):
 *       data[i] += thread_id + 1
 *   Print: "[Thread X] Phase 2: transformed chunk"
 *
 * Barrier: pthread_barrier_wait(&barrier)
 *
 * Phase 3 (Read all):
 *   Print the entire array to prove cross-thread data is available
 *   Print: "[Thread X] Full array: {val0, val1, ..., val15}"
 *
 * Return: NULL
 */

int main(void) {
    pthread_t threads[NUM_THREADS];
    ThreadRange ranges[NUM_THREADS];
    int chunk_size = ARRAY_SIZE / NUM_THREADS;

    /* TODO: Initialize barrier for NUM_THREADS threads */

    /* TODO: Create NUM_THREADS threads with appropriate ranges */

    /* TODO: Join all threads */

    printf("Final array: ");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    /* TODO: Destroy barrier */

    return 0;
}
