# Bài tập 02: GDB Advanced — Breakpoints & Memory ⭐⭐

## 🎯 Mục tiêu

Sử dụng các tính năng nâng cao của GDB: conditional breakpoints, watchpoints, memory inspection, và disassembly để tìm lỗi bộ nhớ khó phát hiện.

## 📋 Yêu cầu

Chương trình `linked_list.c` cài đặt một linked list đơn giản. Chương trình compile được nhưng **có 3 lỗi runtime** tinh vi:

1. **Memory corruption** — ghi dữ liệu sai vị trí làm hỏng cấu trúc linked list
2. **Logic error** — nút mới được chèn sai thứ tự, danh sách không đúng
3. **Uninitialized memory access** — đọc con trỏ chưa được khởi tạo

Sử dụng các lệnh GDB nâng cao:
- `break ... if ...` — conditional breakpoint
- `watch` / `rwatch` / `awatch` — watchpoint theo dõi biến
- `x/Nx <address>` — examine memory ở dạng hex
- `x/Ns <address>` — examine memory ở dạng string
- `disassemble` / `disas` — xem assembly code
- `info registers` — xem thanh ghi CPU
- `info args` — xem tham số function

## ✅ Tiêu chí đạt

- Sửa được cả 3 lỗi, chương trình chạy đúng
- Biết đặt conditional breakpoint: `b insert_node if value == 42`
- Biết dùng `watch` để theo dõi khi biến thay đổi
- Biết dùng `x/` để kiểm tra nội dung bộ nhớ
- Output đúng: danh sách được in theo thứ tự tăng dần

## 💡 Gợi ý

- Conditional breakpoint chỉ dừng khi điều kiện đúng:
  ```gdb
  (gdb) b 42 if current->next == NULL
  ```

- Watchpoint theo dõi khi giá trị thay đổi:
  ```gdb
  (gdb) watch head->next
  (gdb) rwatch head->next   # theo dõi read
  (gdb) awatch head->next   # theo dõi cả read và write
  ```

- Examine memory:
  ```gdb
  (gdb) x/4x 0x7fffffffe000   # 4 words dạng hex
  (gdb) x/8xb 0x7fffffffe000  # 8 bytes dạng hex
  (gdb) x/s 0x7fffffffe000    # string
  (gdb) x/4gx 0x7fffffffe000  # 4 giant words (8-byte)
  ```

- Dùng `disas` để xem assembly tại vị trí crash:
  ```gdb
  (gdb) disas
  ```

## 🔍 Kiểm tra

```bash
# Build
make

# Chạy thử
./linked_list
# Output sai hoặc crash

# Debug
gdb ./linked_list
(gdb) b main
(gdb) r
(gdb) ...

# Output đúng sau khi sửa:
# List: 10 -> 20 -> 30 -> 42 -> 50 -> NULL
# Search 30: found
# Search 99: not found
```

## 📁 File cần tạo

- [ ] `linked_list.c` — Source code chương trình (có bug)
- [ ] `Makefile` — Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `debugging-tool.md`:
- Section 2.4: Breakpoints và Watchpoints
- Section 2.6: Memory Inspection
- Section 2.8: Conditional Breakpoints

---

**Bonus:** Thử dùng GDB TUI mode (gdb -tui) để xem source code và assembly cùng lúc. Dùng `layout split` để chia màn hình.

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
