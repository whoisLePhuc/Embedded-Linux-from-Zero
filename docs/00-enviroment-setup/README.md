# Khóa học Embedded Linux - Tài liệu hướng dẫn

Chào mừng bạn đến với tài liệu hướng dẫn học Embedded Linux! Đây là bộ tài liệu đầy đủ giúp bạn từ bước cài đặt môi trường đến lập trình C trên Linux.

---

## 📚 Giới thiệu

Bộ tài liệu này được thiết kế theo lộ trình từ cơ bản đến nâng cao, giúp bạn:

- Thiết lập môi trường phát triển Embedded Linux
- Làm quen với Linux và công cụ lập trình
- Viết chương trình C đầu tiên trên Linux
- Phát triển kỹ năng lập trình nhúng

---

## 🗺️ Lộ trình học tập

### Bước 1: Chuẩn bị môi trường (Bắt buộc)

Đọc theo thứ tự sau để thiết lập môi trường làm việc:

#### 1.1. Cài đặt VMware Workstation
📄 **File:** [`huong-dan-cai-dat-vmware-workstation.md`](huong-dan-cai-dat-vmware-workstation.md)

**Nội dung:**
- Tải và cài đặt VMware Workstation Pro
- Cấu hình cơ bản

**Thời gian:** ~15 phút

---

#### 1.2. Cài đặt Ubuntu 22.04 LTS trên VMware
📄 **File:** [`huong-dan-cai-dat-ubuntu.md`](huong-dan-cai-dat-ubuntu.md)

**Nội dung:**
- Tạo máy ảo trong VMware
- Cài đặt Ubuntu 22.04 LTS
- Cấu hình cơ bản
- Cài đặt VMware Tools

**Thời gian:** ~30-45 phút

---

#### 1.3. Cài đặt Visual Studio Code
📄 **File:** [`huong-dan-cai-dat-vscode.md`](huong-dan-cai-dat-vscode.md)

**Nội dung:**
- Tải và cài đặt VS Code trên Windows
- Cài đặt extensions cần thiết (Remote - SSH, C/C++, CMake Tools)

**Thời gian:** ~10 phút

---

#### 1.4. Kết nối SSH từ VS Code đến Ubuntu
📄 **File:** [`huong-dan-ssh-toi-vmware-qua-vscode.md`](huong-dan-ssh-toi-vmware-qua-vscode.md)

**Nội dung:**
- Cài đặt SSH Server trên Ubuntu
- Lấy địa chỉ IP của máy ảo
- Cấu hình Remote SSH trong VS Code
- Kết nối và kiểm tra
- Xử lý sự cố thường gặp

**Thời gian:** ~15-20 phút

---

### Bước 2: Lập trình C đầu tiên (Thực hành)

#### 2.1. Chương trình Hello World
📄 **File:** [`hello-world-program.md`](hello-world-program.md)

**Nội dung:**
- Tạo dự án C đầu tiên
- Biên dịch với GCC
- Sử dụng VS Code để lập trình
- Tạo Makefile
- Debugging với GDB
- Xử lý lỗi thường gặp

**Thời gian:** ~30-60 phút

---

### Bước 3: Tham khảo thiết bị (Tùy chọn)

#### 3.1. Danh sách thiết bị
📄 **File:** [`danh-sach-thiet-bi.md`](danh-sach-thiet-bi.md)

**Nội dung:**
- Raspberry Pi Zero
- Module OLED (I2C, SPI)
- USB-to-UART converter
- Phụ kiện cần thiết

**Mục đích:** Tham khảo khi cần mua thiết bị thực hành

---

## 📖 Cách sử dụng tài liệu

### ✅ Cho người mới bắt đầu

Đọc **tuần tự** theo thứ tự sau:

```
1. huong-dan-cai-dat-vmware-workstation.md
   ↓
2. huong-dan-cai-dat-ubuntu.md
   ↓
3. huong-dan-cai-dat-vscode.md
   ↓
4. huong-dan-ssh-toi-vmware-qua-vscode.md
   ↓
5. hello-world-program.md
```

### 🔍 Cho người đã có kinh nghiệm

Bạn có thể:
- Bỏ qua các bước đã biết
- Đọc trực tiếp phần cần thiết
- Sử dụng mục lục trong mỗi file để tìm nội dung cụ thể

---

## 🎯 Mục tiêu sau khi hoàn thành

