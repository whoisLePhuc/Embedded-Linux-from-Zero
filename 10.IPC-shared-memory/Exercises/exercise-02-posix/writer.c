#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>

#define SIZE 4096
#define NAME "/test_shm"

int main() {
    int shm_fd;
    void *ptr;

    // 1. Create shared memory object
    // shm_fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);
    /* TODO: shm_open */
    

    // 2. Set size
    // ftruncate(shm_fd, SIZE);
    /* TODO: ftruncate */
    

    // 3. Memory Map
    // ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    /* TODO: mmap */
    

    printf("Shared memory created. Writing data...\n");

    // 4. Write Data
    // sprintf(ptr, "%s", "Hello from POSIX Shared Memory!");
    /* TODO: Ghi dữ liệu vào ptr */


    // 5. Cleanup
    /* TODO: munmap và close */

    return 0;
}
