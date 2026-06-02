# Embedded Linux - Debugging Tools: GDB & AddressSanitizer

> Tài liệu chuyên sâu về hai công cụ debug quan trọng nhất cho C/C++ trên Linux: **GDB** (GNU Debugger) cho debug runtime và **AddressSanitizer** (ASan) cho phát hiện lỗi bộ nhớ. Bao gồm cả ứng dụng trên embedded Linux.

---

## 📑 Mục Lục

### [1. Tổng quan về Debugging Tools](#1-tổng-quan-về-debugging-tools)

### [2. GNU Debugger (GDB)](#2-gnu-debugger-gdb)
- [2.1. GDB là gì?](#21-gdb-là-gì)
- [2.2. Compile cho debug](#22-compile-cho-debug)
- [2.3. Basic GDB Commands](#23-basic-gdb-commands)
- [2.4. Breakpoints và Watchpoints](#24-breakpoints-và-watchpoints)
- [2.5. Backtrace và Frame Inspection](#25-backtrace-và-frame-inspection)
- [2.6. Memory Inspection](#26-memory-inspection)
- [2.7. Core Dump Analysis](#27-core-dump-analysis)
- [2.8. Conditional Breakpoints](#28-conditional-breakpoints)
- [2.9. GDB Scripting](#29-gdb-scripting)
- [2.10. .gdbinit](#210-gdbinit)

### [3. GDB cho Embedded Linux](#3-gdb-cho-embedded-linux)
- [3.1. Remote Debugging với gdbserver](#31-remote-debugging-với-gdbserver)
- [3.2. Cross GDB](#32-cross-gdb)
- [3.3. Debug với JTAG/SWD](#33-debug-với-jtagswd)
- [3.4. Debug Shared Libraries](#34-debug-shared-libraries)
- [3.5. Debug Kernel Module](#35-debug-kernel-module)

### [4. AddressSanitizer (ASan)](#4-addresssanitizer-asan)
- [4.1. ASan là gì?](#41-asan-là-gì)
- [4.2. Cơ chế hoạt động](#42-cơ-chế-hoạt-động)
- [4.3. Shadow Memory](#43-shadow-memory)
- [4.4. Compile và sử dụng](#44-compile-và-sử-dụng)
- [4.5. Đọc hiểu ASan output](#45-đọc-hiểu-asan-output)
- [4.6. Các loại lỗi ASan phát hiện](#46-các-loại-lỗi-asan-phát-hiện)

### [5. Các Sanitizer khác](#5-các-sanitizer-khác)
- [5.1. LeakSanitizer (LSan)](#51-leaksanitizer-lsan)
- [5.2. UndefinedBehaviorSanitizer (UBSan)](#52-undefinedbehaviorsanitizer-ubsan)
- [5.3. ThreadSanitizer (TSan)](#53-threadsanitizer-tsan)
- [5.4. MemorySanitizer (MSan)](#54-memorysanitizer-msan)
- [5.5. So sánh các Sanitizer](#55-so-sánh-các-sanitizer)

### [6. ASan trên Embedded Linux](#6-asan-trên-embedded-linux)
- [6.1. Cross-compile với ASan](#61-cross-compile-với-asan)
- [6.2. ASan trên ARM](#62-asan-trên-arm)
- [6.3. Hạn chế của ASan trên embedded](#63-hạn-chế-của-asan-trên-embedded)

### [7. Practical Debugging Scenarios](#7-practical-debugging-scenarios)
- [7.1. Buffer Overflow — GDB + ASan](#71-buffer-overflow--gdb--asan)
- [7.2. Use-After-Free](#72-use-after-free)
- [7.3. Memory Leak](#73-memory-leak)
- [7.4. Double Free](#74-double-free)
- [7.5. Stack Overflow](#75-stack-overflow)
- [7.6. Data Race (Threading)](#76-data-race-threading)

### [8. Tips và Best Practices](#8-tips-và-best-practices)

---

# 1. Tổng quan về Debugging Tools

**Phân loại công cụ debug:**

| Loại lỗi | Công cụ phù hợp | Phát hiện lúc |
|-----------|-----------------|---------------|
| Segmentation fault (SIGSEGV) | GDB (core dump) | Runtime |
| Buffer overflow (heap) | **ASan**, Valgrind | Runtime |
| Use-after-free | **ASan**, Valgrind | Runtime |
| Memory leak | **LSan**, Valgrind | Runtime |
| Stack overflow | GDB, **ASan**, -fstack-protector | Runtime |
| Double free | **ASan** | Runtime |
| Undefined behavior (shift, overflow...) | **UBSan** | Runtime |
| Data race | **TSan**, Helgrind | Runtime |
| Uninitialized memory | **MSan**, Valgrind | Runtime |
| Infinite loop | GDB (attach + bt) | Runtime |
| Logic error | GDB (step through) | Runtime |
| Cross-compilation bug | GDB (remote) + ASan | Runtime |

**Cách tiếp cận debug tối ưu:**

```
1. Bắt đầu với ASan + UBSan
   → Phát hiện nhanh lỗi bộ nhớ và undefined behavior

2. Nếu ASan ra output, dùng GDB để phân tích sâu
   → ASan báo "Address 0x...", dùng GDB xem vùng nhớ đó là gì

3. Nếu không tìm được → Valgrind (chậm nhưng chi tiết)

4. Nếu lỗi ngẫu nhiên → TSan (data race)
```

**Triết lý:** Dùng cả hai — ASan cho phát hiện nhanh, GDB cho phân tích sâu.

---

# 2. GNU Debugger (GDB)

## 2.1. GDB là gì?

**GDB** (GNU Debugger) là debugger mạnh mẽ nhất cho Linux, cho phép bạn:
- Chạy chương trình từng bước
- Dừng tại breakpoints và watchpoints
- Xem và thay đổi giá trị biến, memory, registers
- Phân tích core dump sau crash
- Debug từ xa (remote debugging) cho embedded

## 2.2. Compile cho debug

Để debug hiệu quả, bạn **phải** compile với flag `-g`:

```bash
# Cơ bản — thêm debug symbols
gcc -g -o myapp myapp.c

# Optimized + debug (có thể dùng, nhưng variable bị optimized)
gcc -O2 -g -o myapp myapp.c

# Debug + no optimization (khuyên dùng)
gcc -O0 -g -o myapp myapp.c

# Debug + no optimization + more info
gcc -O0 -g -ggdb -o myapp myapp.c
# -g: debug symbols chuẩn
# -ggdb: thêm info dành riêng cho GDB (macro definitions, ...)

# Embedded — cross-compile với debug:
arm-linux-gnueabihf-gcc -O0 -g -o myapp myapp.c

# Strip symbols cho production (bỏ debug info):
arm-linux-gnueabihg-strip myapp   # → kích thước giảm mạnh
```

### Debug symbols là gì?

```bash
# Không -g:
file myapp
# myapp: ELF 32-bit LSB executable, ARM, EABI5
size myapp
# text    data     bss     dec     hex
# 8236    572      8       8816    2270

# Với -g:
file myapp
# myapp: ELF 32-bit LSB executable, ARM, EABI5, with debug_info
size myapp
# text    data     bss     dec     hex
# 8236    572      14200   23008   59e0
# Debug symbols ~14KB (nằm trong .debug_* sections)
```

Debug symbols chứa:
- Tên function và biến
- Số dòng trong source code
- Cấu trúc struct (tên field, offset, type)
- File path của source

## 2.3. Basic GDB Commands

```bash
# ============================================================
# Khởi động GDB
# ============================================================

gdb ./myapp                    # Mở file debug
gdb --args ./myapp --flag arg  # Mở với arguments
gdb ./myapp core               # Phân tích core dump
gdb -p 1234                    # Attach vào process đang chạy (PID 1234)

# ============================================================
# Chạy chương trình
# ============================================================
(gdb) run                       # Chạy
(gdb) run arg1 arg2             # Chạy với arguments
(gdb) run < input.txt           # Redirect stdin
(gdb) start                     # Chạy và dừng ở main()
(gdb) continue                  # Tiếp tục chạy (sau breakpoint)
(gdb) next                      # Step over (không vào function)
(gdb) step                      # Step into (vào function)
(gdb) finish                    # Chạy đến khi function kết thúc
(gdb) until                     # Chạy đến dòng hiện tại + 1

# ============================================================
# Dừng chương trình
# ============================================================
(gdb) Ctrl+C                    # Interrupt (nếu đang chạy)

# ============================================================
# Xem code
# ============================================================
(gdb) list                      # 10 dòng code quanh dòng hiện tại
(gdb) list main                 # 10 dòng xung quanh main()
(gdb) list 42                   # Xem dòng 42
(gdb) list foo.c:10             # Xem dòng 10 của file foo.c

# ============================================================
# Xem biến
# ============================================================
(gdb) print x                   # In biến x
(gdb) print &x                  # Địa chỉ của x
(gdb) print *ptr                # Dereference pointer
(gdb) print array[10]           # Phần tử thứ 10
(gdb) print my_struct           # In toàn bộ struct
(gdb) print my_struct.field     # In một field
(gdb) p/x var                   # In dạng hex
(gdb) p/d var                   # In dạng decimal
(gdb) p/s str                   # In dạng string
(gdb) p/f f                     # In dạng float
(gdb) display x                 # Tự động in x sau mỗi step
(gdb) undisplay 1               # Xóa display

# ============================================================
# Thoát
# ============================================================
(gdb) quit                      # Thoát
(gdb) q                         # Shortcut
```

### Session GDB mẫu — debug đơn giản:

```c
// buggy.c
#include <stdio.h>

int divide(int a, int b) {
    return a / b;  // Bug: b = 0
}

int main() {
    int x = 10, y = 0;
    int z = divide(x, y);
    printf("Result: %d\n", z);
    return 0;
}
```

```bash
$ gcc -g -o buggy buggy.c
$ gdb ./buggy
(gdb) run
Starting program: ./buggy

Program received signal SIGFPE, Arithmetic exception.
0x... in divide (a=10, b=0) at buggy.c:5
5           return a / b;
(gdb) print a
$1 = 10
(gdb) print b
$2 = 0
(gdb) backtrace
#0  divide (a=10, b=0) at buggy.c:5
#1  0x... in main () at buggy.c:10
(gdb) info locals
a = 10
b = 0
(gdb) frame 1
#1  0x... in main () at buggy.c:10
10          int z = divide(x, y);
(gdb) print x
$3 = 10
(gdb) print y
$4 = 0              # ← Found it! y = 0 gây chia cho 0
```

## 2.4. Breakpoints và Watchpoints

### Breakpoints cơ bản:

```bash
(gdb) break main                # Break tại đầu hàm main()
(gdb) b main                    # Shortcut
(gdb) b 42                      # Break tại dòng 42 (file hiện tại)
(gdb) b foo.c:42                # Break tại dòng 42 của foo.c
(gdb) b myfile.c:42 if x > 10  # Conditional break (chỉ dừng nếu x > 10)
(gdb) b func                    # Break tại đầu function func

# Xem breakpoints:
(gdb) info breakpoints
# Num     Type           Disp Enb Address    What
# 1       breakpoint     keep y   0x...      in main at buggy.c:9
# 2       breakpoint     keep y   0x...      in divide at buggy.c:5

(gdb) info b                    # Shortcut

# Xóa breakpoints:
(gdb) delete 1                  # Xóa breakpoint số 1
(gdb) delete                    # Xóa tất cả
(gdb) clear main                # Xóa break tại main

# Enable/Disable:
(gdb) disable 1                 # Tạm tắt breakpoint 1
(gdb) enable 1                  # Bật lại

# Temporary breakpoint (tự xóa sau lần đầu hit):
(gdb) tbreak main

# Hardware breakpoint (dùng register debug hardware):
(gdb) hbreak 0x8000             # Hardware break tại address
```

### Watchpoints — dừng khi biến thay đổi:

```bash
(gdb) watch x                   # Dừng khi x thay đổi
(gdb) watch *ptr                # Dừng khi giá trị con trỏ trỏ đến thay đổi
(gdb) rwatch x                  # Dừng khi x bị đọc
(gdb) awatch x                  # Dừng khi x bị đọc hoặc ghi

(gdb) info watchpoints          # Xem tất cả watchpoints

# Ví dụ — tìm chỗ buffer overflow:
(gdb) watch buffer[100]         # Dừng khi vượt quá buffer[100]
(gdb) continue
Hardware watchpoint 2: buffer[100]
Old value = 0
New value = 65
0x... in strcpy at ...          # ← Found it! strcpy tràn buffer
```

### Catchpoints — dừng khi event xảy ra:

```bash
(gdb) catch syscall write       # Dừng khi gọi syscall write()
(gdb) catch syscall open        # Dừng khi mở file
(gdb) catch fork                # Dừng khi fork()
(gdb) catch signal SIGSEGV      # Dừng khi segmentation fault
(gdb) catch throw               # Dừng khi C++ exception được throw
```

## 2.5. Backtrace và Frame Inspection

Backtrace là **công cụ số 1** khi debug crash — nó cho bạn biết **hàm nào gọi hàm nào dẫn đến crash**.

```bash
# ============================================================
# backtrace — Stack trace từ crash point
# ============================================================
(gdb) backtrace                 # Stack trace
(gdb) bt                        # Shortcut
(gdb) bt full                   # Stack trace kèm local variables
(gdb) bt -10                    # Chỉ 10 frame gần nhất

# Output mẫu:
(gdb) bt
#0  0x... in process_data (buf=0x0, size=100) at app.c:25
#1  0x... in read_packet (fd=3) at app.c:50
#2  0x... in handle_connection (sock=5) at server.c:120
#3  0x... in main (argc=1, argv=0x...) at main.c:30

# ============================================================
# Frame navigation
# ============================================================
(gdb) frame 2                   # Chuyển đến frame #2 (handle_connection)
(gdb) f 2                       # Shortcut

(gdb) info frame                # Thông tin frame hiện tại
(gdb) info args                 # Arguments của function hiện tại
(gdb) info locals               # Local variables

(gdb) up                        # Lên frame cha (caller)
(gdb) down                      # Xuống frame con (callee)

# ============================================================
# Ứng dụng thực tế — từ crash đến root cause
# ============================================================
# Giả sử chương trình crash với segfault
(gdb) run
Program received signal SIGSEGV, Segmentation fault.
0x... in process_data (buf=0x0, size=100) at app.c:25
25          memcpy(dest, buf, size);

(gdb) bt
#0  process_data (buf=0x0, size=100) at app.c:25
#1  read_packet (fd=3) at app.c:50       ← buf được gọi từ đây
#2  handle_connection (sock=5) at server.c:120

(gdb) frame 1                   # Xem frame read_packet
(gdb) info locals               # ← buf = NULL vì lỗi từ trên
```

## 2.6. Memory Inspection

```bash
# ============================================================
# Exam memory (x)
# ============================================================
(gdb) x/20x ptr                 # 20 hex words tại địa chỉ ptr
(gdb) x/10i ptr                 # 10 instructions (disassemble)
(gdb) x/s ptr                   # String tại ptr
(gdb) x/10c ptr                 # 10 ký tự
(gdb) x/10gx ptr               # 10 giant words (8-byte)
(gdb) x/20b ptr                 # 20 bytes

# Cú pháp: x/<count><format><size> <address>
# count: số lượng
# format: x (hex), d (dec), s (string), i (instruction), c (char)
# size: b (byte), h (halfword), w (word), g (giant 8 bytes)

# Ví dụ — xem buffer:
(gdb) x/32xb buffer
# 0x...:  0x41 0x42 0x43 0x44 0x00 0x00 0x00 0x00
# 0x...:  0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
# ...

# ============================================================
# Find pattern trong memory
# ============================================================
(gdb) find begin_addr, end_addr, 'A'   # Tìm byte 'A' trong khoảng
(gdb) find /b begin, end, 0x41, 0x42   # Tìm bytes 0x41 0x42

# ============================================================
# Xem registers
# ============================================================
(gdb) info registers            # Tất cả registers
(gdb) info reg r0 r1 r2        # Register cụ thể (ARM)
(gdb) info all-registers       # Tất cả kể cả floating point/SIMD

# ============================================================
# So sánh memory layout của struct
# ============================================================
(gdb) ptype struct mystruct     # Xem type definition
(gdb) p &mystruct.field         # Offset của field
(gdb) p sizeof(mystruct)        # Kích thước struct
(gdb) p (char*)&mystruct.field - (char*)&mystruct  # Byte offset
```

## 2.7. Core Dump Analysis

Core dump là **ảnh chụp bộ nhớ của process tại thời điểm crash** — cho phép phân tích mà không cần chạy lại chương trình.

### Cấu hình core dump:

```bash
# Kiểm tra kích thước core dump cho phép
ulimit -c                      # 0 = không cho phép
ulimit -c unlimited            # Cho phép core dump (không giới hạn kích thước)

# Kiểm tra core dump pattern
cat /proc/sys/kernel/core_pattern
# |/usr/lib/systemd/systemd-coredump %P %u %g %s %t %c %h
# Hoặc: /var/lib/systemd/coredump/

# Để đơn giản, set thành file trong thư mục hiện tại:
echo "core.%p.%t" > /proc/sys/kernel/core_pattern
# %p = PID, %t = timestamp, %e = executable name

# Manually generate core:
gcore <PID>                    # Lấy core dump mà không kill process
```

### Phân tích core dump:

```bash
# Có core dump:
gdb ./myapp core.1234.1234567890

(gdb) bt                       # Stack trace tại thời điểm crash
(gdb) bt full                  # Full stack + local variables
(gdb) info registers           # CPU registers tại crash
(gdb) print $pc                # Program counter (địa chỉ lệnh crash)
(gdb) print $sp                # Stack pointer
(gdb) x/10i $pc-20             # 10 instructions quanh crash point
(gdb) info threads             # Tất cả threads (nếu multi-thread)
(gdb) thread apply all bt      # Backtrace tất cả threads

# Ví dụ — crash do NULL pointer:
(gdb) bt
#0  process_packet (pkt=0x0) at app.c:42
#1  0x... in main (argc=1, argv=0x...) at main.c:15
(gdb) frame 0
(gdb) info locals
pkt = 0x0                     # ← NULL pointer!
len = <optimized out>         # ← optimized variable
```

### systemd-coredump:

```bash
# Nếu dùng systemd:
coredumpctl list               # Xem danh sách core dumps
coredumpctl info <PID>         # Thông tin core dump
coredumpctl gdb <PID>          # Mở GDB với core dump
coredumpctl debug <PID>        # Tương tự

# Ví dụ:
coredumpctl list
# Sun 2025-05-25 10:30:00 +07  12345  /usr/bin/myapp
coredumpctl gdb 12345
```

## 2.8. Conditional Breakpoints

```bash
# Breakpoint chỉ dừng khi điều kiện đúng
(gdb) b malloc if size > 4096       # Dừng khi malloc > 4KB
(gdb) b foo.c:42 if strcmp(str, "error") == 0  # Dừng khi str là "error"
(gdb) b process_packet if pkt->id == 5         # Dừng khi packet ID = 5

# Breakpoint với hit count
(gdb) b func
(gdb) ignore 1 100              # Bỏ qua 100 lần đầu
(gdb) condition 1 x > 10        # Thêm condition sau

# Thay đổi breakpoint thành conditional:
(gdb) condition 1 x > 100       # Breakpoint 1 chỉ dừng nếu x > 100
(gdb) condition 1               # Xóa condition (dừng mọi lần)

# Breakpoint với commands (tự động chạy lệnh khi hit):
(gdb) b malloc
(gdb) commands
> silent
> print size
> backtrace 3
> continue
> end
# Mỗi lần malloc được gọi, GDB in size + 3 stack frames + tiếp tục
```

## 2.9. GDB Scripting

### Python scripting trong GDB:

```bash
# GDB có built-in Python interpreter
(gdb) python print("Hello from GDB!")
(gdb) python import os
(gdb) python print(os.uname())

# pretty-printer cho struct phức tạp:
(gdb) python
>class MyStructPrinter:
>    def __init__(self, val):
>        self.val = val
>    def to_string(self):
>        return f"MyStruct: id={self.val['id']}, name={self.val['name']}"
>gdb.pretty_printers.append(MyStructPrinter)
>end
```

### GDB script file (.gdb):

```gdb
# debug_script.gdb
set pagination off
set print pretty on
set print elements 0    # Không giới hạn số phần tử
set print array-indexes on

break main
run

# Khi dừng ở main:
print argc
print *argv@argc
break malloc
continue
```

```bash
# Chạy script:
gdb -x debug_script.gdb ./myapp
gdb -batch -x debug_script.gdb ./myapp  # Non-interactive (dùng trong CI)
```

### Python + GDB cho embedded debugging:

```python
# gdb_helpers.py — dump device register từ struct
class RegisterDump(gdb.Command):
    """Dump device registers from a memory-mapped struct"""
    
    def __init__(self):
        super().__init__("dump_regs", gdb.COMMAND_USER)
    
    def invoke(self, arg, from_tty):
        base = int(arg, 0) if arg else 0
        for i in range(32):
            addr = base + i * 4
            val = int(gdb.selected_inferior().read_memory(addr, 4).tobytes()[::-1].hex(), 16)
            print(f"REG[{i:02d}] @ 0x{addr:08X} = 0x{val:08X}")

RegisterDump()
```

```bash
(gdb) source gdb_helpers.py
(gdb) dump_regs 0x20E0000
```

## 2.10. .gdbinit

`.gdbinit` là file config tự động chạy mỗi khi GDB khởi động:

```bash
# ~/.gdbinit — global (tất cả projects)
set print pretty on
set print elements 200
set print array-indexes on
set history save on
set history size 1000

# Tự động load .gdbinit trong project directory
set auto-load local-gdbinit on

# Alias hữu ích
define btfull
    bt full
end

define stack
    info stack
end

# Pretty-print std containers
python
import sys
sys.path.insert(0, '/usr/share/gdb/python')
from libstdcxx.v6.printers import register_libstdcxx_printers
register_libstdcxx_printers(gdb.current_objfile())
end
```

```bash
# .gdbinit — per-project (trong thư mục project)
set substitute-path /build/path /home/user/project  # Source path mapping
set follow-fork-mode child                          # Debug child process
set detach-on-fork off
```

---

# 3. GDB cho Embedded Linux

## 3.1. Remote Debugging với gdbserver

Đây là kỹ thuật debug **quan trọng nhất** cho embedded: chạy chương trình trên board, debug từ PC.

### Kiến trúc:

```
PC (Host)                           Board (Target)
┌──────────────────┐               ┌──────────────────────┐
│ gdb (arm-gdb)    │ ←─── TCP ───→ │ gdbserver            │
│   - Architecture |     :1234     │   - Attach vào PID    │
│     ARM          |               │   - Hoặc chạy app     │
│   - Có source    |               │   - Binary đã strip   │
│   - Có debug sym |               │   - Không cần -g      │
└──────────────────┘               └──────────────────────┘
```

### Các bước thực hiện:

```bash
# Bước 1: Trên HOST — cross-compile với debug symbols
arm-linux-gnueabihf-gcc -O0 -g -o myapp myapp.c

# Bước 2: Copy binary lên target + strip cho production binary
# (Giữ binary có debug symbol ở host, gửi bản strip lên target)
arm-linux-gnueabihf-strip myapp
scp myapp root@192.168.1.100:/tmp/

# Bước 3: Trên TARGET — chạy gdbserver
# Trên board:
gdbserver :1234 /tmp/myapp
# Process 1234 created
# Listening on port 1234

# Hoặc attach vào process đang chạy:
gdbserver :1234 --attach $(pidof myapp)

# Bước 4: Trên HOST — kết nối GDB
arm-linux-gnueabihf-gdb myapp    # Dùng binary CÓ debug symbol
(gdb) target remote 192.168.1.100:1234
(gdb) continue                    # Tiếp tục chạy
(gdb) break main                  # Set breakpoint
(gdb) continue
# ... debug bình thường
```

### GDB với gdbserver — đầy đủ:

```bash
# Host:
arm-linux-gnueabihf-gdb myapp
(gdb) set sysroot /path/to/target/rootfs  # Target's root filesystem
(gdb) set solib-search-path /path/to/target/lib  # Shared libs
(gdb) target remote 192.168.1.100:1234
(gdb) break main
(gdb) continue

# Khi breakpoint hit:
(gdb) print argc
(gdb) print argv[0]
(gdb) next                       # Step trên target qua network!
(gdb) step
(gdb) bt
(gdb) info registers
(gdb) x/10i $pc                  # Disassemble instruction hiện tại

# Multi-process:
(gdb) set follow-fork-mode child  # Debug child process
(gdb) set detach-on-fork off
```

### GDB + gdbserver + initramfs:

```bash
# Trong embedded, board có thể chưa có network lúc đầu
# Dùng serial thay thế:
gdbserver /dev/ttyS0:115200 ./myapp   # Serial console

# Kết nối:
arm-linux-gnueabihf-gdb myapp
(gdb) target remote /dev/ttyUSB0       # Serial GDB
(gdb) set serial baud 115200
```

## 3.2. Cross GDB

Khi phát triển cho ARM, bạn cần **cross-GDB** — GDB chạy trên x86 nhưng hiểu kiến trúc ARM:

```bash
# Cross-GDB toolchains:
arm-linux-gnueabihf-gdb          # ARM 32-bit
aarch64-linux-gnu-gdb            # ARM 64-bit
riscv64-linux-gnu-gdb            # RISC-V

# Kiểm tra:
arm-linux-gnueabihf-gdb --version
# GNU gdb (Linaro GCC) 9.2

# Cấu hình cross-GDB:
(gdb) set architecture arm
(gdb) set endian little
(gdb) set arm force-mode thumb    # Nếu code dùng Thumb instruction set
```

### Debug application trên QEMU:

```bash
# Chạy ARM binary trên QEMU user-mode
qemu-arm -g 1234 ./myapp          # Chờ GDB connect port 1234

# Kết nối:
arm-linux-gnueabihf-gdb myapp
(gdb) target remote :1234
(gdb) continue
```

## 3.3. Debug với JTAG/SWD

Trên embedded, GDB cũng có thể giao tiếp với **JTAG debugger** (OpenOCD, JLink, ST-Link):

```bash
# OpenOCD (kết nối với board qua JTAG/SWD):
openocd -f interface/stlink-v2.cfg -f target/stm32mp1.cfg

# OpenOCD tạo GDB server trên port 3333:
# Info : Listening on port 3333 for gdb connections

# Kết nối GDB:
arm-none-eabi-gdb myapp.elf
(gdb) target remote :3333
(gdb) monitor reset halt         # Reset board, halt CPU
(gdb) load myapp.elf             # Load program vào flash/RAM
(gdb) break main
(gdb) continue

# Debug bare-metal (không OS) — không có process, không có virtual memory
(gdb) monitor arm semihosting_enable  # Dùng semihosting cho I/O
(gdb) set $pc = 0x80000000       # Set program counter
(gdb) stepi                      # Step 1 instruction
```

## 3.4. Debug Shared Libraries

```bash
# Khi app dùng shared library, GDB cần tìm symbols:

# Set search path:
(gdb) set solib-search-path /path/to/target/lib:/path/to/project/lib
(gdb) info sharedlibrary         # Xem shared libraries đã load
(gdb) sharedlibrary              # Load symbols cho tất cả shared libs

# Debug vào shared library:
(gdb) b lib_function             # Breakpoint trong shared lib
(gdb) b /path/to/src/lib.c:42    # Breakpoint theo source

# Kiểm tra library có debug symbol không:
(gdb) info sharedlibrary
# From                To                  Syms Read   Shared Object
# 0x...               0x...              Yes (*)      /lib/libc.so.6
#                                         ↑ Yes = có debug symbols
```

## 3.5. Debug Kernel Module

```bash
# Load module:
insmod mydriver.ko

# Trên target — lấy thông tin:
cat /proc/modules | grep mydriver
ls -la /sys/module/mydriver/sections/
cat /sys/module/mydriver/sections/.text  # Địa chỉ .text section

# Trên host — debug với GDB:
# Cần vmlinux (kernel có debug) + module .ko
gdb vmlinux
(gdb) add-symbol-file mydriver.ko 0x...   # Địa chỉ .text trên target
(gdb) b mydriver_ioctl
(gdb) continue

# Hoặc dùng kgdb (kernel GDB stub):
echo g > /proc/sysrq-trigger      # Tạm dừng kernel, chờ GDB
# Trên host qua serial:
gdb vmlinux
(gdb) target remote /dev/ttyS0
(gdb) break do_fork
(gdb) continue
```

---

# 4. AddressSanitizer (ASan)

## 4.1. ASan là gì?

**AddressSanitizer** (ASan) là công cụ **phát hiện lỗi bộ nhớ** do Google phát triển, được tích hợp sẵn trong GCC và Clang (từ GCC 4.8, Clang 3.1).

### Không phải "công cụ khác" — nó là compiler instrumentation:

```
ASan không phải:
❌ Công cụ bên ngoài (như Valgrind)
❌ Runtime library riêng

ASan là:
✅ Compiler tự động thêm code kiểm tra vào binary
✅ Chỉ cần thêm flag -fsanitize=address khi compile
✅ Runtime library (libasan) được link tự động
```

### ASan vs Valgrind:

| Tiêu chí | ASan | Valgrind |
|----------|------|----------|
| **Tốc độ** | ~2x chậm hơn bình thường | ~20-50x chậm hơn |
| **Memory** | ~2x RAM hơn | ~5-10x RAM hơn |
| **Phát hiện OOB** | Xuất sắc (cả stack + heap + global) | Tốt (heap) |
| **Use-after-free** | Xuất sắc | Tốt |
| **Memory leak** | Qua LSan | Tốt |
| **Uninitialized mem** | Không | Có (--undef-value-errors) |
| **Yêu cầu** | Cần recompile | Binary bất kỳ |
| **Phù hợp** | Development, CI | CI, khi không có source |

## 4.2. Cơ chế hoạt động

ASan hoạt động dựa trên **3 kỹ thuật chính**:

### 1. Redzones — Vùng bảo vệ quanh mỗi allocation

```c
// Khi bạn malloc(100):
// ASan tự động cấp phát:
// [redzone 32B] [user data 100B] [redzone 32B]
//     ↑                               ↑
//     Poisoned (không thể đọc/ghi)    Poisoned

// Nếu bạn ghi byte thứ 101 (buffer[100]), ASan phát hiện ngay:
// → Đang ghi vào redzone → báo lỗi!

// Kích thước redzone: 32 bytes mặc định
// Có thể điều chỉnh: ASAN_REDZONE_SIZE=64
```

### 2. Quarantine — Cách ly vùng nhớ đã free

```c
// Khi bạn free(ptr):
// ASan không trả memory về heap ngay
// Nó đưa vào "quarantine" (cách ly)
//   [freed memory] ← đánh dấu "poisoned"
//   Mọi truy cập vào đây → Use-After-Free!

// Kích thước quarantine mặc định: 256MB (có thể điều chỉnh)
// ASAN_OPTIONS=quarantine_size_mb=64
```

### 3. Shadow Memory — Bộ nhớ theo dõi (chi tiết ở phần 4.3)

```c
// Mỗi 8 bytes trong main memory → 1 byte shadow memory
// Shadow byte: 0 = 8 bytes đều valid
//               k = k bytes valid, (8-k) bytes poisoned (từ cuối)
//               <0 = toàn bộ poisoned (redzone/freed)
//               0xFA = stack redzone
//               0xFD = heap redzone
//               0xF9 = global redzone
//               0xCA = freed memory (quarantine)
```

## 4.3. Shadow Memory

Shadow memory là **trái tim của ASan**. Nó cho phép ASan kiểm tra mỗi lần memory access chỉ với **2 instructions**.

### Cách ASan kiểm tra mỗi lần đọc/ghi:

```c
// Code gốc:
char c = buffer[idx];

// Sau khi ASan instrument:
// Step 1: Tính shadow address
shadow_addr = (addr >> 3) + SHADOW_OFFSET;

// Step 2: Đọc shadow byte
shadow_byte = *shadow_addr;

// Step 3: Kiểm tra
if (shadow_byte != 0) {
    // Nếu != 0 → có vấn đề
    if (shadow_byte < 0 || (shadow_byte > 0 && (addr & 7) >= shadow_byte)) {
        __asan_report_error(addr);
    }
}
// 2 lệnh này được compiler thêm vào TRƯỚC mỗi lần load/store!
```

### Shadow memory layout:

```c
// Trên Linux x86_64 64-bit:
// Main memory:     0x000000000000 – 0x7fffffffffff  (128TB user space)
// Shadow memory:   0x00007fff8000 – 0x00007fffffffff  (1/8 main)

// Cách tính: shadow = (addr >> 3) + 0x7fff8000
// VD: addr 0x1000 → shadow 0x7fff800200

// Giá trị shadow byte:
#define ASAN_HEAP_RZ         0xFA  // Heap redzone
#define ASAN_STACK_RZ        0xF3  // Stack redzone  
#define ASAN_STACK_PARTIAL   0xF2  // Stack partial redzone
#define ASAN_GLOBAL_RZ       0xF9  // Global redzone
#define ASAN_FREED           0xFD  // Freed memory
#define ASAN_QUARANTINE      0xFE  // Quarantined (use-after-free)
```

### Ví dụ shadow memory mapping:

```c
// Bạn có: int arr[4] = {1,2,3,4};  // 16 bytes
// Ở địa chỉ: 0x1000

// Memory map:
// Address      Content       Shadow byte
// 0x1000       arr[0] = 1    \
// 0x1004       arr[1] = 2     → shadow_at(0x1000) = 0 (8 bytes valid)
// 0x1008       arr[2] = 3    \
// 0x100C       arr[3] = 4     → shadow_at(0x1008) = 0 (8 bytes valid)
// 0x1010       redzone (32B) → shadow_at(0x1010) = 0xFA (HEAP_RZ)
// 0x1011                         shadow[0x1010>>3] = 0xFA
// ...

// Nếu bạn đọc arr[5] (offset 20, address 0x1014):
// shadow = (0x1014 >> 3) + offset = reading 0xFA
// → 0xFA < 0 → ERROR! Heap buffer overflow
```

## 4.4. Compile và sử dụng

### Cơ bản:

```bash
# GCC (từ 4.8):
gcc -fsanitize=address -g -O0 -o myapp myapp.c

# Clang (từ 3.1):
clang -fsanitize=address -g -O0 -o myapp myapp.c

# C++:
g++ -fsanitize=address -g -O0 -o myapp myapp.cpp

# Với CMake:
cmake -DCMAKE_C_FLAGS="-fsanitize=address -g -O0" ..
cmake -DCMAKE_CXX_FLAGS="-fsanitize=address -g -O0" ..
cmake -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address" ..
```

### ASan detection levels:

```bash
# Level 1: Stack + Heap + Global buffer overflow
# Level 2: Use-after-free
# Level 3: Double free, invalid free
# Level 4: Memory leak (cần LSan kết hợp)

# ASan options — set qua environment variable:
ASAN_OPTIONS="detect_leaks=1:verbosity=1"
ASAN_OPTIONS="detect_stack_use_after_return=1"  # Stack use-after-return
ASAN_OPTIONS="max_redzone=128"                   # Redzone 128 bytes
ASAN_OPTIONS="quarantine_size_mb=64"             # Quarantine 64MB
ASAN_OPTIONS="malloc_context_size=30"            # Stack trace depth 
ASAN_OPTIONS="check_initialization_order=1"      # Static init order

# Detect ODR violations:
ASAN_OPTIONS="detect_odr_violation=1"

# Suppress known errors (file):
LSAN_OPTIONS="suppressions=asan.supp"
```

### O1/O2 optimization:

```bash
# ASan có thể dùng với optimization:
gcc -fsanitize=address -O1 -g -o myapp myapp.c
gcc -fsanitize=address -O2 -g -o myapp myapp.c

# Lưu ý: optimization có thể thay đổi stack trace, làm khó debug hơn
# Nhưng vẫn phát hiện được lỗi!

# Không nên dùng -O0 nếu cần test performance
# ASan với -O2: overhead ~2x (vẫn chấp nhận được)
```

### Link:

```bash
# Static linking:
gcc -fsanitize=address -static -g -o myapp myapp.c
# → libasan được link trực tiếp vào binary
# → Không cần libasan trên target (quan trọng cho embedded!)

# Dynamic (mặc định):
gcc -fsanitize=address -g -o myapp myapp.c
# → Cần libasan.so trên hệ thống
```

## 4.5. Đọc hiểu ASan output

### Buffer overflow — ASan output mẫu:

```c
// test_asan.c
#include <stdlib.h>
#include <string.h>

int main() {
    char *buf = (char*)malloc(10);
    strcpy(buf, "Hello World!");  // Copy 13 bytes vào buffer 10 bytes
    free(buf);
    return 0;
}
```

```bash
$ gcc -fsanitize=address -g -o test_asan test_asan.c
$ ./test_asan

=================================================================
==12345==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x60200000001a
   ↑PID      ↑ Loại lỗi                    ↑ Địa chỉ bị vi phạm

WRITE of size 13 at 0x60200000001a thread T0
↑ Operation   ↑ Kích thước    ↑ Địa chỉ           ↑ Thread

    #0 0x... in main /home/user/test_asan.c:6
    ↑ Stack trace: dòng 6: strcpy(buf, "Hello World!")

   0x60200000001a is located 0 bytes after 10-byte region
   ↑ Vị trí            | sau vùng 10 byte
   [0x602000000010, 0x60200000001a) ← allocated by thread T0:
   ↑ Khoảng allocated        ↑ allocated at:
   #0 0x... in malloc
   #1 0x... in main /home/user/test_asan.c:5
   ↑ Dòng 5: buf = (char*)malloc(10)
```

**Cách đọc từng phần:**

| Thành phần | Ý nghĩa | Hành động |
|-----------|---------|-----------|
| `heap-buffer-overflow` | Ghi/đọc vượt quá heap allocation | Kiểm tra offset + allocation size |
| `WRITE of size 13` | Ghi 13 bytes | So với malloc(10) → tràn 3 bytes |
| `at 0x60200000001a` | Địa chỉ bị lỗi | Có thể dùng GDB xem địa chỉ này |
| `0 bytes after 10-byte region` | Ngay sau vùng 10 bytes | Tràn 3 bytes đầu tiên của redzone |
| `#1 main test_asan.c:5` | malloc(10) tại dòng 5 | Đây là allocation gốc |

### Use-After-Free output:

```c
char *ptr = malloc(10);
free(ptr);
strcpy(ptr, "test");  // Use-After-Free!
```

```bash
=================================================================
==12346==ERROR: AddressSanitizer: heap-use-after-free on address 0x602000000010

WRITE of size 5 at 0x602000000010 thread T0
    #0 0x... in main /test_asan.c:7

0x602000000010 is located 0 bytes inside of 10-byte region
freed by thread T0 here:
    #0 0x... in __interceptor_free
    #1 0x... in main /test_asan.c:6    ← Dòng free()!

previously allocated by thread T0 here:
    #0 0x... in __interceptor_malloc
    #1 0x... in main /test_asan.c:5    ← Dòng malloc()!
```

**Điểm mạnh của ASan:** Nó cho bạn **cả stack trace của free() và malloc()** — bạn biết chính xác ai allocated, ai freed.

## 4.6. Các loại lỗi ASan phát hiện

### 1. Heap buffer overflow:

```c
char *buf = malloc(10);
buf[10] = 'A';           // Heap buffer overflow
// ASan: heap-buffer-overflow
```

### 2. Stack buffer overflow:

```c
void func() {
    char buf[10];
    buf[10] = 'A';       // Stack buffer overflow
}
// ASan: stack-buffer-overflow
// ASan dùng redzone trên STACK, không chỉ heap!
// Mỗi local array được thêm redzone
```

### 3. Global buffer overflow:

```c
char global[10];

int main() {
    global[10] = 'A';    // Global buffer overflow
}
// ASan: global-buffer-overflow
```

### 4. Use-after-free:

```c
int *ptr = malloc(sizeof(int));
free(ptr);
*ptr = 42;               // Use-after-free
// ASan: heap-use-after-free
```

### 5. Double free:

```c
int *ptr = malloc(sizeof(int));
free(ptr);
free(ptr);               // Double free
// ASan: double-free
```

### 6. Invalid free:

```c
int *ptr = malloc(10);
free(ptr + 1);           // Invalid free (con trỏ sai offset)
// ASan: invalid-free
```

### 7. Stack use-after-return:

```c
int *ptr;

void func() {
    int local = 42;
    ptr = &local;        // Trỏ đến stack variable
}                        // ← local mất scope

int main() {
    func();
    return *ptr;         // ← Use-after-return!
}
// Cần bật: ASAN_OPTIONS=detect_stack_use_after_return=1
// ASan: stack-use-after-return
```

### 8. Memory leak (qua LSan):

```c
void func() {
    int *ptr = malloc(100);
    // Quên free(ptr)!
}
// Cần ASAN_OPTIONS=detect_leaks=1
// ASan: LeakSanitizer: detected memory leaks
```

---

# 5. Các Sanitizer khác

## 5.1. LeakSanitizer (LSan)

LSan phát hiện memory leak, thường được tích hợp trong ASan:

```bash
# Kích hoạt LSan (mặc định ON với ASan trên Linux):
ASAN_OPTIONS="detect_leaks=1" ./myapp

# Chạy standalone (không ASan):
clang -fsanitize=leak -g -o myapp myapp.c
```

```bash
# Output LSan:
=================================================================
==12347==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 100 byte(s) in 1 object(s) allocated from:
    #0 0x... in malloc
    #1 0x... in func /test.c:4
    #2 0x... in main /test.c:8

SUMMARY: AddressSanitizer: 100 byte(s) leaked in 1 allocation(s).
```

### Suppress known leaks:

```bash
# asan.supp
# Suppress leak trong thư viện third-party
leak:libssl.so
leak:libcrypto.so

# Dùng:
LSAN_OPTIONS="suppressions=asan.supp" ./myapp
```

## 5.2. UndefinedBehaviorSanitizer (UBSan)

UBSan phát hiện **undefined behavior** của C/C++:

```bash
# Compile:
gcc -fsanitize=undefined -g -o myapp myapp.c

# Hoặc kết hợp với ASan:
gcc -fsanitize=address,undefined -g -o myapp myapp.c
```

### Các lỗi UBSan phát hiện:

```c
// 1. Integer overflow
int x = INT_MAX;
x += 1;              // signed integer overflow → UB

// 2. Shift overflow
int y = 1 << 33;     // Shift > bit width

// 3. NULL pointer
int *p = NULL;
int z = *p;          // NULL pointer dereference

// 4. Misaligned access
int *ptr = (int*)0x1;  // Misaligned address
int val = *ptr;

// 5. Out-of-bounds enum
enum Color { RED, GREEN };
enum Color c = 42;   // Out-of-range enum

// 6. Division by zero
int div = x / 0;
```

```bash
# Output UBSan:
test_ubsan.c:5:5: runtime error: signed integer overflow:
  2147483647 + 1 cannot be represented in type 'int'
```

## 5.3. ThreadSanitizer (TSan)

TSan phát hiện **data race** trong multi-thread:

```bash
gcc -fsanitize=thread -g -pthread -o myapp myapp.c
```

### Ví dụ — data race:

```c
// race.c
#include <pthread.h>

int counter = 0;  // ← Không mutex!

void *worker(void *arg) {
    for (int i = 0; i < 1000000; i++)
        counter++;  // ← Data race!
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, worker, NULL);
    pthread_create(&t2, NULL, worker, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}
```

```bash
$ gcc -fsanitize=thread -g -pthread -o race race.c
$ ./race

==================
WARNING: ThreadSanitizer: data race (pid=12348)
  Read of size 4 at 0x... by thread T2:
    #0 worker race.c:6 (counter++)
  
  Previous write of size 4 at 0x... by thread T1:
    #0 worker race.c:6 (counter++)
  
  Location is global 'counter' at 0x...

SUMMARY: ThreadSanitizer: data race race.c:6 in worker
```

## 5.4. MemorySanitizer (MSan)

MSan phát hiện **đọc uninitialized memory**:

```bash
# Chỉ Clang hỗ trợ:
clang -fsanitize=memory -g -o myapp myapp.c

# Lưu ý: MSan yêu cầu ALL libraries được compile với MSan
# → Khó dùng trong thực tế (trừ khi build toàn bộ từ source)
```

### Ví dụ:

```c
#include <stdlib.h>

int main() {
    int *arr = malloc(10 * sizeof(int));
    int sum = 0;
    for (int i = 0; i < 10; i++)
        sum += arr[i];  // arr không được initialize!
    free(arr);
    return sum;
}
```

## 5.5. So sánh các Sanitizer

| Sanitizer | Flag | Phát hiện | Overhead | Linux | Embedded |
|-----------|------|-----------|----------|-------|----------|
| **ASan** | `-fsanitize=address` | Buffer overflow, UAF, double free | ~2x | ✅ Có | ✅ Hạn chế |
| **LSan** | `-fsanitize=leak` | Memory leak | ~1.1x | ✅ Có | ✅ Có |
| **UBSan** | `-fsanitize=undefined` | Undefined behavior | ~1.2x | ✅ Có | ✅ Có |
| **TSan** | `-fsanitize=thread` | Data race | ~5x | ✅ Có | ⚠️ Limited |
| **MSan** | `-fsanitize=memory` | Uninitialized read | ~3x | ✅ Clang only | ❌ |

**Khuyên dùng cho embedded development:**
```
ASan + UBSan luôn bật trong build debug
TSan khi có multi-threading
LSan trong CI pipeline
```

---

# 6. ASan trên Embedded Linux

## 6.1. Cross-compile với ASan

```bash
# Cần cross-compiler toolchain có hỗ trợ ASan!
# Kiểm tra:
arm-linux-gnueabihf-gcc -fsanitize=address -v 2>&1 | grep -i asan
# → Nếu không có output là toolchain không hỗ trợ ASan

# LINARO toolchain có ASan:
# linaro-7.5, linaro-8.3 trở lên hỗ trợ

# Yocto: thêm vào local.conf
LICENSE_FLAGS_WHITELIST += "commercial"
PACKAGECONFIG_append_pn-gcc-runtime = " sanitizers"

# Buildroot: enable trong menuconfig
# Toolchain → C++ support → Enable address sanitizer

# Cross-compile với ASan:
arm-linux-gnueabihf-gcc \
    -fsanitize=address \
    -static-libasan \     # Static link (quan trọng cho embedded!)
    -g -O0 -o myapp myapp.c

# Copy lên target:
scp myapp root@board:/tmp/
ssh root@board 'ASAN_OPTIONS="detect_leaks=0" /tmp/myapp'
```

## 6.2. ASan trên ARM

### Hạn chế về shadow memory trên ARM32:

```c
// ARM32 có không gian địa chỉ khác x86_64
// ASan cần 1/8 memory space cho shadow

// Trên ARM32, cần kernel patch:
// CONFIG_VMSPLIT_2G thay vì 3G
// → Cần kernel custom!

// Nếu không có kernel patch:
// ASan sử dụng "low shadow" mode
// → Cần /proc/sys/vm/mmap_min_addr < 64K
echo 0 > /proc/sys/vm/mmap_min_addr

// Kiểm tra ASan có hoạt động trên ARM không:
// Error: "Shadow memory range is not available"
// → Kernel không hỗ trợ, cần chọn toolchain phù hợp
```

### ARM64 (AArch64):

```bash
# ARM64 hỗ trợ ASan tốt hơn ARM32
aarch64-linux-gnu-gcc \
    -fsanitize=address \
    -static-libasan \
    -g -O0 -o myapp myapp.c

# AArch64 có 48-bit address space
# Shadow memory mapping tự động
```

## 6.3. Hạn chế của ASan trên embedded

| Hạn chế | Nguyên nhân | Giải pháp |
|---------|-------------|-----------|
| **Kích thước binary lớn** | ASan instrumentation + static libasan | Có thể tăng 2-5x | Dùng cho debug build, bản release bỏ ASan |
| **RAM tăng 2-3x** | Shadow memory + quarantine | Giới hạn quarantine (`quarantine_size_mb=32`) |
| **Flash không đủ** | Binary + libasan lớn (vài MB) | `-static-libasan` vs dynamic libasan |
| **ARM32 không hỗ trợ** | Kernel memory split | Dùng ARM64 hoặc kernel custom |
| **Không có trên toolchain** | Toolchain cũ không build ASan | Dùng Linaro 8.3+ hoặc build from source |
| **Không stack trace** | Không có addr2line trên target | Cross addr2line, dùng GDB remote |

### Workaround — file nhỏ:

```c
// Nếu ASan không chạy được trên target, dùng cách này:

// 1. Compile với ASan cho HOST
// 2. Tạo test case reproduce lỗi trên HOST
// 3. Debug trên HOST, fix lỗi
// 4. Cross-compile không ASan cho target

// Hoặc dùng chế độ SDT (Statically Define Tracing):
// Chạy valgrind trên host với ARM emulation:
qemu-arm -L /path/to/rootfs valgrind ./myapp
```

---

# 7. Practical Debugging Scenarios

## 7.1. Buffer Overflow — GDB + ASan

```c
// overflow.c
#include <string.h>
#include <stdio.h>

void process(char *input) {
    char buf[16];
    strcpy(buf, input);  // Nếu input > 16 bytes → overflow!
    printf("Buffer: %s\n", buf);
}

int main(int argc, char **argv) {
    if (argc > 1)
        process(argv[1]);
    return 0;
}
```

**Cách 1 — Dùng ASan (nhanh nhất):**
```bash
$ gcc -fsanitize=address -g -o overflow overflow.c
$ ./overflow $(python -c "print('A'*32)")

==12349==ERROR: AddressSanitizer: stack-buffer-overflow on address 0x...
WRITE of size 33 at 0x... thread T0
    #0 0x... in process overflow.c:6
    #1 0x... in main overflow.c:11
```

**Cách 2 — Dùng GDB (khi không có ASan):**
```bash
$ gcc -g -o overflow overflow.c
$ gdb ./overflow
(gdb) run AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHH

Program received signal SIGSEGV, Segmentation fault.
strcpy () at .../strcpy.c:...
(gdb) bt
#0  strcpy () at ...
#1  0x... in process (input=0x...) at overflow.c:6
(gdb) frame 1
(gdb) info registers
# Xem $pc, $sp
(gdb) x/20x $sp
# Xem stack frame bị ghi đè như thế nào
```

## 7.2. Use-After-Free

```c
// uaf.c
#include <stdlib.h>

typedef struct {
    int id;
    char name[32];
    void (*callback)(void);
} User;

void exploit() {
    // Giả lập attacker control
}

int main() {
    User *user = malloc(sizeof(User));
    user->id = 1;
    user->callback = NULL;
    
    free(user);              // Free
    
    // ... nhiều code sau đó ...
    
    if (user->callback)      // Use-After-Free!
        user->callback();
    
    return 0;
}
```

```bash
$ gcc -fsanitize=address -g -o uaf uaf.c
$ ./uaf

==12350==ERROR: AddressSanitizer: heap-use-after-free
READ of size 8 at 0x...
    #0 0x... in main uaf.c:21

0x... is located 16 bytes inside of 40-byte region
freed by thread T0 here:
    #0 0x... in free
    #1 0x... in main uaf.c:17

previously allocated by thread T0 here:
    #0 0x... in malloc
    #1 0x... in main uaf.c:12
```

## 7.3. Memory Leak

```c
// leak.c
#include <stdlib.h>

char *read_file(const char *path) {
    char *buf = malloc(4096);
    // ... đọc file vào buf ...
    return buf;  // Caller phải free
}

int main() {
    for (int i = 0; i < 100; i++) {
        char *data = read_file("/etc/passwd");
        // Quên free(data)!
    }
    return 0;
}
```

```bash
$ gcc -fsanitize=address -g -o leak leak.c
$ ASAN_OPTIONS="detect_leaks=1" ./leak

=================================================================
==12351==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 409600 byte(s) in 100 object(s) allocated from:
    #0 0x... in malloc
    #1 0x... in read_file leak.c:5
    #2 0x... in main leak.c:12

SUMMARY: AddressSanitizer: 409600 byte(s) leaked in 100 allocation(s).
```

## 7.4. Double Free

```c
// double_free.c
#include <stdlib.h>

int main() {
    int *ptr = malloc(sizeof(int));
    *ptr = 42;
    free(ptr);
    free(ptr);  // Double free!
    return 0;
}
```

```bash
$ gcc -fsanitize=address -g -o double_free double_free.c
$ ./double_free

==12352==ERROR: AddressSanitizer: attempting double-free on 0x...
    #0 0x... in free
    #1 0x... in main double_free.c:7

0x... is located 0 bytes inside of 4-byte region
freed by thread T0 here:
    #0 0x... in free
    #1 0x... in main double_free.c:6   ← Lần free thứ nhất
previously allocated by thread T0 here:
    #0 0x... in malloc
    #1 0x... in main double_free.c:4
```

## 7.5. Stack Overflow (đệ quy vô hạn)

```c
// stackoverflow.c
void recurse(int depth) {
    char buf[1024];
    printf("Depth: %d\n", depth);
    recurse(depth + 1);
}

int main() {
    recurse(0);
    return 0;
}
```

```bash
# GDB — phát hiện stack overflow:
$ gcc -g -o stackoverflow stackoverflow.c
$ gdb ./stackoverflow
(gdb) run
...
Program received signal SIGSEGV, Segmentation fault.
(gdb) bt
#0  0x... in recurse (depth=262074) at stackoverflow.c:4
#1  0x... in recurse (depth=262073) at stackoverflow.c:6
#2  0x... in recurse (depth=262072) at stackoverflow.c:6
#3  0x... in recurse (depth=262071) at stackoverflow.c:6
... (hàng ngàn frame)
(gdb) bt full -5
# Chỉ xem 5 frame cuối
(gdb) print depth
$1 = 262074
# → Stack depth ~262K frames đã làm tràn stack!

# Với ASan:
$ gcc -fsanitize=address -g -o stackoverflow stackoverflow.c
$ ./stackoverflow
# ASan không bắt được stack overflow do đệ quy
# → Dùng GDB hoặc -fstack-check
```

## 7.6. Data Race (Threading)

```c
// race.c — cải thiện từ section 5.3
#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex;
    int counter;
} SharedData;

void *worker(void *arg) {
    SharedData *data = (SharedData*)arg;
    for (int i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&data->mutex);
        data->counter++;  // Protected by mutex
        pthread_mutex_unlock(&data->mutex);
    }
    return NULL;
}

int main() {
    SharedData data = {PTHREAD_MUTEX_INITIALIZER, 0};
    pthread_t t1, t2;
    pthread_create(&t1, NULL, worker, &data);
    pthread_create(&t2, NULL, worker, &data);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Counter: %d\n", data.counter);
    return 0;
}
```

```bash
# Với TSan:
$ gcc -fsanitize=thread -g -pthread -o race race.c
$ ./race
# Không có warning → không data race

# Nếu bỏ mutex (race_unsafe.c):
$ gcc -fsanitize=thread -g -pthread -o race_unsafe race_unsafe.c  
$ ./race_unsafe
# ThreadSanitizer: data race → flag ngay!
```

---

# 8. Tips và Best Practices

### 1. Compilation strategy:

```bash
# Debug build (cho development):
CFLAGS += -g -O0 -fsanitize=address,undefined
LDFLAGS += -fsanitize=address,undefined

# Release build (cho production):
CFLAGS += -O2 -DNDEBUG
# Không có -g, không có sanitizer

# CI build:
CFLAGS += -g -O1 -fsanitize=address,undefined,leak
ASAN_OPTIONS="detect_leaks=1:exitcode=1"
```

### 2. GDB quick reference:

```bash
# Khi chương trình crash — 3 lệnh đầu tiên:
(gdb) bt           # Stack trace từ crash point
(gdb) bt full      # + local variables
(gdb) frame 0      # Xem vị trí crash

# Khi cần tìm lỗi — 3 lệnh tiếp theo:
(gdb) print var    # Giá trị biến
(gdb) info locals  # Tất cả local variables
(gdb) list         # Code quanh crash point
```

### 3. ASan trong CI:

```yaml
# .gitlab-ci.yml
test-asan:
  script:
    - gcc -fsanitize=address -g -O1 -o test test.c
    - ASAN_OPTIONS="exitcode=1:detect_leaks=1" ./test
      # exitcode=1: ASan trả về exit code 1 khi có lỗi
      # → CI pipeline fail
```

### 4. Debug embedded — REMOTE checklist:

```bash
# Checklist khi remote debug:
# [ ] Target có gdbserver? (busybox có thể không có!)
# [ ] Port 1234 không bị firewall?
# [ ] Binary có debug symbol ở host? (ko cần trên target)
# [ ] sysroot/solib-search-path set đúng?
# [ ] Cùng kiến trúc (ARM/x86/RISC-V)?
# [ ] ASan có chạy trên target không?

# Nếu không có gdbserver:
# → Dùng core dump (ulimit -c unlimited)
# → Gửi core dump về host
# → arm-linux-gnueabihf-gdb myapp core
```

### 5. Kết hợp GDB + ASan:

```bash
# ASan báo lỗi, muốn xem chi tiết hơn:
ASAN_OPTIONS="abort_on_error=1:disable_coredump=0" ./myapp
# → ASan gọi abort() thay vì exit()
# → Tạo core dump
# → Dùng GDB phân tích core

# Hoặc chạy trực tiếp trong GDB:
gdb ./myapp
(gdb) set environment ASAN_OPTIONS "abort_on_error=1"
(gdb) run
# ASan phát hiện lỗi → abort() → GDB bắt được!
(gdb) bt                      # Stack trace từ GDB (chi tiết hơn ASan)
(gdb) frame 0
(gdb) print *relevant_var
```

### 6. Khi ASan không phát hiện được:

```bash
# Các lỗi ASan KHÔNG phát hiện:
# - Infinite loop → GDB (Ctrl+C, bt)
# - Logic error → GDB step through
# - Stack overflow (đệ quy) → GDB
# - Memory corruption nhỏ (1 byte trong alloc) → khó, dùng Valgrind
# - Use-after-free nhưng memory đã được cấp lại → Valgrind

# Khi ASan silent nhưng chương trình vẫn crash:
# 1. Chạy với Valgrind
valgrind --tool=memcheck ./myapp

# 2. Dùng GDB xem regular crash
gdb ./myapp
(gdb) run
# Crash → bt → tìm bug
```

---

## 📚 Tài liệu tham khảo

- [GDB Documentation](https://sourceware.org/gdb/current/onlinedocs/)
- [AddressSanitizer (Google)](https://github.com/google/sanitizers/wiki/AddressSanitizer)
- [GCC Sanitizers](https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html)
- [ASan for ARM](https://github.com/google/sanitizers/wiki/AddressSanitizerForARM)
- [UndefinedBehaviorSanitizer](https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html)
- [ThreadSanitizer](https://github.com/google/sanitizers/wiki/ThreadSanitizerCppManual)
- [OpenOCD User's Guide](https://openocd.org/doc/html/index.html)
