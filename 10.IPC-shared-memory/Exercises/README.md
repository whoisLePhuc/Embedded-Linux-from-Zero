# BÀI TẬP VỀ IPC SHARED MEMORY

Bộ bài tập gồm 3 bài, bao quát các phương pháp Shared Memory phổ biến.

## 📚 Danh sách bài tập

1.  **exercise-01-system-v**: Dùng `shmget`, `shmat` (Key-based) để chia sẻ dữ liệu giữa Writer và Reader.
2.  **exercise-02-posix**: Dùng `shm_open`, `mmap` (File-descriptor based) - Chuẩn hiện đại hơn.
3.  **exercise-03-anonymous**: Dùng `mmap` với `MAP_ANONYMOUS` để chia sẻ bộ nhớ giữa process Cha và Con mà không cần tạo file.

## 🎯 Hướng dẫn

- Đi vào từng thư mục bài tập và đọc `README.md`.
- Shared Memory rất nhanh nhưng **nguy hiểm** nếu không đồng bộ (synchronization). Trong các bài tập này, chúng ta sẽ giả lập đồng bộ đơn giản bằng cách cho Writer chạy trước, Reader chạy sau, hoặc dùng `sleep()`. Trong thực tế, bạn cần dùng **Semaphore**.

Chúc bạn học tốt! 🚀
