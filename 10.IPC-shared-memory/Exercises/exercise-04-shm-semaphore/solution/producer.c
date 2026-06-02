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

struct shared_buffer {
    int items[BUFFER_SIZE];
    int write_idx;
};

int main() {
    int fd;
    struct shared_buffer *buf;
    sem_t *sem_prod, *sem_cons;

    srand(time(NULL));

    // Create shared memory
    fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }

    // Set size
    if (ftruncate(fd, sizeof(struct shared_buffer)) == -1) {
        perror("ftruncate");
        exit(1);
    }

    // Map shared memory
    buf = mmap(NULL, sizeof(struct shared_buffer),
               PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (buf == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    buf->write_idx = 0;

    // Create named semaphores
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

    printf("Producer started. Generating %d items...\n", NUM_ITEMS);

    for (int i = 0; i < NUM_ITEMS; i++) {
        int value = rand() % 100;

        // Wait for empty slot
        sem_wait(sem_prod);

        // Write to buffer
        int idx = buf->write_idx % BUFFER_SIZE;
        buf->items[idx] = value;
        buf->write_idx++;

        // Signal item available
        sem_post(sem_cons);

        printf("Produced: %d (at slot %d)\n", value, idx);
        sleep(1);
    }

    printf("Producer finished.\n");

    // Cleanup (don't unlink - consumer may need it)
    munmap(buf, sizeof(struct shared_buffer));
    close(fd);
    sem_close(sem_prod);
    sem_close(sem_cons);

    return 0;
}
