# Makefile cho bài tập 03 - SOLUTION

CC = gcc
CFLAGS = -Wall -g -I..
AR = ar
ARFLAGS = rcs

TARGET = string_demo
LIBRARY = libstring.a

# Source files
LIB_SRC = string_utils.c
LIB_OBJ = $(LIB_SRC:.c=.o)
MAIN_OBJ = main.o

# Tìm source files ở thư mục cha
VPATH = ..

# Target mặc định: build chương trình
all: $(TARGET)

# Tạo static library
library: $(LIBRARY)

$(LIBRARY): $(LIB_OBJ)
	@echo "Creating static library $(LIBRARY)..."
	$(AR) $(ARFLAGS) $@ $^
	@echo "Library created successfully!"

# Link chương trình với library
$(TARGET): $(MAIN_OBJ) $(LIBRARY)
	@echo "Linking $(TARGET) with $(LIBRARY)..."
	$(CC) $(CFLAGS) -o $@ $(MAIN_OBJ) -L. -lstring
	@echo "Build thành công!"

# Compile object files
%.o: %.c string_utils.h
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Chạy chương trình
.PHONY: run
run: $(TARGET)
	./$(TARGET)

# Dọn dẹp
.PHONY: clean
clean:
	rm -f $(LIB_OBJ) $(MAIN_OBJ) $(LIBRARY) $(TARGET)
	@echo "Đã xóa tất cả file build"

# Rebuild
.PHONY: rebuild
rebuild: clean all
