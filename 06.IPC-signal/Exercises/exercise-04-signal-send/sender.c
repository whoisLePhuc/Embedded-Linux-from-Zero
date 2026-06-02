#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    /*
     * TODO 1: Kiem tra tham so dong lenh
     * - argc phai bang 2 (ten chuong trinh + PID dich)
     * - Parse argv[1] thanh kieu pid_t bang atoi()
     * - In ra PID cua sender va target PID
     */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <receiver_pid>\n", argv[0]);
        return 1;
    }

    pid_t receiver_pid = (pid_t)atoi(argv[1]);
    printf("Sender PID: %d\n", getpid());
    printf("Target PID: %d\n", receiver_pid);

    /*
     * TODO 2: Gui SIGUSR1 den receiver bang kill()
     * In ra thong bao truoc khi gui
     * Xu ly loi: neu kill() tra ve -1, in perror va return 1
     * sleep(1) giua cac lan gui de receiver co thoi gian xu ly
     */
    printf("\n1. Sending SIGUSR1 via kill()...\n");
    // TODO: Dung kill() de gui SIGUSR1
    sleep(1);

    /*
     * TODO 3: Gui SIGUSR2 kem du lieu bang sigqueue()
     * - Khai bao union sigval, gan sival_int = 42
     * - Goi sigqueue(receiver_pid, SIGUSR2, value)
     * - Xu ly loi
     */
    printf("\n2. Sending SIGUSR2 with data via sigqueue()...\n");
    // TODO: Dung sigqueue() de gui SIGUSR2 kem du lieu
    sleep(1);

    /*
     * TODO 4: Gui SIGTERM cho chinh process nay bang raise()
     * DONG PRINTF CUOI CUNG KHONG DUOC THUC THI neu raise() thanh cong
     */
    printf("\n3. Sending SIGTERM to self via raise()...\n");
    sleep(1);
    // TODO: Dung raise() de gui SIGTERM

    printf("This line should NOT appear.\n");
    return 0;
}
