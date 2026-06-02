# Chương trình "Hello World" trên Embedded Linux

Tài liệu này hướng dẫn chi tiết cách tạo, biên dịch và chạy chương trình C đầu tiên trên môi trường Embedded Linux sử dụng GCC compiler.

---

## 📑 Mục lục

- [1. Giới thiệu](#1-giới-thiệu)
- [2. Yêu cầu trước khi bắt đầu](#2-yêu-cầu-trước-khi-bắt-đầu)
- [3. Tạo dự án Hello World](#3-tạo-dự-án-hello-world)
  - [Bước 1: Tạo thư mục dự án](#bước-1-tạo-thư-mục-dự-án)
  - [Bước 2: Tạo file hello.c](#bước-2-tạo-file-helloc)
  - [Bước 3: Viết code](#bước-3-viết-code)
- [4. Biên dịch với GCC](#4-biên-dịch-với-gcc)
  - [Biên dịch cơ bản](#biên-dịch-cơ-bản)
  - [Compiler flags quan trọng](#compiler-flags-quan-trọng)
- [5. Chạy chương trình](#5-chạy-chương-trình)
- [6. Sử dụng VS Code (Khuyến nghị)](#6-sử-dụng-vs-code-khuyến-nghị)
- [7. Tạo Makefile](#7-tạo-makefile)
- [8. Debugging với GDB](#8-debugging-với-gdb)
- [9. Xử lý sự cố thường gặp](#9-xử-lý-sự-cố-thường-gặp)

---

## 1. Giới thiệu

**"Hello World"** là chương trình truyền thống đầu tiên khi học lập trình. Trong môi trường Embedded Linux, chương trình này giúp:

- Kiểm tra toolchain (GCC compiler) hoạt động đúng
- Làm quen với quy trình: viết code → biên dịch → chạy
- Hiểu cấu trúc cơ bản của chương trình C
- Thiết lập môi trường phát triển

---

## 2. Yêu cầu trước khi bắt đầu

### ✅ Checklist

- [x] Máy ảo Ubuntu 22.04 LTS đã cài đặt và chạy
- [x] VS Code đã kết nối SSH đến Ubuntu (khuyến nghị)
- [x] GCC compiler đã cài đặt trên Ubuntu
- [x] Quyền truy cập terminal

### Kiểm tra GCC đã cài đặt

Mở Terminal trong Ubuntu và chạy:

```bash
gcc --version
```

Kết quả mong đợi:

```
gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
Copyright (C) 2021 Free Software Foundation, Inc.
```

### Cài đặt GCC (nếu chưa có)

```bash
sudo apt update
sudo apt install build-essential -y
```

Package `build-essential` bao gồm:
- **gcc**: GNU C Compiler
- **g++**: GNU C++ Compiler
- **make**: Build automation tool
- **libc-dev**: C standard library headers

---

## 3. Tạo dự án Hello World

### 🔸 Bước 1: Tạo thư mục dự án

Tạo thư mục để tổ chức code:

```bash
mkdir -p ~/embedded-linux/hello-world
cd ~/embedded-linux/hello-world
```

**Giải thích:**
- `mkdir -p`: Tạo thư mục kể cả thư mục cha chưa tồn tại
- `~/embedded-linux/hello-world`: Đường dẫn tuyệt đối đến thư mục dự án
- `cd`: Chuyển vào thư mục vừa tạo

---

### 🔸 Bước 2: Tạo file hello.c

#### Cách 1: Sử dụng nano (Terminal)

```bash
nano hello.c
```

#### Cách 2: Sử dụng VS Code (Khuyến nghị)

1. Trong VS Code đã kết nối SSH
2. Mở thư mục: **File → Open Folder** → chọn `~/embedded-linux/hello-world`
3. Tạo file mới: **New File** → đặt tên `hello.c`

---

### 🔸 Bước 3: Viết code

Nhập nội dung sau vào file `hello.c`:

```c
#include <stdio.h>

int main() {
    printf("Hello Embedded Linux!\n");
    return 0;
}
```

**Giải thích code:**

| Dòng code | Ý nghĩa |
|-----------|---------|
| `#include <stdio.h>` | Include thư viện chuẩn I/O (input/output) |
| `int main()` | Hàm main - điểm bắt đầu chương trình |
| `printf(...)` | In chuỗi ra màn hình |
| `\n` | Ký tự xuống dòng (newline) |
| `return 0` | Trả về 0 = chương trình thành công |

Lưu file:
- **nano**: `Ctrl + O` → `Enter` → `Ctrl + X`
- **VS Code**: `Ctrl + S`

---

## 4. Biên dịch với GCC

### Biên dịch cơ bản

Biên dịch file `hello.c` thành file thực thi `hello`:

```bash
gcc hello.c -o hello
```

**Giải thích:**
- `gcc`: Gọi GNU C Compiler
- `hello.c`: File nguồn (source code)
- `-o hello`: Output file tên `hello` (executable)

Kiểm tra file đã tạo:

```bash
ls -lh
```

Kết quả:

```
-rwxrwxr-x 1 embedded embedded 16K Dec  4 11:00 hello
-rw-rw-r-- 1 embedded embedded  95 Dec  4 10:55 hello.c
```

> **Lưu ý:** File `hello` có quyền thực thi (`x`)

---

### Compiler flags quan trọng

#### 🔹 Bảng các flags thường dùng

| Flag | Mô tả | Ví dụ |
|------|-------|-------|
| `-o <file>` | Đặt tên file output | `gcc hello.c -o hello` |
| `-Wall` | Hiển thị tất cả warnings | `gcc -Wall hello.c -o hello` |
| `-Wextra` | Warnings bổ sung | `gcc -Wall -Wextra hello.c -o hello` |
| `-g` | Thêm debug symbols cho GDB | `gcc -g hello.c -o hello` |
| `-O0` | Không optimization (debug) | `gcc -O0 -g hello.c -o hello` |
| `-O2` | Optimization level 2 (release) | `gcc -O2 hello.c -o hello` |
| `-std=c11` | Sử dụng C11 standard | `gcc -std=c11 hello.c -o hello` |

#### 🔹 Biên dịch với warnings (Khuyến nghị)

```bash
gcc -Wall -Wextra -std=c11 hello.c -o hello
```

Lệnh này sẽ:
- Hiển thị tất cả warnings
- Sử dụng C11 standard
- Giúp phát hiện lỗi tiềm ẩn

#### 🔹 Biên dịch cho debugging

```bash
gcc -g -O0 -Wall hello.c -o hello
```

Lệnh này tạo file có debug symbols để dùng với GDB.

---

## 5. Chạy chương trình

### Chạy file thực thi

```bash
./hello
```

**Kết quả:**

```
Hello Embedded Linux!
```

**Giải thích:**
- `./`: Chạy file trong thư mục hiện tại
- `hello`: Tên file thực thi

---

### Kiểm tra exit code

```bash
./hello
echo $?
```

Kết quả:

```
Hello Embedded Linux!
0
```

Exit code `0` = chương trình chạy thành công ✅

---

## 6. Sử dụng VS Code (Khuyến nghị)

### Lợi ích của VS Code

- **IntelliSense**: Gợi ý code tự động
- **Syntax highlighting**: Tô màu cú pháp
- **Integrated terminal**: Terminal tích hợp
- **Debugging**: Debug trực quan với breakpoints

---

### Thiết lập VS Code cho C/C++

#### 🔸 Bước 1: Mở thư mục dự án

1. Trong VS Code (đã kết nối SSH)
2. **File → Open Folder**
3. Chọn `~/embedded-linux/hello-world`

---

#### 🔸 Bước 2: Tạo tasks.json (Build task)

1. Nhấn `Ctrl + Shift + P`
2. Gõ: **Tasks: Configure Default Build Task**
3. Chọn **C/C++: gcc build active file**

File `.vscode/tasks.json` được tạo:

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "type": "cppbuild",
            "label": "C/C++: gcc build active file",
            "command": "/usr/bin/gcc",
            "args": [
                "-fdiagnostics-color=always",
                "-g",
                "-Wall",
                "-Wextra",
                "${file}",
                "-o",
                "${fileDirname}/${fileBasenameNoExtension}"
            ],
            "options": {
                "cwd": "${fileDirname}"
            },
            "problemMatcher": [
                "$gcc"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}
```

---

#### 🔸 Bước 3: Build với VS Code

1. Mở file `hello.c`
2. Nhấn `Ctrl + Shift + B` (Build)
3. Chương trình được biên dịch tự động

---

#### 🔸 Bước 4: Chạy trong Terminal

1. Mở terminal: `Ctrl + ~`
2. Chạy: `./hello`

---

## 7. Tạo Makefile

**Makefile** tự động hóa quá trình build, đặc biệt hữu ích cho dự án lớn.

### Tạo file Makefile

```bash
nano Makefile
```

Nội dung:

```makefile
# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11 -g

# Target executable
TARGET = hello

# Source files
SRCS = hello.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Build executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(TARGET) $(OBJS)

# Rebuild
rebuild: clean all

.PHONY: all clean rebuild
```

---

### Sử dụng Makefile

#### Build chương trình

```bash
make
```

Kết quả:

```
gcc -Wall -Wextra -std=c11 -g -c hello.c -o hello.o
gcc -Wall -Wextra -std=c11 -g -o hello hello.o
```

#### Xóa files build

```bash
make clean
```

#### Rebuild từ đầu

```bash
make rebuild
```

---

### Giải thích Makefile

| Thành phần | Ý nghĩa |
|------------|---------|
| `CC = gcc` | Định nghĩa compiler |
| `CFLAGS` | Các flags cho compiler |
| `TARGET` | Tên file thực thi |
| `SRCS` | Danh sách file nguồn |
| `OBJS` | Danh sách object files (.o) |
| `all:` | Target mặc định |
| `clean:` | Xóa files build |
| `.PHONY` | Khai báo targets không phải files |

---

## 8. Debugging với GDB

**GDB (GNU Debugger)** là công cụ debug mạnh mẽ cho C/C++.

### Cài đặt GDB

```bash
sudo apt install gdb -y
```

---

### Biên dịch với debug symbols

```bash
gcc -g -O0 -Wall hello.c -o hello
```

Flag `-g` thêm debug information vào executable.

---

### Sử dụng GDB cơ bản

#### Khởi động GDB

```bash
gdb ./hello
```

#### Các lệnh GDB cơ bản

| Lệnh | Mô tả |
|------|-------|
| `run` hoặc `r` | Chạy chương trình |
| `break main` hoặc `b main` | Đặt breakpoint tại hàm main |
| `break 4` hoặc `b 4` | Đặt breakpoint tại dòng 4 |
| `next` hoặc `n` | Chạy dòng tiếp theo (không vào hàm) |
| `step` hoặc `s` | Chạy dòng tiếp theo (vào hàm) |
| `print <var>` hoặc `p <var>` | In giá trị biến |
| `continue` hoặc `c` | Tiếp tục chạy |
| `quit` hoặc `q` | Thoát GDB |

---

### Ví dụ debug session

```bash
$ gdb ./hello
(gdb) break main          # Đặt breakpoint tại main
Breakpoint 1 at 0x1149: file hello.c, line 4.
(gdb) run                 # Chạy chương trình
Starting program: /home/embedded/embedded-linux/hello-world/hello 

Breakpoint 1, main () at hello.c:4
4           printf("Hello Embedded Linux!\n");
(gdb) next                # Chạy dòng tiếp theo
Hello Embedded Linux!
5           return 0;
(gdb) continue            # Tiếp tục
Continuing.
[Inferior 1 (process 1234) exited normally]
(gdb) quit                # Thoát
```

---

### Debug với VS Code (Nâng cao)

Tạo file `.vscode/launch.json`:

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "C/C++: gcc debug",
            "type": "cppdbg",
            "request": "launch",
            "program": "${fileDirname}/${fileBasenameNoExtension}",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${fileDirname}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],
            "preLaunchTask": "C/C++: gcc build active file",
            "miDebuggerPath": "/usr/bin/gdb"
        }
    ]
}
```

Nhấn `F5` để debug với giao diện trực quan.

---

## 9. Xử lý sự cố thường gặp

### ❌ Lỗi: "gcc: command not found"

**Nguyên nhân:** GCC chưa được cài đặt

**Giải pháp:**

```bash
sudo apt update
sudo apt install build-essential -y
```

---

### ❌ Lỗi: "Permission denied" khi chạy ./hello

**Nguyên nhân:** File không có quyền thực thi

**Giải pháp:**

```bash
chmod +x hello
./hello
```

---

### ❌ Lỗi: "No such file or directory"

**Nguyên nhân:** 
- File `hello.c` không tồn tại
- Đang ở sai thư mục

**Giải pháp:**

```bash
# Kiểm tra thư mục hiện tại
pwd

# Liệt kê files
ls -la

# Chuyển đến thư mục đúng
cd ~/embedded-linux/hello-world
```

---

### ❌ Warning: "implicit declaration of function 'printf'"

**Nguyên nhân:** Thiếu `#include <stdio.h>`

**Giải pháp:** Thêm dòng include vào đầu file:

```c
#include <stdio.h>
```

---

### ❌ Lỗi: "undefined reference to 'main'"

**Nguyên nhân:** Không có hàm `main()` trong code

**Giải pháp:** Đảm bảo có hàm main:

```c
int main() {
    // code here
    return 0;
}
```

---

## 📖 Tài liệu tham khảo

- [GCC Documentation](https://gcc.gnu.org/onlinedocs/)
- [GDB Tutorial](https://www.gnu.org/software/gdb/documentation/)
- [GNU Make Manual](https://www.gnu.org/software/make/manual/)
- [Hướng dẫn SSH từ VS Code vào máy ảo](huong-dan-ssh-toi-vmware-qua-vscode.md)

---
