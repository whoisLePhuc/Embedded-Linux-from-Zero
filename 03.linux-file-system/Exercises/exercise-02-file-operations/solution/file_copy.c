#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

int main() {
    int src_fd, dst_fd;
    ssize_t bytes_read, bytes_written;
    char buffer[BUFFER_SIZE];
    long total_bytes = 0;

    // Thay đổi các đường dẫn này để test
    const char src[] = "test_data/input.txt";
    const char dst[] = "output.txt";
    
    // Open source file for reading
    src_fd = open(src, O_RDONLY);
    if (src_fd == -1) {
        perror("open source");
        return 1;
    }
    
    // Create/open destination file for writing
    dst_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst_fd == -1) {
        perror("open destination");
        close(src_fd);
        return 1;
    }
    
    // Copy data from source to destination
    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(dst_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("write");
            close(src_fd);
            close(dst_fd);
            return 1;
        }
        total_bytes += bytes_written;
    }
    
    if (bytes_read == -1) {
        perror("read");
        close(src_fd);
        close(dst_fd);
        return 1;
    }
    
    // Close both files
    close(src_fd);
    close(dst_fd);
    
    printf("Successfully copied %ld bytes from %s to %s\n", 
           total_bytes, src, dst);
    
    return 0;
}
