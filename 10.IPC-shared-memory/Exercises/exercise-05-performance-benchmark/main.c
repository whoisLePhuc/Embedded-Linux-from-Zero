#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>

/* TODO: Định nghĩa các kích thước file cần test */
/* #define SIZE_1MB  (1024 * 1024) */
/* #define SIZE_10MB (10 * 1024 * 1024) */
/* #define SIZE_100MB (100 * 1024 * 1024) */

/* TODO: Khai báo function prototypes */

int main() {
    printf("=== mmap vs read/write Performance Benchmark ===\n\n");

    /* TODO: Benchmark với nhiều kích thước file */
    /* long sizes[] = {SIZE_1MB, SIZE_10MB, SIZE_100MB}; */
    /* int num_sizes = sizeof(sizes) / sizeof(sizes[0]); */

    /* TODO: In header bảng kết quả */
    /* printf("%-15s %-20s %-20s\n", "File Size", "mmap + msync", "write + fsync"); */

    /* TODO: Với mỗi kích thước, chạy benchmark và in kết quả */

    /* TODO: Cleanup file tạm */
    /* remove("mmap_test.bin"); */
    /* remove("write_test.bin"); */

    return 0;
}

/* TODO: Hàm benchmark mmap write
 * double benchmark_mmap_write(size_t size) {
 *     // Mở file, mmap, memset, msync, đo thời gian
 * }
 */

/* TODO: Hàm benchmark write truyền thống
 * double benchmark_write(size_t size) {
 *     // Mở file, malloc, write, fsync, đo thời gian
 * }
 */
