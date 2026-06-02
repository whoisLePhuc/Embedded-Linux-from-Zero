# Bài tập 01: Hello World Kernel Module ⭐

## 🎯 Mục tiêu

Viết kernel module đơn giản nhất: load module -> in "Hello, World!" ra kernel log, unload module -> in "Goodbye!".

## 📋 Yêu cầu

1. Tạo file `hello.c` với `module_init` và `module_exit`
2. Sử dụng `pr_info()` để in thông báo (KHÔNG dùng `printk` không kèm log level)
3. Khai báo `MODULE_LICENSE("GPL")` và các metadata: `MODULE_AUTHOR`, `MODULE_DESCRIPTION`
4. Viết `Makefile` để build module cho kernel hiện tại
5. Build, load (`insmod`), kiểm tra (`dmesg`), unload (`rmmod`)

## ✅ Tiêu chí đạt

- `make` thành công, tạo ra `hello.ko`
- `sudo insmod hello.ko` -> `dmesg` hiển thị "Hello, World!"
- `sudo rmmod hello` -> `dmesg` hiển thị "Goodbye!"
- `modinfo hello.ko` hiển thị đầy đủ license, author, description

## 💡 Gợi ý

- Skeleton code đã có cấu trúc sẵn, chỉ cần điền nội dung vào hàm `__init` và `__exit`
- `pr_info("Hello, World!\n");` là tương đương với `printk(KERN_INFO "Hello, World!\n");`
- Makefile dùng `obj-m := hello.o`
- Xem log: `dmesg | tail`

## 🔍 Kiểm tra

```bash
make
sudo insmod hello.ko
dmesg | tail -5
sudo rmmod hello
dmesg | tail -5
modinfo hello.ko
```

## 📁 File cần tạo

- [ ] `hello.c` - Source code module
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-kernel-module.md`:
- Section 1.1 - 1.5: Module Basics
- Section 2.1: printk and Logging

---

**Bonus:** Thêm `MODULE_VERSION("1.0")` và kiểm tra với `modinfo`.
