# Bài tập 04: File Locking và Concurrent Access ⭐⭐⭐

## 🎯 Mục tiêu

Tạ chương trình minh họa file locking để xử lý concurrent access, tránh race conditions khi nhiều process cùng truy cập file.

## 📋 Yêu cầu

Viết **2 chương trình** để demonstate file locking:

### Program 1: `account_manager.c` (sử dụng `flock()`)

Mô phỏng hệ thống quản lý số dư tài khoản:

1. Hiển thị menu để người dùng chọn operation
2. Đọc số dư từ file `account.txt`
3. Thực hiện giao dịch (deposit/withdraw/check balance)
4. Cập nhật số dư vào file
5. Sử dụng `flock()` để đảm bảo chỉ một process giao dịch tại một thời điểm

**Menu example:**
```
=== Account Manager ===
1. Check Balance
2. Deposit
3. Withdraw
4. Exit
Choice: _
```

### Program 2: `range_locker.c` (sử dụng `fcntl()`)

Demonstrate byte-range locking:

1. Tạo file với nhiều records (mỗi record 100 bytes)
2. Hiển thị menu để chọn record và operation
3. Lock từng record riêng biệt
4. Cho phép nhiều process update các records khác nhau đồng thời
5. Sử dụng `fcntl()` với `struct flock`

**Menu example:**
```
=== Range Locker ===
1. Show All Records
2. Lock and Update Record
3. Test Lock Status
4. Initialize File
5. Exit
Choice: _
```

## ✅ Tiêu chí đạt

**account_manager.c:**
- Sử dụng `flock()` để lock exclusive toàn bộ file
- Hiển thị menu cho người dùng chọn operation
- Xử lý đúng deposit/withdraw/balance
- Error handling đầy đủ
- Test được với nhiều process chạy đồng thời không bị race condition

**range_locker.c:**
- Sử dụng `fcntl()` với `F_SETLKW` hoặc `F_SETLK`
- Hiển thị menu để chọn operation
- Lock được từng phần của file (byte range)
- Hiển thị rõ record nào đang bị lock
- Nhiều process có thể lock các records khác nhau đồng thời

## 💡 Gợi ý

### flock() example:

```c
#include <sys/file.h>

int fd = open("account.txt", O_RDWR);

// Acquire exclusive lock
if (flock(fd, LOCK_EX) == -1) {
    perror("flock");
    return 1;
}

// Critical section - update account
// ...

// Release lock
flock(fd, LOCK_UN);
close(fd);
```

### fcntl() example:

```c
#include <fcntl.h>

struct flock lock;
lock.l_type = F_WRLCK;      // Exclusive lock
lock.l_whence = SEEK_SET;   // From beginning
lock.l_start = record_num * RECORD_SIZE;  // Offset
lock.l_len = RECORD_SIZE;   // Lock length

// Acquire lock (blocking)
if (fcntl(fd, F_SETLKW, &lock) == -1) {
    perror("fcntl");
    return 1;
}

// Critical section
// ...

// Release lock
lock.l_type = F_UNLCK;
fcntl(fd, F_SETLK, &lock);
```

## 🔍 Kiểm tra

### Test account_manager:

```bash
# Terminal 1
./account_manager
# Chọn option 2 (Deposit), nhập 100

# Terminal 2 (chạy đồng thời)
./account_manager
# Chọn option 2 (Deposit), nhập 50
# Sẽ phải chờ Terminal 1 hoàn thành

# Check balance
./account_manager
# Chọn option 1 (Check Balance)
# Phải ra đúng tổng!
```

### Test range_locker:

```bash
# Terminal 1
./range_locker
# Chọn option 4 (Initialize File)
# Chọn option 2 (Lock and Update), nhập record 0

# Terminal 2
./range_locker
# Chọn option 2 (Lock and Update), nhập record 1
# OK - khác record, không bị block

# Terminal 3
./range_locker
# Chọn option 2 (Lock and Update), nhập record 0
# BLOCK - cùng record với Terminal 1
```

## 📁 File cần tạo

- [ ] `account_manager.c` - Program với flock()
- [ ] `range_locker.c` - Program với fcntl()
- [ ] `Makefile` - Build cả 2 programs

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-file-system.md`:
- Section 4: File Locking
- Section 4.2: `flock()`
- Section 4.3: `fcntl()`

## 📝 Bonus Challenges

1. **Non-blocking lock**: Sử dụng `LOCK_NB` với flock() để không block
2. **Deadlock detection**: Detect và handle potential deadlocks
3. **Shared locks**: Implement reader-writer pattern (shared read locks, exclusive write locks)
4. **Timeout**: Implement timeout cho lock acquisition

## ⚠️ Lưu ý

- **Advisory Locking**: Linux uses advisory locking - processes phải cooperate
- **Process termination**: Locks tự động release khi process exits
- **fork()**: Child process không inherit locks từ flock() nhưng inherit từ fcntl()
- **close()**: Đóng FD sẽ release tất cả locks của FD đó

---

Đây là bài tập khó nhất! Hãy test kỹ với concurrent access.

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
