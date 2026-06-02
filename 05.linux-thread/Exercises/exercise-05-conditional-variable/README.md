# Exercise 04: Condition Variable (Producer-Consumer)

## 🎯 Mục tiêu
- Hiểu cách hoạt động của Conditional Variable (`pthread_cond_t`)
- Thực hiện bài toán Producer-Consumer đơn giản

## 📝 Yêu cầu

Viết chương trình mô phỏng Producer-Consumer:
1. Tài nguyên: `int buffer = 0;` (chỉ chứa 1 sản phẩm 0 hoặc 1)
   - 0: Buffer rỗng
   - 1: Buffer có data
2. **Producer Thread**:
   - Vòng lặp 5 lần
   - Nếu `buffer == 1` (đầy), chờ Consumer lấy đi (`pthread_cond_wait`)
   - Nếu `buffer == 0`, sản xuất (gán `buffer = 1`), in "Produced: 1", và báo hiệu (`pthread_cond_signal`)
3. **Consumer Thread**:
   - Vòng lặp 5 lần
   - Nếu `buffer == 0` (rỗng), chờ Producer tạo ra (`pthread_cond_wait`)
   - Nếu `buffer == 1`, tiêu thụ (gán `buffer = 0`), in "Consumed: 1", và báo hiệu (`pthread_cond_signal`)
4. Sử dụng Mutex để bảo vệ biến `buffer` và dùng cùng với Condition Variable.

## 🔧 Biên dịch và Chạy

```bash
make
./producer_consumer
```

## 💡 Gợi ý
- Luôn kiểm tra điều kiện trong vòng lặp `while` khi dùng `pthread_cond_wait` (đề phòng spurious wakeups):
  ```c
  pthread_mutex_lock(&lock);
  while (condition_is_false) {
      pthread_cond_wait(&cond, &lock);
  }
  // Do work
  pthread_mutex_unlock(&lock);
  ```
