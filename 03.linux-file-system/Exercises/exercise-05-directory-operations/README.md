# Bài tập 07: Directory Traversal và Operations ⭐⭐

## 🎯 Mục tiêu

Thực hành thao tác với thư mục trong Linux sử dụng `opendir()`, `readdir()`, `closedir()` và kết hợp với `stat()` để duyệt cây thư mục đệ quy.

## 📋 Yêu cầu

Viết chương trình `main.c` để:

1. **Liệt kê nội dung thư mục**: Dùng `opendir()` và `readdir()` để đọc tất cả entries trong một thư mục

2. **Hiển thị thông tin chi tiết**: Với mỗi entry, hiển thị:
   - Tên file
   - Loại file (regular file, directory, symlink, ...)
   - Kích thước (bytes)
   - Permissions (dạng octal)

3. **Duyệt thư mục đệ quy**: Nếu entry là thư mục (và không phải `.` hoặc `..`), tiếp tục duyệt vào bên trong với indent tăng dần

4. **Tính tổng kích thước**: Tính tổng kích thước tất cả regular files trong toàn bộ cây thư mục

## ✅ Tiêu chí đạt

- Chương trình chạy được: `./main [directory_path]` (mặc định là thư mục hiện tại nếu không có argument)
- Hiển thị cấu trúc cây thư mục rõ ràng với indent
- Duyệt đệ quy vào tất cả thư mục con (trừ `.` và `..`)
- Hiển thị đúng loại file và kích thước
- Tính và hiển thị tổng kích thước tất cả files
- Error handling đầy đủ
- Không memory leak (đóng hết directories)

## 💡 Gợi ý

- **Mở và đọc thư mục:**
  ```c
  #include <dirent.h>
  
  DIR *dir = opendir(path);
  if (dir == NULL) {
      perror("opendir");
      return;
  }
  
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
      printf("%s\n", entry->d_name);
  }
  
  closedir(dir);
  ```

- **Bỏ qua `.` và `..`:**
  ```c
  if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;
  ```

- **Lấy thông tin chi tiết với stat():**
  ```c
  char full_path[1024];
  snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
  
  struct stat sb;
  if (stat(full_path, &sb) == -1) {
      perror("stat");
      continue;
  }
  
  // Kiểm tra loại file
  if (S_ISDIR(sb.st_mode))
      printf(" (directory)\n");
  else if (S_ISREG(sb.st_mode))
      printf(" (file, %ld bytes)\n", sb.st_size);
  ```

- **Duyệt đệ quy (dùng snprintf để tạo đường dẫn đầy đủ):**

  ```c
  void traverse(const char *base_path, int indent) {
      // In indent
      for (int i = 0; i < indent; i++)
          printf("  ");
      
      // Mở thư mục, đọc entries, ...
      // Nếu entry là thư mục:
      //   traverse(full_path, indent + 1);
  }
  ```

## 🔍 Kiểm tra

```bash
# Build
make

# Test với thư mục hiện tại
./main
# Output: cây thư mục từ vị trí hiện tại

# Test với thư mục cụ thể
./main /tmp
# Output: cây thư mục của /tmp

# Test với thư mục chứa nhiều cấp
./main ..
# Output: cây thư mục của thư mục cha
```

## 📁 File cần tạo

- [ ] `main.c` - Source code chương trình
- [ ] `Makefile` - Build script

## 📁 File đã cho

- `solution/` - Thư mục chứa solution tham khảo

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-file-system.md`:
- Section 1.1.4: Directory và Inode
- Section 1.1.5: File Types

## 📝 Bonus Challenges

1. **Filter by type**: Thêm option `-f` (only files), `-d` (only directories), `-l` (only symlinks)
2. **Sort by name**: Sắp xếp entries theo thứ tự alphabet, directories lên trước
3. **Human-readable size**: Hiển thị kích thước dạng K, M, G thay vì bytes
4. **Max depth**: Thêm option `--max-depth N` để giới hạn độ sâu duyệt
5. **Follow symlinks**: Dùng `stat()` (follow symlinks) và `lstat()` (không follow) để so sánh

---

Đây là bài tập cuối cùng của Module 02! Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
