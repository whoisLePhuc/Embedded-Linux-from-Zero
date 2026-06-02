#!/bin/bash
# Phân tích core dump cho Exercise 03
# Cách dùng: bash analyze_core.sh

set -e

echo "=== Core Dump Analysis Script ==="
echo ""

# B1: Kích hoạt core dump
echo "[1] Enabling core dumps..."
ulimit -c unlimited
echo "ulimit -c = $(ulimit -c)"
echo ""

# B2: Build
echo "[2] Building crash_me..."
make clean 2>/dev/null || true
make
echo ""

# B3: Chạy để tạo core dump
echo "[3] Running to generate core dump..."
./crash_me 2>&1 || true
echo ""

# B4: Tìm core dump
CORE_FILE=$(ls core* 2>/dev/null | head -1)
if [ -z "$CORE_FILE" ]; then
    echo "[ERROR] No core dump file found!"
    echo "Check: ulimit -c, disk space, /proc/sys/kernel/core_pattern"
    exit 1
fi
echo "[4] Found core dump: $CORE_FILE"
ls -lh "$CORE_FILE"
echo ""

# B5: Phân tích với GDB
echo "[5] Analyzing with GDB..."
echo ""
echo "--- Backtrace ---"
gdb -batch -ex "bt" -ex "quit" ./crash_me "$CORE_FILE" 2>/dev/null || \
gdb -batch -ex "bt" ./crash_me "$CORE_FILE"
echo ""

echo "=== PHÂN TÍCH ==="
echo ""
echo "BUG 1: print_inorder() thiếu base case (if root == NULL return)"
echo "Khi cây rỗng hoặc duyệt đến node NULL, đệ quy không dừng"
echo "gây stack overflow (SIGSEGV do stack exhaustion)."
echo ""
echo "Sửa: thêm 'if (root == NULL) return;' ở đầu print_inorder()"
echo ""
echo "BUG 2: Dangling pointer dereference sau khi free_tree()"
echo "dangling->value truy cập vùng nhớ đã được free."
echo ""
echo "Sửa: bỏ đoạn code dùng dangling pointer sau free."
echo ""
echo "Cách dùng GDB step-by-step:"
echo "  gdb ./crash_me $CORE_FILE"
echo "  (gdb) bt                  # xem call stack"
echo "  (gdb) frame 3             # chọn frame sâu trong đệ quy"
echo "  (gdb) info locals         # xem biến địa phương"
echo "  (gdb) list                # xem source"
echo "  (gdb) print root          # xem giá trị node"
