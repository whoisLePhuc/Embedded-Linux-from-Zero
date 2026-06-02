# BÀI TẬP VỀ LINUX THREAD

Bộ bài tập này được thiết kế để giúp bạn làm quen với Linux Thread từ cơ bản đến nâng cao, sử dụng thư viện `pthread`.

## 📚 Cách sử dụng

Mỗi bài tập được đặt trong một thư mục riêng:
- `exercise-01-thread-creation/` - Tạo thread cơ bản ⭐
- `exercise-02-detach-cancel/` - Detach và Cancel thread ⭐⭐
- `exercise-03-thread-args-return/` - Truyền tham số và nhận giá trị trả về ⭐⭐
- `exercise-04-race-condition/` - Race Condition và Mutex ⭐⭐
- `exercise-05-conditional-variable/` - Đồng bộ với Conditional Variable ⭐⭐⭐
- `exercise-06-rwlock/` - Read-Write Lock ⭐⭐⭐
- `exercise-07-barrier/` - Barrier synchronization ⭐⭐⭐

## 🎯 Hướng dẫn

1. Đọc file `README.md` trong mỗi thư mục bài tập
2. Làm theo yêu cầu để hoàn thành bài tập
3. Biên dịch và chạy chương trình để kiểm tra kết quả
4. So sánh với file trong thư mục `solution/` nếu có (hoặc tự kiểm chứng kết quả)

## 📝 Mức độ

- ⭐ Dễ: Exercise 01
- ⭐⭐ Trung bình: Exercise 02, 03, 04
- ⭐⭐⭐ Khó: Exercise 05, 06, 07

## 💡 Tips

- Sử dụng flag `-pthread` khi biên dịch với gcc
- Sử dụng `man pthreads` để xem tài liệu chung
- Sử dụng `man pthread_create`, `man pthread_join`, v.v. để xem chi tiết từng hàm
- Luôn kiểm tra giá trị trả về của các hàm pthread để xử lý lỗi
- Cẩn thận với việc truy cập shared data (Race Condition)

Chúc bạn học vui! 🚀
