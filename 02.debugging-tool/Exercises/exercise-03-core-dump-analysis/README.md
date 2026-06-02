# Bài tập 03: Core Dump Analysis ⭐⭐⭐

## 🎯 Mục tiêu

Làm quen với quy trình phân tích core dump: kích hoạt core dump, chạy chương trình gây crash, và dùng GDB để phân tích nguyên nhân từ core dump.

## 📋 Yêu cầu

Chương trình `crash_me.c` xử lý cấu trúc dữ liệu dạng cây (binary tree). Chương trình bị **segmentation fault** do:
1. **Stack overflow** — đệ quy vô hạn (thiếu base case)
2. **Dangling pointer** — dùng con trỏ đã được free

Yêu cầu cụ thể:

1. Compile chương trình với debug symbols (`-g`)
2. Kích hoạt core dump: `ulimit -c unlimited`
3. Chạy chương trình để tạo core dump
4. Dùng GDB phân tích core dump
5. Xác định chính xác dòng code gây crash

Sử dụng các lệnh GDB cho core dump:
- `gdb -c core <program>` — mở core dump
- `bt` / `backtrace` — xem call stack tại thời điểm crash
- `frame N` — chuyển đến frame cụ thể
- `info locals` — xem biến local tại frame
- `print <var>` — in giá trị biến
- `list` — xem source code tại vị trí crash

## ✅ Tiêu chí đạt

- Tạo được core dump thành công
- Dùng GDB mở core dump và xác định đúng vị trí crash
- Backtrace cho thấy chuỗi function call dẫn đến crash
- Xác định được root cause: stack overflow do đệ quy vô hạn
- Sửa được lỗi, chương trình chạy đúng

## 💡 Gợi ý

- Kiểm tra core dump đã được bật chưa:
  ```bash
  ulimit -c            # 0 = chưa bật
  ulimit -c unlimited  # bật core dump
  ```

- Core dump thường được tạo tại thư mục hiện tại với tên `core` hoặc `core.<pid>`:
  ```bash
  ls -la core*
  ```

- Mở core dump với GDB:
  ```bash
  gdb ./crash_me core
  ```

- Khi mở core dump, GDB tự động hiển thị dòng crash. Gõ:
  ```gdb
  (gdb) bt
  (gdb) frame 3
  (gdb) info locals
  (gdb) list
  ```

- Nếu không thấy core dump, kiểm tra:
  - Kernel có giới hạn không: `cat /proc/sys/kernel/core_pattern`
  - Dung lượng ổ cứng còn trống
  - File core có bị xóa không

## 🔍 Kiểm tra

```bash
# Bật core dump
ulimit -c unlimited

# Build
make

# Chạy — sẽ crash và tạo core dump
./crash_me
# Output: Segmentation fault (core dumped)

# Phân tích core dump
gdb ./crash_me core

(gdb) bt
(gdb) frame <N>
(gdb) info locals
(gdb) list

# Sau khi sửa lỗi
make
./crash_me
# Output:
# Tree height: 5
# In-order: 10 20 30 40 50
```

## 📁 File cần tạo

- [ ] `crash_me.c` — Source code chương trình (có bug)
- [ ] `Makefile` — Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `debugging-tool.md`:
- Section 2.7: Core Dump Analysis
- Section 2.5: Backtrace và Frame Inspection

---

**Bonus:** Thử đặt `ulimit -c 0` (tắt core dump) và xem thông báo lỗi khác nhau thế nào. Dùng `echo /tmp/core.%p > /proc/sys/kernel/core_pattern` để thay đổi nơi lưu core dump.

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
