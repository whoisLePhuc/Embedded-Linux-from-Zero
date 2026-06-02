#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

// Định nghĩa struct message buffer
struct msg_buffer {
    long msg_type;
    char msg_text[100];
} message;

int main() {
    key_t key;
    int msgid;

    // 1. Tạo Key
    // key = ftok("progfile", 65);
    /* TODO: Tạo key bằng ftok. Đảm bảo file "progfile" tồn tại */
    

    // 2. Tạo Message Queue
    // msgid = msgget(key, 0666 | IPC_CREAT);
    /* TODO: Tạo queue id */
    

    // 3. Nhập dữ liệu
    printf("Write Data : ");
    // fgets(message.msg_text, 100, stdin); 
    /* TODO: Nhập dữ liệu vào cấu trúc message */

    // 4. Gửi message
    // message.msg_type = 1;
    // msgsnd(msgid, &message, sizeof(message), 0);
    /* TODO: Gửi message bằng msgsnd */

    printf("Data send is : %s \n", message.msg_text);

    return 0;
}
