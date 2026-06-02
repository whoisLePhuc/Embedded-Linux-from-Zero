#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include "common.h"

int main(int argc, char *argv[]) {
    int server_fd, client_fd;
    struct request req;
    struct response resp;
    char client_fifo[CLIENT_FIFO_NAME_LEN];

    if (argc < 2) {
        printf("Usage: %s <seq_len>\n", argv[0]);
        return 1;
    }

    // 1. Chuẩn bị Request
    req.pid = getpid();
    req.seq_len = atoi(argv[1]);

    // 2. Tạo Private FIFO cho Client
    snprintf(client_fifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, req.pid);
    /* TODO: mkfifo client_fifo */
    

    // 3. Gửi Request tới Server
    /* TODO: open SERVER_FIFO O_WRONLY */
    
    /* TODO: write req to server_fd */
    
    /* TODO: close server_fd (đã gửi xong) */


    // 4. Nhận Response từ Server
    /* TODO: open client_fifo O_RDONLY */
    
    /* TODO: read resp from client_fd */
    

    // 5. In kết quả và dọn dẹp
    printf("Client [%d] received seq: %d\n", req.pid, resp.seq_num);

    close(client_fd);
    unlink(client_fifo); // Xóa FIFO cá nhân
    return 0;
}
