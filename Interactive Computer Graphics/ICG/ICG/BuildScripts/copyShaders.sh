#!/bin/bash
rm "$BUILD_DIR/shader.*"
cp "$SRCROOT/ICG/Shaders/shader.vert" "$BUILD_DIR"
cp "$SRCROOT/ICG/Shaders/shader.frag" "$BUILD_DIR"
