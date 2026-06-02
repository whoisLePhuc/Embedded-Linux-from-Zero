# Bài tập 04: Environment Variables Manager ⭐⭐

## 🎯 Mục tiêu

Thực hành làm việc với biến môi trường (Environment Variables) trong Linux: export, source, PATH, và các biến quan trọng khác.

## 📋 Yêu cầu

### Phần A: Script `env_manager.sh`

Viết script quản lý biến môi trường với các chức năng:

1. **List all** (`-l`): Hiển thị tất cả biến môi trường hiện tại, sắp xếp theo tên
2. **Get value** (`-g <VAR>`): Hiển thị giá trị của một biến cụ thể
3. **Set variable** (`-s <VAR=value>`): Set biến môi trường (export) và xác nhận
4. **Delete variable** (`-d <VAR>`): Xóa (unset) biến môi trường
5. **Path analysis** (`-p`): Phân tích biến `$PATH`:
   - Đếm số lượng directories trong PATH
   - Liệt kê từng directory
   - Kiểm tra directory nào tồn tại / không tồn tại
6. **Compare** (`-c <file1> <file2>`): So sánh biến môi trường giữa 2 file (so sánh sự khác biệt giữa các file env)
7. **Export to file** (`-o <filename>`): Export tất cả biến môi trường ra file định dạng `KEY="VALUE"`

### Phần B: Tạo file `.env` mẫu

Tạo file `sample.env` với các biến cấu hình cho một ứng dụng embedded Linux giả định:

```
APP_NAME=EmbeddedSensor
APP_VERSION=2.1.0
LOG_LEVEL=DEBUG
LOG_DIR=/var/log/sensor
DATA_DIR=/data/sensor
MAX_BUFFER_SIZE=4096
SAMPLE_RATE=100
I2C_BUS=1
I2C_ADDRESS=0x48
SPI_SPEED=1000000
UART_BAUD=115200
UART_DEVICE=/dev/ttyAMA0
ENABLE_DEBUG=1
ENABLE_TELEMETRY=0
```

Viết script `env_loader.sh` để:
1. Load file `.env` bằng `source` hoặc `export`
2. Validate các biến bắt buộc (kiểm tra có tồn tại không)
3. Hiển thị các biến đã load thành công

## ✅ Tiêu chí đạt

- Phần A: Script chạy được tất cả 7 chức năng
- Phần A: Sử dụng `printenv`, `export`, `unset` đúng cách
- Phần A: PATH analysis phải phát hiện directory không tồn tại
- Phần B: File `.env` đúng format (không space quanh `=`, giá trị có thể có quote)
- Phần B: Script `env_loader.sh` validate biến bắt buộc

## 💡 Gợi ý

```bash
# Phân tích PATH
analyze_path() {
    local IFS=':'
    local count=0
    for dir in $PATH; do
        ((count++))
        if [ -d "$dir" ]; then
            echo "  [OK] $dir"
        else
            echo "  [MISSING] $dir"
        fi
    done
    echo "Total directories in PATH: $count"
}

# Validate env variables từ file
validate_env() {
    local required_vars=("APP_NAME" "LOG_LEVEL" "DATA_DIR")
    local missing=0
    for var in "${required_vars[@]}"; do
        if [ -z "${!var}" ]; then
            echo "Missing required variable: $var"
            ((missing++))
        fi
    done
}
```

## 🔍 Kiểm tra

```bash
chmod +x env_manager.sh env_loader.sh

# List tất cả biến
./env_manager.sh -l

# Xem PATH analysis
./env_manager.sh -p

# Set và get biến
./env_manager.sh -s "MY_TEST_VAR=hello_world"
./env_manager.sh -g MY_TEST_VAR

# Export ra file
./env_manager.sh -o my_env.txt

# Load .env file
source sample.env  # hoặc dùng env_loader.sh
./env_loader.sh sample.env
```

## 📁 File cần tạo

- [ ] `env_manager.sh` - Script quản lý biến môi trường
- [ ] `sample.env` - File .env mẫu
- [ ] `env_loader.sh` - Script load và validate .env

## 🎓 Kiến thức liên quan

Đọc lại trong `linux-basic-knowledge.md`:
- Section 7: Biến môi trường
- Section 7.2: Biến thường vs Biến môi trường
- Section 7.3: Các biến quan trọng (PATH, HOME, LD_LIBRARY_PATH...)

---

**Bonus:** Thêm option `-e` để tạo environment module file cho embedded cross-compilation:
```
export CROSS_COMPILE=arm-linux-gnueabihf-
export CC=${CROSS_COMPILE}gcc
export ARCH=arm
export CFLAGS="-O2 -march=armv7-a"
```
