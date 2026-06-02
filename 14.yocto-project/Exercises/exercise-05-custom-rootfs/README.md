# Bài tập 05: Tùy chỉnh RootFS với systemd ⭐⭐⭐

## 🎯 Mục tiêu

Kết hợp custom layer, application recipe, systemd unit, và image recipe để tạo một image Linux nhúng hoàn chỉnh với systemd làm init manager, tự động chạy ứng dụng khi boot.

## 📋 Yêu cầu

1. Tạo image recipe `core-image-demo.bb` trong `meta-embedded/recipes-core/images/`
2. Image kế thừa từ `core-image-minimal` và thêm package `myapp` (từ exercise 03)
3. Tạo systemd service file `myapp.service` cho ứng dụng `helloworld`
4. Cập nhật recipe `myapp_1.0.bb` để `inherit systemd` và cài service file
5. Cấu hình `INIT_MANAGER = "systemd"` trong `local.conf`
6. Build image và kiểm tra systemd hoạt động trong QEMU

## ✅ Tiêu chí đạt

- `core-image-demo.bb` dùng `require recipes-core/images/core-image-minimal.bb` và `inherit core-image`
- `IMAGE_INSTALL:append` bao gồm `myapp`
- systemd service file có đúng cấu trúc `[Unit]`, `[Service]`, `[Install]`
- Recipe `myapp` có `inherit systemd`, `SYSTEMD_SERVICE:${PN}`, `SYSTEMD_AUTO_ENABLE`
- `INIT_MANAGER = "systemd"` trong `local.conf`
- `bitbake core-image-demo` build thành công
- Trong QEMU: `systemctl status myapp` hiển thị service đang chạy

## 💡 Gợi ý

- Image recipe:
  ```bb
  SUMMARY = "Demo embedded Linux image with systemd"

  require recipes-core/images/core-image-minimal.bb

  IMAGE_INSTALL:append = " myapp"

  inherit core-image
  ```

- systemd service file (`myapp.service`):
  ```ini
  [Unit]
  Description=Hello World Application
  After=network.target

  [Service]
  Type=simple
  ExecStart=/usr/bin/helloworld
  Restart=on-failure

  [Install]
  WantedBy=multi-user.target
  ```

- Cập nhật recipe `myapp` để include systemd:
  ```bb
  inherit systemd

  SRC_URI:append = " file://myapp.service"

  SYSTEMD_PACKAGES = "${PN}"
  SYSTEMD_SERVICE:${PN} = "myapp.service"
  SYSTEMD_AUTO_ENABLE:${PN} = "enable"

  do_install:append() {
      install -d ${D}${systemd_system_unitdir}
      install -m 0644 ${UNPACKDIR}/myapp.service ${D}${systemd_system_unitdir}
  }
  ```

- Cấu hình local.conf:
  ```conf
  INIT_MANAGER = "systemd"
  ```

## 🔍 Kiểm tra

```bash
# Build image
bitbake core-image-demo

# Chạy trong QEMU
runqemu qemuarm64 nographic

# Trong QEMU, kiểm tra:
# systemctl status myapp
# systemctl list-units | grep myapp
# journalctl -u myapp
# helloworld (chạy trực tiếp)
# ps aux | grep helloworld
```

## 📁 File cần tạo

- [ ] `meta-embedded/recipes-core/images/core-image-demo.bb` - Image recipe
- [ ] `meta-embedded/recipes-myapp/myapp/files/myapp.service` - systemd unit file

## 🎓 Kiến thức liên quan

Đọc lại trong `yocto-project.md`:
- Section 5.2: Custom Image Recipe
- Section 5.3: systemd as Init Manager

---
**Bonus:** Thêm `IMAGE_FEATURES:append = " ssh-server-dropbear"` để có SSH trong image, và thêm một systemd timer thay vì service để chạy ứng dụng định kỳ.
