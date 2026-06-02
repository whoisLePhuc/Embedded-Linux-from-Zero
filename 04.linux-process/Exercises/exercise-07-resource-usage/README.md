# Bài tập 07: Process Resource Usage ⭐⭐

## 🎯 Mục tiêu

Hiểu và đo lường tài nguyên sử dụng bởi process: CPU time (user/system), context switches, page faults, và memory usage.

## 📋 Yêu cầu

Viết chương trình `resource_usage.c` để:

1. **Đo resource usage** của process hiện tại (`RUSAGE_SELF`) trước và sau khi chạy một workload

2. **Tạo workload** có tính toán:
   - Tính toán số lớn (ví dụ: tính số nguyên tố, factorial, sort array lớn)
   - Sử dụng nhiều vòng lặp để tạo CPU-bound work
   - Sử dụng `malloc`/`free` để tạo page faults

3. **Hiển thị các thông số**:
   - **User CPU time** (giây + microseconds)
   - **System CPU time** (giây + microseconds)
   - **Voluntary context switches** (số lần process tự nguyện nhường CPU)
   - **Involuntary context switches** (số lần bị kernel thu hồi CPU)
   - **Page faults** (minor: đã có sẵn trong RAM, major: phải đọc từ disk)
   - **Peak memory usage** (maximum resident set size)

4. **Đo resource của child process** (`RUSAGE_CHILDREN`) sau khi child hoàn thành

## ✅ Tiêu chí đạt

- Chương trình chạy được: `./resource_usage`
- Hiển thị rõ ràng user CPU time và system CPU time trước/sau
- Workload đủ nặng để thấy sự khác biệt > 0.1s user time
- Hiển thị voluntary và involuntary context switches
- Hiển thí minor và major page faults
- Có phần đo resource của child process (fork + wait)
- Các chỉ số sau workload phải lớn hơn trước workload

## 💡 Gợi ý

- **getrusage()**:
  ```c
  #include <sys/resource.h>

  struct rusage usage;
  getrusage(RUSAGE_SELF, &usage);
  // RUSAGE_SELF: current process
  // RUSAGE_CHILDREN: terminated children

  // CPU time
  printf("User CPU time:   %ld.%06lds\n",
         usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
  printf("System CPU time: %ld.%06lds\n",
         usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);

  // Context switches (Linux only - requires kernel 2.6+)
  printf("Voluntary switches:   %ld\n", usage.ru_nvcsw);
  printf("Involuntary switches: %ld\n", usage.ru_nivcsw);

  // Page faults
  printf("Minor faults: %ld\n", usage.ru_minflt);
  printf("Major faults: %ld\n", usage.ru_majflt);

  // Memory
  printf("Max RSS: %ld KB\n", usage.ru_maxrss);
  ```

- **Workload để tạo CPU time**:
  ```c
  // Tính số nguyên tố - CPU intensive
  int is_prime(int n) {
      if (n < 2) return 0;
      for (int i = 2; i * i <= n; i++)
          if (n % i == 0) return 0;
      return 1;
  }

  void cpu_workload(void) {
      int count = 0;
      for (int i = 0; i < 500000; i++) {
          count += is_prime(i);
      }
      printf("Found %d primes\n", count);
  }
  ```

- **Workload để tạo page faults**:
  ```c
  void memory_workload(void) {
      // Allocate large memory and access sparsely to cause page faults
      int pages = 10000;
      size_t page_size = sysconf(_SC_PAGESIZE);
      char **blocks = malloc(pages * sizeof(char*));
      
      for (int i = 0; i < pages; i++) {
          blocks[i] = malloc(page_size);
          blocks[i][0] = 'A';  // Touch page → minor fault if new
      }
      
      for (int i = 0; i < pages; i++) {
          free(blocks[i]);
      }
      free(blocks);
  }
  ```

- **clock_gettime() để đo wall-clock time**:
  ```c
  #include <time.h>

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  // ... do work ...
  clock_gettime(CLOCK_MONOTONIC, &end);

  double elapsed = (end.tv_sec - start.tv_sec) +
                   (end.tv_nsec - start.tv_nsec) / 1e9;
  printf("Wall-clock time: %.3fs\n", elapsed);
  ```

- **Output mong đợi**:
  ```
  ====================================
       Process Resource Usage Demo
  ====================================

  --- Before Workload ---
  User CPU time:        0.000001s
  System CPU time:      0.000001s
  Voluntary csw:        0
  Involuntary csw:      0
  Minor faults:        12
  Major faults:         0
  Max RSS:           2560 KB

  --- Running CPU workload (finding primes) ---
  Found 41538 primes

  --- After Workload (RUSAGE_SELF) ---
  User CPU time:        0.280001s
  System CPU time:      0.010000s
  Voluntary csw:        3
  Involuntary csw:      12
  Minor faults:        15
  Major faults:         0
  Max RSS:           2688 KB

  ...
  ```

## 🔍 Kiểm tra

```bash
# Build
make

# Chạy chương trình
./resource_usage

# So sánh với time command
time ./resource_usage

# Xem resource usage bằng /usr/bin/time
/usr/bin/time -v ./resource_usage
```

## 📁 File cần tạo

- [ ] `resource_usage.c` - Source code chương trình
- [ ] `Makefile` - Build script

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-process.md`:
- Section 3: Memory Layout (hiểu về page và memory mapping)
- Linux man pages: `man getrusage`, `man clock_gettime`, `man times`

---

**Bonus Challenges**:

1. **Compare with `times()`**: Sử dụng `times()` (từ `sys/times.h`) để đo clock ticks và so sánh với getrusage
2. **Child resource tracking**: Fork nhiều children, mỗi child chạy workload khác nhau, parent dùng RUSAGE_CHILDREN
3. **Major fault trigger**: Dùng `mlockall()` hoặc đọc file lớn từ disk để tạo major page faults

```bash
# Bonus: So sánh với /usr/bin/time
/usr/bin/time -v ./resource_usage 2>&1 | grep -E "CPU|fault|switches|RSS"

# Bonus: Xem memory map
cat /proc/self/status | grep -E "VmPeak|VmSize|VmRSS"
```

Sau khi hoàn thành, hãy so sánh với `solution/` để xem cách làm tham khảo!
