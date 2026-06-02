#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>

#define SEM_NAME "/test_sem"

int main() {
    sem_t *sem;

    // 1. Mở Named Semaphore (không O_CREAT nếu chắc chắn A chạy trước)
    // Hoặc O_CREAT để an toàn
    /* TODO: sem_open */
    

    printf("Process B started...\n");

    for (int i = 0; i < 5; i++) {
        /* TODO: Wait */
        
        // Critical Section
        printf("B"); fflush(stdout);
        sleep(1);
        printf("B"); fflush(stdout);
        sleep(1);
        printf("B\n");
        
        /* TODO: Post */
        
        sleep(1);
    }

    // Close
    /* TODO: sem_close */
    
    return 0;
}
