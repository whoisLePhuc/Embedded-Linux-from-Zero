#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

#define SIZE 4096
#define NAME "/test_shm"

int main() {
    int shm_fd;
    void *ptr;
    char buffer[1024];

    // Create shared memory object
    shm_fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Set segment size
    if (ftruncate(shm_fd, SIZE) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    // Map memory
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    printf("Writer: Enter message to write to shared memory:\n> ");
    if (fgets(buffer, 1024, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;
        sprintf(ptr, "%s", buffer);
        printf("Writer: Data '%s' written.\n", (char*)ptr);
    }

    // Unmap
    munmap(ptr, SIZE);
    close(shm_fd);

    return 0;
}
