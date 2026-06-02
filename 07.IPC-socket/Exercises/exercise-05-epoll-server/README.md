# Bài tập 05: epoll-based Event Server ⭐⭐⭐

## 🎯 Mục tiêu

- Hiểu kiến trúc event-driven và lợi ích của epoll so với select/poll.
- Nắm vững các hệ thống gọi `epoll_create1()`, `epoll_ctl()`, `epoll_wait()`.
- Xây dựng server hiệu năng cao xử lý nhiều client đồng thời.

## 📋 Yêu cầu

Viết chương trình `server.c` sử dụng epoll (Linux) để quản lý nhiều kết nối TCP:

1. Tạo TCP socket IPv4, bind port **8080**, listen.
2. Tạo epoll instance với `epoll_create1(0)`.
3. Thêm `server_fd` vào epoll với event `EPOLLIN` (level-triggered).
4. Vòng lặp `epoll_wait()`:
   - Nếu `server_fd` có event: `accept()` kết nối mới, thêm client socket vào epoll.
   - Nếu client socket có event: đọc dữ liệu, echo lại.
   - Nếu client ngắt kết nối (read == 0): đóng socket, xóa khỏi epoll.
5. Client socket được đặt ở chế độ **non-blocking** và dùng **edge-triggered** (`EPOLLET`).
6. Server chạy vô hạn, không fork, không thread.

## ✅ Tiêu chí đạt

- Server chấp nhận và xử lý nhiều client đồng thời (tối thiểu 10).
- Dùng epoll edge-triggered (`EPOLLET`) kết hợp non-blocking I/O.
- Xử lý đúng `EAGAIN` khi đọc hết dữ liệu trên socket non-blocking.
- Không rò rỉ file descriptor.
- Build không warning với `gcc -Wall -Wextra -std=c11`.

## 💡 Gợi ý

- Khởi tạo epoll:

```c
int epoll_fd = epoll_create1(0);
struct epoll_event ev, events[MAX_EVENTS];
ev.events = EPOLLIN;
ev.data.fd = server_fd;
epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);
```

- Với edge-triggered (`EPOLLET`), cần đặt socket non-blocking và đọc đến `EAGAIN`:

```c
int flags = fcntl(client_fd, F_GETFL, 0);
fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

// Trong vong lap doc:
while (1) {
    int n = read(fd, buf, sizeof(buf));
    if (n > 0) { /* xu ly */ }
    else if (n == 0) { /* dong */ break; }
    else if (errno == EAGAIN) { /* het du lieu */ break; }
}
```

- `epoll_wait()` trả về số lượng fd có event. Duyệt mảng `events` từ 0 đến `nfds - 1`.
- Phân biệt event từ `server_fd` (kết nối mới) và client fd (dữ liệu).
- Khi client ngắt kết nối, gọi `epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL)` trước `close(fd)`.

## 🔍 Kiểm tra

```bash
# Build
make

# Mở terminal 1: chạy server
./server

# Mở nhiều terminal: dùng netcat
nc -v 127.0.0.1 8080

# Hoặc stress test với nhiều kết nối cùng lúc
for i in {1..5}; do nc -v 127.0.0.1 8080 & done
```

## 📁 File cần tạo

- [ ] `server.c` — Source code epoll server
- [ ] `Makefile` — Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `IPC-socket.md`:
- Section 7.3: epoll() - Linux High-Performance
- Section 7.4: So sánh select vs poll vs epoll
- Section 8.3: Event-driven Server (epoll)

---

**Bonus:** So sánh hiệu năng giữa epoll (edge-triggered) với select(). Dùng `strace -c` để đếm system call. Thử kết nối 1000 client cùng lúc và quan sát CPU load.
