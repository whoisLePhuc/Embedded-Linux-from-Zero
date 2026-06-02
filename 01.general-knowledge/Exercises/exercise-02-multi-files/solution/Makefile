# Makefile cho bài tập 02 - SOLUTION

CC = gcc
CFLAGS = -Wall -g -I..
TARGET = calculator
SRC = main.c math_utils.c
OBJ = $(SRC:.c=.o)

# Tìm source files ở thư mục cha
VPATH = ..

# Target mặc định
all: $(TARGET)

# Link các object files thành executable
$(TARGET): $(OBJ)
	@echo "Linking object files..."
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Build thành công!"

# Pattern rule: biên dịch .c thành .o
%.o: %.c math_utils.h
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Chạy chương trình
.PHONY: run
run: $(TARGET)
	./$(TARGET)

# Dọn dẹp
.PHONY: clean
clean:
	rm -f $(OBJ) $(TARGET)
	@echo "Đã xóa tất cả file build"

# Rebuild: clean rồi build lại
.PHONY: rebuild
rebuild: clean all
