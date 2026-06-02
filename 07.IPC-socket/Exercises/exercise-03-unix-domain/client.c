#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>

#define SOCKET_PATH "/tmp/echo.sock"
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE];

    // 1. Tao UNIX domain socket
    /* TODO: Tao socket (AF_UNIX, SOCK_STREAM, 0) */

    // 2. Cau hinh dia chi server
    /* TODO: memset va cau hinh addr.sun_family, addr.sun_path */

    // 3. Connect toi server
    /* TODO: Goi connect() */

    printf("Connected to UNIX socket server at %s\n", SOCKET_PATH);

    // 4. Send/Receive loop
    while (1) {
        printf("Enter message (or 'exit' to quit): ");
        /* TODO: Doc du lieu tu ban phim bang fgets() */

        /* TODO: Xu ly newline tu fgets */

        /* TODO: Neu nguoi dung nhap "exit", thoat */

        /* TODO: Gui du lieu toi server */

        /* TODO: Nhan phan hoi tu server va in ra */
    }

    close(sock);
    return 0;
}
