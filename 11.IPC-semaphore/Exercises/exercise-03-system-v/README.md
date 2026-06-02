# Exercise 3: System V Semaphore (Process Sync)

## 🎯 Mục tiêu
- Thực hiện đồng bộ hóa Process sử dụng API System V Semaphore cũ hơn (`semget`, `semop`).
- Hiểu cấu trúc `sembuf` và cách thao tác P/V.

## 📝 Yêu cầu

Viết chương trình `main.c` sử dụng `fork` tạo ra 2 process con (Child A và Child B).

1. **Process Cha**:
   - Tạo Semaphore set gồm 1 semaphore.
   - Khởi tạo giá trị = 1 (Binary Semaphore).
   - Fork ra 2 con.
   - Chờ 2 con kết thúc.
   - Xóa Semaphore.

2. **Process Con A & B**:
   - Cùng thực hiện một công việc: In 5 lần dòng "AAAAA" (hoặc "BBBBB").
   - Trước khi in: Gọi P opertion (Decrement) để lock.
   - Sau khi in: Gọi V operation (Increment) để unlock.

## 🧪 Hướng dẫn kiểm tra

```bash
make
./sys_v_sem
```

## 💡 Gợi ý

```c
key = ftok("semfile", 65);
semid = semget(key, 1, 0666 | IPC_CREAT);

// Init
union semun arg;
arg.val = 1;
semctl(semid, 0, SETVAL, arg);

// P Operation
struct sembuf p = {0, -1, SEM_UNDO};
semop(semid, &p, 1);

// V Operation
struct sembuf v = {0, 1, SEM_UNDO};
semop(semid, &v, 1);
```
