# Exercise 02: Thread Arguments and Return Values

## 🎯 Mục tiêu
- Biết cách truyền struct dữ liệu vào thread
- Biết cách trả về dữ liệu từ thread
- Hiểu cách sử dụng `malloc` trên heap để trả về kết quả

## 📝 Yêu cầu

Viết chương trình C thực hiện các bước sau:
1. Tạo struct `Person`:
    - `name`: tên người (string)
    - `birth_year`: năm sinh (int)
2. Tạo hàm `calculate_age` làm thread function:
    - Nhận vào pointer của `Person`
    - Tính tuổi (giả sử năm hiện tại là 2025: `2025 - birth_year`)
    - In ra: "Thread: Hi [Name], calculate age..."
    - Cấp phát vùng nhớ cho kết quả (int) trên HEAP
    - Lưu tuổi vào vùng nhớ đó và trả về nó (return pointer hoặc pthread_exit)
3. Trong `main()`:
    - Tạo 2 struct Person với dữ liệu khác nhau
    - Tạo 2 threads tương ứng
    - Join từng thread và lấy kết quả trả về
    - In ra: "Main: [Name] is [Age] years old"
    - **Quan trọng**: Đừng quên `free` bộ nhớ kết quả sau khi dùng xong

## 🔧 Biên dịch và Chạy

```bash
make
./thread_args
```

## 💡 Gợi ý
- Để trả về giá trị từ thread, cần cấp phát động (malloc) vì biến local trong thread function sẽ bị hủy khi function kết thúc (stack destruction).
- `pthread_join(tid, (void **)&result_ptr);`
