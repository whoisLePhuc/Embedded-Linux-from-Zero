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

static mqd_t mq;

static void notification_handler(union sigval sv) {
    char buffer[MAX_SIZE + 1];
    ssize_t bytes_read;
    struct sigevent sev;

    // Receive the message
    bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);
    if (bytes_read == -1) {
        if (errno != EAGAIN) {
            perror("Handler: mq_receive");
        }
        return;
    }

    buffer[bytes_read] = '\0';
    printf("[Notification] Received: %s\n", buffer);

    // Re-register notification for the next message
    // POSIX requires re-registration after each notification
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = notification_handler;
    sev.sigev_notify_attributes = NULL;
    sev.sigev_value.sival_ptr = NULL;

    if (mq_notify(mq, &sev) == -1) {
        perror("Handler: mq_notify re-register");
    }
}

int main() {
    struct mq_attr attr;
    struct sigevent sev;

    // Configure queue attributes
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    // Create the message queue
    mq = mq_open(QUEUE_NAME, O_RDONLY | O_CREAT | O_NONBLOCK, 0644, &attr);
    if (mq == (mqd_t)-1) {
        perror("Server: mq_open");
        exit(1);
    }

    printf("Server started. Waiting for messages...\n");

    // Configure sigevent for SIGEV_THREAD
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = notification_handler;
    sev.sigev_notify_attributes = NULL;
    // Pass mq descriptor to the callback via sival_ptr
    sev.sigev_value.sival_ptr = NULL;

    // Register for notifications
    if (mq_notify(mq, &sev) == -1) {
        perror("Server: mq_notify");
        mq_close(mq);
        mq_unlink(QUEUE_NAME);
        exit(1);
    }

    // Main loop - keep the process alive
    while (1) {
        pause();
    }

    // Cleanup (never reached)
    mq_close(mq);
    mq_unlink(QUEUE_NAME);

    return 0;
}
