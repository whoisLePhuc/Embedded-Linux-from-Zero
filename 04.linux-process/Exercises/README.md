# BÀI TẬP VỀ LINUX PROCESS

Bộ bài tập này được thiết kế để giúp bạn làm quen với Linux Process từ cơ bản đến nâng cao.

## 📚 Cách sử dụng

Mỗi bài tập được đặt trong một thư mục riêng:
- `exercise-01-process-info/` - Thông tin cơ bản về process ⭐
- `exercise-02-memory-layout/` - Memory layout của process ⭐⭐
- `exercise-03-fork-exec/` - Tạo process với fork() và exec() ⭐⭐
- `exercise-04-process-management/` - Quản lý process, zombie và orphan ⭐⭐⭐
- `exercise-05-process-groups/` - Process groups và sessions ⭐⭐⭐
- `exercise-06-daemon-process/` - Daemon process ⭐⭐⭐
- `exercise-07-resource-usage/` - Đo lường tài nguyên với getrusage ⭐⭐⭐

## 🎯 Hướng dẫn

1. Đọc file `README.md` trong mỗi thư mục bài tập
2. Làm theo yêu cầu để hoàn thành bài tập
3. Biên dịch và chạy chương trình để kiểm tra kết quả
4. So sánh với file trong thư mục `solution/` nếu cần gợi ý

## 📝 Mức độ

- ⭐ Dễ: Exercise 01
- ⭐⭐ Trung bình: Exercise 02, 03
- ⭐⭐⭐ Khó: Exercise 04, 05, 06, 07

## 💡 Tips

- Đọc kỹ các section trong `linux-process.md`
- Sử dụng `man 2 <syscall>` để xem chi tiết system calls
- Sử dụng `ps aux` và `pstree` để quan sát processes
- Chú ý error handling và cleanup (wait for children!)
- Tránh tạo zombie processes

Chúc bạn học vui! 🚀
