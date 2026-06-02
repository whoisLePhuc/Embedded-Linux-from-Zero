# Bài tập 05: Dining Philosophers Problem ⭐⭐

## 🎯 Mục tiêu

Giải bài toán kinh điển Dining Philosophers với N triết gia và N đũa (chopsticks) sử dụng POSIX unnamed semaphore, áp dụng kỹ thuật ngăn deadlock.

## 📋 Yêu cầu

Viết chương trình `main.c`:

### 1. Mô phỏng N triết gia (threads)
- 5 triết gia (threads), mỗi người có 1 thread riêng
- Mỗi triết gia luân phiên: suy nghĩ (think) -> ăn (eat) -> suy nghĩ
- Mỗi đũa được bảo vệ bằng 1 semaphore nhị phân

### 2. Đồng bộ hóa
- Mỗi triết gia cần 2 đũa để ăn (đũa trái và đũa phải)
- Dùng mảng semaphore cho N đũa (khởi tạo = 1)
- Áp dụng deadlock prevention: triết gia cuối cùng cầm đũa phải trước (thay đổi thứ tự)
- Hoặc: giới hạn số triết gia ăn cùng lúc (semaphore tài xỉu)

### 3. Hiển thị trạng thái
- In trạng thái từng triết gia: THINKING, HUNGRY, EATING
- Hiển thị khi có deadlock (nếu không áp dụng prevention)

## ✅ Tiêu chí đạt

- Deadlock prevention: không để xảy ra deadlock (thay đổi thứ tự cầm đũa)
- Mỗi triết gia ăn ít nhất 3 lần rồi kết thúc
- Sử dụng `sem_t` array cho chopsticks
- Dùng `sem_init()` cho unnamed semaphore
- Chương trình biên dịch với `gcc -Wall -Wextra -pthread`

## 💡 Gợi ý

- State enum và cách giải với thứ tự cầm đũa:
  ```c
  #define N 5
  #define EAT_TIMES 3

  enum { THINKING, HUNGRY, EATING } state[N];
  sem_t chopsticks[N];
  ```

- Deadlock prevention: triết gia cuối cầm đũa phải trước:
  ```c
  void pickup(int id) {
      if (id == N - 1) {
          // Last philosopher: pick right then left (reverse order)
          sem_wait(&chopsticks[(id + 1) % N]); // right
          sem_wait(&chopsticks[id]);            // left
      } else {
          // Others: pick left then right
          sem_wait(&chopsticks[id]);            // left
          sem_wait(&chopsticks[(id + 1) % N]); // right
      }
  }

  void putdown(int id) {
      sem_post(&chopsticks[id]);            // left
      sem_post(&chopsticks[(id + 1) % N]); // right
  }
  ```

- Triết gia lifecycle:
  ```c
  void *philosopher(void *arg) {
      int id = *(int *)arg;
      for (int i = 0; i < EAT_TIMES; i++) {
          think(id);
          pickup(id);
          eat(id);
          putdown(id);
      }
      return NULL;
  }
  ```

## 🔍 Kiểm tra

```bash
# Build
make

# Chạy
./dining_philosophers
# Quan sát: không có deadlock, tất cả triết gia đều ăn xong
```

## 📁 File cần tạo

- [ ] `main.c` - Source code mô phỏng dining philosophers
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `10.IPC-semaphore.md`:
- Section 1.1: Unnamed POSIX Semaphores
- Section 3.3: Dining Philosophers Problem
- Section 4.1: Deadlock and Prevention

---

**Bonus**: Implement giải pháp dùng monitor (mutex + condition variable) thay vì semaphore, hoặc thêm arbitrator (semaphore giới hạn số người ăn cùng lúc).

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
