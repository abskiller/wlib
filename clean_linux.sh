#!/bin/bash

# 设置变量
BUILD_DIR="build"
BIN_DIR="bin"
LIB_DIR="lib"
CMAKE_CACHE="CMakeCache.txt"

# 删除构建目录
if [ -d "$BUILD_DIR" ]; then
    echo "删除构建目录 $BUILD_DIR..."
    rm -rf "$BUILD_DIR"
else
    echo "构建目录 $BUILD_DIR 不存在，跳过删除。"
fi

# 删除二进制输出目录
if [ -d "$BIN_DIR" ]; then
    echo "删除二进制目录 $BIN_DIR..."
    rm -rf "$BIN_DIR"
else
    echo "二进制目录 $BIN_DIR 不存在，跳过删除。"
fi

# 删除库输出目录
if [ -d "$LIB_DIR" ]; then
    echo "删除库目录 $LIB_DIR..."
    rm -rf "$LIB_DIR"
else
    echo "库目录 $LIB_DIR 不存在，跳过删除。"
fi

# 删除 CMake 缓存文件
if [ -f "$CMAKE_CACHE" ]; then
    echo "删除 CMake 缓存文件 $CMAKE_CACHE..."
    rm -f "$CMAKE_CACHE"
else
    echo "CMake 缓存文件 $CMAKE_CACHE 不存在，跳过删除。"
fi

# 删除其他临时文件（可根据需要添加）
echo "清理完成！"