# Exercise 1: SIGINT Handler

## 🎯 Mục tiêu
- Hiểu cách hoạt động của Signal Handler.
- Biết cách bắt (catch) một signal, cụ thể là `SIGINT` (gửi bởi Ctrl+C).
- Biết cách sử dụng `kill` command hoặc Ctrl+\ để terminate process khi SIGINT đã bị bắt.

## 📝 Yêu cầu

Viết một chương trình C thực hiện các việc sau:

1. In ra Process ID (PID) của nó khi khởi động.
2. Đăng ký một hàm xử lý (signal handler) cho signal `SIGINT` (Signal số 2).
3. Trong vòng lặp vô hạn (while(1)):
    - In ra dòng chữ "Program is running..." mỗi 2 giây.
    - Sử dụng `sleep(2)`.

**Yêu cầu trong Signal Handler:**
- Khi người dùng nhấn `Ctrl+C` (gửi SIGINT), chương trình **KHÔNG ĐƯỢC** kết thúc ngay lập tức.
- Thay vào đó, in ra thông báo: `"\nCaught SIGINT! I will not exit via Ctrl+C. Try Ctrl+\ or 'kill' command.\n"`.

## 🧪 Hướng dẫn kiểm tra

1. Biên dịch chương trình: `make` (hoặc `gcc main.c -o main`)
2. Chạy chương trình: `./main`
3. Nhấn `Ctrl+C`: Chương trình sẽ in ra thông báo handle và tiếp tục chạy.
4. Nhấn `Ctrl+\` (SIGQUIT): Chương trình sẽ kết thúc (Core Dump).

## 💡 Gợi ý

- Sử dụng hàm `signal()`.
- Prototype của handler: `void handler_name(int signum)`.
