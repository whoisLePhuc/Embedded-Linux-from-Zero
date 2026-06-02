#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define PORT 8080
#define MAX_EVENTS 64
#define BUFFER_SIZE 1024

int main() {
    int server_fd, epoll_fd, i, nfds, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    // 1. Tao socket TCP
    /* TODO: Tao socket (AF_INET, SOCK_STREAM, 0) */

    // 2. Bind
    /* TODO: Cau hinh sockaddr_in va goi bind() */

    // 3. Listen
    /* TODO: Goi listen() */

    // 4. Tao epoll instance
    /* TODO: epoll_create1(0) */

    // 5. Them server_fd vao epoll
    /* TODO: epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) */
    /* ev.events = EPOLLIN; ev.data.fd = server_fd; */

    printf("epoll server listening on port %d...\n", PORT);

    // 6. Vong lap event-driven
    while (1) {
        /* TODO: epoll_wait(epoll_fd, events, MAX_EVENTS, -1) */

        // TODO: Duyet cac event
        // if (events[i].data.fd == server_fd) -> accept, set non-blocking, them vao epoll voi EPOLLET
        // else -> doc du lieu tu client socket, echo lai
        // Doc den khi gap EAGAIN (voi socket non-blocking edge-triggered)
        // Neu read() == 0 -> client ngat ket noi, dong socket, xoa khoi epoll
    }

    close(server_fd);
    return 0;
}
