# PHẦN 1: BIẾN (VARIABLES)
# -----------------------
# CC: Compiler sử dụng (gcc cho C)
CC = gcc

# CFLAGS: Các cờ biên dịch (compilation flags)
# -Wall: Hiển thị tất cả warnings
# -g: Thêm thông tin debug
CFLAGS = -Wall -g

# TARGET: Tên file thực thi đầu ra
TARGET = hello_world

# SRC: Danh sách file nguồn (.c)
SRC = hello_world.c

# OBJ: Danh sách file object (.o) - được tạo từ SRC
OBJ = $(SRC:.c=.o)

# PHẦN 2: TARGETS (MỤC TIÊU)

# Target mặc định - chạy khi gõ "make" không có tham số
# Phụ thuộc vào $(TARGET), sẽ build file thực thi
all: $(TARGET)

# Quy tắc để tạo file thực thi từ file object
# $@ đại diện cho target (hello_world)
# $^ đại diện cho tất cả dependencies (hello_world.o)
$(TARGET): $(OBJ)
	@echo " Đang liên kết (linking) object files..."
	$(CC) $(CFLAGS) -o $@ $^
	@echo " Build thành công! File thực thi: $(TARGET)"

# Quy tắc để tạo file .o từ file .c
# $< đại diện cho dependency đầu tiên (file .c)
%.o: %.c
	@echo "  Đang biên dịch (compiling) $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Target để chạy chương trình
# .PHONY: báo cho Make biết "run" không phải là file thật
.PHONY: run
run: $(TARGET)
	@echo " Chạy chương trình..."
	./$(TARGET)

# Target để dọn dẹp các file được tạo ra
.PHONY: clean
clean:
	@echo " Đang dọn dẹp..."
	rm -f $(OBJ) $(TARGET)
	@echo " Đã xóa tất cả file build"

# Target để xem thông tin
.PHONY: info
info:
	@echo " Thông tin Makefile:"
	@echo "  Compiler: $(CC)"
	@echo "  Flags: $(CFLAGS)"
	@echo "  Source files: $(SRC)"
	@echo "  Object files: $(OBJ)"
	@echo "  Target: $(TARGET)"

# Target để hiển thị trợ giúp
.PHONY: help
help:
	@echo "========================================="
	@echo " HƯỚNG DẪN SỬ DỤNG MAKEFILE"
	@echo "========================================="
	@echo "Các lệnh có thể sử dụng:"
	@echo ""
	@echo "  make              - Build chương trình (tạo file thực thi)"
	@echo "  make run          - Build và chạy chương trình"
	@echo "  make clean        - Xóa các file build (.o và executable)"
	@echo "  make info         - Hiển thị thông tin cấu hình"
	@echo "  make help         - Hiển thị trợ giúp này"
	@echo ""
	@echo "========================================="
	@echo " GIẢI THÍCH CÁC THÀNH PHẦN:"
	@echo "========================================="
	@echo "  TARGET   : File thực thi cuối cùng"
	@echo "  .o files : File object (trung gian)"
	@echo "  .c files : File mã nguồn C"
	@echo ""
	@echo "Quy trình: .c → .o → executable"
	@echo "========================================="


# GHI CHÚ VÀ MẸO HỮU ÍCH

# 1. Makefile phân biệt TAB và SPACE!
#    - Các lệnh bên dưới target PHẢI bắt đầu với TAB, không phải space
#
# 2. Ký tự đặc biệt:
#    $@ : Tên của target hiện tại
#    $^ : Tất cả các dependencies
#    $< : Dependency đầu tiên
#
# 3. .PHONY targets:
#    - Dùng cho các target không tạo ra file (như clean, run)
#    - Tránh xung đột nếu có file trùng tên
#
# 4. Ký tự @ ở đầu lệnh:
#    - Không hiển thị lệnh khi chạy (chỉ hiển thị kết quả)
#
# 5. Để debug Makefile:
#    - Thêm -n để xem lệnh sẽ chạy: make -n
#    - Thêm -B để force rebuild: make -B
# ============================================
