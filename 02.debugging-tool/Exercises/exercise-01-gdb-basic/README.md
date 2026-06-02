# Bài tập 01: GDB Basics — Debugging Crashes ⭐⭐

## 🎯 Mục tiêu

Làm quen với GDB để debug các lỗi runtime cơ bản trong C: NULL pointer dereference, buffer overflow, off-by-one.

## 📋 Yêu cầu

Chương trình `crash_debug.c` quản lý danh sách sinh viên. Chương trình compile được nhưng **có 3 lỗi runtime**. Sử dụng GDB để tìm và sửa:

1. **NULL pointer dereference** — chương trình crash khi in danh sách sinh viên
2. **Buffer overflow** — ghi dữ liệu vượt quá kích thước mảng
3. **Off-by-one** — vòng lặp chạy sai số lần

Sử dụng các lệnh GDB:
- `break` / `b` — đặt breakpoint
- `run` / `r` — chạy chương trình
- `backtrace` / `bt` — xem call stack khi crash
- `print` / `p` — xem giá trị biến
- `frame` / `f` — chuyển đổi stack frame
- `list` / `l` — xem source code
- `step` / `s` — chạy từng bước
- `next` / `n` — chạy qua function call
- `quit` / `q` — thoát GDB

## ✅ Tiêu chí đạt

- Chương trình chạy không crash sau khi sửa
- Có khả năng chạy GDB, đặt breakpoint tại các vị trí nghi ngờ
- Biết dùng `bt` để xác định vị trí crash
- Biết dùng `p` và `frame` để kiểm tra trạng thái chương trình
- Output đúng: in ra danh sách 3 sinh viên với điểm số

## 💡 Gợi ý

- Compile với flag `-g` để có debug symbols:
  ```bash
  gcc -g -o crash_debug crash_debug.c
  ```
- Chạy với GDB:
  ```bash
  gdb ./crash_debug
  ```
- Khi crash, GDB tự dừng lại — gõ `bt` để xem call stack
- Sử dụng `frame N` để di chuyển tới frame N, sau đó `list` và `print` để kiểm tra
- Đặt breakpoint sớm: `b main`, `b function_name`
- Step qua code: `s` (step into), `n` (step over)

## 🔍 Kiểm tra

```bash
# Build (sẽ có warning — đó là gợi ý!)
make

# Chạy — sẽ crash
./crash_debug

# Debug với GDB
gdb ./crash_debug
(gdb) run
(gdb) bt
# ...phân tích và sửa lỗi...

# Sau khi sửa xong
make
./crash_debug
# Output:
# Student 1: Nguyen Van A - 8.5
# Student 2: Tran Thi B - 7.2
# Student 3: Le Van C - 9.0
```

## 📁 File cần tạo

- [ ] `crash_debug.c` — Source code chương trình (có bug)
- [ ] `Makefile` — Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `debugging-tool.md`:
- Section 2.2: Compile cho debug (`-g` flag)
- Section 2.3: Basic GDB Commands
- Section 2.5: Backtrace và Frame Inspection

---

**Bonus:** Thử dùng lệnh `info locals` trong GDB để xem tất cả biến local cùng lúc. Dùng `display` để tự động in giá trị mỗi khi dừng.

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
