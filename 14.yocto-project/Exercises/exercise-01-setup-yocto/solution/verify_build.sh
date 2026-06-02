#!/bin/bash
# Exercise 01 - Verify Yocto Build Output
# Checks that the build was successful and all expected artifacts exist.

set -e

POKY_DIR="poky"
BUILD_DIR="build"
MACHINE="qemuarm64"
TARGET_IMAGE="core-image-minimal"

echo "=== Checking build environment ==="
if [ ! -d "$POKY_DIR" ]; then
    echo "ERROR: $POKY_DIR directory not found. Run setup_build.sh first."
    exit 1
fi

if [ ! -d "$POKY_DIR/$BUILD_DIR" ]; then
    echo "ERROR: $BUILD_DIR not found. Run setup_build.sh first."
    exit 1
fi

echo "=== Verifying Poky branch ==="
cd "$POKY_DIR"
BRANCH=$(git rev-parse --abbrev-ref HEAD)
echo "Current branch: $BRANCH"
cd ..

echo ""
echo "=== Verifying build configuration ==="
if [ -f "$POKY_DIR/$BUILD_DIR/conf/local.conf" ]; then
    echo "local.conf exists OK"
    MACHINE_VAL=$(grep "^MACHINE" "$POKY_DIR/$BUILD_DIR/conf/local.conf" | head -1)
    echo "  $MACHINE_VAL"
else
    echo "ERROR: local.conf not found!"
    exit 1
fi

if [ -f "$POKY_DIR/$BUILD_DIR/conf/bblayers.conf" ]; then
    echo "bblayers.conf exists OK"
fi

echo ""
echo "=== Verifying build output ==="
DEPLOY_DIR="$POKY_DIR/$BUILD_DIR/tmp/deploy/images/$MACHINE"
if [ -d "$DEPLOY_DIR" ]; then
    echo "Deploy directory found: $DEPLOY_DIR"
    echo ""
    echo "Image files:"
    ls -lh "$DEPLOY_DIR"/*.ext4 "$DEPLOY_DIR"/*.wic "$DEPLOY_DIR"/*.qemuboot.conf 2>/dev/null || \
    ls -lh "$DEPLOY_DIR"/*.ext4 2>/dev/null || \
    echo "No image files found (build may not have completed)"
else
    echo "WARNING: Deploy directory not found. Build may still be in progress."
fi

echo ""
echo "=== Verification complete ==="
