# BÀI TẬP VỀ LINUX FILE SYSTEM

Bộ bài tập này được thiết kế để giúp bạn làm quen với Linux File System từ cơ bản đến nâng cao.

## 📚 Cách sử dụng

Mỗi bài tập được đặt trong một thư mục riêng:
- `exercise-01-inode-stats/` - Inode và file stats ⭐
- `exercise-02-file-operations/` - System calls và thao tác file ⭐⭐
- `exercise-03-file-monitoring/` - File monitoring và page cache ⭐⭐
- `exercise-04-io-redirection/` - I/O Redirection với dup2() ⭐⭐
- `exercise-05-directory-operations/` - Directory traversal với opendir/readdir ⭐⭐
- `exercise-06-file-locking/` - File locking và concurrent access ⭐⭐⭐
- `exercise-07-mmap-file/` - Memory-mapped files với mmap ⭐⭐⭐

## 🎯 Hướng dẫn

1. Đọc file `README.md` trong mỗi thư mục bài tập
2. Làm theo yêu cầu để hoàn thành bài tập
3. Biên dịch và chạy chương trình để kiểm tra kết quả
4. So sánh với file trong thư mục `solution/` nếu cần gợi ý

## 📝 Mức độ

- ⭐ Dễ: Exercise 01
- ⭐⭐ Trung bình: Exercise 02, 03, 04, 05
- ⭐⭐⭐ Khó: Exercise 06, 07

## 💡 Tips

- Đọc kỹ các section trong `linux-file-system.md`
- Sử dụng `man 2 <syscall>` để xem chi tiết system calls
- Test với nhiều trường hợp khác nhau (file lớn, nhỏ, không tồn tại...)
- Chú ý error handling để chương trình robust
- Nhớ `close()` file descriptor sau khi dùng xong!

Chúc bạn học vui! 🚀
