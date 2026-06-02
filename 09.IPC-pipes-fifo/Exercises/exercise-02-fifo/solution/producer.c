#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define FIFO_NAME "myfifo"
#define BUFFER_SIZE 1024

int main() {
    int fd;
    char buffer[BUFFER_SIZE];

    // Create FIFO
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        // Errors are fine if file exists
        // perror("mkfifo"); 
    }

    printf("Producer: Waiting for readers...\n");
    // Open FIFO for Write only
    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    printf("Producer: Connected. Type messages.\n");

    while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
        write(fd, buffer, strlen(buffer) + 1);
        
        // Remove newline for check
        buffer[strcspn(buffer, "\n")] = 0;
        if (strcmp(buffer, "exit") == 0) {
            break;
        }
    }

    close(fd);
    unlink(FIFO_NAME); // Remove FIFO file
    return 0;
}
