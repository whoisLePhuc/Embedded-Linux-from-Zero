#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define N           5
#define EAT_TIMES   3

enum { THINKING, HUNGRY, EATING };
int state[N];
sem_t chopsticks[N];
pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER;

void think(int id) {
    pthread_mutex_lock(&print_lock);
    printf("Philosopher %d is THINKING...\n", id);
    pthread_mutex_unlock(&print_lock);
    sleep(rand() % 2 + 1);
}

void eat(int id) {
    pthread_mutex_lock(&print_lock);
    printf("Philosopher %d is EATING...\n", id);
    pthread_mutex_unlock(&print_lock);
    sleep(rand() % 2 + 1);
}

void pickup(int id) {
    state[id] = HUNGRY;
    pthread_mutex_lock(&print_lock);
    printf("Philosopher %d is HUNGRY, trying to pick up chopsticks...\n", id);
    pthread_mutex_unlock(&print_lock);

    // Deadlock prevention: last philosopher picks up right chopstick first
    if (id == N - 1) {
        // Pick right, then left (reverse order)
        sem_wait(&chopsticks[(id + 1) % N]);
        sem_wait(&chopsticks[id]);
    } else {
        // Pick left, then right
        sem_wait(&chopsticks[id]);
        sem_wait(&chopsticks[(id + 1) % N]);
    }

    state[id] = EATING;
}

void putdown(int id) {
    sem_post(&chopsticks[id]);
    sem_post(&chopsticks[(id + 1) % N]);
    state[id] = THINKING;

    pthread_mutex_lock(&print_lock);
    printf("Philosopher %d put down chopsticks and returns to THINKING\n", id);
    pthread_mutex_unlock(&print_lock);
}

void *philosopher(void *arg) {
    int id = *(int *)arg;

    for (int i = 0; i < EAT_TIMES; i++) {
        think(id);
        pickup(id);
        eat(id);
        putdown(id);

        pthread_mutex_lock(&print_lock);
        printf("Philosopher %d finished meal %d/%d\n", id, i + 1, EAT_TIMES);
        pthread_mutex_unlock(&print_lock);
    }

    return NULL;
}

int main() {
    pthread_t philosophers[N];
    int ids[N];

    srand(time(NULL));

    // Initialize chopstick semaphores
    for (int i = 0; i < N; i++) {
        sem_init(&chopsticks[i], 0, 1);
    }

    // Create philosopher threads
    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Wait for all philosophers to finish
    for (int i = 0; i < N; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Cleanup
    for (int i = 0; i < N; i++) {
        sem_destroy(&chopsticks[i]);
    }

    printf("All philosophers have finished dining. No deadlock occurred!\n");
    return 0;
}
