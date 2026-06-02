#===============================================================================
# Conditional Build Makefile - Multi-Configuration
#===============================================================================

# === Build Profile (default: debug) ===
BUILD ?= debug

# === Toolchain ===
CC = gcc
CFLAGS_BASE = -Wall -Wextra -std=c11 -Iinclude
LDFLAGS =

# === Directories ===
SRC_DIR = src
BUILD_DIR = build/$(BUILD)
BIN_DIR = bin
HEADERS = $(wildcard include/*.h)

# === Platform Detection ===
UNAME := $(shell uname -m)

# === Profile Configuration ===
ifeq ($(BUILD),debug)
    # Debug: không tối ưu, có debug symbols, định nghĩa DEBUG
    CFLAGS = $(CFLAGS_BASE) -O0 -g -DDEBUG
    TARGET = $(BIN_DIR)/app-debug
    TARGET_SUFFIX = -debug
else ifeq ($(BUILD),release)
    # Release: tối ưu tốc độ, không debug
    CFLAGS = $(CFLAGS_BASE) -O2 -DNDEBUG
    TARGET = $(BIN_DIR)/app
    TARGET_SUFFIX =
else ifeq ($(BUILD),test)
    # Test: debug + test mode
    CFLAGS = $(CFLAGS_BASE) -O0 -g -DTEST_MODE
    TARGET = $(BIN_DIR)/app-test
    TARGET_SUFFIX = -test
else ifeq ($(BUILD),minsize)
    # MinSize: tối ưu kích thước
    CFLAGS = $(CFLAGS_BASE) -Os -DNDEBUG
    TARGET = $(BIN_DIR)/app-min
    TARGET_SUFFIX = -min
else
    $(error Unknown BUILD profile: $(BUILD). Use: debug, release, test, minsize)
endif

# === Platform Flags ===
ifneq (,$(filter arm%,$(UNAME)))
    CFLAGS += -DPLATFORM_ARM
else ifneq (,$(filter x86_64%,$(UNAME)))
    CFLAGS += -DPLATFORM_X86
else ifneq (,$(filter i[3-6]86%,$(UNAME)))
    CFLAGS += -DPLATFORM_X86
endif

# === Feature Flags (có thể override từ command line) ===
ENABLE_LOGGING ?= 1
DEBUG_UART ?= 0
USE_SENSOR_SIM ?= 0

# Convert feature flags to compiler defines
ifeq ($(ENABLE_LOGGING),1)
    CFLAGS += -DENABLE_LOGGING
endif

ifeq ($(DEBUG_UART),1)
    CFLAGS += -DDEBUG_UART
endif

ifeq ($(USE_SENSOR_SIM),1)
    CFLAGS += -DUSE_SENSOR_SIM
endif

# === Source Files ===
# Luôn build
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/sensor_hal.c

# Platform driver - chỉ build khi không dùng SIM
ifneq ($(USE_SENSOR_SIM),1)
    SRCS += $(SRC_DIR)/platform.c
endif

# Sinh object files
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# === Version Info ===
VERSION_H = $(BUILD_DIR)/version.h

# Lấy git commit hash (hoặc "unknown")
GIT_HASH := $(shell git rev-parse --short HEAD 2>/dev/null || echo "unknown")
BUILD_DATE := $(shell date '+%Y-%m-%d %H:%M:%S')

#===============================================================================
# Targets
#===============================================================================

.PHONY: all clean run info profiles

all: $(TARGET)

# Tạo directories
$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

# Sinh version header
$(VERSION_H): | $(BUILD_DIR)
	@echo "Generating version info..."
	@echo "#ifndef VERSION_H" > $@
	@echo "#define VERSION_H" >> $@
	@echo "" >> $@
	@echo "#define BUILD_PROFILE \"$(BUILD)\"" >> $@
	@echo "#define BUILD_DATE \"$(BUILD_DATE)\"" >> $@
	@echo "#define GIT_HASH \"$(GIT_HASH)\"" >> $@
	@echo "" >> $@
	@echo "#endif /* VERSION_H */" >> $@

# Compile
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) $(VERSION_H) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)
	@echo ""
	@echo "Build complete: $@"
	@echo "  Profile: $(BUILD)"
	@echo "  Size: $$(stat -c%s $@ 2>/dev/null || echo '?') bytes"
	@echo "  Features: LOG=$(ENABLE_LOGGING) UART=$(DEBUG_UART) SIM=$(USE_SENSOR_SIM)"

#===============================================================================
# Utility Targets
#===============================================================================

run: $(TARGET)
	./$(TARGET)

# Hiển thị thông tin build
info:
	@echo "=== Build Configuration ==="
	@echo "  Profile:     $(BUILD)"
	@echo "  Platform:    $(UNAME)"
	@echo "  Compiler:    $(CC)"
	@echo "  CFLAGS:      $(CFLAGS)"
	@echo ""
	@echo "=== Feature Flags ==="
	@echo "  ENABLE_LOGGING: $(ENABLE_LOGGING)"
	@echo "  DEBUG_UART:     $(DEBUG_UART)"
	@echo "  USE_SENSOR_SIM: $(USE_SENSOR_SIM)"
	@echo ""
	@echo "=== Version ==="
	@echo "  Git hash:     $(GIT_HASH)"
	@echo "  Build date:   $(BUILD_DATE)"
	@echo ""
	@echo "=== Source Files ==="
	@for src in $(SRCS); do echo "  $$src"; done
	@echo "=== Object Files ==="
	@for obj in $(OBJS); do echo "  $$obj"; done

# Danh sách profiles
profiles:
	@echo "Available build profiles:"
	@echo "  make BUILD=debug     - Debug (-O0 -g -DDEBUG)"
	@echo "  make BUILD=release   - Release (-O2 -DNDEBUG)"
	@echo "  make BUILD=test      - Test mode (-O0 -g -DTEST_MODE)"
	@echo "  make BUILD=minsize   - Min size (-Os -DNDEBUG)"
	@echo ""
	@echo "Feature flags (default):"
	@echo "  ENABLE_LOGGING=1     - Enable logging"
	@echo "  DEBUG_UART=0         - Enable UART debug"
	@echo "  USE_SENSOR_SIM=0     - Use sensor simulation"

clean:
	rm -rf build bin
	@echo "Cleaned all build artifacts"
