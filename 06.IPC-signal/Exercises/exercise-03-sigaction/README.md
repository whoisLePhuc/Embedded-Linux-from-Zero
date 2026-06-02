# Exercise 3: Advanced Signal Handling with sigaction

## 🎯 Mục tiêu
- Hiểu sự khác biệt giữa `signal()` và `sigaction()`.
- Biết cách sử dụng `sigaction` để lấy thêm thông tin về signal.
- Cụ thể: Lấy được Process ID (PID) của tiến trình đã gửi signal đến (Sender PID).

## 📝 Yêu cầu

Viết một chương trình C thực hiện các bước sau:

1. In ra PID của chính nó.
2. Thiết lập signal handler cho **SIGUSR1** bằng `sigaction`.
3. Trong cấu hình `sa_flags` của `sigaction`, BẮT BUỘC phải set cờ `SA_SIGINFO`.
4. Viết hàm handler nhận 3 tham số (theo prototype của `sa_sigaction`): `void handler(int signum, siginfo_t *info, void *context)`.
5. Trong handler, in ra:
    - Số hiệu signal nhận được.
    - **PID của tiến trình gửi signal** (lấy từ `info->si_pid`).
6. Chương trình chính chạy vòng lặp vô hạn và sleep.

## 🧪 Hướng dẫn kiểm tra

1. Biên dịch và chạy chương trình `./main`.
2. Mở một terminal khác.
3. Sử dụng lệnh `kill` để gửi SIGUSR1 đến process đang chạy:
   ```bash
   kill -SIGUSR1 <PID_CUA_MAIN>
   ```
   (Ví dụ: `kill -SIGUSR1 1234`)
4. Quan sát output của chương trình `./main`. Nó phải in ra được PID của terminal (shell) mà bạn vừa dùng lệnh kill.

## 💡 Gợi ý

- Struct `struct sigaction`
- Flag `SA_SIGINFO`
- Struct `siginfo_t`, trường `si_pid`
