#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t pid1, pid2;

    // 1. Create Pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // 2. Fork Child 1 (ls -l)
    pid1 = fork();
    if (pid1 < 0) {
        perror("fork 1");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // --- CHILD 1 ---
        // It writes to pipe, so close read end
        close(pipefd[0]);

        // Redirect stdout to pipe write end
        dup2(pipefd[1], STDOUT_FILENO);
        
        // Close write end (after dup2, we still have STDOUT pointing to pipe)
        close(pipefd[1]);

        // Execute "ls -l"
        execlp("ls", "ls", "-l", NULL);
        
        // If execlp returns, it failed
        perror("execlp ls failed");
        exit(EXIT_FAILURE);
    }

    // 3. Fork Child 2 (wc -l)
    pid2 = fork();
    if (pid2 < 0) {
        perror("fork 2");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        // --- CHILD 2 ---
        // It reads from pipe, so close write end
        close(pipefd[1]);

        // Redirect stdin to pipe read end
        dup2(pipefd[0], STDIN_FILENO);
        
        // Close read end (after dup2, we still have STDIN pointing to pipe)
        close(pipefd[0]);

        // Execute "wc -l"
        execlp("wc", "wc", "-l", NULL);

        // If execlp returns, it failed
        perror("execlp wc failed");
        exit(EXIT_FAILURE);
    }

    // 4. Parent Process
    // Parent must close pipe descriptors, otherwise Child 2 won't see EOF
    close(pipefd[0]);
    close(pipefd[1]);

    // Wait for children
    wait(NULL);
    wait(NULL);

    return 0;
}
