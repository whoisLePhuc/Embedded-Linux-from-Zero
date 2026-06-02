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

/* TODO: Khai báo buffer, chỉ số, và semaphore
 * int buffer[BUFFER_SIZE];
 * int in = 0, out = 0;
 * sem_t sem_empty, sem_full, mutex;
 */

/* TODO: Khai báo prototype cho producer và consumer threads */

int main() {
    pthread_t producers[NUM_PROD], consumers[NUM_CONS];
    int prod_ids[NUM_PROD], cons_ids[NUM_CONS];

    srand(time(NULL));

    // 1. Khởi tạo semaphore
    /* sem_init(&sem_empty, 0, BUFFER_SIZE); */
    /* sem_init(&sem_full, 0, 0); */
    /* sem_init(&mutex, 0, 1); */
    /* TODO: Khởi tạo 3 semaphore */

    printf("Starting Producer-Consumer demo (%d producers, %d consumers)\n",
           NUM_PROD, NUM_CONS);

    // 2. Tạo threads
    /* TODO: Tạo NUM_PROD producer threads */
    /* TODO: Tạo NUM_CONS consumer threads */

    // 3. Chờ các thread kết thúc
    /* TODO: join tất cả threads */

    // 4. Cleanup
    /* TODO: sem_destroy cho 3 semaphore */

    printf("All threads completed.\n");
    return 0;
}

/* TODO: Định nghĩa producer thread function
 * void *producer(void *arg) {
 *     int id = *(int *)arg;
 *     for (int i = 0; i < NUM_ITEMS; i++) {
 *         int value = rand() % 100;
 *         sem_wait(&sem_empty);  // Chờ slot trống
 *         sem_wait(&mutex);      // Lock
 *         buffer[in] = value;
 *         printf("Producer %d: produced %d at slot %d\n", id, value, in);
 *         in = (in + 1) % BUFFER_SIZE;
 *         sem_post(&mutex);      // Unlock
 *         sem_post(&sem_full);   // Báo có item mới
 *         sleep(rand() % 2);     // Simulate variable work speed
 *     }
 *     return NULL;
 * }
 */

/* TODO: Định nghĩa consumer thread function
 * void *consumer(void *arg) {
 *     int id = *(int *)arg;
 *     for (int i = 0; i < NUM_ITEMS; i++) {
 *         sem_wait(&sem_full);   // Chờ có item
 *         sem_wait(&mutex);      // Lock
 *         int value = buffer[out];
 *         printf("Consumer %d: consumed %d from slot %d\n", id, value, out);
 *         out = (out + 1) % BUFFER_SIZE;
 *         sem_post(&mutex);      // Unlock
 *         sem_post(&sem_empty);  // Báo slot trống
 *         sleep(rand() % 2);     // Simulate variable work speed
 *     }
 *     return NULL;
 * }
 */
