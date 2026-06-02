# Bài tập 03: User & Group Manager ⭐⭐

## 🎯 Mục tiêu

Thực hành quản lý User và Group trên Linux thông qua Shell Script, nắm vững các lệnh `useradd`, `usermod`, `groupadd`, `passwd`, `chown`, `chmod`.

## 📋 Yêu cầu

Viết script `user_group_manager.sh` có các chức năng quản lý user và group:

1. **List users** (`-u`): Liệt kê tất cả user trong hệ thống, hiển thị username, UID, GID, home directory, shell
2. **List groups** (`-g`): Liệt kê tất cả group, hiển thị tên group, GID, danh sách members
3. **User info** (`-i <username>`): Hiển thị thông tin chi tiết của một user: UID, GID, groups, home, shell, last login
4. **Create user** (`-c <username>`): Tạo user mới với:
   - Home directory
   - Shell mặc định `/bin/bash`
   - Thêm vào group `users`
   - Set password tạm thời (lấy từ argument hoặc random)
5. **Lock/Unlock user** (`-L|-U <username>`): Khóa/Mở khóa tài khoản user
6. **Show permissions** (`-p <path>`): Hiển thị owner, group, và permissions chi tiết của file/directory

## ✅ Tiêu chí đạt

- Script kiểm tra quyền root trước khi thực hiện các thao tác cần sudo
- Error handling đầy đủ (user không tồn tại, không có quyền...)
- Output có formatting rõ ràng, dạng bảng
- Sử dụng ít nhất 4 function
- Hiển thị usage khi không có argument hoặc `-h`

## 💡 Gợi ý

```bash
#!/bin/bash

# Kiểm tra quyền root
if [ "$(id -u)" -ne 0 ]; then
    echo "Warning: Some operations require root privileges" >&2
fi

# Đọc thông tin user từ /etc/passwd
get_user_info() {
    local username="$1"
    local user_info
    user_info=$(grep "^${username}:" /etc/passwd 2>/dev/null) || {
        echo "User '$username' not found" >&2
        return 1
    }
    IFS=':' read -r user pass uid gid desc home shell <<< "$user_info"
    echo "Username: $user"
    echo "UID: $uid"
    echo "GID: $gid"
    echo "Home: $home"
    echo "Shell: $shell"
}

# Hiển thị permissions dạng rwx
show_permissions() {
    local path="$1"
    if [ -e "$path" ]; then
        ls -ld "$path" | awk '{print "Permissions:", $1, " Links:", $2, " Owner:", $3, " Group:", $4}'
    fi
}
```

## 🔍 Kiểm tra

```bash
chmod +x user_group_manager.sh

# Xem danh sách users
./user_group_manager.sh -u

# Xem thông tin user hiện tại
./user_group_manager.sh -i $(whoami)

# Xem permissions của thư mục hiện tại
./user_group_manager.sh -p .

# Các lệnh cần root
sudo ./user_group_manager.sh -c testuser
sudo ./user_group_manager.sh -L testuser
sudo ./user_group_manager.sh -g
```

## 📁 File cần tạo

- [ ] `user_group_manager.sh` - Script chính

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-basic-knowledge.md`:
- Section 8: User & Group
- Section 9: Root & Sudo

---

**Bonus 1:** Thêm option `--expire <username>` để xem/set ngày hết hạn tài khoản.

**Bonus 2:** Tạo file report `.csv` với tất cả users và groups.
