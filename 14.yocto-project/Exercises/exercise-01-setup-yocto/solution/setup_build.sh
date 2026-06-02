#!/bin/bash
# Exercise 01 - Setup Yocto Build Environment
# This script clones Poky (scarthgap), installs dependencies,
# initializes the build environment, and builds core-image-minimal for qemuarm64.

set -e

POKY_BRANCH="scarthgap"
POKY_DIR="poky"
BUILD_DIR="build"
MACHINE="qemuarm64"
TARGET_IMAGE="core-image-minimal"

echo "=== Step 1: Installing host dependencies ==="
sudo apt update
sudo apt install -y gawk wget git diffstat unzip texinfo \
    gcc build-essential chrpath socat cpio python3 \
    python3-pip python3-pexpect xz-utils debianutils \
    iputils-ping python3-git python3-jinja2 libegl1-mesa \
    libsdl1.2-dev python3-subunit mesa-common-dev \
    zstd liblz4-tool file locales libacl1 \
    libssl-dev python3-pyelftools

echo "=== Step 2: Cloning Poky ($POKY_BRANCH) ==="
if [ ! -d "$POKY_DIR" ]; then
    git clone --depth=1 git://git.yoctoproject.org/poky -b "$POKY_BRANCH"
else
    echo "Poky directory already exists, skipping clone."
fi

echo "=== Step 3: Initializing build environment ==="
cd "$POKY_DIR"
source oe-init-build-env "$BUILD_DIR"

echo "=== Step 4: Configuring MACHINE ==="
sed -i "s/^MACHINE ?= \"qemux86-64\"/MACHINE ?= \"$MACHINE\"/" conf/local.conf
grep "^MACHINE" conf/local.conf

echo "=== Step 5: Building $TARGET_IMAGE ==="
echo "NOTE: First build may take 1-6 hours depending on hardware."
bitbake "$TARGET_IMAGE"

echo "=== Step 6: Build directory structure ==="
echo ""
echo "Build directory contents:"
ls -la "$BUILDDIR"
echo ""
echo "Images available:"
ls -lh "$BUILDDIR/tmp/deploy/images/$MACHINE/" | grep "$TARGET_IMAGE"

echo ""
echo "=== Build completed successfully! ==="
echo "To run the image in QEMU:"
echo "  cd $POKY_DIR/$BUILD_DIR"
echo "  runqemu $MACHINE nographic"
