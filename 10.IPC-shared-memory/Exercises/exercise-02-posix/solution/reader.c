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

    // Open shared memory object (RDONLY)
    shm_fd = shm_open(NAME, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm_open (Writer run first?)");
        exit(EXIT_FAILURE);
    }

    // Map memory
    ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    printf("Reader: Read from Shared Memory:\n%s\n", (char *)ptr);

    // Cleanup
    munmap(ptr, SIZE);
    close(shm_fd);
    
    // Unlink (Delete) the shared memory object
    shm_unlink(NAME);

    return 0;
}
