# Exercise 1: POSIX Unnamed Semaphore (Thread Sync)

## 🎯 Mục tiêu
- Hiểu vấn đề **Race Condition** khi nhiều thread cùng cộng một biến.
- Sử dụng **Semaphore** (`sem_init`, `sem_wait`, `sem_post`) để fix lỗi này.

## 📝 Yêu cầu

Viết chương trình `main.c` thực hiện:

1. Khai báo biến toàn cục `int counter = 0;`.
2. Tạo 2 Thread (Thread 1 và Thread 2).
3. Mỗi thread chạy vòng lặp 1.000.000 lần (1 triệu), mỗi lần thực hiện `counter++`.
4. Main thread chờ 2 thread kết thúc (`pthread_join`).
5. In ra kết quả `counter`.

### Kịch bản kiểm tra:

1. **Không dùng Semaphore**: 
   Chạy chương trình -> Kết quả sẽ < 2.000.000 (sai lệch do Race Condition).
   Ví dụ: 1543820, 1999201...

2. **Dùng Semaphore**:
   Sử dụng `sem_wait` trước khi cộng và `sem_post` sau khi cộng.
   Kết quả luôn là 2.000.000.

## 🧪 Hướng dẫn kiểm tra

```bash
make
./thread_race
```

## 💡 Gợi ý

```c
sem_t sem;
sem_init(&sem, 0, 1); // Value = 1 (Binary Semaphore ~ Mutex)

void *thread_func(void *arg) {
    for (int i = 0; i < 1000000; i++) {
        sem_wait(&sem);
        counter++;
        sem_post(&sem);
    }
    return NULL;
}
```
