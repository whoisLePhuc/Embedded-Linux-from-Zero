# Bài tập 02: Memory Layout ⭐⭐

## 🎯 Mục tiêu

Hiểu về memory layout của process trong Linux: Text, Data, BSS, Heap, Stack.

## 📋 Yêu cầu

Viết chương trình `memory_layout.c` để:

1. Khai báo các loại biến khác nhau:
   - **Global initialized** (data segment)
   - **Global uninitialized** (BSS segment)
   - **Static initialized** (data segment)
   - **Static uninitialized** (BSS segment)
   - **Local variable** (stack)
   - **Dynamic allocation** (heap)

2. In địa chỉ của từng biến và tên function

3. Quan sát và verify thứ tự địa chỉ:
   - Stack có địa chỉ cao nhất
   - Heap thấp hơn stack
   - BSS và Data thấp hơn heap
   - Text (code) thấp nhất

## ✅ Tiêu chí đạt

- Chương trình chạy được: `./memory_layout`
- Hiển thị địa chỉ của từng loại biến với label rõ ràng
- Địa chỉ được sắp xếp theo thứ tự từ cao đến thấp
- Giải thích được tại sao thứ tự như vậy
- Không memory leak (free heap memory)

## 💡 Gợi ý

- Khai báo các biến:
  ```c
  // Global variables
  int global_init = 100;        // Data segment
  int global_uninit;            // BSS segment
  
  // Static variables
  static int static_init = 200; // Data segment
  static int static_uninit;     // BSS segment
  
  int main() {
      int local_var = 300;      // Stack
      int *heap_var = malloc(sizeof(int)); // Heap
      *heap_var = 400;
      
      // Print addresses...
      
      free(heap_var);
      return 0;
  }
  ```

- In địa chỉ với format `%p`:
  ```c
  printf("Address of local_var (stack): %p\n", (void*)&local_var);
  printf("Address of heap_var (heap):   %p\n", (void*)heap_var);
  printf("Address of main (text):       %p\n", (void*)main);
  ```

- Output mong đợi (địa chỉ sẽ khác nhau mỗi lần chạy):
  ```
  ==============================
       Memory Layout Demo
  ==============================
  
  TEXT Segment (Code):
    main():              0x55555555xxxx
  
  DATA Segment (Initialized):
    global_init:         0x555555558xxx
    static_init:         0x555555558xxx
  
  BSS Segment (Uninitialized):
    global_uninit:       0x55555555bxxx
    static_uninit:       0x55555555bxxx
  
  HEAP Segment (Dynamic):
    heap_var:            0x5555555xxxxx
  
  STACK Segment (Local):
    local_var:           0x7ffffffxxxxx
  
  ==============================
  Address Order: Stack > Heap > BSS > Data > Text
  ==============================
  ```

## 🔍 Kiểm tra

```bash
# Build
make

# Chạy chương trình
./memory_layout

# Xem memory map của process (bonus)
# Trong terminal khác, chạy:
cat /proc/$(pgrep memory_layout)/maps
```

## 📁 File cần tạo

- [ ] `memory_layout.c` - Source code chương trình
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-process.md`:
- Section 3: Memory Layout
- Section 3.2-3.6: Chi tiết từng segment

---

**Bonus Challenges**:

1. **Heap growth**: Allocate nhiều lần và quan sát địa chỉ tăng lên
2. **Stack growth**: Gọi recursive function và in địa chỉ local var
3. **ASLR**: Chạy nhiều lần và thấy địa chỉ thay đổi (Address Space Layout Randomization)

```bash
# Tắt ASLR để địa chỉ cố định (cần sudo)
sudo sysctl -w kernel.randomize_va_space=0
./memory_layout

# Bật lại ASLR
sudo sysctl -w kernel.randomize_va_space=2
```

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
