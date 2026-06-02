#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct msg_buffer {
    long msg_type;
    char msg_text[100];
} message;

int main() {
    key_t key;
    int msgid;

    // 1. Tạo Key (Giống Sender)
    /* TODO: Tạo key bằng ftok */
    

    // 2. Lấy Message Queue ID
    // msgid = msgget(key, 0666 | IPC_CREAT);
    /* TODO: Lấy queue id */
    

    // 3. Nhận Message
    // msgrcv(msgid, &message, sizeof(message), 1, 0);
    /* TODO: Nhận message có type 1 */
    

    printf("Data Received is : %s \n", message.msg_text);

    // 4. Xóa Message Queue
    // msgctl(msgid, IPC_RMID, NULL);
    /* TODO: Xóa queue */
    

    return 0;
}
