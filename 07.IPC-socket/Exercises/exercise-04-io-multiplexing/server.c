#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket, max_sd, activity, i, valread;
    int client_sockets[MAX_CLIENTS] = {0};
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // 1. Tao socket TCP
    // int socket(int domain, int type, int protocol);
    /* TODO: Tao socket file descriptor (AF_INET, SOCK_STREAM, 0) */

    // 2. Bind
    // Cau hinh struct sockaddr_in
    /* TODO: Cau hinh address family (AF_INET), port (htons(PORT)), ip (INADDR_ANY) */
    /* TODO: Goi bind() */

    // 3. Listen
    /* TODO: Goi listen() voi backlog = 3 */

    printf("Server listening on port %d...\n", PORT);

    // 4. Vong lap I/O multiplexing voi select()
    while (1) {
        fd_set readfds;
        FD_ZERO(&readfds);

        // Them server_fd vao fd_set
        /* TODO: FD_SET(server_fd, &readfds) */

        max_sd = server_fd;

        // Them cac client sockets vao fd_set
        /* TODO: Duyet mang client_sockets, FD_SET cac socket dang active */
        /* TODO: Cap nhat max_sd la gia tri socket lon nhat */

        // 5. select() - cho activity tren bat ky socket nao
        /* TODO: Goi select(max_sd + 1, &readfds, NULL, NULL, NULL) */

        // 6. Kiem tra activity tren server_fd (co ket noi moi)
        if (FD_ISSET(server_fd, &readfds)) {
            /* TODO: Accept ket noi moi */
            /* TODO: In thong tin client (IP:Port) */
            /* TODO: Them socket moi vao mang client_sockets */
        }

        // 7. Kiem tra activity tren cac client sockets
        /* TODO: Duyet client_sockets, neu FD_ISSET thi doc du lieu va echo lai */
        /* TODO: Neu client disconnect (read == 0), dong socket va xoa khoi mang */
    }

    close(server_fd);
    return 0;
}
