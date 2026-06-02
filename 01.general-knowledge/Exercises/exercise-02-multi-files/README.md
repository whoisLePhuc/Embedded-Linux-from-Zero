# Bài tập 02: Biên dịch nhiều file ⭐⭐

## 🎯 Mục tiêu

Tạo Makefile để biên dịch chương trình gồm nhiều file C (main.c, math_utils.c, math_utils.h).

## 📋 Yêu cầu

1. Chương trình tách thành nhiều file:
   - `main.c`: Chương trình chính
   - `math_utils.c`: Các hàm toán học
   - `math_utils.h`: Header file
   
2. Tạo `Makefile` với các tính năng:
   - Biên dịch từng file .c thành file .o riêng biệt
   - Link các file .o thành file thực thi
   - Target `clean` xóa cả file .o và file thực thi
   - Target `rebuild`: clean rồi build lại

## ✅ Tiêu chí đạt

- Makefile phải biên dịch riêng từng file .c ra file .o
- Khi sửa một file .c, chỉ file đó được biên dịch lại (incremental build)
- Sử dụng biến cho danh sách source files và object files
- Sử dụng pattern rules (%.o: %.c)

## 💡 Gợi ý

- Sử dụng `$(SRC:.c=.o)` để tự động tạo danh sách object files
- Pattern rule: `%.o: %.c` để biên dịch file .c thành .o
- Dependencies: file .o phụ thuộc vào file .c và .h tương ứng
- Automatic variables: `$@`, `$<`, `$^`

## 🔍 Kiểm tra

```bash
# Build lần đầu - sẽ compile tất cả files
make

# Sửa một file .c rồi chạy make lại
# Chỉ file đó được compile lại

# Rebuild toàn bộ
make rebuild

# Dọn dẹp
make clean
```

## 📁 File đã cho

- `main.c` - Chương trình chính
- `math_utils.c` - Implementation các hàm toán
- `math_utils.h` - Header file

## 📁 File cần tạo

- [ ] `Makefile` - File build script của bạn

---

Đây là bài tập quan trọng để hiểu cách Makefile xử lý dependencies!
