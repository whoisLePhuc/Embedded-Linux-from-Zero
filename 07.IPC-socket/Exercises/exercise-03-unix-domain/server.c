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

    // 1. Tao UNIX domain socket
    // int socket(int domain, int type, int protocol);
    /* TODO: Tao socket (AF_UNIX, SOCK_STREAM, 0) */

    // 2. Xoa file socket cu neu ton tai
    /* TODO: Goi unlink(SOCKET_PATH) de xoa file cu */

    // 3. Cau hinh va Bind
    /* TODO: Cau hinh addr.sun_family = AF_UNIX, copy SOCKET_PATH vao addr.sun_path */
    /* TODO: Goi bind() */
    /* Goi y: memset(&addr, 0, sizeof(addr)) truoc khi cau hinh */

    // 4. Listen
    /* TODO: Goi listen() */

    printf("UNIX domain socket server listening at %s...\n", SOCKET_PATH);

    // 5. Accept
    /* TODO: Goi accept() */

    printf("Client connected!\n");

    // 6. Echo loop
    /* TODO: Doc du lieu tu client va echo lai */
    /* TODO: Neu nhan duoc "exit" hoac read() == 0, thoat */

    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}
