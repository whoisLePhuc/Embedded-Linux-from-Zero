#!/bin/bash
# Exercise 02 - Create and add a custom Yocto layer (meta-embedded)
# Run this from the Poky build directory.

set -e

LAYER_NAME="meta-embedded"
POKY_DIR="/path/to/poky"
BUILD_DIR="build"

echo "=== Step 1: Creating custom layer ==="
cd "$POKY_DIR"
source oe-init-build-env "$BUILD_DIR"

bitbake-layers create-layer "$LAYER_NAME"
echo "Layer $LAYER_NAME created successfully."

echo ""
echo "=== Step 2: Verifying layer structure ==="
ls -la "$LAYER_NAME/"
ls -la "$LAYER_NAME/conf/"

echo ""
echo "=== Step 3: Adding layer to build ==="
bitbake-layers add-layer "$LAYER_NAME"

echo ""
echo "=== Step 4: Verifying layer is loaded ==="
bitbake-layers show-layers

echo ""
echo "=== Custom layer setup complete! ==="
echo "Layer location: $POKY_DIR/$LAYER_NAME"
echo "To remove the layer: bitbake-layers remove-layer $LAYER_NAME"
