# Embedded Linux - IPC Message Queues

> Tài liệu học về Linux Message Queues: So sánh System V vs POSIX, APIs, Code Examples, Message Notification, và so sánh chi tiết các cơ chế IPC.

---

## 📑 Mục Lục

### [1. Introduction](#1-introduction)
- [1.1. What is Message Queues?](#11-what-is-message-queues)
- [1.2. So sánh System V vs POSIX](#12-so-sánh-system-v-vs-posix)

### [2. System V Message Queues](#2-system-v-message-queues)
- [2.1. Implementation Workflow](#21-implementation-workflow)
- [2.2. Create Key (ftok)](#22-create-key-ftok)
- [2.3. Create/Get Queue (msgget)](#23-createget-queue-msgget)
- [2.4. Send Message (msgsnd)](#24-send-message-msgsnd)
- [2.5. Receive Message (msgrcv)](#25-receive-message-msgrcv)
- [2.6. Control Queue (msgctl)](#26-control-queue-msgctl)
- [2.7. Code Example: System V Sender](#27-code-example-system-v-sender)
- [2.8. Code Example: System V Receiver](#28-code-example-system-v-receiver)
- [2.9. Quản lý Queue với ipcs/ipcrm](#29-quản-lý-queue-với-ipcsipcrm)

### [3. POSIX Message Queues](#3-posix-message-queues)
- [3.1. Implementation Workflow](#31-implementation-workflow)
- [3.2. Open/Create Queue (mq_open)](#32-opencreate-queue-mq_open)
- [3.3. Set Attributes (mq_attr)](#33-set-attributes-mq_attr)
- [3.4. Send Message (mq_send)](#34-send-message-mq_send)
- [3.5. Receive Message (mq_receive)](#35-receive-message-mq_receive)
- [3.6. Close & Unlink (mq_close, mq_unlink)](#36-close--unlink-mq_close-mq_unlink)
- [3.7. Code Example: POSIX Sender](#37-code-example-posix-sender)
- [3.8. Code Example: POSIX Receiver](#38-code-example-posix-receiver)

### [4. Message Notification (mq_notify)](#4-message-notification-mq_notify)
- [4.1. Overview](#41-overview)
- [4.2. Code Example: mq_notify with Signal](#42-code-example-mq_notify-with-signal)

### [5. Review IPC Mechanisms](#5-review-ipc-mechanisms)
- [5.1. So sánh chi tiết IPC](#51-so-sánh-chi-tiết-ipc)
- [5.2. Khi nào dùng Message Queues?](#52-khi-nào-dùng-message-queues)

---

# 1. Introduction

## 1.1. What is Message Queues?

**Message Queue** là một danh sách liên kết các tin nhắn (messages) được lưu trữ trong kernel.

- Mỗi message queue được định danh bằng một identifier (ID).
- **Message-based**: Khác với Pipe (stream of bytes), Message Queue gửi dữ liệu theo từng gói tin (packet/message).
- **Type-based**: Mỗi message có thể kèm theo một `type`. Người nhận có thể chọn chỉ nhận message có `type` cụ thể.
- **Asynchronous**: Sender và Receiver không cần phải đợi nhau (như Socket/Pipe blocking). Sender gửi xong có thể làm việc khác.
- **Priority**: POSIX message queue hỗ trợ priority, message có priority cao hơn được nhận trước.

```
┌──────────────────────────────────────────────────────┐
│                     Kernel Space                      │
│                                                       │
│   Message Queue (System V / POSIX)                    │
│   ┌──────┬──────┬──────┬──────┬──────┐               │
│   │ Msg1 │ Msg2 │ Msg3 │ ...  │ MsgN │               │
│   │ type:1│ type:2│type:1│      │type:3│               │
│   └──────┴──────┴──────┴──────┴──────┘               │
│         ▲                                    ▲        │
│         │ send()                            │ recv()  │
│         │                                    │        │
│   ┌─────┴──────┐                    ┌────────┴──────┐ │
│   │  Sender    │                    │   Receiver    │ │
│   │  Process A │                    │   Process B   │ │
│   └────────────┘                    └───────────────┘ │
└──────────────────────────────────────────────────────┘
```

Có 2 chuẩn Message Queue trong Linux:
1. **System V (cũ)**: Phức tạp hơn, dùng key (`ftok`).
2. **POSIX (mới)**: API đơn giản hơn, dùng name (`/my_queue`), hỗ trợ real-time.

## 1.2. So sánh System V vs POSIX

| Tiêu chí                | System V                     | POSIX                           |
| ----------------------- | ---------------------------- | ------------------------------- |
| **Định danh**           | key (ftok) + msgid           | Tên file (/my_queue)            |
| **Cấu trúc message**    | Bắt buộc có `long mtype`     | Không cần type (priority riêng) |
| **Priority**            | Qua mtype (số > 0)           | msg_prio (0-31, số lớn = ưu tiên cao) |
| **Maximum message size**| pagesize - 8 bytes           | Cấu hình qua mq_attr            |
| **Notification**        | Không có                     | mq_notify (signal/thread)       |
| **Thread-safe**         | Không                        | Có                             |
| **Kernel persistence**  | Có (đến khi xóa hoặc reboot) | Có (đến khi unlink)            |
| **Linker**              | Không cần                     | -lrt                           |

---

# 2. System V Message Queues

## 2.1. Implementation Workflow

Workflow cơ bản của System V Message Queue:
1. Tạo Key (`ftok`).
2. Lấy/Tạo Queue ID (`msgget`).
3. Gửi Message (`msgsnd`) hoặc Nhận Message (`msgrcv`).
4. Xóa/Quản lý Queue (`msgctl`).

```
┌──────────┐   ┌──────────┐   ┌───────────┐   ┌──────────┐   ┌──────────┐
│  ftok()  │──►│ msgget() │──►│ msgsnd()  │──►│ msgrcv() │──►│ msgctl() │
│  Tạo key │   │ Tạo queue│   │ Gửi tin   │   │ Nhận tin │   │ Xóa queue│
└──────────┘   └──────────┘   └───────────┘   └──────────┘   └──────────┘
```

## 2.2. Create Key (ftok)

Để các process khác nhau có thể truy cập cùng một IPC object, chúng cần một key chung.

```c
#include <sys/types.h>
#include <sys/ipc.h>

key_t ftok(const char *pathname, int proj_id);
```

- `pathname`: Đường dẫn tới một file **tồn tại** trên hệ thống.
- `proj_id`: Một số bất kỳ (thường dùng ký tự 'A', 'a'...).
- Returns: key (số nguyên dương) hoặc -1 (lỗi).

> **Lưu ý quan trọng**: Nếu file pathname bị xóa và tạo lại, ftok sẽ trả về key khác! Nên dùng file cố định như đường dẫn của chính chương trình.

```c
key_t key = ftok("/tmp/msg_queue_key", 'A');
if (key == -1) {
    perror("ftok failed");
    exit(EXIT_FAILURE);
}
```

## 2.3. Create/Get Queue (msgget)

```c
#include <sys/msg.h>

int msgget(key_t key, int msgflg);
```

- `key`: Key tạo bởi `ftok` hoặc `IPC_PRIVATE` (tạo queue riêng tư)
- `msgflg`: `0666 | IPC_CREAT` (tạo mới nếu chưa có), `IPC_CREAT | IPC_EXCL` (chỉ tạo mới, fail nếu đã tồn tại)
- Returns: `msgid` (số dương) hoặc -1 (lỗi).

```c
int msgid = msgget(key, 0666 | IPC_CREAT);
if (msgid == -1) {
    perror("msgget failed");
    exit(EXIT_FAILURE);
}
```

### Cấu trúc message bắt buộc:

```c
struct msgbuf {
    long mtype;       // Message type, must be > 0
    char mtext[1024]; // Message data (tên field có thể khác)
};
```

> Field đầu tiên **PHẢI** là `long mtype`. Phần dữ liệu phía sau có thể là bất kỳ struct nào.

## 2.4. Send Message (msgsnd)

```c
int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
```

- `msqid`: ID của queue (từ msgget).
- `msgp`: Con trỏ tới struct tin nhắn (struct có `long` ở đầu).
- `msgsz`: Kích thước phần dữ liệu, **không** tính phần `long mtype`.
- `msgflg`: `0` (block nếu queue đầy) hoặc `IPC_NOWAIT` (trả về lỗi ngay).

### Ví dụ gửi message với struct phức tạp:

```c
#define TEXT_SIZE 256

typedef struct {
    long mtype;
    int seq_num;
    char data[TEXT_SIZE];
    float value;
} MyMessage;

MyMessage msg;
msg.mtype = 1;      // Message type > 0
msg.seq_num = 42;
strcpy(msg.data, "Hello from sender!");
msg.value = 3.14;

if (msgsnd(msgid, &msg, sizeof(MyMessage) - sizeof(long), 0) == -1) {
    perror("msgsnd failed");
}
```

## 2.5. Receive Message (msgrcv)

```c
ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
```

- `msgtyp`: Quy tắc chọn message để nhận:
  - `0`: Nhận message **đầu tiên** trong queue (FIFO).
  - `> 0`: Nhận message đầu tiên có type **bằng** `msgtyp`.
  - `< 0`: Nhận message đầu tiên có type **nhỏ hơn hoặc bằng** `|msgtyp|` (ưu tiên số nhỏ nhất).

- `msgflg`:
  - `0`: Block nếu không có message phù hợp.
  - `IPC_NOWAIT`: Return -1 ngay nếu không có message.
  - `MSG_NOERROR`: Cắt bớt message nếu lớn hơn buffer.

```c
MyMessage msg;
ssize_t n = msgrcv(msgid, &msg, sizeof(MyMessage) - sizeof(long), 0, 0);
if (n == -1) {
    perror("msgrcv failed");
} else {
    printf("Received: type=%ld, seq=%d, data=%s, val=%.2f\n",
           msg.mtype, msg.seq_num, msg.data, msg.value);
}
```

## 2.6. Control Queue (msgctl)

```c
int msgctl(int msqid, int cmd, struct msqid_ds *buf);
```

| cmd           | Mô tả                                                        |
| ------------- | ------------------------------------------------------------ |
| `IPC_RMID`   | Xóa queue ngay lập tức. Các process đang chờ đọc/ghi nhận lỗi `EIDRM`. |
| `IPC_STAT`   | Lấy thông tin queue vào struct `msqid_ds`.                   |
| `IPC_SET`    | Set quyền (permissions) cho queue.                           |

```c
// Xóa queue
if (msgctl(msgid, IPC_RMID, NULL) == -1) {
    perror("msgctl IPC_RMID failed");
}

// Lấy thông tin queue
struct msqid_ds ds;
if (msgctl(msgid, IPC_STAT, &ds) == 0) {
    printf("Queue permissions: %o\n", ds.msg_perm.mode);
    printf("Messages in queue: %lu\n", ds.msg_qnum);
    printf("Max bytes: %lu\n", ds.msg_qbytes);
    printf("Last send: %s", ctime(&ds.msg_stime));
    printf("Last receive: %s", ctime(&ds.msg_rtime));
}
```

## 2.7. Code Example: System V Sender

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define QUEUE_KEY_PATH "/tmp"
#define PROJECT_ID 'A'

typedef struct {
    long mtype;
    char data[256];
} Message;

int main() {
    // 1. Tạo key
    key_t key = ftok(QUEUE_KEY_PATH, PROJECT_ID);
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // 2. Tạo/Lấy queue
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    printf("Sender: Queue ID = %d\n", msgid);

    // 3. Gửi các message với type khác nhau
    Message msg;
    char *messages[] = {"High priority message", "Normal message", "Low priority"};
    long types[] = {10, 5, 1};  // type 10 được nhận trước nếu dùng msgtyp < 0

    for (int i = 0; i < 3; i++) {
        msg.mtype = types[i];
        strcpy(msg.data, messages[i]);

        if (msgsnd(msgid, &msg, sizeof(msg.data), 0) == -1) {
            perror("msgsnd");
            continue;
        }
        printf("Sent: [type=%ld] %s\n", msg.mtype, msg.data);
    }

    return 0;
}
```

## 2.8. Code Example: System V Receiver

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define QUEUE_KEY_PATH "/tmp"
#define PROJECT_ID 'A'

typedef struct {
    long mtype;
    char data[256];
} Message;

int main() {
    // 1. Lấy key
    key_t key = ftok(QUEUE_KEY_PATH, PROJECT_ID);
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // 2. Lấy queue (giả sử đã được tạo bởi sender)
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    printf("Receiver: Queue ID = %d\n", msgid);

    // 3. Nhận message theo type (0 = đầu tiên, <0 = ưu tiên type nhỏ nhất)
    Message msg;

    // Nhận message có type <= 5 (type 1 và 5)
    printf("\nReceiving messages with type <= 5:\n");
    for (int i = 0; i < 2; i++) {
        if (msgrcv(msgid, &msg, sizeof(msg.data), -5, 0) > 0) {
            printf("Received: [type=%ld] %s\n", msg.mtype, msg.data);
        }
    }

    // Nhận message còn lại (type 10)
    printf("\nReceiving remaining message (type = 10):\n");
    if (msgrcv(msgid, &msg, sizeof(msg.data), 10, 0) > 0) {
        printf("Received: [type=%ld] %s\n", msg.mtype, msg.data);
    }

    // 4. Xóa queue
    msgctl(msgid, IPC_RMID, NULL);
    printf("\nQueue removed.\n");

    return 0;
}
```

**Compile & Run:**
```bash
gcc sysv_sender.c -o sysv_sender
gcc sysv_receiver.c -o sysv_receiver

# Terminal 1 (chạy sau để nhận)
./sysv_receiver

# Terminal 2 (chạy trước để gửi)
./sysv_sender
```

**Output:**
```
Sender: Queue ID = 0
Sent: [type=10] High priority message
Sent: [type=5] Normal message
Sent: [type=1] Low priority

Receiver: Queue ID = 0
Receiving messages with type <= 5:
Received: [type=1] Low priority          # type nhỏ nhất được nhận trước
Received: [type=5] Normal message
Receiving remaining message (type = 10):
Received: [type=10] High priority message
```

## 2.9. Quản lý Queue với ipcs/ipcrm

```bash
# Xem tất cả IPC objects (message queues, shared memory, semaphores)
ipcs

# Xem riêng message queues
ipcs -q

# Output mẫu:
# ------ Message Queues --------
# key        msqid owner    perms used-bytes messages
# 0x4105a2e3 0     user     666   0         0

# Xóa queue theo ID
ipcrm -q <msgid>

# Xóa queue theo key
ipcrm -Q <key>
```

---

# 3. POSIX Message Queues

POSIX Message Queue là một phần của Real-time Extensions. Nó dùng tên (vd `/my_queue`) thay vì key integer.

## 3.1. Implementation Workflow

```
┌──────────┐   ┌───────────┐   ┌───────────┐   ┌──────────┐   ┌────────────┐
│ mq_open()│──►│ mq_send() │──►│mq_receive()│──►│mq_close()│──►│mq_unlink()│
│ Tạo queue│   │ Gửi tin   │   │ Nhận tin   │   │ Đóng FD  │   │ Xóa queue │
└──────────┘   └───────────┘   └───────────┘   └──────────┘   └────────────┘
```

**Linker flag:** `-lrt` (Real-time library).

```bash
gcc posix_mq.c -o posix_mq -lrt
```

## 3.2. Open/Create Queue (mq_open)

```c
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

mqd_t mq_open(const char *name, int oflag, ...);
```

- `name`: Tên queue, **bắt đầu bằng dấu /** (vd: `/my_queue`).
- `oflag`: `O_RDONLY`, `O_WRONLY`, `O_RDWR`, kết hợp với `O_CREAT`, `O_EXCL`, `O_NONBLOCK`.
- `mode`: Permission (vd `0644`) - chỉ dùng khi có `O_CREAT`.
- `attr`: Pointer tới `struct mq_attr` - chỉ dùng khi có `O_CREAT`, NULL = default.
- Returns: `mqd_t` (message queue descriptor) hoặc `(mqd_t)-1` (lỗi).

```c
mqd_t mq = mq_open("/my_queue", O_CREAT | O_RDWR, 0644, NULL);
if (mq == (mqd_t)-1) {
    perror("mq_open failed");
    exit(EXIT_FAILURE);
}
```

## 3.3. Set Attributes (mq_attr)

```c
struct mq_attr {
    long mq_flags;    // Flags: 0 or O_NONBLOCK
    long mq_maxmsg;   // Max number of messages on queue
    long mq_msgsize;  // Max message size (bytes)
    long mq_curmsgs;  // Number of messages currently in queue (read-only)
};
```

```c
struct mq_attr attr;
attr.mq_flags = 0;        // Blocking mode
attr.mq_maxmsg = 10;       // Tối đa 10 messages
attr.mq_msgsize = 1024;    // Mỗi message tối đa 1024 bytes

mqd_t mq = mq_open("/my_queue", O_CREAT | O_RDWR, 0644, &attr);

// Lấy attributes (kể cả default attributes)
struct mq_attr cur_attr;
mq_getattr(mq, &cur_attr);
printf("Max messages: %ld\n", cur_attr.mq_maxmsg);
printf("Max size: %ld\n", cur_attr.mq_msgsize);
printf("Current messages: %ld\n", cur_attr.mq_curmsgs);
```

## 3.4. Send Message (mq_send)

```c
int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len,
            unsigned int msg_prio);
```

- `msg_prio`: Độ ưu tiên (0 = thấp nhất, đến sysconf limit - thường 31).
- Message có priority cao hơn được nhận trước (cùng priority thì FIFO).
- Nếu queue đầy: block (hoặc return -1 với `O_NONBLOCK`).

## 3.5. Receive Message (mq_receive)

```c
ssize_t mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len,
                   unsigned int *msg_prio);
```

- `msg_len` phải **lớn hơn hoặc bằng** `attr.mq_msgsize`.
- `msg_prio` có thể là NULL nếu không cần biết priority.
- Returns: số bytes nhận được, hoặc -1 (lỗi).

## 3.6. Close & Unlink (mq_close, mq_unlink)

```c
int mq_close(mqd_t mqdes);     // Đóng descriptor (queue vẫn tồn tại)
int mq_unlink(const char *name); // Xóa queue khỏi hệ thống
```

- `mq_close()` chỉ đóng descriptor trong process hiện tại. Queue vẫn tồn tại trong kernel.
- `mq_unlink()` đánh dấu queue sẽ bị xóa khi tất cả process đóng nó.
- Tương tự `close()` và `unlink()` cho file.

## 3.7. Code Example: POSIX Sender

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

#define QUEUE_NAME  "/my_posix_queue"
#define MAX_SIZE    1024

int main() {
    // 1. Open queue (tạo nếu chưa có)
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;

    mqd_t mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    // 2. Send messages with different priorities
    char *messages[] = {"Low priority", "Normal priority", "High priority"};
    unsigned int priorities[] = {1, 5, 10};

    for (int i = 0; i < 3; i++) {
        if (mq_send(mq, messages[i], strlen(messages[i]) + 1,
                    priorities[i]) == -1) {
            perror("mq_send");
            continue;
        }
        printf("Sent: [prio=%u] %s\n", priorities[i], messages[i]);
    }

    // 3. Close (queue vẫn tồn tại)
    mq_close(mq);
    return 0;
}
```

## 3.8. Code Example: POSIX Receiver

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

#define QUEUE_NAME  "/my_posix_queue"
#define MAX_SIZE    1024

int main() {
    // 1. Open queue
    mqd_t mq = mq_open(QUEUE_NAME, O_RDONLY);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_SIZE];
    unsigned int prio;

    // 2. Receive messages
    printf("Receiving messages (highest priority first):\n");
    for (int i = 0; i < 3; i++) {
        ssize_t n = mq_receive(mq, buffer, MAX_SIZE, &prio);
        if (n == -1) {
            perror("mq_receive");
            continue;
        }
        buffer[n] = '\0';
        printf("Received: [prio=%u] %s\n", prio, buffer);
    }

    // 3. Cleanup
    mq_close(mq);
    mq_unlink(QUEUE_NAME);
    printf("Queue removed.\n");

    return 0;
}
```

**Compile & Run:**
```bash
gcc posix_sender.c -o posix_sender -lrt
gcc posix_receiver.c -o posix_receiver -lrt

# Terminal 1
./posix_receiver

# Terminal 2
./posix_sender
```

**Output:**
```
Sender:
Sent: [prio=1] Low priority
Sent: [prio=5] Normal priority
Sent: [prio=10] High priority

Receiver:
Receiving messages (highest priority first):
Received: [prio=10] High priority      # Priority cao nhất được nhận trước
Received: [prio=5] Normal priority
Received: [prio=1] Low priority
```

### Xem POSIX Message Queue trong hệ thống:

```bash
# Mount virtual filesystem (nếu chưa mount)
sudo mount -t mqueue none /dev/mqueue

# Xem danh sách queue
ls -l /dev/mqueue/
# -rw-r--r-- 1 user user 80 ... /dev/mqueue/my_posix_queue

# Xem chi tiết queue
cat /dev/mqueue/my_posix_queue
# QSIZE:0          NOTIFY:0     SIGNO:0     NOTIFY_PID:0
```

---

# 4. Message Notification (mq_notify)

## 4.1. Overview

POSIX Message Queue cung cấp cơ chế **bất đồng bộ** để thông báo khi có message mới, thay vì phải block trong `mq_receive()` hoặc poll.

```c
#include <mqueue.h>

int mq_notify(mqd_t mqdes, const struct sigevent *notification);
```

- `notification` = NULL: Hủy đăng ký notification
- `notification->sigev_notify`:
  - `SIGEV_NONE`: Không thông báo (dùng để kiểm tra)
  - `SIGEV_SIGNAL`: Gửi signal (vd: SIGUSR1)
  - `SIGEV_THREAD`: Gọi hàm callback trong thread mới (glibc extension)

> **Lưu ý quan trọng**: Chỉ có **một** process có thể đăng ký notification cho một queue tại một thời điểm. Notification chỉ được gửi **một lần** - phải đăng ký lại sau mỗi lần nhận.

## 4.2. Code Example: mq_notify with Signal

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>

#define QUEUE_NAME  "/notify_queue"
#define MAX_SIZE    1024

mqd_t mq;

void handle_notify(int sig) {
    // Nhận message sau khi được notification
    char buffer[MAX_SIZE];
    unsigned int prio;

    ssize_t n = mq_receive(mq, buffer, MAX_SIZE, &prio);
    if (n > 0) {
        buffer[n] = '\0';
        printf("[Notification] Received: [prio=%u] %s\n", prio, buffer);
    }

    // Đăng ký lại notification cho lần sau
    struct sigevent sev;
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGUSR1;
    mq_notify(mq, &sev);
}

int main() {
    // Setup signal handler
    struct sigaction sa;
    sa.sa_handler = handle_notify;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);

    // Create queue
    struct mq_attr attr = {0, 10, MAX_SIZE, 0};
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);

    // Register notification
    struct sigevent sev;
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGUSR1;
    mq_notify(mq, &sev);

    printf("Waiting for messages (PID: %d)...\n", getpid());
    printf("Run in another terminal: ./posix_sender\n");

    // Main loop - làm việc khác, không bị block
    while (1) {
        pause();  // Chờ signal
    }

    mq_close(mq);
    mq_unlink(QUEUE_NAME);
    return 0;
}
```

---

# 5. Review IPC Mechanisms

## 5.1. So sánh chi tiết IPC

| Mechanism         | Đặc điểm chính                   | Ưu điểm                             | Nhược điểm                                    |
| ----------------- | -------------------------------- | ----------------------------------- | --------------------------------------------- |
| **Pipe / FIFO**   | Byte stream, đơn giản            | Dễ dùng, POSIX                     | Không cấu trúc, đơn chiều (pipe)             |
| **Socket**        | Byte stream/Datagram, qua mạng   | Giao tiếp qua mạng hoặc local       | Phức tạp hơn Pipe                            |
| **Message Queue** | Message-based, type/priority     | Dữ liệu có cấu trúc, async          | Giới hạn kích thước message/queue             |
| **Shared Memory** | Chia sẻ vùng nhớ vật lý          | Tốc độ nhanh nhất                   | Cần cơ chế đồng bộ                           |
| **Signal**        | Ngắt mềm, báo sự kiện            | Đơn giản, xử lý bất đồng bộ         | Không truyền dữ liệu lớn                      |

### So sánh Message Queue vs Pipe:

| Tiêu chí              | Pipe/FIFO                  | Message Queue                      |
| --------------------- | -------------------------- | ---------------------------------- |
| **Dữ liệu**           | Byte stream (không ranh giới)| Message (có ranh giới gói tin)    |
| **Loại/tin nhắn**     | Không                       | Có (type/priority)                 |
| **Đồng bộ**           | Block tự nhiên              | Block + async notification          |
| **Kích thước**        | pipe buffer (thường 64KB)   | Cấu hình được (mq_maxmsg * mq_msgsize) |
| **Nhiều reader**      | Một reader (FIFO có thể nhiều) | Nhiều reader được                |

## 5.2. Khi nào dùng Message Queues?

**Nên dùng Message Queue khi:**
1. Dữ liệu có cấu trúc (cần gửi/nhận theo message, không phải byte stream)
2. Cần priority hoặc type-based receiving
3. Cần giao tiếp async (sender không cần đợi receiver)
4. Nhiều producer và consumer cùng lúc

**Không nên dùng Message Queue khi:**
1. Cần tốc độ tối đa → **Shared Memory**
2. Dữ liệu đơn giản, một chiều → **Pipe**
3. Cần giao tiếp qua mạng → **Socket**
4. Chỉ cần báo sự kiện đơn giản → **Signal**

---

## 📚 Tài liệu tham khảo

- *The Linux Programming Interface* - Michael Kerrisk (Chapters 52-56)
- *UNIX Network Programming, Volume 2* - W. Richard Stevens
- Linux man pages: `man msgget`, `man msgsnd`, `man msgrcv`, `man mq_overview`
- [POSIX Message Queue Overview](https://man7.org/linux/man-pages/man7/mq_overview.7.html)
- [System V IPC Overview](https://man7.org/linux/man-pages/man7/svipc.7.html)
