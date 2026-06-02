#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main() {
    int pipefd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];
    char *message = "Hello Child, I am Parent!";

    // 1. Create Pipe
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    // 2. Fork
    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // --- PARENT PROCESS ---
        // Close unused Read end
        close(pipefd[0]);

        // Write to pipe
        printf("Parent writing to pipe...\n");
        write(pipefd[1], message, strlen(message) + 1); // +1 for null terminator

        // Close Write end (sends EOF to reader)
        close(pipefd[1]);

        // Wait for child
        wait(NULL);

    } else {
        // --- CHILD PROCESS ---
        // Close unused Write end
        close(pipefd[1]);

        // Read from pipe
        ssize_t nbytes = read(pipefd[0], buffer, BUFFER_SIZE);
        if (nbytes > 0) {
            printf("Child received: %s\n", buffer);
        } else {
            printf("Child received nothing or error.\n");
        }

        // Close Read end
        close(pipefd[0]);
    }

    return 0;
}
