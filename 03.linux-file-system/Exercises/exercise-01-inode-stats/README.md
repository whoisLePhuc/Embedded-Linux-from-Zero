# Bài tập 01: Inode và File Stats ⭐

## 🎯 Mục tiêu

Tạo chương trình hiển thị thông tin chi tiết về file sử dụng system call `stat()`.

## 📋 Yêu cầu

Viết chương trình `file_info.c` để:

1. Định nghĩa đường dẫn file cần kiểm tra trong code (hard-coded)
2. Sử dụng `stat()` để lấy thông tin file
3. Hiển thị các thông tin sau:
   - **Inode number**
   - **File type** (regular file, directory, symbolic link, etc.)
   - **File size** (bytes)
   - **Number of hard links**
   - **Permissions** (dạng octal như 0644)
   - **Owner UID và GID**
   - **Timestamps**: Last access, Last modification, Last status change
   - **Number of blocks** (512B blocks)

## ✅ Tiêu chí đạt

- Chương trình chạy được: `./file_info` (file path được định nghĩa trong code)
- Hiển thị đầy đủ và đúng các thông tin yêu cầu
- Error handling tốt (file không tồn tại, không có quyền đọc...)
- Timestamps được format dễ đọc (không phải raw epoch)
- Permissions hiển thị dạng octal (ví dụ: 0644)

## 💡 Gợi ý

- Định nghĩa file path trong code:
  ```c
  // Thay đổi đường dẫn này để test các file khác nhau
  const char *filename = "test_data/sample.txt";
  ```

- Sử dụng `stat()` system call:
  ```c
  #include <sys/stat.h>
  struct stat sb;
  if (stat(filename, &sb) == -1) {
      perror("stat");
      return 1;
  }
  ```

- Kiểm tra file type bằng macros:
  - `S_ISREG(sb.st_mode)` - Regular file
  - `S_ISDIR(sb.st_mode)` - Directory
  - `S_ISLNK(sb.st_mode)` - Symbolic link

- Format permissions: `sb.st_mode & 0777`

- Format timestamp: sử dụng `ctime()` hoặc `strftime()`

## 🔍 Kiểm tra

```bash
# Build
make

# Test với file thường (mặc định: test_data/sample.txt)
./file_info

# Để test file/directory khác, sửa đổi biến filename trong file_info.c
# Ví dụ: const char *filename = "test_data/";
# Sau đó build lại và chạy

# So sánh với ls -li
ls -li test_data/sample.txt
```

## 📁 File cần tạo

- [ ] `file_info.c` - Source code chương trình
- [ ] `Makefile` - Build script

## 📁 File đã cho

- `test_data/sample.txt` - File mẫu để test

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-file-system.md`:
- Section 1.2: Inodes
- Section 2.1: System Call `stat()`

---

**Bonus**: Thử tạo hard link và kiểm tra xem inode number và link count có thay đổi không!

```bash
ln test_data/sample.txt test_data/hardlink.txt
./file_info test_data/sample.txt
./file_info test_data/hardlink.txt
# Inode number giống nhau, link count tăng lên!
```

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
