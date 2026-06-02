# Bài tập 06: Remote Debugging với GDB ⭐⭐⭐

## 🎯 Mục tiêu

Làm quen với remote debugging workflow: cross-compile ARM binary với debug symbols, sử dụng GDB để tìm và sửa lỗi.

## 📋 Yêu cầu

File `buggy.c` có 3 lỗi cố tình:
1. **Buffer overflow**: `init_buffer` ghi quá giới hạn mảng
2. **Stack smashing**: `process_data` dùng `strcpy` không kiểm tra độ dài
3. **Null pointer dereference**: `faulty_math` dereference con trỏ NULL

Thực hiện:
1. Biên dịch `buggy.c` cho ARM với `-g -O0`
2. Chạy với `qemu-arm-static` (sẽ crash)
3. Dùng `gdb-multiarch` để debug:
   - `break main`, `run`, `next`, `step`
   - `backtrace` để xem call stack khi crash
   - `print` để xem giá trị biến
   - `info locals` để xem biến địa phương
4. Sửa tất cả lỗi, tạo `fixed.c`

## ✅ Tiêu chí đạt

- Biên dịch thành công với `arm-linux-gnueabihf-gcc -g -O0`
- GDB hiển thị được backtrace chính xác khi crash
- Tìm được chính xác 3 lỗi
- `fixed.c` chạy hoàn chỉnh không crash với QEMU
- File `gdb_commands.txt` chứa các lệnh GDB đã dùng

## 💡 Gợi ý

- Biên dịch:
  ```bash
  arm-linux-gnueabihf-gcc -g -O0 -o buggy buggy.c
  ```

- Debug với GDB:
  ```bash
  # Cách 1: Dùng core dump
  qemu-arm-static -g 1234 ./buggy &
  gdb-multiarch ./buggy
  (gdb) target remote :1234
  (gdb) continue

  # Cách 2: Chạy trực tiếp với core dump
  qemu-arm-static ./buggy
  # Sẽ crash, lấy core dump nếu có
  ```

- Các lệnh GDB cần dùng:
  ```
  (gdb) break main
  (gdb) run
  (gdb) next
  (gdb) continue
  (gdb) backtrace
  (gdb) print variable_name
  (gdb) info locals
  (gdb) list
  (gdb) quit
  ```

- Cách sửa từng lỗi:
  1. `for (int i = 0; i <= size)` → `i < size`
  2. `strcpy` → `strncpy` + null-terminate
  3. Kiểm tra `ptr == NULL` trước khi dereference

## 🔍 Kiểm tra

```bash
# Build và chạy buggy (sẽ crash)
arm-linux-gnueabihf-gcc -g -O0 -o buggy buggy.c
qemu-arm-static ./buggy

# Debug
qemu-arm-static -g 1234 ./buggy &
gdb-multiarch -x gdb_commands.txt ./buggy

# Build và chạy fixed
arm-linux-gnueabihf-gcc -g -O0 -o fixed fixed.c
qemu-arm-static ./fixed
```

## 📁 File cần tạo

- [ ] `fixed.c` - Source code đã sửa lỗi
- [ ] `gdb_commands.txt` - Các lệnh GDB sử dụng

## 📁 File đã cho

- `buggy.c` - Source code có lỗi cố tình

## 🎓 Kiến thức liên quan

Đọc lại trong `cross-compilation-and-toolchain.md`:
- Section 6.1: gdbserver Setup
- Section 6.2: Cross-GDB Connection
- Section 5.4: Running ARM Binaries

---

**Bonus**: Thiết lập môi trường debug đầy đủ: compile, copy ARM binary lên board Raspberry Pi (hoặc container), dùng `gdbserver` trên target và `gdb-multiarch` trên host để debug từ xa qua network.
