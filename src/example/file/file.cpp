#include <iostream>
#include <fstream>
#include <random>
#include "file.h"
#include "codec.h"

void create_random_file(const std::string& filepath, size_t size_in_bytes) {
    std::ofstream file(filepath, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to create file: " << filepath << std::endl;
        return;
    }

    // 使用随机数生成器生成随机数据
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned int> dist(0, 255); // 改用 unsigned int

    for (size_t i = 0; i < size_in_bytes; ++i) {
        unsigned char random_byte = static_cast<unsigned char>(dist(gen)); // 显式转换
        file.put(static_cast<char>(random_byte));
    }

    file.close();
    std::cout << "Random file created: " << filepath << " (" << size_in_bytes << " bytes)" << std::endl;
}

int main() {
    std::string filepath = "random_file.bin"; // 随机文件路径
    size_t file_size = 1024; // 文件大小（字节）

    // 创建随机文件
    create_random_file(filepath, file_size);

    // 检查文件是否存在
    if (!wlib::file::exist(filepath)) {
        std::cerr << "File does not exist: " << filepath << std::endl;
        return 1;
    }

    // 读取文件内容
    wlib::bytearray fileContent = wlib::file::read(filepath);
    if (fileContent.size() == 0) {
        std::cerr << "Failed to read file: " << filepath << std::endl;
        return 1;
    }

    // 计算 MD5
    std::string md5Hash = wlib::codec::md5(fileContent.data(), fileContent.size());
    if (md5Hash.empty()) {
        std::cerr << "Failed to compute MD5 hash." << std::endl;
        return 1;
    }
    std::cout << "MD5 of file " << filepath << ": " << md5Hash << std::endl;

    return 0;
}