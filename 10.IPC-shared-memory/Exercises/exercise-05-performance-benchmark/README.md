# Bài tập 05: Performance Benchmark: mmap vs read() ⭐⭐

## 🎯 Mục tiêu

So sánh hiệu năng đọc/ghi file giữa `mmap()` (memory-mapped I/O) và `read()`/`write()` (traditional I/O), sử dụng `msync()` và đo thời gian thực thi.

## 📋 Yêu cầu

Viết chương trình `main.c` thực hiện:

### 1. Ghi file với mmap
- Tạo file với `open()` + `ftruncate()`
- Map file vào memory với `mmap()`
- Ghi dữ liệu trực tiếp vào vùng nhớ đã map
- Gọi `msync()` để đồng bộ xuống disk

### 2. Ghi file với write()
- Tạo file, ghi dữ liệu với `write()` (từ userspace buffer)
- Không dùng mmap

### 3. Đo thời gian và so sánh
- Dùng `clock_gettime()` để đo thời gian
- Benchmark với các kích thước file: 1MB, 10MB, 100MB
- In kết quả dạng bảng so sánh

## ✅ Tiêu chí đạt

- Dùng `mmap()` với `MAP_SHARED` khi cần đồng bộ
- Dùng `msync()` với `MS_SYNC` hoặc `MS_ASYNC`
- Đo thời gian chính xác với `clock_gettime()` và `CLOCK_MONOTONIC`
- So sánh ít nhất 2 kích thước file khác nhau
- Chương trình biên dịch với `gcc -Wall -Wextra`

## 💡 Gợi ý

- Đo thời gian:
  ```c
  #include <time.h>

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  // ... do work ...
  clock_gettime(CLOCK_MONOTONIC, &end);
  double elapsed = (end.tv_sec - start.tv_sec) +
                   (end.tv_nsec - start.tv_nsec) / 1e9;
  ```

- mmap write:
  ```c
  int fd = open("mmap_test.bin", O_RDWR | O_CREAT | O_TRUNC, 0644);
  ftruncate(fd, FILE_SIZE);
  char *map = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE,
                   MAP_SHARED, fd, 0);
  memset(map, 'A', FILE_SIZE); // Write directly to mapped memory
  msync(map, FILE_SIZE, MS_SYNC); // Sync to disk
  ```

- Traditional write:
  ```c
  int fd = open("write_test.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  char *buf = malloc(FILE_SIZE);
  memset(buf, 'A', FILE_SIZE);
  write(fd, buf, FILE_SIZE); // Write via system call
  fsync(fd); // Sync to disk
  ```

## 🔍 Kiểm tra

```bash
# Build
make

# Chạy benchmark
./mmap_benchmark
```

## 📁 File cần tạo

- [ ] `main.c` - Source code benchmark
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `09.IPC-shared-memory.md`:
- Section 2.3: mmap() vs read() Performance
- Section 2.4: msync and Data Integrity

---

**Bonus**: Thêm benchmark cho random access (seek + read) và so sánh với mmap random access.

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
