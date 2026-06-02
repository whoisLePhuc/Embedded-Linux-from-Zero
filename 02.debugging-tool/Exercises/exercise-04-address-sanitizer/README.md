# Bài tập 04: Address Sanitizer — Phát hiện lỗi bộ nhớ ⭐⭐⭐

## 🎯 Mục tiêu

Sử dụng AddressSanitizer (ASan) để phát hiện và sửa các lỗi bộ nhớ phổ biến: heap buffer overflow, use-after-free (UAF), và memory leak.

## 📋 Yêu cầu

Chương trình `memory_ops.c` thực hiện các thao tác với bộ nhớ động. Chương trình **có 3 lỗi** mà ASan có thể phát hiện:

1. **Heap buffer overflow** — ghi dữ liệu vượt quá vùng nhớ được cấp phát
2. **Use-after-free (UAF)** — truy cập vùng nhớ đã được giải phóng
3. **Memory leak** — không giải phóng bộ nhớ đã cấp phát

Sử dụng ASan để phát hiện và phân tích các lỗi:
- Compile với `-fsanitize=address -g -O1`
- Chạy chương trình và đọc ASan output
- Xác định loại lỗi, vị trí, và root cause
- Sửa tất cả các lỗi

## ✅ Tiêu chí đạt

- ASan phát hiện được cả 3 lỗi khi chạy chương trình gốc
- Đọc được ASan output: xác định loại lỗi, địa chỉ, stack trace
- Phân biệt được heap-buffer-overflow, use-after-free, và memory leak
- Sửa hết lỗi, chương trình chạy sạch với ASan (không output báo lỗi)
- Output đúng: chuỗi "Hello" được in ra, message được xử lý đúng

## 💡 Gợi ý

- Compile với ASan:
  ```bash
  gcc -fsanitize=address -g -O1 -o memory_ops memory_ops.c
  ```

- ASan output cho heap-buffer-overflow:
  ```
  ==PID==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x...
  WRITE of size 4 at 0x... thread T0
      #0 main memory_ops.c:NN
  ```

- ASan output cho use-after-free:
  ```
  ==PID==ERROR: AddressSanitizer: heap-use-after-free on address 0x...
  READ of size 1 at 0x... thread T0
      #0 main memory_ops.c:NN
  ```

- ASan output cho memory leak (kèm LSan - LeakSanitizer):
  ```
  ==PID==ERROR: LeakSanitizer: detected memory leaks
  Direct leak of NN byte(s) in 1 object(s) allocated from:
      #0 malloc
      #1 main memory_ops.c:NN
  ```

- Các flag hữu ích:
  - `ASAN_OPTIONS=detect_leaks=1` — bật leak detection
  - `ASAN_OPTIONS=abort_on_error=1` — dừng ngay khi phát hiện lỗi
  - Thêm `-fsanitize=undefined` để phát hiện undefined behavior

## 🔍 Kiểm tra

```bash
# Build không ASan (chạy vẫn ra output nhưng có lỗi ngầm)
make

# Build với ASan
make asan

# Chạy — sẽ thấy ASan báo lỗi
./memory_ops

# Output ASan điển hình:
# ==PID==ERROR: AddressSanitizer: heap-buffer-overflow
# ...
# ==PID==ERROR: AddressSanitizer: heap-use-after-free
# ... (dòng cuối)
# ==PID==ERROR: LeakSanitizer: detected memory leaks

# Sau khi sửa, chạy với ASan — không có output lỗi
make asan && ./memory_ops
# Output:
# Message: Hello
# Processed: HELLO
```

## 📁 File cần tạo

- [ ] `memory_ops.c` — Source code chương trình (có bug)
- [ ] `Makefile` — Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `debugging-tool.md`:
- Section 4: AddressSanitizer (ASan)
- Section 4.5: Đọc hiểu ASan output
- Section 4.6: Các loại lỗi ASan phát hiện
- Section 7.1-7.3: Buffer Overflow, Use-After-Free, Memory Leak

---

**Bonus:** Thêm flag `-fsanitize=undefined` để phát hiện thêm undefined behavior. Dùng `ASAN_OPTIONS=detect_stack_use_after_return=1` để phát hiện use-after-return trên stack.

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
