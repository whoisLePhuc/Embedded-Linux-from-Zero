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

    // 1. Open shared memory object
    /* TODO: shm_open O_RDONLY */
    

    // 2. Memory Map
    // ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    /* TODO: mmap với PROT_READ */
    

    // 3. Read Data
    printf("Read from Shared Memory: %s\n", (char *)ptr);

    // 4. Cleanup
    /* TODO: munmap, close, và shm_unlink */
    // shm_unlink(NAME);

    return 0;
}
