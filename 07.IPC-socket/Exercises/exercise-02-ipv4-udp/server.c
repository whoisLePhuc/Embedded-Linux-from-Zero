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
    struct sockaddr_in servaddr, cliaddr;

    // 1. Tạo socket (UDP)
    // int socket(int domain, int type, int protocol);
    /* TODO: Tạo socket file descriptor với SOCK_DGRAM */
    

    // 2. Cấu hình địa chỉ Server
    /* TODO: Cấu hình servaddr, ip INADDR_ANY, port 8080 */
    

    // 3. Bind socket với địa chỉ Server
    /* TODO: Bind socket */
    

    printf("UDP Server is running on port %d...\n", PORT);

    int len, n;
    len = sizeof(cliaddr); 
    
    // 4. Recv & Send Loop
    while (1) {
        /* TODO: Nhận message từ client bằng recvfrom */
        // n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *) &cliaddr, &len);
        
        /* TODO: In ra message nhận được và IP:Port của client */
        
        /* TODO: Gửi phản hồi lại cho client bằng sendto */
    }

    return 0;
}
