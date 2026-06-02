# Bài tập 03: Platform Driver với Device Tree ⭐⭐⭐

## 🎯 Mục tiêu

Viết platform driver skeleton đọc memory resource từ device tree, đăng ký với `platform_driver` và `of_match_table`.

## 📋 Yêu cầu

1. Định nghĩa `of_match_table` với compatible string `"vendor,my-device"`
2. Sử dụng `MODULE_DEVICE_TABLE(of, ...)`
3. Implement `probe` và `remove` trong `platform_driver`
4. Trong `probe`: lấy memory resource bằng `platform_get_resource`, map bằng `devm_ioremap_resource`
5. Dùng `module_platform_driver()` macro
6. Viết device tree overlay example (`.dts`) trong `solution/`

## ✅ Tiêu chí đạt

- Module build thành công
- `modinfo` hiển thị `alias: platform:my-platform-driver`
- Khi DT node được thêm, probe tự động gọi, in thông tin resource
- `remove` được gọi khi module unload

## 💡 Gợi ý

- `of_match_table` kết thúc bằng sentinel `{ }`
- `probe` trả về 0 thành công, negative error code nếu lỗi
- `devm_ioremap_resource` tự động quản lý memory (devres)
- Platform driver name dùng trong `driver.name`
- `.dts` chứa `compatible = "vendor,my-device"` và `reg = <addr size>`

## 🔍 Kiểm tra

```bash
make
modinfo platform.ko
# Trên target board có DT:
sudo insmod platform.ko
# Nếu DT node không tồn tại, probe sẽ không được gọi
dmesg | tail
sudo rmmod platform
```

## 📁 File cần tạo

- [ ] `platform.c` - Source code platform driver
- [ ] `Makefile` - Build script
- [ ] (solution) `mydriver.dts` - Device tree overlay example

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-kernel-module.md`:
- Section 4.1 - 4.5: Platform Driver and Device Tree

---

**Bonus:** Thêm xử lý interrupt với `platform_get_irq` và `devm_request_irq`.
