# Bài tập 03: Fork và Exec ⭐⭐

## 🎯 Mục tiêu

Thực hành tạo process con với `fork()` và thực thi chương trình mới với `exec()`.

## 📋 Yêu cầu

Viết chương trình `simple_shell.c` - một shell đơn giản:

1. Hiển thị prompt (ví dụ: `myshell> `)
2. Đọc command từ user
3. Sử dụng `fork()` để tạo child process
4. Child sử dụng `execvp()` để thực thi command
5. Parent sử dụng `wait()` để đợi child hoàn thành
6. Hỗ trợ lệnh `exit` để thoát shell

## ✅ Tiêu chí đạt

- Chương trình chạy được: `./simple_shell`
- Có thể chạy các lệnh cơ bản: `ls`, `pwd`, `echo hello`, `date`
- Parent đợi child hoàn thành trước khi hiện prompt mới
- Lệnh `exit` thoát shell
- Error handling khi command không tồn tại
- Không tạo zombie process

## 💡 Gợi ý

- Cấu trúc cơ bản:
  ```c
  while (1) {
      // 1. Hiển thị prompt
      printf("myshell> ");
      
      // 2. Đọc command
      fgets(line, sizeof(line), stdin);
      
      // 3. Kiểm tra exit
      if (strcmp(line, "exit\n") == 0) break;
      
      // 4. Parse command thành argv[]
      
      // 5. Fork
      pid_t pid = fork();
      
      if (pid == 0) {
          // Child: exec
          execvp(argv[0], argv);
          perror("exec failed");
          _exit(1);
      } else {
          // Parent: wait
          wait(NULL);
      }
  }
  ```

- Parse command với `strtok()`:
  ```c
  char *args[MAX_ARGS];
  int argc = 0;
  char *token = strtok(line, " \n");
  while (token != NULL) {
      args[argc++] = token;
      token = strtok(NULL, " \n");
  }
  args[argc] = NULL;  // argv must end with NULL
  ```

- Child phải dùng `_exit()` thay vì `exit()` khi exec thất bại

## 🔍 Kiểm tra

```bash
# Build
make

# Chạy shell
./simple_shell

# Test các lệnh
myshell> ls -la
myshell> pwd
myshell> echo Hello World
myshell> date
myshell> whoami

# Test lệnh không tồn tại
myshell> abc123
exec failed: No such file or directory

# Thoát
myshell> exit
```

## 📁 File cần tạo

- [ ] `simple_shell.c` - Source code chương trình
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-process.md`:
- Section 4.1: System call fork()
- Section 4.2: Exec Family
- Section 5.2: System call wait()

---

**Bonus Challenges**:

1. **Command history**: Lưu và hiển thị lịch sử commands
2. **Pipe support**: Hỗ trợ `ls | grep txt`
3. **Background process**: Hỗ trợ `command &` chạy background
4. **Built-in commands**: Thêm `cd` command

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
