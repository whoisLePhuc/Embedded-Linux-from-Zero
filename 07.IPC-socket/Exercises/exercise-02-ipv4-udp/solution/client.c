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

    // 1. Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // 2. Server info
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    // servaddr.sin_addr.s_addr = INADDR_ANY; 
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

    int n; 
    socklen_t len;

    // 3. Loop
    while (1) {
        printf("Enter message: ");
        if (fgets(message, BUFFER_SIZE, stdin) == NULL) break;
        
        // Remove newline
        message[strcspn(message, "\n")] = 0;
        
        // Send to server
        sendto(sockfd, (const char *)message, strlen(message), 
              MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
              
        // Receive from server
        // Note: In UDP, we can receive from anyone, but here we expect from server.
        // We pass NULL to address if we don't care who sent it, but good practice to check.
        n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, 
                    MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
        buffer[n] = '\0';
        printf("Server : %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
