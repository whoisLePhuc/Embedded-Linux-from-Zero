#!/bin/bash
set -e

echo "[*] Detecting OS..."
if [ -f /etc/os-release ]; then
    . /etc/os-release
    echo "[*] OS: $NAME $VERSION"
else
    echo "[-] Cannot detect OS"
    exit 1
fi

echo "[*] Installing ARM cross-compiler..."
sudo apt update
sudo apt install -y gcc-arm-linux-gnueabihf binutils-arm-linux-gnueabihf

echo "[*] Verifying installation..."
arm-linux-gnueabihf-gcc --version

echo "[*] Creating test program..."
cat > test.c << 'EOF'
#include <stdio.h>

int main() {
    printf("Hello from ARM!\n");
    return 0;
}
EOF

echo "[*] Cross-compiling..."
arm-linux-gnueabihf-gcc -Wall -O2 -o test test.c

echo ""
echo "[*] Cross-compilation successful!"
echo "[*] Running verification scripts..."
