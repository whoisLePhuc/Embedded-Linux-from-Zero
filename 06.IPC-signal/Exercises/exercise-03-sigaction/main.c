#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/*
 * TODO 1: Viết hàm signal handler kiểu mới (sa_sigaction)
 * Prototype: void handler(int signum, siginfo_t *info, void *context)
 */
void sigusr1_handler(int signum, siginfo_t *info, void *context) {
    /*
     * TODO 2: In ra số hiệu signal và PID của người gửi (sender)
     * Sender PID nằm trong info->si_pid
     */
    
    // (void)context; // Dòng này đễ tránh warning unused parameter
}

int main() {
    struct sigaction sa;

    printf("My PID: %d\n", getpid());

    /*
     * TODO 3: Cấu hình struct sigaction
     * - sa_sigaction: trỏ đến hàm handler ở trên
     * - sa_flags: phải có SA_SIGINFO
     */
     

    /*
     * TODO 4: Đăng ký handler cho SIGUSR1
     * Dùng sigaction(SIGUSR1, &sa, NULL)
     */
    if (sigaction(SIGUSR1, &sa, NULL) < 0) {
        perror("sigaction");
        return 1;
    }

    printf("Waiting for SIGUSR1...\n");
    while (1) {
        pause(); // Chờ signal
    }

    return 0;
}
