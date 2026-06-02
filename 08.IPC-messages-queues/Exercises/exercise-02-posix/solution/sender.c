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

    // Initialize the queue attributes
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    // Create the message queue
    mq = mq_open(QUEUE_NAME, O_WRONLY | O_CREAT, 0644, &attr);
    if (mq == (mqd_t)-1) {
        perror("Sender: mq_open");
        exit(1);
    }

    printf("Send to receiver (enter 'exit' to stop):\n> ");

    while (fgets(buffer, MAX_SIZE, stdin) != NULL) {
        // Remove newline
        buffer[strcspn(buffer, "\n")] = 0;
        
        // Send message
        // Priority 0 is default
        if (mq_send(mq, buffer, strlen(buffer) + 1, 0) == -1) {
            perror("Sender: mq_send");
            exit(1);
        }

        if (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)) == 0) {
            break;
        }

        printf("> ");
    }

    if (mq_close(mq) == -1) {
        perror("Sender: mq_close");
        exit(1);
    }

    return 0;
}
