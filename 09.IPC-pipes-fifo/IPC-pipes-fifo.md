# Embedded Linux - IPC Pipes & FIFO

> Tài liệu học về Linux Pipes (Anonymous Pipe) và Named Pipes (FIFO): Cách tạo, sử dụng, các kỹ thuật nâng cao như popen, dup2, non-blocking I/O, và mô hình Client-Server.

---

## 📑 Mục Lục

### [1. Introduction](#1-introduction)
- [1.1. What is Pipe?](#11-what-is-pipe)
- [1.2. How Pipe Works in Kernel](#12-how-pipe-works-in-kernel)
- [1.3. Pipe Capacity](#13-pipe-capacity)

### [2. Anonymous Pipe](#2-anonymous-pipe)
- [2.1. Create Pipe (pipe())](#21-create-pipe-pipe)
- [2.2. Pipe với fork() - Parent ghi, Child đọc](#22-pipe-với-fork---parent-ghi-child-đọc)
- [2.3. Bidirectional Communication (2 pipes)](#23-bidirectional-communication-2-pipes)
- [2.4. Pipe với exec() - Kết nối với program khác](#24-pipe-với-exec---kết-nối-với-program-khác)

### [3. Shell Pipe Simulation (dup2)](#3-shell-pipe-simulation-dup2)
- [3.1. dup() và dup2() System Calls](#31-dup-và-dup2-system-calls)
- [3.2. Mô phỏng `ls | wc -l`](#32-mô-phỏng-ls--wc--l)

### [4. popen() - Simplified Pipe](#4-popen---simplified-pipe)
- [4.1. popen() và pclose()](#41-popen-và-pclose)
- [4.2. Code Example: Đọc output của shell command](#42-code-example-đọc-output-của-shell-command)

### [5. FIFO (Named Pipe)](#5-fifo-named-pipe)
- [5.1. What is FIFO?](#51-what-is-fifo)
- [5.2. Create FIFO from Shell](#52-create-fifo-from-shell)
- [5.3. Create FIFO from Code](#53-create-fifo-from-code)
- [5.4. Blocking Behavior của FIFO](#54-blocking-behavior-của-fifo)
- [5.5. Code Example: Producer-Consumer với FIFO](#55-code-example-producer-consumer-với-fifo)

### [6. Client-Server Model with FIFOs](#6-client-server-model-with-fifos)
- [6.1. Well-known FIFO + Private FIFO](#61-well-known-fifo--private-fifo)
- [6.2. Code Example: FIFO Server](#62-code-example-fifo-server)
- [6.3. Code Example: FIFO Client](#63-code-example-fifo-client)

### [7. Non-blocking I/O với Pipe/FIFO](#7-non-blocking-io-với-pipefifo)
- [7.1. Set Non-blocking Mode](#71-set-non-blocking-mode)
- [7.2. O_NONBLOCK với FIFO](#72-o_nonblock-với-fifo)

### [8. So sánh Pipe vs FIFO vs Socket](#8-so-sánh-pipe-vs-fifo-vs-socket)

---

# 1. Introduction

## 1.1. What is Pipe?

**Pipe** (ống dẫn) là cơ chế IPC lâu đời nhất trên Unix/Linux.

- Nó tạo ra một kênh dữ liệu **đơn chiều** (unidirectional data channel).
- Dữ liệu được ghi vào một đầu (write-end) và đọc ra ở đầu kia (read-end).
- **Anonymous Pipe**: Chỉ tồn tại trong bộ nhớ kernel, không có tên trên filesystem. Thường dùng giữa process cha và con.

```
Process A (Writer)           Pipe Buffer            Process B (Reader)
     │                    ┌──────────────┐                │
     │    write(fd[1])    │  ┌──┬──┬──┐  │   read(fd[0]) │
     ├───────────────────►│  │H │E │L │  ├───────────────►│
     │                    │  │L │O │\n│  │                │
     │                    │  └──┴──┴──┘  │                │
     │                    │  FIFO Order  │                │
     ▼                    └──────────────┘                ▼
   fd[1] (write)              Kernel Buffer            fd[0] (read)
```

## 1.2. How Pipe Works in Kernel

- Về mặt kernel, pipe là một **bộ đệm (buffer) tròn** trong memory.
- Dữ liệu ghi vào pipe theo kiểu **byte stream** (FIFO - First In First Out).
- Nếu pipe đầy, process ghi sẽ bị **block**.
- Nếu pipe rỗng, process đọc sẽ bị **block** (trừ khi set non-blocking).
- Pipe có tính chất **một chiều**: data chỉ đi từ fd[1] → fd[0].

```
Pipe Kernel Buffer Layout:
┌──────────────────────────────────────────────────────┐
│                                                       │
│    write(fd[1])                                       │
│         │                                             │
│         ▼                                             │
│    ┌───────────────────────────────────────┐          │
│    │   │ d │ a │ t │ a │   │   │   │   │   │          │
│    └───▲───────────────────────────────────┘          │
│         │            write_pos    read_pos             │
│         │              │             │                 │
│    ┌────┴──────────────┴─────────────┴─────────┐       │
│    │       Circular Buffer (e.g., 64KB)         │       │
│    └────────────────────────────────────────────┘       │
│                                                       │
│         read(fd[0])                                    │
│         │                                             │
│         ▼                                             │
└──────────────────────────────────────────────────────┘
```

## 1.3. Pipe Capacity

Dung lượng pipe trong Linux:

| Kernel Version | Pipe Capacity (default) | Ghi chú                      |
| -------------- | ----------------------- | ---------------------------- |
| < 2.6.11       | 4 KB (4096 bytes)       |                              |
| 2.6.11 - 2.6.35| 64 KB (65536 bytes)     | 16 memory pages              |
| >= 2.6.35      | 16 * PAGE_SIZE          | Thường 65536 bytes (64KB)    |
| Từ 5.x+        | Có thể tăng lên 1MB+    | Dùng fcntl với F_SETPIPE_SZ  |

```bash
# Xem pipe capacity (hệ thống)
ulimit -p
# Output: 8  (số pages, mỗi page 8KB = 64KB)

# Xem pipe capacity (trong code)
#include <unistd.h>
long pipe_max = fpathconf(fd[0], _PC_PIPE_BUF);
printf("PIPE_BUF: %ld\n", pipe_max);  // POSIX minimum 512 bytes
```

**PIPE_BUF**: Số bytes tối đa có thể ghi **atomically** (không bị trộn lẫn với process khác). POSIX yêu cầu tối thiểu 512 bytes, Linux thường là 4096 bytes.

```c
// Tăng pipe size lên 1MB
#include <fcntl.h>
int fd[2];
pipe(fd);
fcntl(fd[1], F_SETPIPE_SZ, 1048576);  // 1MB

long size = fcntl(fd[1], F_GETPIPE_SZ);
printf("Pipe size: %ld bytes\n", size);
```

---

# 2. Anonymous Pipe

## 2.1. Create Pipe (pipe())

```c
#include <unistd.h>

int pipe(int pipefd[2]);
```

- `pipefd[0]`: Là File Descriptor dùng để **Đọc**.
- `pipefd[1]`: Là File Descriptor dùng để **Ghi**.
- Trả về 0 nếu thành công, -1 nếu lỗi.

```
Before pipe():
   Process
   ┌──────────┐
   │  stdin=0 │
   │ stdout=1 │
   │ stderr=2 │
   │   ...    │
   └──────────┘

After pipe(pipefd):
   Process
   ┌──────────┐
   │  stdin=0 │
   │ stdout=1 │
   │ stderr=2 │
   │ pipefd[0]│─────► (read end)
   │ pipefd[1]│─────► (write end)
   └──────────┘
```

## 2.2. Pipe với fork() - Parent ghi, Child đọc

Sau khi gọi `pipe()`, thường process sẽ gọi `fork()`.
- Process cha đóng một đầu (vd: đọc).
- Process con đóng đầu kia (vd: ghi).
- Kết quả là luồng dữ liệu một chiều.

```
Before fork (after pipe):
   Parent Process
   ┌────────────────────┐
   │  pipefd[0] (read)  │
   │  pipefd[1] (write) │
   └─────────┬──────────┘
             │ fork()
             ▼
   ┌────────────────────┐  ┌────────────────────┐
   │    Parent          │  │    Child           │
   │  close(fd[0])      │  │  close(fd[1])      │
   │  write(fd[1], ...) │─►│  read(fd[0], ...)  │
   └────────────────────┘  └────────────────────┘
   (Writer)                 (Reader)
```

### Quy tắc quan trọng khi dùng Pipe:

1. **Luôn đóng đầu không sử dụng**: Nếu chỉ ghi thì đóng fd[0], nếu chỉ đọc thì đóng fd[1].
2. **Đóng write-end để gửi EOF**: Khi tất cả write-ends được đóng, `read()` trả về 0 (EOF).
3. **Kiểm tra lỗi**: Pipe có capacity giới hạn, nếu không đọc kịp, write sẽ block.

### Code Example: Cơ bản

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main() {
    int pipefd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];
    const char *message = "Hello Child, I am Parent!";

    // 1. Create pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // 2. Fork
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // PARENT: Writer
        close(pipefd[0]);  // Close read end

        printf("Parent writing: %s\n", message);
        write(pipefd[1], message, strlen(message) + 1);  // +1 for null terminator

        close(pipefd[1]);  // Close write end (sends EOF)
        wait(NULL);        // Wait for child
    } else {
        // CHILD: Reader
        close(pipefd[1]);  // Close write end

        ssize_t n = read(pipefd[0], buffer, BUFFER_SIZE);
        if (n > 0) {
            printf("Child received: %s\n", buffer);
        }

        close(pipefd[0]);
    }

    return 0;
}
```

**Output:**
```
Parent writing: Hello Child, I am Parent!
Child received: Hello Child, I am Parent!
```

## 2.3. Bidirectional Communication (2 pipes)

Pipe chuẩn là đơn chiều. Để giao tiếp 2 chiều (Full Duplex) giữa Cha và Con, ta cần **2 Pipes**:

```
   Parent Process                    Child Process
   ┌──────────────────┐              ┌──────────────────┐
   │  pipe1: write ───┼──────────────┼──► pipe1: read   │
   │                  │              │                  │
   │  pipe2: read  ◄─┼──────────────┼─── pipe2: write  │
   └──────────────────┘              └──────────────────┘
```

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int pipe_parent_to_child[2], pipe_child_to_parent[2];
    pid_t pid;
    char buffer[1024];

    // Create 2 pipes
    if (pipe(pipe_parent_to_child) == -1 || pipe(pipe_child_to_parent) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == 0) {
        // Child
        close(pipe_parent_to_child[1]);  // Close write of pipe1
        close(pipe_child_to_parent[0]);  // Close read of pipe2

        // Read from parent
        read(pipe_parent_to_child[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);

        // Write back
        write(pipe_child_to_parent[1], "Hello back!", 12);

        close(pipe_parent_to_child[0]);
        close(pipe_child_to_parent[1]);
    } else {
        // Parent
        close(pipe_parent_to_child[0]);  // Close read of pipe1
        close(pipe_child_to_parent[1]);  // Close write of pipe2

        // Write to child
        write(pipe_parent_to_child[1], "Hello Child!", 13);

        // Read response
        read(pipe_child_to_parent[0], buffer, sizeof(buffer));
        printf("Parent received: %s\n", buffer);

        close(pipe_parent_to_child[1]);
        close(pipe_child_to_parent[0]);
        wait(NULL);
    }

    return 0;
}
```

**Output:**
```
Child received: Hello Child!
Parent received: Hello back!
```

## 2.4. Pipe với exec() - Kết nối với program khác

Pipe có thể kết nối output của một process với input của process khác thông qua `fork()` + `exec()`:

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t pid;

    pipe(pipefd);
    pid = fork();

    if (pid == 0) {
        // Child: redirect stdout to pipe
        close(pipefd[0]);            // Close read end
        dup2(pipefd[1], STDOUT_FILENO);  // stdout → pipe write
        close(pipefd[1]);            // Close original fd

        // Execute 'ls -l' - output goes to pipe
        execlp("ls", "ls", "-l", NULL);
        perror("execlp");
        _exit(1);
    } else {
        // Parent: read from pipe
        close(pipefd[1]);  // Close write end

        char buffer[4096];
        ssize_t n;
        while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, n);  // Display output
        }

        close(pipefd[0]);
        wait(NULL);
    }

    return 0;
}
```

---

# 3. Shell Pipe Simulation (dup2)

## 3.1. dup() và dup2() System Calls

```c
#include <unistd.h>

int dup(int oldfd);             // Copy fd, dùng số thấp nhất còn trống
int dup2(int oldfd, int newfd); // Copy oldfd vào newfd (đóng newfd trước nếu cần)
```

**dup2()** rất hữu ích để **redirect** I/O:
- `dup2(pipefd[1], STDOUT_FILENO)`: Redirect stdout → pipe
- `dup2(pipefd[0], STDIN_FILENO)`: Redirect pipe → stdin

## 3.2. Mô phỏng `ls | wc -l`

```
Shell command: ls -l | wc -l

Thực tế:
   Process: ls -l              Process: wc -l
   ┌──────────────┐           ┌──────────────┐
   │   stdout ────┼───pipe────┼───► stdin     │
   └──────────────┘           └──────────────┘
```

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t pid1, pid2;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // First child: ls -l → pipe (write)
    pid1 = fork();
    if (pid1 == 0) {
        close(pipefd[0]);                // Close read end
        dup2(pipefd[1], STDOUT_FILENO);  // stdout → pipe
        close(pipefd[1]);
        execlp("ls", "ls", "-l", NULL);
        perror("execlp ls");
        _exit(1);
    }

    // Second child: wc -l ← pipe (read)
    pid2 = fork();
    if (pid2 == 0) {
        close(pipefd[1]);                // Close write end
        dup2(pipefd[0], STDIN_FILENO);   // pipe → stdin
        close(pipefd[0]);
        execlp("wc", "wc", "-l", NULL);
        perror("execlp wc");
        _exit(1);
    }

    // Parent: close both ends, wait for children
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}
```

**Output:**
```
[Kết quả của 'ls -l | wc -l', ví dụ:]
15
```

### Giải thích từng bước:

```
Step 1: pipe() tạo pipe (fd[0]=3 đọc, fd[1]=4 ghi)
Step 2: fork() ls → con PID1
   PID1: close(3) → dup2(4, 1) → stdout(1) now points to pipe
Step 3: fork() wc → con PID2
   PID2: close(4) → dup2(3, 0) → stdin(0) now reads from pipe
Step 4: Close pipe in parent, wait
```

---

# 4. popen() - Simplified Pipe

## 4.1. popen() và pclose()

`popen()` là cách đơn giản hơn để chạy một shell command và kết nối pipe với nó.

```c
#include <stdio.h>

FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);
```

- `command`: Shell command string (vd: "ls -l")
- `type`: "r" (đọc output của command) hoặc "w" (ghi input cho command)
- Returns: `FILE *` stream hoặc NULL (lỗi)

## 4.2. Code Example: Đọc output của shell command

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Đọc output của 'ls -l'
    FILE *fp = popen("ls -l", "r");
    if (fp == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("LINE: %s", buffer);
    }

    int status = pclose(fp);
    printf("\nCommand exited with status: %d\n", status);

    return 0;
}
```

### popen() với "w" mode - Ghi input vào command:

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Ghi dữ liệu vào 'grep' command
    FILE *fp = popen("grep error", "w");
    if (fp == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    // Ghi các dòng vào grep
    fprintf(fp, "everything is fine\n");
    fprintf(fp, "error: file not found\n");
    fprintf(fp, "all good\n");
    fprintf(fp, "critical error: out of memory\n");

    pclose(fp);  // Grep sẽ in ra các dòng có "error"

    return 0;
}
```

**Output:**
```
error: file not found
critical error: out of memory
```

### So sánh popen() vs pipe() + fork():

| popen()                          | pipe() + fork() + exec()         |
| -------------------------------- | -------------------------------- |
| Đơn giản, ít dòng code           | Phức tạp hơn                     |
| Chạy command qua shell (`/bin/sh`)| Chạy trực tiếp executable        |
| Không control được chi tiết      | Kiểm soát hoàn toàn FD, redirect |
| Dùng `printf/scanf` với stream   | Dùng `read/write` với raw FD     |
| Xử lý lỗi hạn chế                | Xử lý lỗi chi tiết               |

---

# 5. FIFO (Named Pipe)

## 5.1. What is FIFO?

**FIFO** (First In, First Out) hay còn gọi là **Named Pipe**.

- Khác với Anonymous Pipe, FIFO **có tên** và tồn tại như một file đặc biệt trên Filesystem.
- Vì có tên, nên **bất kỳ process nào** cũng có thể mở nó (miễn là có quyền permission), không cần quan hệ huyết thống cha-con.
- Dữ liệu vẫn nằm trong kernel buffer, không ghi xuống đĩa (file chỉ là điểm truy cập).

```
Process A (Producer)               Process B (Consumer)
     │                                 │
     │    open("my_fifo", O_WRONLY)     │
     ├─────────────────────────────────►│
     │    write(fd, data, len)          │
     │    ─────────────────────────►    │
     │                                 │
     │                                 │  open("my_fifo", O_RDONLY)
     │                                 │  read(fd, buffer, size)
     │                                 │
     ▼                                 ▼
  Kernel pipe buffer (ring buffer)
  ┌────────────────────────────────────┐
  │ d1 │ d2 │ d3 │ ... │              │
  └────────────────────────────────────┘
```

## 5.2. Create FIFO from Shell

```bash
# Tạo FIFO
mkfifo my_fifo

# Kiểm tra (chữ 'p' ở đầu permissions)
ls -l my_fifo
# prw-r--r-- 1 user user 0 Dec 15 10:00 my_fifo

# Thử nghiệm 2 terminal:
# Terminal 1 (Reader - sẽ block đợi dữ liệu)
cat my_fifo

# Terminal 2 (Writer)
echo "Hello via FIFO!" > my_fifo
```

## 5.3. Create FIFO from Code

```c
#include <sys/types.h>
#include <sys/stat.h>

int mkfifo(const char *pathname, mode_t mode);
```

- `pathname`: Đường dẫn file FIFO.
- `mode`: Permission (vd `0666`).
- Return: 0 (thành công), -1 (lỗi).
- Sau khi tạo, dùng `open()`, `read()`, `write()`, `close()` như file bình thường.

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_PATH "/tmp/my_fifo"

int main() {
    // Create FIFO
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        perror("mkfifo");
        // EEXIST là OK nếu FIFO đã tồn tại
        exit(EXIT_FAILURE);
    }

    printf("FIFO created at %s\n", FIFO_PATH);
    printf("Open another terminal and run: cat %s\n", FIFO_PATH);

    // Open FIFO (block until reader opens)
    int fd = open(FIFO_PATH, O_WRONLY);
    write(fd, "Hello through FIFO!\n", 20);
    close(fd);

    unlink(FIFO_PATH);  // Remove FIFO
    return 0;
}
```

## 5.4. Blocking Behavior của FIFO

Một trong những điểm quan trọng nhất cần hiểu về FIFO là **blocking behavior** của `open()`:

```c
// Mở để đọc: BLOCK cho đến khi có writer mở FIFO
int fd = open("/tmp/my_fifo", O_RDONLY);
// Hàm này sẽ không return cho đến khi có process khác mở FIFO để ghi!

// Mở để ghi: BLOCK cho đến khi có reader mở FIFO
int fd = open("/tmp/my_fifo", O_WRONLY);
// Hàm này sẽ không return cho đến khi có process khác mở FIFO để đọc!
```

**Với O_NONBLOCK:**

```c
int fd = open("/tmp/my_fifo", O_RDONLY | O_NONBLOCK);
// Return ngay lập tức (không cần writer)

int fd = open("/tmp/my_fifo", O_WRONLY | O_NONBLOCK);
// Return -1 với errno = ENXIO nếu chưa có reader
```

## 5.5. Code Example: Producer-Consumer với FIFO

### Producer:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_PATH "/tmp/producer_fifo"

int main() {
    // Create FIFO
    mkfifo(FIFO_PATH, 0666);

    int fd = open(FIFO_PATH, O_WRONLY);
    printf("Producer: FIFO opened for writing\n");

    const char *items[] = {"Apple", "Banana", "Cherry", "exit"};
    for (int i = 0; i < 4; i++) {
        write(fd, items[i], strlen(items[i]) + 1);
        printf("Produced: %s\n", items[i]);
        sleep(1);
    }

    close(fd);
    unlink(FIFO_PATH);
    return 0;
}
```

### Consumer:

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_PATH "/tmp/producer_fifo"

int main() {
    // Wait for FIFO to be created, then open
    printf("Consumer: waiting for FIFO...\n");
    int fd = open(FIFO_PATH, O_RDONLY);
    printf("Consumer: connected\n");

    char buffer[256];
    while (1) {
        ssize_t n = read(fd, buffer, sizeof(buffer));
        if (n <= 0) break;  // EOF or error

        if (strcmp(buffer, "exit") == 0) break;
        printf("Consumed: %s\n", buffer);
    }

    close(fd);
    return 0;
}
```

---

# 6. Client-Server Model with FIFOs

## 6.1. Well-known FIFO + Private FIFO

Do tính chất "Named", FIFO rất tốt để làm điểm giao tiếp Server-Client.

```
                    ┌─────────────────┐
                    │     Server      │
                    │                 │
                    │ /tmp/srv_fifo   │ (Well-known FIFO - public)
                    └──────┬──────────┘
                           │
          ┌────────────────┼────────────────┐
          │                │                │
    ┌─────▼──────┐  ┌─────▼──────┐  ┌─────▼──────┐
    │ Client 1   │  │ Client 2   │  │ Client 3   │
    │ /tmp/c1.123 │  │ /tmp/c2.456 │  │ /tmp/c3.789 │
    └────────────┘  └────────────┘  └────────────┘
    Private FIFOs (mỗi client tạo riêng)
```

**Well-known FIFO**: Server tạo một FIFO cố định. Tất cả Clients đều biết tên FIFO này để gửi request.

**Private FIFO**: Để Server phản hồi cho đúng Client, Client cần tạo một FIFO riêng, gửi request kèm tên Private FIFO.

## 6.2. Code Example: FIFO Server

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define SERVER_FIFO "/tmp/server_fifo"
#define MAX_BUFFER  256

typedef struct {
    char client_fifo[MAX_BUFFER];  // FIFO để gửi response
    char data[MAX_BUFFER];         // Request data
} Request;

int main() {
    // Create server FIFO
    unlink(SERVER_FIFO);
    mkfifo(SERVER_FIFO, 0666);

    int server_fd = open(SERVER_FIFO, O_RDONLY);
    if (server_fd == -1) {
        perror("open server FIFO");
        exit(EXIT_FAILURE);
    }
    printf("Server: listening on %s\n", SERVER_FIFO);

    Request req;
    while (read(server_fd, &req, sizeof(Request)) > 0) {
        printf("Server: received request: %s\n", req.data);

        // Open client's private FIFO and send response
        int client_fd = open(req.client_fifo, O_WRONLY);
        if (client_fd != -1) {
            char response[MAX_BUFFER];
            snprintf(response, sizeof(response),
                     "Response to: %s", req.data);
            write(client_fd, response, strlen(response) + 1);
            close(client_fd);
        }
    }

    close(server_fd);
    unlink(SERVER_FIFO);
    return 0;
}
```

## 6.3. Code Example: FIFO Client

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define SERVER_FIFO "/tmp/server_fifo"
#define MAX_BUFFER  256

int main() {
    // Create private FIFO for response
    char private_fifo[MAX_BUFFER];
    snprintf(private_fifo, sizeof(private_fifo),
             "/tmp/client_%d_fifo", getpid());
    mkfifo(private_fifo, 0666);

    // Send request to server
    int server_fd = open(SERVER_FIFO, O_WRONLY);
    if (server_fd == -1) {
        perror("open server FIFO");
        unlink(private_fifo);
        exit(EXIT_FAILURE);
    }

    Request req;
    strncpy(req.client_fifo, private_fifo, MAX_BUFFER);
    strncpy(req.data, "Hello Server!", MAX_BUFFER);
    write(server_fd, &req, sizeof(Request));
    close(server_fd);
    printf("Client: request sent\n");

    // Read response from private FIFO
    int client_fd = open(private_fifo, O_RDONLY);
    if (client_fd != -1) {
        char response[MAX_BUFFER];
        read(client_fd, response, sizeof(response));
        printf("Client: received response: %s\n", response);
        close(client_fd);
    }

    unlink(private_fifo);
    return 0;
}
```

**Compile & Run:**
```bash
gcc fifo_server.c -o fifo_server
gcc fifo_client.c -o fifo_client

./fifo_server &
./fifo_client
```

---

# 7. Non-blocking I/O với Pipe/FIFO

## 7.1. Set Non-blocking Mode

```c
#include <fcntl.h>

// Cách 1: Khi open FIFO
int fd = open("/tmp/my_fifo", O_RDONLY | O_NONBLOCK);

// Cách 2: Dùng fcntl (cho pipe)
int fd[2];
pipe(fd);
int flags = fcntl(fd[0], F_GETFL, 0);
fcntl(fd[0], F_SETFL, flags | O_NONBLOCK);
```

### Ví dụ: Non-blocking read từ pipe

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main() {
    int pipefd[2];
    char buffer[1024];

    pipe(pipefd);

    // Set non-blocking
    int flags = fcntl(pipefd[0], F_GETFL, 0);
    fcntl(pipefd[0], F_SETFL, flags | O_NONBLOCK);

    // Thử đọc từ pipe rỗng (sẽ không block)
    ssize_t n = read(pipefd[0], buffer, sizeof(buffer));
    if (n == -1 && errno == EAGAIN) {
        printf("Pipe is empty (no data, not an error)\n");
    }

    // Ghi dữ liệu
    write(pipefd[1], "data", 5);

    // Đọc lại (lần này thành công)
    n = read(pipefd[0], buffer, sizeof(buffer));
    if (n > 0) {
        printf("Read %zd bytes: %s\n", n, buffer);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    return 0;
}
```

## 7.2. O_NONBLOCK với FIFO

| Scenario                              | Blocking (default)            | O_NONBLOCK                      |
| ------------------------------------- | ------------------------------ | ------------------------------- |
| **open() cho read** (chưa có writer)  | Block cho đến khi có writer    | Open thành công ngay             |
| **open() cho write** (chưa có reader) | Block cho đến khi có reader    | Return -1, errno = ENXIO        |
| **read()** không có dữ liệu           | Block cho đến khi có dữ liệu   | Return -1, errno = EAGAIN       |
| **write()** pipe đầy                  | Block cho đến khi có chỗ trống | Return -1, errno = EAGAIN       |

---

# 8. So sánh Pipe vs FIFO vs Socket

| Tiêu chí              | Anonymous Pipe              | FIFO (Named Pipe)          | Unix Domain Socket           |
| --------------------- | ---------------------------- | --------------------------- | ---------------------------- |
| **Định danh**         | Không có tên                 | File path trong FS          | File path trong FS           |
| **Process relationship** | Cần quan hệ cha-con       | Không cần                   | Không cần                    |
| **Hướng dữ liệu**     | Đơn chiều                    | Đơn chiều                   | Song chiều (stream)          |
| **Số process**        | 2 (thường)                   | 2 hoặc nhiều                | 2 hoặc nhiều                 |
| **Persistent**        | Không (theo process)         | Có (trên filesystem)        | Có (socket file)             |
| **Giao tiếp mạng**    | Không                        | Không                       | Có (AF_INET)                 |
| **Tốc độ**            | Cao (kernel buffer)          | Cao (kernel buffer)         | Trung bình (phụ thuộc domain)|
| **Dữ liệu**           | Byte stream                  | Byte stream                 | Byte stream / Datagram       |

---

## 📚 Tài liệu tham khảo

- *The Linux Programming Interface* - Michael Kerrisk (Chapters 44-46)
- *Advanced Programming in the UNIX Environment* - W. Richard Stevens
- Linux man pages: `man 7 pipe`, `man 2 pipe`, `man 3 mkfifo`, `man 3 popen`, `man 2 dup2`
- [Linux Pipe Capacity](https://man7.org/linux/man-pages/man7/pipe.7.html)
