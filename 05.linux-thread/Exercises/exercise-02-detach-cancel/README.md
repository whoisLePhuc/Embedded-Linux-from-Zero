# Bài tập 05: Thread Detach và Cancel ⭐⭐

## 🎯 Mục tiêu
- Hiểu cách tách thread (detach) để tự động giải phóng tài nguyên
- Biết cách hủy thread từ thread khác bằng `pthread_cancel()`
- Sử dụng `pthread_cleanup_push()/pop()` để dọn dẹp tài nguyên khi thread bị cancel
- Phân biệt deferred cancellation và asynchronous cancellation

## 📋 Yêu cầu

Viết chương trình mô phỏng một server xử lý tác vụ với background worker threads:

1. Tạo một `cleanup_handler` function để giải phóng bộ nhớ đã cấp phát cho worker
2. Viết `worker` thread function:
   - Nhận thread ID qua argument
   - Gọi `pthread_detach(pthread_self())` để tự detach ngay khi bắt đầu
   - Cấp phát một buffer trên heap (mô phỏng tài nguyên, ví dụ: `char *buffer = malloc(64)`)
   - Đăng ký cleanup handler để tự động `free(buffer)` nếu thread bị cancel
   - Vòng lặp vô hạn: in message và `sleep(1)` (mô phỏng xử lý task)
   - Dùng `pthread_cleanup_push()` và `pthread_cleanup_pop(0)`
3. Trong `main()`:
   - Tạo 2 worker threads với cancel type khác nhau:
     - Worker 1: deferred cancellation (`PTHREAD_CANCEL_DEFERRED`)
     - Worker 2: asynchronous cancellation (`PTHREAD_CANCEL_ASYNCHRONOUS`)
   - Để workers chạy trong 3 giây
   - Gọi `pthread_cancel()` cho cả 2 workers
   - Đợi 1 giây, in "All workers cancelled"
4. Quan sát sự khác biệt trong output giữa deferred và async cancellation

## ✅ Tiêu chí đạt
- Chương trình biên dịch và chạy được: `./detach_cancel`
- Worker tự detach thành công (không cần pthread_join)
- Cleanup handler được gọi tự động khi thread bị cancel
- Không có memory leak (buffer được free dù thread bị cancel)
- Output thể hiện rõ sự khác biệt giữa deferred và async cancellation
- Xử lý lỗi cho `pthread_create()`, `pthread_cancel()`

## 💡 Gợi ý

- Đặt cancel type ở đầu thread function:
  ```c
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  ```

- Cleanup handler pattern:
  ```c
  void cleanup(void *arg) {
      printf("[Cleanup] Freeing buffer at %p\n", arg);
      free(arg);
  }

  // Trong thread function:
  char *buffer = malloc(64);
  pthread_cleanup_push(cleanup, buffer);
  // ... work loop (có thể bị cancel) ...
  pthread_cleanup_pop(0);  // Execute=0: chỉ chạy cleanup nếu bị cancel
  ```

- `pthread_cleanup_push()` và `pthread_cleanup_pop()` phải xuất hiện trong cùng một block scope (cùng cặp dấu `{}`)

- Deferred cancellation: chỉ kiểm tra cancel tại **cancellation points** (sleep, printf, read, write...). Worker có thể in thêm vài message sau khi `pthread_cancel()` được gọi.

- Async cancellation: thread có thể bị terminate ở **bất kỳ instruction nào**, kể cả giữa lúc đang cấp phát bộ nhớ.

- Dùng `pthread_self()` để lấy ID của thread hiện tại trong cleanup handler.

## 🔍 Kiểm tra

```bash
make
./detach_cancel

# Quan sát output mẫu (thứ tự có thể khác):
# [Worker 1] Started with DEFERRED cancellation
# [Worker 2] Started with ASYNCHRONOUS cancellation
# [Worker 1] Processing task...
# [Worker 2] Processing task...
# ...
# Main: Waiting 3 seconds...
# Main: Cancelling all workers...
# [Cleanup] Freeing buffer at 0x...
# [Worker 1] Processing task...     (deferred: có thể in thêm 1 lần)
# [Cleanup] Freeing buffer at 0x...
# Main: All workers cancelled
```

## 📁 File cần tạo
- [ ] `main.c` - Source code chương trình
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan
Đọc lại trong `linux-thread.md`:
- Section 4.3: Thread Detaching
- Section 4.4: pthread_detach()
- Section 3.3: Terminate Thread (pthread_cancel)

---
**Bonus:** Chạy với `valgrind ./detach_cancel` để kiểm tra không có memory leak. Thử bỏ cleanup handler và xem leak report của valgrind.
