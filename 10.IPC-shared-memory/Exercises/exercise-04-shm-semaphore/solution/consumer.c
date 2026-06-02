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

struct shared_buffer {
    int items[BUFFER_SIZE];
    int write_idx;
};

int main() {
    int fd;
    struct shared_buffer *buf;
    sem_t *sem_prod, *sem_cons;

    // Open shared memory (created by producer)
    fd = shm_open(SHM_NAME, O_RDWR, 0644);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }

    // Map shared memory
    buf = mmap(NULL, sizeof(struct shared_buffer),
               PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (buf == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // Open semaphores
    sem_prod = sem_open(SEM_PROD, O_CREAT, 0644, BUFFER_SIZE);
    if (sem_prod == SEM_FAILED) {
        perror("sem_open (prod)");
        exit(1);
    }

    sem_cons = sem_open(SEM_CONS, O_CREAT, 0644, 0);
    if (sem_cons == SEM_FAILED) {
        perror("sem_open (cons)");
        exit(1);
    }

    printf("Consumer started. Waiting for items...\n");

    for (int i = 0; i < NUM_ITEMS; i++) {
        // Wait for item to be available
        sem_wait(sem_cons);

        // Read from buffer
        int value = buf->items[i % BUFFER_SIZE];

        // Signal empty slot
        sem_post(sem_prod);

        printf("Consumed: %d\n", value);
    }

    printf("Consumer finished.\n");

    // Cleanup and destroy
    munmap(buf, sizeof(struct shared_buffer));
    close(fd);
    sem_close(sem_prod);
    sem_close(sem_cons);

    sem_unlink(SEM_PROD);
    sem_unlink(SEM_CONS);
    shm_unlink(SHM_NAME);

    return 0;
}
