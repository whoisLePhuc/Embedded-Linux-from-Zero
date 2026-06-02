#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>

#define SOCKET_PATH "/tmp/echo.sock"
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE] = {0};
    socklen_t addrlen;

    // 1. Create UNIX domain socket
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Remove old socket file if it exists
    unlink(SOCKET_PATH);

    // 3. Configure and bind
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // Calculate exact address length (struct prefix + path string)
    addrlen = sizeof(sa_family_t) + strlen(addr.sun_path);

    if (bind(server_fd, (struct sockaddr *)&addr, addrlen) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 4. Listen
    if (listen(server_fd, 1) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("UNIX domain socket server listening at %s...\n", SOCKET_PATH);

    // 5. Accept a client connection
    if ((client_fd = accept(server_fd, NULL, NULL)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("Client connected!\n");

    // 6. Echo loop
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(client_fd, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            printf("Client disconnected.\n");
            break;
        }

        buffer[valread] = '\0';
        printf("Received: %s", buffer);

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Exit command received. Closing connection.\n");
            break;
        }

        send(client_fd, buffer, strlen(buffer), 0);
    }

    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}
