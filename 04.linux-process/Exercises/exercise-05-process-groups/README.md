# Bài tập 06: Process Groups và Sessions ⭐⭐

## 🎯 Mục tiêu

Hiểu về process groups, sessions, và cách sử dụng group signaling để quản lý nhóm tiến trình.

## 📋 Yêu cầu

Viết chương trình `process_groups.c` để:

1. **Tạo process tree** với cấu trúc:
   - Parent (session leader mới)
   - Child 1: **cùng process group** với parent
   - Child 2: **process group riêng**
   - Child 3: **process group riêng**

2. **Hiển thị thông tin** process groups:
   - PID, PPID của mỗi process
   - PGID (Process Group ID) của mỗi process
   - SID (Session ID)
   - Phân biệt foreground/background group

3. **Group signaling**:
   - Parent gửi `SIGTERM` đến tất cả process trong một group bằng `killpg()`
   - Demonstrate rằng signal chỉ ảnh hưởng đến group được chỉ định

## ✅ Tiêu chí đạt

- Chương trình chạy được: `./process_groups`
- Hiển thị đầy đủ PID, PPID, PGID, SID cho mỗi process
- Child 2 và Child 3 ở process groups khác parent
- Khi gửi `killpg()` đến group của Child 2, chỉ Child 2 nhận signal
- Không tạo zombie process (parent wait cho tất cả children)
- Output có dạng cây thể hiện quá trình tạo và signaling

## 💡 Gợi ý

- **Tạo child với process group riêng**:
  ```c
  pid_t pid = fork();
  if (pid == 0) {
      // Tạo process group mới cho child này
      setpgid(0, 0);  // or setsid() for new session
      // ... do work ...
      exit(0);
  }
  // Parent: set process group cho child từ parent side
  setpgid(pid, pid);
  ```

- **Các system calls**:
  ```c
  #include <unistd.h>

  pid_t getpgrp(void);                    // Get current process group
  int setpgid(pid_t pid, pid_t pgid);     // Set process group
  pid_t getpgid(pid_t pid);               // Get process group of a PID
  pid_t getsid(pid_t pid);                // Get session ID
  pid_t setsid(void);                     // Create new session
  ```

- **Group signaling**:
  ```c
  #include <signal.h>

  int killpg(pid_t pgrp, int sig);  // Send signal to all processes in group
  ```

- **Cấu trúc output mong đợi**:
  ```
  === Process Tree ===
  Parent  (PID: 1234, PPID: 1000, PGID: 1234, SID: 1234)
    ├── Child 1  (PID: 1235, PPID: 1234, PGID: 1234, SID: 1234) [same group as parent]
    ├── Child 2  (PID: 1236, PPID: 1234, PGID: 1236, SID: 1234) [separate group]
    └── Child 3  (PID: 1237, PPID: 1234, PGID: 1237, SID: 1234) [separate group]

  === Signaling ===
  Parent sending SIGTERM to process group 1236...
  Child 2: received SIGTERM, exiting...
  Parent: Child 2 terminated
  Parent sending SIGTERM to process group 1237...
  ...
  ```

- **Xử lý signal trong child**:
  ```c
  volatile sig_atomic_t sig_received = 0;

  void handler(int sig) {
      (void)sig;
      sig_received = 1;
  }

  // Child loop
  while (!sig_received) {
      pause();  // Wait for signal
  }
  printf("Child %d: received signal, exiting\n", getpid());
  ```

## 🔍 Kiểm tra

```bash
# Build
make

# Chạy chương trình
./process_groups

# Kiểm chứng bằng ps
ps -o pid,ppid,pgid,sid,comm,args

# Xem process tree
pstree -p $(pgrep process_groups)
```

## 📁 File cần tạo

- [ ] `process_groups.c` - Source code chương trình
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-process.md`:
- Section 5: Process Management
- Cột STAT: session leader (s) và foreground process group (+)
- Linux man pages: `man setpgid`, `man getpgrp`, `man killpg`, `man setsid`

---

**Bonus Challenges**:

1. **New session**: Dùng `setsid()` để một child trở thành session leader, quan sát SID thay đổi
2. **Foreground group**: Dùng `tcsetpgrp()` để đưa một process group lên foreground
3. **Signal cascade**: Gửi signal đến tất cả process trong cùng session với `kill(-getpid(), sig)`

```bash
# Bonus: Xem process groups chi tiết
ps -ejH
# Dấu hiệu: processes cùng PGID ở cùng group
```

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
