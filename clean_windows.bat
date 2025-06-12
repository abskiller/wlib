@echo off
:: 设置控制台编码为 UTF-8
chcp 65001 >nul

:: 设置变量
set BUILD_DIR=build
set BIN_DIR=bin
set LIB_DIR=lib
set CMAKE_CACHE=CMakeCache.txt

:: 删除构建目录
if exist %BUILD_DIR% (
    echo 删除构建目录 %BUILD_DIR%...
    rmdir /s /q %BUILD_DIR%
) else (
    echo 构建目录 %BUILD_DIR% 不存在，跳过删除。
)

:: 删除二进制输出目录
if exist %BIN_DIR% (
    echo 删除二进制目录 %BIN_DIR%...
    rmdir /s /q %BIN_DIR%
) else (
    echo 二进制目录 %BIN_DIR% 不存在，跳过删除。
)

:: 删除库输出目录
if exist %LIB_DIR% (
    echo 删除库目录 %LIB_DIR%...
    rmdir /s /q %LIB_DIR%
) else (
    echo 库目录 %LIB_DIR% 不存在，跳过删除。
)

:: 删除 CMake 缓存文件
if exist %CMAKE_CACHE% (
    echo 删除 CMake 缓存文件 %CMAKE_CACHE%...
    del /q %CMAKE_CACHE%
) else (
    echo CMake 缓存文件 %CMAKE_CACHE% 不存在，跳过删除。
)

:: 删除其他临时文件（可根据需要添加）
echo 清理完成！