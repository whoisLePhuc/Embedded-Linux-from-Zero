# Exercise 3: Anonymous Shared Memory

## 🎯 Mục tiêu
- Hiểu cách chia sẻ bộ nhớ giữa process Cha và Con mà không cần đặt tên (Anonymous).
- Sử dụng `mmap()` với flag `MAP_SHARED | MAP_ANONYMOUS`.
- Ứng dụng: Giao tiếp nhanh giữa các process có quan hệ huyết thống mà không cần file backing.

## 📝 Yêu cầu

Viết chương trình `main.c` thực hiện:

1. Process Cha tạo vùng nhớ chung bằng `mmap` (size = int).
2. Khởi tạo giá trị ban đầu trong vùng nhớ (ví dụ `*ptr = 10`).
3. `fork()` tạo Process Con.
4. **Process Con**:
    - Đọc giá trị từ vùng nhớ chung.
    - Tăng giá trị lên (ví dụ += 5).
    - In ra: "Child updated value to: ..."
5. **Process Cha**:
    - Wait cho con kết thúc.
    - Đọc giá trị từ vùng nhớ chung (đã được con sửa).
    - In ra: "Parent sees value: ..."
6. Unmap (`munmap`).

## 🧪 Hướng dẫn kiểm tra

```bash
make
./anonymous
```

Kết quả mong đợi:
```
Original value: 10
Child updated value to: 15
Parent sees value: 15
```

## 💡 Gợi ý

```c
int *ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, 
                MAP_SHARED | MAP_ANONYMOUS, -1, 0);
*ptr = 10;
// fork...
```
