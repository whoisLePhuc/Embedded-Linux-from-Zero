# Bài tập 03: Tạo Recipe cho Ứng dụng C ⭐⭐⭐

## 🎯 Mục tiêu

Viết một Yocto recipe (`.bb`) cho chương trình C `helloworld`, sử dụng `do_compile` và `do_install` để build và cài đặt binary vào image.

## 📋 Yêu cầu

1. Tạo recipe `myapp_1.0.bb` trong `meta-embedded/recipes-myapp/myapp/`
2. Recipe cần có đầy đủ: `SUMMARY`, `LICENSE`, `LIC_FILES_CHKSUM`, `SRC_URI`, `S`, `do_compile`, `do_install`
3. Sử dụng `S = "${UNPACKDIR}"` (cú pháp Scarthgap trở lên, KHÔNG dùng `${WORKDIR}`)
4. File nguồn `helloworld.c` đặt trong thư mục `files/` cùng cấp với recipe
5. Binary được cài vào `${D}${bindir}`
6. Thêm recipe vào image qua `CORE_IMAGE_EXTRA_INSTALL` hoặc `IMAGE_INSTALL:append`
7. Build và kiểm tra binary có trong image

## ✅ Tiêu chí đạt

- Recipe có đầy đủ metadata (`SUMMARY`, `LICENSE`, `LIC_FILES_CHKSUM`)
- `SRC_URI` trỏ đúng đến `file://helloworld.c`
- `S = "${UNPACKDIR}"` được set
- `do_compile` dùng `${CC}` và `${LDFLAGS}` để build
- `do_install` dùng `install -d` và `install -m 0755` để cài đặt
- `bitbake myapp` build thành công
- Binary xuất hiện trong image: `which helloworld` hoạt động trong QEMU

## 💡 Gợi ý

- Cấu trúc recipe:
  ```bb
  SUMMARY = "Simple Hello World application"
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

- C file nguồn đơn giản:
  ```c
  #include <stdio.h>
  int main(void) {
      printf("Hello from Yocto!\n");
      return 0;
  }
  ```

- Thêm vào image trong `conf/local.conf`:
  ```conf
  CORE_IMAGE_EXTRA_INSTALL:append = " myapp"
  ```

## 🔍 Kiểm tra

```bash
# Kiểm tra recipe parse
bitbake-layers show-recipes myapp

# Build recipe
bitbake myapp

# Build image với recipe included
bitbake core-image-minimal

# Kiểm tra binary trong image
runqemu qemuarm64 nographic
# Trong QEMU:
# helloworld
# which helloworld
```

## 📁 File cần tạo

- [ ] `meta-embedded/recipes-myapp/myapp/myapp_1.0.bb` - Recipe file
- [ ] `meta-embedded/recipes-myapp/myapp/files/helloworld.c` - Source code

## 🎓 Kiến thức liên quan

Đọc lại trong `yocto-project.md`:
- Section 4.3: Writing a .bb Recipe
- Section 4.4: Recipe Variables
- Section 5.1: Adding Packages to Image

---
**Bonus:** Thêm DEPENDS vào recipe và tạo một phiên bản recipe thứ hai dùng `SRCREV` với git source thay vì file local.
