# Exercise 2: Named Pipe (FIFO)

## 🎯 Mục tiêu
- Hiểu cách tạo và sử dụng Named Pipe (FIFO).
- Thực hiện giao tiếp giữa 2 process hoàn toàn độc lập (không có quan hệ cha-con).
- Hiểu cơ chế blocking của FIFO khi mở (open).

## 📝 Yêu cầu

Viết 2 chương trình độc lập: `producer.c` và `consumer.c`.

### 1. Producer (Người ghi)
- Tạo FIFO có tên `myfifo` (nếu chưa tồn tại) dùng `mkfifo`.
- Mở FIFO để **Ghi** (`O_WRONLY`).
    - *Lưu ý*: Hàm `open` sẽ block cho đến khi có process khác mở FIFO để đọc.
- Cho phép người dùng nhập chuỗi từ bàn phím.
- Ghi chuỗi vào FIFO.
- Gõ "exit" để thoát.

### 2. Consumer (Người đọc)
- Mở FIFO `myfifo` để **Đọc** (`O_RDONLY`).
- Đọc dữ liệu từ FIFO và in ra màn hình.
- Kết thúc khi nhận được chuỗi "exit" hoặc khi Producer đóng kết nối (EOF).

## 🧪 Hướng dẫn kiểm tra

1. Mở Terminal 1 (Consumer):
   ```bash
   make consumer
   ./consumer
   ```
   (Chương trình sẽ block đợi Producer)

2. Mở Terminal 2 (Producer):
   ```bash
   make producer
   ./producer
   ```

3. Nhập text bên Producer, Consumer sẽ hiển thị ngay lập tức.

## 💡 Gợi ý

- `mkfifo("myfifo", 0666)`
- `open("myfifo", O_WRONLY)` vs `open("myfifo", O_RDONLY)`
- `fgets(buffer, size, stdin)`
- `write(fd, buffer, size)` / `read(fd, buffer, size)`
