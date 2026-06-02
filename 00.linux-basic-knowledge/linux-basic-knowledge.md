# Embedded Linux - Linux Basic Knowledge

> Tài liệu học về Linux cơ bản: Hệ điều hành Linux, Terminal, Shell, Shell Script, Biến môi trường, User/Group và quản lý hệ thống

---

## 📑 Mục Lục

### [1. Hệ điều hành Linux](#1-hệ-điều-hành-linux)
- [1.1. Tổng quan về Linux](#11-tổng-quan-về-linux)
- [1.2. Linux Kernel](#12-linux-kernel)
- [1.3. User Space](#13-user-space)

### [2. Linux trong Embedded](#2-linux-trong-embedded)
- [2.1. Tại sao Linux phổ biến?](#21-tại-sao-linux-phổ-biến)
- [2.2. Khi nào Embedded cần Linux?](#22-khi-nào-embedded-cần-linux)
- [2.3. Linux vs RTOS/MCU](#23-linux-vs-rtosmcu)
- [2.4. Cơ hội việc làm Linux Embedded](#24-cơ-hội-việc-làm-linux-embedded)

### [3. Kiến trúc Linux](#3-kiến-trúc-linux)
- [3.1. Kernel Space vs User Space](#31-kernel-space-vs-user-space)
- [3.2. System Call Interface](#32-system-call-interface)
- [3.3. Nguyên tắc bảo mật](#33-nguyên-tắc-bảo-mật)

### [4. Terminal](#4-terminal)
- [4.1. Terminal là gì?](#41-terminal-là-gì)
- [4.2. Chế độ nhập liệu](#42-chế-độ-nhập-liệu)
- [4.3. Các Terminal phổ biến](#43-các-terminal-phổ-biến)

### [5. Shell](#5-shell)
- [5.1. Shell là gì?](#51-shell-là-gì)
- [5.2. Quy trình xử lý lệnh](#52-quy-trình-xử-lý-lệnh)
- [5.3. Các loại Shell](#53-các-loại-shell)

### [6. Shell Script](#6-shell-script)
- [6.1. Shell Script là gì?](#61-shell-script-là-gì)
- [6.2. Cấu trúc Script](#62-cấu-trúc-script)
- [6.3. Biến và Argument](#63-biến-và-argument)
- [6.4. Cấu trúc điều khiển](#64-cấu-trúc-điều-khiển)
- [6.5. Hàm](#65-hàm)
- [6.6. Chạy Script](#66-chạy-script)

### [7. Biến môi trường](#7-biến-môi-trường)
- [7.1. Khái niệm](#71-khái-niệm)
- [7.2. Biến thường vs Biến môi trường](#72-biến-thường-vs-biến-môi-trường)
- [7.3. Các biến quan trọng](#73-các-biến-quan-trọng)
- [7.4. Lưu ý bảo mật](#74-lưu-ý-bảo-mật)

### [8. User & Group](#8-user--group)
- [8.1. Khái niệm User](#81-khái-niệm-user)
- [8.2. Khái niệm Group](#82-khái-niệm-group)
- [8.3. Các lệnh quản lý User](#83-các-lệnh-quản-lý-user)
- [8.4. Các lệnh quản lý Group](#84-các-lệnh-quản-lý-group)

### [9. Root & Sudo](#9-root--sudo)
- [9.1. Root User](#91-root-user)
- [9.2. Sudo](#92-sudo)
- [9.3. Phân quyền an toàn](#93-phân-quyền-an-toàn)

---

# 1. Hệ điều hành Linux

## 1.1. Tổng quan về Linux

**Linux** là một hệ điều hành mã nguồn mở, được sử dụng từ máy tính cá nhân đến server, thiết bị nhúng và hệ thống cloud.

```
Linux OS = Linux Kernel + User Space (tools + libraries + applications...)
```

### Đặc điểm nổi bật:

| Đặc điểm | Mô tả |
| --------- | ----- |
| **Mã nguồn mở** | Miễn phí, tự do sử dụng và chỉnh sửa |
| **Ổn định** | Được kiểm chứng qua hàng thập kỷ |
| **Bảo mật** | Cộng đồng lớn, phát hiện và sửa lỗi nhanh |
| **Linh hoạt** | Có thể tùy biến từ embedded đến supercomputer |
| **Hệ sinh thái** | Hỗ trợ rộng rãi với nhiều công cụ |

### Linux Distribution (Distro):

```
Linux Distribution = Linux Kernel + GNU Tools + User Space + Package Manager

Ví dụ:
├── Debian/Ubuntu      → apt package manager
├── Fedora/RHEL        → dnf package manager
├── Arch              → pacman package manager
├── Android           → Google Play, bPlay Store
└── Embedded Distro   → Buildroot, Yocto, OpenWrt
```

> **Lưu ý:** Các bản phân phối Linux khác nhau chủ yếu ở **User Space**, còn **Kernel** là chung.

## 1.2. Linux Kernel

**Linux Kernel** là nhân (core) của hệ điều hành Linux, nằm giữa phần mềm (application) và phần cứng.

### Các chức năng chính của Kernel:

```
Kernel Functions:
├── Process Management      → Lập lịch CPU, quản lý tiến trình
├── Memory Management       → Cấp phát và quản lý bộ nhớ
├── Device Management       → Giao tiếp với phần cứng qua drivers
├── File System Management  → Tổ chức và truy cập dữ liệu
├── Network Management      → TCP/IP stack, routing
└── Security & Access Control → Permissions, capabilities
```

### Đặc điểm:

- **Ra đời năm 1991** bởi Linus Torvalds (sinh viên Đại học Helsinki)
- Ban đầu là dự án nghiên cứu cá nhân
- Hiện là một trong những dự án mã nguồn mở lớn nhất thế giới
- **~27+ triệu dòng code** (tính đến 2024)

### Kernel trong Embedded:

```bash
# Xem phiên bản kernel
uname -r
# Output: 5.15.0-generic

# Xem thông tin kernel đầy đủ
uname -a

# Xem kernel image
ls -l /boot/vmlinuz-*
```

## 1.3. User Space

**User Space** là vùng bộ nhớ nơi các ứng dụng người dùng chạy, tách biệt với Kernel để đảm bảo bảo mật và ổn định.

### Thành phần của User Space:

```
User Space Components:
├── System Libraries       → glibc, uClibc, musl
├── GNU Tools              → bash, ls, cp, mv, grep, sed...
├── Package Manager        → apt, dnf, pacman
├── Init System            → systemd, init, OpenRC
├── Shells                 → bash, sh, zsh, fish
├── Applications           → nginx, mysql, python, nodejs
└── Configuration Files    → /etc/passwd, /etc/fstab
```

### Tương tác User Space ↔ Kernel:

```
┌─────────────────────────────────────────────┐
│              User Space                      │
│  ┌─────────┐  ┌─────────┐  ┌─────────────┐  │
│  │   App   │  │   App   │  │    App      │  │
│  └────┬────┘  └────┬────┘  └──────┬──────┘  │
│       │            │              │          │
│  ┌────▼────────────▼──────────────▼────┐    │
│  │         System Call Interface        │    │
│  └──────────────────┬───────────────────┘    │
└─────────────────────┼───────────────────────┘
                      │
┌─────────────────────┼───────────────────────┐
│                     │    Kernel Space        │
│              ┌──────▼──────┐                 │
│              │   Kernel    │                 │
│              └──────┬──────┘                 │
│                     │                        │
│  ┌─────────┐  ┌──────▼──────┐  ┌──────────┐ │
│  │ Drivers │  │    VFS      │  │ Network  │ │
│  └─────────┘  └─────────────┘  └──────────┘ │
└─────────────────────────────────────────────┘
```

---

# 2. Linux trong Embedded

## 2.1. Tại sao Linux phổ biến?

### Ưu điểm của Linux trong Embedded:

| Ưu điểm | Mô tả |
| -------- | ----- |
| **Mã nguồn mở** | Không phụ thuộc vendor, tự do tùy biến |
| **Miễn phí** | Giảm chi phí license |
| **Linh hoạt** | Build từ tiny (2MB) đến full-featured |
| **Hiệu năng cao** | Tối ưu cho nhiều loại hardware |
| **Ổn định** | Được kiểm chứng trong production |
| **Hệ sinh thái** | Sẵn sàng drivers, libraries, tools |
| **Cộng đồng** | Hỗ trợ rộng, nhiều tài liệu |

> **Kết luận:** Miễn phí + Mở + Linh hoạt + Ổn định + Chạy được mọi nơi

### Linux phổ biến trong Embedded:

```
Embedded Linux Applications:
├── Router & Gateway     → OpenWrt, dd-wrt
├── Smart TV & STB      → Android TV, webOS
├── Automotive          → Automotive Grade Linux (AGL)
├── IoT Devices          → Raspberry Pi, BeagleBone
├── Industrial HMI      → SCADA, PLC interfaces
└── Aerospace & Defense → L4Linux, real-time extensions
```

## 2.2. Khi nào Embedded cần Linux?

Không phải mọi thiết bị embedded đều cần Linux. Việc chọn Linux phụ thuộc vào yêu cầu của ứng dụng.

### Trường hợp nên dùng Linux:

| Trường hợp | Lý do | Ví dụ |
| ---------- | ----- | ----- |
| **Đa nhiệm phức tạp** | Linux quản lý đa tiến trình tốt | Media center, NAS |
| **Networking phức tạp** | Có sẵn driver + full network stack | Router, gateway |
| **UI/UX phức tạp** | Graphic stack, multimedia sẵn có | Smart TV, HMI |
| **Multimedia** | Video/audio processing có sẵn | IP Camera, streaming |
| **AI/ML** | TensorFlow, PyTorch, OpenCV | Edge AI, robotics |
| **Tài nguyên lớn** | RAM > 256MB, Flash > 1GB | Single board computers |

### Trường hợp nên dùng RTOS/MCU:

| Trường hợp | Lý do | Ví dụ |
| ---------- | ----- | ----- |
| **Đơn giản** | Chỉ cần 1 task đơn giản | LED blinker, relay control |
| **Thời gian thực** | Cần deterministic response | Motor control, safety systems |
| **Tài nguyên ít** | RAM < 64KB, Flash < 512KB | Sensor nodes, simple MCU |
| **Tiêu thụ thấp** | Cần tiết kiệm pin | Wearables, remote sensors |
| **Chi phí thấp** | MCU giá rẻ | Home appliances, toys |

### Quyết định chọn Linux:

```
Decision Tree:
┌─────────────────────────┐
│  Cần networking phức    │───Yes──→ Linux
│        tạp?             │
└───────────┬─────────────┘
            │ No
            ▼
┌─────────────────────────┐
│  Cần GUI/multimedia?    │───Yes──→ Linux
└───────────┬─────────────┘
            │ No
            ▼
┌─────────────────────────┐
│  Cần multi-tasking      │───Yes──→ Linux
│     phức tạp?           │
└───────────┬─────────────┘
            │ No
            ▼
┌─────────────────────────┐
│  Tài nguyên có thể      │
│  chứa Linux?           │
│  (RAM>128MB, Flash>1GB) │
└───────────┬─────────────┘
            │
      Yes───┴───No──→ RTOS/MCU
        │
        ▼
     Linux
```

## 2.3. Linux vs RTOS/MCU

### So sánh Linux Embedded vs RTOS:

| Tiêu chí | Linux Embedded | RTOS/MCU |
| -------- | -------------- | -------- |
| **Đa nhiệm** | Full multi-tasking | Limited tasks |
| **Networking** | Full stack (TCP/IP, WiFi, BT) | Minimal/none |
| **Memory** | RAM 128MB - 1GB+ | RAM 4KB - 1MB |
| **Storage** | Flash 1GB - 32GB | Flash 16KB - 16MB |
| **Boot time** | 10-60 giây | < 1 giây |
| **Real-time** | Soft real-time (preempt-rt) | Hard real-time |
| **GUI** | Qt, GTK, Electron | Little/none |
| **Cost** | BOM cao hơn | BOM thấp |
| **Complexity** | Cao | Thấp |

### Ví dụ so sánh:

```
┌─────────────────┬─────────────────┬─────────────────┐
│   Thiết bị      │     Giải pháp   │      Lý do      │
├─────────────────┼─────────────────┼─────────────────┤
│ LED nhấp nháy  │    STM32/MCU     │ Đơn giản, rẻ   │
│ Đèn tín hiệu   │    STM32/MCU     │ Real-time cần  │
│ Sensor gửi UART│    ESP32/RTOS    │ Nhẹ, WiFi có   │
│ Router WiFi     │    Linux        │ Network stack   │
│ IP Camera       │    Linux         │ Video + network │
│ Smart Speaker   │    Linux/Android │ Audio + AI     │
└─────────────────┴─────────────────┴─────────────────┘
```

## 2.4. Cơ hội việc làm Linux Embedded

### Lĩnh vực công việc:

```
Linux Embedded Career Paths:
├── Kernel Layer
│   ├── Kernel Developer        → Tối ưu kernel
│   ├── Driver Developer        → Viết driver cho hardware
│   ├── BSP Engineer            → Board Support Package
│   └── Porting Engineer        → Port Linux lên hardware mới
│
├── User Space Layer
│   ├── Application Developer   → Viết ứng dụng embedded
│   ├── System Developer        → System services, daemons
│   ├── Network Engineer        → Network protocols, security
│   └── Multimedia Developer    → Audio/Video processing
│
└── DevOps & Tooling
    ├── Build System Engineer   → Yocto, Buildroot
    ├── CI/CD Engineer           → Automated testing, deployment
    └── DevOps Engineer         → Development infrastructure
```

### Yêu cầu chung:

| Cấp độ | Yêu cầu |
| ------ | -------- |
| **Junior** | C++/C, Linux basics, Shell scripting, Git |
| **Mid** | System programming, Driver basics, Debugging |
| **Senior** | Kernel internals, Performance optimization, Architecture |
| **Expert** | Security, Real-time, Custom kernel development |

---

# 3. Kiến trúc Linux

## 3.1. Kernel Space vs User Space

### Phân chia bộ nhớ:

```
Virtual Memory Layout:
┌─────────────────────────────┐ 0xFFFFFFFF (4GB)
│                             │
│      Kernel Space           │ ← Ring 0 (Highest privilege)
│      (1GB - typical)        │    Cannot be accessed directly
│                             │    from user programs
├─────────────────────────────┤
│                             │
│      User Space             │ ← Ring 3 (Lower privilege)
│      (3GB - typical)        │    Applications run here
│                             │
└─────────────────────────────┘ 0x00000000
```

### Kernel Space:

| Chức năng | Mô tả |
| --------- | ----- |
| **Quản lý tài nguyên** | CPU, RAM, I/O devices |
| **Giao tiếp hardware** | Direct hardware access qua drivers |
| **System call handling** | Xử lý requests từ user space |
| **Memory management** | Virtual memory, paging, swapping |
| **Process scheduling** | CPU time allocation |

### User Space:

| Chức năng | Mô tả |
| --------- | ----- |
| **Chạy ứng dụng** | User applications |
| **System libraries** | glibc, POSIX interfaces |
| **Development tools** | Compiler, debugger |
| **Services** | systemd, network manager |

### Ví dụ tương tác:

```c
// User Space - Application
#include <stdio.h>
#include <unistd.h>

int main() {
    // Đây là User Space code
    printf("Running in User Space\n");

    // Khi gọi system call, chuyển sang Kernel Space
    int fd = open("/dev/mydevice", O_RDWR);
    read(fd, buffer, 100);

    // Kernel xử lý, trả kết quả về User Space
    close(fd);

    return 0;
}
```

## 3.2. System Call Interface

**System Call (syscall)** là cầu nối giữa User Space và Kernel Space.

### Cách hoạt động:

```
System Call Flow:
┌──────────────────────────────────────────────────────────┐
│ User Space                                                │
│                                                           │
│ 1. Application calls function                             │
│    write(fd, buffer, size);                              │
│                                                           │
│ 2. Library (glibc) prepares arguments                     │
│    và gọi syscall instruction                            │
│                                                           │
│    ┌─────────────────────────────────────────────┐       │
│    │ syscall number = __NR_write (1)              │       │
│    │ Arguments: fd=3, buffer=0x7fff..., size=100  │       │
│    └─────────────────────┬───────────────────────┘       │
└─────────────────────────┼────────────────────────────────┘
                          │ syscall instruction (Ring 3 → Ring 0)
                          ▼
┌─────────────────────────┼────────────────────────────────┐
│                         │ Kernel Space                    │
│                    ┌────▼────┐                            │
│                    │  SYS    │                            │
│                    │  CALL   │                            │
│                    │  TABLE  │                            │
│                    └────┬────┘                            │
│                         │                                 │
│ 3. Kernel looks up syscall number                         │
│    and calls sys_write()                                  │
│                         │                                 │
│                    ┌────▼────┐                            │
│                    │sys_write│                            │
│                    │ (kernel │                            │
│                    │ function)│                           │
│                    └────┬────┘                            │
│                         │                                 │
│ 4. Returns to user space                                 │
└───────────────────────────────────────────────────────────┘
```

### Các System Call quan trọng:

| Category | System Calls |
| -------- | ------------ |
| **Process** | fork, exec, wait, exit |
| **File** | open, close, read, write, lseek |
| **Memory** | mmap, brk, mprotect |
| **Network** | socket, connect, send, recv |
| **IPC** | pipe, shmget, msgget |

### Ví dụ System Call:

```bash
# Xem system calls của một command
strace ls -l

# Output mẫu:
# open(".", O_RDONLY|O_NONBLOCK|O_CLOEXEC|O_DIRECTORY) = 3
# fstat(3, {st_mode=S_IFDIR|0755, st_size=4096, ...}) = 0
# getdents(3, [/* 10 entries */], 1024) = 80
# close(3)                                = 0
```

## 3.3. Nguyên tắc bảo mật

### Nguyên tắc cốt lõi:

> **User Space không bao giờ truy cập trực tiếp Hardware. Tất cả phải qua Kernel.**

### Lý do phân tách:

| Lý do | Giải thích |
| ----- | ---------- |
| **Bảo vệ hệ thống** | Ngăn应用程序不小心或恶意破坏系统 |
| **Trừu tượng hóa** | 应用程序不需要知道硬件细节 |
| **Modular** | Hardware thay đổi, application không cần sửa |
| **Stability** | Một ứng dụng crash không làm crash toàn hệ thống |
| **Security** | Kernel kiểm soát quyền truy cập |

### Protection Levels (Rings):

```
CPU Privilege Rings:
┌─────────────────────────────────────────────────────┐
│  Ring 0 (Highest) │ Kernel Space    │ Full access  │
├─────────────────────────────────────────────────────┤
│  Ring 1            │ (Not typically used in Linux) │
├─────────────────────────────────────────────────────┤
│  Ring 2            │ (Not typically used in Linux) │
├─────────────────────────────────────────────────────┤
│  Ring 3 (Lowest)   │ User Space      │ Limited     │
└─────────────────────────────────────────────────────┘

Linux chỉ sử dụng Ring 0 (kernel) và Ring 3 (user)
```

### Violations:

```c
// ❌ KHÔNG THỂ làm điều này trong User Space
int *ptr = (int *)0x12345678;  // Try to access arbitrary address
*ptr = 100;                     // Segmentation Fault!

// ✅ Phải thông qua Kernel
int fd = open("/dev/mem", O_RDWR);  // Need special permissions
read(fd, buffer, 100);
```

---

# 4. Terminal

## 4.1. Terminal là gì?

**Terminal** (Console) là giao diện để người dùng giao tiếp với Linux thông qua command line.

```
Terminal Components:
┌─────────────────────────────────────────────────────────────┐
│                                                              │
│  ┌───────────────────────────────────────────────────────┐  │
│  │ myuser@hostname:~$ _                                   │  │
│  └───────────────────────────────────────────────────────┘  │
│                                                              │
│  ├── Shell prompt (bash, zsh, sh...)                        │
│  ├── Command input                                          │
│  └── Output display                                         │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

### Prompt Format:

```
username@hostname:current_directory$

Ví dụ:
john@dev-server:~/projects$
root@embedded-device:/#
```

## 4.2. Chế độ nhập liệu

### Canonical Mode (Line Editing):

| Đặc điểm | Mô tả |
| -------- | ----- |
| **Buffer** | Input được buffer theo dòng |
| **Submit** | Nhấn Enter để gửi |
| **Editing** | Backspace, Delete để sửa |
| **History** | Up/Down arrows để xem lịch sử |

```bash
# Terminal mặc định
myuser@server:~$ ls -l
# Nhập "ls -l", Enter để thực thi
```

### Non-Canonical Mode (Raw Mode):

| Đặc điểm | Mô tả |
| -------- | ----- |
| **Immediate** | Mỗi ký tự được gửi ngay lập tức |
| **No buffering** | Không buffer theo dòng |
| **Applications** | top, vim, less, editors |

```bash
# Ví dụ: top command
top
# Hiển thị real-time process list
# Mỗi phím nhấn được xử lý ngay lập tức
```

### Chuyển đổi Mode:

```c
// Canonical mode (default)
struct termios orig_termios;
tcgetattr(STDIN_FILENO, &orig_termios);

// Non-canonical mode
struct termios raw = orig_termios;
raw.c_lflag &= ~(ICANON | ECHO);  // Disable canonical và echo
tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
```

## 4.3. Các Terminal phổ biến

| Terminal | Mô tả | Features |
| -------- | ----- | -------- |
| **GNOME Terminal** | Mặc định GNOME | GUI tabs, profiles |
| **Konsole** | KDE default | Powerful, configurable |
| **xterm** | Classic X terminal | Lightweight, basic |
| **alacritty** | GPU accelerated | Fast, minimal |
| **tmux** | Terminal multiplexer | Split panes, sessions |
| **screen** | Terminal multiplexer | Legacy, stable |
| **ssh** | Remote terminal | Network connection |

```bash
# SSH remote connection
ssh user@hostname
ssh -p 2222 user@hostname

# Screen session
screen -S mysession    # Tạo session
screen -r mysession   # Resume session
screen -ls            # Liệt kê sessions

# Tmux session
tmux new -s mysession
tmux attach -t mysession
```

---

# 5. Shell

## 5.1. Shell là gì?

**Shell** là chương trình command interpreter, đóng vai trò giao diện giữa người dùng và Kernel.

```
Shell as Command Interpreter:
┌─────────────────────────────────────────────────────────────┐
│                      USER                                   │
└─────────────────────┬───────────────────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────────────────┐
│                     SHELL                                   │
│                                                              │
│  1. READ     ← Nhận lệnh từ Terminal                       │
│  2. PARSE    ← Phân tích cú pháp                           │
│  3. EXPAND   ← Xử lý biến, wildcard, redirect             │
│  4. EXECUTE  ← Tìm và chạy chương trình                    │
│  5. OUTPUT   ← Trả kết quả về Terminal                     │
│                                                              │
└─────────────────────┬───────────────────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────────────────┐
│                     KERNEL                                  │
└─────────────────────────────────────────────────────────────┘
```

## 5.2. Quy trình xử lý lệnh

### Ví dụ: `ls -l /home`

```
Command Execution Flow:
┌─────────────────────────────────────────────────────────────┐
│                                                              │
│  1. INPUT          user nhập "ls -l /home"                  │
│                    ↓                                         │
│  2. PARSE          Shell tách thành:                        │
│                    command="ls", args=["-l", "/home"]        │
│                    ↓                                         │
│  3. EXPAND         Xử lý:                                   │
│                    - Wildcards (*, ?)                       │
│                    - Variables ($HOME, $PATH)                │
│                    - Tilde expansion (~)                     │
│                    ↓                                         │
│  4. LOOKUP         Tìm "ls" trong $PATH:                   │
│                    /bin/ls, /usr/bin/ls                     │
│                    ↓                                         │
│  5. FORK           Tạo child process                        │
│                    ↓                                         │
│  6. EXEC           Gọi execve() để thay thế bằng /bin/ls  │
│                    ↓                                         │
│  7. OUTPUT         Kết quả hiển thị lên Terminal            │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

## 5.3. Các loại Shell

| Shell | Path | Mô tả |
| ----- | ---- | ----- |
| **sh** (Bourne Shell) | `/bin/sh` | Original Unix shell |
| **bash** | `/bin/bash` | Bourne Again Shell, mặc định Linux |
| **dash** | `/bin/dash` | Debian Almquist Shell, nhanh hơn |
| **zsh** | `/bin/zsh` | Z Shell, nhiều tính năng |
| **fish** | `/bin/fish` | Friendly Interactive Shell |

```bash
# Kiểm tra shell hiện tại
echo $SHELL
# Output: /bin/bash

# Liệt kê available shells
cat /etc/shells

# Thay đổi shell
chsh -s /bin/zsh
```

### Bash Features:

```bash
# Command History
history              # Xem lịch sử lệnh
!!                  # Lặp lại lệnh trước
!n                  # Lặp lại lệnh thứ n
!string            # Lặp lại lệnh gần nhất bắt đầu bằng string

# Tab Completion
ls -<TAB><TAB>     # Hiển thị options
git <TAB><TAB>     # Command completion

# Aliases
alias ll='ls -la'
alias gs='git status'

# Functions
myfunc() {
    echo "Argument: $1"
}
```

---

# 6. Shell Script

## 6.1. Shell Script là gì?

**Shell Script** là file chứa các lệnh Shell, dùng để tự động hóa các tác vụ lặp đi lặp lại.

### Ứng dụng:

| Ứng dụng | Ví dụ |
| -------- | ----- |
| **System Automation** | Backup, cron jobs |
| **Build Scripts** | Makefile, cross-compilation |
| **Deployment** | Deploy scripts, CI/CD |
| **Monitoring** | System health checks |
| **Embedded Setup** | Board initialization, flashing |

## 6.2. Cấu trúc Script

### Template chuẩn:

```bash
#!/bin/bash
#===============================================================================
# Script Name: example.sh
# Description: Mô tả chức năng script
# Author: Tên tác giả
# Created: 2024-01-01
#===============================================================================

# Configuration
CONFIG_FILE="/path/to/config"

# Function definitions
function check_status() {
    echo "Checking system status..."
}

# Main
main() {
    check_status
}

# Run main
main "$@"
```

### Shebang:

```bash
#!/bin/bash          # Bash script
#!/bin/sh            # POSIX shell (dash)
#!/usr/bin/env bash  # Tìm bash trong $PATH
```

## 6.3. Biến và Argument

### Khai báo biến:

```bash
# Cú pháp
name="value"        # Không có khoảng trắng
age=25

# Sử dụng biến
echo $name
echo "${name}"

# Read-only variable
readonly PI=3.14159

# Unset variable
unset name
```

### Argument:

| Biến | Mô tả |
| ----- | ----- |
| `$0` | Tên script |
| `$1, $2, ...$9` | Argument thứ 1, 2, ... 9 |
| `${10}` | Argument thứ 10 trở lên |
| `$#` | Số lượng arguments |
| `$@` | Tất cả arguments (mảng) |
| `$*` | Tất cả arguments (string) |
| `$$` | PID của script |
| `$?` | Exit status của command trước |

```bash
#!/bin/bash
# script.sh

echo "Script name: $0"
echo "First arg: $1"
echo "Second arg: $2"
echo "All args: $@"
echo "Arg count: $#"

# Duyệt qua tất cả arguments
for arg in "$@"; do
    echo "Arg: $arg"
done
```

### Special Variables:

```bash
$?    # Exit status (0 = success, != 0 = error)
$$    # Current process ID
$!    # PID của background process gần nhất
$-    # Current shell flags
```

## 6.4. Cấu trúc điều khiển

### If-Else:

```bash
# Basic if
if [ "$name" = "admin" ]; then
    echo "Welcome admin"
fi

# If-else
if [ -f "$file" ]; then
    echo "File exists"
else
    echo "File not found"
fi

# Elif
if [ "$status" = "start" ]; then
    echo "Starting..."
elif [ "$status" = "stop" ]; then
    echo "Stopping..."
else
    echo "Unknown status"
fi
```

### Test Conditions:

| Condition | Mô tả |
| --------- | ----- |
| `-f file` | File tồn tại và là regular file |
| `-d dir` | Directory tồn tại |
| `-r file` | File có quyền đọc |
| `-w file` | File có quyền ghi |
| `-x file` | File có quyền thực thi |
| `-z string` | String rỗng |
| `-n string` | String không rỗng |
| `str1 = str2` | Strings bằng nhau |
| `num1 -eq num2` | Numbers bằng nhau |

### Case:

```bash
case "$action" in
    start)
        echo "Starting service..."
        ;;
    stop)
        echo "Stopping service..."
        ;;
    restart)
        echo "Restarting service..."
        ;;
    *)
        echo "Unknown action: $action"
        exit 1
        ;;
esac
```

### For Loop:

```bash
# Loop qua list
for item in apple banana cherry; do
    echo "Item: $item"
done

# Loop qua files
for file in *.txt; do
    echo "Processing: $file"
done

# Loop với số
for i in {1..5}; do
    echo "Count: $i"
done

# C-style for loop
for ((i=0; i<10; i++)); do
    echo "i = $i"
done
```

### While Loop:

```bash
# Read file line by line
while IFS= read -r line; do
    echo "Line: $line"
done < "$filename"

# While with condition
count=0
while [ $count -lt 5 ]; do
    echo "Count: $count"
    count=$((count + 1))
done
```

## 6.5. Hàm

### Định nghĩa và gọi:

```bash
# Function definition
my_function() {
    echo "Hello, $1!"
    return 0
}

# Function with return
add_numbers() {
    local sum=$(( $1 + $2 ))
    return $sum    # Exit status, not value
}

# Gọi function
my_function "World"
result=$?    # Get return value

# Function với return value
add() {
    echo $(( $1 + $2 ))  # Echo value instead of return
}
result=$(add 5 3)
```

### Function với Arguments:

```bash
process_file() {
    local filename="$1"
    local verbose="${2:-false}"

    if [ ! -f "$filename" ]; then
        echo "Error: File not found" >&2
        return 1
    fi

    if [ "$verbose" = "true" ]; then
        echo "Processing: $filename"
    fi

    # Process file...
    return 0
}

# Gọi với arguments
process_file "data.txt" "true"
```

## 6.6. Chạy Script

### Cách 1: Chạy với Interpreter

```bash
bash script.sh
sh script.sh
```

### Cách 2: Thực thi trực tiếp

```bash
chmod +x script.sh    # Thêm quyền thực thi
./script.sh           # Chạy script
```

### Debug Script:

```bash
# Dry run - hiển thị commands mà không chạy
bash -n script.sh

# Debug mode - hiển thị từng command
bash -x script.sh

# Verbose - hiển thị commands trước khi thực thi
bash -v script.sh
```

### Ví dụ Script hoàn chỉnh:

```bash
#!/bin/bash
#===============================================================================
# Script Name: system_info.sh
# Description: Hiển thị thông tin hệ thống
#===============================================================================

set -euo pipefail  # Exit on error, undefined var, pipe failure

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Function definitions
show_usage() {
    echo "Usage: $0 [-h] [-v]"
    echo "  -h    Show this help"
    echo "  -v    Verbose mode"
    exit 1
}

log_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1" >&2
}

get_system_info() {
    log_info "Collecting system information..."

    echo "Hostname: $(hostname)"
    echo "Kernel: $(uname -r)"
    echo "OS: $(uname -o)"
    echo "Uptime: $(uptime -p)"
    echo "CPU: $(nproc) cores"
    echo "Memory: $(free -h | awk '/^Mem:/ {print $2}')"
}

# Parse arguments
VERBOSE=false
while getopts "hv" opt; do
    case $opt in
        h) show_usage ;;
        v) VERBOSE=true ;;
        *) show_usage ;;
    esac
done

# Main
main() {
    get_system_info
    log_info "Done!"
}

main
```

---

# 7. Biến môi trường

## 7.1. Khái niệm

**Environment Variables** (Biến môi trường) là các biến được hệ điều hành sử dụng để lưu trữ thông tin cấu hình hệ thống.

```
Environment Variable Format:
KEY=VALUE
```

### Ví dụ:

```bash
# Xem một biến cụ thể
echo $PATH
# Output: /usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

# Xem tất cả biến môi trường
printenv

# Xem biến môi trường
env
```

## 7.2. Biến thường vs Biến môi trường

### So sánh:

| Loại | Khai báo | Phạm vi | Inheritance |
| ---- | -------- | ------- | ---------- |
| **Shell Variable** | `VAR=value` | Chỉ shell hiện tại | Không |
| **Environment Variable** | `export VAR=value` | Shell + child processes | Có |

### Ví dụ:

```bash
# Shell variable (chỉ trong shell hiện tại)
MY_VAR="hello"
echo $MY_VAR          # OK
bash -c 'echo $MY_VAR'  # Empty (không kế thừa)

# Environment variable (kế thừa cho child processes)
export MY_VAR="hello"
bash -c 'echo $MY_VAR'  # "hello" (kế thừa)
```

### Export:

```bash
# Cách 1: export khi khai báo
export VAR="value"

# Cách 2: export sau khi khai báo
VAR="value"
export VAR

# Cách 3: export với readonly
export PATH=/opt/bin:$PATH

# Xóa biến môi trường
unset VAR
```

## 7.3. Các biến quan trọng

### Hệ thống:

| Biến | Mô tả | Ví dụ |
| ---- | ----- | ----- |
| `PATH` | Danh sách thư mục tìm command | `/usr/local/sbin:/usr/bin` |
| `HOME` | Thư mục home của user | `/home/john` |
| `USER` | Tên user hiện tại | `john` |
| `SHELL` | Shell mặc định | `/bin/bash` |
| `PWD` | Thư mục hiện tại | `/home/john/projects` |
| `HOSTNAME` | Tên máy | `dev-server` |
| `LANG` | Ngôn ngữ và locale | `en_US.UTF-8` |

### Thường dùng:

| Biến | Mô tả | Ví dụ |
| ---- | ----- | ----- |
| `EDITOR` | Editor mặc định | `vim` |
| `PAGER` | Pager mặc định | `less` |
| `TMPDIR` | Thư mục tạm | `/tmp` |
| `LD_LIBRARY_PATH` | Library search path | `/opt/lib` |
| `CFLAGS` | C compiler flags | `-O2 -Wall` |

### Embedded Linux:

```bash
# Cross-compilation
export CROSS_COMPILE=arm-linux-gnueabihf-
export CC=${CROSS_COMPILE}gcc
export ARCH=arm

# Buildroot
export BR2_JLEVEL=4
export BR2_DL_DIR=/opt/buildroot/download
```

## 7.4. Lưu ý bảo mật

### Không nên lưu:

```bash
# ❌ KHÔNG BAO GIỜ lưu trong environment variable
export PASSWORD="my_secret_password"
export API_KEY="sk-1234567890abcdef"
export PRIVATE_KEY="-----BEGIN RSA KEY-----"
```

### Lưu trữ an toàn:

```bash
# ✅ Sử dụng .env files với quyền hạn chế
chmod 600 .env

# ✅ Sử dụng secrets management
# - HashiCorp Vault
# - AWS Secrets Manager
# - Kubernetes Secrets

# ✅ Mã hóa sensitive data
gpg --encrypt secrets.txt
```

### Tránh lỗi thường gặp:

```bash
# ❌ Ghi đè PATH hoàn toàn
PATH="/opt/bin"    # Mất các path mặc định!

# ✅ Append vào PATH
PATH="/opt/bin:${PATH}"

# ❌ Quên quote
VAR="Hello World"
echo $VAR    # Lỗi! World được tách thành argument

# ✅ Luôn dùng quotes
echo "${VAR}"    # Đúng
echo "$VAR"      # Cũng đúng trong hầu hết trường hợp
```

---

# 8. User & Group

## 8.1. Khái niệm User

**User** là tài khoản người dùng trong hệ thống Linux, dùng để:
- Xác định quyền truy cập
- Phân biệt dữ liệu giữa người dùng
- Theo dõi hoạt động (audit)

### User Information:

| Thành phần | Mô tả |
| ---------- | ----- |
| **Username** | Tên đăng nhập (unique) |
| **UID** | User ID (unique, 0-65535) |
| **GID** | Primary Group ID |
| **Home** | Thư mục home |
| **Shell** | Default shell |
| **Password** | Mã hóa trong `/etc/shadow` |

### User Types:

| UID | User | Mô tả |
| --- | ---- | ----- |
| 0 | root | Superuser, toàn quyền |
| 1-999 | system | System users (daemons) |
| 1000+ | regular | Regular users |

## 8.2. Khái niệm Group

**Group** là tập hợp người dùng, dùng để chia sẻ quyền truy cập tài nguyên.

### Group File:

```
/etc/group format:
group_name:password:GID:member1,member2,member3

Ví dụ:
sudo:x:27:john,alice
www-data:x:33:www-data
```

### Ví dụ:

```bash
# Primary group - Mỗi user có một primary group
# Supplementary groups - User có thể thuộc nhiều group
```

## 8.3. Các lệnh quản lý User

### Xem thông tin:

```bash
# User hiện tại
whoami

# UID của user
id

# UID cụ thể
id -u username

# Thông tin user trong /etc/passwd
cat /etc/passwd | grep username
# username:x:1000:1000:Full Name:/home/username:/bin/bash
```

### Tạo User:

```bash
# Tạo user đơn giản
sudo adduser username

# Tạo user với options
sudo useradd -m \
    -s /bin/bash \
    -c "Full Name" \
    -G sudo,www-data \
    username

# Options:
# -m: Tạo home directory
# -s: Shell mặc định
# -c: Comment (full name)
# -G: Secondary groups
# -u: UID cụ thể
```

### Xóa User:

```bash
# Xóa user (giữ home directory)
sudo userdel username

# Xóa user và home directory
sudo userdel -r username
```

### Sửa đổi User:

```bash
# Đổi password
sudo passwd username

# Thêm vào group
sudo usermod -aG groupname username

# Thay đổi shell
sudo usermod -s /bin/zsh username

# Thay đổi home directory
sudo usermod -d /new/home -m username
```

## 8.4. Các lệnh quản lý Group

### Tạo Group:

```bash
# Tạo group mới
sudo groupadd groupname

# Tạo group với GID cụ thể
sudo groupadd -g 1500 groupname
```

### Xóa Group:

```bash
# Xóa group
sudo groupdel groupname

# Xóa group khỏi user
sudo gpasswd -d username groupname
```

### Quản lý Group Members:

```bash
# Thêm user vào group
sudo usermod -aG groupname username

# Kiểm tra group membership
groups username

# Xem tất cả groups của user hiện tại
id
```

---

# 9. Root & Sudo

## 9.1. Root User

**Root** (UID = 0) là superuser có toàn quyền trên hệ thống Linux.

### Đặc điểm:

| Đặc điểm | Mô tả |
| -------- | ----- |
| **UID** | 0 |
| **Home** | `/root` |
| **Shell** | `/bin/bash` |
| **Quyền** | Toàn quyền hệ thống |

### Root có thể làm gì:

```
Root Permissions:
├── User management       → Tạo, xóa, sửa any user
├── File permissions      → Sở hữu, chmod any file
├── System services       → Start, stop, configure any service
├── Kernel modules        → Load, unload modules
├── Network configuration  → Thay đổi IP, routing
├── Software install      → apt install, dpkg
└── Critical operations   → Shutdown, reboot, fdisk
```

### Nguy hiểm khi dùng Root:

```bash
# ⚠️ CÁC LỆNH NGUY HIỂM - KHÔNG CHẠY!
sudo rm -rf /          # Xóa toàn bộ hệ thống!
sudo dd if=/dev/zero of=/dev/sda  # Xóa ổ cứng!
sudo mkfs.ext4 /dev/sda1          # Format ổ cứng!
```

## 9.2. Sudo

**sudo** (superuser do) cho phép regular user thực thi commands với quyền root.

### Cú pháp:

```bash
sudo <command>

# Ví dụ:
sudo apt update
sudo systemctl restart nginx
sudo whoami    # Output: root
```

### Configuration:

```bash
# Mở file sudoers để chỉnh sửa
sudo visudo

# Ví dụ cấu hình sudoers:
# user    ALL=(ALL:ALL) ALL
# %admin  ALL=(ALL) ALL
# user    ALL=(ALL) NOPASSWD: /usr/bin/systemctl restart
```

### Sudoers Options:

| Option | Mô tả |
| ------ | ----- |
| `ALL=(ALL:ALL) ALL` | User có thể chạy bất kỳ command nào |
| `NOPASSWD` | Không cần password |
| `ONLY=(user)` | Chỉ có thể impersonate user cụ thể |
| `TAG` | No logging, etc. |

## 9.3. Phân quyền an toàn

### Best Practices:

| Practice | Mô tả |
| -------- | ----- |
| **Không dùng root trực tiếp** | Sử dụng sudo với user thường |
| **Principle of Least Privilege** | Chỉ cấp quyền cần thiết |
| **sudoers thay vì chmod 777** | Kiểm soát truy cập tốt hơn |
| **Audit sudo usage** | Log và theo dõi |
| **Password policies** | Strong passwords, expiration |

### Disable Root Login:

```bash
# Khóa root account (vẫn dùng được sudo)
sudo passwd -l root

# Cho phép root login lại (nếu cần)
sudo passwd -u root
```

### File Permissions:

```bash
# Quan trọng:
chmod 600 /etc/shadow       # Chỉ root đọc/ghi password hashes
chmod 644 /etc/passwd       # Đọc được bởi tất cả
chmod 700 ~/.ssh            # Chỉ user đọc SSH keys
chmod 644 ~/.ssh/authorized_keys
```

### SSH Key-based Access:

```bash
# Tạo SSH key
ssh-keygen -t ed25519 -C "user@hostname"

# Copy public key lên server
ssh-copy-id user@server

# Sau đó có thể SSH không cần password
ssh user@server
```

---

## 📚 Tài liệu tham khảo

- *Linux Programming Interface* - Michael Kerrisk
- *The Linux Command Line* - William Shotts
- *Advanced Bash-Scripting Guide* - Mendel Cooper
- Linux man pages: `man bash`, `man sh`, `man sudo`
- [GNU Bash Documentation](https://www.gnu.org/software/bash/manual/)
- [Linux Kernel Documentation](https://www.kernel.org/doc/html/latest/)
