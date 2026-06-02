# Bài tập 01: System Information Script ⭐

## 🎯 Mục tiêu

Làm quen với Shell Script cơ bản: biến, command substitution, echo/printf, và tương tác với hệ thống Linux.

## 📋 Yêu cầu

Viết script `system_info.sh` hiển thị thông tin hệ thống với định dạng đẹp:

1. **System Info:**
   - Hostname
   - Kernel version (`uname -r`)
   - OS architecture (`uname -m`)
   - Uptime (`uptime -p`)

2. **CPU Info:**
   - Số lượng CPU cores (`nproc`)
   - CPU model (đọc từ `/proc/cpuinfo`)

3. **Memory Info:**
   - Tổng RAM (`free -h`)
   - RAM đã dùng
   - RAM còn trống

4. **Disk Info:**
   - Dung lượng ổ đĩa (`df -h /`)
   - Phần trăm đã dùng

5. **Network Info:**
   - Hostname
   - IP address (dùng `hostname -I`)

## ✅ Tiêu chí đạt

- Script chạy được: `bash system_info.sh` hoặc `./system_info.sh`
- Hiển thị đầy đủ 5 nhóm thông tin
- Output có định dạng rõ ràng, dễ đọc (dùng `printf` để căn chỉnh)
- Có phần header với title và đường kẻ ngăn cách
- Error handling: kiểm tra lệnh có tồn tại trước khi gọi

## 💡 Gợi ý

```bash
#!/bin/bash

# Cấu trúc script
TITLE="System Information Report"
SEPARATOR="========================================"

echo "$TITLE"
echo "$SEPARATOR"

# Command substitution
HOSTNAME=$(hostname)
KERNEL=$(uname -r)

# Sử dụng printf để căn chỉnh
printf "%-20s: %s\n" "Hostname" "$HOSTNAME"
printf "%-20s: %s\n" "Kernel" "$KERNEL"

# Kiểm tra lệnh trước khi dùng
if command -v nproc &> /dev/null; then
    CPU_CORES=$(nproc)
    printf "%-20s: %s\n" "CPU Cores" "$CPU_CORES"
fi
```

## 🔍 Kiểm tra

```bash
# Phân quyền thực thi
chmod +x system_info.sh

# Chạy script
./system_info.sh

# Kiểm tra cú pháp
bash -n system_info.sh
```

## 📁 File cần tạo

- [ ] `system_info.sh` - Script chính

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-basic-knowledge.md`:
- Section 5: Shell
- Section 6: Shell Script (6.1-6.3: Biến, Argument)

---

**Bonus:** Thêm option `-f` (full) hiển thị thêm thông tin về network interfaces, logged-in users, và running processes.
