# Bài tập 04: Producer-Consumer với Semaphores ⭐⭐

## 🎯 Mục tiêu

Giải bài toán kinh điển Producer-Consumer với bounded buffer sử dụng multiple counting semaphores trong môi trường đa luồng.

## 📋 Yêu cầu

Viết chương trình `main.c` với các luồng (threads):

### 1. Cấu trúc dữ liệu
- Bounded buffer (circular buffer) với kích thước cố định (ví dụ: 5)
- 3 counting semaphores:
  - `sem_empty`: đếm số slot trống (khởi tạo = BUFFER_SIZE)
  - `sem_full`: đếm số slot có dữ liệu (khởi tạo = 0)
  - `mutex`: semaphore nhị phân (khởi tạo = 1) để bảo vệ buffer index

### 2. Producer Thread(s)
- Tạo 2 producer threads
- Mỗi producer sinh số ngẫu nhiên và đưa vào buffer
- Chờ `sem_empty` trước khi ghi
- Lock `mutex`, ghi dữ liệu, unlock `mutex`
- Post `sem_full` sau khi ghi

### 3. Consumer Thread(s)
- Tạo 2 consumer threads
- Mỗi consumer lấy số từ buffer
- Chờ `sem_full` trước khi đọc
- Lock `mutex`, đọc dữ liệu, unlock `mutex`
- Post `sem_empty` sau khi đọc

## ✅ Tiêu chí đạt

- Dùng `sem_init()` cho unnamed semaphore (không dùng named)
- Sử dụng đúng 3 semaphore: empty, full, mutex
- Không có data race (mutex bảo vệ buffer index)
- Không có deadlock
- Mỗi producer/consumer chạy đúng số lần quy định
- Chương trình biên dịch với `gcc -Wall -Wextra -pthread`

## 💡 Gợi ý

- Cấu trúc shared data:
  ```c
  #define BUFFER_SIZE 5
  #define NUM_ITEMS   10

  int buffer[BUFFER_SIZE];
  int in = 0, out = 0;

  sem_t sem_empty;
  sem_t sem_full;
  sem_t mutex;
  ```

- Khởi tạo semaphore:
  ```c
  sem_init(&sem_empty, 0, BUFFER_SIZE); // Counting: BUFFER_SIZE empty slots
  sem_init(&sem_full, 0, 0);            // Counting: 0 full slots
  sem_init(&mutex, 0, 1);              // Binary: mutex
  ```

- Producer:
  ```c
  sem_wait(&sem_empty);  // Wait for empty slot
  sem_wait(&mutex);      // Lock
  buffer[in] = value;
  in = (in + 1) % BUFFER_SIZE;
  sem_post(&mutex);      // Unlock
  sem_post(&sem_full);   // Signal item available
  ```

- Consumer:
  ```c
  sem_wait(&sem_full);   // Wait for item
  sem_wait(&mutex);      // Lock
  value = buffer[out];
  out = (out + 1) % BUFFER_SIZE;
  sem_post(&mutex);      // Unlock
  sem_post(&sem_empty);  // Signal empty slot
  ```

## 🔍 Kiểm tra

```bash
# Build
make

# Chạy
./producer_consumer
```

## 📁 File cần tạo

- [ ] `main.c` - Source code với threads
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `10.IPC-semaphore.md`:
- Section 1.1: Unnamed POSIX Semaphores
- Section 2.1: Counting Semaphores
- Section 3.1: Producer-Consumer Pattern

---

**Bonus**: Thêm tham số dòng lệnh để cấu hình số lượng producer, consumer, và buffer size. Xử lý SIGINT để dừng sớm.

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
