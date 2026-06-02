#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <mqueue.h>
#include <pthread.h>
#include <errno.h>

#define QUEUE_NAME  "/notify_queue"
#define MAX_SIZE    1024
#define MSG_STOP    "exit"

/* TODO: Khai báo prototype cho notification handler */

int main() {
    mqd_t mq;
    struct mq_attr attr;
    struct sigevent sev;

    // 1. Cấu hình Attribute
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    // 2. Tạo Message Queue
    mq = mq_open(QUEUE_NAME, O_RDONLY | O_CREAT | O_NONBLOCK, 0644, &attr);
    if (mq == (mqd_t)-1) {
        perror("Server: mq_open");
        exit(1);
    }
    /* TODO: Kiểm tra lỗi */

    printf("Server started. Waiting for messages...\n");

    // 3. Thiết lập sigevent cho SIGEV_THREAD
    /* TODO: Cấu hình sev.sigev_notify, sigev_notify_function, sigev_notify_attributes */
    /* TODO: Truyền mq descriptor vào callback qua sigev_value.sival_ptr */

    // 4. Đăng ký notification
    /* if (mq_notify(mq, &sev) == -1) { ... } */
    /* TODO: Gọi mq_notify để đăng ký */

    // 5. Vòng lặp chính - giữ server chạy
    /* TODO: while(1) pause() hoặc sleep */

    // 6. Cleanup (không bao giờ tới được nếu chạy vô hạn)
    mq_close(mq);
    mq_unlink(QUEUE_NAME);

    return 0;
}

/* TODO: Định nghĩa notification handler
 * static void notification_handler(union sigval sv) {
 *     mqd_t *mq_ptr = (mqd_t *)sv.sival_ptr;
 *     // Nhận tin nhắn
 *     // Re-register notification
 * }
 */
