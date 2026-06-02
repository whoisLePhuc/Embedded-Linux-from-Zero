# BÀI TẬP VỀ IPC MESSAGE QUEUES

Bộ bài tập này bao gồm các ví dụ về 2 chuẩn Message Queue trong Linux: System V và POSIX.

## 📚 Cách sử dụng

Mỗi bài tập được đặt trong một thư mục riêng:
- `exercise-01-system-v/` - Message Queue chuẩn System V cũ (dùng `ftok`, `msgget`).
- `exercise-02-posix/` - Message Queue chuẩn POSIX mới (dùng `mq_open`, thư viện `-lrt`).

## 🎯 Hướng dẫn

1. Đọc file `README.md` trong mỗi thư mục bài tập.
2. Code 2 chương trình: **Sender** (Gửi) và **Receiver** (Nhận).
3. Mở 2 terminal để chạy song song.

## 📝 Mức độ

- ⭐ Trung bình: Cả 2 bài tập đều yêu cầu hiểu về struct message và cách queue hoạt động.

## 💡 Tips

- System V: Dùng lệnh `ipcs -q` để xem các queue đang active, `ipcrm -q <id>` để xóa thủ công.
- POSIX: Queue được mount trong `/dev/mqueue` (trên một số hệ thống).
- Luôn define struct message đúng cách (`long type` ở đầu).
- POSIX cần link với `-lrt` (`gcc ... -lrt`).
