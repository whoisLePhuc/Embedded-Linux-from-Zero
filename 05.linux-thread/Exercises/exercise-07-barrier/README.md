# Bài tập 07: Thread Barrier Synchronization ⭐⭐

## 🎯 Mục tiêu
- Hiểu cách hoạt động của Barrier (`pthread_barrier_t`) trong đồng bộ hóa threads
- Biết cách chia computation thành nhiều phase với barrier giữa các phase
- Áp dụng barrier để giải bài toán parallel computation đồng bộ

## 📋 Yêu cầu

Viết chương trình thực hiện parallel array transformation với 3 phase, sử dụng barrier để đồng bộ:

1. Khai báo shared resources:
   - `int data[ARRAY_SIZE];` — mảng dùng chung (ARRAY_SIZE = 16)
   - `pthread_barrier_t barrier;` — barrier đồng bộ
   - Struct `ThreadRange` chứa: `thread_id`, `start`, `end` (chỉ mục đầu/cuối của phần mảng mà thread xử lý)

2. Viết `worker` thread function với 3 phase:
   - **Phase 1 (Initialize)**: Mỗi thread gán `data[i] = thread_id * 10 + (i - start)` cho phần tử trong range của nó. In ra: `[Thread X] Phase 1: data[start..end] initialized`
   - **Barrier**: Chờ tất cả threads hoàn thành Phase 1
   - **Phase 2 (Transform)**: Mỗi thread cộng `thread_id + 1` vào mỗi phần tử trong range của nó. In ra: `[Thread X] Phase 2: transformed chunk`
   - **Barrier**: Chờ tất cả threads hoàn thành Phase 2
   - **Phase 3 (Read cross-thread data)**: Mỗi thread đọc và in TOÀN BỘ mảng để chứng minh dữ liệu từ các thread khác đã sẵn sàng

3. Trong `main()`:
   - Khởi tạo barrier với `pthread_barrier_init(&barrier, NULL, NUM_THREADS)`
   - Tạo NUM_THREADS threads, mỗi thread xử lý `ARRAY_SIZE / NUM_THREADS` phần tử
   - Join tất cả threads
   - In mảng kết quả cuối cùng
   - Hủy barrier

## ✅ Tiêu chí đạt
- Chương trình biên dịch và chạy được: `./barrier_sync`
- Phase 2 chỉ bắt đầu sau khi TẤT CẢ threads hoàn thành Phase 1
- Phase 3 hiển thị đúng dữ liệu từ tất cả threads (chứng tỏ barrier hoạt động)
- Output hiển thị rõ ràng thứ tự các phase
- Xử lý lỗi cho barrier và pthread functions

## 💡 Gợi ý

- Barrier API:
  ```c
  pthread_barrier_t barrier;

  // Khởi tạo barrier cho N threads
  pthread_barrier_init(&barrier, NULL, NUM_THREADS);

  // Đợi tại barrier (block cho đến khi đủ N threads đến đây)
  int ret = pthread_barrier_wait(&barrier);
  // Trả về PTHREAD_BARRIER_SERIAL_THREAD cho một thread (thường là thread cuối)
  // Trả về 0 cho các thread còn lại

  // Hủy barrier
  pthread_barrier_destroy(&barrier);
  ```

- Pattern cho multi-phase computation:
  ```c
  void *worker(void *arg) {
      // Phase 1
      do_phase1(range);
      pthread_barrier_wait(&barrier);  // Sync: đợi phase 1

      // Phase 2 (phụ thuộc vào kết quả phase 1 của tất cả threads)
      do_phase2(range);
      pthread_barrier_wait(&barrier);  // Sync: đợi phase 2

      // Phase 3 (phụ thuộc vào phase 2 của tất cả threads)
      do_phase3(range);
      return NULL;
  }
  ```

- `PTHREAD_BARRIER_SERIAL_THREAD` thường được dùng để một thread (thường là thread cuối) thực hiện công việc đặc biệt sau barrier (ví dụ: in log, kiểm tra kết quả trung gian).

- Tính chỉ mục range cho mỗi thread:
  ```c
  int chunk_size = ARRAY_SIZE / NUM_THREADS;
  int start = id * chunk_size;
  int end = (id == NUM_THREADS - 1) ? ARRAY_SIZE : (id + 1) * chunk_size;
  ```

## 🔍 Kiểm tra

```bash
make
./barrier_sync

# Output mẫu:
# [Thread 0] Phase 1: data[0..3] = {0, 1, 2, 3}
# [Thread 1] Phase 1: data[4..7] = {10, 11, 12, 13}
# [Thread 2] Phase 1: data[8..11] = {20, 21, 22, 23}
# [Thread 3] Phase 1: data[12..15] = {30, 31, 32, 33}
# --- Barrier 1: All threads completed Phase 1 ---
# [Thread 2] Phase 2: transformed chunk
# [Thread 0] Phase 2: transformed chunk
# [Thread 1] Phase 2: transformed chunk
# [Thread 3] Phase 2: transformed chunk
# --- Barrier 2: All threads completed Phase 2 ---
# [Thread 0] Full array: {1, 2, 3, 4, 11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 34}
# ... (similar for other threads)
# Final array: 1 2 3 4 11 12 13 14 21 22 23 24 31 32 33 34
```

## 📁 File cần tạo
- [ ] `main.c` - Source code chương trình
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan
Đọc lại trong `linux-thread.md`:
- Section 5: Thread Synchronization
- man pages: `pthread_barrier_init(3)`, `pthread_barrier_wait(3)`

---
**Bonus 1:** Thêm barrier thứ 3 trước Phase 3 và dùng `PTHREAD_BARRIER_SERIAL_THREAD` để chỉ một thread in kết quả cuối cùng.

**Bonus 2:** Sửa bài toán thành parallel prefix sum: Phase 1 khởi tạo, Phase 2 mỗi thread tính prefix sum trên chunk của nó, barrier, Phase 3 hiệu chỉnh prefix sum dựa trên tổng của chunk trước đó.
