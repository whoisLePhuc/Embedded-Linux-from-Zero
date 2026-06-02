# Embedded Linux - Cross-compilation & Toolchain

> Tài liệu về cross-compilation: toolchain, sysroot, CMake/autotools cross-build, remote debugging

---

## Table of Contents

### [1. Cross-compilation Concepts](#1-cross-compilation-concepts)
- [1.1. Native vs Cross Compilation](#11-native-vs-cross-compilation)
- [1.2. The Toolchain Triplet](#12-the-toolchain-triplet)
- [1.3. When Cross-compilation is Needed](#13-when-cross-compilation-is-needed)

### [2. Toolchain Setup](#2-toolchain-setup)
- [2.1. Ubuntu Pre-built Packages](#21-ubuntu-pre-built-packages)
- [2.2. Linaro Toolchain](#22-linaro-toolchain)
- [2.3. crosstool-NG](#23-crosstool-ng)
- [2.4. Toolchain Verification](#24-toolchain-verification)

### [3. Cross-compilation in Practice](#3-cross-compilation-in-practice)
- [3.1. Hello World](#31-hello-world)
- [3.2. Static Library](#32-static-library)
- [3.3. Shared Library](#33-shared-library)
- [3.4. Sysroot Concept](#34-sysroot-concept)
- [3.5. Using pkg-config](#35-using-pkg-config)

### [4. Build Systems for Cross-compilation](#4-build-systems-for-cross-compilation)
- [4.1. Manual Makefile](#41-manual-makefile)
- [4.2. Autotools (./configure)](#42-autotools-configure)
- [4.3. CMake Toolchain File](#43-cmake-toolchain-file)
- [4.4. Meson Cross File](#44-meson-cross-file)

### [5. QEMU User-mode for Testing](#5-qemu-user-mode-for-testing)
- [5.1. Installing QEMU](#51-installing-qemu)
- [5.2. binfmt_misc Setup](#52-binfmt_misc-setup)
- [5.3. Docker-based Approach](#53-docker-based-approach)
- [5.4. Running ARM Binaries](#54-running-arm-binaries)

### [6. Remote Debugging](#6-remote-debugging)
- [6.1. gdbserver Setup](#61-gdbserver-setup)
- [6.2. Cross-GDB Connection](#62-cross-gdb-connection)
- [6.3. Debug with Core Dump](#63-debug-with-core-dump)

### [7. Real-world Project Structure](#7-real-world-project-structure)

---

## 1. Cross-compilation Concepts

### 1.1. Native vs Cross Compilation

**Native compilation:** Building code on the same architecture it will run on (x86-64 → x86-64).

**Cross compilation:** Building code on a host system for a different target architecture (x86-64 → ARM).

```
Native:     gcc -o app app.c          (x86-64 → x86-64)
Cross:      arm-linux-gnueabihf-gcc -o app app.c  (x86-64 → ARM)
```

### Why cross-compilation in Embedded Linux?

- **Performance:** Building a Linux kernel or Yocto image on ARM is 10-50x slower than on x86-64
- **Resources:** Embedded boards have limited RAM (256MB-2GB vs 16-64GB on host)
- **Tool availability:** Many development tools only run on x86-64
- **CI/CD:** Automated builds must be fast

### 1.2. The Toolchain Triplet

Every cross-compiler targets a specific architecture defined by a **triplet**:

```
<arch>-<vendor>-<os>-<abi>
```

| Triplet | Architecture | OS | ABI |
|---------|-------------|----|-----|
| `arm-linux-gnueabihf` | ARM 32-bit | Linux | hard-float |
| `arm-linux-gnueabi` | ARM 32-bit | Linux | soft-float |
| `aarch64-linux-gnu` | ARM 64-bit | Linux | standard |
| `x86_64-linux-gnu` | x86-64 | Linux | standard |

**ARM-specific ABIs:**
- **gnueabi**: Soft-float (emulates FPU in software) — older ARM CPUs without FPU
- **gnueabihf**: Hard-float (uses hardware FPU) — ARMv7-A with VFP/NEON
- **ARM 64-bit**: Always uses hardware FPU, no ABI distinction

### 1.3. When Cross-compilation is Needed

```
Development Workflow:

┌──────────────────┐         ┌──────────────────┐
│   HOST (x86-64)  │         │ TARGET (ARM)     │
│                  │         │                  │
│  arm-gcc compile │ ──────→ │  Receive binary  │
│  arm-gdb debug   │ ←────── │  gdbserver       │
│  QEMU run/test   │ ──────→ │  (or QEMU)       │
└──────────────────┘         └──────────────────┘
```

Typical applications:
- Building bootloader (U-Boot)
- Building Linux kernel + modules
- Building user-space applications
- Building root filesystem with Buildroot/Yocto

---

## 2. Toolchain Setup

### 2.1. Ubuntu Pre-built Packages

Ubuntu provides ready-to-use cross-compilers for ARM:

```bash
# Install ARM 32-bit (hard-float)
sudo apt install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf

# Install ARM 64-bit
sudo apt install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu

# Verify
arm-linux-gnueabihf-gcc --version
# arm-linux-gnueabihf-gcc (Ubuntu 13.2.0-7ubuntu1) 13.2.0

aarch64-linux-gnu-gcc --version
```

Toolchain location:
```
/usr/bin/arm-linux-gnueabihf-*     # All tools
/usr/arm-linux-gnueabihf/          # Sysroot
/usr/lib/gcc-cross/arm-linux-gnueabihf/  # Internal files
```

**Complete tool list:**
```bash
ls /usr/bin/arm-linux-gnueabihf-*
# arm-linux-gnueabihf-addr2line  arm-linux-gnueabihf-g++
# arm-linux-gnueabihf-ar         arm-linux-gnueabihf-gcc
# arm-linux-gnueabihf-as         arm-linux-gnueabihf-gcc-13
# arm-linux-gnueabihf-c++        arm-linux-gnueabihf-gcc-nm
# arm-linux-gnueabihf-cpp        arm-linux-gnueabihf-gcc-ranlib
# arm-linux-gnueabihf-dwp        arm-linux-gnueabihf-gcov
# arm-linux-gnueabihf-elfedit    arm-linux-gnueabihf-gcov-dump
# arm-linux-gnueabihf-g++        arm-linux-gnueabihf-gcov-tool
# arm-linux-gnueabihf-gcc        arm-linux-gnueabihf-gprof
# arm-linux-gnueabihf-gcc-ar     arm-linux-gnueabihf-ld
# arm-linux-gnueabihf-gcc-nm     arm-linux-gnueabihf-ld.bfd
# arm-linux-gnueabihf-gcc-ranlib arm-linux-gnueabihf-ld.gold
# arm-linux-gnueabihf-gcov       arm-linux-gnueabihf-nm
# arm-linux-gnueabihf-gcov-dump  arm-linux-gnueabihf-objcopy
# arm-linux-gnueabihf-gcov-tool  arm-linux-gnueabihf-objdump
# arm-linux-gnueabihf-gprof      arm-linux-gnueabihf-ranlib
# arm-linux-gnueabihf-ld         arm-linux-gnueabihf-readelf
# arm-linux-gnueabihf-ld.bfd     arm-linux-gnueabihf-size
# arm-linux-gnueabihf-ld.gold    arm-linux-gnueabihf-strings
# arm-linux-gnueabihf-nm         arm-linux-gnueabihf-strip
# arm-linux-gnueabihf-objcopy    arm-linux-gnueabihf-c++filt
# arm-linux-gnueabihf-objdump    arm-linux-gnueabihf-c++filt
# arm-linux-gnueabihf-ranlib
```

### 2.2. Linaro Toolchain

Linaro provides pre-built toolchains optimized for ARM:

```bash
# Download Linaro ARM GCC 13.x
wget https://releases.linaro.org/components/toolchain/binaries/latest-7/arm-linux-gnueabihf/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf.tar.xz

tar -xf gcc-linaro-*.tar.xz
export PATH=$PATH:$(pwd)/gcc-linaro-*/bin
```

For newer versions, use ARM's official site:
```bash
# ARM GNU Toolchain (official)
wget https://developer.arm.com/-/media/Files/downloads/gnu/13.3.rel1/binrel/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-linux-gnueabihf.tar.xz

tar -xf arm-gnu-toolchain-*.tar.xz
```

### 2.3. crosstool-NG

crosstool-NG lets you build a custom toolchain with exact versions of GCC, binutils, and C library.

**Installation:**
```bash
git clone https://github.com/crosstool-ng/crosstool-ng
cd crosstool-ng
./bootstrap
./configure --enable-local
make -j$(nproc)
```

**Build a toolchain:**
```bash
# List available configurations
./ct-ng list-samples

# Select ARM Cortex-A7 hard-float
./ct-ng arm-cortex_a7-linux-gnueabihf

# Customize (optional)
./ct-ng menuconfig

# Build (takes 30-60 minutes)
./ct-ng build

# Toolchain location
# ~/x-tools/arm-cortex_a7-linux-gnueabihf/
```

**Key crosstool-NG options:**
- Target architecture: arm, aarch64, x86_64, mips, riscv64
- C library: glibc, musl, uClibc-ng, newlib
- GCC version: 12.x, 13.x, 14.x
- binutils version
- Kernel headers version

### 2.4. Toolchain Verification

Always verify your toolchain after installation:

```bash
# Check architecture
arm-linux-gnueabihf-gcc -dumpmachine
# arm-linux-gnueabihf

# Check sysroot
arm-linux-gnueabihf-gcc -print-sysroot
# /usr/arm-linux-gnueabihf

# Test compile
echo 'int main(){return 0;}' | arm-linux-gnueabihf-gcc -x c - -o /tmp/test
file /tmp/test
# /tmp/test: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, ...

# Check libraries available in sysroot
ls $(arm-linux-gnueabihf-gcc -print-sysroot)/lib/arm-linux-gnueabihf/
```

---

## 3. Cross-compilation in Practice

### 3.1. Hello World

```c
// hello.c
#include <stdio.h>

int main(void) {
    printf("Hello from ARM!\n");
    printf("Compiled on: %s\n", __DATE__);
    return 0;
}
```

```bash
arm-linux-gnueabihf-gcc -o hello hello.c

file hello
# hello: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux-armhf.so.3

# Test with QEMU
qemu-arm-static ./hello
# Hello from ARM!
```

### 3.2. Static Library

```c
// math_ops.c
int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }
```

```c
// main.c
#include <stdio.h>
int add(int, int);
int main() { printf("3 + 4 = %d\n", add(3, 4)); return 0; }
```

```bash
arm-linux-gnueabihf-gcc -c -o math_ops.o math_ops.c
arm-linux-gnueabihf-ar rcs libmath.a math_ops.o

arm-linux-gnueabihf-gcc -o main main.c -L. -lmath

file main
# main: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked...
```

### 3.3. Shared Library

```bash
arm-linux-gnueabihf-gcc -fPIC -c -o math_ops.o math_ops.c
arm-linux-gnueabihf-gcc -shared -o libmath.so math_ops.o

arm-linux-gnueabihf-gcc -o main main.c -L. -lmath -Wl,-rpath,.
```

### 3.4. Sysroot Concept

The **sysroot** is the target root filesystem used by the compiler to find headers and libraries during cross-compilation.

```
Sysroot Directory Structure:
/usr/arm-linux-gnueabihf/           # Ubuntu package sysroot
├── lib/                            # Target libraries
│   └── arm-linux-gnueabihf/
│       ├── libc.so.6
│       ├── libm.so.6
│       └── libpthread.so.0
├── usr/
│   ├── include/                    # Target headers
│   │   ├── stdio.h
│   │   └── pthread.h
│   └── lib/
│       └── arm-linux-gnueabihf/    # Additional target libs
└── etc/
```

**Using sysroot explicitly:**
```bash
# The compiler automatically uses its built-in sysroot
arm-linux-gnueabihf-gcc -print-sysroot
# /usr/arm-linux-gnueabihf

# But you can override it
arm-linux-gnueabihf-gcc --sysroot=/path/to/custom/sysroot -o app app.c
```

**Building a custom sysroot with external libraries:**
```bash
# Install zlib-dev for target
sudo apt install libz-dev:armhf

# Now the library is available in sysroot
arm-linux-gnueabihf-gcc -o app app.c -lz
```

### 3.5. Using pkg-config

When cross-compiling, pkg-config must be told where to find .pc files:

```bash
# Set for cross-compilation
export PKG_CONFIG_LIBDIR=/usr/arm-linux-gnueabihf/lib/pkgconfig
export PKG_CONFIG_SYSROOT_DIR=/usr/arm-linux-gnueabihf

# Now pkg-config returns target libraries
pkg-config --cflags --libs libgpiod
# -I/usr/arm-linux-gnueabihf/usr/include -lgpiod
```

---

## 4. Build Systems for Cross-compilation

### 4.1. Manual Makefile

```makefile
# Makefile for cross-compilation
CROSS_COMPILE ?= arm-linux-gnueabihf-

CC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar
STRIP = $(CROSS_COMPILE)strip

CFLAGS = -Wall -O2
LDFLAGS =

TARGET = myapp
SRCS = main.c utils.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^
	$(STRIP) $@

clean:
	rm -f $(OBJS) $(TARGET)
```

Usage:
```bash
# Cross-compile for ARM
make CROSS_COMPILE=arm-linux-gnueabihf-

# Native compile (default)
make
```

### 4.2. Autotools (./configure)

For projects using autotools (GNU style):

```bash
# For ARM 32-bit
./configure --host=arm-linux-gnueabihf --prefix=/usr

# For ARM 64-bit
./configure --host=aarch64-linux-gnu --prefix=/usr

# Install to staging directory
make -j$(nproc)
make install DESTDIR=/path/to/staging
```

**With explicit tools:**
```bash
export CC=arm-linux-gnueabihf-gcc
export CXX=arm-linux-gnueabihf-g++
export AR=arm-linux-gnueabihf-ar
export RANLIB=arm-linux-gnueabihf-ranlib

./configure --host=arm-linux-gnueabihf --prefix=/usr
make -j$(nproc)
```

### 4.3. CMake Toolchain File

Create `arm-cross.cmake`:

```cmake
# Target system
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Cross-compiler
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

# Sysroot
set(CMAKE_SYSROOT /usr/arm-linux-gnueabihf)
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})

# Find programs on host, libs/headers on target
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Architecture-specific flags
set(CMAKE_C_FLAGS_INIT "-march=armv7-a -mfpu=neon -mfloat-abi=hard")
set(CMAKE_CXX_FLAGS_INIT "-march=armv7-a -mfpu=neon -mfloat-abi=hard")
```

Usage:
```bash
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../arm-cross.cmake ..
make
```

### 4.4. Meson Cross File

Create `arm-cross.txt`:

```ini
[binaries]
c = 'arm-linux-gnueabihf-gcc'
cpp = 'arm-linux-gnueabihf-g++'
ar = 'arm-linux-gnueabihf-ar'
strip = 'arm-linux-gnueabihf-strip'
pkgconfig = 'arm-linux-gnueabihf-pkg-config'

[host_machine]
system = 'linux'
cpu_family = 'arm'
cpu = 'armv7hf'
endian = 'little'

[properties]
c_args = ['-march=armv7-a', '-mfpu=neon', '-mfloat-abi=hard']
c_link_args = []
```

Usage:
```bash
meson setup build --cross-file arm-cross.txt
ninja -C build
```

---

## 5. QEMU User-mode for Testing

QEMU user-mode allows running ARM binaries directly on x86-64 without emulating a full system.

### 5.1. Installing QEMU

```bash
sudo apt install qemu-user-static qemu-user
```

### 5.2. binfmt_misc Setup

The Linux kernel can automatically invoke QEMU based on ELF binary magic bytes.

**Method 1: systemd-binfmt (persistent)**
```bash
# Install QEMU static binaries (also registers binfmt handlers)
sudo apt install qemu-user-static

# Verify
cat /proc/sys/fs/binfmt_misc/qemu-arm
# enabled
```

**Method 2: Manual registration**
```bash
# Register ARM interpreter
sudo sh -c 'echo ":arm:M::\\x7fELF\\x01\\x01\\x01\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x00\\x02\\x00\\x28\\x00:\\xff\\xff\\xff\\xff\\xff\\xff\\xff\\x00\\xff\\xff\\xff\\xff\\xff\\xff\\xff\\xff\\xfe\\xff\\xff\\xff:/usr/bin/qemu-arm-static:F" > /proc/sys/fs/binfmt_misc/register'
```

### 5.3. Docker-based Approach

The simplest approach for running ARM in a consistent environment:

```bash
# Register QEMU binfmt handlers for all architectures
docker run --rm --privileged multiarch/qemu-user-static --reset -p yes

# Run an ARM container
docker run --rm -it arm32v7/ubuntu bash

# Inside the container — it's ARM!
root@...:# uname -m
# armv7l

root@...:# gcc --version
root@...:# ./my_arm_binary  # Works natively
```

### 5.4. Running ARM Binaries

```bash
# Method 1: Direct (with binfmt_misc)
./hello
# Hello from ARM!

# Method 2: Explicit QEMU
qemu-arm-static ./hello

# Method 3: With library path
QEMU_LD_PREFIX=/usr/arm-linux-gnueabihf qemu-arm-static ./hello

# Method 4: Debug with GDB stub
qemu-arm-static -g 1234 ./hello
# Then connect with gdb-multiarch: target remote :1234
```

---

## 6. Remote Debugging

### 6.1. gdbserver Setup

**On target (ARM device running Linux):**
```bash
# Start gdbserver for an existing binary
gdbserver :3333 ./myapp

# Or attach to a running process
gdbserver --attach :3333 $(pidof myapp)

# Multi-process mode
gdbserver --multi :3333
```

**On host (compile debug binary):**
```bash
arm-linux-gnueabihf-gcc -g -O0 -o myapp_debug myapp.c
```

### 6.2. Cross-GDB Connection

**On host:**
```bash
# Install cross-GDB
sudo apt install gdb-multiarch

# Or use toolchain's GDB
arm-linux-gnueabihf-gdb ./myapp_debug

# Inside GDB:
(gdb) set sysroot /usr/arm-linux-gnueabihf
(gdb) target remote <target-ip>:3333
(gdb) break main
(gdb) continue
(gdb) next
(gdb) print variable_name
(gdb) backtrace
```

**GDB commands for remote debugging:**
```
(gdb) info registers        # View all registers
(gdb) disassemble           # Disassemble current function
(gdb) list                  # Show source code
(gdb) break file.c:42       # Set breakpoint
(gdb) watch variable        # Watch variable changes
(gdb) continue              # Continue execution
(gdb) stepi                 # Step one instruction
(gdb) x/10x $sp             # Examine memory
```

### 6.3. Debug with Core Dump

```bash
# On target, enable core dumps
ulimit -c unlimited
echo "/tmp/core.%p" > /proc/sys/kernel/core_pattern

# Run program until it crashes
./myapp
# Segmentation fault (core dumped)

# On host, analyze core dump
arm-linux-gnueabihf-gdb ./myapp_debug /tmp/core.1234

(gdb) bt                    # Backtrace
(gdb) frame 3               # Select frame
(gdb) info locals           # Local variables
(gdb) print *ptr            # Print value
```

---

## 7. Real-world Project Structure

```
embedded-project/
├── Makefile                    # Top-level build
├── toolchain.cmake             # CMake toolchain (optional)
├── cross-arm.env               # Cross-compilation environment
├── src/                        # Source code
│   ├── main.c
│   ├── sensor.c
│   └── sensor.h
├── lib/                        # External libraries
│   └── libfoo/                 # Built for target
├── output/                     # Build artifacts
│   ├── arm/                    # ARM binaries
│   └── native/                 # Native binaries (for host tools)
├── staging/                    # Custom sysroot
│   ├── include/
│   └── lib/
└── deploy/                     # Files to copy to target
    ├── myapp
    └── scripts/
```

**cross-arm.env — source this to set up environment:**
```bash
#!/bin/bash
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-
export CC=${CROSS_COMPILE}gcc
export CXX=${CROSS_COMPILE}g++
export AR=${CROSS_COMPILE}ar
export STRIP=${CROSS_COMPILE}strip
export SYSROOT=$(arm-linux-gnueabihf-gcc -print-sysroot)
export CFLAGS="--sysroot=$SYSROOT -march=armv7-a -mfpu=vfpv3"
export LDFLAGS="--sysroot=$SYSROOT"
export PKG_CONFIG_LIBDIR=$SYSROOT/usr/lib/arm-linux-gnueabihf/pkgconfig
export PKG_CONFIG_SYSROOT_DIR=$SYSROOT
echo "ARM cross-compilation environment loaded"
```

---

## 📚 References

- [ARM GNU Toolchain Downloads](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain)
- [crosstool-NG](https://crosstool-ng.github.io/)
- [QEMU User Mode](https://www.qemu.org/docs/master/user/main.html)
- [CMake Toolchain](https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html)
- [Yocto Project Cross-compilation](https://docs.yoctoproject.org/dev-manual/cross-development.html)
