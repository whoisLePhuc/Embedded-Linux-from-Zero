#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include "../common.h"

int main() {
    int server_fd, client_fd;
    struct request req;
    struct response resp;
    int seq_num = 0;
    char client_fifo[CLIENT_FIFO_NAME_LEN];

    // Create server FIFO
    if (mkfifo(SERVER_FIFO, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    // Open server FIFO for reading
    // Note: This blocks until a client opens it for writing
    // Ideally open with O_RDWR to prevent EOF when last client closes it
    server_fd = open(SERVER_FIFO, O_RDONLY); 
    if (server_fd == -1) {
        perror("open server fifo");
        exit(EXIT_FAILURE);
    }
    
    // Trick: open a dummy helper fd to write to server fifo so it never sees EOF
    int dummy_fd = open(SERVER_FIFO, O_WRONLY);

    printf("Server started. Reading requests...\n");

    while (1) {
        if (read(server_fd, &req, sizeof(struct request)) != sizeof(struct request)) {
            fprintf(stderr, "Error reading request or partial read\n");
            continue; // or break
        }

        snprintf(client_fifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, req.pid);

        client_fd = open(client_fifo, O_WRONLY);
        if (client_fd == -1) {
            fprintf(stderr, "Cannot open client fifo %s\n", client_fifo);
            continue;
        }

        resp.seq_num = seq_num;
        
        if (write(client_fd, &resp, sizeof(struct response)) != sizeof(struct response)) {
             fprintf(stderr, "Error writing to client fifo %s\n", client_fifo);
        }

        if (close(client_fd) == -1) {
            perror("close client fifo");
        }

        seq_num += req.seq_len;
        printf("Served Client PID %d, assigned seq %d, next seq %d\n", req.pid, resp.seq_num, seq_num);
    }
    
    close(dummy_fd);
    close(server_fd);
    unlink(SERVER_FIFO);
    return 0;
}
