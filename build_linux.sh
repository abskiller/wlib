#!/bin/bash

# 设置变量
BUILD_DIR="build"

# 创建构建目录
if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

# 进入构建目录
cd "$BUILD_DIR"

# 配置 Debug 版本
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
if [ $? -ne 0 ]; then
    echo "配置 Debug 版本失败！"
    exit 1
fi

# 构建 Debug 版本
make
if [ $? -ne 0 ]; then
    echo "构建 Debug 版本失败！"
    exit 1
fi

# 配置 Release 版本
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
if [ $? -ne 0 ]; then
    echo "配置 Release 版本失败！"
    exit 1
fi

# 构建 Release 版本
make
if [ $? -ne 0 ]; then
    echo "构建 Release 版本失败！"
    exit 1
fi

# 返回上级目录
cd ..
echo "Linux 构建完成！"