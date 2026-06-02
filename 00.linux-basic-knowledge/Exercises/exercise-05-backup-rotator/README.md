# Bài tập 05: Backup Rotator ⭐⭐⭐

## 🎯 Mục tiêu

Thực hành Shell Script tổng hợp: argument parsing, error handling, logging, automation với một script backup và rotation hoàn chỉnh.

## 📋 Yêu cầu

Viết script `backup_rotator.sh` tự động backup một thư mục và xoay vòng (rotation) các bản backup cũ.

### Chức năng chính:

1. **Create backup** (`-c <source> [dest_dir]`):
   - Tạo compressed archive (tar.gz) của thư mục nguồn
   - Đặt tên theo format: `backup_<dirname>_YYYYMMDD_HHMMSS.tar.gz`
   - Lưu vào thư mục đích (mặc định: `./backups/`)

2. **List backups** (`-l [backup_dir]`):
   - Liệt kê tất cả backup trong thư mục
   - Hiển thị tên, kích thước, ngày tạo
   - Sắp xếp theo thời gian (mới nhất trước)

3. **Rotate backups** (`-r <keep_count> [backup_dir]`):
   - Giữ lại N bản backup mới nhất
   - Xóa các bản cũ hơn
   - Hiển thị danh sách đã xóa

4. **Extract backup** (`-x <backup_file> [dest_dir]`):
   - Extract một backup file vào thư mục đích
   - Kiểm tra tính toàn vẹn của archive trước khi extract

5. **Schedule backup** (`-s <interval_minutes> <source> [dest_dir]`):
   - Chạy backup định kỳ mỗi N phút
   - Tự động rotate giữ 10 bản gần nhất
   - Ghi log vào file `backup.log`

6. **Verify backup** (`-v <backup_file>`):
   - Kiểm tra tính toàn vẹn của file backup (dùng `tar -tzf`)
   - Hiển thị nội dung bên trong archive (danh sách file)

## ✅ Tiêu chí đạt

- Script hoạt động với cả absolute và relative paths
- Error handling: source không tồn tại, không có quyền ghi, disk full
- Sử dụng `set -euo pipefail` để an toàn
- Có ít nhất 6 function
- Usage/help đầy đủ
- Logging: ghi timestamp vào file `backup.log`
- Màu sắc và format output rõ ràng

## 💡 Gợi ý

```bash
#!/bin/bash
set -euo pipefail

BACKUP_DIR="./backups"
LOG_FILE="backup.log"

# Function: log message với timestamp
log() {
    local level="$1"
    local message="$2"
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] [$level] $message" | tee -a "$LOG_FILE"
}

# Function: tạo backup
create_backup() {
    local source="$1"
    local dest="${2:-$BACKUP_DIR}"
    local timestamp
    timestamp=$(date '+%Y%m%d_%H%M%S')
    local dirname
    dirname=$(basename "$source")
    local backup_file="${dest}/backup_${dirname}_${timestamp}.tar.gz"

    mkdir -p "$dest"
    tar -czf "$backup_file" -C "$(dirname "$source")" "$dirname"
    echo "$backup_file"
}

# Function: xoay vòng backups
rotate_backups() {
    local keep="$1"
    local dir="${2:-$BACKUP_DIR}"
    # List backups sorted by time (oldest first), delete excess
    local count=0
    while IFS= read -r file; do
        ((count++))
        if [ "$count" -gt "$keep" ]; then
            rm "$file"
            log "DELETE" "Removed old backup: $file"
        fi
    done < <(ls -1t "$dir"/backup_*.tar.gz 2>/dev/null | tail -n +$((keep + 1)) | sort)
}
```

## 🔍 Kiểm tra

```bash
chmod +x backup_rotator.sh

# Tạo thư mục test
mkdir -p test_project/src test_project/docs
echo "int main() {}" > test_project/src/main.c
echo "doc content" > test_project/docs/readme.txt

# Tạo backup
./backup_rotator.sh -c test_project
./backup_rotator.sh -c test_project /tmp/my_backups

# List backups
./backup_rotator.sh -l

# Verify backup
./backup_rotator.sh -v backups/backup_test_project_*.tar.gz

# Rotate (giữ 3 bản mới nhất)
./backup_rotator.sh -r 3

# Extract
./backup_rotator.sh -x backups/backup_test_project_*.tar.gz ./restored

# Schedule (backup mỗi 1 phút, chạy 3 lần rồi kill)
./backup_rotator.sh -s 1 test_project &
BGPID=$!
sleep 3
kill $BGPID
```

## 📁 File cần tạo

- [ ] `backup_rotator.sh` - Script backup chính
- [ ] `backup.log` - File log (tự động tạo)

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-basic-knowledge.md`:
- Section 6: Shell Script (toàn bộ)
- Section 6.4: Cấu trúc điều khiển
- Section 6.5: Hàm
- Section 6.6: Debug Script

---

**Bonus 1:** Thêm mã hóa backup bằng `gpg` trước khi lưu.

**Bonus 2:** Gửi email notification khi backup hoàn tất (dùng `mail` hoặc `sendmail`).

**Bonus 3:** Backup incremental - chỉ backup những file đã thay đổi.
