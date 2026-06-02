#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>

// structure for message queue
struct msg_buffer {
    long msg_type;
    char msg_text[100];
} message;

int main() {
    key_t key;
    int msgid;

    // ftok to generate unique key
    key = ftok("progfile", 65);
    if (key == -1) {
        perror("ftok error (Make sure 'progfile' exists)");
        exit(1);
    }

    // msgget creates a message queue and returns identifier
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    message.msg_type = 1;

    printf("Write Data : ");
    if (fgets(message.msg_text, 100, stdin) == NULL) {
        perror("fgets failed");
        exit(1);
    }

    // msgsnd to send message
    // sizeof(message) is actually larger than just the text part (100 in definition)
    // But msgsnd expects size of DATA part only.
    // Correct way: sizeof(message.msg_text) or sizeof(message) - sizeof(long)
    if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) {
        perror("msgsnd failed");
        exit(1);
    }

    printf("Data sent is : %s\n", message.msg_text);

    return 0;
}