Sau khi hoàn thành tất cả các bước, bạn sẽ có:

- ✅ Môi trường phát triển Embedded Linux hoàn chỉnh
- ✅ Kỹ năng sử dụng VMware, Ubuntu, VS Code
- ✅ Hiểu quy trình: viết code → biên dịch → chạy → debug
- ✅ Nền tảng để học các chủ đề nâng cao hơn

---

## 📂 Cấu trúc thư mục

```
embedded-linux/
│
├── README.md                                    # File này - hướng dẫn tổng quan
│
├── huong-dan-cai-dat-vmware-workstation.md     # Bước 1.1
├── huong-dan-cai-dat-ubuntu.md                 # Bước 1.2
├── huong-dan-cai-dat-vscode.md                 # Bước 1.3
├── huong-dan-ssh-toi-vmware-qua-vscode.md      # Bước 1.4
│
├── hello-world-program.md                      # Bước 2.1
│
├── danh-sach-thiet-bi.md                       # Tham khảo
│
└── images/                                      # Hình ảnh minh họa
    ├── huong-dan-cai-dat-ubuntu/
    ├── huong-dan-cai-dat-vmware/
    ├── huong-dan-cai-dat-vscode/
    ├── huong-dan-ssh/
    └── danh-sach-thiet-bi/
```

---

## 💡 Lưu ý quan trọng

### Yêu cầu hệ thống

- **Hệ điều hành:** Windows 10/11 (64-bit)
- **CPU:** Intel Core i5 trở lên (hỗ trợ VT-x/AMD-V)
- **RAM:** Tối thiểu 8GB (khuyến nghị 16GB)
- **Ổ cứng trống:** Tối thiểu 100GB
- **Internet:** Để tải phần mềm và cập nhật

### Trước khi bắt đầu

1. **Bật Virtualization** trong BIOS/UEFI
2. **Tải trước** các file cài đặt:
   - VMware Workstation Pro
   - Ubuntu 22.04 LTS ISO
   - VS Code installer
3. **Dành thời gian:** Khoảng 2-3 giờ để hoàn thành toàn bộ setup

---

## 🆘 Hỗ trợ và xử lý sự cố

Mỗi file tài liệu đều có phần **"Xử lý sự cố thường gặp"** với:
- Các lỗi phổ biến
- Nguyên nhân
- Cách khắc phục chi tiết

Nếu gặp vấn đề:
1. Đọc phần troubleshooting trong file tương ứng
2. Kiểm tra lại các bước đã thực hiện
3. Đảm bảo yêu cầu hệ thống được đáp ứng

---

## 📊 Tiến độ học tập

Đánh dấu ✅ khi hoàn thành mỗi bước:

- [ ] Cài đặt VMware Workstation
- [ ] Cài đặt Ubuntu 22.04 LTS
- [ ] Cài đặt Visual Studio Code
- [ ] Kết nối SSH từ VS Code đến Ubuntu
- [ ] Viết và chạy chương trình Hello World
- [ ] Sử dụng GCC compiler
- [ ] Tạo Makefile
- [ ] Debug với GDB

---

## 🚀 Bước tiếp theo

Sau khi hoàn thành các tài liệu này, bạn có thể:

### Học thêm về C Programming
- Biến và kiểu dữ liệu
- Cấu trúc điều khiển (if, for, while)
- Functions và pointers
- Arrays và strings
- Structs và file I/O

### Embedded Linux nâng cao
- GPIO programming
- I2C và SPI communication
- Device drivers
- Cross-compilation cho ARM
- Real-time Linux

### Thực hành với phần cứng
- Raspberry Pi programming
- Sensor interfacing
- Motor control
- IoT projects

---

## 📝 Ghi chú

- Tất cả tài liệu được viết bằng **Markdown** để dễ đọc và chỉnh sửa
- Mỗi file có **mục lục** để dễ dàng điều hướng
- **Hình ảnh minh họa** được lưu trong thư mục `images/`
- Code examples có **syntax highlighting** và giải thích chi tiết

---

## 📜 Phiên bản

- **Phiên bản:** 1.0
- **Cập nhật:** 2025-12-04
- **Tương thích:** Ubuntu 22.04 LTS, VMware Workstation 17, VS Code latest

---

**Chúc bạn học tập hiệu quả!** 🎓

Nếu có câu hỏi hoặc gặp khó khăn, hãy đọc kỹ phần troubleshooting trong mỗi tài liệu.
