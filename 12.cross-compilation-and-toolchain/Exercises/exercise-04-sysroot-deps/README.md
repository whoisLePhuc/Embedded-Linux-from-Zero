# Bài tập 04: Sysroot và External Dependencies ⭐⭐⭐

## 🎯 Mục tiêu

Hiểu khái niệm sysroot trong cross-compilation và cách biên dịch chương trình phụ thuộc vào thư viện bên ngoài (libm, libpthread).

## 📋 Yêu cầu

Cho file `main.c` sử dụng:
- `<math.h>` — các hàm `sqrt()`, `sin()`, `cos()`
- `<pthread.h>` — tạo thread để tính toán

Thực hiện:

1. Tìm sysroot path của cross-compiler bằng `arm-linux-gnueabihf-gcc -print-sysroot`
2. Tìm file header `math.h` và `pthread.h` trong sysroot
3. Biên dịch `main.c` với cross-compiler và liên kết thư viện `-lm -lpthread`
4. Kiểm tra dependencies với `arm-linux-gnueabihf-readelf -d`

## ✅ Tiêu chí đạt

- Biên dịch thành công với `-lm -lpthread`
- `readelf -d` hiển thị các NEEDED entries: `libm.so.6`, `libpthread.so.0`, `libc.so.6`
- Output là ARM ELF 32-bit
- Có thể chạy với `qemu-arm-static` (có thể cần `QEMU_LD_PREFIX`)

## 💡 Gợi ý

- Tìm sysroot:
  ```bash
  arm-linux-gnueabihf-gcc -print-sysroot
  # Thường là /usr/arm-linux-gnueabihf
  ```

- Kiểm tra header location:
  ```bash
  ls /usr/arm-linux-gnueabihf/usr/include/math.h
  ```

- Biên dịch:
  ```bash
  arm-linux-gnueabihf-gcc -Wall -O2 -o math_app main.c -lm -lpthread
  ```

- Dùng `--sysroot` để chỉ định sysroot tùy chỉnh (nếu cần):
  ```bash
  arm-linux-gnueabihf-gcc --sysroot=$(arm-linux-gnueabihf-gcc -print-sysroot) -o math_app main.c -lm
  ```

- Kiểm tra dependencies:
  ```bash
  arm-linux-gnueabihf-readelf -d math_app | grep NEEDED
  ```

- Chạy với QEMU:
  ```bash
  QEMU_LD_PREFIX=/usr/arm-linux-gnueabihf qemu-arm-static ./math_app 100
  ```

## 🔍 Kiểm tra

```bash
# Cross-compile
arm-linux-gnueabihf-gcc -Wall -O2 -o math_app main.c -lm -lpthread

# Kiểm tra architecture
file math_app

# Kiểm tra linked libraries
arm-linux-gnueabihf-readelf -d math_app

# Chạy với QEMU
QEMU_LD_PREFIX=/usr/arm-linux-gnueabihf qemu-arm-static ./math_app 100
```

## 📁 File cần tạo

- [ ] `math_app.c` (hoặc sửa `main.c`) — Chương trình C sử dụng math.h và pthread
- [ ] Lệnh biên dịch với `-lm -lpthread` và `--sysroot`

## 📁 File đã cho

- `main.c` - Source code sử dụng math.h và pthread

## 🎓 Kiến thức liên quan

Đọc lại trong `cross-compilation-and-toolchain.md`:
- Section 3.4: Sysroot Concept
- Section 3.1: Hello World (basic cross-compile)
- Section 1.2: Toolchain Triplet

---

**Bonus**: Viết script `verify_sysroot.sh` tự động tìm sysroot, kiểm tra header, biên dịch, và hiển thị dependencies. So sánh output khi dùng `--sysroot` khác nhau.
