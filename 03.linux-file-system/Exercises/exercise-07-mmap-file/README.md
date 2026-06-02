# Bài tập 06: Memory-Mapped Files với mmap ⭐⭐

## 🎯 Mục tiêu

Hiểu và thực hành memory-mapped file I/O sử dụng `mmap()`, `munmap()`, `msync()` để truy cập file thông qua bộ nhớ ảo.

## 📋 Yêu cầu

Viết chương trình `main.c` để:

1. **Mở file và lấy kích thước**: Mở file với `O_RDWR`, dùng `fstat()` để lấy kích thước file

2. **Map file vào bộ nhớ**: Dùng `mmap()` với `PROT_READ | PROT_WRITE` và `MAP_SHARED`

3. **Truy cập nội dung qua con trỏ**: Đọc và in nội dung file bằng cách truy cập vùng nhớ đã map như một char array

4. **Chỉnh sửa nội dung**: Chuyển đổi toàn bộ nội dung file thành chữ IN HOA (uppercase)

5. **Ghi thay đổi về file**: Dùng `msync()` để đảm bảo thay đổi được ghi từ memory xuống disk

6. **Unmap và đóng file**: Dùng `munmap()` và `close()`

## ✅ Tiêu chí đạt

- Chương trình chạy được: `./main` (file path hard-coded trong code)
- Đọc được nội dung file và in ra màn hình trước khi chỉnh sửa
- Chuyển đổi thành công toàn bộ chữ thường thành chữ IN HOA
- Ghi thay đổi thành công xuống file và có thể kiểm tra lại
- Error handling đầy đủ cho mọi system call
- Không memory leak (unmap và close đúng cách)

## 💡 Gợi ý

- **Lấy kích thước file:**
  ```c
  struct stat sb;
  if (fstat(fd, &sb) == -1) {
      perror("fstat");
      return 1;
  }
  size_t file_size = sb.st_size;
  ```

- **mmap() - Map file vào memory:**
  ```c
  #include <sys/mman.h>
  
  char *mapped = mmap(NULL, file_size, PROT_READ | PROT_WRITE,
                      MAP_SHARED, fd, 0);
  if (mapped == MAP_FAILED) {
      perror("mmap");
      return 1;
  }
  ```
  Sau lệnh này, `mapped` là một con trỏ tới vùng nhớ chứa nội dung file.

- **Truy cập nội dung file:**
  ```c
  // In nội dung
  printf("Content: %.*s\n", (int)file_size, mapped);
  
  // Chỉnh sửa
  for (size_t i = 0; i < file_size; i++) {
      if (mapped[i] >= 'a' && mapped[i] <= 'z')
          mapped[i] -= 32;  // to uppercase
  }
  ```

- **msync() - Ghi thay đổi về disk:**
  ```c
  if (msync(mapped, file_size, MS_SYNC) == -1) {
      perror("msync");
  }
  ```

- **munmap() - Giải phóng mapping:**
  ```c
  if (munmap(mapped, file_size) == -1) {
      perror("munmap");
  }
  ```

- **Các flags quan trọng:**
  - `PROT_READ`: Cho phép đọc từ mapped memory
  - `PROT_WRITE`: Cho phép ghi vào mapped memory
  - `MAP_SHARED`: Thay đổi được ghi lại xuống file
  - `MAP_PRIVATE`: Thay đổi chỉ trong memory (copy-on-write)

## 🔍 Kiểm tra

```bash
# Build
make

# Tạo file test
echo "Hello World! This is a test file." > test_data.txt

# Sửa hard-coded path trong main.c thành "test_data.txt"
# Run
./main

# Kiểm tra nội dung đã được chuyển thành uppercase
cat test_data.txt
# Output: HELLO WORLD! THIS IS A TEST FILE.
```

## 📁 File cần tạo

- [ ] `main.c` - Source code chương trình
- [ ] `Makefile` - Build script

## 📁 File đã cho

- `solution/` - Thư mục chứa solution tham khảo

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-file-system.md`:
- Section 3.1: Page Cache
- Section 3.2: I-node Table
- Section 3.3: Open File Table

Tìm hiểu thêm:
- Memory-mapped files vs read/write system calls performance
- Demand paging và page faults
- MAP_SHARED vs MAP_PRIVATE

## 📝 Bonus Challenges

1. **Performance comparison**: So sánh tốc độ đọc file lớn (100MB+) giữa mmap và read()
2. **Random access**: Tìm kiếm và thay thế chuỗi trong file bằng mmap (tận dụng pointer arithmetic)
3. **MAP_PRIVATE**: Dùng MAP_PRIVATE để đọc file mà không sợ làm hỏng file gốc
4. **Append to file**: Mở rộng file sau khi mmap bằng ftruncate() và mmap lại với kích thước mới

---

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
