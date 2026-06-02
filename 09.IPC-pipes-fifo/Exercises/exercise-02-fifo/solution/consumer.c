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

    // Ensure FIFO exists
    mkfifo(FIFO_NAME, 0666);

    printf("Consumer: Waiting for writers...\n");
    // Open FIFO for Read only
    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    printf("Consumer: Connected.\n");

    while (1) {
        int bytes_read = read(fd, buffer, BUFFER_SIZE);
        if (bytes_read > 0) {
            printf("Received: %s", buffer);
            if (strncmp(buffer, "exit", 4) == 0) {
                break;
            }
        } else {
            // EOF or Error
            break;
        }
    }

    close(fd);
    return 0;
}
