#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/*
 * Signal handler for SIGINT
 */
void sigint_handler(int signum) {
    // Tránh dùng printf trong signal handler thực tế vì nó không reentrant safe,
    // nhưng ở đây dùng để demo cho đơn giản.
    // Trong thực tế nên dùng write() hoặc set flag.
    printf("\nCaught SIGINT (%d)! I will not exit via Ctrl+C. Try Ctrl+\\ or 'kill' command.\n", signum);
}

int main() {
    // Print PID
    printf("My PID: %d\n", getpid());

    // Register signal handler with Error Checking
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("Unable to catch SIGINT");
        exit(1);
    }

    printf("Program is running. Try pressing Ctrl+C...\n");

    // Infinite loop
    while (1) {
        printf("Program is running...\n");
        sleep(2);
    }

    return 0;
}
