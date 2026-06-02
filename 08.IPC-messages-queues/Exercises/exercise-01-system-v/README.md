# Exercise 1: System V Message Queue

## 🎯 Mục tiêu
- Hiểu cách tạo Message Queue theo chuẩn System V (`ftok`, `msgget`).
- Thực hiện giao tiếp giữa 2 process: Sender và Receiver.
- Hiểu cấu trúc `msgbuf` và cách gửi/nhận message theo `mtype`.

## 📝 Yêu cầu

Viết 2 chương trình: `sender.c` và `receiver.c`.

### 1. Common (`common.h`)
- Định nghĩa struct message dùng chung:
  ```c
  struct msg_buffer {
      long msg_type;
      char msg_text[100];
  };
  ```
- Định nghĩa đường dẫn file cho `ftok` (ví dụ: tạo file "progfile").

### 2. Sender (`sender.c`)
- Tạo Key sử dụng `ftok`.
- Lấy Queue ID bằng `msgget`.
- Nhập tin nhắn từ bàn phím.
- Gửi tin nhắn vào queue với `msg_type = 1`.
- In ra thông báo "Data sent is ...".

### 3. Receiver (`receiver.c`)
- Tạo Key (giống sender).
- Lấy Queue ID.
- Nhận tin nhắn có `msg_type = 1` bằng `msgrcv`.
- In tin nhắn ra màn hình.
- Sau khi nhận xong, xóa message queue bằng `msgctl` với `IPC_RMID`.

## 🧪 Hướng dẫn kiểm tra

1. Tạo file dummy để dùng cho ftok: `touch progfile`
2. Mở terminal 1: Build và chạy Sender
   ```bash
   make sender
   ./sender
   ```
3. Mở terminal 2: Build và chạy Receiver
   ```bash
   make receiver
   ./receiver
   ```
4. Nhập text bên Sender, Receiver sẽ nhận được và in ra.

## 💡 Gợi ý

- `ftok("progfile", 65)`
- `msgget(key, 0666 | IPC_CREAT)`
- `msgsnd(msgid, &message, sizeof(message), 0)`
- `msgrcv(msgid, &message, sizeof(message), 1, 0)`
