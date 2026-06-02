# Exercise 2: UDP Client-Server (IPv4)

## 🎯 Mục tiêu
- Hiểu mô hình giao tiếp không kết nối (Connectionless) sử dụng UDP.
- Nắm vững các hàm `socket`, `bind`, `sendto`, `recvfrom`.
- So sánh sự khác nhau với TCP (không có `listen`, `accept`, `connect`).

## 📝 Yêu cầu

Viết 2 chương trình: **Server** và **Client** để trao đổi message.

### 1. Server (`server.c`)
- Tạo một UDP socket IPv4 (`SOCK_DGRAM`).
- Bind socket tới port **8080** và IP **INADDR_ANY**.
- Vòng lặp:
    - Chờ nhận dữ liệu từ Client (`recvfrom`).
    - In ra nội dung tin nhắn và thông tin người gửi (IP:Port).
    - Gửi phản hồi "Hello from Server" lại cho Client (`sendto`).

### 2. Client (`client.c`)
- Tạo một UDP socket IPv4.
- Không cần Bind (hệ thống tự cấp port ngẫu nhiên).
- Nhập message từ bàn phím.
- Gửi message tới Server 127.0.0.1:8080 (`sendto`).
- Chờ nhận phản hồi từ Server (`recvfrom`).
- In phản hồi ra màn hình.

## 🧪 Hướng dẫn kiểm tra

1. Mở terminal 1: Build và chạy Server
   ```bash
   make server
   ./server
   ```
2. Mở terminal 2: Build và chạy Client
   ```bash
   make client
   ./client
   ```
3. Nhập text bên Client và xem Server nhận được, Client nhận phản hồi.

## 💡 Gợi ý

- Hàm `recvfrom` và `sendto` yêu cầu tham số `struct sockaddr *` để lưu/biết địa chỉ đối phương.
- UDP không đảm bảo thứ tự hoặc độ tin cậy, nhưng nhanh và đơn giản hơn TCP.
