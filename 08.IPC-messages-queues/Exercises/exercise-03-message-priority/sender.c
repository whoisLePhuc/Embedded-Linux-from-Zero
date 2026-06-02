#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define QUEUE_NAME  "/msg_queue"
#define MAX_SIZE    1024
#define MSG_STOP    "exit"

int main() {
    mqd_t mq;
    char buffer[MAX_SIZE];
    struct mq_attr attr;
    unsigned int priority;

    // 1. Cấu hình Attribute
    attr.mq_flags = 0;
    // attr.mq_maxmsg = 10;
    // attr.mq_msgsize = MAX_SIZE;
    // attr.mq_curmsgs = 0;
    /* TODO: Cấu hình attribute */

    // 2. Tạo hoặc mở Message Queue
    // mq = mq_open(QUEUE_NAME, O_WRONLY | O_CREAT, 0644, &attr);
    /* TODO: Mở queue với O_WRONLY | O_CREAT */

    printf("Enter messages with priority (0-31). Type 'exit' to stop.\n");
    printf("Format: <priority> <message>\n> ");

    // 3. Loop nhập và gửi tin nhắn với priority
    /* TODO: Đọc input, phân tích priority và nội dung, gọi mq_send */
    while (fgets(buffer, MAX_SIZE, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;

        /* TODO: Phân tích priority từ input */
        /* Ví dụ: "10 Hello World" -> priority=10, message="Hello World" */

        /* TODO: Gửi tin nhắn với mq_send */
        /* if (mq_send(mq, msg, strlen(msg) + 1, priority) == -1) { ... } */

        /* TODO: Kiểm tra nếu là "exit" thì break */

        printf("> ");
    }

    // 4. Đóng Queue
    /* TODO: mq_close */

    return 0;
}
