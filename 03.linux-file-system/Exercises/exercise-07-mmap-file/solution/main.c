#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <ctype.h>
#include <time.h>

#define FILE_PATH "test_data/sample.txt"

int main(void) {
    int fd;
    struct stat sb;
    char *mapped;
    size_t file_size;

    // Open the file with read-write access
    fd = open(FILE_PATH, O_RDWR);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // Get the file size using fstat()
    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        close(fd);
        return 1;
    }
    file_size = sb.st_size;

    if (file_size == 0) {
        printf("File is empty, nothing to do.\n");
        close(fd);
        return 0;
    }

    printf("File size: %zu bytes\n", file_size);

    // Map the file into memory
    mapped = mmap(NULL, file_size, PROT_READ | PROT_WRITE,
                  MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    // File descriptor no longer needed after mmap
    close(fd);

    // Print the original file contents
    printf("\n=== Original Content ===\n");
    printf("%.*s", (int)file_size, mapped);
    printf("=== End ===\n\n");

    // Convert lowercase letters to uppercase
    printf("Converting to uppercase...\n");
    for (size_t i = 0; i < file_size; i++) {
        if (mapped[i] >= 'a' && mapped[i] <= 'z') {
            mapped[i] -= 32;
        }
    }

    // Write changes back to file using msync()
    if (msync(mapped, file_size, MS_SYNC) == -1) {
        perror("msync");
        munmap(mapped, file_size);
        return 1;
    }

    printf("Changes synced to disk.\n");

    // Unmap the memory
    if (munmap(mapped, file_size) == -1) {
        perror("munmap");
        return 1;
    }

    // Verify by reading the file with traditional read()
    printf("\n=== Verification (read via read()) ===\n");
    fd = open(FILE_PATH, O_RDONLY);
    if (fd == -1) {
        perror("open for verification");
        return 1;
    }

    char buffer[4096];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);
    }
    close(fd);
    printf("=== End ===\n");

    printf("\nDone! File contents converted to uppercase.\n");
    return 0;
}
