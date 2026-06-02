# Embedded Linux - Linux Thread

> Tài liệu học về Linux Thread: Thread basics, Thread operations với POSIX Threads (pthread), Thread Management, và Thread Synchronization với Mutex và Condition Variable.

---

## 📑 Mục Lục

### [1. Introduction](#1-introduction)
- [1.1. What is Thread](#11-what-is-thread)
- [1.2. Thread Executing](#12-thread-executing)
- [1.3. Context Switching](#13-context-switching)

### [2. Compare between Process and Thread](#2-compare-between-process-and-thread)
- [2.1. Context Switching Time](#21-context-switching-time)
- [2.2. Shared Memory](#22-shared-memory)
- [2.3. Crashed](#23-crashed)

### [3. Operations on Thread](#3-operations-on-thread)
- [3.1. Thread ID](#31-thread-id)
- [3.2. Create Thread](#32-create-thread)
- [3.3. Terminate Thread](#33-terminate-thread)

### [4. Thread Management](#4-thread-management)
- [4.1. Thread Joining](#41-thread-joining)
- [4.2. pthread_join()](#42-pthread_join)
- [4.3. Thread Detaching](#43-thread-detaching)
- [4.4. pthread_detach()](#44-pthread_detach)

### [5. Thread Synchronization](#5-thread-synchronization)
- [5.1. Race Condition](#51-race-condition)
- [5.2. Critical Section](#52-critical-section)

### [6. Thread Sync - Mutex Lock](#6-thread-sync---mutex-lock)
- [6.1. Mutex Lock](#61-mutex-lock)
- [6.2. Allocate Mutex](#62-allocate-mutex)
- [6.3. Locking & Unlocking Mutex](#63-locking--unlocking-mutex)

### [7. Thread Sync - Conditional Variable](#7-thread-sync---conditional-variable)
- [7.1. Conditional Variable](#71-conditional-variable)
- [7.2. Allocate Conditional Variable](#72-allocate-conditional-variable)
- [7.3. Signaling & Waiting](#73-signaling--waiting)

### [8. Exercises](#8-exercises)
- [8.1. Exercise List](#81-exercise-list)

---

# 1. Introduction

## 1.1. What is Thread

### Thread là gì?

**Thread** (Luồng) là đơn vị thực thi nhỏ nhất trong một process. Một process có thể chứa một hoặc nhiều threads, và tất cả các threads trong cùng một process sẽ chia sẻ không gian địa chỉ (address space) và tài nguyên của process đó.

```
Thread Characteristics:
├── Lightweight execution unit
├── Shares process's memory space
├── Has its own:
│   ├── Thread ID
│   ├── Stack
│   ├── Registers (PC, SP)
│   └── Signal mask
├── Shares with other threads in same process:
│   ├── Code segment (text)
│   ├── Data segment
│   ├── Heap
│   ├── Open file descriptors
│   └── Environment variables
└── Cheaper to create than process
```

### Tại sao cần Thread?

1. **Tối ưu tài nguyên**: Thread nhẹ hơn process, tạo và hủy nhanh hơn
2. **Chia sẻ dữ liệu dễ dàng**: Các threads trong cùng process có thể truy cập trực tiếp dữ liệu chung
3. **Responsive applications**: Một thread có thể xử lý UI trong khi thread khác xử lý background tasks
4. **Tận dụng multi-core CPUs**: Các threads có thể chạy song song trên các CPU cores khác nhau

### POSIX Threads (pthread)

Trên Linux, **POSIX Threads (pthreads)** là thư viện chuẩn để làm việc với threads. Để sử dụng:

```c
#include <pthread.h>

// Biên dịch với flag -pthread
// gcc program.c -o program -pthread
```

## 1.2. Thread Executing

### Cách Thread thực thi

Khi một process được tạo, nó mặc định có một thread chính gọi là **main thread**. Main thread có thể tạo thêm các threads khác, và tất cả sẽ thực thi đồng thời (concurrent execution).

```
Process Memory Layout với Threads:
┌─────────────────────────────────────────┐ High Address
│          Shared Resources               │
│    ├── Code (Text Segment)              │
│    ├── Initialized Data                 │
│    ├── BSS                              │
│    └── Heap                             │
├─────────────────────────────────────────┤
│        Thread 3 Stack                   │ ← SP3
├─────────────────────────────────────────┤
│        Thread 2 Stack                   │ ← SP2
├─────────────────────────────────────────┤
│        Thread 1 Stack                   │ ← SP1
├─────────────────────────────────────────┤
│        Main Thread Stack                │ ← SP main
└─────────────────────────────────────────┘ Low Address
```

### Thread Execution Model

```
                    ┌──────────────┐
                    │ Main Thread  │
                    │   (start)    │
                    └──────┬───────┘
                           │
           ┌───────────────┼───────────────┐
           ▼               ▼               ▼
    ┌────────────┐  ┌────────────┐  ┌────────────┐
    │  Thread 1  │  │  Thread 2  │  │  Thread 3  │
    │   (work)   │  │   (work)   │  │   (work)   │
    └─────┬──────┘  └─────┬──────┘  └─────┬──────┘
          │               │               │
          ▼               ▼               ▼
    ┌────────────┐  ┌────────────┐  ┌────────────┐
    │  exit/join │  │  exit/join │  │  exit/join │
    └─────┬──────┘  └─────┬──────┘  └─────┬──────┘
          │               │               │
          └───────────────┼───────────────┘
                          ▼
                    ┌──────────────┐
                    │ Main Thread  │
                    │   (end)      │
                    └──────────────┘
```

## 1.3. Context Switching

### Context Switching là gì?

**Context Switching** là quá trình CPU lưu trạng thái của thread/process hiện tại và khôi phục trạng thái của thread/process khác để thực thi.

### Thread Context vs Process Context

| Loại Context        | Bao gồm                                                      |
| ------------------- | ------------------------------------------------------------ |
| **Thread Context**  | Registers, Program Counter, Stack Pointer, Thread-local data |
| **Process Context** | Thread context + Memory mappings, File descriptors, Signals  |

```
Thread Context Switch (trong cùng process):
┌──────────────────────────────────────────────────────────────┐
│                                                              │
│  Thread A running   →   Save Thread A context   →   Restore  │
│                              │                        │      │
│                              ▼                        ▼      │
│                    [CPU Registers]            [CPU Registers]│
│                    [Stack Pointer]            [Stack Pointer]│
│                    [Program Counter]          [Program Counter]
│                              │                        │      │
│                              └────────────────────────┘      │
│                                        ↓                     │
│                                Thread B running              │
│                                                              │
│  NOTE: Memory space remains the SAME (no TLB flush needed)   │
└──────────────────────────────────────────────────────────────┘
```

---

# 2. Compare between Process and Thread

## 2.1. Context Switching Time

### So sánh thời gian Context Switch

| Đặc điểm                | Process Switch            | Thread Switch                  |
| ----------------------- | ------------------------- | ------------------------------ |
| **Thời gian**           | Chậm (~1-10 microseconds) | Nhanh (~100 nanoseconds)       |
| **TLB Flush**           | Cần thiết                 | Không cần                      |
| **Memory Space Switch** | Cần switch page tables    | Không cần (cùng address space) |
| **Cache**               | Thường bị invalidate      | Vẫn còn hiệu lực               |

### Tại sao Thread Switch nhanh hơn?

```
Process Context Switch:
┌────────────────────────────────────────────────────────────────┐
│ 1. Save registers, PC, SP của process hiện tại                 │
│ 2. Save memory management info (page tables)                   │
│ 3. Flush TLB (Translation Lookaside Buffer)                    │
│ 4. Switch to kernel mode → update scheduler                    │
│ 5. Load page tables của process mới                            │
│ 6. Restore registers, PC, SP của process mới                   │
│                                                                │
│ → Chi phí rất cao do memory management operations              │
└────────────────────────────────────────────────────────────────┘

Thread Context Switch (same process):
┌─────────────────────────────────────────────────────────────────┐
│ 1. Save registers, PC, SP của thread hiện tại                   │
│ 2. Switch to kernel mode → update scheduler                     │
│ 3. Restore registers, PC, SP của thread mới                     │
│                                                                 │
│ → Chi phí thấp, không cần thay đổi memory management            │
└─────────────────────────────────────────────────────────────────┘
```

## 2.2. Shared Memory

### Process - Memory riêng biệt

Mỗi process có không gian địa chỉ riêng. Để giao tiếp, cần sử dụng IPC (Inter-Process Communication):

```
Process 1                          Process 2
┌──────────────────┐              ┌──────────────────┐
│ Virtual Memory   │              │ Virtual Memory   │
│  ├── Stack       │              │  ├── Stack       │
│  ├── Heap        │              │  ├── Heap        │
│  ├── Data        │              │  ├── Data        │
│  └── Text        │              │  └── Text        │
└────────┬─────────┘              └────────┬─────────┘
         │                                 │
         │    ┌──────────────────┐         │
         └───►│ IPC Mechanisms   │◄────────┘
              │ - Pipe           │
              │ - Message Queue  │
              │ - Shared Memory  │
              │ - Socket         │
              └──────────────────┘
```

### Thread - Memory chia sẻ

Các threads trong cùng process chia sẻ trực tiếp memory:

```
                     Same Process
┌─────────────────────────────────────────────────────┐
│                 Shared Memory                       │
│  ├── Global Variables                               │
│  ├── Static Variables                               │
│  ├── Heap (dynamic allocation)                      │
│  └── Open Files, Sockets                            │
├────────────┬───────────────┬────────────────────────┤
│ Thread 1   │   Thread 2    │       Thread 3         │
│ ┌────────┐ │ ┌────────┐    │      ┌────────┐        │
│ │ Stack  │ │ │ Stack  │    │      │ Stack  │        │
│ │ (own)  │ │ │ (own)  │    │      │ (own)  │        │
│ └────────┘ │ └────────┘    │      └────────┘        │
│ Registers  │ Registers     │      Registers         │
└────────────┴───────────────┴────────────────────────┘

→ Threads có thể truy cập trực tiếp shared data
→ Không cần IPC, nhưng cần synchronization!
```

### So sánh chi tiết

| Đặc điểm             | Process                          | Thread                           |
| -------------------- | -------------------------------- | -------------------------------- |
| **Address Space**    | Riêng biệt                       | Chia sẻ                          |
| **Global Variables** | Không chia sẻ                    | Chia sẻ                          |
| **Heap Memory**      | Riêng biệt                       | Chia sẻ                          |
| **Stack**            | Riêng biệt                       | Riêng biệt (mỗi thread có stack) |
| **File Descriptors** | Độc lập (có thể copy khi fork)   | Chia sẻ                          |
| **Giao tiếp**        | Cần IPC (pipe, socket, shmem...) | Truy cập trực tiếp shared memory |

## 2.3. Crashed

### Khi Process crash

Nếu một process crash, các process khác **không bị ảnh hưởng** do có memory space riêng biệt:

```
Process A          Process B          Process C
    │                  │                  │
    │                  │                  │
    ▼                  ▼                  ▼
┌───────┐          ┌───────┐          ┌───────┐
│ CRASH │          │  OK   │          │  OK   │
│ ✗✗✗✗  │          │ ✓✓✓✓  │          │ ✓✓✓✓  │
└───────┘          └───────┘          └───────┘
    │
    ▼
Process A terminated, B and C continue running
```

### Khi Thread crash

Nếu một thread crash (ví dụ: segmentation fault), **toàn bộ process bị terminate**:

```
             Same Process
┌──────────────────────────────────────┐
│  Thread 1     Thread 2     Thread 3  │
│     │            │            │      │
│     ▼            ▼            ▼      │
│  ┌─────┐      ┌─────┐      ┌─────┐   │
│  │ OK  │      │CRASH│      │ OK  │   │
│  │ ✓✓  │      │✗✗✗✗ │      │ ✓✓  │   │
│  └─────┘      └─────┘      └─────┘   │
│                  │                   │
│                  ▼                   │
│        ENTIRE PROCESS TERMINATED    │
│              ✗✗✗✗✗✗✗✗               │
└──────────────────────────────────────┘

→ All threads die together!
```

### Tổng kết so sánh

| Tiêu chí            | Process                       | Thread                         |
| ------------------- | ----------------------------- | ------------------------------ |
| **Context Switch**  | Chậm (memory switch)          | Nhanh (chỉ switch registers)   |
| **Memory**          | Riêng biệt, cần IPC           | Chia sẻ, truy cập trực tiếp    |
| **Crash Isolation** | Độc lập, không ảnh hưởng nhau | Một thread crash = tất cả chết |
| **Tạo mới**         | Chi phí cao (fork)            | Chi phí thấp (pthread_create)  |
| **Độ phức tạp**     | Đơn giản (không sync)         | Phức tạp (cần synchronization) |

---

# 3. Operations on Thread

## 3.1. Thread ID

### pthread_t

Mỗi thread có một định danh duy nhất kiểu `pthread_t`. Không giống như PID của process, `pthread_t` là một opaque type (có thể là integer hoặc struct tùy implementation).

### Lấy Thread ID

```c
#include <pthread.h>

pthread_t pthread_self(void);
```

| Function         | Mô tả                                |
| ---------------- | ------------------------------------ |
| `pthread_self()` | Trả về thread ID của thread hiện tại |

### So sánh Thread ID

```c
#include <pthread.h>

int pthread_equal(pthread_t t1, pthread_t t2);
```

- Trả về **non-zero** nếu `t1` và `t2` là cùng một thread
- Trả về **0** nếu khác nhau

### Ví dụ:

```c
#include <stdio.h>
#include <pthread.h>

void *thread_func(void *arg) {
    printf("Thread ID: %lu\n", (unsigned long)pthread_self());
    return NULL;
}

int main() {
    pthread_t tid;
    
    printf("Main thread ID: %lu\n", (unsigned long)pthread_self());
    
    pthread_create(&tid, NULL, thread_func, NULL);
    pthread_join(tid, NULL);
    
    // So sánh thread ID
    if (pthread_equal(pthread_self(), tid)) {
        printf("Same thread\n");
    } else {
        printf("Different threads\n");
    }
    
    return 0;
}
```

## 3.2. Create Thread

### pthread_create()

Tạo một thread mới bằng hàm `pthread_create()`:

```c
#include <pthread.h>

int pthread_create(pthread_t *thread, 
                   const pthread_attr_t *attr,
                   void *(*start_routine)(void *), 
                   void *arg);
```

| Parameter       | Mô tả                                   |
| --------------- | --------------------------------------- |
| `thread`        | Con trỏ để lưu thread ID của thread mới |
| `attr`          | Thread attributes (NULL = default)      |
| `start_routine` | Hàm mà thread sẽ thực thi               |
| `arg`           | Argument truyền vào start_routine       |

| Return value | Ý nghĩa                            |
| ------------ | ---------------------------------- |
| `0`          | Thành công                         |
| `!= 0`       | Error code (EAGAIN, EINVAL, EPERM) |

### Ví dụ cơ bản:

```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *print_message(void *arg) {
    char *message = (char *)arg;
    for (int i = 0; i < 5; i++) {
        printf("%s - iteration %d\n", message, i);
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    
    // Tạo 2 threads
    pthread_create(&thread1, NULL, print_message, "Thread 1");
    pthread_create(&thread2, NULL, print_message, "Thread 2");
    
    // Đợi cả 2 threads hoàn thành
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    printf("All threads completed\n");
    return 0;
}
```

**Biên dịch và chạy:**
```bash
gcc threads.c -o threads -pthread
./threads
```

**Output mẫu:**
```
Thread 1 - iteration 0
Thread 2 - iteration 0
Thread 1 - iteration 1
Thread 2 - iteration 1
...
All threads completed
```

### Ví dụ truyền multiple arguments:

```c
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// Struct để truyền nhiều arguments
typedef struct {
    int id;
    char *name;
    int count;
} ThreadArgs;

void *worker(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    
    printf("Thread %d (%s): counting to %d\n", 
           args->id, args->name, args->count);
    
    for (int i = 1; i <= args->count; i++) {
        printf("Thread %d: %d\n", args->id, i);
    }
    
    return NULL;
}

int main() {
    pthread_t threads[3];
    ThreadArgs args[3] = {
        {1, "Alpha", 3},
        {2, "Beta", 5},
        {3, "Gamma", 2}
    };
    
    // Tạo các threads
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, worker, &args[i]);
    }
    
    // Join tất cả threads
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
    
    return 0;
}
```

## 3.3. Terminate Thread

### Các cách kết thúc Thread

| Cách                         | Mô tả                                                  |
| ---------------------------- | ------------------------------------------------------ |
| `return` từ start_routine    | Kết thúc tự nhiên, return value = thread's exit status |
| `pthread_exit(void *retval)` | Kết thúc thread hiện tại với exit status               |
| Thread bị cancel             | Bởi thread khác gọi `pthread_cancel()`                 |
| Process exit                 | Tất cả threads bị terminate                            |

### pthread_exit()

```c
#include <pthread.h>

void pthread_exit(void *retval);
```

- `retval`: Giá trị trả về cho thread đang join (có thể lấy qua `pthread_join`)
- `pthread_exit()` **không bao giờ return**

### Ví dụ:

```c
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *compute(void *arg) {
    int *num = (int *)arg;
    int *result = malloc(sizeof(int));
    
    *result = (*num) * (*num);  // Tính bình phương
    
    printf("Thread computed: %d^2 = %d\n", *num, *result);
    
    // Cách 1: return pointer to result
    // return result;
    
    // Cách 2: pthread_exit 
    pthread_exit(result);
}

int main() {
    pthread_t tid;
    int value = 7;
    int *thread_result;
    
    pthread_create(&tid, NULL, compute, &value);
    
    // Lấy kết quả từ thread
    pthread_join(tid, (void **)&thread_result);
    
    printf("Main received result: %d\n", *thread_result);
    
    free(thread_result);
    return 0;
}
```

**Output:**
```
Thread computed: 7^2 = 49
Main received result: 49
```

### pthread_cancel()

Để hủy một thread từ thread khác:

```c
#include <pthread.h>

int pthread_cancel(pthread_t thread);
```

```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *long_task(void *arg) {
    while (1) {
        printf("Working...\n");
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t tid;
    
    pthread_create(&tid, NULL, long_task, NULL);
    
    sleep(3);  // Để thread chạy 3 giây
    
    printf("Cancelling thread...\n");
    pthread_cancel(tid);
    
    pthread_join(tid, NULL);
    printf("Thread cancelled\n");
    
    return 0;
}
```

---

# 4. Thread Management

## 4.1. Thread Joining

### Joining là gì?

**Thread Joining** là việc một thread (thường là main thread) đợi một thread khác kết thúc và thu thập kết quả của nó. Tương tự như `wait()` cho processes.

```
Main Thread              Worker Thread
    │                         │
    │  pthread_create()       │
    ├────────────────────────►│
    │                         │ (doing work)
    │                         │
    │  pthread_join()         │
    │   (blocked)             │
    │      ...                │
    │      ...                ▼
    │                    pthread_exit() or return
    │◄────────────────────────┤
    │  (gets result)          │
    ▼                         
    │ continues...
```

### Tại sao cần Join?

1. **Đợi thread hoàn thành**: Đảm bảo công việc được hoàn thành trước khi tiếp tục
2. **Lấy kết quả**: Thu thập return value từ thread
3. **Giải phóng resources**: Nếu không join hoặc detach, thread resources không được thu hồi

## 4.2. pthread_join()

### Prototype

```c
#include <pthread.h>

int pthread_join(pthread_t thread, void **retval);
```

| Parameter | Mô tả                                           |
| --------- | ----------------------------------------------- |
| `thread`  | Thread ID của thread cần đợi                    |
| `retval`  | Con trỏ để lưu exit status (NULL nếu không cần) |

| Return value | Ý nghĩa                                |
| ------------ | -------------------------------------- |
| `0`          | Thành công                             |
| `EINVAL`     | Thread không thể join                  |
| `ESRCH`      | Thread không tồn tại                   |
| `EDEADLK`    | Deadlock detected (tự join chính mình) |

### Ví dụ chi tiết:

```c
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int start;
    int end;
} Range;

typedef struct {
    long sum;
    int count;
} Result;

void *sum_range(void *arg) {
    Range *range = (Range *)arg;
    Result *result = malloc(sizeof(Result));
    
    result->sum = 0;
    result->count = 0;
    
    for (int i = range->start; i <= range->end; i++) {
        result->sum += i;
        result->count++;
    }
    
    printf("Thread: sum from %d to %d = %ld\n", 
           range->start, range->end, result->sum);
    
    return result;
}

int main() {
    pthread_t threads[4];
    Range ranges[4] = {
        {1, 25},
        {26, 50},
        {51, 75},
        {76, 100}
    };
    
    // Tạo 4 threads
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, sum_range, &ranges[i]);
    }
    
    // Join và thu thập kết quả
    long total_sum = 0;
    int total_count = 0;
    
    for (int i = 0; i < 4; i++) {
        Result *result;
        pthread_join(threads[i], (void **)&result);
        
        total_sum += result->sum;
        total_count += result->count;
        
        free(result);
    }
    
    printf("\nTotal sum from 1 to 100: %ld\n", total_sum);
    printf("Total count: %d\n", total_count);
    
    return 0;
}
```

**Output:**
```
Thread: sum from 1 to 25 = 325
Thread: sum from 26 to 50 = 950
Thread: sum from 51 to 75 = 1575
Thread: sum from 76 to 100 = 2200

Total sum from 1 to 100: 5050
Total count: 100
```

## 4.3. Thread Detaching

### Detaching là gì?

- Joinable thread (mặc định): khi thread kết thúc, bộ dữ liệu liên quan đến thread (thread descriptor, tài nguyên hệ thống) vẫn giữ lại cho đến khi một thread khác gọi pthread_join() để lấy mã trả về và giải phóng tài nguyên đó.

- Detached thread: khi thread kết thúc, hệ thống tự động giải phóng tất cả tài nguyên liên quan đến thread — không thể gọi pthread_join() với thread này để lấy mã trả về.

```
Joinable Thread (default):      Detached Thread:
┌──────────────────┐            ┌──────────────────┐
│  Thread runs     │            │  Thread runs     │
│       │          │            │       │          │
│       ▼          │            │       ▼          │
│  Thread exits    │            │  Thread exits    │
│       │          │            │       │          │
│   Wait for join  │◄── (hung)  │  Auto cleanup    │ ← (immediate)
│       │          │            │                  │
│   Resources freed│            │                  │
└──────────────────┘            └──────────────────┘
```

### Khi nào dùng Detach?

1. Không cần biết kết quả của thread
2. Không cần đợi thread hoàn thành
3. Thread chạy background tasks (như logging, monitoring)

## 4.4. pthread_detach()

### Prototype

```c
#include <pthread.h>

int pthread_detach(pthread_t thread);
```

| Return value | Ý nghĩa               |
| ------------ | --------------------- |
| `0`          | Thành công            |
| `EINVAL`     | Thread không joinable |
| `ESRCH`      | Thread không tồn tại  |

### Ví dụ:

```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *background_task(void *arg) {
    int id = *(int *)arg;
    
    // Tự detach ngay từ đầu
    pthread_detach(pthread_self());
    
    for (int i = 0; i < 3; i++) {
        printf("Background thread %d: working (%d)...\n", id, i);
        sleep(1);
    }
    
    printf("Background thread %d: completed\n", id);
    return NULL;
}

int main() {
    pthread_t threads[3];
    int ids[] = {1, 2, 3};
    
    // Tạo detached threads
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, background_task, &ids[i]);
        // Hoặc detach từ main: pthread_detach(threads[i]);
    }
    
    printf("Main thread: doing other work...\n");
    
    // Main không cần join, detached threads tự cleanup
    sleep(5);  // Đợi đủ thời gian cho demo
    
    printf("Main thread: exiting\n");
    return 0;
}
```

### Tạo thread với attribute detached:

```c
#include <stdio.h>
#include <pthread.h>

void *worker(void *arg) {
    printf("Detached thread running\n");
    return NULL;
}

int main() {
    pthread_t tid;
    pthread_attr_t attr;
    
    // Khởi tạo thread attributes
    pthread_attr_init(&attr);
    
    // Set detach state
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    
    // Tạo thread với attribute
    pthread_create(&tid, &attr, worker, NULL);
    
    // Cleanup attributes
    pthread_attr_destroy(&attr);
    
    // Không cần pthread_join()
    sleep(1);  // Để thread có thời gian chạy
    
    return 0;
}
```

---

# 5. Thread Synchronization

Khi multiple threads truy cập shared resources, cần có cơ chế đồng bộ hóa để tránh data corruption.

## 5.1. Race Condition

### Race Condition là gì?

**Race Condition** xảy ra khi nhiều threads truy cập và thay đổi shared data đồng thời, dẫn đến kết quả không xác định (undefined behavior).

### Ví dụ Race Condition:

```c
#include <stdio.h>
#include <pthread.h>

int counter = 0;  // Shared variable

void *increment(void *arg) {
    for (int i = 0; i < 100000; i++) {
        counter++;  // Race condition!
        // counter++ thực ra gồm 3 bước:
        // 1. Read counter từ memory
        // 2. Increment giá trị
        // 3. Write lại vào memory
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    // Expected: 200000
    // Actual: unpredictable (often less than 200000)
    printf("Counter = %d (expected 200000)\n", counter);
    
    return 0;
}
```

**Output (chạy nhiều lần):**
```
Counter = 143256 (expected 200000)
Counter = 167892 (expected 200000)
Counter = 158341 (expected 200000)
```

### Tại sao xảy ra Race Condition?

```
Thread 1                    Memory              Thread 2
    │                    ┌────────┐                │
    │ READ counter       │   10   │                │
    │◄───────────────────│        │                │
    │                    │        │ READ counter   │
    │                    │        │───────────────►│
    │ counter+1 = 11     │        │                │
    │                    │        │ counter+1 = 11 │
    │ WRITE 11           │        │                │
    │───────────────────►│   11   │                │
    │                    │        │ WRITE 11       │
    │                    │   11   │◄───────────────│
    │                    └────────┘                │
    │                                              │

Expected: counter = 12
Actual: counter = 11 (Thread 2 overwrote Thread 1's work!)
```

## 5.2. Critical Section

### Critical Section là gì?

**Critical Section** là đoạn code truy cập shared resources và cần được bảo vệ để chỉ một thread thực thi tại một thời điểm.

```
            Thread 1                Thread 2
                │                      │
    ┌───────────▼───────────┐          │
    │   Non-critical code   │          │
    └───────────┬───────────┘          │
                │                      │
    ┌───────────▼───────────┐          │
    │   ENTER Critical      │◄─────────┤ (BLOCKED)
    │   Section (lock)      │          │    ↑
    └───────────┬───────────┘          │    │
                │                      │    │
    ┌───────────▼───────────┐          │    │
    │   Access shared       │          │    │ Waiting
    │   resource            │          │    │
    └───────────┬───────────┘          │    │
                │                      │    │
    ┌───────────▼───────────┐          │    │
    │   EXIT Critical       │          │    │
    │   Section (unlock)    │──────────┤    │
    └───────────┬───────────┘          │    ↓
                │              ┌───────▼───────────┐
                │              │   ENTER Critical  │
                │              │   Section (lock)  │
                │              └───────────────────┘
```

### Giải pháp: Sử dụng Synchronization Primitives

Linux cung cấp các cơ chế đồng bộ hóa:

| Mechanism              | Mô tả                                                    |
| ---------------------- | -------------------------------------------------------- |
| **Mutex**              | Mutual exclusion lock, chỉ 1 thread vào critical section |
| **Condition Variable** | Cho phép threads đợi event/condition                     |
| **Semaphore**          | Counting semaphore, cho phép N threads                   |
| **Read-Write Lock**    | Nhiều readers hoặc 1 writer                              |
| **Spinlock**           | Busy-waiting lock (kernel level)                         |

---

# 6. Thread Sync - Mutex Lock

## 6.1. Mutex Lock

### Mutex là gì?

**Mutex** (Mutual Exclusion) là cơ chế đồng bộ hóa đảm bảo chỉ có một thread được phép vào critical section tại một thời điểm.

```
Mutex States:
┌─────────────────────────────────────────────────────────────┐
│                                                             │
│   UNLOCKED                         LOCKED                   │
│   ┌───────┐                       ┌───────┐                 │
│   │       │      lock()           │ Owner │                 │
│   │ Free  │ ─────────────────────►│Thread │                 │
│   │       │                       │       │                 │
│   └───────┘◄──────────────────────└───────┘                 │
│               unlock()                                      │
│                                                             │
│   Other threads calling lock() will BLOCK until unlocked   │
└─────────────────────────────────────────────────────────────┘
```

### Quy tắc sử dụng Mutex:

1. Chỉ owner (thread đã lock) mới được unlock
2. Phải unlock sau mỗi lần lock
3. Lock phải thành công trước khi truy cập resource
4. Tránh deadlock (đừng lock 2 mutexes theo thứ tự khác nhau)

## 6.2. Allocate Mutex

### Khởi tạo Mutex

Có 2 cách khởi tạo mutex:

**Cách 1: Static initialization**

```c
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
```

**Cách 2: Dynamic initialization**

```c
#include <pthread.h>

int pthread_mutex_init(pthread_mutex_t *mutex, 
                       const pthread_mutexattr_t *attr);

int pthread_mutex_destroy(pthread_mutex_t *mutex);
```

| Parameter | Mô tả                             |
| --------- | --------------------------------- |
| `mutex`   | Con trỏ đến mutex variable        |
| `attr`    | Mutex attributes (NULL = default) |

### Ví dụ:

```c
#include <pthread.h>

// Cách 1: Static
pthread_mutex_t static_mutex = PTHREAD_MUTEX_INITIALIZER;

// Cách 2: Dynamic
pthread_mutex_t dynamic_mutex;

int main() {
    // Khởi tạo dynamic mutex
    pthread_mutex_init(&dynamic_mutex, NULL);
    
    // ... sử dụng mutexes ...
    
    // Cleanup dynamic mutex
    pthread_mutex_destroy(&dynamic_mutex);
    
    return 0;
}
```

## 6.3. Locking & Unlocking Mutex

### Các hàm Lock/Unlock

```c
#include <pthread.h>

// Lock mutex (blocking)
int pthread_mutex_lock(pthread_mutex_t *mutex);

// Unlock mutex
int pthread_mutex_unlock(pthread_mutex_t *mutex);

// Try to lock (non-blocking)
int pthread_mutex_trylock(pthread_mutex_t *mutex);
```

| Function                  | Behavior                                        |
| ------------------------- | ----------------------------------------------- |
| `pthread_mutex_lock()`    | Block nếu mutex đã locked, đợi cho đến khi free |
| `pthread_mutex_unlock()`  | Unlock mutex, cho phép thread khác vào          |
| `pthread_mutex_trylock()` | Return ngay, 0 nếu thành công, EBUSY nếu locked |

### Ví dụ: Giải quyết Race Condition

```c
#include <stdio.h>
#include <pthread.h>

int counter = 0;
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

void *increment(void *arg) {
    for (int i = 0; i < 100000; i++) {
        // Lock trước khi truy cập shared variable
        pthread_mutex_lock(&counter_mutex);
        
        counter++;  // Critical section
        
        // Unlock sau khi xong
        pthread_mutex_unlock(&counter_mutex);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    // Giờ sẽ luôn là 200000
    printf("Counter = %d (expected 200000)\n", counter);
    
    return 0;
}
```

**Output (luôn đúng):**
```
Counter = 200000 (expected 200000)
```

### Ví dụ: Thread-safe Counter

```c
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct {
    int value;
    pthread_mutex_t mutex;
} SafeCounter;

void counter_init(SafeCounter *c) {
    c->value = 0;
    pthread_mutex_init(&c->mutex, NULL);
}

void counter_increment(SafeCounter *c) {
    pthread_mutex_lock(&c->mutex);
    c->value++;
    pthread_mutex_unlock(&c->mutex);
}

int counter_get(SafeCounter *c) {
    pthread_mutex_lock(&c->mutex);
    int val = c->value;
    pthread_mutex_unlock(&c->mutex);
    return val;
}

void counter_destroy(SafeCounter *c) {
    pthread_mutex_destroy(&c->mutex);
}

// Global thread-safe counter
SafeCounter shared_counter;

void *worker(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < 50000; i++) {
        counter_increment(&shared_counter);
    }
    printf("Thread %d finished\n", id);
    return NULL;
}

int main() {
    counter_init(&shared_counter);
    
    pthread_t threads[4];
    int ids[] = {1, 2, 3, 4};
    
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }
    
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Final counter value: %d (expected 200000)\n", 
           counter_get(&shared_counter));
    
    counter_destroy(&shared_counter);
    return 0;
}
```

---

# 7. Thread Sync - Conditional Variable

## 7.1. Conditional Variable

### Conditional Variable là gì?

**Conditional Variable** (biến điều kiện) cho phép một thread đợi (sleep) cho đến khi một điều kiện cụ thể được thỏa mãn, và thread khác thông báo (signal) khi điều kiện đã thay đổi.

```
Producer-Consumer với Condition Variable:
┌──────────────────────────────────────────────────────────────────┐
│                                                                  │
│   Producer Thread              Buffer             Consumer Thread│
│        │                    ┌────────┐                  │        │
│        │                    │ Empty  │                  │        │
│        │  produce()         │        │   wait()         │        │
│        │───────────────────►│  Item  │◄─────────────────│        │
│        │                    │        │   (sleeping)     │        │
│        │  signal()          └────┬───┘                  │        │
│        │──────────────────────────────────────────────► │        │
│        │                         │      (wakes up)      │        │
│        │                         │   consume()          │        │
│        │                         │◄─────────────────────│        │
│                                                                  │
└──────────────────────────────────────────────────────────────────┘
```

### Tại sao cần Conditional Variable?

**Không có Condition Variable (polling - tốn CPU):**
```c
// ❌ Busy waiting - wastes CPU
while (queue_empty()) {
    // Liên tục kiểm tra, tốn CPU
}
```

**Với Condition Variable (efficient):**
```c
// ✅ Sleep until condition changes
pthread_mutex_lock(&mutex);
while (queue_empty()) {
    pthread_cond_wait(&cond, &mutex);  // Sleep, release mutex
}
pthread_mutex_unlock(&mutex);
```

## 7.2. Allocate Conditional Variable

### Khởi tạo Condition Variable

**Cách 1: Static initialization**

```c
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
```

**Cách 2: Dynamic initialization**

```c
#include <pthread.h>

int pthread_cond_init(pthread_cond_t *cond, 
                      const pthread_condattr_t *attr);

int pthread_cond_destroy(pthread_cond_t *cond);
```

### Lưu ý quan trọng:

Condition variable **luôn** được sử dụng cùng với **mutex**:

```c
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
```

## 7.3. Signaling & Waiting

### Các hàm Wait và Signal

```c
#include <pthread.h>

// Wait - sleep until signaled
int pthread_cond_wait(pthread_cond_t *cond, 
                      pthread_mutex_t *mutex);

// Signal - wake up ONE waiting thread
int pthread_cond_signal(pthread_cond_t *cond);

// Broadcast - wake up ALL waiting threads
int pthread_cond_broadcast(pthread_cond_t *cond);
```

| Function                   | Mô tả                                                 |
| -------------------------- | ----------------------------------------------------- |
| `pthread_cond_wait()`      | Atomically unlock mutex và sleep, lock lại khi wakeup |
| `pthread_cond_signal()`    | Đánh thức **một** thread đang wait                    |
| `pthread_cond_broadcast()` | Đánh thức **tất cả** threads đang wait                |

### Quy trình Wait:

```
pthread_cond_wait() internally does:
1. Unlock mutex (atomically)
2. Sleep, waiting for signal
3. When signaled, wake up
4. Lock mutex again (before returning)
```

### Pattern chuẩn:

```c
// Waiter (Consumer)
pthread_mutex_lock(&mutex);
while (!condition) {  // PHẢI dùng while, không dùng if
    pthread_cond_wait(&cond, &mutex);
}
// Process data
pthread_mutex_unlock(&mutex);

// Signaler (Producer)
pthread_mutex_lock(&mutex);
// Change condition
condition = true;
pthread_cond_signal(&cond);  // hoặc broadcast
pthread_mutex_unlock(&mutex);
```

### Ví dụ: Producer-Consumer

```c
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 5

typedef struct {
    int buffer[BUFFER_SIZE];
    int count;
    int in;   // Index to insert
    int out;  // Index to remove
    pthread_mutex_t mutex;
    pthread_cond_t not_full;   // Condition: buffer không đầy
    pthread_cond_t not_empty;  // Condition: buffer không rỗng
} BoundedBuffer;

BoundedBuffer bb;

void buffer_init(BoundedBuffer *b) {
    b->count = 0;
    b->in = 0;
    b->out = 0;
    pthread_mutex_init(&b->mutex, NULL);
    pthread_cond_init(&b->not_full, NULL);
    pthread_cond_init(&b->not_empty, NULL);
}

void buffer_put(BoundedBuffer *b, int item) {
    pthread_mutex_lock(&b->mutex);
    
    // Đợi cho đến khi buffer không đầy
    while (b->count == BUFFER_SIZE) {
        printf("Producer: buffer full, waiting...\n");
        pthread_cond_wait(&b->not_full, &b->mutex);
    }
    
    // Thêm item vào buffer
    b->buffer[b->in] = item;
    b->in = (b->in + 1) % BUFFER_SIZE;
    b->count++;
    
    printf("Producer: put %d, count = %d\n", item, b->count);
    
    // Signal cho consumer biết buffer không rỗng
    pthread_cond_signal(&b->not_empty);
    
    pthread_mutex_unlock(&b->mutex);
}

int buffer_get(BoundedBuffer *b) {
    pthread_mutex_lock(&b->mutex);
    
    // Đợi cho đến khi buffer không rỗng
    while (b->count == 0) {
        printf("Consumer: buffer empty, waiting...\n");
        pthread_cond_wait(&b->not_empty, &b->mutex);
    }
    
    // Lấy item từ buffer
    int item = b->buffer[b->out];
    b->out = (b->out + 1) % BUFFER_SIZE;
    b->count--;
    
    printf("Consumer: got %d, count = %d\n", item, b->count);
    
    // Signal cho producer biết buffer không đầy
    pthread_cond_signal(&b->not_full);
    
    pthread_mutex_unlock(&b->mutex);
    
    return item;
}

void buffer_destroy(BoundedBuffer *b) {
    pthread_mutex_destroy(&b->mutex);
    pthread_cond_destroy(&b->not_full);
    pthread_cond_destroy(&b->not_empty);
}

void *producer(void *arg) {
    for (int i = 1; i <= 10; i++) {
        buffer_put(&bb, i);
        usleep(100000);  // 100ms
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < 10; i++) {
        int item = buffer_get(&bb);
        usleep(200000);  // 200ms (slower than producer)
    }
    return NULL;
}

int main() {
    buffer_init(&bb);
    
    pthread_t prod, cons;
    
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);
    
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    
    buffer_destroy(&bb);
    
    printf("Done!\n");
    return 0;
}
```

**Output mẫu:**
```
Producer: put 1, count = 1
Consumer: got 1, count = 0
Producer: put 2, count = 1
Producer: put 3, count = 2
Consumer: got 2, count = 1
Producer: put 4, count = 2
Producer: put 5, count = 3
Consumer: got 3, count = 2
...
Done!
```

### Tại sao dùng `while` thay vì `if`?

```c
// ❌ WRONG - có thể bị spurious wakeup
if (!condition) {
    pthread_cond_wait(&cond, &mutex);
}

// ✅ CORRECT - kiểm tra lại condition sau khi wakeup
while (!condition) {
    pthread_cond_wait(&cond, &mutex);
}
```

**Lý do:**
1. **Spurious wakeup**: Thread có thể được đánh thức mà không có signal
2. **Stolen wakeup**: Thread khác có thể đã xử lý condition trước khi thread này wakeup


### [8. Exercises](#8-exercises)
- [8.1. Exercise List](#81-exercise-list)

---

# 8. Exercises

## 8.1. Exercise List

Để củng cố kiến thức, hãy hoàn thành các bài tập trong thư mục [Exercises](./Exercises/):

1. **[Exercise 01: Thread Creation](./Exercises/exercise-01-thread-creation/)**
   - Làm quen với `pthread_create` và `pthread_join`.
2. **[Exercise 02: Thread Arguments & Return Values](./Exercises/exercise-02-thread-args-return/)**
   - Truyền struct vào thread và lấy kết quả trả về từ Heap.
3. **[Exercise 03: Race Condition & Mutex](./Exercises/exercise-03-race-condition/)**
   - Quan sát Race Condition và sửa lỗi bằng Mutex.
4. **[Exercise 04: Condition Variable](./Exercises/exercise-04-conditional-variable/)**
   - Thực hiện bài toán Producer-Consumer.

---

## 📚 Tài liệu tham khảo

- *The Linux Programming Interface* - Michael Kerrisk (Chapters 29-33)
- *Programming with POSIX Threads* - David R. Butenhof
- Linux man pages: `man pthreads`, `man pthread_create`, `man pthread_mutex_lock`
- [POSIX Threads Programming](https://computing.llnl.gov/tutorials/pthreads/)