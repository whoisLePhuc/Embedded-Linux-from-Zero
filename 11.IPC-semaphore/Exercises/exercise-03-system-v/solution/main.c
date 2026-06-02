#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

// Must define semun explicitly on most Linux systems
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void p_op(int semid) {
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1; // Decrement
    sb.sem_flg = SEM_UNDO; // Auto release if process crashes
    semop(semid, &sb, 1);
}

void v_op(int semid) {
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = 1; // Increment
    sb.sem_flg = SEM_UNDO;
    semop(semid, &sb, 1);
}

void critical_section(int id) {
    printf("Process %d ENTERING Critical Section\n", id);
    for (int i = 0; i < 3; i++) {
        printf("  Process %d working...\n", id);
        sleep(1);
    }
    printf("Process %d LEAVING Critical Section\n", id);
}

int main() {
    key_t key;
    int semid;
    pid_t pid;

    // Generate unique key
    // Ensure "semfile" exists or use current dir "."
    key = ftok(".", 65); 
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // Create semaphore set with 1 semaphore
    semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    // Initialize value to 1
    union semun arg;
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("semctl init");
        exit(1);
    }

    printf("Semaphore initialized to 1.\n");

    // Process 1
    if ((pid = fork()) == 0) {
        p_op(semid);
        critical_section(1);
        v_op(semid);
        exit(0);
    }

    // Process 2
    if ((pid = fork()) == 0) {
        p_op(semid);
        critical_section(2);
        v_op(semid);
        exit(0);
    }

    // Parent waits
    wait(NULL);
    wait(NULL);

    // Remove semaphore
    semctl(semid, 0, IPC_RMID);
    printf("Semaphore removed.\n");

    return 0;
}
