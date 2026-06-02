#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main() {
    int pipefd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];
    char *message = "Hello Child, I am Parent!";

    // 1. Tạo Pipe
    // int pipe(int pipefd[2]);
    /* TODO: Tạo pipe và kiểm tra lỗi */
    

    // 2. Fork process
    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid > 0) {
        // --- PARENT PROCESS ---
        /* TODO: Parent chỉ ghi, nên đóng đầu đọc pipefd[0] */
        

        /* TODO: Ghi message vào pipefd[1] */
        

        /* TODO: Đóng đầu ghi pipefd[1] sau khi ghi xong */
        

        /* TODO: Wait cho con kết thúc */
        wait(NULL);

    } else {
        // --- CHILD PROCESS ---
        /* TODO: Child chỉ đọc, nên đóng đầu ghi pipefd[1] */
        

        /* TODO: Đọc dữ liệu từ pipefd[0] vào buffer */
        

        /* In ra màn hình */
        // printf("Child received: %s\n", buffer);

        /* TODO: Đóng đầu đọc pipefd[0] */
        
    }

    return 0;
}
