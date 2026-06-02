# Exercise 03: Race Condition and Mutex

## 🎯 Mục tiêu
- Hiểu được Race Condition là gì thông qua thực hành
- Sử dụng `pthread_mutex_t` để ngăn chặn Race Condition

## 📝 Yêu cầu

### Phần 1: Tái tạo Race Condition
1. Viết chương trình có biến toàn cục `int counter = 0;`
2. Hàm `thread_func`:
    - Vòng lặp 1.000.000 lần
    - Tăng `counter` lên 1 mỗi lần lặp (`counter++`)
3. `main`:
    - Tạo 2 threads chạy `thread_func`
    - Join 2 threads
    - In kết quả `counter` cuối cùng
4. Chạy nhiều lần và quan sát kết quả. Nếu kết quả KHÔNG phải 2.000.000 (khi counter đủ lớn), bạn đã thấy Race Condition.

### Phần 2: Fix với Mutex
1. Khai báo `pthread_mutex_t lock;` và khởi tạo nó
2. Sử dụng `pthread_mutex_lock(&lock)` trước khi tăng counter
3. Sử dụng `pthread_mutex_unlock(&lock)` sau khi tăng counter
4. Đảm bảo kết quả luôn là 2.000.000

## 🔧 Biên dịch và Chạy

```bash
make
./race_condition
```

## 💡 Gợi ý
- `pthread_mutex_init(&lock, NULL);`
- `pthread_mutex_destroy(&lock);`
