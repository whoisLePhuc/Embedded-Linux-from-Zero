# Bài tập 07: Conditional Build và Multi-Configuration ⭐⭐⭐

## 🎯 Mục tiêu

Làm chủ conditional compilation trong Makefile: debug/release profiles, platform detection, feature flags, và custom build configurations.

## 📋 Yêu cầu

Tạo Makefile hỗ trợ nhiều build configurations cho một dự án embedded Linux.

### Cấu trúc thư mục:

```
exercise-07-conditional-build/
├── README.md
├── src/
│   ├── main.c
│   ├── sensor_hal.c
│   └── platform.c
├── include/
│   ├── sensor_hal.h
│   └── platform.h
└── solution/
```

### Yêu cầu Makefile:

1. **Build Profiles** (dùng biến `BUILD`):
   ```
   BUILD=debug   → make BUILD=debug   # Debug: -O0 -g -DDEBUG
   BUILD=release → make BUILD=release # Release: -O2 -DNDEBUG
   BUILD=test    → make BUILD=test    # Test: -O0 -g -DTEST_MODE
   BUILD=minsize → make BUILD=minsize # MinSize: -Os -DNDEBUG
   ```

2. **Platform Detection**:
   - Tự động phát hiện architecture (`uname -m`)
   - Set flag `-DPLATFORM_ARM` nếu là ARM
   - Set flag `-DPLATFORM_X86` nếu là x86

3. **Feature Flags**:
   ```makefile
   ENABLE_LOGGING ?= 1     # Bật/tắt logging
   ENABLE_DEBUG_UART ?= 0  # Bật/tắt UART debug
   USE_SENSOR_SIM ?= 0     # Dùng sensor simulation
   ```

4. **Conditional Source Files**:
   - `sensor_hal.c` luôn được build
   - `platform.c` chỉ build khi `USE_SENSOR_SIM=0`

5. **Version Info**:
   - Tự động sinh file `version.h` với:
     - GIT commit hash (`git rev-parse HEAD`)
     - Build date
     - Build profile name

6. **Các targets**:
   - `all` + `clean` + `run`
   - `profiles`: Hiển thị tất cả profiles available
   - `info`: Hiển thị thông tin build config hiện tại

## ✅ Tiêu chí đạt

- Makefile hỗ trợ 4 build profiles (debug/release/test/minsize)
- Feature flags có thể override từ command line
- Conditional compilation dùng `-D` flags
- `make` (không argument) mặc định là debug profile
- Version info được tự động sinh

## 💡 Gợi ý

```makefile
# Build profiles
BUILD ?= debug

# Profile configurations
ifeq ($(BUILD),debug)
    CFLAGS += -O0 -g -DDEBUG
    TARGET_SUFFIX = -debug
else ifeq ($(BUILD),release)
    CFLAGS += -O2 -DNDEBUG
    TARGET_SUFFIX =
else ifeq ($(BUILD),test)
    CFLAGS += -O0 -g -DTEST_MODE
    TARGET_SUFFIX = -test
endif

# Platform detection
UNAME := $(shell uname -m)
ifneq (,$(filter arm%,$(UNAME)))
    CFLAGS += -DPLATFORM_ARM
else ifneq (,$(filter x86_64%,$(UNAME)))
    CFLAGS += -DPLATFORM_X86
endif

# Feature flags
ENABLE_LOGGING ?= 1
ifeq ($(ENABLE_LOGGING),1)
    CFLAGS += -DENABLE_LOGGING
endif

# Conditional sources
ifneq ($(USE_SENSOR_SIM),1)
    SRCS += src/platform.c
endif
```

## 🔍 Kiểm tra

```bash
# Debug profile (mặc định)
make clean all info
./build/app-debug

# Release profile
make BUILD=release clean all
./build/app

# Test profile với feature flags
make BUILD=test ENABLE_LOGGING=0 USE_SENSOR_SIM=1 clean all

# MinSize profile
make BUILD=minsize clean all

# Xem kích thước binary giữa các profiles
ls -lh build/
```

## 📁 File cần tạo

- [ ] `Makefile` - Build script với conditional build

## 📁 File đã cho

- `src/main.c`, `src/sensor_hal.c`, `src/platform.c`
- `include/sensor_hal.h`, `include/platform.h`

## 🎓 Kiến thức liên quan

Đọc lại trong `general-knowledge.md`:
- Section 1.4: Variable Makefile (conditional assignment)
- Section 1.5: Automatic Variable

---

**Bonus 1:** Thêm target `cross` để cross-compile cho ARM (sử dụng biến `CROSS_COMPILE`).

**Bonus 2:** Tự động tạo `.gitignore` với các thư mục build.
