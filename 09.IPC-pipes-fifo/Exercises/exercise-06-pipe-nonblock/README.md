# Bài tập 06: Pipe Non-blocking I/O ⭐⭐

## 🎯 Mục tiêu

Hiểu cách sử dụng `O_NONBLOCK` trên pipes, kiểm tra kích thước pipe buffer (`PIPE_BUF`), và dùng `select()` để monitor pipe I/O.

## 📋 Yêu cầu

Viết chương trình `main.c` thực hiện các chức năng sau:

### 1. Pipe với Non-blocking Read
- Tạo pipe bằng `pipe()`
- Set cờ `O_NONBLOCK` cho đầu đọc bằng `fcntl()`
- Thử đọc khi pipe rỗng, kiểm tra lỗi `EAGAIN`

### 2. Pipe Buffer Size (PIPE_BUF)
- Ghi dữ liệu vào pipe cho đến khi đầy (block hoặc kiểm tra)
- Sử dụng non-blocking write và đếm số byte ghi được
- Xác định `PIPE_BUF` trên hệ thống

### 3. select() trên Pipe
- Dùng `select()` để chờ dữ liệu từ pipe với timeout
- Ghi dữ liệu từ một tiến trình con, đọc từ parent

## ✅ Tiêu chí đạt

- Dùng `fcntl(fd, F_SETFL, flags | O_NONBLOCK)` để set non-blocking
- Phát hiện `EAGAIN` khi đọc/ghi trên pipe non-blocking
- Dùng `select()` với timeout để monitor pipe
- Chương trình biên dịch với `gcc -Wall -Wextra`

## 💡 Gợi ý

- Set non-blocking flag:
  ```c
  int flags = fcntl(fd[0], F_GETFL);
  fcntl(fd[0], F_SETFL, flags | O_NONBLOCK);
  ```

- Kiểm tra EAGAIN khi đọc non-blocking:
  ```c
  ssize_t n = read(fd[0], buf, sizeof(buf));
  if (n == -1 && errno == EAGAIN) {
      printf("Pipe empty (EAGAIN) - no data available\n");
  }
  ```

- Xác định PIPE_BUF:
  ```c
  #include <limits.h>
  printf("PIPE_BUF = %d\n", _POSIX_PIPE_BUF); // minimum guaranteed
  // Or: long val = fpathconf(fd[1], _PC_PIPE_BUF);
  ```

- select() trên pipe:
  ```c
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(fd[0], &rfds);

  struct timeval tv = {2, 0}; // 2 second timeout
  int ret = select(fd[0] + 1, &rfds, NULL, NULL, &tv);
  if (ret == -1) {
      perror("select");
  } else if (ret == 0) {
      printf("Timeout - no data in 2 seconds\n");
  } else {
      // Data available
      read(fd[0], buf, sizeof(buf));
  }
  ```

## 🔍 Kiểm tra

```bash
# Build
make

# Chạy chương trình
./pipe_nonblock
```

## 📁 File cần tạo

- [ ] `main.c` - Source code chính
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `08.IPC-pipes-fifo.md`:
- Section 1.6: Non-blocking I/O with pipes
- Section 1.7: PIPE_BUF and Atomicity
- Section 2.3: select() and poll() with pipes

---

**Bonus**: So sánh `select()` với `poll()`: viết thêm phiên bản dùng `poll()` và so sánh API.

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
