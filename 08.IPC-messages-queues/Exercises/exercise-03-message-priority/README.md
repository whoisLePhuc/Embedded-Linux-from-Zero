# Bài tập 03: Message Priority ⭐⭐

## 🎯 Mục tiêu

Hiểu cách sử dụng message priority trong POSIX message queue, cách gửi và nhận tin nhắn theo thứ tự ưu tiên.

## 📋 Yêu cầu

Viết 2 chương trình: `sender.c` và `receiver.c`.

### 1. Sender (`sender.c`)

- Tạo/mở POSIX message queue với tên `"/msg_queue"` (cờ `O_WRONLY | O_CREAT`)
- Gửi tin nhắn với các mức priority khác nhau (0–31)
- Cho phép người dùng nhập nội dung và priority từ bàn phím
- Gửi và hiển thị priority của từng tin nhắn

### 2. Receiver (`receiver.c`)

- Mở queue ở chế độ `O_RDONLY`
- Nhận tin nhắn và hiển thị nội dung cùng với priority
- Quan sát thứ tự nhận: `mq_receive` luôn trả về tin nhắn có priority cao nhất
- Kết thúc khi nhận được tin nhắn đặc biệt (nội dung "exit")

## ✅ Tiêu chí đạt

- Chương trình biên dịch với `gcc -Wall -Wextra -lrt`
- Sender gửi được tin nhắn với priority từ 0–31
- Receiver nhận tin nhắn đúng thứ tự priority (cao nhất trước)
- Khi nhiều tin nhắn cùng priority, nhận theo FIFO
- Xử lý lỗi đầy đủ (`mq_open`, `mq_send`, `mq_receive`)

## 💡 Gợi ý

- Priority có giá trị từ 0 (thấp nhất) đến 31 (cao nhất):
  ```c
  // Gửi với priority = 10
  mq_send(mq, msg, strlen(msg) + 1, 10);
  ```

- Nhận priority của tin nhắn:
  ```c
  unsigned int prio;
  mq_receive(mq, buffer, MAX_SIZE, &prio);
  printf("Priority: %u, Message: %s\n", prio, buffer);
  ```

- Cấu hình attribute cho queue:
  ```c
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;
  attr.mq_msgsize = MAX_SIZE;
  attr.mq_curmsgs = 0;
  ```

## 🔍 Kiểm tra

```bash
# Build
make

# Terminal 1 - Receiver
./receiver

# Terminal 2 - Sender
./sender
# Nhập: "urgent" với priority 30
# Nhập: "normal" với priority 5
# Nhập: "critical" với priority 31
# Nhập: "exit" với priority 0

# Quan sát: "critical" đến trước, "urgent" đến sau, "normal" cuối cùng
```

## 📁 File cần tạo

- [ ] `sender.c` - Gửi tin nhắn với priority
- [ ] `receiver.c` - Nhận tin nhắn theo thứ tự priority
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `07.IPC-messages-queues.md`:
- Section 2.2: POSIX Message Queue Attributes
- Section 2.4: Message Prioritization and Scheduling
- Section 3.3: Priority Inversion and Prevention

---

**Bonus**: Thêm signal handler để xử lý CTRL+C. Khi nhận SIGINT, gửi tin nhắn "exit" với priority cao nhất (31) để receiver dừng ngay lập tức.

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
