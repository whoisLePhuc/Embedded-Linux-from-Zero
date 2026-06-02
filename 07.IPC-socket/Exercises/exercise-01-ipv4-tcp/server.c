#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // 1. Tạo socket
    // int socket(int domain, int type, int protocol);
    /* TODO: Tạo socket file descriptor */
    

    // 2. Gán địa chỉ (Bind)
    // Cấu hình struct sockaddr_in
    /* TODO: Cấu hình address family, port, ip address */
    

    // 3. Listen
    /* TODO: Listen connection */
    

    printf("Server listening on port %d...\n", PORT);

    // 4. Accept
    /* TODO: Accept connection từ client */
    
    // In thông tin Client mới kết nối
    
    // 5. Read & Echo
    /* TODO: Loop đọc dữ liệu từ client và gửi lại (Echo) */
    

    return 0;
}
