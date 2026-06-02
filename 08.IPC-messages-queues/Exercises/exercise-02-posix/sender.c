#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define QUEUE_NAME  "/test_queue"
#define MAX_SIZE    1024
#define MSG_STOP    "exit"

int main() {
    mqd_t mq;
    char buffer[MAX_SIZE];
    struct mq_attr attr;

    // 1. Cấu hình Attribute
    attr.mq_flags = 0;
    // attr.mq_maxmsg = 10;
    // attr.mq_msgsize = MAX_SIZE;
    // attr.mq_curmsgs = 0;
    /* TODO: Cấu hình attribute */

    // 2. Tạo hoặc mở Message Queue
    // mq = mq_open(QUEUE_NAME, O_WRONLY | O_CREAT, 0644, &attr);
    /* TODO: Mở queue */
    

    // 3. Nhập và gửi dữ liệu
    printf("Send to receiver (enter 'exit' to stop):\n> ");
    /* TODO: Loop nhập và gửi tin nhắn */
    

    // 4. Đóng Queue
    /* TODO: Đóng queue */

    return 0;
}
