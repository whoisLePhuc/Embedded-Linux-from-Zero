#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE];

    // 1. Tạo socket
    /* TODO: Tạo socket */
    

    // 2. Cấu hình địa chỉ Server để connect
    /* TODO: Cấu hình serv_addr, ip 127.0.0.1, port 8080 */
    

    // 3. Connect tới Server
    /* TODO: Connect */
    

    printf("Connected to Server!\n");

    // 4. Send & Receive loop
    while (1) {
        printf("Enter message: ");
        /* TODO: Nhập dữ liệu từ bàn phím */
        
        /* TODO: Gửi tới server */
        
        /* TODO: Nhận phản hồi từ server và in ra */
    }

    return 0;
}
