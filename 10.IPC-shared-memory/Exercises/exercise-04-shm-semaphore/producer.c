#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <time.h>

#define SHM_NAME    "/shm_buffer"
#define SEM_PROD    "/sem_prod"
#define SEM_CONS    "/sem_cons"
#define BUFFER_SIZE 10
#define NUM_ITEMS   10

/* TODO: Định nghĩa struct shared_buffer
 * struct shared_buffer {
 *     int items[BUFFER_SIZE];
 *     int write_idx;
 * };
 */

int main() {
    int fd;
    struct shared_buffer *buf;
    sem_t *sem_prod, *sem_cons;

    srand(time(NULL));

    // 1. Tạo shared memory
    /* fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0644); */
    /* TODO: Tạo shared memory object */

    /* ftruncate(fd, sizeof(struct shared_buffer)); */
    /* TODO: Set kích thước */

    /* buf = mmap(NULL, sizeof(struct shared_buffer), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); */
    /* TODO: Map shared memory */

    /* buf->write_idx = 0; */
    /* TODO: Khởi tạo chỉ số */

    // 2. Tạo semaphore
    /* sem_prod = sem_open(SEM_PROD, O_CREAT, 0644, BUFFER_SIZE); */
    /* sem_cons = sem_open(SEM_CONS, O_CREAT, 0644, 0); */
    /* TODO: Tạo semaphore counting */

    printf("Producer started. Generating %d items...\n", NUM_ITEMS);

    // 3. Producer loop
    for (int i = 0; i < NUM_ITEMS; i++) {
        int value = rand() % 100;

        /* TODO: sem_wait(sem_prod) - chờ slot trống */
        /* TODO: Ghi giá trị vào buffer */
        /* TODO: buf->items[buf->write_idx % BUFFER_SIZE] = value; */
        /* TODO: buf->write_idx++; */
        /* TODO: sem_post(sem_cons) - báo có item mới */

        printf("Produced: %d\n", value);
        sleep(1); // Simulate work
    }

    printf("Producer finished.\n");

    // 4. Cleanup
    /* TODO: munmap, close fd, sem_close */
    /* Không unlink ở producer - consumer cần dùng */

    return 0;
}
