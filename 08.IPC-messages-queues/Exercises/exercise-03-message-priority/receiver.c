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
    char buffer[MAX_SIZE + 1];
    struct mq_attr attr;
    unsigned int priority;

    // 1. Cấu hình Attribute (giống Sender)
    attr.mq_flags = 0;
    // attr.mq_maxmsg = 10;
    // attr.mq_msgsize = MAX_SIZE;
    // attr.mq_curmsgs = 0;
    /* TODO: Cấu hình attribute */

    // 2. Tạo hoặc mở Message Queue
    // mq = mq_open(QUEUE_NAME, O_RDONLY | O_CREAT, 0644, &attr);
    /* TODO: Mở queue để đọc (O_RDONLY) */

    printf("Waiting for messages (ordered by priority)...\n");

    // 3. Nhận tin nhắn và hiển thị cùng priority
    while (1) {
        ssize_t bytes_read;

        /* TODO: Nhận tin nhắn với mq_receive, lấy priority */
        /* bytes_read = mq_receive(mq, buffer, MAX_SIZE, &priority); */

        buffer[bytes_read] = '\0';
        /* TODO: In nội dung và priority */
        /* printf("[Priority %u] %s\n", priority, buffer); */

        /* TODO: Kiểm tra nếu là "exit" thì break */
    }

    // 4. Cleanup
    /* TODO: Đóng queue và unlink */
    /* mq_close(mq); */
    /* mq_unlink(QUEUE_NAME); */

    return 0;
}
