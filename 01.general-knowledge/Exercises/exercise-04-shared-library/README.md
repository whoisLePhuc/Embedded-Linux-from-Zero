# Bài tập 05: Shared Library (.so) ⭐⭐⭐

## 🎯 Mục tiêu

Tạo và sử dụng shared library (dynamic library `.so`) trong Linux. Hiểu cách biên dịch, link, và chạy chương trình với shared library.

## 📋 Yêu cầu

Tạo một shared library `libmath.so` chứa các hàm toán học, sau đó link với chương trình chính.

### Các file đã cho:

**`math_ops.c`** - Implement các hàm toán học:
```c
int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }
int divide(int a, int b) { return a / b; }  // TODO: handle divide by 0
int factorial(int n);    // Tính n!
int gcd(int a, int b);   // USCLN
```

**`math_ops.h`** - Header file với declarations

**`main.c`** - Chương trình chính sử dụng các hàm từ thư viện

### Yêu cầu Makefile:

1. Target `libmath.so`: Tạo shared library từ `math_ops.c`
   - Dùng flag `-fPIC` khi compile
   - Dùng flag `-shared` khi link

2. Target `all`: Build chương trình chính và link với shared library
   - Link với `-L. -lmath`
   - Set `RPATH` để tìm library khi chạy (`-Wl,-rpath,.`)

3. Target `test`: Chạy chương trình (kiểm tra LD_LIBRARY_PATH nếu cần)

4. Target `clean`: Xóa tất cả file build

5. Target `info`: Dùng `ldd` để kiểm tra dependencies của chương trình

## ✅ Tiêu chí đạt

- Shared library được tạo với đuôi `.so`
- Chương trình chạy được mà không cần set `LD_LIBRARY_PATH` thủ công (dùng RPATH)
- `ldd` hiển thị đúng đường dẫn đến library
- Phân biệt được static library (`.a`) vs shared library (`.so`)

## 💡 Gợi ý

```makefile
# Tạo shared library
libmath.so: math_ops.o
	$(CC) -shared -o $@ $^

# Compile với -fPIC (Position Independent Code)
math_ops.o: math_ops.c math_ops.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

# Link với shared library, set RPATH
main: main.o libmath.so
	$(CC) -o $@ main.o -L. -lmath -Wl,-rpath,.

# Kiểm tra dependencies
info: main
	@echo "=== Dependencies ==="
	ldd main
	@echo "=== Symbols ==="
	nm -D libmath.so
```

## 🔍 Kiểm tra

```bash
# Build
make clean all

# Kiểm tra dependencies
ldd main

# Chạy
LD_LIBRARY_PATH=. ./main
# hoặc (nếu đã dùng RPATH)
./main

# Xem symbols trong shared library
nm -D libmath.so
readelf -d main | grep RPATH
```

## 📁 File cần tạo

- [ ] `Makefile` - Build script

## 📁 File đã cho

- `math_ops.c` - Implementation của thư viện
- `math_ops.h` - Header file
- `main.c` - Chương trình chính

## 🎓 Kiến thức liên quan

Đọc lại trong `general-knowledge.md`:
- Section 3.2: Shared Library
- Section 4: Demo create shared library

---

**Bonus 1:** Tạo cả static library và shared library trong cùng Makefile, cho phép chọn loại khi build.

**Bonus 2:** Thêm versioning cho shared library: `libmath.so.1.0.0` với symlink `libmath.so -> libmath.so.1`.
