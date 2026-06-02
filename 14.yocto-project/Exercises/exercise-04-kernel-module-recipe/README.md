# Bài tập 04: Recipe cho Kernel Module ⭐⭐⭐

## 🎯 Mục tiêu

Viết Yocto recipe cho một kernel module out-of-tree (`hello.ko`) sử dụng `inherit module`, cấu hình `KERNEL_MODULE_AUTOLOAD`, và build với BitBake.

## 📋 Yêu cầu

1. Tạo recipe `mymodule_1.0.bb` trong `meta-embedded/recipes-kernel/mymodule/`
2. Recipe sử dụng `inherit module` class
3. File `Makefile` và `hello.c` đặt trong thư mục `files/`
4. `Makefile` dùng `KERNEL_SRC` (biến môi trường do Yocto cung cấp)
5. Thêm `do_configure[depends] += "virtual/kernel:do_shared_workdir"` để đảm bảo kernel source sẵn sàng
6. Cấu hình `KERNEL_MODULE_AUTOLOAD` để module tự động load khi boot
7. Recipe sử dụng `S = "${WORKDIR}"` (mặc định cho kernel modules)

## ✅ Tiêu chí đạt

- Recipe có `inherit module` và `SRC_URI` trỏ đến `Makefile`, `hello.c`
- `Makefile` dùng `obj-m := hello.o` và build với `$(MAKE) -C $(KERNEL_SRC) M=$(SRC) modules`
- File C kernel module: `module_init`, `module_exit`, `MODULE_LICENSE("GPL")`
- `do_configure[depends]` trỏ đến `virtual/kernel:do_shared_workdir`
- `KERNEL_MODULE_AUTOLOAD:append = " mymodule"` trong recipe
- `bitbake mymodule` build thành công, tạo ra `hello.ko`
- Module load/unload được trong QEMU

## 💡 Gợi ý

- Cấu trúc recipe:
  ```bb
  SUMMARY = "Example kernel module"
  LICENSE = "GPL-2.0-only"
  LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/GPL-2.0-only;md5=801f80980d171dd6425610833a22dbe6"

  inherit module

  SRC_URI = "file://Makefile \
             file://hello.c"

  S = "${WORKDIR}"

  do_configure[depends] += "virtual/kernel:do_shared_workdir"

  KERNEL_MODULE_AUTOLOAD:append = " mymodule"
  ```

- Makefile:
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

- Kernel module C file:
  ```c
  #include <linux/module.h>
  #include <linux/kernel.h>

  MODULE_LICENSE("GPL");
  MODULE_AUTHOR("Your Name");
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

## 🔍 Kiểm tra

```bash
# Build module
bitbake mymodule

# Tìm module trong deploy
find tmp/deploy/ -name "hello.ko" 2>/dev/null

# Kiểm tra module được package
bitbake -e mymodule | grep ^KERNEL_MODULE_AUTOLOAD

# Chạy image và kiểm tra
runqemu qemuarm64 nographic
# Trong QEMU:
# lsmod | grep hello
# modinfo hello
# dmesg | grep Hello
```

## 📁 File cần tạo

- [ ] `meta-embedded/recipes-kernel/mymodule/mymodule_1.0.bb` - Kernel module recipe
- [ ] `meta-embedded/recipes-kernel/mymodule/files/Makefile` - Kernel module Makefile
- [ ] `meta-embedded/recipes-kernel/mymodule/files/hello.c` - Kernel module source

## 🎓 Kiến thức liên quan

Đọc lại trong `yocto-project.md`:
- Section 6.1: Kernel Module Recipe
- Section 6.2: Auto-loading Modules

---
**Bonus:** Thêm `KERNEL_MODULE_PROBECONF` và module parameters để cấu hình module tham số qua command line.
