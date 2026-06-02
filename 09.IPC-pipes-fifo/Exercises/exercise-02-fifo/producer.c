#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define FIFO_NAME "myfifo"
#define BUFFER_SIZE 1024

int main() {
    int fd;
    char buffer[BUFFER_SIZE];

    // 1. Tạo FIFO nếu chưa tồn tại
    // int mkfifo(const char *pathname, mode_t mode);
    /* TODO: Tạo FIFO tên "myfifo", permission 0666 */
    

    printf("Producer: Waiting for readers...\n");

    // 2. Mở FIFO để ghi
    /* TODO: Mở FIFO với flag O_WRONLY */
    // Lưu ý: open sẽ block cho đến khi có process mở để đọc
    

    printf("Producer: Got a reader, enter proper message:\n");

    // 3. Loop ghi dữ liệu
    while (1) {
        printf("> ");
        /* TODO: Nhập dữ liệu từ stdin */
        
        /* TODO: Ghi vào FIFO */
        
        /* Kiểm tra nếu nhập "exit" thì thoát */
    }

    /* TODO: Đóng file descriptor */
    return 0;
}
