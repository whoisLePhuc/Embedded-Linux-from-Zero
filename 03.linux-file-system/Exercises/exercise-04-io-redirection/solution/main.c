#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define LOG_FILE "output.log"

void print_banner(void) {
    printf("=== I/O Redirection Demo ===\n");
    printf("This program demonstrates dup() and dup2()\n\n");
}

int main(void) {
    int log_fd;
    int saved_stdout, saved_stderr;

    print_banner();

    // Open (or create) the log file for writing
    log_fd = open(LOG_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (log_fd == -1) {
        perror("open");
        return 1;
    }

    // Flush stdout before changing the underlying fd
    if (fflush(stdout) == EOF) {
        perror("fflush stdout");
        close(log_fd);
        return 1;
    }

    // Save the original stdout file descriptor
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1) {
        perror("dup stdout");
        close(log_fd);
        return 1;
    }

    // Redirect stdout to the log file
    if (dup2(log_fd, STDOUT_FILENO) == -1) {
        perror("dup2 stdout");
        close(saved_stdout);
        close(log_fd);
        return 1;
    }

    // Print messages to redirected stdout
    printf("=== Redirected Output ===\n");
    printf("This text is written to %s\n", LOG_FILE);
    printf("Using dup2() to redirect file descriptors\n");
    printf("stdout (fd 1) now points to the file\n");
    printf("Process PID: %d\n", getpid());
    printf("Current time: %ld\n", (long)time(NULL));

    // Flush stdout again before restoring fd
    if (fflush(stdout) == EOF) {
        perror("fflush stdout before restore");
        close(saved_stdout);
        close(log_fd);
        return 1;
    }

    // Restore original stdout
    if (dup2(saved_stdout, STDOUT_FILENO) == -1) {
        perror("restore stdout");
        close(saved_stdout);
        close(log_fd);
        return 1;
    }
    close(saved_stdout);

    // Flush stderr before changing its fd
    if (fflush(stderr) == EOF) {
        perror("fflush stderr");
        close(log_fd);
        return 1;
    }

    // Save original stderr
    saved_stderr = dup(STDERR_FILENO);
    if (saved_stderr == -1) {
        perror("dup stderr");
        close(log_fd);
        return 1;
    }

    // Redirect stderr to the same log file
    if (dup2(log_fd, STDERR_FILENO) == -1) {
        perror("dup2 stderr");
        close(saved_stderr);
        close(log_fd);
        return 1;
    }

    // Print error message to redirected stderr
    fprintf(stderr, "=== Redirected Stderr ===\n");
    fprintf(stderr, "This error message goes to %s\n", LOG_FILE);
    fprintf(stderr, "stderr has been redirected using dup2()\n");

    // Flush stderr before restoring its fd
    if (fflush(stderr) == EOF) {
        perror("fflush stderr before restore");
        close(saved_stderr);
        close(log_fd);
        return 1;
    }

    // Restore original stderr
    if (dup2(saved_stderr, STDERR_FILENO) == -1) {
        perror("restore stderr");
        close(saved_stderr);
        close(log_fd);
        return 1;
    }
    close(saved_stderr);

    // Close the log file
    close(log_fd);

    // This message goes to terminal (stdout is restored)
    printf("\nDone! Check %s for redirected output.\n", LOG_FILE);
    return 0;
}
