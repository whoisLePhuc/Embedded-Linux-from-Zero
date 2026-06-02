# Embedded Linux - IPC Signal

> Tài liệu học về Linux Signal: Signal basics, Signal Lifecycle, Signal Handler, Sending Signal, và Signal Blocking/Unblocking.

---

## 📑 Mục Lục

### [1. Introduction](#1-introduction)
- [1.1. What is Signal?](#11-what-is-signal)
- [1.2. Signal Lifecycle](#12-signal-lifecycle)

### [2. Signal Handler](#2-signal-handler)
- [2.1. System call signal()](#21-system-call-signal)

### [3. Some common Signal](#3-some-common-signal)
- [3.1. Some common signals](#31-some-common-signals)

### [4. Sending Signal](#4-sending-signal)
- [4.1. System call kill()](#41-system-call-kill)

### [5. Blocking and unblocking signal](#5-blocking-and-unblocking-signal)
- [5.1. Definition](#51-definition)
- [5.2. Blocking & Unblocking signal](#52-blocking--unblocking-signal)

---

# 1. Introduction

## 1.1. What is Signal?

### Signal là gì?

**Signal** (Tín hiệu) là một cơ chế "ngắt mềm" (software interrupt) được sử dụng để thông báo cho một process biết rằng một sự kiện nào đó đã xảy ra. Signal có thể được gửi từ kernel đến process, hoặc từ một process này sang process khác.

Khi một process nhận được signal, nó sẽ tạm dừng công việc hiện tại để xử lý signal đó (hoặc bị terminate tùy theo loại signal).

### Đặc điểm của Signal:
- **Asynchronous**: Signal có thể đến bất cứ lúc nào, không đồng bộ với luồng thực thi của chương trình.
- **Inter-process Communication (IPC)**: Là một dạng IPC đơn giản nhất nhưng hạn chế (chỉ gửi được số hiệu signal, không gửi được dữ liệu phức tạp).
- **Default Actions**: Mỗi signal có một hành động mặc định (ví dụ: Terminate, Ignore, Stop).

## 1.2. Signal Lifecycle

Vòng đời của một signal bao gồm 3 giai đoạn chính:

1. **Generation (Tạo ra)**: Signal được tạo ra bởi một sự kiện (ví dụ: user nhấn Ctrl+C, lỗi chia cho 0) hoặc bởi một lệnh/hàm (lệnh `kill`, hàm `kill()`).
2. **Pending (Chờ xử lý)**: Signal đã được tạo ra nhưng chưa được chuyển đến process. Điều này xảy ra khi signal bị block (blocked).
3. **Delivery (Chuyển giao)**: Signal được chuyển đến process và process thực hiện hành động xử lý (handler, ignore, hoặc default action).

```
Signal Lifecycle:

   Event Occurs (e.g., Ctrl+C)
         │
         ▼
   ┌─────────────┐
   │ Generation  │ Signal được tạo ra
   └─────┬───────┘
         │
         ▼
   ┌─────────────┐  <-- Nếu Signal bị Block, nó sẽ nằm ở đây
   │   Pending   │      cho đến khi được Unblock
   └─────┬───────┘
         │
         ▼
   ┌─────────────┐
   │  Delivery   │  Kernel chuyển signal cho Process
   └─────┬───────┘
         │
         ▼
   ┌───────────────────────┐
   │ Action / Handling     │
   │ 1. Ignore             │
   │ 2. Default Action     │
   │ 3. Catch (Handler)    │
   └───────────────────────┘
```

---

# 2. Signal Handler

## 2.1. System call signal()

Để thay đổi hành động mặc định khi nhận signal (ví dụ: chạy một hàm riêng), ta sử dụng **Signal Handler**.

### Prototype

```c
#include <signal.h>

typedef void (*sighandler_t)(int);

sighandler_t signal(int signum, sighandler_t handler);
```

| Parameter | Mô tả                                                      |
| --------- | ---------------------------------------------------------- |
| `signum`  | Số hiệu của signal muốn bắt (ví dụ: `SIGINT`, `SIGTERM`).  |
| `handler` | Hàm xử lý signal (`SIG_DFL`, `SIG_IGN`, hoặc địa chỉ hàm). |

### Các giá trị của handler:
- `SIG_DFL`: Khôi phục hành động mặc định của signal.
- `SIG_IGN`: Bỏ qua (Ignore) signal.
- **Function Pointer**: Hàm do user định nghĩa sẽ được gọi khi signal đến.

### Ví dụ: Bắt signal SIGINT (Ctrl+C)

```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Hàm xử lý signal
void sigint_handler(int signum) {
    printf("\nCaught signal SIGINT (%d). Press Ctrl+\\ to quit.\n", signum);
}

int main() {
    // Đăng ký hàm xử lý cho SIGINT
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("Cannot handle SIGINT");
        exit(EXIT_FAILURE);
    }

    printf("Process ID: %d\n", getpid());
    printf("Press Ctrl+C to send SIGINT...\n");

    while (1) {
        printf("Working...\n");
        sleep(2);
    }

    return 0;
}
```

**Output:**
```
Process ID: 1234
Press Ctrl+C to send SIGINT...
Working...
Working...
^C
Caught signal SIGINT (2). Press Ctrl+\ to quit.
Working...
```

---

# 3. Some common Signal

## 3.1. Some common signals

Dưới đây là các signal phổ biến trong Linux (xem đầy đủ bằng lệnh `kill -l`):

| Signal Name | ID  | Default Action | Mô tả                                                                                 |
| ----------- | --- | -------------- | ------------------------------------------------------------------------------------- |
| **SIGINT**  | 2   | Terminate      | Interrupt từ bàn phím (Ctrl+C).                                                       |
| **SIGQUIT** | 3   | Core Dump      | Quit từ bàn phím (Ctrl+\).                                                            |
| **SIGILL**  | 4   | Core Dump      | Illegal Instruction - Lệnh không hợp lệ.                                              |
| **SIGKILL** | 9   | Terminate      | Kill signal. **Không thể catch hoặc ignore**. Dùng để tiêu diệt process ngay lập tức. |
| **SIGSEGV** | 11  | Core Dump      | Segmentation Fault - Truy cập bộ nhớ trái phép.                                       |
| **SIGPIPE** | 13  | Terminate      | Ghi vào pipe không có người đọc.                                                      |
| **SIGALRM** | 14  | Terminate      | Timer signal từ `alarm()`.                                                            |
| **SIGTERM** | 15  | Terminate      | Termination signal. Đây là signal lịch sự để yêu cầu process kết thúc.                |
| **SIGCHLD** | 17  | Ignore         | Child process stopped or terminated. Parent nhận để `wait()`.                         |
| **SIGSTOP** | 19  | Stop           | Stop process. **Không thể catch hoặc ignore**.                                        |
| **SIGCONT** | 18  | Continue       | Tiếp tục process nếu nó đang bị stop.                                                 |

> **Lưu ý**: `SIGKILL` (9) và `SIGSTOP` (19) không thể bị bắt (catch), bị chặn (block) hoặc bị bỏ qua (ignore).

---

# 4. Sending Signal

## 4.1. System call kill()

Một process có thể gửi signal cho một process khác (nếu có quyền) bằng system call `kill()`.

### Prototype

```c
#include <sys/types.h>
#include <signal.h>

int kill(pid_t pid, int sig);
```

| Parameter | Mô tả                        |
| --------- | ---------------------------- |
| `pid`     | Process ID của process nhận. |
| `sig`     | Số hiệu signal cần gửi.      |

### Giá trị của PID:
- `pid > 0`: Gửi signal đến process có ID là `pid`.
- `pid == 0`: Gửi đến mọi process trong cùng process group với người gửi.
- `pid == -1`: Gửi đến mọi process mà user có quyền gửi (trừ process 1 - init).

### Ví dụ: Gửi signal SIGKILL

```c
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <pid>\n", argv[0]);
        return 1;
    }

    pid_t target_pid = atoi(argv[1]);

    printf("Sending SIGKILL to PID %d...\n", target_pid);
    
    if (kill(target_pid, SIGKILL) == 0) {
        printf("Signal sent successfully.\n");
    } else {
        perror("Failed to send signal");
    }

    return 0;
}
```

---

# 5. Blocking and unblocking signal

## 5.1. Definition

### Signal Mask (Mặt nạ tín hiệu)
Mỗi process (và mỗi thread) có một tập hợp các signal bị chặn, gọi là **Signal Mask**.
- Khi một signal bị block, nó sẽ được giữ ở trạng thái **Pending**.
- Signal sẽ không được gửi đến process cho đến khi nó được unblock.
- Việc block signal thường được dùng để bảo vệ các vùng **Critical Section** nơi mà việc nhận signal có thể gây lỗi dữ liệu.

### Sigset (Tập hợp signal)
Linux cung cấp kiểu dữ liệu `sigset_t` để biểu diễn một tập hợp các signal.

**Các hàm thao tác với `sigset_t`:**

```c
#include <signal.h>

int sigemptyset(sigset_t *set);       // Khởi tạo set rỗng
int sigfillset(sigset_t *set);        // Thêm tất cả signal vào set
int sigaddset(sigset_t *set, int signum); // Thêm signum vào set
int sigdelset(sigset_t *set, int signum); // Xóa signum khỏi set
int sigismember(const sigset_t *set, int signum); // Kiểm tra signum có trong set không
```

## 5.2. Blocking & Unblocking signal

### System call `sigprocmask()`

Dùng để thay đổi hoặc kiểm tra signal mask của process hiện tại.

```c
#include <signal.h>

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
```

| Parameter | Mô tả                                                            |
| --------- | ---------------------------------------------------------------- |
| `how`     | Cách thay đổi: `SIG_BLOCK`, `SIG_UNBLOCK`, `SIG_SETMASK`.        |
| `set`     | Con trỏ đến tập signal mới (NULL nếu chỉ muốn kiểm tra mask cũ). |
| `oldset`  | Lưu mask cũ trước khi thay đổi (NULL nếu không cần).             |

**Các mode của `how`:**
- `SIG_BLOCK`: Thêm các signal trong `set` vào mask hiện tại (`mask = mask | set`).
- `SIG_UNBLOCK`: Xóa các signal trong `set` khỏi mask hiện tại (`mask = mask & ~set`).
- `SIG_SETMASK`: Gán mask hiện tại bằng `set` (`mask = set`).

### Ví dụ: Block SIGINT trong 5 giây

```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
    sigset_t new_set, old_set;

    // 1. Khởi tạo set rỗng và thêm SIGINT vào set
    sigemptyset(&new_set);
    sigaddset(&new_set, SIGINT);

    // 2. Block SIGINT và lưu mask cũ
    printf("Blocking SIGINT for 5 seconds...\n");
    if (sigprocmask(SIG_BLOCK, &new_set, &old_set) < 0) {
        perror("sigprocmask");
        return 1;
    }

    // Trong 5s này, nếu nhấn Ctrl+C, signal sẽ bị Pending
    // chứ không terminate chương trình ngay.
    sleep(5);

    // 3. Kiểm tra xem có signal nào đang pending không
    sigset_t pending_set;
    sigpending(&pending_set);
    if (sigismember(&pending_set, SIGINT)) {
        printf("\nOop! SIGINT was pending. Only now it is delivered!\n");
    }

    // 4. Restore lại mask cũ (Unblock SIGINT)
    printf("Unblocking SIGINT...\n");
    if (sigprocmask(SIG_SETMASK, &old_set, NULL) < 0) {
        perror("sigprocmask");
        return 1;
    }

    // Nếu lúc nãy có nhấn Ctrl+C, ngay lập tức chương trình sẽ bị terminate ở đây
    printf("Program finished normally.\n");

    return 0;
}
```

**Output Scenario:**
1. Chạy chương trình.
2. Nhấn `Ctrl+C` trong vòng 5 giây đầu.
3. Không có gì xảy ra ngay lập tức (do bị Block).
4. Hết 5 giây, in ra thông báo pending.
5. Khi unblock, `SIGINT` được delivery -> Chương trình terminate.