#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <limits.h>

#define BUF_SIZE 4096

ssize_t test_pipe_capacity(int write_fd) {
    char buf[1024] = {0};
    ssize_t total = 0;
    ssize_t n;

    printf("Testing pipe buffer capacity (non-blocking)...\n");
    while (1) {
        n = write(write_fd, buf, sizeof(buf));
        if (n == -1) {
            if (errno == EAGAIN) {
                printf("Pipe full after writing %ld bytes\n", (long)total);
                break;
            }
            perror("write");
            return -1;
        }
        total += n;
    }
    return total;
}

int main() {
    int fd[2];
    pid_t pid;

    // Create pipe
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    printf("=== Non-blocking Pipe Demo ===\n\n");

    // 1. Non-blocking read demo
    int flags = fcntl(fd[0], F_GETFL);
    fcntl(fd[0], F_SETFL, flags | O_NONBLOCK);

    char buf[BUF_SIZE];
    ssize_t n = read(fd[0], buf, sizeof(buf));
    if (n == -1 && errno == EAGAIN) {
        printf("[Part 1] Non-blocking read on empty pipe: EAGAIN (correct!)\n\n");
    }

    // 2. Pipe buffer size
    printf("[Part 2] PIPE_BUF (guaranteed minimum): %d\n", _POSIX_PIPE_BUF);
    long pipe_buf = fpathconf(fd[1], _PC_PIPE_BUF);
    if (pipe_buf != -1) {
        printf("PIPE_BUF from fpathconf: %ld\n", pipe_buf);
    }

    // Temporarily set write end to non-blocking for capacity test
    flags = fcntl(fd[1], F_GETFL);
    fcntl(fd[1], F_SETFL, flags | O_NONBLOCK);
    test_pipe_capacity(fd[1]);
    // Reset write to blocking
    fcntl(fd[1], F_SETFL, flags);

    printf("\n[Part 3] select() demo with timeout\n");

    // 3. select() demo
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        // Child: write data after 1 second
        close(fd[0]); // close unused read end
        sleep(1);
        const char *msg = "Hello from child via select!";
        write(fd[1], msg, strlen(msg) + 1);
        close(fd[1]);
        exit(0);
    }

    // Parent: use select() to wait for data
    close(fd[1]); // close write end

    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(fd[0], &rfds);

    struct timeval tv = {5, 0}; // 5 second timeout
    printf("Waiting for data from child (timeout: 5s)...\n");

    int ret = select(fd[0] + 1, &rfds, NULL, NULL, &tv);
    if (ret == -1) {
        perror("select");
    } else if (ret == 0) {
        printf("Timeout: no data received in 5 seconds\n");
    } else {
        if (FD_ISSET(fd[0], &rfds)) {
            char msg[BUF_SIZE] = {0};
            read(fd[0], msg, sizeof(msg));
            printf("Received via select: %s\n", msg);
        }
    }

    close(fd[0]);
    wait(NULL);

    return 0;
}
