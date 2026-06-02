#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int *ptr;

    // 1. Tạo Anonymous Shared Memory
    // Kích thước: sizeof(int)
    // Protection: PROT_READ | PROT_WRITE
    // Flags: MAP_SHARED | MAP_ANONYMOUS
    /* TODO: mmap */
    
    // Gán giá trị ban đầu
    /* TODO: *ptr = 10; */

    printf("Original value: %d\n", *ptr);

    // 2. Fork
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        // --- CHILD ---
        // Con chia sẻ vùng nhớ ptr với cha
        /* TODO: Tăng giá trị *ptr += 5 */
        
        /* TODO: In ra giá trị mới */
        
        exit(0);
    } else {
        // --- PARENT ---
        // Đợi con xong
        wait(NULL);

        /* TODO: In ra giá trị *ptr để kiểm chứng */
        // printf("Parent sees value: %d\n", ...);

        // 3. Cleanup
        /* TODO: munmap */
    }

    return 0;
}
