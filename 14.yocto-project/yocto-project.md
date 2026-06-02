# Embedded Linux - Yocto Project

> Tài liệu về Yocto Project: build system, layers, recipes, image customization, kernel modules, QEMU testing

---

## Table of Contents

### [1. Introduction to Yocto](#1-introduction-to-yocto)
- [1.1. What is Yocto Project?](#11-what-is-yocto-project)
- [1.2. Yocto vs Buildroot](#12-yocto-vs-buildroot)
- [1.3. Key Concepts](#13-key-concepts)
- [1.4. Current Releases](#14-current-releases)

### [2. Environment Setup](#2-environment-setup)
- [2.1. Host Requirements](#21-host-requirements)
- [2.2. Cloning Poky](#22-cloning-poky)
- [2.3. Initializing the Build](#23-initializing-the-build)
- [2.4. Build Directory Structure](#24-build-directory-structure)

### [3. First Build](#3-first-build)
- [3.1. Configuring local.conf](#31-configuring-localconf)
- [3.2. Building core-image-minimal](#32-building-core-image-minimal)
- [3.3. Testing with QEMU](#33-testing-with-qemu)
- [3.4. Common Build Errors](#34-common-build-errors)

### [4. Layers and Recipes](#4-layers-and-recipes)
- [4.1. Layer Concept](#41-layer-concept)
- [4.2. Creating a Custom Layer](#42-creating-a-custom-layer)
- [4.3. Writing a .bb Recipe](#43-writing-a-bb-recipe)
- [4.4. Recipe Variables](#44-recipe-variables)
- [4.5. SRC_URI and File Fetching](#45-src-uri-and-file-fetching)

### [5. Customizing Images](#5-customizing-images)
- [5.1. Adding Packages to Image](#51-adding-packages-to-image)
- [5.2. Custom Image Recipe](#52-custom-image-recipe)
- [5.3. systemd as Init Manager](#53-systemd-as-init-manager)
- [5.4. Kernel Configuration](#54-kernel-configuration)

### [6. Kernel Modules in Yocto](#6-kernel-modules-in-yocto)
- [6.1. Kernel Module Recipe](#61-kernel-module-recipe)
- [6.2. Auto-loading Modules](#62-auto-loading-modules)
- [6.3. Out-of-tree vs In-tree Modules](#63-out-of-tree-vs-in-tree-modules)

### [7. Development Workflow](#7-development-workflow)
- [7.1. devshell](#71-devshell)
- [7.2. devtool](#72-devtool)
- [7.3. BitBake Tasks](#73-bitbake-tasks)
- [7.4. Debugging Build Failures](#74-debugging-build-failures)

### [8. Performance and Caching](#8-performance-and-caching)
- [8.1. Shared State Cache](#81-shared-state-cache)
- [8.2. Parallel Builds](#82-parallel-builds)
- [8.3. Download Mirrors](#83-download-mirrors)

---

## 1. Introduction to Yocto

### 1.1. What is Yocto Project?

Yocto Project is an open-source build system for creating custom Linux distributions for embedded devices. It produces everything you need: kernel, bootloader, root filesystem, toolchain, and SDK.

```
Source Layers          →  BitBake (build engine)  →  Output
                                                         │
poky (core metadata)                                    ├── Image (.wic, .ext4)
meta-oe (open-source)                                   ├── Kernel (bzImage, zImage)
meta-raspberrypi                                        ├── Toolchain (SDK)
meta-mylayer (custom)                                   └── Package feed (.rpm/.deb/.ipk)
```

### 1.2. Yocto vs Buildroot

| Aspect | Yocto | Buildroot |
|--------|-------|-----------|
| Initial build time | 2-6 hours | 10-30 minutes |
| Learning curve | Steep | Moderate |
| Package management | RPM/DEB/IPK + feeds | No (single rootfs) |
| Layer system | Yes (modular) | No (monolithic) |
| SDK generation | Built-in | Manual |
| Enterprise adoption | Very high | Medium |
| Target use case | Complex products | Simple to medium devices |

**When to choose Yocto:**
- Product needs updates/package management
- Multiple hardware variants
- Team collaboration with CI/CD
- Long-term maintenance

### 1.3. Key Concepts

```
Terminology:
┌──────────────────────────────────────────────────────────────┐
│                                                              │
│  Layer       — Collection of recipes and configurations      │
│  Recipe      — .bb file describing how to build a component  │
│  Append      — .bbappend file to modify an existing recipe   │
│  Machine     — Target hardware configuration                 │
│  Distro      — Distribution policy (features, init system)   │
│  Image       — Output rootfs (core-image-minimal, etc.)      │
│  Task        — Build step (do_compile, do_install, etc.)     │
│  Class       — Shared functionality (.bbclass files)         │
│  SState      — Shared state cache (speeds up rebuilds)       │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

### 1.4. Current Releases

| Release | Series | Status | LTS |
|---------|--------|--------|:---:|
| **Wrynose** | 6.0 | Current LTS (April 2026) | ✅ |
| Scarthgap | 5.0 | LTS (April 2024) | ✅ |
| Kirkstone | 4.0 | LTS (April 2022) | ✅ |
| Mickledore | 4.2 | Stable (May 2023) | ❌ |
| Nanbield | 4.3 | Stable (Nov 2023) | ❌ |

> **Recommendation:** Use **Scarthgap (5.0)** for production — it's well-tested. Use **Wrynose (6.0)** for latest features.

---

## 2. Environment Setup

### 2.1. Host Requirements

**Minimum hardware:**
- 4 CPU cores (8+ recommended)
- 8GB RAM (16GB+ recommended)
- 100GB free disk (SSD strongly recommended)

**Required packages (Ubuntu 24.04):**
```bash
sudo apt install gawk wget git diffstat unzip texinfo \
    gcc build-essential chrpath socat cpio python3 \
    python3-pip python3-pexpect xz-utils debianutils \
    iputils-ping python3-git python3-jinja2 libegl1-mesa \
    libsdl1.2-dev python3-subunit mesa-common-dev \
    zstd liblz4-tool file locales libacl1 \
    libssl-dev python3-pyelftools
```

### 2.2. Cloning Poky

Poky is the reference Yocto distribution:

```bash
# Clone with a specific branch (recommended)
git clone git://git.yoctoproject.org/poky -b scarthgap
cd poky

# Or a specific tag
git clone git://git.yoctoproject.org/poky -b scarthgap-5.0.6
```

### 2.3. Initializing the Build

```bash
cd poky

# Source the build environment setup script
source oe-init-build-env
# Creates build/ directory and changes to it
# Sets up environment variables (PATH, etc.)

# Verify
echo $BUILDDIR
# /path/to/poky/build
```

### 2.4. Build Directory Structure

After initialization:

```
build/
├── conf/
│   ├── local.conf      # Local configuration (machine, packages, features)
│   ├── bblayers.conf   # Layer configuration (which layers to use)
│   └── templateconf.cfg
├── tmp/                # Build artifacts (can be >50GB)
│   ├── work/           # Per-recipe build directories
│   ├── deploy/         # Output images, packages, SDKs
│   └── sysroots/       # Target sysroots
├── downloads/          # Downloaded source archives (shared between builds)
└── sstate-cache/       # Shared state cache
```

---

## 3. First Build

### 3.1. Configuring local.conf

Edit `conf/local.conf`:

```conf
# Target machine
MACHINE ?= "qemuarm64"         # ARM 64-bit virtual machine

# Parallelism
BB_NUMBER_THREADS ?= "${@int(4)}"
PARALLEL_MAKE ?= "-j 4"

# Enable additional package formats
PACKAGE_CLASSES ?= "package_rpm"

# Root filesystem size (KB)
IMAGE_ROOTFS_SIZE ?= "524288"

# Extra packages
CORE_IMAGE_EXTRA_INSTALL ?= ""
```

**Common MACHINE values:**
| Machine | Description |
|---------|-------------|
| `qemux86-64` | x86-64 QEMU |
| `qemuarm64` | ARM 64-bit QEMU (Cortex-A57) |
| `qemuarm` | ARM 32-bit QEMU |
| `raspberrypi3` | Raspberry Pi 3 (needs meta-raspberrypi) |
| `beaglebone-yocto` | BeagleBone Black |

### 3.2. Building core-image-minimal

```bash
# Build the minimal console image
bitbake core-image-minimal

# Process:
# 1. Parse recipes (~10-30 seconds)
# 2. Fetch sources (~5-30 minutes, depends on internet)
# 3. Build toolchain (~10-20 minutes)
# 4. Build kernel (~10-30 minutes)
# 5. Build rootfs (~5-10 minutes)
# Total first build: 1-6 hours
```

**Build output location:**
```bash
ls tmp/deploy/images/qemuarm64/
# core-image-minimal-qemuarm64.ext4
# core-image-minimal-qemuarm64.qemuboot.conf
# core-image-minimal-qemuarm64.wic
# Image  (kernel)
# Image--6.6.x-qemuarm64.bin
# modules--6.6.x-qemuarm64.tgz
```

### 3.3. Testing with QEMU

```bash
# Method 1: runqemu (from build directory)
runqemu

# Method 2: specify parameters
runqemu qemuarm64 core-image-minimal ext4

# Method 3: with networking (SLIRP, no root required)
runqemu qemuarm64 slirp

# Inside QEMU:
# Login: root (no password)
# Run commands, test your binaries
```

**QEMU configuration options:**
```bash
runqemu nographic    # Serial console (no GUI)
runqemu kvm          # KVM acceleration (faster, x86 only)
runqemu slirp        # User-mode networking
```

### 3.4. Common Build Errors

```bash
# Error: Missing required packages
# → Install them: sudo apt install <missing-packages>

# Error: Disk full
# → Clean tmp: bitbake -c cleanall <recipe>
# → Or: rm -rf tmp/

# Error: Network timeout fetching sources
# → Download manually, place in downloads/
# → Or set up a mirror (see section 8.3)

# Error: Task failed
# → View log: cat tmp/work/<arch>/<recipe>/<version>/temp/log.do_<task>
```

---

## 4. Layers and Recipes

### 4.1. Layer Concept

A layer is a collection of recipes, configurations, and machine definitions. Yocto's power comes from its modular layer system.

```
meta-yocto-bsp/      (BSP for reference hardware)
meta-raspberrypi/    (Raspberry Pi support)
meta-openembedded/   (Additional packages: networking, multimedia...)
meta-mylayer/        (Your custom application)
```

**Layer priority:** Higher number = higher priority (wins if conflicts):

```conf
# bblayers.conf
BBLAYERS ?= " \
    /path/to/poky/meta \
    /path/to/poky/meta-poky \
    /path/to/poky/meta-yocto-bsp \
    /path/to/meta-mylayer \
    "

BBFILE_PRIORITY_meta-mylayer = "10"
BBFILE_PRIORITY_meta = "5"
BBFILE_PRIORITY_meta-poky = "5"
```

### 4.2. Creating a Custom Layer

```bash
# From poky build directory
bitbake-layers create-layer meta-mylayer

# Output:
# meta-mylayer/
# ├── conf/
# │   └── layer.conf
# ├── recipes-example/
# │   └── example/
# │       └── example_0.1.bb
# └── README

# Add layer to build
bitbake-layers add-layer meta-mylayer

# Verify
bitbake-layers show-layers
```

**layer.conf:**
```conf
# meta-mylayer/conf/layer.conf
BBPATH .= ":${LAYERDIR}"

BBFILES += "${LAYERDIR}/recipes-*/*/*.bb \
            ${LAYERDIR}/recipes-*/*/*.bbappend"

BBFILE_COLLECTIONS += "mylayer"
BBFILE_PATTERN_mylayer = "^${LAYERDIR}/"
BBFILE_PRIORITY_mylayer = "10"

LAYERSERIES_COMPAT_mylayer = "scarthgap"
```

### 4.3. Writing a .bb Recipe

**Simple recipe for a C program (`meta-mylayer/recipes-myapp/myapp/myapp_1.0.bb`):**

```bb
SUMMARY = "Simple Hello World application"
DESCRIPTION = "A minimal C application built with Yocto"
SECTION = "examples"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://helloworld.c"

S = "${UNPACKDIR}"

do_compile() {
    ${CC} ${LDFLAGS} helloworld.c -o helloworld
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 helloworld ${D}${bindir}
}
```

**Source file (`meta-mylayer/recipes-myapp/myapp/files/helloworld.c`):**
```c
#include <stdio.h>

int main(void) {
    printf("Hello from Yocto!\n");
    return 0;
}
```

**Build:**
```bash
bitbake myapp
```

### 4.4. Recipe Variables

```bb
# Basic metadata
SUMMARY = "Brief description"           # Required
DESCRIPTION = "Longer description"
HOMEPAGE = "https://example.com"
SECTION = "networking"                  # Package category
LICENSE = "GPL-2.0-only"               # Required
LIC_FILES_CHKSUM = "..."               # Required

# Source control
SRC_URI = "git://..."                   # Source location
SRCREV = "${AUTOREV}"                  # Git revision
PV = "1.0+git${SRCPV}"                # Package version

# Build
DEPENDS = "libfoo libbar"               # Build-time dependencies
RDEPENDS:${PN} = "libfoo"              # Runtime dependencies
S = "${WORKDIR}/git"                   # Source directory

# Install
inherit autotools                      # Use autotools class
inherit cmake                          # Or CMake class
inherit systemd                        # For systemd services
```

### 4.5. SRC_URI and File Fetching

```bb
# Local file
SRC_URI = "file://mypatch.patch"

# HTTP download
SRC_URI = "https://example.com/source.tar.gz"

# Git repository
SRC_URI = "git://github.com/user/repo.git;branch=main;protocol=https"

# Multiple sources
SRC_URI = "\
    git://github.com/user/repo.git;branch=main;protocol=https \
    file://0001-fix-build.patch \
    file://config.cfg \
    "

# Git submodules
SRC_URI = "gitsm://github.com/user/repo.git;branch=main"
```

---

## 5. Customizing Images

### 5.1. Adding Packages to Image

**Method 1: CORE_IMAGE_EXTRA_INSTALL (simplest)**
```conf
# conf/local.conf
CORE_IMAGE_EXTRA_INSTALL = " strace ltrace i2c-tools"
```

**Method 2: IMAGE_INSTALL:append**
```conf
IMAGE_INSTALL:append = " strace"
# Note: leading space before "strace" is required!
```

**Method 3: Custom image include**
```conf
# conf/local.conf
require conf/include/my-packages.inc
```

### 5.2. Custom Image Recipe

Create `meta-mylayer/recipes-core/images/core-image-mycustom.bb`:

```bb
SUMMARY = "My custom embedded Linux image"

# Include the base image
require recipes-core/images/core-image-minimal.bb

# Add extra packages
IMAGE_INSTALL:append = " \
    strace \
    i2c-tools \
    myapp \
    "

# Set extra features
IMAGE_FEATURES:append = " ssh-server-dropbear"

# Rootfs size
IMAGE_ROOTFS_SIZE = "1048576"

# Inherit core image class
inherit core-image
```

Build:
```bash
bitbake core-image-mycustom
```

### 5.3. systemd as Init Manager

Enable systemd instead of SysVinit:

```conf
# conf/local.conf
INIT_MANAGER = "systemd"
```

This automatically:
- Sets `DISTRO_FEATURES:append = " systemd"`
- Adds `usrmerge` feature
- Installs systemd as PID 1
- Removes sysvinit components

**systemd service unit in a recipe:**
```bb
# recipe.bb
inherit systemd

SRC_URI:append = " file://myapp.service"

SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = "myapp.service"

do_install:append() {
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${UNPACKDIR}/myapp.service \
        ${D}${systemd_system_unitdir}
}
```

**Unit file (`myapp.service`):**
```ini
[Unit]
Description=My Application
After=network.target

[Service]
Type=simple
ExecStart=/usr/bin/myapp
Restart=on-failure

[Install]
WantedBy=multi-user.target
```

### 5.4. Kernel Configuration

```conf
# conf/local.conf — enable/disable kernel options
KERNEL_FEATURES:append = " features/netfilter/netfilter.scc"
KERNEL_FEATURES:append = " cfg/fs/vfat.scc"
```

**Using kernel config fragments:**
```bb
# recipe-kernel/my-kernel-config.bbappend
# or in local.conf
SRC_URI:append:pn-linux-yocto = " file://my-kvm.cfg"
```

**my-kvm.cfg:**
```
CONFIG_KVM=y
CONFIG_VHOST_NET=y
CONFIG_TUN=y
```

---

## 6. Kernel Modules in Yocto

### 6.1. Kernel Module Recipe

Full recipe for building an out-of-tree kernel module:

**`meta-mylayer/recipes-kernel/mymodule/mymodule_1.0.bb`:**
```bb
SUMMARY = "Example kernel module"
DESCRIPTION = "An out-of-tree kernel module built by Yocto"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/GPL-2.0-only;md5=801f80980d171dd6425610833a22dbe6"

inherit module

SRC_URI = "file://Makefile \
           file://hello.c \
           "

S = "${WORKDIR}"

# Ensure consistent kernel version
do_configure[depends] += "virtual/kernel:do_shared_workdir"
```

**`files/Makefile`:**
```makefile
obj-m := hello.o

SRC := $(shell pwd)

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) modules

modules_install:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) modules_install

clean:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) clean
```

**`files/hello.c`:**
```c
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yocto Tutorial");
MODULE_DESCRIPTION("Hello World kernel module");

static int __init hello_init(void) {
    printk(KERN_INFO "Hello from Yocto kernel module!\n");
    return 0;
}

static void __exit hello_exit(void) {
    printk(KERN_INFO "Goodbye from kernel module!\n");
}

module_init(hello_init);
module_exit(hello_exit);
```

Build:
```bash
bitbake mymodule
```

### 6.2. Auto-loading Modules

**Option 1: KERNEL_MODULE_AUTOLOAD in local.conf**
```conf
KERNEL_MODULE_AUTOLOAD:append = " mymodule"
```

**Option 2: In recipe**
```bb
# mymodule_1.0.bb
KERNEL_MODULE_AUTOLOAD:append = " mymodule"
```

**Option 3: Systemd module load**
```
# /etc/modules-load.d/mymodule.conf on target
mymodule
```

### 6.3. Out-of-tree vs In-tree Modules

| Aspect | Out-of-tree | In-tree |
|--------|-------------|---------|
| Maintenance | Recipe only | Kernel config change |
| Kernel version | Must match | Automatic |
| Distribution | Easy (single recipe) | Requires kernel rebuild |
| Build time | Fast (minutes) | Slow (hours) |
| Best for | Custom drivers, prototyping | Stable, upstreamed drivers |

---

## 7. Development Workflow

### 7.1. devshell

Enter a build shell for a recipe to debug interactively:

```bash
bitbake -c devshell myapp

# This opens a shell in the recipe's build directory:
# WORKDIR = tmp/work/<arch>/<recipe>/<version>/
# Environment (CC, CFLAGS, etc.) is set up
# You can run make manually, edit files, test changes

# Exit and rebuild
exit
bitbake myapp -c clean
bitbake myapp
```

### 7.2. devtool

Devtool streamlines the modify-build-test cycle:

```bash
# Create a new recipe
devtool add myapp ~/src/myapp

# Modify existing recipe
devtool modify myapp

# Build
devtool build myapp

# Deploy to target
devtool deploy-target myapp root@<target-ip>

# Finish and create patch
devtool finish myapp meta-mylayer

# Reset
devtool reset myapp
```

### 7.3. BitBake Tasks

```bash
bitbake myapp -c fetch        # Fetch sources only
bitbake myapp -c patch        # Apply patches
bitbake myapp -c configure    # Run configure step
bitbake myapp -c compile      # Compile only
bitbake myapp -c install      # Install to DESTDIR
bitbake myapp -c package      # Create packages
bitbake myapp -c clean        # Clean WORKDIR
bitbake myapp -c cleanall     # Clean WORKDIR + downloads + sstate

# Show task list for recipe
bitbake myapp -c listtasks
```

### 7.4. Debugging Build Failures

```bash
# View log for failed task
cat tmp/work/<arch>/<recipe>/<version>/temp/log.do_compile

# Show BitBake environment
bitbake -e myapp | grep ^WORKDIR=
bitbake -e myapp | grep ^S=
bitbake -e myapp | grep ^D=
bitbake -e myapp | grep ^CC=

# Clean and retry
bitbake myapp -c cleanall
bitbake myapp

# Force re-run specific task
bitbake myapp -c compile -f
```

---

## 8. Performance and Caching

### 8.1. Shared State Cache

The sstate cache dramatically speeds up rebuilds by reusing task outputs:

```conf
# conf/local.conf
SSTATE_DIR ?= "/path/to/sstate-cache"

# Enable hash equivalence server (optimizes rebuilds)
BB_HASHSERVE ?= "auto"
BB_SIGNATURE_HANDLER ?= "OEEquivHash"
```

**Sstate mirror (for team use):**
```conf
SSTATE_MIRRORS ?= "\
    file://.* https://sstate.example.com/sstate/PATH;downloadfilename=PATH \
    "
```

### 8.2. Parallel Builds

```conf
# conf/local.conf
BB_NUMBER_THREADS = "8"           # BitBake threads (parsing)
PARALLEL_MAKE = "-j 8"           # Make jobs (compilation)

# Even more aggressive:
BB_NUMBER_THREADS = "${@int(16)}"
PARALLEL_MAKE = "-j ${@int(16)}"
```

### 8.3. Download Mirrors

Speed up initial builds with a local mirror:

```conf
# conf/local.conf
# Premirror (check first)
PREMIRRORS:prepend = "\
    git://.*/.* file:///opt/yocto/mirror/git/ \
    https?://.*/.* file:///opt/yocto/mirror/downloads/ \
    "

# Or use a public mirror
PREMIRRORS:prepend = "\
    git://.*/.* http://mirror.example.com/yocto/ \
    "
```

---

## 📚 References

- [Yocto Project Documentation](https://docs.yoctoproject.org/)
- [Yocto Project Quick Build](https://docs.yoctoproject.org/brief-yoctoprojectqs/index.html)
- [BitBake User Manual](https://docs.yoctoproject.org/bitbake/)
- [Yocto Project devtool Guide](https://docs.yoctoproject.org/sdk-manual/extensible.html)
- [OpenEmbedded Layer Index](https://layers.openembedded.org/)
