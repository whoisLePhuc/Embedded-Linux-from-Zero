# Exercise 3: FIFO Client-Server Model

## 🎯 Mục tiêu
- Xây dựng mô hình Client-Server phục vụ nhiều Client cùng lúc bằng Named Pipes (FIFO).
- Hiểu cách sử dụng "Well-known FIFO" để nhận request và "Private FIFO" để gửi response.

## 📝 Kiến trúc

1.  **Server**:
    - Tạo một FIFO công khai (Well-known FIFO) tên là `/tmp/seqnum_fifo`.
    - Mở FIFO này để đọc (`O_RDONLY`).
    - Liên tục đọc các request struct từ các Client gửi đến.
    - Xử lý request (ví dụ: cấp phát số thứ tự sequence number).
    - Mở Private FIFO của Client (tên được gửi trong request) để gửi phản hồi.

2.  **Client**:
    - Tạo một Private FIFO của riêng mình (ví dụ `/tmp/seqnum_cl_1234` với 1234 là PID).
    - Gửi request chứa PID và độ dài sequence mong muốn tới Well-known FIFO của Server.
    - Mở Private FIFO của mình để đọc response.
    - Nhận kết quả và in ra.
    - Xóa Private FIFO khi thoát.

## 💾 Cấu trúc dữ liệu (`common.h`)

```c
#define SERVER_FIFO "/tmp/seqnum_server"
#define CLIENT_FIFO_TEMPLATE "/tmp/seqnum_cl_%d"

struct request {
    pid_t pid;      // PID của Client (để Server biết gửi về đâu)
    int seq_len;    // Độ dài chuỗi số muốn xin
};

struct response {
    int seq_num;    // Số thứ tự đầu tiên được cấp
};
```

## 📝 Yêu cầu

Viết `server.c` và `client.c` thực hiện luồng trên.

## 🧪 Hướng dẫn kiểm tra

1. Mở Terminal 1 (Server):
   ```bash
   make server
   ./server
   ```

2. Mở Terminal 2 (Client 1):
   ```bash
   make client
   ./client 5
   # Output: Client [PID] received seq: 0
   ```

3. Mở Terminal 3 (Client 2):
   ```bash
   ./client 10
   # Output: Client [PID] received seq: 5
   ```
   (Số thứ tự sẽ tăng dần do Server quản lý biến toàn cục).

## 💡 Gợi ý

- Dùng `snprintf` để tạo tên file FIFO động dựa trên PID.
- `mkfifo` có thể thất bại nếu file đã tồn tại -> `errno == EEXIST` là bình thường.
- Server cần handle trường hợp Client bị ngắt kết nối giữa chừng (write vào pipe vỡ sẽ gây SIGPIPE).
