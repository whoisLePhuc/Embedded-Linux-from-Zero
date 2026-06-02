#!/bin/bash
set -e

echo "[*] Finding sysroot path..."
SYSROOT=$(arm-linux-gnueabihf-gcc -print-sysroot)
echo "    Sysroot: $SYSROOT"
echo ""

echo "[*] Contents of sysroot:"
ls "$SYSROOT"
echo ""

echo "[*] Checking header availability..."
HEADERS=("$SYSROOT/usr/include/math.h" "$SYSROOT/usr/include/pthread.h")
for h in "${HEADERS[@]}"; do
    if [ -f "$h" ]; then
        echo "    [OK] $h"
    else
        echo "    [MISS] $h"
    fi
done
echo ""

echo "[*] Cross-compiling with math and pthread..."
arm-linux-gnueabihf-gcc -Wall -O2 -o math_app main.c -lm -lpthread
echo "    Compilation successful!"
echo ""

echo "[*] Verifying architecture..."
file math_app
echo ""

echo "[*] Checking dynamic library dependencies..."
arm-linux-gnueabihf-readelf -d math_app | grep NEEDED
echo ""

echo "[*] Done."
echo ""
echo "To run with QEMU:"
echo "  QEMU_LD_PREFIX=$SYSROOT qemu-arm-static ./math_app 100"
