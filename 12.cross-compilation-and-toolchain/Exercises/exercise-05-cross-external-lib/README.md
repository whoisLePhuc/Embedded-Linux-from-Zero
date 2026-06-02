# Bài tập 05: CMake Cross Toolchain File ⭐⭐⭐

## 🎯 Mục tiêu

Sử dụng CMake với toolchain file để cross-compile cho ARM, thay vì dùng Makefile thủ công.

## 📋 Yêu cầu

Cho file `CMakeLists.txt` và `main.c`:

1. Tạo file `arm-cross.cmake` — toolchain file cho CMake, chỉ định:
   - `CMAKE_SYSTEM_NAME` = Linux
   - `CMAKE_SYSTEM_PROCESSOR` = arm
   - `CMAKE_C_COMPILER` = arm-linux-gnueabihf-gcc
   - `CMAKE_CXX_COMPILER` = arm-linux-gnueabihf-g++
   - `CMAKE_SYSROOT` = sysroot path
   - `CMAKE_FIND_ROOT_PATH` policies
   - Architecture flags (`-march=armv7-a -mfpu=vfpv3`)

2. Build với CMake:
   ```bash
   mkdir build && cd build
   cmake -DCMAKE_TOOLCHAIN_FILE=../arm-cross.cmake ..
   make
   ```

## ✅ Tiêu chí đạt

- `arm-cross.cmake` có đầy đủ các biến CMAKE cần thiết cho cross-compilation
- Build ra ARM ELF binary (kiểm tra với `file`)
- `CMAKE_FIND_ROOT_PATH_MODE` được setup đúng:
  - `PROGRAM`: NEVER (tìm trên host)
  - `LIBRARY`: ONLY (tìm trong sysroot)
  - `INCLUDE`: ONLY (tìm trong sysroot)
- Có thể build native bằng cách bỏ qua toolchain file

## 💡 Gợi ý

- Toolchain file template:
  ```cmake
  set(CMAKE_SYSTEM_NAME Linux)
  set(CMAKE_SYSTEM_PROCESSOR arm)
  set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
  set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
  set(CMAKE_SYSROOT /usr/arm-linux-gnueabihf)
  set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})

  set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
  set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
  set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

  set(CMAKE_C_FLAGS_INIT "-march=armv7-a -mfpu=vfpv3")
  ```

- Build commands:
  ```bash
  # Cross-compile
  mkdir -p build && cd build
  cmake -DCMAKE_TOOLCHAIN_FILE=../arm-cross.cmake ..
  make
  file hello-arm

  # Native compile (so sánh)
  cd .. && mkdir -p build-native && cd build-native
  cmake ..
  make
  file hello-arm
  ```

## 🔍 Kiểm tra

```bash
# Cross-compile với toolchain
mkdir -p build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../arm-cross.cmake ..
make
file hello-arm
# Output mong đợi: ELF 32-bit LSB ARM

# Chạy với QEMU
qemu-arm-static ./hello-arm
```

## 📁 File cần tạo

- [ ] `arm-cross.cmake` - CMake toolchain file

## 📁 File đã cho

- `CMakeLists.txt` - CMake project (hoàn chỉnh)
- `main.c` - Source code

## 🎓 Kiến thức liên quan

Đọc lại trong `cross-compilation-and-toolchain.md`:
- Section 4.3: CMake Toolchain File
- Section 3.4: Sysroot Concept

---

**Bonus**: Thêm `find_package(mbedtls)` vào CMakeLists.txt, cài `libmbedtls-dev:armhf` cho target, và verify rằng CMake tìm đúng library trong sysroot nhờ toolchain file.
