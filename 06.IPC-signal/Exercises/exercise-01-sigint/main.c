#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/*
 * TODO 1: Viết hàm signal handler cho SIGINT
 * Hàm này sẽ được gọi khi bạn nhấn Ctrl+C
 */
void sigint_handler(int signum) {
    // Code xử lý signal ở đây
    // In ra thông báo: "\nCaught SIGINT! I will not exit via Ctrl+C..."
}

int main() {
    /*
     * TODO 2: Lấy PID hiện tại và in ra màn hình
     * Gợi ý: dùng getpid()
     */
     printf("My PID: %d\n", getpid());


    /*
     * TODO 3: Đăng ký signal handler cho SIGINT
     * Sử dụng hàm signal(SIGINT, ...)
     * Kiểm tra lỗi nếu signal trả về SIG_ERR
     */
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("Cannot handle SIGINT");
        return EXIT_FAILURE;
    }

    printf("Program is running. Try pressing Ctrl+C...\n");

    /*
     * TODO 4: Tạo vòng lặp vô hạn
     * In ra "Program is running..." và sleep(2)
     */
    while (1) {
        printf("Program is running...\n");
        sleep(2);
    }

    return 0;
}
