#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

int main() {
    key_t key;
    int shmid;
    char *str;

    // 1. Key
    /* TODO: ftok (giống writer) */
    

    // 2. Get Shared Memory ID
    /* TODO: shmget */
    

    // 3. Attach
    /* TODO: shmat */
    

    // 4. Read Data
    // printf("Data read from memory: %s\n", str);
    /* TODO: In dữ liệu từ str */
    

    // 5. Detach & Remove
    /* TODO: shmdt */
    
    /* TODO: shmctl IPC_RMID */

    return 0;
}
