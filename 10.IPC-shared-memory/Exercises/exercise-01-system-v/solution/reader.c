#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

int main() {
    key_t key;
    int shmid;
    char *str;

    // ftok to generate unique key
    key = ftok("shmfile", 65);
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // shmget returns an identifier in shmid
    shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // shmat to attach to shared memory
    str = (char*) shmat(shmid, (void*)0, 0);
    if (str == (char*)(-1)) {
        perror("shmat");
        exit(1);
    }

    printf("Data read from memory: %s\n", str);

    // detach from shared memory 
    shmdt(str);

    // destroy the shared memory
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
