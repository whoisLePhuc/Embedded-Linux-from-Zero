#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

void print_file_type(mode_t mode) {
    printf("File type:        ");
    if (S_ISREG(mode))  printf("Regular file\n");
    else if (S_ISDIR(mode))  printf("Directory\n");
    else if (S_ISLNK(mode))  printf("Symbolic link\n");
    else if (S_ISCHR(mode))  printf("Character device\n");
    else if (S_ISBLK(mode))  printf("Block device\n");
    else if (S_ISFIFO(mode)) printf("FIFO/pipe\n");
    else printf("Unknown\n");
}

void print_permissions(mode_t mode) {
    printf("Permissions:      0%o\n", mode & 0777);
    printf("Access rights:    ");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
    printf("\n");
}

int main() {
    struct stat sb;
    
    // Thay đổi đường dẫn này để test các file khác nhau
    const char *filename = "test_data/sample.txt";
    
    // Get file information
    if (stat(filename, &sb) == -1) {
        perror("stat");
        return 1;
    }
    
    printf("File: %s\n", filename);
    printf("=====================================\n");
    
    // Inode number
    printf("Inode number:     %lu\n", sb.st_ino);
    
    // File type
    print_file_type(sb.st_mode);
    
    // File size
    printf("Size:             %ld bytes\n", sb.st_size);
    
    // Number of blocks
    printf("Blocks:           %ld (512B blocks)\n", sb.st_blocks);
    printf("Block size:       %ld bytes\n", sb.st_blksize);
    
    // Number of hard links
    printf("Hard links:       %lu\n", sb.st_nlink);
    
    // Permissions
    print_permissions(sb.st_mode);
    
    // Owner information
    printf("Owner UID:        %u\n", sb.st_uid);
    printf("Owner GID:        %u\n", sb.st_gid);
    
    // Timestamps
    printf("Last access:      %s", ctime(&sb.st_atime));
    printf("Last modified:    %s", ctime(&sb.st_mtime));
    printf("Last status chg:  %s", ctime(&sb.st_ctime));
    
    return 0;
}
