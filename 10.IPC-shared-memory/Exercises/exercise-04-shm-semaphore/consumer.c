#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>

#define SHM_NAME    "/shm_buffer"
#define SEM_PROD    "/sem_prod"
#define SEM_CONS    "/sem_cons"
#define BUFFER_SIZE 10
#define NUM_ITEMS   10

/* TODO: Định nghĩa struct shared_buffer (giống producer)
 * struct shared_buffer {
 *     int items[BUFFER_SIZE];
 *     int write_idx;
 * };
 */

int main() {
    int fd;
    struct shared_buffer *buf;
    sem_t *sem_prod, *sem_cons;

    // 1. Mở shared memory (không O_CREAT)
    /* fd = shm_open(SHM_NAME, O_RDWR, 0644); */
    /* TODO: Mở shared memory (không tạo mới) */

    /* buf = mmap(NULL, sizeof(struct shared_buffer), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); */
    /* TODO: Map shared memory */

    // 2. Mở semaphore
    /* sem_prod = sem_open(SEM_PROD, O_CREAT, 0644, BUFFER_SIZE); */
    /* sem_cons = sem_open(SEM_CONS, O_CREAT, 0644, 0); */
    /* TODO: Mở semaphore (dùng O_CREAT để mở nếu chạy trước producer) */

    printf("Consumer started. Waiting for items...\n");

    // 3. Consumer loop
    for (int i = 0; i < NUM_ITEMS; i++) {
        /* TODO: sem_wait(sem_cons) - chờ có item */
        /* TODO: Đọc giá trị từ buffer */
        /* int value = buf->items[i % BUFFER_SIZE]; */
        /* TODO: sem_post(sem_prod) - báo slot trống */

        printf("Consumed: %d\n", value);
    }

    printf("Consumer finished.\n");

    // 4. Cleanup
    /* TODO: munmap, close fd, sem_close */
    /* TODO: sem_unlink cho cả hai semaphore */
    /* TODO: shm_unlink(SHM_NAME) */

    return 0;
}
