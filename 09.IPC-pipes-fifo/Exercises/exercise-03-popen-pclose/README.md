# Bài tập 05: popen/pclose Shell Pipe ⭐⭐

## 🎯 Mục tiêu

Hiểu cách sử dụng `popen()` và `pclose()` để thực thi lệnh shell và giao tiếp với tiến trình con qua pipe.

## 📋 Yêu cầu

Viết chương trình `main.c` thực hiện các chức năng sau:

### 1. Đọc output từ lệnh shell
- Dùng `popen()` với mode `"r"` để đọc output của lệnh shell
- Ví dụ: đọc kết quả `ls -la`, `ps aux`, hoặc `df -h`
- In từng dòng output ra màn hình

### 2. Ghi input vào lệnh shell  
- Dùng `popen()` với mode `"w"` để ghi dữ liệu vào stdin của lệnh shell
- Ví dụ: pipe dữ liệu vào `sort`, `grep`, hoặc `bc`
- Đóng pipe và kiểm tra kết quả

### 3. Cho phép người dùng nhập lệnh từ bàn phím
- Nhập lệnh cần thực thi
- Chọn chế độ đọc hoặc ghi

## ✅ Tiêu chí đạt

- Sử dụng `popen()` thay vì tự tạo pipe + fork + exec
- Gọi `pclose()` để đóng pipe và kiểm tra exit status
- Xử lý lỗi khi `popen()` hoặc `pclose()` thất bại
- Không gọi `waitpid()` thủ công (pclose tự làm)
- Chương trình biên dịch với `gcc -Wall -Wextra`

## 💡 Gợi ý

- Đọc output từ lệnh:
  ```c
  FILE *fp = popen("ls -la", "r");
  if (fp == NULL) {
      perror("popen");
      exit(1);
  }

  char line[256];
  while (fgets(line, sizeof(line), fp) != NULL) {
      printf("%s", line);
  }
  ```

- Ghi input vào lệnh:
  ```c
  FILE *fp = popen("sort -n", "w");
  if (fp == NULL) {
      perror("popen");
      exit(1);
  }

  fprintf(fp, "3\n1\n2\n");
  ```

- Kiểm tra exit status với `pclose()`:
  ```c
  int status = pclose(fp);
  if (status == -1) {
      perror("pclose");
  } else if (WIFEXITED(status)) {
      printf("Exit code: %d\n", WEXITSTATUS(status));
  }
  ```

## 🔍 Kiểm tra

```bash
# Build
make

# Chạy chương trình
./popen_demo

# Test nhập lệnh thủ công
# Nhập: ls -la
# Nhập: ps aux | grep bash
# Nhập: echo "5+3" | bc
```

## 📁 File cần tạo

- [ ] `main.c` - Source code chính
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `08.IPC-pipes-fifo.md`:
- Section 1.4: popen() and pclose()
- Section 1.5: Differences between popen and raw pipe

---

**Bonus**: Tạo shell đơn giản cho phép người dùng nhập lệnh liên tục cho đến khi gõ "exit". Hỗ trợ cả pipes (vd: `ls | wc -l`) vì popen dùng shell.

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
