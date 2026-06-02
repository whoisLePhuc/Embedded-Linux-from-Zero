# Bài tập 06: Makefile Functions và Wildcards ⭐⭐⭐

## 🎯 Mục tiêu

Làm chủ các hàm và wildcards trong Makefile: `wildcard`, `patsubst`, `notdir`, `foreach`, `filter`, `basename` để tạo Makefile linh hoạt và tái sử dụng.

## 📋 Yêu cầu

Tạo Makefile cho dự án có cấu trúc thư mục, sử dụng các hàm Makefile để tự động tìm source files và xử lý biến.

### Cấu trúc thư mục:

```
exercise-06-makefile-functions/
├── README.md
├── src/
│   ├── main.c
│   ├── calculator.c
│   ├── logger.c
│   ├── sensor.c
│   └── display.c
├── include/
│   ├── calculator.h
│   ├── logger.h
│   ├── sensor.h
│   └── display.h
└── solution/
```

### Yêu cầu Makefile:

1. **Tự động tìm source files**:
   ```makefile
   SRC_DIR = src
   SRCS = $(wildcard $(SRC_DIR)/*.c)
   # SRCS sẽ tự động chứa: src/main.c src/calculator.c src/logger.c src/sensor.c src/display.c
   ```

2. **Chuyển đổi path tự động**:
   ```makefile
   # Từ SRCS, sinh ra:
   # - OBJS: src/main.o src/calculator.o ...
   OBJS = $(SRCS:.c=.o)
   
   # - Tên file không path:
   #   main.c calculator.c ...
   FILES = $(notdir $(SRCS))
   ```

3. **Thư mục build riêng** (build/, bin/):
   - Object files vào `build/`
   - Executable vào `bin/`

4. **Các targets**:
   - `all`: Build chương trình
   - `clean`: Xóa build và bin
   - `run`: Chạy chương trình
   - `list`: Hiển thị danh sách source files, object files
   - `stats`: Đếm số dòng code trong src/

## ✅ Tiêu chí đạt

- KHÔNG hard-code tên file nào trong Makefile
- Sử dụng `wildcard` để tìm source files
- Sử dụng `patsubst` hoặc substitution references để tạo object paths
- Makefile hoạt động khi thêm/bớt file .c trong src/ mà không cần sửa
- Có target `list` để debug danh sách file

## 💡 Gợi ý

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
TARGET = $(BIN_DIR)/app

# Tự động tìm tất cả .c files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Tạo tên .o files trong build/
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Hoặc dùng substitution reference
# OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Target list để debug
list:
	@echo "Source files: $(SRCS)"
	@echo "Object files: $(OBJS)"
	@echo "Source count: $(words $(SRCS))"

# Pattern rule cho build/%.o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
```

## 🔍 Kiểm tra

```bash
# Build
make clean all

# Xem danh sách file tự động
make list

# Thêm file mới để test
touch src/test_module.c
make list  # Sẽ thấy test_module.c tự động xuất hiện

# Xóa file test
rm src/test_module.c

# Đếm dòng code
make stats
```

## 📁 File cần tạo

- [ ] `Makefile` - Build script sử dụng functions và wildcards

## 📁 File đã cho

- `src/main.c`, `src/calculator.c`, `src/logger.c`, `src/sensor.c`, `src/display.c`
- `include/*.h` - Header files

## 🎓 Kiến thức liên quan

Đọc lại trong `general-knowledge.md`:
- Section 1.4: Variable Makefile
- Section 1.5: Automatic Variable

---

**Bonus 1:** Thêm `auto-dependencies` với `gcc -MM` để tự động sinh header dependencies.

**Bonus 2:** Sử dụng `$(foreach)` để tạo nhiều targets giống nhau cho debug/release profiles.
