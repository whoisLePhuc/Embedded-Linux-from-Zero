#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

// Định nghĩa union semun nếu hệ thống chưa có (thường linux man page yêu cầu tự định nghĩa)
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void critical_section(int id) {
    for (int i = 0; i < 3; i++) {
        printf("Process %d inside Critical Section\n", id);
        sleep(1);
    }
}

int main() {
    key_t key;
    int semid;
    pid_t pid;

    // 1. Tạo Key & Semaphore
    /* TODO: ftok */
    
    /* TODO: semget 1 semaphore */
    

    // 2. Init Semaphore Value = 1
    /* TODO: semctl SETVAL 1 */
    

    // 3. Fork Process 1
    pid = fork();
    if (pid == 0) {
        // --- CHILD 1 ---
        /* TODO: P Operation (Wait) */
        
        critical_section(1);
        
        /* TODO: V Operation (Post) */
        
        exit(0);
    }

    // 4. Fork Process 2
    pid = fork();
    if (pid == 0) {
        // --- CHILD 2 ---
        /* TODO: P Operation (Wait) */
        
        critical_section(2);
        
        /* TODO: V Operation (Post) */
        
        exit(0);
    }

    // 5. Parent wait & clean
    wait(NULL);
    wait(NULL);

    /* TODO: semctl IPC_RMID */
    
    return 0;
}
