# Bài tập 05: Daemon Process Creation ⭐⭐

## 🎯 Mục tiêu

Hiểu và thực hành tạo daemon process trong Linux: fork, session, signal handling, và file logging.

## 📋 Yêu cầu

Viết chương trình `daemon_timestamp.c` - một daemon đơn giản:

1. **Daemon hóa** process bằng các bước chuẩn:
   - `fork()` để chạy background
   - `setsid()` tạo session mới
   - `umask(0)` xóa bỏ file mode mask
   - `chdir("/")` chuyển về root directory
   - Đóng tất cả file descriptors không cần thiết
   - Redirect stdin/stdout/stderr đến `/dev/null`

2. **Ghi log** timestamp vào file `/tmp/daemon_timestamp.log` mỗi 5 giây

3. **Xử lý signal**:
   - `SIGTERM`: Dọn dẹp và thoát sạch sẽ
   - `SIGHUP`: Reload (re-open log file)

## ✅ Tiêu chí đạt

- Daemon chạy background, không chiếm terminal
- Log file được ghi mỗi 5 giây với timestamp
- `SIGTERM` kill daemon sạch sẽ (không zombie, log có "shutdown" message)
- `SIGHUP` gửi đến daemon không crash
- Có thể dùng `kill` command để tương tác với daemon
- Output log dạng: `[2024-01-01 12:00:00] Daemon running (PID: 1234)`

## 💡 Gợi ý

- **Daemon hóa process**:
  ```c
  void daemonize() {
      pid_t pid = fork();
      if (pid < 0) exit(EXIT_FAILURE);
      if (pid > 0) exit(EXIT_SUCCESS);  // Parent exits

      // Child continues
      if (setsid() < 0) exit(EXIT_FAILURE);

      // Second fork to prevent acquiring terminal
      pid = fork();
      if (pid < 0) exit(EXIT_FAILURE);
      if (pid > 0) exit(EXIT_SUCCESS);

      umask(0);
      chdir("/");

      // Close all open FDs
      for (int i = 0; i < sysconf(_SC_OPEN_MAX); i++)
          close(i);

      // Redirect stdin/stdout/stderr to /dev/null
      open("/dev/null", O_RDWR);  // stdin
      dup(0);                     // stdout
      dup(0);                     // stderr
  }
  ```

- **Signal handlers**:
  ```c
  volatile sig_atomic_t running = 1;
  volatile sig_atomic_t reload = 0;

  void signal_handler(int sig) {
      if (sig == SIGTERM) running = 0;
      else if (sig == SIGHUP) reload = 1;
  }

  int main() {
      signal(SIGTERM, signal_handler);
      signal(SIGHUP, signal_handler);

      FILE *log = fopen(LOG_FILE, "a");
      while (running) {
          if (reload) {
              // Re-open log file
              reload = 0;
          }
          // Write timestamp
          sleep(5);
      }
      fclose(log);
      return 0;
  }
  ```

- **Ghi log với timestamp**:
  ```c
  time_t now = time(NULL);
  struct tm *tm_info = localtime(&now);
  char buf[64];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
  fprintf(log, "[%s] Daemon running (PID: %d)\n", buf, getpid());
  fflush(log);
  ```

## 🔍 Kiểm tra

```bash
# Build
make

# Chạy daemon
./daemon_timestamp

# Kiểm tra daemon đang chạy
ps aux | grep daemon_timestamp
# STAT column should show 'S' or 'Ss'

# Xem log (terminal 2)
tail -f /tmp/daemon_timestamp.log

# Gửi SIGHUP (terminal 2)
kill -HUP $(pgrep daemon_timestamp)

# Dừng daemon
kill -TERM $(pgrep daemon_timestamp)

# Kiểm tra log ghi "shutdown"
tail /tmp/daemon_timestamp.log
```

## 📁 File cần tạo

- [ ] `daemon_timestamp.c` - Source code chương trình
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-process.md`:
- Section 4.1: System call fork()
- Section 6.3: Double fork (daemon pattern)
- Linux man pages: `man daemon`, `man setsid`, `man signal`

---

**Bonus Challenges**:

1. **PID file**: Tạo `/var/run/daemon_timestamp.pid` để tránh chạy multiple instances
2. **Log rotation**: Implement SIGHUP để đóng và mở lại log file (dùng cho logrotate)
3. **Syslog**: Thay vì ghi file, sử dụng `syslog()` để ghi log system-wide

```bash
# Bonus: Sử dụng syslog
#include <syslog.h>
openlog("daemon_timestamp", LOG_PID | LOG_CONS, LOG_DAEMON);
syslog(LOG_INFO, "Daemon started");
closelog();
```

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
