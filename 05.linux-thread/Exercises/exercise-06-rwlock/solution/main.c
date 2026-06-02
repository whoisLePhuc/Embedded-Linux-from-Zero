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
pthread_mutex_t mutex;
pthread_rwlock_t rwlock;

void *reader_mutex(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < READ_LOOPS; i++) {
        pthread_mutex_lock(&mutex);
        int sum = 0;
        for (int j = 0; j < CACHE_SIZE; j++)
            sum += config_cache[j];
        pthread_mutex_unlock(&mutex);
    }
    printf("[Reader %d] Mutex reader finished\n", id);
    return NULL;
}

void *writer_mutex(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < WRITE_LOOPS; i++) {
        pthread_mutex_lock(&mutex);
        for (int j = 0; j < CACHE_SIZE; j++)
            config_cache[j]++;
        total_writes++;
        pthread_mutex_unlock(&mutex);
    }
    printf("[Writer %d] Mutex writer finished\n", id);
    return NULL;
}

void *reader_rwlock(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < READ_LOOPS; i++) {
        pthread_rwlock_rdlock(&rwlock);
        int sum = 0;
        for (int j = 0; j < CACHE_SIZE; j++)
            sum += config_cache[j];
        pthread_rwlock_unlock(&rwlock);
    }
    printf("[Reader %d] RWLock reader finished\n", id);
    return NULL;
}

void *writer_rwlock(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < WRITE_LOOPS; i++) {
        pthread_rwlock_wrlock(&rwlock);
        for (int j = 0; j < CACHE_SIZE; j++)
            config_cache[j]++;
        total_writes++;
        pthread_rwlock_unlock(&rwlock);
    }
    printf("[Writer %d] RWLock writer finished\n", id);
    return NULL;
}

double run_mutex_benchmark(void) {
    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    int reader_ids[NUM_READERS];
    int writer_ids[NUM_WRITERS];
    struct timespec start, end;

    printf("\n=== Mutex Benchmark ===\n");
    printf("%d readers (%d loops), %d writers (%d loops)\n",
           NUM_READERS, READ_LOOPS, NUM_WRITERS, WRITE_LOOPS);

    for (int j = 0; j < CACHE_SIZE; j++)
        config_cache[j] = 1;
    total_writes = 0;

    pthread_mutex_init(&mutex, NULL);

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i;
        pthread_create(&readers[i], NULL, reader_mutex, &reader_ids[i]);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i;
        pthread_create(&writers[i], NULL, writer_mutex, &writer_ids[i]);
    }
    for (int i = 0; i < NUM_READERS; i++)
        pthread_join(readers[i], NULL);
    for (int i = 0; i < NUM_WRITERS; i++)
        pthread_join(writers[i], NULL);

    clock_gettime(CLOCK_MONOTONIC, &end);

    pthread_mutex_destroy(&mutex);

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

    for (int j = 0; j < CACHE_SIZE; j++)
        config_cache[j] = 1;
    total_writes = 0;

    pthread_rwlock_init(&rwlock, NULL);

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i;
        pthread_create(&readers[i], NULL, reader_rwlock, &reader_ids[i]);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i;
        pthread_create(&writers[i], NULL, writer_rwlock, &writer_ids[i]);
    }
    for (int i = 0; i < NUM_READERS; i++)
        pthread_join(readers[i], NULL);
    for (int i = 0; i < NUM_WRITERS; i++)
        pthread_join(writers[i], NULL);

    clock_gettime(CLOCK_MONOTONIC, &end);

    pthread_rwlock_destroy(&rwlock);

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
