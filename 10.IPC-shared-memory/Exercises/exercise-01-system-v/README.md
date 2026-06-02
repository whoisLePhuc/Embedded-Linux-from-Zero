# Exercise 1: System V Shared Memory

## 🎯 Mục tiêu
- Hiểu cách tạo và sử dụng Shared Memory theo chuẩn System V (`shmget`, `shmat`).
- Thực hiện chia sẻ dữ liệu giữa 2 process độc lập (Writer và Reader).

## 📝 Yêu cầu

Viết 2 chương trình: `writer.c` và `reader.c`.

### 1. Writer
- Tạo key bằng `ftok` (tạo file "shmfile" nếu chưa có).
- Tạo Shared Memory segment kích thước 1024 bytes (`shmget`).
- Attach segment vào không gian nhớ (`shmat`).
- Ghi một chuỗi nhập từ bàn phím vào vùng nhớ đó.
- Detach (`shmdt`).

### 2. Reader
- Tạo key bằng `ftok` (giống Writer).
- Lấy Shared Memory ID (`shmget`).
- Attach segment (`shmat`).
- Đọc nội dung từ vùng nhớ và in ra màn hình.
- Detach và Xóa segment (`shmctl` với `IPC_RMID`).

## 🧪 Hướng dẫn kiểm tra

1. Tạo file dummy: `touch shmfile`
2. Terminal 1 (Writer):
   ```bash
   make writer
   ./writer
   # Nhập text...
   ```
3. Terminal 2 (Reader):
   ```bash
   make reader
   ./reader
   # Sẽ thấy text vừa nhập
   ```

## 💡 Gợi ý

```c
key = ftok("shmfile", 65);
shmid = shmget(key, 1024, 0666 | IPC_CREAT);
char *str = (char*) shmat(shmid, (void*)0, 0);
// Write: gets(str);
// Read: printf("%s", str);
shmdt(str);
shmctl(shmid, IPC_RMID, NULL);
```
