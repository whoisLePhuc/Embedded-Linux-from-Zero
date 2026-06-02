# Exercise 2: POSIX Shared Memory

## 🎯 Mục tiêu
- Hiểu cách sử dụng Shared Memory theo chuẩn POSIX (`shm_open`, `mmap`).
- So sánh sự khác biệt với System V (dùng tên file ảo `/name`, file descriptor).

## 📝 Yêu cầu

Viết 2 chương trình: `writer.c` và `reader.c`.

### 1. Common
- Define tên Shared Memory Object: `"/test_shm"`.
- Define kích thước: 4096 bytes (SIZE).

### 2. Writer
- Tạo Shared Memory object với `shm_open` (`O_CREAT | O_RDWR`).
- Set kích thước file bằng `ftruncate`.
- Map vào bộ nhớ bằng `mmap` (`PROT_READ | PROT_WRITE`, `MAP_SHARED`).
- Ghi một chuỗi dữ liệu vào vùng nhớ map đó.
- Unmap (`munmap`) và đóng file descriptor (`close`).

### 3. Reader
- Mở Shared Memory object với `shm_open` (`O_RDONLY`).
- Map vào bộ nhớ bằng `mmap` (`PROT_READ`, `MAP_SHARED`).
- Đọc dữ liệu và in ra.
- Unmap, đóng file descriptor.
- Xóa object bằng `shm_unlink`.

## 🧪 Hướng dẫn kiểm tra

1. Link với thư viện `-lrt`.
   ```bash
   make writer
   ./writer
   ```
2. Mở terminal khác:
   ```bash
   make reader
   ./reader
   ```

## 💡 Gợi ý

- `int fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);`
- `ftruncate(fd, SIZE);`
- `void *ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);`
- `sprintf(ptr, "%s", "Hello Shared Memory");`
- `shm_unlink(NAME);`
