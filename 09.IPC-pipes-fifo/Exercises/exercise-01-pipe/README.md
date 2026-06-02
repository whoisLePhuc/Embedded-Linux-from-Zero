# Exercise 1: Anonymous Pipe

## 🎯 Mục tiêu
- Hiểu cách sử dụng `pipe()` system call.
- Thực hiện giao tiếp giữa process Cha (Parent) và process Con (Child).
- Hiểu quy tắc: Đóng (close) các đầu pipe không sử dụng.

## 📝 Yêu cầu

Viết chương trình `main.c` thực hiện các bước sau:

1. Process Cha tạo một ống dẫn (pipe).
2. Process Cha gọi `fork()` để tạo Process Con.
3. Flow xử lý:
    - **Process Cha**: 
        - Đóng đầu Đọc (`pipefd[0]`).
        - Gửi tin nhắn "Hello Child, I am Parent!" vào đầu Ghi (`pipefd[1]`).
        - Đóng đầu Ghi và đợi con kết thúc.
    - **Process Con**:
        - Đóng đầu Ghi (`pipefd[1]`).
        - Đọc tin nhắn từ đầu Đọc (`pipefd[0]`) và in ra màn hình.
        - Đóng đầu Đọc và kết thúc.

## 🧪 Hướng dẫn kiểm tra

```bash
make
./pipe_example
```
Kết quả mong đợi:
```
Child received: Hello Child, I am Parent!
```

## 💡 Gợi ý

- `int pipe(int pipefd[2]);`
- `write(fd, buffer, size);`
- `read(fd, buffer, size);`
- Luôn kiểm tra giá trị trả về của `pipe` và `fork`.
