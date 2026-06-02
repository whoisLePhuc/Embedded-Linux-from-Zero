# Bài tập 02: Cross-Compile Hello World ⭐⭐

## 🎯 Mục tiêu

Sử dụng biến môi trường `CROSS_COMPILE` để cross-compile chương trình C đơn giản cho ARM.

## 📋 Yêu cầu

1. Đọc file `hello.c` đã cho — nó in ra thông tin architecture (ARM, x86_64, etc.)
2. Viết `Makefile` sử dụng biến `CROSS_COMPILE` để:
   - Định nghĩa `CC = $(CROSS_COMPILE)gcc`
   - Biên dịch hello.c thành hello (ARM binary)
3. Chạy `make CROSS_COMPILE=arm-linux-gnueabihf-` để build

## ✅ Tiêu chí đạt

- Makefile dùng `?=` để cho phép ghi đè `CROSS_COMPILE`
- Khi build với `make CROSS_COMPILE=arm-linux-gnueabihf-`, output là ARM ELF
- Khi build với `make` (không có CROSS_COMPILE), output là native x86-64
- `file hello` hiển thị đúng kiến trúc tương ứng
- Makefile có target `clean`

## 💡 Gợi ý

- Sử dụng `CROSS_COMPILE ?= ` để mặc định là rỗng (native compile)
- CC luôn là `$(CROSS_COMPILE)gcc`

  ```makefile
  CROSS_COMPILE ?=
  CC = $(CROSS_COMPILE)gcc
  ```

- So sánh output:
  ```bash
  # ARM cross-compile
  make CROSS_COMPILE=arm-linux-gnueabihf- clean all
  file hello
  # hello: ELF 32-bit LSB ARM

  # Native compile
  make clean all
  file hello
  # hello: ELF 64-bit LSB x86-64
  ```

## 🔍 Kiểm tra

```bash
# Cross-compile
make CROSS_COMPILE=arm-linux-gnueabihf- all
file hello
arm-linux-gnueabihf-readelf -h hello

# Chạy thử với QEMU (nếu đã cài)
qemu-arm-static ./hello

# Native compile
make clean
make
./hello
```

## 📁 File cần tạo

- [ ] `Makefile` - Build script với CROSS_COMPILE variable

## 📁 File đã cho

- `hello.c` - Source code in architecture info

## 🎓 Kiến thức liên quan

Đọc lại trong `cross-compilation-and-toolchain.md`:
- Section 3.1: Hello World (cross-compilation)
- Section 4.1: Manual Makefile
- Section 1.2: Toolchain Triplet

---

**Bonus**: Thêm target `run-qemu` vào Makefile để tự động chạy ARM binary với `qemu-arm-static`.
