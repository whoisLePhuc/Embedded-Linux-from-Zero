#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define N           5
#define EAT_TIMES   3

/* TODO: Khai báo state enum và mảng
 * enum { THINKING, HUNGRY, EATING };
 * int state[N];
 * sem_t chopsticks[N];
 * pthread_mutex_t print_lock;
 */

/* TODO: Function prototypes */
/* void think(int id); */
/* void eat(int id); */
/* void pickup(int id); */
/* void putdown(int id); */
/* void *philosopher(void *arg); */

int main() {
    pthread_t philosophers[N];
    int ids[N];

    srand(time(NULL));

    // 1. Khởi tạo chopstick semaphores
    /* TODO: for loop sem_init(&chopsticks[i], 0, 1) */

    // 2. Tạo philosopher threads
    /* TODO: for loop pthread_create */

    // 3. Chờ các thread kết thúc
    /* TODO: for loop pthread_join */

    // 4. Cleanup
    /* TODO: sem_destroy cho từng chopstick */

    printf("All philosophers have finished dining.\n");
    return 0;
}

/* TODO: Định nghĩa think() - sleep ngẫu nhiên */

/* TODO: Định nghĩa eat() - sleep ngẫu nhiên */

/* TODO: Định nghĩa pickup()
 * Deadlock prevention: triết gia cuối cùng (N-1)
 * cầm đũa phải trước, sau đó đũa trái
 */

/* TODO: Định nghĩa putdown() */

/* TODO: Định nghĩa philosopher()
 * - Loop EAT_TIMES lần
 * - Think -> pickup -> eat -> putdown
 */
