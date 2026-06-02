#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main() {
    sigset_t new_set, old_set, pending_set;

    printf("My PID: %d\n", getpid());

    /*
     * TODO 1: Khởi tạo tập signal `new_set` và thêm SIGINT vào
     * Gợi ý: sigemptyset(), sigaddset()
     */


    /*
     * TODO 2: Block SIGINT bằng sigprocmask
     * Sử dụng SIG_BLOCK
     * Lưu mask cũ vào `old_set` để có thể restore sau này
     */
    printf("Blocking SIGINT for 5 seconds. Try pressing Ctrl+C now...\n");
    if (sigprocmask(SIG_BLOCK, &new_set, &old_set) < 0) {
        perror("sigprocmask block");
        return 1;
    }

    /*
     * TODO 3: Sleep 5 giây để mô phỏng Critical Section
     */
    sleep(5);

    /*
     * TODO 4: Kiểm tra xem có signal nào đang pending không
     * Sử dụng sigpending() và sigismember()
     */
    if (sigpending(&pending_set) == 0) {
        if (sigismember(&pending_set, SIGINT)) {
            printf("\nSignal SIGINT is pending!\n");
        }
    }

    /*
     * TODO 5: Unblock signal (Restore old mask)
     * Sử dụng sigprocmask với SIG_SETMASK và old_set
     */
    printf("Unblocking SIGINT...\n");


    printf("Program finished normally.\n");
    return 0;
}
