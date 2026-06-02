#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>

#define SEM_NAME "/test_sem"

int main() {
    sem_t *sem;

    // Create named semaphore
    sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    printf("Process A started...\n");

    for (int i = 0; i < 5; i++) {
        printf("A waiting...\n");
        sem_wait(sem);
        
        // Critical Section
        printf("AAAAA (Writing)\n");
        sleep(1); // Simulate work
        
        sem_post(sem);
        sleep(1);
    }

    sem_close(sem);
    sem_unlink(SEM_NAME); // Clean up
    
    return 0;
}
