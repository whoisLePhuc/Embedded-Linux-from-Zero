# Exercise 4: Pipe Redirection & Shell Simulation

## 🎯 Mục tiêu
- Hiểu cách hoạt động của lệnh Shell pipe: `ls | wc -l`.
- Sử dụng `dup2` để chuyển hướng Standard Input/Output (`STDIN_FILENO`, `STDOUT_FILENO`).
- Kết hợp `pipe`, `fork`, `execvp` và `dup2`.

## 📝 Yêu cầu

Viết chương trình `main.c` mô phỏng lệnh `ls -l | wc -l` bằng C.

### Luồng thực hiện:

1. Process Cha tạo một pipe.
2. Fork ra **Child 1** (Process thực hiện `ls -l`):
    - Đóng đầu Đọc của pipe (không dùng).
    - Dùng `dup2` để chuyển `STDOUT_FILENO` thành đầu Ghi của pipe. -> *Kết quả của ls sẽ chui vào pipe*.
    - Exec lệnh `ls -l` bằng `execlp` hoặc `execvp`.
3. Fork ra **Child 2** (Process thực hiện `wc -l`):
    - Đóng đầu Ghi của pipe (không dùng).
    - Dùng `dup2` để chuyển `STDIN_FILENO` thành đầu Đọc của pipe. -> *wc sẽ đọc từ pipe thay vì bàn phím*.
    - Exec lệnh `wc -l`.
4. Process Cha:
    - Đóng cả 2 đầu pipe (quan trọng! Nếu không đóng, child sẽ không nhận được EOF và treo).
    - Wait cho cả 2 con kết thúc.

## 🧪 Hướng dẫn kiểm tra

```bash
make
./pipe_redirection
```
Kết quả in ra phải giống hệt lệnh:
```bash
ls -l | wc -l
```

## 💡 Gợi ý

- `dup2(src, dest)`: Copy `src` fd đè lên `dest` fd.
- `execlp("ls", "ls", "-l", NULL)`
- Nhớ đóng pipe descriptors trong Parent sau khi fork xong!
