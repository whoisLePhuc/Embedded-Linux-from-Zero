#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int *ptr;

    // Create Anonymous Shared Memory
    // Use MAP_ANONYMOUS, fd is -1
    ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, 
               MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    *ptr = 10;
    printf("Original value: %d\n", *ptr);

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        // --- CHILD ---
        printf("Child: Reading value: %d\n", *ptr);
        *ptr += 5;
        printf("Child: Updated value to: %d\n", *ptr);
        exit(0);
    } else {
        // --- PARENT ---
        wait(NULL);
        printf("Parent sees value: %d\n", *ptr);

        // Cleanup
        munmap(ptr, sizeof(int));
    }

    return 0;
}
