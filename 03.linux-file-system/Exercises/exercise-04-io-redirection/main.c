#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    // TODO 1: Open (or create) the log file for writing
    // Use open() with O_WRONLY | O_CREAT | O_TRUNC, mode 0644
    // If open() fails, perror() and return 1



    // TODO 2: Flush stdout before changing the underlying fd
    // fflush(stdout) ensures no buffered data is lost during the redirect



    // TODO 3: Save the original stdout file descriptor
    // Use dup(STDOUT_FILENO) to duplicate the fd
    // Store the result in saved_stdout, check for errors



    // TODO 4: Redirect stdout to the log file
    // Use dup2(log_fd, STDOUT_FILENO) to redirect
    // After this, any write to fd 1 goes to the log file, not the terminal



    // TODO 5: Print some messages to stdout
    // These will be redirected to the log file
    // Use printf() for normal messages



    // TODO 6: Flush stdout before restoring the fd



    // TODO 7: Restore original stdout
    // Use dup2(saved_stdout, STDOUT_FILENO) to restore
    // Then close(saved_stdout) to release the duplicate



    // TODO 8: Repeat the save/redirect/restore pattern for stderr
    // First fflush(stderr) before changing its fd
    // Then save STDERR_FILENO with dup()
    // Redirect stderr to log_fd with dup2()
    // Print an error message with fprintf(stderr, ...)
    // Flush again, then restore stderr and close the saved fd



    // TODO 7: Close the log file
    // close(log_fd)



    printf("Done! Check %s for redirected output.\n", LOG_FILE);
    return 0;
}
