#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>

int main() {
    key_t key;
    int shmid;
    char *str;

    // ftok to generate unique key
    key = ftok("shmfile", 65);
    if (key == -1) {
        perror("ftok (make sure 'shmfile' exists)");
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

    printf("Write Data : ");
    if (fgets(str, 1024, stdin) == NULL) {
        perror("fgets");
        exit(1);
    }
    
    // Remove newline
    str[strcspn(str, "\n")] = 0;

    printf("Data written in memory: %s\n", str);

    // detach from shared memory 
    shmdt(str);

    return 0;
}
