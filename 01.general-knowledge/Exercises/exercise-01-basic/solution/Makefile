# Makefile cho bài tập 01 - SOLUTION

CC = gcc
CFLAGS = -Wall -g
TARGET = rectangle
SRC = rectangle.c

# Tìm source files ở thư mục cha
VPATH = ..

# Target mặc định
all: $(TARGET)

# Tạo file thực thi
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Build thành công!"

# Chạy chương trình
.PHONY: run
run: $(TARGET)
	./$(TARGET)

# Dọn dẹp
.PHONY: clean
clean:
	rm -f $(TARGET)
	@echo "Đã xóa file build"
