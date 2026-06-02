# Makefile cho bài tập 04 - SOLUTION (Advanced)

# Compiler và flags
CC = gcc
CFLAGS = -Wall -I$(INC_DIR)
LDFLAGS = -lm

# Directories
SRC_DIR = ../src
INC_DIR = ../include
BUILD_DIR = build
BIN_DIR = bin

# Target executable
TARGET = $(BIN_DIR)/calculator

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Colors for output
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
NC = \033[0m # No Color

# Conditional compilation
ifdef DEBUG
    CFLAGS += -g -O0 -DDEBUG
    BUILD_TYPE = Debug
else
    CFLAGS += -O2
    BUILD_TYPE = Release
endif

# Default target
all: $(TARGET)
	@echo "$(GREEN)Build type: $(BUILD_TYPE)$(NC)"

# Create directories if they don't exist
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)
	@echo "$(YELLOW)Created $(BUILD_DIR) directory$(NC)"

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)
	@echo "$(YELLOW)Created $(BIN_DIR) directory$(NC)"

# Link executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	@echo "$(GREEN)Linking $@...$(NC)"
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "$(GREEN)Build thành công!$(NC)"

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo "$(YELLOW)Compiling $<...$(NC)"
	$(CC) $(CFLAGS) -c $< -o $@

# Debug build
.PHONY: debug
debug:
	@$(MAKE) DEBUG=1

# Release build
.PHONY: release
release:
	@$(MAKE)

# Run program
.PHONY: run
run: $(TARGET)
	@echo "$(GREEN)Running $(TARGET)...$(NC)"
	@./$(TARGET)

# Clean build artifacts
.PHONY: clean
clean:
	@echo "$(RED)Cleaning build artifacts...$(NC)"
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "$(GREEN)Clean complete!$(NC)"

# Rebuild
.PHONY: rebuild
rebuild: clean all

# Show info
.PHONY: info
info:
	@echo "$(YELLOW)Project Information:$(NC)"
	@echo "  Build type: $(BUILD_TYPE)"
	@echo "  Compiler: $(CC)"
	@echo "  Flags: $(CFLAGS)"
	@echo "  Sources: $(SOURCES)"
	@echo "  Objects: $(OBJECTS)"
	@echo "  Target: $(TARGET)"

# Help
.PHONY: help
help:
	@echo "$(GREEN)Available targets:$(NC)"
	@echo "  make           - Build project (release mode)"
	@echo "  make debug     - Build with debug symbols"
	@echo "  make release   - Build optimized version"
	@echo "  make run       - Build and run program"
	@echo "  make clean     - Remove all build artifacts"
	@echo "  make rebuild   - Clean and build again"
	@echo "  make info      - Show project information"
	@echo "  make help      - Show this help message"
