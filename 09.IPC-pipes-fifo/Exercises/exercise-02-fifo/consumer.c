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

    // 1. Tạo FIFO (Consumer cũng nên gọi để đảm bảo file tồn tại)
    /* TODO: Tạo FIFO "myfifo" */
    mkfifo(FIFO_NAME, 0666);

    printf("Consumer: Waiting for writers...\n");

    // 2. Mở FIFO để đọc
    /* TODO: Mở FIFO với flag O_RDONLY */
    

    printf("Consumer: Got a writer\n");

    // 3. Loop đọc dữ liệu
    while (1) {
        /* TODO: Đọc dữ liệu từ FIFO */
        
        /* TODO: In ra màn hình */
        /* printf("Received: %s", buffer); */
        
        /* exit loop if read returns 0 (EOF) or signal */
    }

    /* TODO: Đóng file descriptor */
    return 0;
}
