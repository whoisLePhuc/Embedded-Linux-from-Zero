# BÀI TẬP VỀ LINUX BASIC KNOWLEDGE

Bộ bài tập này giúp bạn thực hành các kiến thức nền tảng về Linux: Shell Script, Biến môi trường, User/Group và quản lý hệ thống.

## 📚 Danh sách bài tập

| Bài tập | Chủ đề | Kỹ năng | Mức độ |
|---------|--------|---------|--------|
| `exercise-01-system-info` | Shell Script cơ bản | Biến, command substitution, echo, printf | ⭐ |
| `exercise-02-file-manager` | Shell Script nâng cao | Vòng lặp, rẽ nhánh, hàm, file operations | ⭐⭐ |
| `exercise-03-user-group-manager` | User & Group | User management commands, file permissions | ⭐⭐ |
| `exercise-04-env-manager` | Biến môi trường | export, source, PATH, env variables | ⭐⭐ |
| `exercise-05-backup-rotator` | Shell Script tổng hợp | Arguments, error handling, automation | ⭐⭐⭐ |

## 🎯 Hướng dẫn

1. Đọc file `README.md` trong mỗi thư mục bài tập
2. Tạo file `.sh` theo yêu cầu và làm theo hướng dẫn
3. Chạy script để kiểm tra kết quả
4. So sánh với file trong thư mục `solution/` nếu cần gợi ý

## 📝 Mức độ

- ⭐ Dễ: Exercise 01
- ⭐⭐ Trung bình: Exercise 02, 03, 04
- ⭐⭐⭐ Khó: Exercise 05

## 💡 Tips

- Luôn thêm `#!/bin/bash` (shebang) ở đầu script
- Sử dụng `set -euo pipefail` để script an toàn hơn
- Test với nhiều trường hợp (file không tồn tại, sai argument...)
- Đọc kỹ các section trong `linux-basic-knowledge.md` trước khi làm
- Dùng `man` để tra cứu lệnh: `man bash`, `man useradd`, `man export`

Chúc bạn học vui! 🚀
