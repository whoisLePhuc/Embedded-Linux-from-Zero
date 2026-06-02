#!/bin/bash
set -e

if [ ! -f test ]; then
    echo "[-] test binary not found. Run setup_toolchain.sh first."
    exit 1
fi

echo "[*] ELF Architecture Verification"
echo "--------------------------------"

echo "1. file command:"
file test
echo ""

echo "2. readelf header:"
arm-linux-gnueabihf-readelf -h test
echo ""

echo "3. Checking for ARM architecture..."
if file test | grep -qi "ARM"; then
    echo "[PASS] Binary is ARM architecture"
else
    echo "[FAIL] Binary is NOT ARM architecture"
    exit 1
fi
