#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include "common.h"

int main() {
    int server_fd, client_fd;
    struct request req;
    struct response resp;
    int seq_num = 0;
    char client_fifo[CLIENT_FIFO_NAME_LEN];

    // 1. Tạo Well-known FIFO
    /* TODO: mkfifo SERVER_FIFO */
    mkfifo(SERVER_FIFO, 0666);

    // 2. Mở FIFO để đọc
    /* TODO: open SERVER_FIFO O_RDONLY */
    

    printf("Server started. Reading requests...\n");

    // 3. Loop xử lý request
    while (1) {
        // Đọc request từ FIFO
        /* TODO: read req from server_fd */
        /* Nếu read trả về 0 (EOF), có thể client đóng write-end, nhưng 
           với server FIFO mở liên tục thì nên cẩn thận logic blocking. 
           Đơn giản ở đây: nếu read lỗi thì break */

        
        // Tạo tên FIFO của client: snprintf(client_fifo, ..., CLIENT_FIFO_TEMPLATE, req.pid);
        
        // Mở FIFO client để ghi
        /* TODO: open client_fifo O_WRONLY */
        
        
        // Chuẩn bị response
        resp.seq_num = seq_num;
        
        // Gửi response
        /* TODO: write resp to client_fd */
        

        // Đóng FIFO client
        /* TODO: close client_fd */

        // Cập nhật seq_num cho request sau
        seq_num += req.seq_len;
        
        printf("Served Client PID %d, assigned seq %d\n", req.pid, resp.seq_num);
    }

    close(server_fd);
    unlink(SERVER_FIFO);
    return 0;
}
