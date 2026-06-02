#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    printf("==============================\n");
    printf("    Process Information\n");
    printf("==============================\n");
    
    // Process IDs
    printf("PID:           %d\n", getpid());
    printf("Parent PID:    %d\n", getppid());
    
    // User IDs
    printf("User ID:       %d\n", getuid());
    printf("Effective UID: %d\n", geteuid());
    
    // Group IDs
    printf("Group ID:      %d\n", getgid());
    printf("Effective GID: %d\n", getegid());
    
    printf("==============================\n");
    
    return 0;
}
