# Bài tập 02: File Manager Script ⭐⭐

## 🎯 Mục tiêu

Thực hành Shell Script nâng cao: vòng lặp, rẽ nhánh (if/case), hàm, và thao tác với file hệ thống.

## 📋 Yêu cầu

Viết script `file_manager.sh` quản lý file và thư mục với các chức năng sau:

1. **List files** (`-l <directory>`): Liệt kê tất cả file trong thư mục, hiển thị tên, kích thước, và loại (file/directory/link)

2. **Search files** (`-s <pattern> <directory>`): Tìm file theo pattern (dùng `find` hoặc `grep`), hiển thị đường dẫn đầy đủ và kích thước

3. **Count by type** (`-c <directory>`): Đếm số lượng file theo từng loại: regular files, directories, symbolic links, executables

4. **Cleanup temp** (`-t <directory>`): Tìm và xóa các file tạm (`*.tmp`, `*.log`, `*.bak`) trong thư mục (hỏi xác nhận trước khi xóa)

5. **Show tree** (`-T <directory>`): Hiển thị cấu trúc thư mục dạng tree (2 cấp độ)

## ✅ Tiêu chí đạt

- Script nhận arguments qua command-line (dùng `getopts` hoặc `case $1`)
- Hiển thị usage/help khi không có argument hoặc `-h`
- Error handling cho thư mục không tồn tại, không có quyền đọc
- Sử dụng ít nhất 3 function để tổ chức code
- Output có màu sắc và định dạng rõ ràng

## 💡 Gợi ý

```bash
#!/bin/bash

# Cấu trúc xử lý argument
usage() {
    echo "Usage: $0 [-l <dir>] [-s <pattern> <dir>] [-c <dir>] [-t <dir>] [-T <dir>] [-h]"
    exit 1
}

# Function mẫu
list_files() {
    local dir="${1:-.}"
    if [ ! -d "$dir" ]; then
        echo "Error: Directory '$dir' does not exist" >&2
        return 1
    fi
    for item in "$dir"/*; do
        if [ -f "$item" ]; then
            echo "FILE: $item ($(stat -c%s "$item") bytes)"
        elif [ -d "$item" ]; then
            echo "DIR:  $item"
        fi
    done
}

# Xử lý arguments
while getopts "l:s:c:t:T:h" opt; do
    case $opt in
        l) list_files "$OPTARG" ;;
        h) usage ;;
        *) usage ;;
    esac
done
```

## 🔍 Kiểm tra

```bash
# Tạo cấu trúc test
mkdir -p test_dir/subdir
touch test_dir/file1.txt test_dir/file2.log test_dir/subdir/file3.tmp
ln -s file1.txt test_dir/link1

# Test các chức năng
chmod +x file_manager.sh
./file_manager.sh -l test_dir
./file_manager.sh -s ".txt" test_dir
./file_manager.sh -c test_dir
./file_manager.sh -t test_dir
./file_manager.sh -T test_dir
```

## 📁 File cần tạo

- [ ] `file_manager.sh` - Script chính

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-basic-knowledge.md`:
- Section 6: Shell Script (6.3-6.6)
- Section 5.2: Quy trình xử lý lệnh

---

**Bonus:** Thêm option `-o <old> <new>` để rename hàng loạt file theo pattern.
