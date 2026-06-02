# Bài tập 02: File Operations với System Calls ⭐⭐

## 🎯 Mục tiêu

Tạo chương trình copy file sử dụng system calls (`open`, `read`, `write`, `close`) thay vì C standard library.

## 📋 Yêu cầu

Viết chương trình `file_copy.c` để:

1. Định nghĩa source và destination file paths trong code (hard-coded)

2. Sử dụng system calls để:
   - Mở source file để đọc
   - Tạo destination file để ghi
   - Copy dữ liệu từ source sang destination
   - Đóng cả hai files

3. Yêu cầu kỹ thuật:
   - Sử dụng buffer size 4096 bytes
   - Destination file phải có permissions 0644
   - Xử lý lỗi đầy đủ (file không tồn tại, không có quyền...)
   - Hiển thị số bytes đã copy

## ✅ Tiêu chí đạt

- Chương trình chạy được: `./file_copy` (file paths được định nghĩa trong code)
- Copy đúng và đầy đủ nội dung file
- File mới có permissions 0644
- Error handling tốt cho tất cả system calls
- Không memory leak (đóng hết file descriptors)
- Hiển thị thông báo kết quả (số bytes copied)

## 💡 Gợi ý

- **Định nghĩa file paths trong code:**
  ```c
  // Thay đổi các đường dẫn này để test
  const char *src = "test_data/input.txt";
  const char *dst = "output.txt";
  ```

- **Mở file để đọc:**
  ```c
  int src_fd = open(src, O_RDONLY);
  if (src_fd == -1) {
      perror("open source");
      return 1;
  }
  ```

- **Tạo file mới để ghi:**
  ```c
  int dst_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  ```

- **Đọc và ghi data:**
  ```c
  char buffer[4096];
  ssize_t bytes_read;
  while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
      write(dst_fd, buffer, bytes_read);
  }
  ```

- **Nhớ đóng files:**
  ```c
  close(src_fd);
  close(dst_fd);
  ```

## 🔍 Kiểm tra

```bash
# Build
make

# Test copy file (mặc định: copy test_data/input.txt -> output.txt)
./file_copy

# Kiểm tra nội dung giống nhau
diff test_data/input.txt output.txt

# Kiểm tra permissions
ls -l output.txt
# Phải là: -rw-r--r--

# Để test file khác, sửa đổi biến src và dst trong file_copy.c
# Sau đó build lại và chạy
```

## 📁 File cần tạo

- [ ] `file_copy.c` - Source code chương trình
- [ ] `Makefile` - Build script

## 📁 File đã cho

- `test_data/input.txt` - File mẫu để test

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-file-system.md`:
- Section 2.1: System Calls (`open`, `read`, `write`, `close`)
- Section 3.4: File Descriptor Table

## 📝 Bonus Challenges

1. **Progress indicator**: Hiển thị % progress khi copy file lớn
2. **Preserve permissions**: Copy cả permissions từ source file
3. **Compare performance**: So sánh tốc độ với buffer size khác nhau (512, 4096, 65536)

---

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
