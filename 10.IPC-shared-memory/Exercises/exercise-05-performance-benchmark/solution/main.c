#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>

#define SIZE_1MB   (1 * 1024 * 1024)
#define SIZE_10MB  (10 * 1024 * 1024)
#define SIZE_100MB (100 * 1024 * 1024)

double benchmark_mmap_write(size_t size) {
    int fd = open("mmap_test.bin", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open (mmap)");
        return -1;
    }

    if (ftruncate(fd, (off_t)size) == -1) {
        perror("ftruncate");
        close(fd);
        return -1;
    }

    char *map = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return -1;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Write via memory access
    memset(map, 'A', size);
    msync(map, size, MS_SYNC);

    clock_gettime(CLOCK_MONOTONIC, &end);

    munmap(map, size);
    close(fd);

    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

double benchmark_write(size_t size) {
    int fd = open("write_test.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open (write)");
        return -1;
    }

    char *buf = malloc(size);
    if (!buf) {
        perror("malloc");
        close(fd);
        return -1;
    }
    memset(buf, 'A', size);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Write via system calls
    write(fd, buf, size);
    fsync(fd);

    clock_gettime(CLOCK_MONOTONIC, &end);

    free(buf);
    close(fd);

    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

int main() {
    long sizes[] = {SIZE_1MB, SIZE_10MB, SIZE_100MB};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    const char *labels[] = {"1 MB", "10 MB", "100 MB"};

    printf("=== mmap vs read/write Performance Benchmark ===\n\n");
    printf("%-15s %-20s %-20s %-20s\n",
           "File Size", "mmap + msync (s)", "write + fsync (s)", "Speedup");
    printf("%-15s %-20s %-20s %-20s\n",
           "----------", "-----------------", "------------------", "-------");

    for (int i = 0; i < num_sizes; i++) {
        double t_mmap = benchmark_mmap_write((size_t)sizes[i]);
        double t_write = benchmark_write((size_t)sizes[i]);

        if (t_mmap < 0 || t_write < 0) {
            printf("Error during benchmark\n");
            continue;
        }

        double speedup = t_write / t_mmap;
        printf("%-15s %-20.4f %-20.4f %-20.2fx\n",
               labels[i], t_mmap, t_write, speedup);
    }

    // Cleanup temp files
    remove("mmap_test.bin");
    remove("write_test.bin");

    return 0;
}
