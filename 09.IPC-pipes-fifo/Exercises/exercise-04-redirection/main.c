#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t pid1, pid2;

    // 1. Tạo Pipe
    /* TODO: Tạo pipe */
    

    // 2. Fork Child 1 (ls -l)
    pid1 = fork();
    if (pid1 == 0) {
        // --- CHILD 1 ---
        // Input: Standard Input (giữ nguyên)
        // Output: Pipe Write End
        
        /* TODO: Đóng đầu đọc pipefd[0] (không dùng) */
        
        /* TODO: Dùng dup2 để thay stdout bằng pipefd[1] */
        
        /* TODO: Exec lệnh "ls -l" */
        // execlp("ls", "ls", "-l", NULL);
        
        perror("execlp ls failed");
        exit(1);
    }

    // 3. Fork Child 2 (wc -l)
    pid2 = fork();
    if (pid2 == 0) {
        // --- CHILD 2 ---
        // Input: Pipe Read End
        // Output: Standard Output (giữ nguyên)
        
        /* TODO: Đóng đầu ghi pipefd[1] (không dùng) */
        
        /* TODO: Dùng dup2 để thay stdin bằng pipefd[0] */
        
        /* TODO: Exec lệnh "wc -l" */
        // execlp("wc", "wc", "-l", NULL);

        perror("execlp wc failed");
        exit(1);
    }

    // 4. Parent Process
    /* TODO: Đóng cả 2 đầu pipe trong parent (CỰC KỲ QUAN TRỌNG) */
    /* Nếu parent không đóng pipefd[1], Child 2 sẽ không bao giờ nhận được EOF và treo mãi mãi */
    

    /* TODO: Wait cho 2 con */
    

    return 0;
}
