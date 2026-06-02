#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

int main() {
    key_t key;
    int shmid;
    char *str;

    // 1. Generate unique key
    /* TODO: ftok */
    // key = ftok("shmfile", 65);
    

    // 2. Create Shared Memory segment
    // shmget returns an identifier in shmid
    /* TODO: shmget size 1024 */
    

    // 3. Attach to Shared Memory
    // shmat to attach to shared memory
    /* TODO: shmat */
    

    // 4. Write Data
    printf("Write Data : ");
    // fgets(str, 1024, stdin);
    /* TODO: Nhập dữ liệu vào str */
    
    printf("Data written in memory: %s\n", str);

    // 5. Detach
    // shmdt(str);
    /* TODO: Detach */

    return 0;
}
