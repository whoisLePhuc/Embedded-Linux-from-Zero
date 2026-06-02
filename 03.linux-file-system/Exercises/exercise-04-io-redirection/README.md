# Bài tập 05: I/O Redirection và File Descriptor Manipulation ⭐⭐

## 🎯 Mục tiêu

Hiểu và thực hành manipulation file descriptor trong Linux sử dụng `dup()`, `dup2()` để redirect stdout/stderr.

## 📋 Yêu cầu

Viết chương trình `main.c` để:

1. **Save/Restore stdout pattern**: Lưu file descriptor gốc của stdout bằng `dup()`, sau đó redirect stdout ra file bằng `dup2()`, cuối cùng khôi phục lại stdout ban đầu

2. **Redirect stdout**: Chuyển hướng tất cả output của `printf()` vào một file log thay vì hiển thị ra terminal

3. **Redirect stderr**: Chuyển hướng `fprintf(stderr, ...)` vào cùng file log

4. **Error handling**: Xử lý tất cả các lỗi từ system calls (open, dup, dup2)

## ✅ Tiêu chí đạt

- Chương trình chạy được: `./main` (không cần argument)
- Output của `printf()` ghi vào file `output.log` khi đang redirect
- Output của `fprintf(stderr, ...)` cũng ghi vào file `output.log`
- Sau khi restore, `printf()` lại hiển thị ra terminal bình thường
- Error handling đầy đủ cho mọi system call
- File log được tạo với permissions 0644
- Không memory leak (đóng hết file descriptors)

## 💡 Gợi ý

- **Ba file descriptor mặc định:**
  ```c
  #define STDIN_FILENO  0  // stdin
  #define STDOUT_FILENO 1  // stdout
  #define STDERR_FILENO 2  // stderr
  ```

- **dup() - Sao chép file descriptor:**
  ```c
  int new_fd = dup(old_fd);
  // new_fd là số FD thấp nhất chưa dùng, trỏ tới cùng file table entry
  ```

- **dup2() - Sao chép đến FD chỉ định:**
  ```c
  // Redirect stdout đến file descriptor của log file
  dup2(log_fd, STDOUT_FILENO);
  // Sau lệnh này, write(STDOUT_FILENO, ...) tương đương write(log_fd, ...)
  ```

- **Save/Restore pattern:**
  ```c
  // Save
  int saved = dup(STDOUT_FILENO);
  
  // Redirect
  dup2(log_fd, STDOUT_FILENO);
  
  // ... printf output sẽ vào file ...
  
  // Restore
  dup2(saved, STDOUT_FILENO);
  close(saved);
  ```

- **Mở file log:**
  ```c
  int log_fd = open("output.log", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  ```

## 🔍 Kiểm tra

```bash
# Build
make

# Run
./main
# Terminal sẽ chỉ thấy thông báo restore, output của printf in ra file

# Kiểm tra file log
cat output.log
# Phải thấy nội dung của printf và fprintf(stderr, ...) thay vì ra terminal

# Kiểm tra permissions
ls -l output.log
# Phải là: -rw-r--r--
```

## 📁 File cần tạo

- [ ] `main.c` - Source code chương trình
- [ ] `Makefile` - Build script

## 📁 File đã cho

- `solution/` - Thư mục chứa solution tham khảo

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-file-system.md`:
- Section 3.3: Open File Table
- Section 3.4: File Descriptor Table
- Section 3.5: File Opening Process

## 📝 Bonus Challenges

1. **Redirect to different files**: Redirect stdout và stderr ra 2 files khác nhau
2. **Append mode**: Dùng `O_APPEND` thay vì `O_TRUNC` để giữ lại log từ nhiều lần chạy
3. **Custom file descriptor**: Tạo một FD mới (ví dụ FD số 5) và redirect qua đó
4. **Multiple redirects**: Redirect stdout, sau đó redirect lại lần nữa, rồi restore về ban đầu

---

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
