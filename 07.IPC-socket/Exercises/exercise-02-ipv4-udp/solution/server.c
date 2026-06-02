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

    // 1. Create socket (UDP)
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // 2. Configure Server Address
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // 3. Bind the socket
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP Server is running on port %d...\n", PORT);

    socklen_t len;
    int n;

    len = sizeof(cliaddr); 

    // 4. Loop
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        // Receive message from client
        n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, 
                    MSG_WAITALL, (struct sockaddr *) &cliaddr, &len);
        buffer[n] = '\0';
        
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &cliaddr.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("Client %s:%d : %s\n", client_ip, ntohs(cliaddr.sin_port), buffer);
        
        // Send response
        char *response = "Hello from Server";
        sendto(sockfd, (const char *)response, strlen(response), 
              MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
    }
    
    return 0;
}
