# BÀI TẬP VỀ IPC SIGNAL

Bộ bài tập này được thiết kế để giúp bạn làm quen với Signal trong Linux, từ việc bắt signal cơ bản đến các kỹ thuật nâng cao như blocking và sử dụng sigaction.

## 📚 Cách sử dụng

Mỗi bài tập được đặt trong một thư mục riêng:
- `exercise-01-sigint/` - Bắt và xử lý Signal SIGINT (Ctrl+C)
- `exercise-02-sigprocmask/` - Blocking và Unblocking Signal để bảo vệ Critical Section
- `exercise-03-sigaction/` - Sử dụng sigaction để xử lý signal nâng cao (lấy thông tin người gửi)

## 🎯 Hướng dẫn

1. Đọc file `README.md` trong mỗi thư mục bài tập
2. Làm theo yêu cầu để hoàn thành bài tập
3. Biên dịch và chạy chương trình để kiểm tra kết quả
4. So sánh với file trong thư mục `solution/` nếu có (hoặc tự kiểm chứng kết quả)

## 📝 Mức độ

- ⭐ Dễ: Exercise 01
- ⭐⭐ Trung bình: Exercise 02
- ⭐⭐⭐ Khó: Exercise 03

## 💡 Tips

- Sử dụng `man signal` hoặc `man 2 signal` để xem tài liệu về `signal()`
- Sử dụng `man sigprocmask` để xem về blocking signal
- Sử dụng `man 2 kill` để xem cách gửi signal
- Luôn kiểm tra giá trị trả về của các system call
- `Ctrl+C` gửi `SIGINT`, `Ctrl+\` gửi `SIGQUIT`

Chúc bạn học vui! 🚀
