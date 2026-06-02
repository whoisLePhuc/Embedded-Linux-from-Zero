#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define QUEUE_NAME  "/notify_queue"
#define MAX_SIZE    1024
#define MSG_STOP    "exit"

int main() {
    mqd_t mq;
    char buffer[MAX_SIZE];

    // 1. Mở Message Queue (đã được tạo bởi server)
    /* mq = mq_open(QUEUE_NAME, O_WRONLY); */
    /* TODO: Mở queue để ghi (O_WRONLY, không O_CREAT) */

    printf("Enter messages to send to server. Type 'exit' to stop.\n> ");

    // 2. Gửi tin nhắn
    while (fgets(buffer, MAX_SIZE, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;

        /* TODO: Gửi tin nhắn với mq_send (priority = 0) */

        if (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)) == 0) {
            break;
        }

        printf("> ");
    }

    // 3. Cleanup
    /* TODO: Đóng queue */
    /* mq_close(mq); */

    return 0;
}
