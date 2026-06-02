#===============================================================================
# Makefile với Functions và Wildcards
#===============================================================================
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
LDFLAGS =

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
TARGET = $(BIN_DIR)/app

# Tự động tìm tất cả source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Sinh object files paths: src/main.c -> build/main.o
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Header dependencies (tự động tìm)
HEADERS = $(wildcard include/*.h)

# Số lượng source files
SRC_COUNT = $(words $(SRCS))

# Lấy tên file không path
SRC_NAMES = $(notdir $(SRCS))

# Filter chỉ lấy file main
MAIN_SRC = $(filter %main.c, $(SRCS))
UTIL_SRCS = $(filter-out %main.c, $(SRCS))

#===============================================================================
# Targets
#===============================================================================

.PHONY: all clean run list stats tree

all: $(TARGET)

# Tạo thư mục build và bin
$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

# Compile C files thành object files trong build/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files thành executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)
	@echo "Build complete: $@"

#===============================================================================
# Utility Targets
#===============================================================================

# Danh sách file
list:
	@echo "=== Source Files ($(SRC_COUNT) files) ==="
	@for src in $(SRCS); do \
		echo "  $$src"; \
	done
	@echo ""
	@echo "=== Object Files ==="
	@for obj in $(OBJS); do \
		echo "  $$obj"; \
	done
	@echo ""
	@echo "=== File Names (no path) ==="
	@for name in $(SRC_NAMES); do \
		echo "  $$name"; \
	done
	@echo ""
	@echo "=== By Category ==="
	@echo "  Main: $(MAIN_SRC)"
	@echo "  Utils: $(UTIL_SRCS)"
	@echo "  Headers: $(HEADERS)"

# Đếm dòng code
stats:
	@echo "=== Code Statistics ==="
	@echo "  Source files: $(SRC_COUNT)"
	@echo "  Header files: $(words $(HEADERS))"
	@echo "  Total lines:"
	@wc -l $(SRCS) $(HEADERS) 2>/dev/null | tail -1

# Cấu trúc cây thư mục
tree:
	@echo "Project structure:"
	@echo "  $(SRC_DIR)/"
	@for src in $(SRC_NAMES); do \
		echo "    ├─ $$src"; \
	done
	@echo "  include/"
	@for hdr in $(HEADERS); do \
		echo "    ├─ $$(notdir $$hdr)"; \
	done

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Cleaned build artifacts"
