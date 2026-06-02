# Embedded Linux - IPC Shared Memory

> Tài liệu học về Linux Shared Memory: System V, POSIX, Anonymous Shared Memory, mmap flags, Memory Barrier, và Synchronization Patterns.

---

## 📑 Mục Lục

### [1. Introduction](#1-introduction)
- [1.1. What is IPC Shared Memory?](#11-what-is-ipc-shared-memory)
- [1.2. Why use Shared Memory?](#12-why-use-shared-memory)
- [1.3. So sánh Shared Memory với các IPC khác](#13-so-sánh-shared-memory-với-các-ipc-khác)

### [2. System V Shared Memory](#2-system-v-shared-memory)
- [2.1. Implementation Workflow](#21-implementation-workflow)
- [2.2. Create Key (ftok)](#22-create-key-ftok)
- [2.3. Create Segment (shmget)](#23-create-segment-shmget)
- [2.4. Attach (shmat)](#24-attach-shmat)
- [2.5. Detach (shmdt)](#25-detach-shmdt)
- [2.6. Control/Remove (shmctl)](#26-controlremove-shmctl)
- [2.7. Code Example: System V Writer/Reader](#27-code-example-system-v-writerreader)

### [3. POSIX Shared Memory](#3-posix-shared-memory)
- [3.1. Implementation Workflow](#31-implementation-workflow)
- [3.2. Create/Open Object (shm_open)](#32-createopen-object-shm_open)
- [3.3. Set Size (ftruncate)](#33-set-size-ftruncate)
- [3.4. Map Memory (mmap)](#34-map-memory-mmap)
- [3.5. Unmap & Remove (munmap, shm_unlink)](#35-unmap--remove-munmap-shm_unlink)
- [3.6. Code Example: POSIX Writer/Reader](#36-code-example-posix-writerreader)

### [4. mmap Flags & Options](#4-mmap-flags--options)
- [4.1. Memory Protection Flags](#41-memory-protection-flags)
- [4.2. Map Type Flags](#42-map-type-flags)
- [4.3. Sync Options (msync, mlock)](#43-sync-options-msync-mlock)

### [5. Anonymous Shared Memory (mmap MAP_ANONYMOUS)](#5-anonymous-shared-memory-mmap-map_anonymous)
- [5.1. Shared Memory giữa Cha và Con](#51-shared-memory-giữa-cha-và-con)
- [5.2. Code Example: Anonymous mmap](#52-code-example-anonymous-mmap)

### [6. Memory Barrier & Synchronization](#6-memory-barrier--synchronization)
- [6.1. Vấn đề Memory Ordering](#61-vấn-đề-memory-ordering)
- [6.2. Linux Memory Barrier Functions](#62-linux-memory-barrier-functions)
- [6.3. Atomic Operations with GCC Built-ins](#63-atomic-operations-with-gcc-built-ins)

### [7. Shared Memory + Semaphore Pattern](#7-shared-memory--semaphore-pattern)
- [7.1. Tại sao cần Semaphore?](#71-tại-sao-cần-semaphore)
- [7.2. Code Example: Shared Counter với Semaphore](#72-code-example-shared-counter-với-semaphore)

---

# 1. Introduction

## 1.1. What is IPC Shared Memory?

**Shared Memory** là cơ chế cho phép nhiều process (tiến trình) cùng truy cập vào một vùng nhớ vật lý chung.

- Mỗi process sẽ map (ánh xạ) vùng nhớ chung này vào không gian địa chỉ ảo (virtual address space) riêng của nó.
- Khi Process A ghi dữ liệu vào vùng nhớ này, Process B sẽ thấy ngay lập tức.

```
Process A                           Process B
Virtual Memory                     Virtual Memory
┌──────────────────────┐          ┌──────────────────────┐
│                      │          │                      │
│    Stack             │          │    Stack             │
│                      │          │                      │
│    Shared Memory ◄───┼──────────┼───► Shared Memory    │
│      ptr    ↑        │  Kernel  │    ptr               │
│                      │  Maps    │                      │
│    Heap              │  Same    │    Heap              │
│                      │  Frames  │                      │
│    Data              │          │    Data              │
│    Text              │          │    Text              │
└──────────────────────┘          └──────────────────────┘
         │                              │
         └──────────────┬───────────────┘
                        │
                        ▼
              ┌─────────────────────┐
              │   Physical Memory   │
              │   (RAM Pages)       │
              │   Shared by both    │
              └─────────────────────┘
```

## 1.2. Why use Shared Memory?

**Ưu điểm:**
- **Tốc độ nhanh nhất**: Đây là phương pháp IPC nhanh nhất vì dữ liệu không cần phải copy qua lại giữa kernel space và user space (như Pipe, Socket, Message Queue).
- **Zero-copy**: Dữ liệu nằm sẵn ở đó, chỉ cần truy cập trực tiếp bằng con trỏ.
- **Dung lượng lớn**: Có thể chia sẻ nhiều MB hoặc GB dữ liệu.

**Nhược điểm:**
- **Race Condition**: Vì nhiều process cùng truy cập, nếu không có cơ chế đồng bộ (Semaphore hay Mutex), dữ liệu sẽ bị sai lệch.
- **Phức tạp**: Cần quản lý memory layout và đồng bộ hóa.
- **Bảo mật**: Bất kỳ process nào biết key/name đều có thể truy cập.

## 1.3. So sánh Shared Memory với các IPC khác

| IPC Method      | Copy data | Speed     | Use case                  |
| --------------- | --------- | --------- | ------------------------- |
| **Shared Memory** | 0 lần   | ⚡ Nhanh nhất | Dữ liệu lớn, cần tốc độ cao |
| **Pipe/FIFO**   | 2 lần (kernel) | 🐢 Trung bình | Byte stream, đơn giản   |
| **Message Queue**| 2 lần (kernel) | 🐢 Trung bình | Message có cấu trúc     |
| **Socket**      | 2 lần (kernel) | 🐢 Trung bình | Giao tiếp qua mạng       |

---

# 2. System V Shared Memory

Cơ chế cũ, sử dụng Key (`ftok`).

## 2.1. Implementation Workflow

```
┌─────────┐   ┌──────────┐   ┌─────────┐   ┌──────────┐   ┌──────────┐
│  ftok() │──►│ shmget() │──►│ shmat() │──►│ shmdt()  │──►│ shmctl() │
│ Tạo key │   │Tạo seg   │   │ Attach  │   │ Detach   │   │ Remove   │
└─────────┘   └──────────┘   └─────────┘   └──────────┘   └──────────┘
```

## 2.2. Create Key (ftok)

```c
key_t key = ftok("/tmp/shmfile", 65);
if (key == -1) {
    perror("ftok");
    exit(EXIT_FAILURE);
}
```

## 2.3. Create Segment (shmget)

```c
#include <sys/ipc.h>
#include <sys/shm.h>

int shmget(key_t key, size_t size, int shmflg);
```

- `key`: Key từ ftok hoặc IPC_PRIVATE.
- `size`: Kích thước segment (bytes). Nên là bội số của PAGE_SIZE (4096).
- `shmflg`: `0666 | IPC_CREAT` (tạo mới) hoặc `IPC_CREAT | IPC_EXCL`.
- Returns: `shmid` (số dương) hoặc -1 (lỗi).

```c
int shmid = shmget(key, 4096, 0666 | IPC_CREAT);
if (shmid == -1) {
    perror("shmget");
    exit(EXIT_FAILURE);
}
```

## 2.4. Attach (shmat)

```c
void *shmat(int shmid, const void *shmaddr, int shmflg);
```

- `shmid`: ID từ shmget.
- `shmaddr`: NULL (để OS tự chọn địa chỉ).
- `shmflg`: 0 (read-write), `SHM_RDONLY` (read-only).
- Returns: Con trỏ `void *` tới vùng nhớ chung, hoặc `(void *)-1` (lỗi).

```c
int *shared_data = (int *)shmat(shmid, NULL, 0);
if (shared_data == (void *)-1) {
    perror("shmat");
    exit(EXIT_FAILURE);
}
```

## 2.5. Detach (shmdt)

```c
int shmdt(const void *shmaddr);
```

- Khi không dùng nữa, process phải detach.
- Segment vẫn tồn tại trong Kernel cho đến khi bị xóa bằng `shmctl()`.

```c
if (shmdt(shared_data) == -1) {
    perror("shmdt");
}
```

## 2.6. Control/Remove (shmctl)

```c
int shmctl(int shmid, int cmd, struct shmid_ds *buf);
```

| cmd         | Mô tả                                                          |
| ----------- | -------------------------------------------------------------- |
| `IPC_RMID`  | Đánh dấu segment để xóa. Xóa thật sự khi process cuối cùng detach |
| `IPC_STAT`  | Lấy thông tin segment vào struct `shmid_ds`                    |
| `IPC_SET`   | Set permissions                                                |
| `SHM_LOCK`  | Lock segment vào RAM (mlock)                                   |
| `SHM_UNLOCK`| Unlock segment                                                 |

```c
struct shmid_ds ds;
shmctl(shmid, IPC_STAT, &ds);
printf("Segment size: %lu\n", ds.shm_segsz);
printf("Number of attaches: %lu\n", ds.shm_nattch);

// Xóa segment
shmctl(shmid, IPC_RMID, NULL);
```

## 2.7. Code Example: System V Writer/Reader

### Writer:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY_PATH "/tmp"
#define SHM_PROJ_ID  65
#define SHM_SIZE     1024

typedef struct {
    int written;      // Flag: 1 = data ready to read
    int data[256];    // Shared data
} SharedData;

int main() {
    // 1. Create key
    key_t key = ftok(SHM_KEY_PATH, SHM_PROJ_ID);
    if (key == -1) { perror("ftok"); exit(1); }

    // 2. Create shared memory
    int shmid = shmget(key, sizeof(SharedData), 0666 | IPC_CREAT);
    if (shmid == -1) { perror("shmget"); exit(1); }

    // 3. Attach
    SharedData *sd = (SharedData *)shmat(shmid, NULL, 0);
    if (sd == (void *)-1) { perror("shmat"); exit(1); }

    // 4. Write data
    printf("Writer: writing data...\n");
    for (int i = 0; i < 10; i++) {
        sd->data[i] = i * i;
    }
    sd->written = 1;  // Signal: data ready
    printf("Writer: done. Reader can now read.\n");

    // 5. Detach (keep segment alive for reader)
    printf("Writer: press Enter to detach and remove...\n");
    getchar();
    shmdt(sd);

    // 6. Remove (optional - can also be done by reader)
    shmctl(shmid, IPC_RMID, NULL);
    printf("Writer: segment removed.\n");

    return 0;
}
```

### Reader:

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY_PATH "/tmp"
#define SHM_PROJ_ID  65

typedef struct {
    int written;
    int data[256];
} SharedData;

int main() {
    // 1. Get same key
    key_t key = ftok(SHM_KEY_PATH, SHM_PROJ_ID);
    if (key == -1) { perror("ftok"); exit(1); }

    // 2. Get shared memory ID
    int shmid = shmget(key, sizeof(SharedData), 0666);
    if (shmid == -1) { perror("shmget"); exit(1); }

    // 3. Attach
    SharedData *sd = (SharedData *)shmat(shmid, NULL, 0);
    if (sd == (void *)-1) { perror("shmat"); exit(1); }

    // 4. Wait for writer to finish (simple spin - normally use semaphore)
    while (!sd->written) {
        usleep(10000);  // 10ms
    }

    // 5. Read data
    printf("Reader: reading data...\n");
    for (int i = 0; i < 10; i++) {
        printf("data[%d] = %d\n", i, sd->data[i]);
    }

    // 6. Detach
    shmdt(sd);

    return 0;
}
```

**Compile & Run:**
```bash
gcc sysv_writer.c -o sysv_writer
gcc sysv_reader.c -o sysv_reader

# Terminal 1: chạy writer trước, đợi prompt, chuyển qua reader
./sysv_writer &

# Terminal 2: chạy reader (sẽ thấy dữ liệu)
./sysv_reader
```

---

# 3. POSIX Shared Memory

Cơ chế mới hơn, sử dụng file descriptor và `mmap`.

## 3.1. Implementation Workflow

```
┌────────────┐   ┌──────────────┐   ┌──────────┐   ┌───────────┐   ┌──────────────┐
│ shm_open() │──►│ ftruncate()  │──►│ mmap()   │──►│ munmap()  │──►│ shm_unlink()│
│ Tạo/mở obj │   │ Set kích thước│   │ Map vào  │   │ Unmap     │   │ Xóa object  │
└────────────┘   └──────────────┘   │ bộ nhớ   │   └───────────┘   └──────────────┘
                                    └──────────┘
```

## 3.2. Create/Open Object (shm_open)

```c
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int shm_open(const char *name, int oflag, mode_t mode);
```

- `name`: Bắt đầu bằng `/` (vd: `/my_shm`).
- `oflag`: `O_CREAT | O_RDWR` (tạo mới), `O_RDONLY`, `O_EXCL`.
- `mode`: Permission (vd `0644`).
- Returns: File descriptor.

## 3.3. Set Size (ftruncate)

Mới tạo thì size = 0:

```c
int fd = shm_open("/my_shm", O_CREAT | O_RDWR, 0644);
ftruncate(fd, 4096);  // Set size to 4KB
```

## 3.4. Map Memory (mmap)

```c
#include <sys/mman.h>

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
```

**Các tham số quan trọng:**
- `addr`: NULL (OS tự chọn).
- `length`: Kích thước vùng nhớ muốn map.
- `prot`: Protection flags (xem mục 4.1).
- `flags`: `MAP_SHARED` (quan trọng - thay đổi được ghi lại) hoặc `MAP_PRIVATE`.
- `fd`: File descriptor (hoặc -1 với MAP_ANONYMOUS).
- `offset`: 0 (từ đầu file/shm object).

```c
int *shared = (int *)mmap(NULL, 4096, PROT_READ | PROT_WRITE,
                          MAP_SHARED, fd, 0);
if (shared == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
}
```

## 3.5. Unmap & Remove (munmap, shm_unlink)

```c
munmap(ptr, 4096);           // Unmap memory
shm_unlink("/my_shm");       // Remove shm object
```

## 3.6. Code Example: POSIX Writer/Reader

### Writer:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHM_NAME "/my_shared_memory"
#define SHM_SIZE 4096

typedef struct {
    int ready;
    char message[256];
} SharedData;

int main() {
    // 1. Create shared memory object
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0644);
    if (fd == -1) { perror("shm_open"); exit(1); }

    // 2. Set size
    if (ftruncate(fd, SHM_SIZE) == -1) { perror("ftruncate"); exit(1); }

    // 3. Map
    SharedData *sd = (SharedData *)mmap(NULL, SHM_SIZE,
        PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (sd == MAP_FAILED) { perror("mmap"); exit(1); }

    close(fd);  // FD not needed after mmap

    // 4. Write
    printf("Writer: writing message...\n");
    strcpy(sd->message, "Hello from POSIX shared memory!");
    sd->ready = 1;

    printf("Writer: press Enter to cleanup...\n");
    getchar();

    // 5. Cleanup
    munmap(sd, SHM_SIZE);
    shm_unlink(SHM_NAME);
    printf("Writer: done.\n");

    return 0;
}
```

### Reader:

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHM_NAME "/my_shared_memory"
#define SHM_SIZE 4096

typedef struct {
    int ready;
    char message[256];
} SharedData;

int main() {
    // 1. Open shared memory object
    int fd = shm_open(SHM_NAME, O_RDONLY, 0644);
    if (fd == -1) { perror("shm_open"); exit(1); }

    // 2. Map
    SharedData *sd = (SharedData *)mmap(NULL, SHM_SIZE,
        PROT_READ, MAP_SHARED, fd, 0);
    if (sd == MAP_FAILED) { perror("mmap"); exit(1); }

    close(fd);

    // 3. Wait and read
    while (!sd->ready) usleep(10000);
    printf("Reader: message = %s\n", sd->message);

    // 4. Unmap
    munmap(sd, SHM_SIZE);
    printf("Reader: done.\n");

    return 0;
}
```

---

# 4. mmap Flags & Options

## 4.1. Memory Protection Flags

| Flag               | Mô tả                        |
| ------------------ | ---------------------------- |
| `PROT_NONE`        | Không có quyền truy cập       |
| `PROT_READ`        | Có thể đọc                   |
| `PROT_WRITE`       | Có thể ghi                   |
| `PROT_EXEC`        | Có thể thực thi              |

Thường dùng: `PROT_READ | PROT_WRITE`

## 4.2. Map Type Flags

| Flag               | Mô tả                                                        |
| ------------------ | ------------------------------------------------------------ |
| **MAP_SHARED**     | **Chia sẻ**: Thay đổi được ghi vào file/shm, process khác thấy được (dùng cho IPC) |
| **MAP_PRIVATE**    | **Riêng**: Thay đổi chỉ trong process (Copy-on-Write), không ảnh hưởng đến process khác |
| **MAP_ANONYMOUS**  | Không dùng file (fd = -1), tạo vùng nhớ ẩn danh             |
| **MAP_POPULATE**   | Pre-fault các pages (mmap nhanh hơn khi truy cập)           |
| **MAP_LOCKED**     | Lock pages vào RAM (không swap)                              |
| **MAP_HUGETLB**    | Dùng huge pages (2MB/1GB) cho hiệu năng cao                  |

```c
// Shared (IPC)
void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE,
                 MAP_SHARED, fd, 0);

// Private (copy-on-write)
void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE,
                 MAP_PRIVATE, fd, 0);

// Anonymous (không cần fd)
void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE,
                 MAP_SHARED | MAP_ANONYMOUS, -1, 0);
```

## 4.3. Sync Options (msync, mlock)

### msync() - Đồng bộ shared memory với file:

```c
#include <sys/mman.h>

int msync(void *addr, size_t length, int flags);
```

- `MS_SYNC`: Đồng bộ đồng bộ (block đến khi ghi xong).
- `MS_ASYNC`: Đồng bộ bất đồng bộ (queue ghi, return ngay).
- `MS_INVALIDATE`: Hợp lệ hóa cache (các process khác thấy thay đổi).

```c
// Đảm bảo dữ liệu đã ghi xuống file (giống fsync)
msync(ptr, size, MS_SYNC);
```

### mlock() - Lock pages vào RAM:

```c
#include <sys/mman.h>

int mlock(const void *addr, size_t len);       // Lock pages
int mlockall(int flags);                        // Lock all pages
int munlock(const void *addr, size_t len);     // Unlock
```

```c
// Lock shared memory pages để tránh swapping
mlock(shared_data, sizeof(SharedData));
```

---

# 5. Anonymous Shared Memory (mmap MAP_ANONYMOUS)

## 5.1. Shared Memory giữa Cha và Con

Sau `fork()`, child process thường có bản sao riêng (copy-on-write). Nhưng với `mmap MAP_SHARED | MAP_ANONYMOUS`, cha và con có thể chia sẻ bộ nhớ.

```
Before fork:
   Process
   ┌────────────────────────────┐
   │  mmap(... MAP_ANONYMOUS)   │ ← Vùng nhớ chung (trước fork)
   └────────────────────────────┘
            │ fork()
            ▼
   ┌────────────────┐  ┌────────────────┐
   │   Parent       │  │   Child        │
   │   share same   │  │   share same   │
   │   memory page  │  │   memory page  │
   └────────────────┘  └────────────────┘
```

## 5.2. Code Example: Anonymous mmap

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define SHARED_SIZE 4096

int main() {
    // Create anonymous shared memory (before fork!)
    int *shared_counter = (int *)mmap(NULL, SHARED_SIZE,
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,  // No file needed
        -1, 0);  // fd = -1, offset = 0

    if (shared_counter == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    *shared_counter = 0;

    pid_t pid = fork();
    if (pid == 0) {
        // Child: increment counter
        for (int i = 0; i < 1000000; i++) {
            (*shared_counter)++;  // Có race condition! Cần semaphore.
        }
        printf("Child done. Counter = %d\n", *shared_counter);
        exit(0);
    } else {
        // Parent: increment counter
        for (int i = 0; i < 1000000; i++) {
            (*shared_counter)++;
        }
        wait(NULL);
        printf("Parent done. Final counter = %d\n", *shared_counter);
        // Expected: 2000000, Actual: unpredictable (race condition!)
    }

    munmap(shared_counter, SHARED_SIZE);
    return 0;
}
```

### So sánh Anonymous mmap vs POSIX shm_open:

| Tiêu chí                | Anonymous mmap                 | POSIX shm_open + mmap         |
| ----------------------- | ------------------------------ | ----------------------------- |
| **Tên**                 | Không (chỉ dùng giữa cha-con)  | Có (/name), bất kỳ process nào |
| **Cần file**            | Không (fd = -1)                | Cần (shm_open tạo object)     |
| **fork()**              | Hoạt động (kế thừa mmap)       | Hoạt động                     |
| **Process độc lập**     | Không (không có tên)           | Có                            |
| **Đơn giản**            | Đơn giản nhất                  | Trung bình                    |

---

# 6. Memory Barrier & Synchronization

## 6.1. Vấn đề Memory Ordering

Khi lập trình shared memory, compiler và CPU có thể **reorder** các memory operations, gây ra lỗi khó debug.

```c
// Giả sử shared memory layout:
int data_ready;   // flag
char buffer[256]; // dữ liệu

// Writer:
strcpy(buffer, "important data");
data_ready = 1;       // ❌ CPU/Compiler có thể ghi data_ready trước buffer!

// Reader:
while (!data_ready);  // ❌ Có thể thấy data_ready = 1 nhưng buffer chưa sẵn sàng!
```

## 6.2. Linux Memory Barrier Functions

Memory barrier đảm bảo:
1. Các memory operations **trước** barrier hoàn thành trước barrier
2. Các operations **sau** barrier chỉ thực hiện sau barrier

```c
#include <stdatomic.h>  // C11 atomics (khuyến nghị)

// Dùng atomic flag với memory ordering
atomic_int data_ready = 0;
char buffer[256];

// Writer:
strcpy(buffer, "important data");
atomic_store_explicit(&data_ready, 1, memory_order_release);

// Reader:
while (!atomic_load_explicit(&data_ready, memory_order_acquire));
```

> **Luôn dùng atomic operations** với memory ordering khi làm việc với shared memory. Đây là cách chính xác nhất để tránh race condition trong C11+.

### Linux Kernel-style Barriers (cho C code không dùng C11):

```c
// GCC built-in atomic builtins
__sync_synchronize();  // Full memory barrier

// Hoặc dùng atomic builtins:
__sync_lock_test_and_set(&flag, 1);   // Atomic write
__sync_fetch_and_add(&counter, 1);    // Atomic increment
```

## 6.3. Atomic Operations with GCC Built-ins

```c
#include <stdio.h>
#include <stdatomic.h>

// C11 atomic (C11+)
atomic_int counter = 0;

int old = atomic_fetch_add(&counter, 1);  // counter++, returns old value
atomic_store(&counter, 100);               // counter = 100
int val = atomic_load(&counter);           // Đọc giá trị

// GCC built-ins (C99+)
int val = __sync_add_and_fetch(&counter, 1);  // ++counter
int val = __sync_sub_and_fetch(&counter, 1);  // --counter
bool ret = __sync_bool_compare_and_swap(&counter, old, new); // CAS
```

---

# 7. Shared Memory + Semaphore Pattern

## 7.1. Tại sao cần Semaphore?

Shared Memory là cơ chế nhanh nhất nhưng cũng **nguy hiểm nhất**. Không có đồng bộ, dữ liệu sẽ bị hỏng.

Lý tưởng nhất là kết hợp **Shared Memory (dữ liệu)** + **Semaphore (đồng bộ)**:

```
┌────────────────────────────────────────────────┐
│              Application Pattern                │
│                                                │
│  Shared Memory (data transfer)                  │
│  ┌─────────────────────────────────────┐        │
│  │   data buffer        +  semaphore   │        │
│  │   [   lớn, nhiều    ]  [ đồng bộ ]  │        │
│  └─────────────────────────────────────┘        │
│                                                │
│  Semaphore (POSIX Named / System V)             │
│  - Bảo vệ critical section                      │
│  - Chỉ 1 process đọc/ghi cùng lúc               │
└────────────────────────────────────────────────┘
```

## 7.2. Code Example: Shared Counter với Semaphore

### Shared + Semaphore:

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/wait.h>

#define SHM_NAME "/counter_shm"
#define SEM_NAME "/counter_sem"

typedef struct {
    int counter;
} SharedData;

int main() {
    // Create shared memory
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0644);
    ftruncate(fd, sizeof(SharedData));

    SharedData *sd = (SharedData *)mmap(NULL, sizeof(SharedData),
        PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    // Create named semaphore (initial value = 1 = unlocked)
    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);
    sd->counter = 0;

    // Fork 2 children
    for (int i = 0; i < 2; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child: increment with semaphore protection
            for (int j = 0; j < 1000000; j++) {
                sem_wait(sem);
                sd->counter++;   // Critical section
                sem_post(sem);
            }
            exit(0);
        }
    }

    // Wait for both children
    for (int i = 0; i < 2; i++) wait(NULL);

    printf("Final counter: %d (expected 2000000)\n", sd->counter);

    // Cleanup
    munmap(sd, sizeof(SharedData));
    shm_unlink(SHM_NAME);
    sem_close(sem);
    sem_unlink(SEM_NAME);

    return 0;
}
```

**Output:**
```
Final counter: 2000000 (expected 2000000)
```

---

## 📚 Tài liệu tham khảo

- *The Linux Programming Interface* - Michael Kerrisk (Chapters 48-54)
- *UNIX Network Programming, Volume 2* - W. Richard Stevens
- Linux man pages: `man 7 shm_overview`, `man 2 mmap`, `man 2 shmget`, `man 2 shmat`
- [POSIX Shared Memory](https://man7.org/linux/man-pages/man7/shm_overview.7.html)
- [mmap man page](https://man7.org/linux/man-pages/man2/mmap.2.html)
- [GCC Atomic Builtins](https://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html)
