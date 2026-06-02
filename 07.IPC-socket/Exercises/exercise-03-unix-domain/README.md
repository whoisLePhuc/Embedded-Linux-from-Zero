# Bài tập 04: UNIX Domain Socket ⭐⭐

## 🎯 Mục tiêu

- Hiểu khái niệm UNIX Domain Socket và khi nào nên dùng thay vì TCP/IP loopback.
- Nắm vững cấu trúc `struct sockaddr_un` và họ địa chỉ `AF_UNIX`.
- So sánh UNIX Domain Socket với TCP Socket (hiệu năng, địa chỉ, use case).

## 📋 Yêu cầu

Viết 2 chương trình: **Server** và **Client** giao tiếp qua UNIX Domain Stream Socket.

### 1. Server (`server.c`)
- Tạo UNIX Domain Stream socket (`AF_UNIX`, `SOCK_STREAM`).
- Thiết lập địa chỉ socket tại đường dẫn `"/tmp/echo.sock"`.
- Bind, listen, accept một client.
- Đọc dữ liệu từ client và echo ngược lại.
- Đóng kết nối khi client gửi "exit".
- **Quan trọng:** Xóa file socket cũ (nếu tồn tại) trước khi bind.

### 2. Client (`client.c`)
- Tạo UNIX Domain Stream socket.
- Kết nối tới server tại `"/tmp/echo.sock"`.
- Cho phép người dùng nhập tin nhắn từ bàn phím.
- Gửi tin nhắn tới server và nhận phản hồi.

## ✅ Tiêu chí đạt

- Server và client giao tiếp thành công qua UNIX socket.
- Không cần mạng — có thể chạy trên máy không có loopback interface.
- Xử lý lỗi khi file socket đã tồn tại (`unlink` trước khi bind).
- Client gửi "exit" thì server đóng kết nối sạch sẽ.
- Build không warning với `gcc -Wall -Wextra`.
- Sử dụng `struct sockaddr_un` và hằng `AF_UNIX`.

## 💡 Gợi ý

- Khai báo và cấu hình:

```c
#include <sys/un.h>

struct sockaddr_un addr;
addr.sun_family = AF_UNIX;
strcpy(addr.sun_path, "/tmp/echo.sock");
```

- Trước khi bind, luôn gọi `unlink("/tmp/echo.sock")` để xóa file cũ.
- UNIX Domain Socket nhanh hơn TCP loopback vì không qua network stack.
- Khi bind, có thể dùng `addrlen = sizeof(sa_family_t) + strlen(path)` thay vì `sizeof(addr)` để tương thích với các hệ thống khác.
- Kiểm tra socket bằng `ls -la /tmp/echo.sock` và `file /tmp/echo.sock`.
- Độ dài tối đa của `sun_path` là `sizeof(addr.sun_path)` (thường 108 bytes).

## 🔍 Kiểm tra

```bash
# Build
make

# Mở terminal 1: chạy server
./server

# Mở terminal 2: chạy client
./client

# Nhập tin nhắn bên client, xem server echo lại

# Kiểm tra socket file
ls -la /tmp/echo.sock
file /tmp/echo.sock
```

## 📁 File cần tạo

- [ ] `server.c` — Source code UNIX Domain Socket server
- [ ] `client.c` — Source code UNIX Domain Socket client
- [ ] `Makefile` — Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `IPC-socket.md`:
- Section 5.1: Unix Socket Address (`sockaddr_un`)
- Section 5.2: Unix Domain Stream Server
- Section 5.3: Unix Domain Stream Client

---

**Bonus:** Sửa server để xử lý nhiều client (accept lần lượt). Hoặc nâng cấp dùng `select()` để xử lý đồng thời nhiều client.
