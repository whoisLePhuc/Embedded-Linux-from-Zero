# Exercise 2: Blocking and Unblocking Signals

## 🎯 Mục tiêu
- Hiểu khái niệm Signal Mask và cách sử dụng `sigprocmask`.
- Thực hành chặn (block) signal để bảo vệ đoạn code quan trọng (Critical Section).
- Quan sát hiện tượng Signal Pending (signal được gửi nhưng chưa được xử lý ngay do bị block).

## 📝 Yêu cầu

Viết một chương trình C thực hiện các bước sau:

1. In ra PID hiện tại.
2. Khởi tạo một tập signal (`sigset_t`) và thêm `SIGINT` vào tập này.
3. **Block SIGINT**: Sử dụng `sigprocmask` với `SIG_BLOCK` để chặn SIGINT.
4. In ra thông báo: `"SIGINT is blocked for 5 seconds. Try Ctrl+C now..."`.
5. Thực hiện "công việc giả lập": `sleep(5)`. Trong thời gian này, nếu user nhấn Ctrl+C, chương trình **không được dừng lại**.
6. **Kiểm tra Pending**: Sau khi sleep xong, sử dụng `sigpending()` để kiểm tra xem Signal SIGINT có đang chờ (pending) hay không. Nếu có, in ra thông báo: `"Signal SIGINT is pending!"`.
7. **Unblock SIGINT**: Sử dụng `sigprocmask` với `SIG_UNBLOCK` (hoặc `SIG_SETMASK` với mask cũ) để bỏ chặn SIGINT.
8. Sau khi unblock, nếu lúc nãy có nhấn Ctrl+C, process sẽ nhận được signal và terminate ngay lập tức (do ta không cài đặt handler riêng, nên nó dùng default action là terminate).
9. In ra thông báo: `"Program finished normally."` (Dòng này sẽ không được in nếu chương trình bị terminate do SIGINT).

## 🧪 Hướng dẫn kiểm tra

**Kịch bản 1: Không nhấn Ctrl+C**
1. Chạy `./main`
2. Đợi 5s.
3. Chương trình in `"Program finished normally."` và kết thúc.

**Kịch bản 2: Nhấn Ctrl+C trong khi đang Block**
1. Chạy `./main`.
2. Khi thấy thông báo blocked, nhấn `Ctrl+C`.
3. Chương trình **vẫn chạy tiếp**, không bị dừng ngay.
4. Sau 5s, in ra `"Signal SIGINT is pending!"`.
5. Ngay khi Unblock, chương trình bị Terminate (dòng `"Program finished normally."` **không** xuất hiện).

## 💡 Gợi ý

- `sigemptyset()`, `sigaddset()`
- `sigprocmask(SIG_BLOCK, ...)`
- `sigpending()`
- `sigismember()`
