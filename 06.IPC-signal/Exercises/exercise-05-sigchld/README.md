# Bài tập 05: SIGCHLD Handler ⭐⭐

## 🎯 Mục tiêu
- Hiểu cách SIGCHLD hoạt động khi child process kết thúc.
- Thực hành reaping child processes trong signal handler để tránh zombie.
- Sử dụng `waitpid()` với tùy chọn `WNOHANG` trong vòng lặp.

## 📋 Yêu cầu
Viết chương trình `main.c` thực hiện:

1. Đăng ký signal handler cho `SIGCHLD` sử dụng `sigaction()` với flag `SA_NOCLDSTOP | SA_RESTART`.
2. Fork **3 child processes**, mỗi child:
   - Chạy một khoảng thời gian khác nhau (dùng `sleep((i+1)*2)`).
   - Thoát với exit code khác nhau (dùng `exit((i+1) * 10)`).
   - In ra PID và thông báo khi bắt đầu và kết thúc.
3. Trong SIGCHLD handler:
   - Dùng `waitpid(-1, &status, WNOHANG)` trong vòng lặp `while` để reap **tất cả** children đã kết thúc (có thể nhiều child cùng lúc).
   - In ra PID của child đã được reap và exit status (`WEXITSTATUS`).
   - Dùng biến `volatile sig_atomic_t` để đếm số child đã reap.
4. Parent chạy vòng lặp và in ra số lượng child đã reaped, kết thúc khi tất cả 3 children đã được reap.

## ✅ Tiêu chí đạt
- Fork đúng 3 children, mỗi child có thời gian chạy khác nhau.
- SIGCHLD handler reap được tất cả children, không bỏ sót.
- **Không có zombie processes** — kiểm tra bằng `ps` trong lúc chương trình chạy.
- Parent chỉ kết thúc sau khi tất cả children đã được reap.
- Build không warning/error với `gcc -Wall -Wextra -std=c11`.

## 💡 Gợi ý
- `SA_NOCLDSTOP`: chỉ nhận SIGCHLD khi child terminates, không phải khi stopped.
- `SA_RESTART`: tự động restart các system call bị ngắt bởi signal.
- Dùng `while ((pid = waitpid(-1, &status, WNOHANG)) > 0)` để reap nhiều child cùng lúc — quan trọng vì nhiều child có thể kết thúc cùng lúc nhưng handler chỉ chạy một lần.
- `WEXITSTATUS(status)` lấy exit code của child.
- `volatile sig_atomic_t` là kiểu dữ liệu an toàn để đọc/ghi trong signal handler.

## 🔍 Kiểm tra
```bash
# Build
make

# Chay chuong trinh
./main

# Output mong đợi (PID có thể khác):
# Parent PID: 1234
# [Child 1] PID 1235 starting, will exit in 2 seconds
# [Parent] Forked child 1 with PID 1235
# ...
# [Parent] Working... (reaped 0/3 children)
# [Child 1] PID 1235 exiting now
# [Parent] Reaped child PID 1235 (exit status: 10)
# [Parent] Working... (reaped 1/3 children)
# ...

# Kiem tra zombie o terminal khac:
ps -C main -o pid,stat,cmd
# STAT column KHONG duoc co 'Z'
```

## 📁 File cần tạo
- [ ] `main.c` - Source code chương trình
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan
Đọc lại trong `IPC-signal.md`:
- Section 3.1: SIGCHLD (Signal 17, Default: Ignore)
- Section 2.2: `sigaction()` và cờ `SA_NOCLDSTOP`, `SA_RESTART`
- `man 2 waitpid`, `man 2 sigaction`

---
**Bonus:** Thêm một child bị kill bởi signal (dùng `kill(getpid(), SIGKILL)`) và dùng `WIFSIGNALED()` / `WTERMSIG()` trong handler để xử lý.
