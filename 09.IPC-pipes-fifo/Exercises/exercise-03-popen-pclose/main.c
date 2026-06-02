#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 256

/* TODO: Khai báo function prototypes */

int main() {
    char command[MAX_LINE];
    int choice;

    printf("=== popen/pclose Shell Pipe Demo ===\n");
    printf("1. Read output from shell command\n");
    printf("2. Write input to shell command\n");
    printf("Choice: ");
    /* TODO: Đọc lựa chọn từ người dùng */

    printf("Enter shell command: ");
    /* TODO: Đọc lệnh từ người dùng */
    /* fgets(command, MAX_LINE, stdin); */
    /* command[strcspn(command, "\n")] = 0; */

    /* TODO: Xử lý theo lựa chọn */
    /* if (choice == 1) { */
    /*     // Đọc output từ lệnh */
    /* } else if (choice == 2) { */
    /*     // Ghi input vào lệnh */
    /* } */

    return 0;
}

/* TODO: Hàm đọc output từ lệnh shell
 * int run_read_command(const char *cmd) {
 *     FILE *fp = popen(cmd, "r");
 *     // Đọc và in từng dòng
 *     // Đóng với pclose, kiểm tra exit status
 * }
 */

/* TODO: Hàm ghi input vào lệnh shell
 * int run_write_command(const char *cmd) {
 *     FILE *fp = popen(cmd, "w");
 *     // Ghi dữ liệu mẫu hoặc từ người dùng
 *     // Đóng với pclose, kiểm tra exit status
 * }
 */
