# Exercise 2: POSIX Named Semaphore (Process Sync)

## 🎯 Mục tiêu
- Đồng bộ hóa 2 process hoàn toàn độc lập (không cha-con) sử dụng **Named Semaphore**.
- Hiểu cách tạo semaphore toàn cục trên hệ thống (`/mysem`).

## 📝 Yêu cầu

Viết 2 chương trình: `processA.c` và `processB.c`.

1. **Process A**:
   - Mở/Tạo Named Semaphore `/test_sem` với giá trị 1.
   - Lặp 5 lần:
     - `sem_wait`: Vào Critical Section.
     - In ra màn hình: `AAAAA` (sleep 1 giây để giả lập công việc lâu).
     - `sem_post`: Ra khỏi Critical Section.

2. **Process B**:
   - Mở Named Semaphore `/test_sem`.
   - Lặp 5 lần:
     - `sem_wait`.
     - In ra màn hình: `BBBBB` (sleep 1 giây).
     - `sem_post`.

### Kiểm tra:
- Mở 2 terminal. Chạy `processA` và `processB` gần như cùng lúc.
- **Kết quả đúng**: Output không bị xen kẽ từng ký tự.
  ```
  AAAAA
  BBBBB
  AAAAA
  ...
  ```
- **Kết quả sai (nếu không có sem)**:
  ```
  AABBBBAA...
  ```

## 🧪 Hướng dẫn kiểm tra

```bash
# Terminal 1
make processA
./processA

# Terminal 2
make processB
./processB
```

## 💡 Gợi ý

```c
sem_t *sem = sem_open("/test_sem", O_CREAT, 0666, 1);
// sem_wait(sem);
// ...
// sem_close(sem);
// sem_unlink("/test_sem"); // Chỉ 1 process gọi unlink
```
