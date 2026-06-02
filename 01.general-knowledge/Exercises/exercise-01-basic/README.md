# Bài tập 01: Makefile cơ bản ⭐

## 🎯 Mục tiêu

Tạo một Makefile đơn giản để biên dịch chương trình tính toán diện tích hình chữ nhật.

## 📋 Yêu cầu

1. Có file `rectangle.c` chứa chương trình tính diện tích hình chữ nhật
2. Tạo file `Makefile` với các targets sau:
   - `all`: Build chương trình (target mặc định)
   - `clean`: Xóa các file build
   - `run`: Build và chạy chương trình

## ✅ Tiêu chí đạt

- Chạy `make` để tạo file thực thi `rectangle`
- Chạy `make run` để build và chạy chương trình
- Chạy `make clean` để xóa file thực thi
- Makefile phải sử dụng biến `CC` và `CFLAGS`

## 💡 Gợi ý

- Sử dụng biến `TARGET` để đặt tên file thực thi
- Sử dụng `$@` để tham chiếu đến target hiện tại
- Nhớ rằng mỗi dòng lệnh trong target phải bắt đầu bằng TAB!

## 🔍 Kiểm tra

```bash
# Build chương trình
make

# Chạy chương trình
make run

# Dọn dẹp
make clean
```

## 📁 File cần tạo

- [ ] `Makefile` - File build script của bạn

File `rectangle.c` đã được cung cấp sẵn.

---

Sau khi hoàn thành, hãy so sánh với `solution/Makefile` để xem cách làm tham khảo!
