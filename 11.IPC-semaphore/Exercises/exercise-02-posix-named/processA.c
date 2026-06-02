#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>

#define SEM_NAME "/test_sem"

int main() {
    sem_t *sem;

    // 1. Tạo/Mở Named Semaphore
    /* TODO: sem_open với O_CREAT, value = 1 */
    // sem = sem_open(SEM_NAME, ...);
    

    printf("Process A started...\n");

    for (int i = 0; i < 5; i++) {
        /* TODO: Wait */
        
        // Critical Section
        printf("A"); fflush(stdout);
        sleep(1);
        printf("A"); fflush(stdout);
        sleep(1);
        printf("A\n");
        
        /* TODO: Post */
        
        sleep(1); // Nghỉ 1 chút để process kia có cơ hội chạy
    }

    // Close
    /* TODO: sem_close */

    // Unlink (Chỉ cần 1 process làm việc này)
    /* TODO: sem_unlink(SEM_NAME); */
    
    return 0;
}
