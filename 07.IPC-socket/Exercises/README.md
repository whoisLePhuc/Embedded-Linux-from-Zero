# BÀI TẬP VỀ IPC SOCKET

Bộ bài tập này được thiết kế để giúp bạn làm quen với Socket Programming trong Linux, bao gồm cả TCP và UDP.

## 📚 Cách sử dụng

Mỗi bài tập được đặt trong một thư mục riêng:
- `exercise-01-ipv4-tcp/` - TCP Echo Server/Client ⭐⭐
- `exercise-02-ipv4-udp/` - UDP Client/Server ⭐⭐
- `exercise-03-unix-domain/` - UNIX Domain Socket ⭐⭐
- `exercise-04-io-multiplexing/` - I/O Multiplexing với select() ⭐⭐⭐
- `exercise-05-epoll-server/` - Event-driven server với epoll ⭐⭐⭐

## 🎯 Hướng dẫn

1. Đọc file `README.md` trong mỗi thư mục bài tập.
2. Bài tập Socket thường yêu cầu 2 chương trình: **Server** và **Client**. Hãy code cả hai.
3. Mở 2 terminal:
   - Terminal 1: Chạy Server (`./server`)
   - Terminal 2: Chạy Client (`./client`)
4. Quan sát kết quả giao tiếp giữa 2 process.

## 📝 Mức độ

- ⭐⭐ Trung bình: Exercise 01, 02, 03
- ⭐⭐⭐ Khó: Exercise 04, 05

## 💡 Tips

- Luôn kiểm tra lỗi của các hàm `socket`, `bind`, `listen`, `accept`, `connect`.
- `perror()` là bạn thân của lập trình viên Socket.
- Sử dụng địa chỉ Loopback `127.0.0.1` để test trên cùng một máy.
- Port số nhỏ (< 1024) cần quyền root, hãy dùng port cao (v.d. 8080, 9000).

Chúc bạn học vui! 🚀
