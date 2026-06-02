#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

/*
 * TODO 1: Khai bao bien dem so child da reaped
 * Dung volatile sig_atomic_t de dam bao an toan trong signal handler
 */
// volatile sig_atomic_t child_exit_count = 0;

/*
 * TODO 2: Viet SIGCHLD handler
 * - Dung waitpid(-1, &status, WNOHANG) trong vong lap WHILE
 *   de reap NHIEU child cung luc (tranh bo sot)
 * - In ra PID cua child da reaped va exit status (WEXITSTATUS)
 * - Tang bien dem cho moi child reaped
 */
void sigchld_handler(int signum) {
    // TODO: Viet code reap child trong handler

    (void)signum;
}

int main() {
    struct sigaction sa;

    printf("Parent PID: %d\n", getpid());

    /*
     * TODO 3: Thiet lap sigaction cho SIGCHLD
     * - sa.sa_handler = sigchld_handler
     * - sa.sa_flags = SA_NOCLDSTOP | SA_RESTART
     * - sigemptyset(&sa.sa_mask)
     * - Goi sigaction(SIGCHLD, &sa, NULL)
     */
    

    /*
     * TODO 4: Fork 3 child processes
     * - Moi child ngu (i+1)*2 giay roi thoat voi exit code (i+1)*10
     * - Child in thong bao khi bat dau va ket thuc
     * - Parent in thong bao moi khi fork thanh cong
     * - Xu ly loi fork
     */
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            // TODO: Code cho child process
            // In thong bao bat dau, sleep, in thong bao ket thuc, exit

        } else if (pid > 0) {
            printf("[Parent] Forked child %d\n", i + 1);
        } else {
            perror("fork");
            return 1;
        }
    }

    /*
     * TODO 5: Parent cho tat ca children hoan thanh
     * - Vong lap kiem tra child_exit_count < 3
     * - In trang thai va sleep(1) moi lan lap
     * - Khi xong, in thong bao hoan tat
     */
    printf("[Parent] Waiting for all children to finish...\n");
    // TODO: Vong lap cho den khi reaped het 3 children

    return 0;
}
