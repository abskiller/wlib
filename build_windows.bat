@echo off
:: 设置控制台编码为 UTF-8
chcp 65001 >nul

:: 设置变量
set BUILD_DIR=build
set GENERATOR="Visual Studio 15 2017"
set ARCH=x64

:: 删除旧的构建目录（清理缓存）
if exist %BUILD_DIR% (
    echo 删除旧的构建目录...
    rmdir /s /q %BUILD_DIR%
)

:: 创建新的构建目录
echo 创建新的构建目录...
mkdir %BUILD_DIR%

:: 进入构建目录
cd %BUILD_DIR%

:: 配置 Debug 版本
echo 配置 Debug 版本...
cmake -G %GENERATOR% -A %ARCH% -DCMAKE_BUILD_TYPE=Debug ..
if %errorlevel% neq 0 (
    echo 配置 Debug 版本失败！
    exit /b %errorlevel%
)

:: 构建 Debug 版本
echo 构建 Debug 版本...
cmake --build . --config Debug
if %errorlevel% neq 0 (
    echo 构建 Debug 版本失败！
    exit /b %errorlevel%
)

:: 配置 Release 版本
echo 配置 Release 版本...
cmake -G %GENERATOR% -A %ARCH% -DCMAKE_BUILD_TYPE=Release ..
if %errorlevel% neq 0 (
    echo 配置 Release 版本失败！
    exit /b %errorlevel%
)

:: 构建 Release 版本
echo 构建 Release 版本...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo 构建 Release 版本失败！
    exit /b %errorlevel%
)

:: 返回上级目录
cd ..
echo Windows 构建完成！