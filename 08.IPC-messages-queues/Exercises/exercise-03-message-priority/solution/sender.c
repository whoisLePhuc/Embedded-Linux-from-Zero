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

    printf("Enter messages with priority (0-31). Type 'exit' to stop.\n");
    printf("Format: <priority> <message>\n> ");

    while (fgets(buffer, MAX_SIZE, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;

        // Parse priority and message from input
        char *prio_str = strtok(buffer, " ");
        char *msg = strtok(NULL, "");
        if (msg == NULL) {
            // Only priority entered, use empty message
            msg = "";
        }

        priority = (unsigned int)atoi(prio_str);
        if (priority > 31) priority = 31;

        // Send message with specified priority
        if (mq_send(mq, msg, strlen(msg) + 1, priority) == -1) {
            perror("Sender: mq_send");
            exit(1);
        }
        printf("Sent [Priority %u]: %s\n", priority, msg);

        if (strncmp(msg, MSG_STOP, strlen(MSG_STOP)) == 0) {
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
