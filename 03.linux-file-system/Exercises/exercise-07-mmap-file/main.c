#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define FILE_PATH "test_data/sample.txt"

int main(void) {
    int fd;
    struct stat sb;
    char *mapped;
    size_t file_size;

    // TODO 1: Open the file with read-write access
    // Use open() with O_RDWR
    // If it fails, perror() and return 1



    // TODO 2: Get the file size using fstat()
    // Use fstat(fd, &sb) to fill the stat structure
    // Store sb.st_size in file_size



    // TODO 3: Map the file into memory using mmap()
    // Use PROT_READ | PROT_WRITE for access
    // Use MAP_SHARED so changes are written back to file
    // Check for MAP_FAILED



    // TODO 4: Print the original file contents
    // Access mapped memory using array notation: mapped[i]
    // Use file_size to know how many bytes to print



    // TODO 5: Convert lowercase letters to uppercase
    // Loop through each byte of mapped memory
    // If a character is between 'a' and 'z', convert it



    // TODO 6: Write changes back to file using msync()
    // Use MS_SYNC flag for synchronous write



    // TODO 7: Unmap the memory and close the file
    // munmap() then close()



    // TODO 8: (Optional) Verify by reading file with read()
    // Re-open the file, read contents, and print to confirm



    printf("Done! File contents converted to uppercase.\n");
    return 0;
}
