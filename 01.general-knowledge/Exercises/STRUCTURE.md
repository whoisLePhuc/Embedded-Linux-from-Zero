# Cấu trúc thư mục Bài tập Makefile

```
Exercises/
├── README.md                          # Hướng dẫn chung về bài tập
│
├── exercise-01-basic/                 # ⭐ Bài tập cơ bản
│   ├── README.md                      # Hướng dẫn bài tập 01
│   ├── rectangle.c                    # Code mẫu
│   └── solution/
│       └── Makefile                   # Đáp án
│
├── exercise-02-multi-files/           # ⭐⭐ Nhiều file
│   ├── README.md                      # Hướng dẫn bài tập 02
│   ├── main.c                         # File chính
│   ├── math_utils.c                   # Implementation
│   ├── math_utils.h                   # Header
│   └── solution/
│       └── Makefile                   # Đáp án
│
├── exercise-03-library/               # ⭐⭐ Static library
│   ├── README.md                      # Hướng dẫn bài tập 03
│   ├── main.c                         # File chính
│   ├── string_utils.c                 # Library implementation
│   ├── string_utils.h                 # Library header
│   └── solution/
│       └── Makefile                   # Đáp án
│
└── exercise-04-advanced/              # ⭐⭐⭐ Nâng cao
    ├── README.md                      # Hướng dẫn bài tập 04
    ├── include/                       # Header files
    │   ├── calculator.h
    │   └── logger.h
    ├── src/                           # Source files
    │   ├── main.c
    │   ├── calculator.c
    │   └── logger.c
    └── solution/
        └── Makefile                   # Đáp án
```

## Tóm tắt các bài tập

### Bài 01: Makefile cơ bản ⭐
- **Mục tiêu**: Tạo Makefile đơn giản cho 1 file C
- **Học được**:
  - Cú pháp cơ bản của Makefile
  - Sử dụng biến (CC, CFLAGS, TARGET)
  - Tạo targets (all, clean, run)
  - Automatic variables ($@, $^)

### Bài 02: Biên dịch nhiều file ⭐⭐
- **Mục tiêu**: Build chương trình từ nhiều file .c
- **Học được**:
  - Pattern rules (%.o: %.c)
  - Quản lý dependencies
  - Incremental build
  - Object files (.o)
  - Variable substitution ($(SRC:.c=.o))

### Bài 03: Tạo Static Library ⭐⭐
- **Mục tiêu**: Tạo và sử dụng thư viện tĩnh (.a)
- **Học được**:
  - Lệnh `ar` để tạo library
  - Link với library (-L, -l)
  - Tách library ra khỏi main program
  - Target dependencies phức tạp hơn

### Bài 04: Makefile nâng cao ⭐⭐⭐
- **Mục tiêu**: Makefile cho dự án có cấu trúc thư mục
- **Học được**:
  - Tổ chức thư mục (src/, include/, build/, bin/)
  - VPATH để tìm source files
  - Tự động tạo thư mục
  - Conditional compilation (DEBUG/RELEASE)
  - Colored output
  - Nhiều targets nâng cao

## Cách làm bài tập

1. **Đọc README.md** trong mỗi thư mục bài tập
2. **Tự viết Makefile** của bạn (không nhìn solution)
3. **Test** bằng cách chạy `make`, `make run`, `make clean`
4. **So sánh** với solution nếu gặp khó khăn
5. **Hiểu nguyên lý** - đừng chỉ copy solution!

## Tips học tập

- Bắt đầu từ bài 01 và làm tuần tự
- Đọc kỹ error messages khi build bị lỗi
- Sử dụng `make -n` để xem lệnh sẽ chạy mà không thực sự chạy
- Sử dụng `make -B` để force rebuild toàn bộ
- Nhớ: Makefile dùng TAB, không phải SPACE!

## Kiểm tra solution

Tất cả solution đều có thể chạy từ thư mục `solution/`:

```bash
cd exercise-XX/solution
make clean
make
make run
```

Chúc bạn học tốt! 🚀
