# Bài tập 04: Makefile nâng cao ⭐⭐⭐

## 🎯 Mục tiêu

Tạo một Makefile hoàn chỉnh cho dự án có cấu trúc thư mục phức tạp, hỗ trợ nhiều tính năng nâng cao.

## 📋 Cấu trúc dự án

```
exercise-04-advanced/
├── src/          # Source files (.c)
├── include/      # Header files (.h)
├── build/        # Object files (.o)
├── bin/          # Executable files
└── Makefile
```

## 📋 Yêu cầu

1. **Cấu trúc thư mục**:
   - Compile file .c từ `src/` thành .o trong `build/`
   - Đặt executable trong `bin/`
   - Tự động tạo thư mục `build/` và `bin/` nếu chưa có

2. **Các targets bắt buộc**:
   - `all`: Build toàn bộ dự án
   - `clean`: Xóa các file build
   - `run`: Chạy chương trình
   - `debug`: Build với debug symbols (-g) và không optimize
   - `release`: Build với optimization (-O2) và không có debug
   - `test`: Chạy test cases (nếu có)

3. **Tính năng nâng cao**:
   - Sử dụng `VPATH` hoặc `vpath` để tìm source files
   - Tự động tạo dependencies (.d files)
   - Hỗ trợ biến môi trường (DEBUG=1 make)
   - Colored output cho dễ đọc

## ✅ Tiêu chí đạt

- Makefile phải tự động tạo thư mục build/ và bin/
- Object files phải được đặt trong build/, không lẫn với source
- Hỗ trợ incremental build (chỉ compile file thay đổi)
- Có thể build debug hoặc release version
- Clean phải xóa cả thư mục build/ và bin/

## 💡 Gợi ý nâng cao

1. **Tự động tạo thư mục**:
   ```makefile
   $(BUILD_DIR):
       mkdir -p $@
   ```

2. **Đưa .o vào build/**:
   ```makefile
   $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
       $(CC) $(CFLAGS) -c $< -o $@
   ```

3. **Sử dụng VPATH**:
   ```makefile
   VPATH = src:include
   ```

4. **Conditional compilation**:
   ```makefile
   ifdef DEBUG
       CFLAGS += -g -O0
   else
       CFLAGS += -O2
   endif
   ```

## 🔍 Kiểm tra

```bash
# Build thông thường
make

# Build version debug
make debug

# Build version release
make release

# Chạy
make run

# Dọn dẹp
make clean
```

## 📁 File đã cho

- `src/main.c` - Entry point
- `src/calculator.c` - Calculator functions
- `src/logger.c` - Logging utilities
- `include/calculator.h` - Calculator header
- `include/logger.h` - Logger header

## 📁 File cần tạo

- [ ] `Makefile` - File build script nâng cao của bạn

---

Đây là bài tập thử thách! Hoàn thành bài này, bạn đã thành thạo Makefile! 💪
