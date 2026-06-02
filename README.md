<p align="center">
  <img src="https://img.shields.io/badge/status-active-success?style=flat-square" alt="Status">
  <img src="https://img.shields.io/badge/focus-Embedded%20Linux-blue?style=flat-square" alt="Focus">
  <img src="https://img.shields.io/badge/exercises-68-green?style=flat-square" alt="68 Exercises">
  <img src="https://img.shields.io/badge/license-MIT-lightgrey?style=flat-square" alt="License">
</p>

# Embedded-Linux-Zero-to-Hire

A structured Embedded Linux curriculum for Freshers and Juniors pursuing careers in **IoT**, **Automotive**, **Firmware**, and **Embedded Systems**.

Built from real industry job requirements, this repository combines **theory notes** with **hands-on C programming exercises** — each with skeleton code, solution, and build setup — to bridge the gap between learning and being hire-ready.

---

## Table of Contents

- [Curriculum Overview](#curriculum-overview)
- [How to Use](#how-to-use)
- [Prerequisites](#prerequisites)
- [Exercise Format](#exercise-format)
- [Hardware Setup](#hardware-setup)
- [Contributing](#contributing)
- [License](#license)

---

## Curriculum Overview

The program is organized into **15 modules** progressing from Linux foundations through system programming, inter-process communication, and embedded-specific topics. Each module includes detailed theory notes and practical exercises.

| # | Module | Exercises | Topics |
|:-:|--------|:---------:|--------|
| 00 | Linux Basic Knowledge | 5 | OS architecture, Shell, Shell Script, Env Variables, User/Group Management |
| 01 | General Knowledge | 7 | Makefile (basic → multi-file → static lib → shared lib → advanced → functions → conditional build) |
| **02** | **Debugging Tools** | **4** | **GDB basic → GDB advanced → core dump → AddressSanitizer** |
| 03 | Linux File System | 7 | Inode stats → file ops → file monitoring → IO redirection → directory ops → file locking → mmap |
| 04 | Linux Process | 7 | Process info → memory layout → fork/exec → process mgmt → process groups → daemon → resource usage |
| 05 | Linux Thread | 7 | Thread creation → detach/cancel → args/return → race condition → condvar → rwlock → barrier |
| 06 | IPC Signal | 5 | SIGINT → sigprocmask → sigaction → kill/sigqueue → SIGCHLD |
| 07 | IPC Socket | 5 | TCP → UDP → UNIX domain → select → epoll |
| 08 | IPC Message Queues | 4 | System V → POSIX → message priority → mq_notify |
| 09 | IPC Pipes & FIFO | 6 | Anonymous pipe → named FIFO → popen → shell pipe redirection → client-server → non-blocking |
| 10 | IPC Shared Memory | 5 | System V → POSIX → anonymous → SHM+semaphore → performance benchmark |
| 11 | IPC Semaphore | 5 | POSIX unnamed → POSIX named → System V → producer-consumer → dining philosophers |
| **12** | **Cross-compilation & Toolchain** | **6** | **Setup toolchain → hello world → static lib → sysroot → CMake → remote debugging** |
| **13** | **Linux Kernel Module** | **6** | **Hello module → char device → platform driver → procfs/sysfs → kthread/timer → GPIO driver** |
| **14** | **Yocto Project** | **5** | **Setup build → custom layer → custom recipe → kernel module recipe → custom rootfs + systemd** |

---

## How to Use

### 1. Study the Theory

Each module directory contains a markdown file (e.g., `03.linux-file-system/linux-file-system.md`) covering concepts, system call APIs, diagrams, and code examples.

### 2. Practice with Exercises

Navigate to the `Exercises/` subdirectory within each module:

```bash
# Pick an exercise and read the description
cd "03.linux-file-system/Exercises/exercise-01-inode-stats"
cat README.md

# Write your solution, build and test
# (create file_info.c and Makefile based on the README)
make
./file_info

# Compare with the reference solution
diff solution/file_info.c file_info.c
```

### 3. Follow the Progression

Exercises within each module are ordered by difficulty:

| Level | Description |
|:-----:|-------------|
| ⭐ | Foundational — single concept, basic syntax |
| ⭐⭐ | Intermediate — combining multiple concepts |
| ⭐⭐⭐ | Advanced — multi-file, real-world scenarios |

---

## Prerequisites

| Skill | Required | Notes |
|-------|:--------:|-------|
| **C Programming** | ✅ | Variables, functions, pointers, structs |
| **Linux Command Line** | Recommended | Basic familiarity with the terminal |
| **Git** | Recommended | For cloning and tracking progress |

---

## Exercise Format

```
exercise-NN-name/
├── README.md              # Description with objectives, requirements, hints, test steps
├── main.c                 # Skeleton code with TODO markers for students
├── Makefile               # Build setup
└── solution/
    ├── main.c             # Complete reference implementation
    └── Makefile           # Builds with -Wall -Wextra -std=c11
```

Solutions compile cleanly with zero warnings:

```bash
gcc -Wall -Wextra -std=c11 -o target source.c
```

---

## Hardware Setup

The course supports practical exercises on actual hardware:

| Component | Purpose |
|-----------|---------|
| **Raspberry Pi Zero** | Main Linux board for hands-on practice |
| **SSD1306 OLED (I2C/SPI)** | Display interfacing via Linux GPIO |
| **CP2102 USB-to-UART** | Serial debug and communication |

See [docs/00-enviroment-setup/](docs/00-enviroment-setup/) for the full hardware list and setup guide.

---

## Contributing

Contributions are welcome — open an issue or submit a pull request to improve theory content, add exercises, or fix bugs.

---

## License

This project is distributed under the **MIT License**.
