# Bài tập 02: Tạo Custom Layer ⭐⭐⭐

## 🎯 Mục tiêu

Tạo một custom layer Yocto có tên `meta-embedded` bằng `bitbake-layers`, cấu hình `layer.conf` đúng chuẩn, và thêm nó vào `bblayers.conf`.

## 📋 Yêu cầu

1. Sử dụng `bitbake-layers create-layer` để tạo layer `meta-embedded`
2. Cấu hình `conf/layer.conf` với:
   - `BBFILE_PRIORITY` phù hợp (ưu tiên cao hơn layer mặc định)
   - `LAYERSERIES_COMPAT` tương thích với `scarthgap`
3. Thêm layer vào `bblayers.conf` bằng `bitbake-layers add-layer`
4. Xác minh layer đã được thêm bằng `bitbake-layers show-layers`
5. Tạo file `README` cho layer mô tả mục đích và nội dung

## ✅ Tiêu chí đạt

- Layer được tạo với cấu trúc thư mục chuẩn
- `conf/layer.conf` chứa đầy đủ: `BBPATH`, `BBFILES`, `BBFILE_COLLECTIONS`, `BBFILE_PATTERN`, `BBFILE_PRIORITY`, `LAYERSERIES_COMPAT`
- `LAYERSERIES_COMPAT` phải khớp với release đang dùng (scarthgap)
- `bitbake-layers show-layers` hiển thị layer `meta-embedded`
- File `README` cho layer có nội dung mô tả rõ ràng

## 💡 Gợi ý

- Tạo layer từ build directory của Poky:
  ```bash
  bitbake-layers create-layer meta-embedded
  # Output: meta-embedded/ với conf/layer.conf, recipes-example/, README
  ```

- Cấu trúc `layer.conf`:
  ```conf
  BBPATH .= ":${LAYERDIR}"
  BBFILES += "${LAYERDIR}/recipes-*/*/*.bb \
              ${LAYERDIR}/recipes-*/*/*.bbappend"
  BBFILE_COLLECTIONS += "embedded"
  BBFILE_PATTERN_embedded = "^${LAYERDIR}/"
  BBFILE_PRIORITY_embedded = "10"
  LAYERSERIES_COMPAT_embedded = "scarthgap"
  ```

- Thêm layer vào build:
  ```bash
  bitbake-layers add-layer meta-embedded
  ```

- Kiểm tra:
  ```bash
  bitbake-layers show-layers
  ```

## 🔍 Kiểm tra

```bash
# Kiểm tra cấu trúc layer
ls -la meta-embedded/
ls -la meta-embedded/conf/

# Kiểm tra layer config
cat meta-embedded/conf/layer.conf

# Kiểm tra layer được load
bitbake-layers show-layers | grep embedded

# Kiểm tra layer paths
bitbake-layers show-layers
```

## 📁 File cần tạo

- [ ] `meta-embedded/conf/layer.conf` - Cấu hình layer
- [ ] `meta-embedded/README` - Mô tả layer

## 🎓 Kiến thức liên quan

Đọc lại trong `yocto-project.md`:
- Section 4.1: Layer Concept
- Section 4.2: Creating a Custom Layer

---
**Bonus:** Thêm một folder `recipes-example/hello/` trong layer với recipe mẫu và kiểm tra xem BitBake có parse được không (`bitbake-layers show-recipes`).
