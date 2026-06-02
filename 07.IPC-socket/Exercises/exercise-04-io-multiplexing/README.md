# Bài tập 03: I/O Multiplexing với select ⭐⭐

## 🎯 Mục tiêu

- Hiểu khái niệm I/O multiplexing và khi nào cần sử dụng.
- Nắm vững cách sử dụng `select()` để giám sát nhiều file descriptor.
- Xây dựng server đơn luồng có thể xử lý nhiều client đồng thời.

## 📋 Yêu cầu

Viết chương trình `server.c` sử dụng `select()` để quản lý nhiều kết nối TCP:

1. Tạo TCP socket IPv4, bind port **8080**, listen với backlog phù hợp.
2. Sử dụng `select()` để giám sát đồng thời:
   - Socket lắng nghe (`server_fd`) — phát hiện kết nối mới.
   - Các socket client đã kết nối — phát hiện dữ liệu đến hoặc ngắt kết nối.
3. Khi có kết nối mới: `accept()` và thêm socket mới vào danh sách quản lý.
4. Khi client gửi dữ liệu: đọc nội dung và echo ngược lại.
5. Khi client ngắt kết nối: đóng socket và xóa khỏi danh sách quản lý.
6. Server chạy vô hạn, **không fork hay thread** — chỉ một luồng đơn với `select()`.

## ✅ Tiêu chí đạt

- Server chấp nhận tối thiểu **3 client đồng thời**.
- Dữ liệu từ client A không ảnh hưởng đến client B.
- Không xảy ra tình trạng treo (block) khi một client không gửi dữ liệu.
- Xử lý đúng khi client ngắt kết nối (không crash).
- Build không warning với `gcc -Wall -Wextra`.

## 💡 Gợi ý

- Khởi tạo mảng `client_sockets[MAX_CLIENTS]` với giá trị 0 để lưu các socket.
- Sử dụng `FD_ZERO`, `FD_SET`, `FD_ISSET` để thao tác với `fd_set`.
- `select()` nhận tham số `nfds` là (giá trị socket lớn nhất + 1).
- Reset `fd_set` mỗi vòng lặp — tất cả socket cần monitor đều phải được `FD_SET` lại.
- Khi `FD_ISSET(server_fd, &readfds)` là true: có kết nối mới.
- Khi `read()` trả về 0: client đã đóng kết nối.

```c
// Cấu trúc vòng lặp select cơ bản:
fd_set readfds;
FD_ZERO(&readfds);
FD_SET(server_fd, &readfds);
// ... FD_SET cho các client socket ...
int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
```

## 🔍 Kiểm tra

```bash
# Build
make

# Mở terminal 1: chạy server
./server

# Mở terminal 2, 3, 4: dùng netcat hoặc telnet
nc -v 127.0.0.1 8080
# hoặc
telnet 127.0.0.1 8080

# Gõ tin nhắn từ mỗi client, quan sát server echo lại
```

## 📁 File cần tạo

- [ ] `server.c` — Source code server sử dụng select()
- [ ] `Makefile` — Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `IPC-socket.md`:
- Section 7.1: I/O Multiplexing với select()
- Section 8: Multi-Client Server Models

---

**Bonus:** Thêm timeout cho `select()`. Nếu không có activity trong 5 giây, in ra "Server is idle..." và tiếp tục chờ.
