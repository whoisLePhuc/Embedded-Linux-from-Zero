# Exercise 2: POSIX Message Queue

## 🎯 Mục tiêu
- Hiểu cách sử dụng Message Queue theo chuẩn POSIX (`mq_open`, `mq_send`, `mq_receive`).
- So sánh sự khác biệt với System V (dùng tên file ảo thay vì key, dùng library `rt`).

## 📝 Yêu cầu

Viết 2 chương trình: `sender.c` và `receiver.c`.

### 1. Structure và Constants
- Tên queue: `"/test_queue"` (Bắt buộc phải có dấu `/` ở đầu).
- Max size message: 1024 bytes.
- Link với thư viện real-time: thêm flag `-lrt` khi biên dịch.

### 2. Sender (`sender.c`)
- Mở queue bằng `mq_open` với cờ `O_WRONLY | O_CREAT`.
- Cấu hình attribute cho queue (`mq_flags`, `mq_maxmsg`, `mq_msgsize`, `mq_curmsgs`).
- Nhập tin nhắn từ bàn phím.
- Gửi tin nhắn bằng `mq_send`.
- Đóng queue bằng `mq_close`.

### 3. Receiver (`receiver.c`)
- Mở queue bằng `mq_open` với cờ `O_RDONLY | O_CREAT`.
- Nhận tin nhắn bằng `mq_receive`.
- In tin nhắn ra màn hình.
- Đóng queue và xóa queue bằng `mq_unlink` sau khi xong.

## 🧪 Hướng dẫn kiểm tra

1. Mở terminal 1: Build và chạy Sender
   ```bash
   make sender
   ./sender
   ```
2. Mở terminal 2: Build và chạy Receiver
   ```bash
   make receiver
   ./receiver
   ```
3. Nhập text bên Sender và quan sát Receiver.

## 💡 Gợi ý

- `mq_open(QUEUE_NAME, O_WRONLY | O_CREAT, 0666, &attr)`
- `mq_send(mq, buffer, strlen(buffer) + 1, 0)`
- `mq_receive(mq, buffer, MAX_SIZE, NULL)`
- Nhớ include `<mqueue.h>` và link `-lrt`.
