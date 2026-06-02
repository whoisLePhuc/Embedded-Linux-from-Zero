#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/*
 * TODO 1: Viet signal handler voi prototype sa_sigaction
 * - signum: so hieu signal nhan duoc
 * - info: thong tin ve signal va nguoi gui
 * - context: ng canh CPU (khong dung)
 *
 * In ra:
 *   - So hieu signal (signum)
 *   - PID cua process gui (info->si_pid)
 *   - Neu la SIGUSR2, in ra info->si_value.sival_int
 */
void signal_handler(int signum, siginfo_t *info, void *context) {
    // TODO: Viet code xu ly trong thanh handler
    // Nho fflush(stdout) de dam bao output duoc in ra ngay

    (void)context;
}

int main() {
    struct sigaction sa;

    /*
     * TODO 2: In ra PID cua receiver de sender biet
     */
    printf("Receiver PID: %d\n", getpid());

    /*
     * TODO 3: Cau hinh struct sigaction
     * - sa_sigaction: tro den signal_handler
     * - sa_flags: SA_SIGINFO
     * - Dung sigemptyset() de khoi tao sa.sa_mask rong
     */
    

    /*
     * TODO 4: Dang ky handler cho SIGUSR1 va SIGUSR2
     * Dung sigaction(SIGUSR1, &sa, NULL) va sigaction(SIGUSR2, &sa, NULL)
     */
    if (sigaction(SIGUSR1, &sa, NULL) < 0) {
        perror("sigaction SIGUSR1");
        return 1;
    }

    if (sigaction(SIGUSR2, &sa, NULL) < 0) {
        perror("sigaction SIGUSR2");
        return 1;
    }

    printf("Waiting for signals...\n");

    /*
     * TODO 5: Cho signal bang pause() trong vong lap vo han
     */
    while (1) {
        pause();
    }

    return 0;
}
