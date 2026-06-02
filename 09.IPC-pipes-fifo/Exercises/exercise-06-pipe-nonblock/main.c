#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/select.h>
#include <limits.h>

#define BUF_SIZE 4096

/* TODO: Khai báo function prototypes */

int main() {
    int fd[2];

    // 1. Tạo pipe
    /* if (pipe(fd) == -1) { ... } */
    /* TODO: Tạo pipe và kiểm tra lỗi */

    printf("=== Non-blocking Pipe Demo ===\n");

    // 2. Non-blocking Read Demo
    /* TODO: Set O_NONBLOCK cho đầu đọc fd[0] */
    /* TODO: Thử đọc khi pipe rỗng, kiểm tra EAGAIN */

    // 3. Pipe Buffer Size (PIPE_BUF) Demo
    /* TODO: Ghi dữ liệu đến khi pipe đầy */
    /* TODO: Sử dụng fpathconf để kiểm tra _PC_PIPE_BUF */

    // 4. select() Demo với timeout
    /* TODO: Tạo tiến trình con ghi dữ liệu sau 1 giây */
    /* TODO: Parent dùng select() để chờ dữ liệu với timeout */
    /* int ret = select(fd[0] + 1, &rfds, NULL, NULL, &tv); */

    // 5. Cleanup
    close(fd[0]);
    close(fd[1]);

    return 0;
}

/* TODO: Hàm pipe buffer test
 * ssize_t test_pipe_capacity(int write_fd) {
 *     // Ghi non-blocking cho đến khi EAGAIN
 * }
 */
