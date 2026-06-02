# Bài tập 04: Async Notification với mq_notify ⭐⭐

## 🎯 Mục tiêu

Hiểu cách sử dụng `mq_notify()` để nhận thông báo bất đồng bộ khi có tin nhắn mới trong POSIX message queue, sử dụng cơ chế `SIGEV_THREAD`.

## 📋 Yêu cầu

Viết 2 chương trình: `server.c` và `client.c`.

### 1. Server (`server.c`)

- Tạo POSIX message queue với tên `"/notify_queue"`
- Đăng ký notification với `mq_notify` dùng `SIGEV_THREAD`
- Thread callback được gọi tự động khi có tin nhắn mới
- Trong callback: nhận tin nhắn, xử lý, và đăng ký lại notification cho lần tiếp theo
- Hiển thị thông báo khi nhận được tin nhắn

### 2. Client (`client.c`)

- Mở queue (không tạo mới) với cờ `O_WRONLY`
- Gửi tin nhắn đến server
- Gửi tin nhắn "exit" để kết thúc

## ✅ Tiêu chí đạt

- Server dùng `mq_notify` với `SIGEV_THREAD` (không dùng polling)
- Thread callback `notification_handler` được gọi tự động mỗi khi có tin nhắn
- Server re-register notification sau mỗi lần nhận (theo POSIX spec)
- Chương trình biên dịch với `gcc -Wall -Wextra -pthread -lrt`
- Xử lý lỗi đầy đủ

## 💡 Gợi ý

- Cấu trúc `sigevent` cho `SIGEV_THREAD`:
  ```c
  #include <signal.h>
  #include <mqueue.h>

  struct sigevent sev;
  sev.sigev_notify = SIGEV_THREAD;
  sev.sigev_notify_function = handler;
  sev.sigev_notify_attributes = NULL; // NULL = default thread attributes
  ```

- Đăng ký notification:
  ```c
  if (mq_notify(mq, &sev) == -1) {
      perror("mq_notify");
      exit(1);
  }
  ```

- Trong handler, cần re-register notification ngay sau khi nhận:
  ```c
  static void handler(union sigval sv) {
      mqd_t *mq_ptr = (mqd_t *)sv.sival_ptr;
      // Receive message
      mq_receive(*mq_ptr, buffer, MAX_SIZE, NULL);
      // Re-register notification
      mq_notify(*mq_ptr, &sev);
  }
  ```

## 🔍 Kiểm tra

```bash
# Build
make

# Terminal 1 - Server
./server

# Terminal 2 - Client
./client
# Nhập tin nhắn, server sẽ tự động nhận và hiển thị

# Hoặc test nhanh:
echo "Hello from bash" > /dev/stdin &  # Cần chạy client riêng
```

## 📁 File cần tạo

- [ ] `server.c` - Server với mq_notify async notification
- [ ] `client.c` - Client gửi tin nhắn
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `07.IPC-messages-queues.md`:
- Section 4.1: Asynchronous Notification with mq_notify
- Section 4.2: SIGEV_THREAD vs SIGEV_SIGNAL
- Section 4.3: Re-registration Requirement

---

**Bonus**: Thêm tùy chọn dùng `SIGEV_SIGNAL` thay vì `SIGEV_THREAD`, xử lý tín hiệu `SIGUSR1` trong signal handler.

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
