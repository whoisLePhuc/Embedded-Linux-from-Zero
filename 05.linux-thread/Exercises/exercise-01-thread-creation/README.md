# Exercise 01: Thread Creation

## 🎯 Mục tiêu
- Biết cách tạo thread mới sử dụng `pthread_create()`
- Biết cách đợi thread kết thúc sử dụng `pthread_join()`
- Hiểu sự thực thi song song của các thread

## 📝 Yêu cầu

Viết chương trình C thực hiện các bước sau:
1. Tạo một struct `ThreadInfo` chứa:
    - `thread_id`: số thứ tự của thread (không phải pthread_t)
    - `message`: chuỗi thông báo
2. Viết hàm `thread_func` nhận vào pointer của `ThreadInfo`:
    - In ra thông điệp: "Thread [ID]: Starting... [Message]"
    - Ngủ một khoảng thời gian ngẫu nhiên (1-3 giây) dùng `sleep()`
    - In ra thông điệp: "Thread [ID]: Finished"
3. Trong `main()`:
    - Tạo 3 threads, mỗi thread có ID và message riêng
    - Đợi tất cả 3 threads kết thúc
    - In ra "Main thread: All threads finished"

## 🔧 Biên dịch và Chạy

```bash
make
./thread_creation
```

## 💡 Gợi ý
- `pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg)`
- `pthread_join(pthread_t thread, void **retval)`
- Nhớ cast argument trong hàm `thread_func` về đúng kiểu struct
