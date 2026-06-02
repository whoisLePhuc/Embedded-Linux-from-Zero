# Bài tập 04: Gửi Signal với kill và sigqueue ⭐⭐

## 🎯 Mục tiêu
- Hiểu cách gửi signal giữa các process bằng các system call khác nhau.
- Thực hành sử dụng `kill()`, `raise()`, và `sigqueue()`.
- Hiểu cách truyền dữ liệu kèm signal qua `sigqueue()` và `siginfo_t`.

## 📋 Yêu cầu
Viết 2 chương trình C: `sender.c` và `receiver.c`.

**receiver.c:**
1. In ra PID của chính nó.
2. Đăng ký handler cho `SIGUSR1` và `SIGUSR2` sử dụng `sigaction()` với flag `SA_SIGINFO`.
3. Trong handler (prototype `sa_sigaction`), in ra:
   - Số hiệu signal nhận được.
   - PID của process gửi signal (`info->si_pid`).
   - Nếu là `SIGUSR2`, in ra giá trị dữ liệu đi kèm (`info->si_value.sival_int`).
4. Chạy vòng lặp vô hạn chờ signal (dùng `pause()`).

**sender.c:**
1. Nhận receiver PID từ tham số dòng lệnh (`argv[1]`).
2. Gửi `SIGUSR1` đến receiver bằng `kill()`.
3. Gửi `SIGUSR2` kèm dữ liệu số nguyên (vd: 42) bằng `sigqueue()`.
4. Gửi `SIGTERM` cho chính nó bằng `raise()`.
5. In ra thông báo trước mỗi lần gửi.

## ✅ Tiêu chí đạt
- `receiver` hiển thị đúng sender PID và signal number khi nhận signal từ `sender`.
- `receiver` hiển thị được dữ liệu đi kèm (`sival_int`) khi nhận SIGUSR2 từ `sigqueue()`.
- `sender` gửi thành công cả 3 loại signal (SIGUSR1, SIGUSR2, SIGTERM).
- `raise(SIGTERM)` kết thúc `sender` — dòng `"This line should NOT appear"` không được in ra.
- Build không warning/error với `gcc -Wall -Wextra -std=c11`.

## 💡 Gợi ý
- `kill(pid_t pid, int sig)` gửi signal đến process khác.
- `sigqueue(pid_t pid, int sig, union sigval value)` gửi signal kèm dữ liệu.
- `raise(int sig)` gửi signal cho chính process đang chạy.
- Trong handler, struct `siginfo_t` có trường `si_pid` và `si_value.sival_int`.
- `sigemptyset(&sa.sa_mask)` để không block signal nào trong lúc handler chạy.

## 🔍 Kiểm tra
```bash
# Build
make

# Terminal 1: chạy receiver
./receiver

# Terminal 2: chạy sender với PID của receiver
./sender <PID_CUA_RECEIVER>

# Terminal 1 sẽ in ra:
# --- Received signal ---
# Signal number: 10 (SIGUSR1)
# Sender PID   : ...
# ...
# --- Received signal ---
# Signal number: 12 (SIGUSR2)
# Sender PID   : ...
# Data value   : 42
```

## 📁 File cần tạo
- [ ] `receiver.c` - Chương trình nhận signal
- [ ] `sender.c` - Chương trình gửi signal
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan
Đọc lại trong `IPC-signal.md`:
- Section 4.1: System call `kill()`
- Section 4.2: Gửi signal với `sigqueue()` và `raise()`
- Section 2.2: `sigaction()` với flag `SA_SIGINFO`
- `man 2 kill`, `man 2 sigqueue`, `man 2 raise`

---
**Bonus:** Sửa sender để gửi nhiều giá trị khác nhau qua sigqueue (struct, pointer) và receiver in ra tất cả.
