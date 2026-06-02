# Embedded Linux - IPC Semaphore

> Tài liệu học về Linux Semaphore: Cơ chế đồng bộ hóa process/thread, Race Condition, POSIX Semaphore (Named/Unnamed), System V Semaphore, Deadlock, và so sánh Mutex vs Semaphore.

---

## 📑 Mục Lục

### [1. Introduction](#1-introduction)
- [1.1. Race Condition & Critical Section](#11-race-condition--critical-section)
- [1.2. What is Semaphore?](#12-what-is-semaphore)
- [1.3. P (Wait) và V (Post) Operations](#13-p-wait-và-v-post-operations)
- [1.4. Binary vs Counting Semaphore](#14-binary-vs-counting-semaphore)

### [2. POSIX Unnamed Semaphore](#2-posix-unnamed-semaphore)
- [2.1. Overview](#21-overview)
- [2.2. Initialize (sem_init)](#22-initialize-sem_init)
- [2.3. Wait/Post (sem_wait, sem_post)](#23-waitpost-sem_wait-sem_post)
- [2.4. Try Wait (sem_trywait, sem_timedwait)](#24-try-wait-sem_trywait-sem_timedwait)
- [2.5. Destroy (sem_destroy)](#25-destroy-sem_destroy)
- [2.6. Code Example: Thread Synchronization](#26-code-example-thread-synchronization)
- [2.7. Code Example: Process Synchronization (with shared memory)](#27-code-example-process-synchronization-with-shared-memory)

### [3. POSIX Named Semaphore](#3-posix-named-semaphore)
- [3.1. Overview](#31-overview)
- [3.2. Open/Create (sem_open)](#32-opencreate-sem_open)
- [3.3. Close/Unlink (sem_close, sem_unlink)](#33-closeunlink-sem_close-sem_unlink)
- [3.4. Get Value (sem_getvalue)](#34-get-value-sem_getvalue)
- [3.5. Code Example: Producer-Consumer](#35-code-example-producer-consumer)

### [4. System V Semaphore](#4-system-v-semaphore)
- [4.1. Overview](#41-overview)
- [4.2. Lifecycle (semget, semop, semctl)](#42-lifecycle-semget-semop-semctl)
- [4.3. Code Example: System V Semaphore](#43-code-example-system-v-semaphore)

### [5. Mutex vs Semaphore](#5-mutex-vs-semaphore)
- [5.1. So sánh chi tiết](#51-so-sánh-chi-tiết)
- [5.2. Khi nào dùng Mutex? Khi nào dùng Semaphore?](#52-khi-nào-dùng-mutex-khi-nào-dùng-semaphore)

### [6. Deadlock & Priority Inversion](#6-deadlock--priority-inversion)
- [6.1. Deadlock là gì?](#61-deadlock-là-gì)
- [6.2. Deadlock Prevention](#62-deadlock-prevention)
- [6.3. Priority Inversion](#63-priority-inversion)

### [7. Semaphore với Shared Memory](#7-semaphore-với-shared-memory)
- [7.1. Pattern: Shared Resource + Semaphore](#71-pattern-shared-resource--semaphore)
- [7.2. Code Example: Shared Ring Buffer](#72-code-example-shared-ring-buffer)

---

# 1. Introduction

## 1.1. Race Condition & Critical Section

### Race Condition (Điều kiện tranh đua)

**Race Condition** xảy ra khi nhiều process/thread cùng truy cập và thay đổi một dữ liệu chung (shared resource) đồng thời, dẫn đến kết quả không xác định (sai lệch).

```
Thread A                    Memory                  Thread B
  │                      ┌────────┐                   │
  │ READ counter (10)    │   10   │                   │
  │◄─────────────────────│        │                   │
  │                      │        │  READ counter (10)│
  │                      │        │──────────────────►│
  │ counter = 10 + 1     │        │                   │
  │                      │        │  counter = 10 + 1 │
  │ WRITE counter = 11   │        │                   │
  │─────────────────────►│   11   │                   │
  │                      │        │  WRITE counter=11 │
  │                      │   11   │◄──────────────────│
  │                      └────────┘                   │
  Kết quả đúng: counter = 12
  Kết quả thực tế: counter = 11 (mất 1 lần tăng!)
```

### Critical Section (Đoạn găng)

**Critical Section** là đoạn code truy cập vào shared resource. Cần đảm bảo tại một thời điểm chỉ có 1 process/thread được vào đây (**Mutual Exclusion**).

```
Thread 1                      Thread 2
  │                              │
  │  [Non-critical code]          │
  │                              │
  │  ┌──Critical Section──┐      │
  │  │ ENTER (lock)       │      │
  │  │ counter++          │      │  (BLOCKED - đang chờ)
  │  │ EXIT (unlock)      │      │
  │  └────────────────────┘      │
  │                              │
  │  [Non-critical code]         │  ┌──Critical Section──┐
  │                              │  │ ENTER (lock)       │
  │                              │  │ counter++          │
  │                              │  │ EXIT (unlock)      │
  │                              │  └────────────────────┘
```

## 1.2. What is Semaphore?

**Semaphore** là một biến nguyên (integer) được duy trì bởi Kernel, dùng để giải quyết vấn đề đồng bộ hóa.

- Nó hoạt động như một cái "cổng" barie.
- Nếu giá trị > 0: Cổng mở, cho phép process đi qua (và giảm giá trị).
- Nếu giá trị = 0: Cổng đóng, process phải đợi (block).
- Các thao tác trên semaphore là **nguyên tử (atomic)** → không thể xảy ra race condition trên chính semaphore.

```
Semaphore Concept:
Semaphore S = 1 (Binary Semaphore)

  Thread A                         Thread B
     │                                │
  Wait(S)                          Wait(S)
  S = 0 ─── CỔNG ĐÓNG ───          S = 0 ─── BLOCKED ───
     │                                │
  [Critical Section]                   │ (đang ngủ)
     │                                │
  Post(S)                             │
  S = 1 ─── CỔNG MỞ ───             S = 1 ─── ĐÁNH THỨC ───
     │                             [Critical Section]
     │                                │
     │                             Post(S)
```

## 1.3. P (Wait) và V (Post) Operations

Hai thao tác cơ bản trên Semaphore (nguyên tử - atomic):

### Wait (P operation) - Giảm, chờ:
```
P(S):  // "Proberen" = Test (Dutch)
  while (S <= 0)
    block_and_wait();   // Nếu S = 0, chờ
  S = S - 1;            // Giảm S
```

Khi gọi `sem_wait()`:
1. Nếu S > 0: Giảm S đi 1 và tiếp tục.
2. Nếu S == 0: Block (ngủ) cho đến khi S > 0.

### Post (V operation) - Tăng, đánh thức:
```
V(S):  // "Verhogen" = Increment (Dutch)
  S = S + 1;            // Tăng S
  if (threads_waiting())
    wake_one_up();      // Đánh thức 1 thread đang chờ
```

Khi gọi `sem_post()`:
1. Tăng S lên 1.
2. Nếu có process nào đang ngủ đợi S, đánh thức nó dậy.

## 1.4. Binary vs Counting Semaphore

### Binary Semaphore (S = 0 hoặc 1)

Hoạt động như một **lock** (giống Mutex):
- S = 1: unlocked (cho phép 1 process vào critical section)
- S = 0: locked (có process đang trong critical section)

```
Binary Semaphore (S = 1):
┌────────────────────────────────────────────┐
│   Unlocked (S=1)    ◄──►    Locked (S=0)   │
│    ┌──────────────┐        ┌─────────────┐ │
│    │ Có thể vào   │        │ Đang có người│ │
│    │ critical sect│        │ trong crit.  │ │
│    └──────────────┘        └─────────────┘ │
└────────────────────────────────────────────┘
```

### Counting Semaphore (S >= 0)

Cho phép N process cùng truy cập tài nguyên:
- Ví dụ: S = 3 cho phép 3 threads cùng đọc database connection pool.
- Mỗi thread gọi `sem_wait()` giảm S, `sem_post()` tăng S.

```
Counting Semaphore (S = 3):
Pool of 3 resources:

Available: │■│■│■│    S = 3
Thread A takes 1:  │□│■│■│    S = 2
Thread B takes 1:  │□│□│■│    S = 1
Thread C takes 1:  │□│□│□│    S = 0
Thread D tries:    │□│□│□│    S = 0 → BLOCKED!
Thread A releases: │■│□│□│    S = 1 → D được đánh thức
```

---

# 2. POSIX Unnamed Semaphore

Dễ sử dụng, hiện đại, thread-safe. Thư viện `<semaphore.h>`.

## 2.1. Overview

**Unnamed Semaphore** (semaphore không tên):
- Chỉ nằm trong bộ nhớ (biến global hoặc shared memory).
- Dùng cho **Threads** (cùng process) hoặc **Related Processes** (sau fork với shared memory).
- Không cần `sem_open()`, dùng `sem_init()` trực tiếp.

## 2.2. Initialize (sem_init)

```c
#include <semaphore.h>

int sem_init(sem_t *sem, int pshared, unsigned int value);
```

| Parameter | Mô tả                                                       |
| --------- | ----------------------------------------------------------- |
| `sem`     | Con trỏ đến semaphore variable                               |
| `pshared` | 0 = dùng giữa các threads trong cùng process                |
|           | != 0 = dùng giữa các processes (cần đặt trong shared memory) |
| `value`   | Giá trị khởi tạo của semaphore (>= 0)                       |

```c
// Thread synchronization trong cùng process
sem_t thread_sem;
sem_init(&thread_sem, 0, 1);   // Binary semaphore (như mutex)

sem_t counting_sem;
sem_init(&counting_sem, 0, 3); // Counting semaphore (cho phép 3 threads)

// Process synchronization (cần shared memory)
sem_t *proc_sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);
sem_init(proc_sem, 1, 1);      // pshared = 1 (shared between processes)
```

## 2.3. Wait/Post (sem_wait, sem_post)

```c
#include <semaphore.h>

int sem_wait(sem_t *sem);    // P operation (wait/decrement)
int sem_post(sem_t *sem);    // V operation (post/increment)
```

- `sem_wait()`: Nếu S > 0, giảm S và return. Nếu S = 0, block.
- `sem_post()`: Tăng S, đánh thức thread đang block (nếu có).
- Returns: 0 (thành công), -1 (lỗi).

## 2.4. Try Wait (sem_trywait, sem_timedwait)

**Non-blocking wait:**
```c
int sem_trywait(sem_t *sem);
// Nếu S = 0, return ngay với errno = EAGAIN (không block)
```

**Wait với timeout:**
```c
#include <time.h>

int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
// Block tối đa đến abs_timeout, hết hạn return -1, errno = ETIMEDOUT
```

```c
sem_t sem;
sem_init(&sem, 0, 0);

struct timespec ts;
clock_gettime(CLOCK_REALTIME, &ts);
ts.tv_sec += 5;  // Timeout sau 5 giây

if (sem_timedwait(&sem, &ts) == -1) {
    if (errno == ETIMEDOUT) {
        printf("Timeout! No one posted the semaphore in 5 seconds.\n");
    }
}
```

## 2.5. Destroy (sem_destroy)

```c
int sem_destroy(sem_t *sem);
```

Giải phóng tài nguyên của semaphore. Chỉ gọi khi không còn thread nào dùng nó.

## 2.6. Code Example: Thread Synchronization

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_THREADS 5

sem_t semaphore;
int shared_resource = 0;

void *worker(void *arg) {
    int id = *(int *)arg;

    printf("Thread %d: waiting to enter...\n", id);
    sem_wait(&semaphore);  // Wait (P)

    // Critical section
    printf("Thread %d: ENTERED, resource = %d\n", id, shared_resource);
    shared_resource++;
    sleep(1);  // Simulate work
    printf("Thread %d: EXITING, resource = %d\n", id, shared_resource);

    sem_post(&semaphore);  // Post (V)

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    // Init: counting semaphore, cho phép 2 threads cùng vào
    sem_init(&semaphore, 0, 2);

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&semaphore);
    printf("Final resource value: %d\n", shared_resource);
    return 0;
}
```

**Compile & Run:**
```bash
gcc sem_thread.c -o sem_thread -pthread
./sem_thread
```

**Output:**
```
Thread 1: waiting to enter...
Thread 1: ENTERED, resource = 0
Thread 2: waiting to enter...
Thread 2: ENTERED, resource = 1    # 2 threads cùng vào (counting = 2)
Thread 3: waiting to enter...       # Blocked (đã có 2 threads)
Thread 1: EXITING, resource = 2
Thread 3: ENTERED, resource = 2
Thread 2: EXITING, resource = 3
Thread 4: waiting to enter...
Thread 4: ENTERED, resource = 3
...
```

## 2.7. Code Example: Process Synchronization (with shared memory)

Khi dùng semaphore giữa các process, cần đặt semaphore trong **shared memory**:

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/wait.h>

typedef struct {
    sem_t semaphore;
    int counter;
} SharedData;

int main() {
    // Create shared memory
    SharedData *sd = mmap(NULL, sizeof(SharedData),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // Initialize semaphore (pshared = 1 for processes)
    sem_init(&sd->semaphore, 1, 1);  // Binary semaphore
    sd->counter = 0;

    // Fork 3 children
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child
            for (int j = 0; j < 100000; j++) {
                sem_wait(&sd->semaphore);
                sd->counter++;  // Protected critical section
                sem_post(&sd->semaphore);
            }
            exit(0);
        }
    }

    // Wait for all children
    for (int i = 0; i < 3; i++) wait(NULL);

    printf("Final counter: %d (expected 300000)\n", sd->counter);

    sem_destroy(&sd->semaphore);
    munmap(sd, sizeof(SharedData));
    return 0;
}
```

---

# 3. POSIX Named Semaphore

## 3.1. Overview

**Named Semaphore** có tên file trên hệ thống (vd `/mysem`):
- Dùng cho các **process không liên quan** (không fork, không shared memory).
- Kernel persistence: tồn tại đến khi `sem_unlink()`.
- Được lưu trong `/dev/shm/` hoặc `/sem/`.

## 3.2. Open/Create (sem_open)

```c
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

sem_t *sem_open(const char *name, int oflag, ...);
```

| Mode                 | Mô tả                                              |
| -------------------- | -------------------------------------------------- |
| `O_CREAT`            | Tạo mới nếu chưa tồn tại (cần mode + value)        |
| `O_CREAT | O_EXCL`  | Chỉ tạo mới, fail nếu đã tồn tại                   |
| (none)               | Mở semaphore đã tồn tại                            |

```c
// Tạo named semaphore với value = 1
sem_t *sem = sem_open("/my_sem", O_CREAT, 0644, 1);

// Mở semaphore đã tồn tại
sem_t *sem = sem_open("/my_sem", 0);
```

## 3.3. Close/Unlink (sem_close, sem_unlink)

```c
int sem_close(sem_t *sem);    // Đóng trong process hiện tại
int sem_unlink(const char *name);  // Xóa semaphore khỏi hệ thống
```

- `sem_close()`: Đóng semaphore descriptor. Semaphore vẫn tồn tại.
- `sem_unlink()`: Đánh dấu xóa. Semaphore thực sự bị hủy khi process cuối cùng close.

## 3.4. Get Value (sem_getvalue)

```c
int sem_getvalue(sem_t *sem, int *sval);
```

Lấy giá trị hiện tại của semaphore (hữu ích cho debugging).

```c
int val;
sem_getvalue(sem, &val);
printf("Semaphore value: %d\n", val);
```

## 3.5. Code Example: Producer-Consumer

### Producer:

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

#define SEM_NAME "/prod_cons_sem"

int main() {
    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    printf("Producer: starting...\n");
    for (int i = 1; i <= 5; i++) {
        sem_wait(sem);
        // Critical section: produce an item
        printf("Producer: producing item %d\n", i);
        sleep(1);  // Simulate work
        sem_post(sem);
        sleep(1);  // Wait before next production
    }

    sem_close(sem);
    // Don't unlink - let consumer do it
    return 0;
}
```

### Consumer:

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

#define SEM_NAME "/prod_cons_sem"

int main() {
    // Open existing semaphore
    sem_t *sem = sem_open(SEM_NAME, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    printf("Consumer: starting...\n");
    for (int i = 1; i <= 5; i++) {
        sem_wait(sem);
        // Critical section: consume an item
        printf("Consumer: consuming item %d\n", i);
        sleep(1);
        sem_post(sem);
        sleep(1);
    }

    sem_close(sem);
    sem_unlink(SEM_NAME);  // Cleanup
    return 0;
}
```

**Chạy 2 terminal:**
```bash
# Terminal 1
gcc producer.c -o producer -pthread
./producer

# Terminal 2
gcc consumer.c -o consumer -pthread
./consumer
```

---

# 4. System V Semaphore

## 4.1. Overview

Cơ chế cũ, phức tạp hơn nhưng mạnh mẽ:
- Quản lý theo **set** (mảng semaphore).
- Có thể thao tác nhiều semaphore trong 1 lần gọi `semop()`.
- Dùng **Key** (`ftok`) giống System V Message Queue và Shared Memory.

## 4.2. Lifecycle (semget, semop, semctl)

```c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

// 1. Tạo/Lấy semaphore set
int semid = semget(key, nsems, semflg);

// 2. Điều khiển (set value, get info, remove)
int semctl(int semid, int semnum, int cmd, ...);

// 3. Thao tác (wait/post)
int semop(int semid, struct sembuf *sops, size_t nsops);
```

### struct sembuf:

```c
struct sembuf {
    unsigned short sem_num;  // Index trong set (0, 1, 2, ...)
    short          sem_op;   // Operation: -1 (wait/P), +1 (post/V)
    short          sem_flg;  // Flags: 0, IPC_NOWAIT, SEM_UNDO
};
```

## 4.3. Code Example: System V Semaphore

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define KEY_PATH "/tmp"
#define PROJ_ID   'S'

// Hàm tiện ích: thao tác semaphore
int sem_op(int semid, int sem_num, int op) {
    struct sembuf sb;
    sb.sem_num = sem_num;
    sb.sem_op = op;     // -1 = wait, +1 = post
    sb.sem_flg = 0;
    return semop(semid, &sb, 1);
}

// Hàm tiện ích: set giá trị semaphore
int sem_setval(int semid, int sem_num, int val) {
    return semctl(semid, sem_num, SETVAL, val);
}

int main() {
    // 1. Tạo key
    key_t key = ftok(KEY_PATH, PROJ_ID);
    if (key == -1) { perror("ftok"); exit(1); }

    // 2. Tạo semaphore set với 1 semaphore
    int semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid == -1) { perror("semget"); exit(1); }

    // 3. Set initial value = 1 (binary semaphore)
    if (sem_setval(semid, 0, 1) == -1) {
        perror("semctl SETVAL");
        exit(1);
    }

    printf("System V Semaphore created: ID = %d\n", semid);

    // 4. Fork child
    pid_t pid = fork();
    if (pid == 0) {
        // Child
        printf("Child: waiting for semaphore...\n");
        sem_op(semid, 0, -1);  // Wait (P)

        printf("Child: IN critical section\n");
        sleep(2);
        printf("Child: leaving critical section\n");

        sem_op(semid, 0, 1);   // Post (V)
        exit(0);
    } else {
        // Parent
        printf("Parent: waiting for semaphore...\n");
        sem_op(semid, 0, -1);  // Wait (P)

        printf("Parent: IN critical section\n");
        sleep(2);
        printf("Parent: leaving critical section\n");

        sem_op(semid, 0, 1);   // Post (V)

        wait(NULL);

        // Cleanup
        semctl(semid, 0, IPC_RMID);
        printf("Semaphore removed.\n");
    }

    return 0;
}
```

**Compile & Run:**
```bash
gcc sysv_sem.c -o sysv_sem
./sysv_sem
```

**Quản lý System V Semaphore:**
```bash
# Xem tất cả IPC Semaphores
ipcs -s

# Xóa semaphore set
ipcrm -s <semid>
```

---

# 5. Mutex vs Semaphore

## 5.1. So sánh chi tiết

| Tiêu chí                | Mutex (pthread_mutex_t)         | Binary Semaphore (S=1)          |
| ----------------------- | ------------------------------- | ------------------------------- |
| **Mục đích**            | Mutual exclusion (khóa)         | Đồng bộ hóa (signaling)         |
| **Owner**               | Chỉ thread đã lock mới unlock   | Bất kỳ thread nào có thể post   |
| **Đánh thức**           | Chỉ unlocker                    | sem_post từ thread bất kỳ       |
| **Priority inversion**  | Có (cần protocol)               | Có                              |
| **Dùng cho process**    | Không (cần pthread_mutexattr)   | Có (pshared=1 hoặc named)       |
| **Recursive**           | Có thể (PTHREAD_MUTEX_RECURSIVE)| Không                           |
| **Performance**         | Nhanh hơn (user-space)          | Chậm hơn (kernel involvement)   |

### Lưu ý quan trọng:

- **Mutex** chỉ nên dùng cho **mutual exclusion** - khóa critical section.
- **Semaphore** dùng cho **signaling** - thông báo sự kiện (producer signal consumer).
- Binary semaphore KHÔNG phải là mutex, dù cùng có S=1.

```
Sai lầm thường gặp:
❌ sem_post từ thread khác với thread đã sem_wait → OK với semaphore
❌ pthread_mutex_unlock từ thread khác với thread đã lock → UNDEFINED BEHAVIOR!
```

## 5.2. Khi nào dùng Mutex? Khi nào dùng Semaphore?

| Tình huống                                    | Nên dùng              |
| --------------------------------------------- | --------------------- |
| Bảo vệ critical section đơn giản              | Mutex                 |
| Cần signaling (producer-consumer)             | Semaphore             |
| Đồng bộ process không liên quan               | Named Semaphore       |
| Nhiều thread đọc, một thread ghi              | Read-Write Lock       |
| Cần counting resource (pool connections)      | Counting Semaphore    |
| Thread-safety đơn giản trong C++              | std::mutex            |

### Producer-Consumer với Semaphore (signaling):

```c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t empty_slots;  // How many slots are empty
sem_t filled_slots; // How many slots are filled
int buffer[5];       // Shared buffer

void *producer(void *arg) {
    for (int i = 1; i <= 10; i++) {
        sem_wait(&empty_slots);   // Wait for empty slot
        // Produce
        buffer[i % 5] = i;
        printf("Produced: %d\n", i);
        sem_post(&filled_slots);  // Signal: new item available
        usleep(300000);
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < 10; i++) {
        sem_wait(&filled_slots);  // Wait for item
        // Consume
        printf("Consumed: %d\n", buffer[i % 5]);
        sem_post(&empty_slots);   // Signal: slot is empty
        usleep(500000);
    }
    return NULL;
}

int main() {
    sem_init(&empty_slots, 0, 5);  // 5 empty slots
    sem_init(&filled_slots, 0, 0); // 0 filled slots

    pthread_t p, c;
    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);
    pthread_join(p, NULL);
    pthread_join(c, NULL);

    sem_destroy(&empty_slots);
    sem_destroy(&filled_slots);
    return 0;
}
```

---

# 6. Deadlock & Priority Inversion

## 6.1. Deadlock là gì?

**Deadlock** là tình huống 2 hoặc nhiều process/thread đang chờ nhau giải phóng tài nguyên, dẫn đến không process nào tiếp tục được.

```
Thread A                      Thread B
  │                              │
  │ lock(mutex_1)  ─── OK        │ lock(mutex_2) ─── OK
  │                              │
  │ lock(mutex_2)  ─── BLOCKED   │ lock(mutex_1)  ─── BLOCKED
  │ (A đang chờ B unlock mutex_2)│ (B đang chờ A unlock mutex_1)
  │                              │
  └──────────── DEADLOCK ────────┘
```

**4 điều kiện cần cho Deadlock:**
1. **Mutual Exclusion**: Tài nguyên không thể chia sẻ
2. **Hold and Wait**: Giữ tài nguyên đang có và chờ thêm
3. **No Preemption**: Không thể lấy tài nguyên từ process khác
4. **Circular Wait**: Vòng tròn chờ đợi

## 6.2. Deadlock Prevention

### 1. Cố định thứ tự khóa (Lock Ordering):

```c
// ✅ CORRECT: Cả 2 threads cùng order: mutex_1 → mutex_2
void thread_a() {
    pthread_mutex_lock(&mutex_1);
    pthread_mutex_lock(&mutex_2);
    // critical section
    pthread_mutex_unlock(&mutex_2);
    pthread_mutex_unlock(&mutex_1);
}

void thread_b() {
    pthread_mutex_lock(&mutex_1);  // Cùng thứ tự với thread_a
    pthread_mutex_lock(&mutex_2);
    // critical section
    pthread_mutex_unlock(&mutex_2);
    pthread_mutex_unlock(&mutex_1);
}
```

### 2. Dùng trylock và backoff:

```c
void thread_safe_function() {
    pthread_mutex_lock(&mutex_1);
    
    // Try to get mutex_2, release mutex_1 and retry if failed
    while (pthread_mutex_trylock(&mutex_2) != 0) {
        pthread_mutex_unlock(&mutex_1);
        usleep(1000);  // Backoff
        pthread_mutex_lock(&mutex_1);
    }
    
    // Both locks acquired
    // Critical section...
    
    pthread_mutex_unlock(&mutex_2);
    pthread_mutex_unlock(&mutex_1);
}
```

### 3. Giới hạn thời gian chờ (timed wait):

```c
struct timespec ts;
clock_gettime(CLOCK_REALTIME, &ts);
ts.tv_sec += 1;  // 1 second timeout

if (sem_timedwait(&sem, &ts) == -1) {
    if (errno == ETIMEDOUT) {
        printf("Timeout! Handle gracefully instead of deadlock.\n");
        // Rollback or retry
    }
}
```

## 6.3. Priority Inversion

**Priority Inversion** xảy ra khi một thread ưu tiên thấp đang giữ lock, thread ưu tiên cao cần lock đó phải chờ, trong khi thread ưu tiên trung bình (không cần lock) lại được chạy trước.

### Giải pháp: Priority Inheritance Protocol:

```c
// Linux mutex hỗ trợ priority inheritance
pthread_mutexattr_t attr;
pthread_mutexattr_init(&attr);
pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);

pthread_mutex_t mutex;
pthread_mutex_init(&mutex, &attr);
```

---

# 7. Semaphore với Shared Memory

## 7.1. Pattern: Shared Resource + Semaphore

Đây là pattern IPC mạnh nhất: **Shared Memory (dữ liệu lớn, zero-copy)** + **Semaphore (đồng bộ)**.

```
┌─────────────────────────────────────────────────────────────┐
│                  Shared Memory Segment                       │
│  ┌─────────────────────────────────────────────────────┐    │
│  │  Data Area (lớn, nhiều MB)                          │    │
│  └─────────────────────────────────────────────────────┘    │
│  ┌────────┐                                                │
│  │ sem_t  │  ← Semaphore bảo vệ truy cập                   │
│  └────────┘                                                │
└─────────────────────────────────────────────────────────────┘
```

## 7.2. Code Example: Shared Ring Buffer

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/wait.h>

#define BUFFER_SIZE 256
#define NUM_ITEMS 1000

typedef struct {
    int buffer[BUFFER_SIZE];
    int in;          // Producer index
    int out;         // Consumer index
    sem_t mutex;     // Mutual exclusion
    sem_t empty;     // Count of empty slots
    sem_t full;      // Count of filled slots
} RingBuffer;

int main() {
    // Create shared ring buffer
    RingBuffer *rb = mmap(NULL, sizeof(RingBuffer),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // Initialize
    rb->in = 0;
    rb->out = 0;
    sem_init(&rb->mutex, 1, 1);          // Binary semaphore
    sem_init(&rb->empty, 1, BUFFER_SIZE); // BUFFER_SIZE empty slots
    sem_init(&rb->full, 1, 0);            // 0 filled slots

    // Fork producer
    pid_t prod = fork();
    if (prod == 0) {
        for (int i = 0; i < NUM_ITEMS; i++) {
            sem_wait(&rb->empty);     // Wait for empty slot
            sem_wait(&rb->mutex);     // Lock

            rb->buffer[rb->in] = i;
            rb->in = (rb->in + 1) % BUFFER_SIZE;

            sem_post(&rb->mutex);     // Unlock
            sem_post(&rb->full);      // Signal: item available
        }
        printf("Producer done\n");
        exit(0);
    }

    // Fork consumer
    pid_t cons = fork();
    if (cons == 0) {
        int sum = 0;
        for (int i = 0; i < NUM_ITEMS; i++) {
            sem_wait(&rb->full);      // Wait for item
            sem_wait(&rb->mutex);     // Lock

            int val = rb->buffer[rb->out];
            rb->out = (rb->out + 1) % BUFFER_SIZE;
            sum += val;

            sem_post(&rb->mutex);     // Unlock
            sem_post(&rb->empty);     // Signal: slot empty
        }
        printf("Consumer done. Sum = %d (expected %d)\n",
               sum, NUM_ITEMS * (NUM_ITEMS - 1) / 2);
        exit(0);
    }

    wait(NULL);
    wait(NULL);
    printf("Parent: all done.\n");

    // Cleanup
    sem_destroy(&rb->mutex);
    sem_destroy(&rb->empty);
    sem_destroy(&rb->full);
    munmap(rb, sizeof(RingBuffer));

    return 0;
}
```

---

## 📚 Tài liệu tham khảo

- *The Linux Programming Interface* - Michael Kerrisk (Chapters 52-54)
- *UNIX Network Programming, Volume 2* - W. Richard Stevens
- *Operating Systems: Three Easy Pieces* - Remzi Arpaci-Dusseau
- Linux man pages: `man 7 sem_overview`, `man sem_init`, `man sem_open`, `man semop`
- [POSIX Semaphore Overview](https://man7.org/linux/man-pages/man7/sem_overview.7.html)
