#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define CACHE_SIZE  64
#define NUM_READERS 6
#define NUM_WRITERS 2
#define READ_LOOPS  200000
#define WRITE_LOOPS 2000

int config_cache[CACHE_SIZE];
int total_writes = 0;

/* TODO: Declare global mutex and rwlock */

/*
 * TODO: Implement reader_mutex()
 *   - Loop READ_LOOPS times:
 *     - Lock mutex
 *     - Compute sum of all config_cache elements
 *     - Unlock mutex
 * Return: NULL
 */

/*
 * TODO: Implement writer_mutex()
 *   - Loop WRITE_LOOPS times:
 *     - Lock mutex
 *     - Increment every element of config_cache by 1
 *     - total_writes++
 *     - Unlock mutex
 * Return: NULL
 */

/*
 * TODO: Implement reader_rwlock() — same logic but use rdlock
 * TODO: Implement writer_rwlock() — same logic but use wrlock
 */

double run_mutex_benchmark(void) {
    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    int reader_ids[NUM_READERS];
    int writer_ids[NUM_WRITERS];
    struct timespec start, end;

    printf("\n=== Mutex Benchmark ===\n");
    printf("%d readers (%d loops), %d writers (%d loops)\n",
           NUM_READERS, READ_LOOPS, NUM_WRITERS, WRITE_LOOPS);

    /* TODO: Initialize config_cache elements to 1, reset total_writes */

    /* TODO: Initialize mutex */

    clock_gettime(CLOCK_MONOTONIC, &start);

    /* TODO: Create reader_mutex threads and writer_mutex threads */
    /* TODO: Join all threads */

    clock_gettime(CLOCK_MONOTONIC, &end);

    /* TODO: Destroy mutex */

    double elapsed = (end.tv_sec - start.tv_sec) +
                     (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Total writes: %d | cache[0] = %d\n", total_writes, config_cache[0]);
    printf("Time: %.3f seconds\n", elapsed);
    return elapsed;
}

double run_rwlock_benchmark(void) {
    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    int reader_ids[NUM_READERS];
    int writer_ids[NUM_WRITERS];
    struct timespec start, end;

    printf("\n=== RWLock Benchmark ===\n");
    printf("%d readers (%d loops), %d writers (%d loops)\n",
           NUM_READERS, READ_LOOPS, NUM_WRITERS, WRITE_LOOPS);

    /* TODO: Initialize config_cache elements to 1, reset total_writes */

    /* TODO: Initialize rwlock */

    clock_gettime(CLOCK_MONOTONIC, &start);

    /* TODO: Create reader_rwlock threads and writer_rwlock threads */
    /* TODO: Join all threads */

    clock_gettime(CLOCK_MONOTONIC, &end);

    /* TODO: Destroy rwlock */

    double elapsed = (end.tv_sec - start.tv_sec) +
                     (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Total writes: %d | cache[0] = %d\n", total_writes, config_cache[0]);
    printf("Time: %.3f seconds\n", elapsed);
    return elapsed;
}

int main(void) {
    double mutex_time = run_mutex_benchmark();
    double rwlock_time = run_rwlock_benchmark();

    printf("\n=== Performance Comparison ===\n");
    printf("Mutex  time: %.3f seconds\n", mutex_time);
    printf("RWLock time: %.3f seconds\n", rwlock_time);
    if (rwlock_time < mutex_time)
        printf("Speedup: %.2fx (RWLock is faster)\n", mutex_time / rwlock_time);
    else
        printf("Note: RWLock not faster — try increasing readers or read loops\n");

    return 0;
}
