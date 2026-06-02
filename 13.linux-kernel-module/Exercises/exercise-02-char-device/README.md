# Bài tập 02: Character Device Driver ⭐⭐

## 🎯 Mục tiêu

Viết character device driver đơn giản: tạo `/dev/chardev`, hỗ trợ `open`, `read`, `write`. Driver lưu một giá trị int và cho phép user space đọc/ghi giá trị đó.

## 📋 Yêu cầu

1. Cấp phát device number động với `alloc_chrdev_region`
2. Khởi tạo cdev với `cdev_init` và `cdev_add`
3. Tạo class và device node với `class_create` + `device_create`
4. Implement `open`, `release`, `read`, `write` trong `file_operations`
5. Dùng `copy_to_user` / `copy_from_user` (KHÔNG dùng `memcpy` trực tiếp)
6. Viết Makefile build module

## ✅ Tiêu chí đạt

- Load module tạo `/dev/chardev`
- `cat /dev/chardev` hiển thị giá trị hiện tại
- `echo "42" > /dev/chardev` ghi giá trị mới
- `cat /dev/chardev` hiển thị 42 sau khi ghi
- `rmmod` dọn dẹp sạch sẽ

## 💡 Gợi ý

- Dùng `struct file_operations` với `.unlocked_ioctl` (KHÔNG `.ioctl`)
- Lưu giá trị trong biến global `static int my_value`
- Trong `open` gán `filp->private_data = &my_value;`
- `read` format số thành chuỗi, dùng `snprintf` + `copy_to_user`
- `write` dùng `copy_from_user` + `kstrtoint`
- `unregister_chrdev_region` khi cleanup

## 🔍 Kiểm tra

```bash
make
sudo insmod chardev.ko
ls -l /dev/chardev
cat /dev/chardev
echo "123" | sudo tee /dev/chardev
cat /dev/chardev
sudo rmmod chardev
dmesg | tail
```

## 📁 File cần tạo

- [ ] `chardev.c` - Source code driver
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-kernel-module.md`:
- Section 3.1 - 3.5: Character Device Drivers
- Section 2.4: Error Handling

---

**Bonus:** Thêm `unlocked_ioctl` với 2 command: `MY_GET_VALUE` và `MY_SET_VALUE`.
