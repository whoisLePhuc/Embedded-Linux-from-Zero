# Exercise 1: TCP Echo Server (IPv4)

## 🎯 Mục tiêu
- Hiểu mô hình Client-Server sử dụng TCP.
- Nắm vững quy trình tạo socket TCP: `socket`, `bind`, `listen`, `accept`, `connect`.
- Thực hành gửi và nhận dữ liệu qua socket với `send` và `recv`.

## 📝 Yêu cầu

Viết 2 chương trình: **Server** và **Client**.

### 1. Server (`server.c`)
- Tạo một TCP socket IPv4.
- Bind socket tới port **8080** và IP **INADDR_ANY** (lắng nghe mọi interface).
- Listen các kết nối đến.
- Khi có Client kết nối (`accept` thành công):
    - In ra thông tin IP và Port của Client.
    - Đợi nhận dữ liệu từ Client (`recv`).
    - Gửi ngược lại đúng dữ liệu đó cho Client (Echo).
    - Nếu Client ngắt kết nối hoặc gửi "exit", đóng kết nối và tiếp tục đợi Client khác (hoặc thoát tùy bạn).

### 2. Client (`client.c`)
- Tạo một TCP socket IPv4.
- Kết nối tới Server tại địa chỉ **127.0.0.1** (localhost), port **8080**.
- Cho phép người dùng nhập chuỗi từ bàn phím.
- Gửi chuỗi đó tới Server (`send`).
- Nhận dữ liệu phản hồi từ Server (`recv`) và in ra màn hình.

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
3. Nhập text bên Client và xem Server echo lại.

## 💡 Gợi ý

- `struct sockaddr_in`
- `htons()`, `inet_pton()`
- Vòng lặp `while(1)` trong Server để xử lý liên tục.
