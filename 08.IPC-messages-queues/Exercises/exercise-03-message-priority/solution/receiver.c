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

    // Initialize the queue attributes
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    // Create or Open the message queue
    mq = mq_open(QUEUE_NAME, O_RDONLY | O_CREAT, 0644, &attr);
    if (mq == (mqd_t)-1) {
        perror("Receiver: mq_open");
        exit(1);
    }

    printf("Waiting for messages (ordered by priority)...\n");

    while (1) {
        ssize_t bytes_read;

        // Receive message with priority
        bytes_read = mq_receive(mq, buffer, MAX_SIZE, &priority);
        if (bytes_read == -1) {
            perror("Receiver: mq_receive");
            exit(1);
        }

        buffer[bytes_read] = '\0';
        printf("[Priority %u] %s\n", priority, buffer);

        if (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)) == 0) {
            break;
        }
    }

    if (mq_close(mq) == -1) {
        perror("Receiver: mq_close");
        exit(1);
    }

    if (mq_unlink(QUEUE_NAME) == -1) {
        perror("Receiver: mq_unlink");
        exit(1);
    }

    return 0;
}
