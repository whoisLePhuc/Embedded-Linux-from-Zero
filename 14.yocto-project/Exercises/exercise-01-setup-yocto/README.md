# Bài tập 01: Thiết lập môi trường build Yocto ⭐⭐

## 🎯 Mục tiêu

Làm quen với quy trình thiết lập môi trường build Yocto: clone Poky, cài đặt dependencies, chạy `oe-init-build-env`, cấu hình machine, và thực hiện build `core-image-minimal`.

## 📋 Yêu cầu

Viết script `setup_build.sh` thực hiện:

1. Kiểm tra và cài đặt các gói phụ thuộc cho Yocto (Ubuntu/Debian)
2. Clone repository Poky (branch `scarthgap`) nếu chưa có
3. Chạy `oe-init-build-env` để khởi tạo build directory
4. Cấu hình `MACHINE = "qemuarm64"` trong `conf/local.conf`
5. Chạy `bitbake core-image-minimal`
6. Hiển thị cấu trúc thư mục build sau khi build hoàn tất

## ✅ Tiêu chí đạt

- Script chạy không lỗi (có kiểm tra điều kiện tiên quyết)
- Clone đúng branch `scarthgap`
- `oe-init-build-env` tạo được thư mục `build/` với cấu trúc chuẩn
- `MACHINE` được set thành `qemuarm64` trong `local.conf`
- `bitbake core-image-minimal` chạy thành công (có thể mất 1-6 giờ)
- Script kiểm tra được output images trong `tmp/deploy/images/qemuarm64/`

## 💡 Gợi ý

- Kiểm tra Poky đã tồn tại trước khi clone:
  ```bash
  if [ ! -d poky ]; then
      git clone git://git.yoctoproject.org/poky -b scarthgap
  fi
  ```

- Dùng `source oe-init-build-env` để khởi tạo build (không phải `bash`):
  ```bash
  source poky/oe-init-build-env build
  ```

- Set MACHINE trong local.conf bằng `sed` hoặc `echo`:
  ```bash
  sed -i 's/^MACHINE ?= "qemux86-64"/MACHINE ?= "qemuarm64"/' conf/local.conf
  ```

- Build image:
  ```bash
  bitbake core-image-minimal
  ```

- Kiểm tra output sau build:
  ```bash
  ls -lh tmp/deploy/images/qemuarm64/
  ```

## 🔍 Kiểm tra

```bash
# Kiểm tra Poky branch
cd poky && git branch && cd ..

# Kiểm tra cấu trúc build
ls -la build/conf/
cat build/conf/local.conf | grep MACHINE

# Kiểm tra output images
ls -lh build/tmp/deploy/images/qemuarm64/ | grep core-image-minimal

# Test image với QEMU
cd build
runqemu qemuarm64 nographic
```

## 📁 File cần tạo

- [ ] `setup_build.sh` - Script thiết lập và build Yocto
- [ ] `verify_build.sh` - Script kiểm tra kết quả build

## 🎓 Kiến thức liên quan

Đọc lại trong `yocto-project.md`:
- Section 2: Environment Setup
- Section 3: First Build
- Section 3.3: Testing with QEMU

---
**Bonus:** Thêm bước tự động chạy `runqemu` sau build thành công để kiểm tra image trong QEMU!
