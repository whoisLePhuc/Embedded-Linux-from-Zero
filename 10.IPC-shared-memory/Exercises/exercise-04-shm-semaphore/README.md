# Bài tập 04: Shared Memory với Semaphore Sync ⭐⭐

## 🎯 Mục tiêu

Kết hợp POSIX shared memory (`shm_open` + `mmap`) với POSIX named semaphore (`sem_open`) để giải quyết bài toán producer-consumer đồng bộ.

## 📋 Yêu cầu

Viết 2 chương trình: `producer.c` và `consumer.c`.

### 1. Producer (`producer.c`)

- Tạo shared memory object với tên `"/shm_buffer"` dùng `shm_open()` + `ftruncate()` + `mmap()`
- Shared memory chứa struct với: mảng số nguyên (10 phần tử), semaphore và chỉ số write index
- Sử dụng named semaphore:
  - `sem_prod`: đếm số slot trống (khởi tạo = 10)
  - `sem_cons`: đếm số item có sẵn (khởi tạo = 0)
- Tạo 10 số ngẫu nhiên, ghi vào buffer, chờ giữa các lần ghi

### 2. Consumer (`consumer.c`)

- Mở shared memory object `"/shm_buffer"` với `shm_open()` + `mmap()`
- Mở semaphore `sem_prod` và `sem_cons`
- Đọc và hiển thị các số từ buffer
- Dừng sau khi đọc đủ số lượng

## ✅ Tiêu chí đạt

- Sử dụng `shm_open()` với `O_CREAT | O_RDWR` cho producer
- Sử dụng `shm_open()` với `O_RDWR` (không O_CREAT) cho consumer
- Dùng `sem_open()` với tên bắt đầu bằng `/`
- Producer không ghi đè khi buffer đầy (chờ sem_prod)
- Consumer không đọc khi buffer rỗng (chờ sem_cons)
- Cleanup: unlink shared memory và semaphore
- Chương trình biên dịch với `gcc -Wall -Wextra -pthread -lrt`

## 💡 Gợi ý

- Cấu trúc shared memory:
  ```c
  #define BUFFER_SIZE 10

  struct shared_buffer {
      int items[BUFFER_SIZE];
      int write_idx;
      int read_idx;
  };
  ```

- Tạo shared memory:
  ```c
  int fd = shm_open("/shm_buffer", O_CREAT | O_RDWR, 0644);
  ftruncate(fd, sizeof(struct shared_buffer));
  struct shared_buffer *buf = mmap(NULL, sizeof(struct shared_buffer),
                                    PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  ```

- Tạo semaphore:
  ```c
  sem_t *sem_prod = sem_open("/sem_prod", O_CREAT, 0644, BUFFER_SIZE);
  sem_t *sem_cons = sem_open("/sem_cons", O_CREAT, 0644, 0);
  ```

- Producer loop:
  ```c
  sem_wait(sem_prod);   // Wait for empty slot
  buf->items[buf->write_idx % BUFFER_SIZE] = value;
  buf->write_idx++;
  sem_post(sem_cons);   // Signal item available
  ```

## 🔍 Kiểm tra

```bash
# Build
make

# Terminal 1 - Consumer (chạy trước hoặc sau đều được)
./consumer

# Terminal 2 - Producer
./producer
```

## 📁 File cần tạo

- [ ] `producer.c` - Producer ghi vào shared memory
- [ ] `consumer.c` - Consumer đọc từ shared memory
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `09.IPC-shared-memory.md`:
- Section 2.1: POSIX Shared Memory Objects
- Section 2.2: Memory Mapping with mmap()
- Section 3.1: Synchronization with Semaphores

---

**Bonus**: Thêm mutex (semaphore nhị phân) để bảo vệ write_idx/read_idx, cho phép nhiều producer hoặc consumer.

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
