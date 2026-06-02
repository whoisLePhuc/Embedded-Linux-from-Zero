#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>

int is_numeric(const char *str) {
    if (!str || *str == '\0') {
        return 0;
    }
    
    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

void print_fd_info(const char *fd_name) {
    char path[256];
    char link[1024];
    ssize_t len;
    
    // Build path to /proc/self/fd/<fd_number>
    int ret = snprintf(path, sizeof(path), "/proc/self/fd/%s", fd_name);
    if (ret < 0 || (size_t)ret >= sizeof(path)) {
        fprintf(stderr, "Warning: Path too long for FD %s\n", fd_name);
        return;
    }
    
    // Read the symbolic link to get the actual file path
    len = readlink(path, link, sizeof(link) - 1);
    
    // FIX: Kiểm tra len hợp lệ trước khi sử dụng
    if (len > 0 && len < (ssize_t)sizeof(link)) {
        link[len] = '\0';
        
        // Add descriptions for standard FDs
        if (strcmp(fd_name, "0") == 0) {
            printf("FD %-3s -> %s (stdin)\n", fd_name, link);
        } else if (strcmp(fd_name, "1") == 0) {
            printf("FD %-3s -> %s (stdout)\n", fd_name, link);
        } else if (strcmp(fd_name, "2") == 0) {
            printf("FD %-3s -> %s (stderr)\n", fd_name, link);
        } else {
            printf("FD %-3s -> %s\n", fd_name, link);
        }
    } else if (len == -1) {
        // FIX: Xử lý lỗi nhưng không hiển thị (một số FD có thể không đọc được)
        // Chỉ log nếu cần debug
        // fprintf(stderr, "Could not read link for FD %s: %s\n", fd_name, strerror(errno));
    }
}

int main() {
    DIR *dir;
    struct dirent *entry;
    int fd_count = 0;
    int dir_fd = -1; 
    
    printf("=== File Descriptor Monitoring ===\n");
    printf("PID: %d\n\n", getpid());
    
    // Open some test files to demonstrate FD allocation
    printf("Opening test files...\n");
    
    int fd1 = open("/etc/hostname", O_RDONLY);
    if (fd1 != -1) {
        printf("Opened /etc/hostname (FD %d)\n", fd1);
    } else {
        printf("Failed to open /etc/hostname: %s\n", strerror(errno));
    }
    
    int fd2 = open("/etc/os-release", O_RDONLY);
    if (fd2 != -1) {
        printf("Opened /etc/os-release (FD %d)\n", fd2);
    } else {
        printf("Failed to open /etc/os-release: %s\n", strerror(errno));
    }
    
    printf("\n=== File Descriptor Table ===\n");
    
    // Open /proc/self/fd directory
    dir = opendir("/proc/self/fd");
    if (dir == NULL) {
        perror("opendir");
        
        if (fd1 != -1) close(fd1);
        if (fd2 != -1) close(fd2);
        return 1;
    }
    
    dir_fd = dirfd(dir);
    if (dir_fd == -1) {
        perror("dirfd");
        closedir(dir);
        if (fd1 != -1) close(fd1);
        if (fd2 != -1) close(fd2);
        return 1;
    }
    
    // Iterate through all FDs
    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || 
            strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        // Only process numeric entries (FD numbers)
        if (is_numeric(entry->d_name)) {
            int current_fd = atoi(entry->d_name);
            
            // FIX: Loại trừ FD của chính opendir() để đếm chính xác
            if (current_fd != dir_fd) {
                print_fd_info(entry->d_name);
                fd_count++;
            }
        }
    }
    
    closedir(dir);
    
    printf("=============================\n");
    printf("Total FDs open: %d\n", fd_count);
    
    // Close the files we opened
    if (fd1 != -1) {
        close(fd1);
        printf("\nClosed FD %d (/etc/hostname)\n", fd1);
    }
    if (fd2 != -1) {
        close(fd2);
        printf("Closed FD %d (/etc/os-release)\n", fd2);
    }
    
    printf("\nNote: You can also check with: ls -l /proc/%d/fd/\n", getpid());
    
    return 0;
}