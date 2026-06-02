# Bài tập 03: Cross-Compile Static Library ⭐⭐

## 🎯 Mục tiêu

Tạo một static library (`.a`) với cross-compiler ARM và liên kết nó vào chương trình chính.

## 📋 Yêu cầu

Cho các file:
- `math_ops.h` — header với khai báo hàm (add, sub, mul, div)
- `math_ops.c` — implementation của các hàm
- `main.c` — chương trình chính gọi các hàm

Viết `Makefile` để:
1. Biên dịch `math_ops.c` thành `math_ops.o` dùng cross-compiler
2. Đóng gói `math_ops.o` thành `libmath.a` dùng `arm-linux-gnueabihf-ar`
3. Biên dịch `main.c` và liên kết với `libmath.a` thành `main`
4. Dùng `CROSS_COMPILE` prefix cho `CC` và `AR`

## ✅ Tiêu chí đạt

- `libmath.a` được tạo từ ARM object files (kiểm tra bằng `file`)
- `main` là ARM ELF, chạy được với QEMU
- `arm-linux-gnueabihf-nm libmath.a` hiển thị các symbols: add, sub, mul, div_op
- Makefile dùng `$(AR) rcs` để tạo archive
- Makefile có target `clean` xoá `.o`, `.a`, và binary

## 💡 Gợi ý

- Định nghĩa `AR` giống như `CC`:
  ```makefile
  AR = $(CROSS_COMPILE)ar
  ```

- Tạo static library:
  ```makefile
  libmath.a: math_ops.o
      $(AR) rcs $@ $^
  ```

- Liên kết:
  ```makefile
  main: main.c libmath.a
      $(CC) $(CFLAGS) -o $@ $< -L. -lmath
  ```

- Kiểm tra symbols:
  ```bash
  arm-linux-gnueabihf-nm libmath.a
  ```

## 🔍 Kiểm tra

```bash
# Build
make CROSS_COMPILE=arm-linux-gnueabihf-

# Kiểm tra library
file libmath.a
arm-linux-gnueabihf-nm libmath.a

# Kiểm tra binary
file main
arm-linux-gnueabihf-readelf -h main

# Chạy thử với QEMU
qemu-arm-static ./main
```

## 📁 File cần tạo

- [ ] `Makefile` - Build script tạo static library và binary

## 📁 File đã cho

- `math_ops.h` - Header chứa khai báo hàm
- `math_ops.c` - Implementation các hàm toán
- `main.c` - Chương trình chính

## 🎓 Kiến thức liên quan

Đọc lại trong `cross-compilation-and-toolchain.md`:
- Section 3.2: Static Library
- Section 4.1: Manual Makefile
- Section 1.2: Toolchain Triplet

---

**Bonus**: Thêm shared library variant: tạo `libmath.so` với `-shared -fPIC` và so sánh kích thước với static library.
