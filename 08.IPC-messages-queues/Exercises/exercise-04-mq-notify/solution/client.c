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

    // Open the message queue (created by server)
    mq = mq_open(QUEUE_NAME, O_WRONLY);
    if (mq == (mqd_t)-1) {
        perror("Client: mq_open - make sure server is running first");
        exit(1);
    }

    printf("Enter messages to send to server. Type 'exit' to stop.\n> ");

    while (fgets(buffer, MAX_SIZE, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;

        if (mq_send(mq, buffer, strlen(buffer) + 1, 0) == -1) {
            perror("Client: mq_send");
            exit(1);
        }
        printf("Sent: %s\n", buffer);

        if (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)) == 0) {
            break;
        }

        printf("> ");
    }

    mq_close(mq);

    return 0;
}
