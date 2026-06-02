#define _GNU_SOURCE
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
    struct epoll_event ev, events[MAX_EVENTS];

    // 1. Create TCP socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 2. Bind
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 3. Listen
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // 4. Create epoll instance
    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    // 5. Add server_fd to epoll (level-triggered for accept)
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) < 0) {
        perror("epoll_ctl: server_fd");
        exit(EXIT_FAILURE);
    }

    printf("epoll server listening on port %d...\n", PORT);

    // 6. Event-driven loop
    while (1) {
        nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds < 0) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (i = 0; i < nfds; i++) {
            if (events[i].data.fd == server_fd) {
                // New connection
                if ((client_fd = accept(server_fd,
                                        (struct sockaddr *)&address,
                                        (socklen_t *)&addrlen)) < 0) {
                    perror("accept");
                    continue;
                }

                // Set non-blocking for edge-triggered mode
                int flags = fcntl(client_fd, F_GETFL, 0);
                fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

                // Add to epoll with edge-triggered
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) < 0) {
                    perror("epoll_ctl: client_fd");
                    close(client_fd);
                }

                char client_ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &address.sin_addr, client_ip, INET_ADDRSTRLEN);
                printf("New client: %s:%d\n", client_ip, ntohs(address.sin_port));
            } else {
                // Data from existing client
                client_fd = events[i].data.fd;
                while (1) {
                    memset(buffer, 0, BUFFER_SIZE);
                    int valread = read(client_fd, buffer, BUFFER_SIZE - 1);
                    if (valread > 0) {
                        printf("Received: %s", buffer);
                        send(client_fd, buffer, strlen(buffer), 0);
                    } else if (valread == 0) {
                        // Client disconnected
                        printf("Client disconnected (fd=%d)\n", client_fd);
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
                        close(client_fd);
                        break;
                    } else {
                        if (errno == EAGAIN) {
                            // No more data to read (edge-triggered)
                            break;
                        }
                        // Real error
                        perror("read");
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
                        close(client_fd);
                        break;
                    }
                }
            }
        }
    }

    close(server_fd);
    return 0;
}
