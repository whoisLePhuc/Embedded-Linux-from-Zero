# Embedded Linux - General Knowledge

> Tài liệu học Embedded Linux: Makefile, Compilation Process, Libraries và Demo thực tế

---

## 📑 Mục Lục

### [1. Introduction to MakeFile](#1-introduction-to-makefile)
- [1.1. What is Makefile](#11-what-is-makefile)
- [1.2. Makefile Structure](#12-makefile-structure)
- [1.3. How to run make](#13-how-to-run-make)
- [1.4. Variable Makefile](#14-variable-makefile)
- [1.5. Automatic Variable](#15-automatic-variable)

### [2. The Process of Compiling C Program](#2-the-process-of-compiling-c-program)
- [2.1. Preprocessing](#21-preprocessing)
- [2.2. Compilation](#22-compilation)
- [2.3. Assembling](#23-assembling)
- [2.4. Linking](#24-linking)

### [3. Static and Shared Library in Linux](#3-static-and-shared-library-in-linux)
- [3.1. Static Library](#31-static-library)
- [3.2. Shared Library](#32-shared-library)

### [4. Demo create shared library by using Makefile](#4-demo-create-shared-library-by-using-makefile)

---

# 1. Introduction to MakeFile

## 1.1. What is Makefile

**Makefile** là một file đặc biệt được sử dụng bởi công cụ `make` để tự động hóa quá trình biên dịch (compilation) và quản lý dependencies của các dự án phần mềm. Makefile giúp:

- **Tự động hóa quá trình build**: Không cần gõ lại các lệnh biên dịch phức tạp
- **Quản lý dependencies**: Chỉ biên dịch lại những file đã thay đổi
- **Tăng hiệu suất**: Tiết kiệm thời gian bằng cách build incremental
- **Chuẩn hóa quy trình**: Đảm bảo mọi người build code theo cùng một cách

**Ứng dụng trong Embedded Linux:**
- Build kernel modules
- Biên dịch bootloader (U-Boot)
- Tạo và quản lý thư viện
- Cross-compilation cho embedded systems

## 1.2. Makefile Structure

Một Makefile cơ bản có cấu trúc như sau:

```makefile
target: dependencies
	command
	command
```

**Các thành phần:**

1. **Target (Mục tiêu)**: Tên của file cần tạo ra hoặc tên của một hành động
2. **Dependencies (Phụ thuộc)**: Danh sách các file mà target phụ thuộc vào
3. **Command (Lệnh)**: Các lệnh shell để tạo target (phải bắt đầu bằng TAB)

**Ví dụ đơn giản:**

```makefile
# Biến
CC = gcc
CFLAGS = -Wall -O2

# Target: executable
program: main.o utils.o
	$(CC) main.o utils.o -o program

# Target: object file từ main.c
main.o: main.c
	$(CC) $(CFLAGS) -c main.c

# Target: object file từ utils.c
utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

# Target: clean
clean:
	rm -f *.o program
```

**Pattern Rules:**

```makefile
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
```

## 1.3. How to run make

### Các lệnh cơ bản:

**Syntax:** `make [-f makefile] [options] ... [targets] ...`

```bash
# Build target mặc định (target đầu tiên trong Makefile)
make

# Build một target cụ thể
make <target_name>

# Ví dụ
make clean
make all
```

### Common Makefile Targets:

Các **targets** phổ biến thường được định nghĩa trong Makefile:

- **all**: Build the entire program. This is usually the default target and depends on other targets (such as building files, compiling, etc.).

- **info**: Display information about the program (such as name, version, author, etc.). Not all Makefiles include this target.

- **clean**: Remove all temporary and compiled files (`.o`, `.out`, binaries, etc.) to clean the directory. Typically used before rebuilding from scratch.

- **install**: Copy the compiled executable files to system directories (commonly `/usr/local/bin/` or another specified directory).

- **uninstall**: The opposite of install: remove the installed files.

- **tar**: Create a `.tar` or `.tar.gz` archive to package the source code (e.g., for program distribution).

- **test**: Run tests (unit tests, integration tests, etc.) to verify that the program functions correctly after being built.

**Ví dụ sử dụng:**

```bash
make all       # Build toàn bộ project
make clean     # Xóa các file object và binary
make install   # Cài đặt vào hệ thống
make test      # Chạy test suite
make info      # Hiển thị thông tin project
```

### Options hữu ích:

```bash
# Chạy với nhiều jobs song song (tăng tốc build)
make -j4

# Hiển thị các lệnh sẽ chạy mà không thực thi
make -n

# Chỉ định file Makefile khác
make -f MyMakefile

# Build ngay cả khi file không thay đổi
make -B

# Hiển thị debug information
make -d
```

### Phony Targets:

```makefile
.PHONY: clean all install

all: program

clean:
	rm -f *.o program

install: program
	cp program /usr/local/bin/
```

## 1.4. Variable Makefile

**Variables trong Makefile** là các tên được định nghĩa để lưu trữ giá trị (strings, filenames, flags, commands, etc.) nhằm tái sử dụng trong toàn bộ Makefile. Variables giúp:

- **Tránh lặp code**: Định nghĩa một lần, sử dụng nhiều nơi
- **Dễ bảo trì**: Thay đổi giá trị ở một chỗ thay vì nhiều chỗ
- **Linh hoạt**: Có thể override từ command line hoặc environment
- **Rõ ràng**: Đặt tên có ý nghĩa thay vì hard-code values

**Ví dụ:** Thay vì viết `gcc` nhiều lần trong Makefile, ta định nghĩa `CC = gcc` và sử dụng `$(CC)` ở mọi nơi. Khi cần đổi compiler (ví dụ `clang`), chỉ cần sửa một dòng.

### Định nghĩa biến:

```makefile
# Simply expanded variables (sử dụng :=)
CC := gcc
CFLAGS := -Wall -g

# Recursively expanded variables (sử dụng =)
SOURCES = main.c utils.c
OBJECTS = $(SOURCES:.c=.o)

# Conditional assignment (chỉ gán nếu chưa có giá trị)
CC ?= gcc

# Append to variable
CFLAGS += -O2
```

### Sử dụng biến:

```makefile
program: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@
```

### Built-in Variables (Biến có sẵn):

```makefile
CC       # C compiler (mặc định: cc)
CXX      # C++ compiler (mặc định: g++)
CFLAGS   # C compiler flags
CXXFLAGS # C++ compiler flags
LDFLAGS  # Linker flags
```

### Environment Variables:

```bash
# Truyền biến từ command line
make CC=arm-linux-gcc

# Hoặc export trong shell
export CC=arm-linux-gcc
make
```

## 1.5. Automatic Variable

**Automatic Variables** là các biến đặc biệt được `make` tự động định nghĩa trong mỗi rule.

### Các Automatic Variables quan trọng:

| Variable | Ý nghĩa                     | Ví dụ                |
| -------- | --------------------------- | -------------------- |
| `$@`     | Tên của target              | `program`            |
| `$<`     | Tên của dependency đầu tiên | `main.c`             |
| `$^`     | Tên của tất cả dependencies | `main.o utils.o`     |
| `$?`     | Dependencies mới hơn target | `main.o`             |
| `$*`     | Stem của pattern rule       | `main` (từ `main.o`) |
| `$(@D)`  | Thư mục của target          | `build/`             |
| `$(@F)`  | File name của target        | `program`            |

### Ví dụ sử dụng:

```makefile
CC = gcc
CFLAGS = -Wall -g

# Sử dụng $@ và $^
program: main.o utils.o helper.o
	$(CC) $^ -o $@
	# Tương đương: gcc main.o utils.o helper.o -o program

# Sử dụng $< và $@
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	# $< là file .c tương ứng
	# $@ là file .o cần tạo

# Ví dụ: main.o: main.c
#   $(CC) $(CFLAGS) -c main.c -o main.o
```

### Ví dụ thực tế:

```makefile
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

$(BIN_DIR)/program: $(OBJECTS)
	mkdir -p $(@D)
	$(CC) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
```

---

# 2. The Process of Compiling C Program

Quá trình biên dịch một chương trình C gồm **4 giai đoạn chính**:

```
Source Code (.c) → Preprocessor → Compiler → Assembler → Linker → Executable
     ↓                  ↓             ↓           ↓           ↓
  main.c      →      main.i      →  main.s  →  main.o  →  program
```

## 2.1. Preprocessing

**Preprocessor** xử lý các chỉ thị tiền xử lý (directives) bắt đầu bằng `#`.

### Công việc của Preprocessor:

1. **Include headers**: Chèn nội dung của header files
2. **Macro expansion**: Thay thế các macro
3. **Conditional compilation**: Xử lý `#ifdef`, `#ifndef`, `#if`
4. **Remove comments**: Loại bỏ chú thích

### Ví dụ:

**File nguồn (main.c):**
```c
#include <stdio.h>
#define PI 3.14159
#define SQUARE(x) ((x) * (x))

int main() {
    double radius = 5.0;
    double area = PI * SQUARE(radius);
    printf("Area: %f\n", area);
    return 0;
}
```

**Chạy preprocessor:**
```bash
gcc -E main.c -o main.i
# Hoặc
cpp main.c > main.i
```

**Kết quả (main.i - đơn giản hóa):**
```c
# Hàng nghìn dòng từ stdio.h
...
int main() {
    double radius = 5.0;
    double area = 3.14159 * ((radius) * (radius));
    printf("Area: %f\n", area);
    return 0;
}
```

### Các options hữu ích:

```bash
# Chỉ preprocessor, không compile
gcc -E source.c -o output.i

# Thêm include path
gcc -I/path/to/headers -E source.c

# Define macro từ command line
gcc -DDEBUG=1 -E source.c
```

## 2.2. Compilation

**Compiler** chuyển đổi mã C (đã được preprocessed) thành mã Assembly.

### Quá trình:

1. **Lexical Analysis**: Phân tích từ vựng
2. **Syntax Analysis**: Phân tích cú pháp
3. **Semantic Analysis**: Phân tích ngữ nghĩa
4. **Optimization**: Tối ưu hóa code
5. **Code Generation**: Sinh mã Assembly

### Ví dụ:

```bash
# Compile thành assembly
gcc -S main.i -o main.s
# Hoặc trực tiếp từ .c
gcc -S main.c -o main.s
```

**Kết quả (main.s - x86_64 assembly):**
```assembly
	.file	"main.c"
	.text
	.section	.rodata
.LC1:
	.string	"Area: %f\n"
	.text
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movsd	.LC0(%rip), %xmm0
	movsd	%xmm0, -8(%rbp)
	movsd	-8(%rbp), %xmm0
	mulsd	%xmm0, %xmm0
	movsd	.LC2(%rip), %xmm1
	mulsd	%xmm1, %xmm0
	movsd	%xmm0, -16(%rbp)
	...
```

### Optimization levels:

```bash
gcc -O0 main.c  # Không tối ưu (debug)
gcc -O1 main.c  # Tối ưu cơ bản
gcc -O2 main.c  # Tối ưu nâng cao (khuyên dùng)
gcc -O3 main.c  # Tối ưu tích cực
gcc -Os main.c  # Tối ưu kích thước (embedded)
```

## 2.3. Assembling

**Assembler** chuyển đổi mã Assembly thành mã máy (machine code) dạng object file.

### Đặc điểm Object File (.o):

- Chứa **machine code** (binary)
- Chứa **symbol table** (bảng ký hiệu)
- Chứa **relocation information**
- Chưa thể thực thi được (cần linking)

### Ví dụ:

```bash
# Assemble
gcc -c main.s -o main.o
# Hoặc trực tiếp từ .c
gcc -c main.c -o main.o

# Sử dụng assembler trực tiếp
as main.s -o main.o
```

### Xem nội dung object file:

```bash
# Hiển thị symbol table
nm main.o

# Ví dụ output:
# 0000000000000000 T main
# U printf
# U _GLOBAL_OFFSET_TABLE_

# Disassemble object file
objdump -d main.o

# Hiển thị sections
readelf -S main.o

# Hiển thị headers
objdump -h main.o
```

### Các sections trong object file:

- `.text`: Chứa mã máy
- `.data`: Biến global/static đã khởi tạo
- `.bss`: Biến global/static chưa khởi tạo
- `.rodata`: Dữ liệu chỉ đọc (constants)
- `.symtab`: Symbol table
- `.strtab`: String table

## 2.4. Linking

**Linker** kết hợp các object files và libraries thành một executable file hoàn chỉnh.

### Công việc của Linker:

1. **Symbol Resolution**: Giải quyết các tham chiếu giữa modules
2. **Relocation**: Gán địa chỉ thực cho symbols
3. **Library Linking**: Liên kết với thư viện (static/dynamic)
4. **Generate Executable**: Tạo file thực thi

### Ví dụ:

```bash
# Linking
gcc main.o utils.o -o program

# Sử dụng linker trực tiếp
ld main.o utils.o -o program -lc

# Link với thư viện
gcc main.o -o program -lm -lpthread
```

### Các loại linking:

**1. Static Linking:**
```bash
gcc main.o -o program -static -lmylib
# Thư viện được nhúng vào executable
# File size lớn nhưng không phụ thuộc runtime
```

**2. Dynamic Linking:**
```bash
gcc main.o -o program -lmylib
# Chỉ chứa reference đến thư viện
# File size nhỏ, cần thư viện khi chạy
```

### Xem dependencies:

```bash
# Hiển thị shared libraries cần thiết
ldd program

# Ví dụ output:
# linux-vdso.so.1 =>  (0x00007fff)
# libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6
# /lib64/ld-linux-x86-64.so.2

# Xem symbols trong executable
nm program

# Xem sections
readelf -S program
```

### Linker options:

```bash
# Chỉ định thư mục thư viện
gcc main.o -L/path/to/libs -lmylib

# Chỉ định runtime library path
gcc main.o -Wl,-rpath,/path/to/libs -lmylib

# Tạo map file
gcc main.o -Wl,-Map=output.map -o program
```

---

# 3. Static and Shared Library in Linux

## 3.1. Static Library

**Static Library** (`.a` file) là một archive chứa các object files được link vào executable tại compile time.

### Đặc điểm:

✅ **Ưu điểm:**
- Không phụ thuộc vào thư viện runtime
- Executable độc lập, dễ deploy
- Nhanh hơn (không cần load runtime)
- Không có version conflicts

❌ **Nhược điểm:**
- File size lớn
- Tốn disk space (mỗi program có bản copy)
- Tốn RAM (multiple copies khi chạy)
- Khó update (phải recompile)

### Tạo Static Library:

**Bước 1: Tạo object files**
```bash
gcc -c utils.c -o utils.o
gcc -c math_ops.c -o math_ops.o
```

**Bước 2: Tạo archive**
```bash
ar rcs libmylib.a utils.o math_ops.o

# r: replace hoặc insert
# c: create archive
# s: tạo index (symbol table)
```

**Bước 3: Xem nội dung**
```bash
# Liệt kê nội dung
ar -t libmylib.a

# Hiển thị symbol table
nm libmylib.a
```

### Sử dụng Static Library:

```bash
# Method 1: Link trực tiếp
gcc main.c libmylib.a -o program

# Method 2: Sử dụng -l và -L
gcc main.c -L. -lmylib -o program

# Method 3: Static linking explicit
gcc main.c -static -L. -lmylib -o program
```

## 3.2. Shared Library

**Shared Library** (`.so` file - Shared Object) là thư viện được load vào memory tại runtime và có thể được share giữa nhiều processes.

### Đặc điểm:

✅ **Ưu điểm:**
- File size nhỏ
- Tiết kiệm disk space
- Tiết kiệm RAM (share giữa processes)
- Dễ update (không cần recompile)
- Plugin architecture

❌ **Nhược điểm:**
- Phụ thuộc runtime dependencies
- Có thể xảy ra version conflicts ("DLL Hell")
- Chậm hơn một chút (dynamic loading)
- Khó deploy (cần đảm bảo dependencies)

### Tạo Shared Library:

**Bước 1: Compile với Position Independent Code (-fPIC)**
```bash
gcc -c -fPIC utils.c -o utils.o
gcc -c -fPIC math_ops.c -o math_ops.o

# -fPIC: Position Independent Code (bắt buộc cho shared library)
```

**Bước 2: Tạo shared library**
```bash
gcc -shared utils.o math_ops.o -o libmylib.so

# Hoặc với version
gcc -shared -Wl,-soname,libmylib.so.1 utils.o math_ops.o -o libmylib.so.1.0.0
```

**Bước 3: Tạo symbolic links (nếu có version)**
```bash
ln -s libmylib.so.1.0.0 libmylib.so.1
ln -s libmylib.so.1 libmylib.so
```

### Sử dụng Shared Library:

**Compile:**
```bash
gcc main.c -L. -lmylib -o program
```

**Run - cần chỉ định library path:**

```bash
# Method 1: LD_LIBRARY_PATH
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
./program

# Method 2: rpath (embed path vào executable)
gcc main.c -L. -lmylib -Wl,-rpath,/path/to/libs -o program
./program

# Method 3: Copy vào system library path
sudo cp libmylib.so /usr/local/lib/
sudo ldconfig
./program
```

### Library Naming Convention:

```
libname.so.major.minor.patch

Ví dụ: libmylib.so.1.2.3
- lib: prefix bắt buộc
- mylib: tên thư viện
- .so: shared object extension
- 1: major version (breaking changes)
- 2: minor version (new features)
- 3: patch version (bug fixes)
```

### Tools hữu ích:

```bash
# Kiểm tra dependencies
ldd program

# Xem symbols trong .so
nm -D libmylib.so

# Tìm library trong system
ldconfig -p | grep mylib

# Xem thông tin .so file
readelf -d libmylib.so

# Reload library cache
sudo ldconfig
```

---

# 4. Demo create shared library by using Makefile

## Project Structure

```
myproject/
├── Makefile
├── include/
│   └── mylib.h
├── src/
│   ├── utils.c
│   └── math_ops.c
├── lib/
│   └── (thư viện sẽ được tạo ở đây)
└── examples/
    └── main.c
```

## Step 1: Tạo Header File

**include/mylib.h:**
```c
#ifndef MYLIB_H
#define MYLIB_H

// Utils functions
void print_message(const char *msg);
int string_length(const char *str);

// Math operations
int add(int a, int b);
int multiply(int a, int b);
double power(double base, int exp);

#endif // MYLIB_H
```

## Step 2: Implement Functions

**src/utils.c:**
```c
#include <stdio.h>
#include "mylib.h"

void print_message(const char *msg) {
    printf("[MYLIB] %s\n", msg);
}

int string_length(const char *str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}
```

**src/math_ops.c:**
```c
#include "mylib.h"

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

double power(double base, int exp) {
    double result = 1.0;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}
```

## Step 3: Tạo Example Program

**examples/main.c:**
```c
#include <stdio.h>
#include "mylib.h"

int main() {
    print_message("Hello from shared library!");
    
    printf("String length of 'Hello': %d\n", string_length("Hello"));
    printf("5 + 3 = %d\n", add(5, 3));
    printf("4 * 7 = %d\n", multiply(4, 7));
    printf("2^10 = %.0f\n", power(2, 10));
    
    return 0;
}
```

## Step 4: Tạo Makefile

**Makefile:**
```makefile
# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -O2 -fPIC
INCLUDES = -Iinclude

# Directories
SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = lib
EXAMPLE_DIR = examples

# Library settings
LIB_NAME = mylib
LIB_MAJOR = 1
LIB_MINOR = 0
LIB_PATCH = 0
LIB_VERSION = $(LIB_MAJOR).$(LIB_MINOR).$(LIB_PATCH)

# Shared library names
SO_NAME = lib$(LIB_NAME).so
SO_VERSION_FULL = $(SO_NAME).$(LIB_VERSION)
SO_VERSION_MAJOR = $(SO_NAME).$(LIB_MAJOR)

# Static library name
STATIC_LIB = lib$(LIB_NAME).a

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Phony targets
.PHONY: all clean shared static example install uninstall help

# Default target
all: shared static

# Help
help:
	@echo "Available targets:"
	@echo "  all      - Build both shared and static libraries (default)"
	@echo "  shared   - Build shared library (.so)"
	@echo "  static   - Build static library (.a)"
	@echo "  example  - Build example program"
	@echo "  install  - Install libraries to system"
	@echo "  clean    - Remove build artifacts"
	@echo "  help     - Show this help message"

# Create directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Build shared library
shared: $(LIB_DIR)/$(SO_VERSION_FULL)
	@echo "Creating symbolic links..."
	cd $(LIB_DIR) && ln -sf $(SO_VERSION_FULL) $(SO_VERSION_MAJOR)
	cd $(LIB_DIR) && ln -sf $(SO_VERSION_MAJOR) $(SO_NAME)
	@echo "Shared library built successfully!"

$(LIB_DIR)/$(SO_VERSION_FULL): $(OBJECTS) | $(LIB_DIR)
	@echo "Linking shared library..."
	$(CC) -shared -Wl,-soname,$(SO_VERSION_MAJOR) $(OBJECTS) -o $@

# Build static library
static: $(LIB_DIR)/$(STATIC_LIB)
	@echo "Static library built successfully!"

$(LIB_DIR)/$(STATIC_LIB): $(OBJECTS) | $(LIB_DIR)
	@echo "Creating static library..."
	ar rcs $@ $(OBJECTS)

# Build example program (using shared library)
example: shared
	@echo "Building example program..."
	$(CC) $(INCLUDES) $(EXAMPLE_DIR)/main.c -L$(LIB_DIR) -l$(LIB_NAME) \
		-Wl,-rpath,$(PWD)/$(LIB_DIR) -o $(EXAMPLE_DIR)/example
	@echo "Example built! Run with: ./$(EXAMPLE_DIR)/example"

# Install libraries to system
install: all
	@echo "Installing libraries..."
	sudo cp $(LIB_DIR)/$(SO_VERSION_FULL) /usr/local/lib/
	sudo cp $(LIB_DIR)/$(STATIC_LIB) /usr/local/lib/
	sudo cp include/*.h /usr/local/include/
	cd /usr/local/lib && sudo ln -sf $(SO_VERSION_FULL) $(SO_VERSION_MAJOR)
	cd /usr/local/lib && sudo ln -sf $(SO_VERSION_MAJOR) $(SO_NAME)
	sudo ldconfig
	@echo "Installation complete!"

# Uninstall libraries
uninstall:
	@echo "Uninstalling libraries..."
	sudo rm -f /usr/local/lib/$(SO_NAME)*
	sudo rm -f /usr/local/lib/$(STATIC_LIB)
	sudo rm -f /usr/local/include/mylib.h
	sudo ldconfig
	@echo "Uninstallation complete!"

# Clean build artifacts
clean:
	@echo "Cleaning..."
	rm -rf $(OBJ_DIR) $(LIB_DIR)
	rm -f $(EXAMPLE_DIR)/example
	@echo "Clean complete!"

# Show library info
info:
	@echo "Library Information:"
	@echo "  Name: $(LIB_NAME)"
	@echo "  Version: $(LIB_VERSION)"
	@echo "  Shared library: $(LIB_DIR)/$(SO_VERSION_FULL)"
	@echo "  Static library: $(LIB_DIR)/$(STATIC_LIB)"
	@echo ""
	@if [ -f $(LIB_DIR)/$(SO_VERSION_FULL) ]; then \
		echo "Symbols in shared library:"; \
		nm -D $(LIB_DIR)/$(SO_VERSION_FULL) | grep " T "; \
	fi
```

## Step 5: Build và Test

### Build thư viện:

```bash
# Build both shared and static libraries
make all

# Hoặc build riêng
make shared
make static

# Xem thông tin
make info
```

### Build và chạy example:

```bash
# Build example (sẽ tự động build shared library nếu chưa có)
make example

# Chạy example
./examples/example

# Output:
# [MYLIB] Hello from shared library!
# String length of 'Hello': 5
# 5 + 3 = 8
# 4 * 7 = 28
# 2^10 = 1024
```

### Kiểm tra dependencies:

```bash
# Xem dependencies của example program
ldd examples/example

# Output:
# libmylib.so.1 => /path/to/myproject/lib/libmylib.so.1
# libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6
# ...

# Xem symbols trong shared library
nm -D lib/libmylib.so

# Xem nội dung static library
ar -t lib/libmylib.a
nm lib/libmylib.a
```

### Install vào system:

```bash
# Install (cần sudo)
make install

# Sau khi install, có thể compile mà không cần -L và -Wl,-rpath
gcc examples/main.c -lmylib -o test
./test

# Uninstall
make uninstall
```

### Clean up:

```bash
make clean
```

## Advanced Makefile Features

### Thêm Debug và Release builds:

```makefile
# Build modes
DEBUG ?= 0
ifeq ($(DEBUG), 1)
    CFLAGS += -g -DDEBUG
    BUILD_DIR = build/debug
else
    CFLAGS += -O2 -DNDEBUG
    BUILD_DIR = build/release
endif

# Sử dụng:
make DEBUG=1  # Debug build
make          # Release build
```

### Cross-compilation cho ARM:

```makefile
# Cross-compilation
CROSS_COMPILE ?=

CC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar

# Sử dụng:
make CROSS_COMPILE=arm-linux-gnueabihf-
```

### Tự động generate dependencies:

```makefile
# Auto-generate header dependencies
DEPS = $(OBJECTS:.o=.d)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

-include $(DEPS)
```
