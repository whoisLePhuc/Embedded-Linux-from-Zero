#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];
    struct sockaddr_in servaddr;

    // 1. Tạo socket (UDP)
    /* TODO: Tạo socket SOCK_DGRAM */
    

    // 2. Cấu hình địa chỉ Server
    /* TODO: Cấu hình servaddr, ip 127.0.0.1, port 8080 */
    

    int n, len;
    len = sizeof(servaddr);

    // 3. Send & Recv Loop
    while (1) {
        printf("Enter message: ");
        /* TODO: Nhập message từ phím */
        
        /* TODO: Gửi message tới server bằng sendto */
        
        /* TODO: Nhận phản hồi từ server bằng recvfrom */

        printf("Server : %s\n", buffer);
    }
    
    close(sockfd);
    return 0;
}
