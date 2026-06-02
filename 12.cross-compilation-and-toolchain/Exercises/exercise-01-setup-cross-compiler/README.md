# Bài tập 01: Thiết lập Cross-Compiler ⭐

## 🎯 Mục tiêu

Làm quen với việc cài đặt và kiểm tra cross-compiler cho kiến trúc ARM trên Ubuntu.

## 📋 Yêu cầu

Viết script `setup_toolchain.sh` để:

1. Phát hiện OS đang chạy (Ubuntu, Debian, etc.)
2. Cài đặt `gcc-arm-linux-gnueabihf` và `binutils-arm-linux-gnueabihf`
3. Tạo file `test.c` với chương trình C đơn giản in ra "Hello from ARM!"
4. Biên dịch `test.c` bằng `arm-linux-gnueabihf-gcc`
5. Dùng `file` và `arm-linux-gnueabihf-readelf` để kiểm tra kiến trúc của file ELF

## ✅ Tiêu chí đạt

- Script chạy thành công và cài đặt được cross-compiler
- File `test.c` được tạo và biên dịch không lỗi
- Output của `file test` hiển thị `ARM` (không phải x86-64)
- Output của `readelf -h test` hiển thị `Machine: ARM`
- Script có error handling (kiểm tra quyền sudo, exit code, etc.)

## 💡 Gợi ý

- Kiểm tra OS bằng `/etc/os-release`:
  ```bash
  . /etc/os-release
  echo "$NAME $VERSION"
  ```

- Cài đặt cross-compiler:
  ```bash
  sudo apt update && sudo apt install -y gcc-arm-linux-gnueabihf binutils-arm-linux-gnueabihf
  ```

- Kiểm tra ELF architecture:
  ```bash
  file test
  arm-linux-gnueabihf-readelf -h test | grep -i machine
  ```

- Dùng `set -e` để script dừng nếu có lỗi

## 🔍 Kiểm tra

```bash
# Chạy script
chmod +x setup_toolchain.sh
./setup_toolchain.sh

# Kiểm tra thủ công nếu script thành công
file test
# Output mong đợi: test: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV)...

arm-linux-gnueabihf-readelf -h test | head -10
# Output mong đợi: Machine: ARM
```

## 📁 File cần tạo

- [ ] `setup_toolchain.sh` - Script cài đặt và kiểm tra toolchain

## 🎓 Kiến thức liên quan

Đọc lại trong `cross-compilation-and-toolchain.md`:
- Section 2.1: Ubuntu Pre-built Packages (toolchain setup)
- Section 2.4: Toolchain Verification
- Section 1.2: The Toolchain Triplet

---

**Bonus**: Mở rộng script để cài thêm `g++-arm-linux-gnueabihf`, tạo file C++ và kiểm tra `readelf -d` để xem dynamic sections.
