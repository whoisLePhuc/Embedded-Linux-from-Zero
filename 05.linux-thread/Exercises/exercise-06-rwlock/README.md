# Bài tập 06: Read-Write Lock ⭐⭐

## 🎯 Mục tiêu
- Hiểu cách hoạt động của Read-Write Lock (`pthread_rwlock_t`)
- Phân biệt read lock (cho nhiều readers đồng thời) và write lock (độc quyền)
- So sánh hiệu năng giữa mutex và rwlock trong tình huống nhiều reads, ít writes

## 📋 Yêu cầu

Viết chương trình mô phỏng một configuration cache được chia sẻ giữa nhiều threads:

1. Khai báo shared resources toàn cục:
   - `int config_cache[CACHE_SIZE];` — mảng dữ liệu dùng chung (CACHE_SIZE = 64)
   - `int total_writes;` — đếm tổng số lần ghi
   - `pthread_mutex_t mutex;` — dùng cho benchmark mutex
   - `pthread_rwlock_t rwlock;` — dùng cho benchmark rwlock

2. Viết 4 thread functions:
   - `reader_mutex()`: Lock mutex, tính tổng toàn bộ mảng `config_cache`, unlock. Lặp READ_LOOPS lần
   - `writer_mutex()`: Lock mutex, tăng mỗi phần tử của mảng lên 1, `total_writes++`, unlock. Lặp WRITE_LOOPS lần
   - `reader_rwlock()`: Giống reader_mutex nhưng dùng `pthread_rwlock_rdlock()`
   - `writer_rwlock()`: Giống writer_mutex nhưng dùng `pthread_rwlock_wrlock()`

3. Trong `main()`:
   - Benchmark mutex: tạo NUM_READERS reader threads + NUM_WRITERS writer threads, đo thời gian
   - Benchmark rwlock: tạo cùng số lượng threads, đo thời gian
   - In kết quả so sánh

## ✅ Tiêu chí đạt
- Chương trình biên dịch và chạy được: `./rwlock_benchmark`
- Kết quả `shared_config` cuối cùng đúng với cả 2 phương pháp
- Benchmark rwlock nhanh hơn mutex khi có nhiều reader threads
- Xử lý lỗi cho các pthread functions
- Output hiển thị rõ ràng thời gian của từng phương pháp

## 💡 Gợi ý

- Read-Write Lock API:
  ```c
  pthread_rwlock_t rwlock;

  // Khởi tạo (hoặc dùng PTHREAD_RWLOCK_INITIALIZER)
  pthread_rwlock_init(&rwlock, NULL);

  // Read lock — nhiều threads có thể lấy cùng lúc
  pthread_rwlock_rdlock(&rwlock);
  int sum = 0;
  for (int j = 0; j < CACHE_SIZE; j++)
      sum += config_cache[j];  // Critical section (read-only)
  pthread_rwlock_unlock(&rwlock);

  // Write lock — chỉ 1 thread, không ai được đọc
  pthread_rwlock_wrlock(&rwlock);
  for (int j = 0; j < CACHE_SIZE; j++)
      config_cache[j]++;  // Critical section (write)
  pthread_rwlock_unlock(&rwlock);

  // Hủy
  pthread_rwlock_destroy(&rwlock);
  ```

- Đo thời gian bằng `clock_gettime()`:
  ```c
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  // ... benchmark ...
  clock_gettime(CLOCK_MONOTONIC, &end);
  double elapsed = (end.tv_sec - start.tv_sec) +
                   (end.tv_nsec - start.tv_nsec) / 1e9;
  ```

- Với rwlock, reader threads KHÔNG block nhau → nhiều readers có thể đọc đồng thời
- Với mutex, reader threads block nhau dù chỉ đọc → performance kém hơn

- Số threads đề xuất: 6 readers + 2 writers, READ_LOOPS = 50000, WRITE_LOOPS = 1000

## 🔍 Kiểm tra

```bash
make
./rwlock_benchmark

# Output mẫu:
# === Mutex Benchmark ===
# 6 readers (200000 loops), 2 writers (2000 loops)
# Total writes: 4000 | cache[0] = 4001
# Time: 0.392 seconds
#
# === RWLock Benchmark ===
# 6 readers (200000 loops), 2 writers (2000 loops)
# Total writes: 4000 | cache[0] = 4001
# Time: 0.137 seconds
#
# === Performance Comparison ===
# Mutex  time: 0.392 seconds
# RWLock time: 0.137 seconds
# Speedup: 2.85x (RWLock is faster)
```

## 📁 File cần tạo
- [ ] `main.c` - Source code chương trình
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan
Đọc lại trong `linux-thread.md`:
- Section 5: Thread Synchronization
- Section 6: Thread Sync - Mutex Lock
- Bảng so sánh synchronization primitives (Section 5)

---
**Bonus:** Thay đổi tỉ lệ readers/writers (ví dụ: 1 reader + 7 writers). Khi nào rwlock chậm hơn mutex? Giải thích tại sao.
