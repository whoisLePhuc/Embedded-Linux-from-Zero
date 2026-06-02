# Bài tập 03: File Descriptor Monitoring ⭐⭐

## 🎯 Mục tiêu

Tạo chương trình để monitor file descriptors của chính process đó, giúp hiểu rõ cách kernel quản lý FD table.

## 📋 Yêu cầu

Viết chương trình `fd_monitor.c` để:

1. Mở nhiều files khác nhau (ít nhất 3 files)
2. Hiển thị danh sách tất cả file descriptors đang mở bằng cách đọc `/proc/self/fd/`
3. Cho mỗi FD, hiển thị:
   - FD number
   - File path mà FD trỏ tới
   - Loại file (regular file, directory, pipe, socket...)

4. Demonstrate các standard FDs:
   - FD 0: stdin
   - FD 1: stdout
   - FD 2: stderr

## ✅ Tiêu chí đạt

- Chương trình hiển thị đầy đủ FDs (0, 1, 2, và các FDs tự mở)
- Mỗi FD hiển thị rõ số và đường dẫn
- Sử dụng `readlink()` để resolve symbolic links trong `/proc/self/fd/`
- Đóng tất cả FDs đã mở (không leak)
- Code có comment giải thích rõ ràng

## 💡 Gợi ý

- **Đọc directory `/proc/self/fd/`:**
  ```c
  #include <dirent.h>
  DIR *dir = opendir("/proc/self/fd");
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
      // Process each FD...
  }
  closedir(dir);
  ```

- **Resolve symbolic link:**
  ```c
  char path[256];
  snprintf(path, sizeof(path), "/proc/self/fd/%s", entry->d_name);
  
  char link[1024];
  ssize_t len = readlink(path, link, sizeof(link) - 1);
  if (len != -1) {
      link[len] = '\0';
      printf("FD %s -> %s\n", entry->d_name, link);
  }
  ```

- **Mở files để test:**
  ```c
  int fd1 = open("file1.txt", O_RDONLY);
  int fd2 = open("file2.txt", O_WRONLY | O_CREAT, 0644);
  int fd3 = open("/etc/passwd", O_RDONLY);
  ```

## 🔍 Kiểm tra

```bash
# Build
make

# Run program
./fd_monitor

# So sánh với lsof
lsof -p $$

# So sánh với ls
ls -l /proc/self/fd/
```

**Expected output:**
```
=== File Descriptor Table ===
FD 0  -> /dev/pts/0 (stdin)
FD 1  -> /dev/pts/0 (stdout)
FD 2  -> /dev/pts/0 (stderr)
FD 3  -> /path/to/file1.txt
FD 4  -> /path/to/file2.txt
FD 5  -> /etc/passwd
FD 6  -> /proc/12345/fd (opendir's fd)
===========================
Total FDs open: 7
```

## 📁 File cần tạo

- [ ] `fd_monitor.c` - Source code chương trình
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-file-system.md`:
- Section 3.4: File Descriptor Table
- Section 3.5: File Opening Process

## 📝 Bonus Challenges

1. **FD limit**: Hiển thị soft/hard limit của FDs (`getrlimit()`)
2. **FD type detection**: Phân biệt regular file, directory, socket, pipe
3. **Watch mode**: Update FD list mỗi giây (use `sleep()` và clear screen)

---

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
