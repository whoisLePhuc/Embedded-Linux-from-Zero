# Bài tập 03: Tạo và sử dụng Static Library ⭐⭐

## 🎯 Mục tiêu

Tạo một static library (.a) chứa các hàm thao tác với chuỗi, sau đó link library này vào chương trình chính.

## 📋 Yêu cầu

1. Tạo static library `libstring.a` từ `string_utils.c`:
   - Hàm `str_length()`: Tính độ dài chuỗi
   - Hàm `str_reverse()`: Đảo ngược chuỗi
   - Hàm `str_upper()`: Chuyển sang chữ hoa

2. Tạo `Makefile` với các targets:
   - `library`: Tạo static library `libstring.a`
   - `all`: Build chương trình chính và link với library
   - `clean`: Xóa tất cả file build
   - `run`: Chạy chương trình

## ✅ Tiêu chí đạt

- Sử dụng lệnh `ar` để tạo static library
- File library có đuôi `.a`
- Chương trình main link với library bằng `-L` và `-l`
- Makefile phải cho phép rebuild library độc lập

## 💡 Gợi ý

- Tạo library: `ar rcs libstring.a string_utils.o`
- Link library: `gcc -o program main.o -L. -lstring`
  - `-L.`: Tìm library trong thư mục hiện tại
  - `-lstring`: Link với libstring.a (bỏ 'lib' và '.a')
- Library phụ thuộc vào object file

## 🔍 Kiểm tra

```bash
# Tạo library
make library

# Kiểm tra library đã tạo
ar -t libstring.a

# Build chương trình
make

# Chạy
make run

# Dọn dẹp
make clean
```

## 📁 File đã cho

- `string_utils.h` - Header file
- `string_utils.c` - Implementation các hàm
- `main.c` - Chương trình chính

## 📁 File cần tạo

- [ ] `Makefile` - File build script của bạn

---

Sau bài này, bạn sẽ hiểu cách tạo và sử dụng static library trong C!
