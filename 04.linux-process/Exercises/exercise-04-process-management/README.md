# Bài tập 04: Process Management ⭐⭐⭐

## 🎯 Mục tiêu

Hiểu và thực hành quản lý process: zombie process, orphan process, và process pool pattern.

## 📋 Yêu cầu

### Phần 1: Zombie Demo (`zombie_demo.c`)

Viết chương trình minh họa:
1. **Tạo zombie process** - parent không wait cho child
2. **Quan sát zombie** trong `ps` output
3. **Phòng tránh zombie** với signal handler

### Phần 2: Process Pool (`process_pool.c`)

Viết chương trình tạo process pool đơn giản:
1. Tạo N worker processes (ví dụ: 3)
2. Mỗi worker thực hiện một task rồi exit
3. Parent sử dụng `waitpid()` với `WNOHANG` để monitor
4. Hiển thị khi mỗi worker hoàn thành

## ✅ Tiêu chí đạt

### Zombie Demo:
- Tạo được zombie process quan sát được bằng `ps aux | grep Z`
- Có cách phòng tránh zombie
- Giải thích tại sao zombie xuất hiện

### Process Pool:
- Tạo đúng số lượng workers
- Parent không bị block
- Không tạo zombie processes
- Hiển thị khi mỗi worker hoàn thành

## 💡 Gợi ý

### Zombie Demo:
```c
int main() {
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child exits immediately
        printf("Child: exiting now\n");
        exit(0);
    }
    
    // Parent sleeps without waiting
    printf("Parent: sleeping 30s, run 'ps aux | grep Z'\n");
    sleep(30);
    // Child becomes zombie during this time!
    
    return 0;
}
```

### Signal handler để prevent zombie:
```c
#include <signal.h>

void sigchld_handler(int sig) {
    (void)sig;
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main() {
    signal(SIGCHLD, sigchld_handler);
    // ... fork children ...
}
```

### Process Pool:
```c
#define NUM_WORKERS 3

int main() {
    pid_t workers[NUM_WORKERS];
    int running = NUM_WORKERS;
    
    // Create workers
    for (int i = 0; i < NUM_WORKERS; i++) {
        workers[i] = fork();
        if (workers[i] == 0) {
            // Worker: do task
            sleep(i + 1);  // Different work times
            exit(i);
        }
    }
    
    // Monitor workers
    while (running > 0) {
        for (int i = 0; i < NUM_WORKERS; i++) {
            if (workers[i] > 0) {
                int status;
                pid_t result = waitpid(workers[i], &status, WNOHANG);
                if (result == workers[i]) {
                    printf("Worker %d finished\n", i);
                    workers[i] = 0;
                    running--;
                }
            }
        }
        usleep(100000);  // 100ms polling
    }
    
    return 0;
}
```

## 🔍 Kiểm tra

### Zombie Demo:
```bash
# Build
make

# Terminal 1: Run zombie demo
./zombie_demo

# Terminal 2: Observe zombie
ps aux | grep Z
# Should see: ... Z ... [zombie_demo] <defunct>

# Wait 30s and zombie disappears when parent exits
```

### Process Pool:
```bash
# Run process pool
./process_pool

# Expected output:
# Created worker 0 (PID: 1234)
# Created worker 1 (PID: 1235)
# Created worker 2 (PID: 1236)
# Worker 0 finished with status 0
# Worker 1 finished with status 1
# Worker 2 finished with status 2
# All workers completed!
```

## 📁 File cần tạo

- [ ] `zombie_demo.c` - Demo zombie process
- [ ] `process_pool.c` - Process pool pattern
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-process.md`:
- Section 5: Process Management
- Section 6: Orphan và Zombie Process
- Section 6.3: Cách phòng tránh Zombie

---

**Bonus Challenges**:

1. **Orphan demo**: Tạo orphan process và quan sát PPID thay đổi
2. **Load balancer**: Distribute tasks to available workers
3. **Worker respawn**: Restart worker nếu nó crash

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
