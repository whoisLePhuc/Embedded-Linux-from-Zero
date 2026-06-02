# Bài tập 01: Process Information ⭐

## 🎯 Mục tiêu

Tạo chương trình hiển thị thông tin cơ bản về process đang chạy.

## 📋 Yêu cầu

Viết chương trình `process_info.c` để:

1. Hiển thị **PID** (Process ID) của process hiện tại
2. Hiển thị **PPID** (Parent Process ID)
3. Hiển thị **UID** và **GID** (User/Group ID)
4. Hiển thị **EUID** và **EGID** (Effective User/Group ID)
5. Hiển thị tên chương trình đang chạy

## ✅ Tiêu chí đạt

- Chương trình chạy được: `./process_info`
- Hiển thị đầy đủ và đúng các thông tin yêu cầu
- Output rõ ràng, có label cho mỗi thông tin
- Kiểm chứng được với lệnh `ps` hoặc `echo $$`

## 💡 Gợi ý

- Sử dụng các system calls:
  ```c
  #include <unistd.h>
  #include <sys/types.h>
  
  pid_t getpid(void);   // Lấy PID
  pid_t getppid(void);  // Lấy Parent PID
  uid_t getuid(void);   // Lấy User ID
  uid_t geteuid(void);  // Lấy Effective User ID
  gid_t getgid(void);   // Lấy Group ID
  gid_t getegid(void);  // Lấy Effective Group ID
  ```

- Format output ví dụ:
  ```
  ==============================
  Process Information
  ==============================
  PID:         12345
  Parent PID:  12340
  User ID:     1000
  Group ID:    1000
  Effective UID: 1000
  Effective GID: 1000
  ==============================
  ```

## 🔍 Kiểm tra

```bash
# Build
make

# Chạy chương trình
./process_info

# Kiểm chứng PID với shell
echo "Shell PID: $$"
./process_info
# PPID của process_info phải bằng PID của shell

# Kiểm chứng với ps
ps -f
```

## 📁 File cần tạo

- [ ] `process_info.c` - Source code chương trình
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-process.md`:
- Section 1.1: Program và Process
- Phần giới thiệu về PID và PPID

---

**Bonus**: Thử chạy program trong terminal khác và so sánh PPID!

```bash
# Terminal 1
echo $$    # Ví dụ: 1234
./process_info  # PPID = 1234

# Terminal 2
echo $$    # Ví dụ: 5678
./process_info  # PPID = 5678
```

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
