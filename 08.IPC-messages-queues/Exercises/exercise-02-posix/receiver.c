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
    char buffer[MAX_SIZE + 1]; // +1 cho null-terminator
    struct mq_attr attr;

    // 1. Cấu hình Attribute (giống Sender)
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    // 2. Tạo hoặc mở Message Queue
    // mq = mq_open(QUEUE_NAME, O_RDONLY | O_CREAT, 0644, &attr);
    /* TODO: Mở queue để đọc (O_RDONLY) */
    

    // 3. Nhận dữ liệu
    /* TODO: Loop nhận tin nhắn và in ra */
    /* ssize_t bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL); */


    // 4. Cleanup
    /* TODO: Đóng và unlink queue */

    return 0;
}
