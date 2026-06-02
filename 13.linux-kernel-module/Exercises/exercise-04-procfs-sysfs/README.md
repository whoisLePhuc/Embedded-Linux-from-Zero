# Bài tập 04: procfs và sysfs ⭐⭐⭐

## 🎯 Mục tiêu

Viết module tạo file `/proc/mydriver` (read-only, hiển thị jiffies) và `/sys/kernel/mydriver/value` (read/write, lưu giá trị int).

## 📋 Yêu cầu

1. Tạo `/proc/mydriver` với `proc_create` và `struct proc_ops` (KHÔNG `file_operations`)
2. Sử dụng `single_open` + `seq_printf` để hiển thị jiffies
3. Tạo kobject với `kobject_create_and_add` dưới `kernel_kobj`
4. Tạo sysfs attribute với `__ATTR` và `sysfs_create_group`
5. Implement `show` và `store` cho sysfs attribute

## ✅ Tiêu chí đạt

- `cat /proc/mydriver` hiển thị jiffies hiện tại
- `cat /sys/kernel/mydriver/value` hiển thị giá trị
- `echo "42" | sudo tee /sys/kernel/mydriver/value` ghi giá trị mới
- Đọc lại `/sys/kernel/mydriver/value` thấy giá trị đã thay đổi

## 💡 Gợi ý

- `/proc` dùng `struct proc_ops` với `.proc_open`, `.proc_read` = `seq_read`, `.proc_lseek` = `seq_lseek`, `.proc_release` = `single_release`
- Hàm show dùng `seq_printf(m, "jiffies: %lu\n", jiffies);`
- sysfs dùng `struct kobj_attribute` + `__ATTR(name, perm, show, store)`
- `sysfs_emit` cho show, `kstrtoint` cho store
- Cleanup: `proc_remove` + `sysfs_remove_group` + `kobject_put`

## 🔍 Kiểm tra

```bash
make
sudo insmod proc_sysfs.ko
cat /proc/mydriver
cat /sys/kernel/mydriver/value
echo "999" | sudo tee /sys/kernel/mydriver/value
cat /sys/kernel/mydriver/value
sudo rmmod proc_sysfs
dmesg | tail
```

## 📁 File cần tạo

- [ ] `proc_sysfs.c` - Source code module
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-kernel-module.md`:
- Section 5.1: procfs with proc_ops
- Section 5.2: sysfs with kobject

---

**Bonus:** Thêm attribute thứ hai `/sys/kernel/mydriver/jiffies` (read-only) hiển thị jiffies.
