#ifndef _BYTEARRAY_HEAD_
#define _BYTEARRAY_HEAD_
#include <vector>
#include <string.h>
#include <sstream>
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include "wlib.h"


namespace wlib
{
    class bytearray
    {
    public:
        // 默认构造
        bytearray();
        // 拷贝构造
        bytearray(const bytearray &data);
        // 字节构造
        bytearray(const char *data, size_t size);
        // Ansi字符串构造
        bytearray(const char *data);
        // Unicode字符串构造
        bytearray(const wchar_t *data);
        // 根据长度构造
        bytearray(size_t size, char c = 0);
        // 根据std::string构造
        bytearray(const std::string &data);
        // 根据Wstd::string构造
        bytearray(const std::wstring &data);
        // 析构函数
        ~bytearray();

    public:
        bytearray &operator=(const bytearray &value);
        bytearray &operator+=(const bytearray &value);
        bytearray operator+(const bytearray &value) const;
        char operator[](const size_t i) const;
        const bool operator==(const bytearray &value) const;
        const bool operator!=(const bytearray &value) const;
        // 重载<<运算符，用于输出bytearray到ostream
        friend std::ostream &operator<<(std::ostream &os, const bytearray &ba)
        {
            for (size_t i = 0; i < ba.m_size; i++)
            {
                os << std::hex << (char)ba.m_buffer[i];
            }
            return os;
        }
        // 重载>>运算符，用于从istream读取数据到bytearray
        friend std::istream &operator>>(std::istream &is, bytearray &ba)
        {
            std::string line;
            std::getline(is, line); // 读取一行输入
            std::istringstream iss(line);
            char value;
            ba.clear(); // 清空当前的bytearray
            while (iss >> std::hex >> value)
            {
                ba.append(value); // 将读取的值添加到bytearray
            }
            return is;
        }
        operator std::string() const;

    public:
        bytearray &append(const bytearray &value);
        bytearray &append(const char *data, size_t size);
        bytearray &append(const char *data);
        bytearray &append(const wchar_t *data);
        bytearray &append(const bool data);
        bytearray &append(const char data);
        bytearray &append(const wchar_t data);
        bytearray &append(const short data);
        bytearray &append(const int32_t data);
        bytearray &append(const int64_t data);
        bytearray &append(const unsigned char data);
        bytearray &append(const unsigned short data);
        bytearray &append(const uint32_t data);
        bytearray &append(const uint64_t data);

        bytearray &prepend(const bytearray &value);
        bytearray &prepend(const char *data, size_t size);
        bytearray &prepend(const char *data);
        bytearray &prepend(const wchar_t *data);
        bytearray &prepend(const bool data);
        bytearray &prepend(const char data);
        bytearray &prepend(const wchar_t data);
        bytearray &prepend(const short data);
        bytearray &prepend(const int32_t data);
        bytearray &prepend(const int64_t data);
        bytearray &prepend(const unsigned char data);
        bytearray &prepend(const unsigned short data);
        bytearray &prepend(const uint32_t data);
        bytearray &prepend(const uint64_t data);

        bytearray left(const size_t &index, bool include = false) const;
        bytearray left(const bytearray &data, bool include = false) const;
        bytearray right(const size_t index, bool include = false) const;
        bytearray right(const bytearray &data, bool include = false) const;
        bytearray mid(const size_t &startIndex, const size_t &endIndex, bool include = false) const;
        bytearray mid(const bytearray &start, const bytearray &end, bool include = false) const;

        bool indexOf(const char c, size_t &index, size_t startIndex = 0) const;
        bool indexOf(const bytearray &data, size_t &index, size_t startIndex = 0) const;
        bool lastIndexOf(const char c, size_t &index, size_t startIndex = 0) const;
        bool lastIndexOf(const bytearray &data, size_t &index, size_t startIndex = 0) const;

        bytearray repalce(const bytearray &src, const bytearray &dest, const bool &once = false) const;
        std::vector<bytearray> split(const bytearray &separator) const;

        std::string toHex(char separator = 0) const;

    public:
        static bytearray fromHex(std::string hex, char separator = 0);

    public:
        void resize(size_t size);
        void resize(char padding, size_t size);
        const size_t size() const;
        const size_t length() const;
        char *data() const;
        void clear();

    private:
        char *m_buffer;
        size_t m_size;
    };

}
#endif