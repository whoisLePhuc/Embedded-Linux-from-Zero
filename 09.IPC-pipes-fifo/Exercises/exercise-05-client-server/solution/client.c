#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include "../common.h"

int main(int argc, char *argv[]) {
    int server_fd, client_fd;
    struct request req;
    struct response resp;
    char client_fifo[CLIENT_FIFO_NAME_LEN];

    if (argc < 2) {
        printf("Usage: %s <seq_len>\n", argv[0]);
        return EXIT_FAILURE;
    }

    req.pid = getpid();
    req.seq_len = atoi(argv[1]);

    // Create client FIFO
    snprintf(client_fifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, req.pid);
    
    if (mkfifo(client_fifo, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        return EXIT_FAILURE;
    }

    // Send request to server
    server_fd = open(SERVER_FIFO, O_WRONLY);
    if (server_fd == -1) {
        perror("open server fifo");
        unlink(client_fifo);
        return EXIT_FAILURE;
    }

    if (write(server_fd, &req, sizeof(struct request)) != sizeof(struct request)) {
        perror("write to server");
        close(server_fd);
        unlink(client_fifo);
        return EXIT_FAILURE;
    }
    
    // Done writing request
    close(server_fd);

    // Open client FIFO to read response
    client_fd = open(client_fifo, O_RDONLY);
    if (client_fd == -1) {
         perror("open client fifo");
         unlink(client_fifo);
         return EXIT_FAILURE;
    }

    if (read(client_fd, &resp, sizeof(struct response)) != sizeof(struct response)) {
        perror("read response");
        close(client_fd);
        unlink(client_fifo);
        return EXIT_FAILURE;
    }

    printf("Client [%d] received seq: %d\n", req.pid, resp.seq_num);

    close(client_fd);
    unlink(client_fifo);
    return EXIT_SUCCESS;
}
