#include "codec.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <iomanip> // std::setfill 和 std::setw
#include <utf8.h> // 使用 utf8cpp

#if defined(WIN32)
#include <Windows.h>
#endif

namespace wlib
{

    codec::codec() {}

    char codec::lower(char c)
    {
        if (c >= 'A' && c <= 'Z')
        {
            return c - 'A' + 'a';
        }
        return c;
    }

    char codec::upper(char c)
    {
        if (c >= 'a' && c <= 'z')
        {
            return c - 'a' + 'A';
        }
        return c;
    }

    wchar_t codec::lowerw(wchar_t wc)
    {
        if (wc >= L'A' && wc <= L'Z')
        {
            return wc - L'A' + L'a';
        }
        return wc;
    }

    wchar_t codec::upperw(wchar_t wc)
    {
        if (wc >= L'a' && wc <= L'z')
        {
            return wc - L'a' + L'A';
        }
        return wc;
    }

    std::wstring codec::utf8_to_unicode(const std::string &utf8_string) {
        #if defined(WIN32)
            // 获取转换所需的缓冲区大小
            int len = MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), -1, NULL, 0);
            if (len == 0) {
                DWORD error = GetLastError();
                throw std::runtime_error("MultiByteToWideChar failed: Unable to convert UTF-8 to Unicode. Error code: " + std::to_string(error));
            }
        
            // 分配缓冲区并进行转换
            std::wstring unicode_string(len, L'\0');
            if (MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), -1, &unicode_string[0], len) == 0) {
                DWORD error = GetLastError();
                throw std::runtime_error("MultiByteToWideChar failed during conversion. Error code: " + std::to_string(error));
            }
        
            // 去掉末尾的空字符
            unicode_string.resize(len - 1);
            return unicode_string;
        #else
            // 使用 utf8cpp 将 UTF-8 转换为 Unicode（std::wstring）
            std::wstring unicode_string;
            if (!is_valid_utf8(utf8_string)) {
                throw std::runtime_error("Invalid UTF-8 string");
            }
            try {
                utf8::utf8to16(utf8_string.begin(), utf8_string.end(), std::back_inserter(unicode_string));
            } catch (const std::exception &e) {
                throw std::runtime_error("Invalid UTF-8 sequence: " + std::string(e.what()));
            }
            return unicode_string;
        #endif
        }

    std::string codec::unicode_to_utf8(const std::wstring &unicode_string) {
        #if defined(WIN32)
            // 获取转换所需的缓冲区大小
            int len = WideCharToMultiByte(CP_UTF8, 0, unicode_string.c_str(), -1, NULL, 0, NULL, NULL);
            if (len == 0) {
                throw std::runtime_error("WideCharToMultiByte failed: Unable to convert Unicode to UTF-8.");
            }
        
            // 分配缓冲区并进行转换
            std::string utf8_string(len, '\0');
            WideCharToMultiByte(CP_UTF8, 0, unicode_string.c_str(), -1, &utf8_string[0], len, NULL, NULL);
        
            // 去掉末尾的空字符
            utf8_string.resize(len - 1);
            return utf8_string;
        #else
            // 非 Windows 平台的实现
            std::string utf8_string;
            for (wchar_t wc : unicode_string) {
                if (wc <= 0x7F) {
                    utf8_string.push_back(static_cast<char>(wc));
                } else if (wc <= 0x7FF) {
                    utf8_string.push_back(static_cast<char>(0xC0 | ((wc >> 6) & 0x1F)));
                    utf8_string.push_back(static_cast<char>(0x80 | (wc & 0x3F)));
                } else {
                    utf8_string.push_back(static_cast<char>(0xE0 | ((wc >> 12) & 0x0F)));
                    utf8_string.push_back(static_cast<char>(0x80 | ((wc >> 6) & 0x3F)));
                    utf8_string.push_back(static_cast<char>(0x80 | (wc & 0x3F)));
                }
            }
            return utf8_string;
        #endif
        }

        std::wstring codec::ansi_to_unicode(const std::string &ansi_string) {
            #if defined(WIN32)
                // 获取转换所需的缓冲区大小
                int len = MultiByteToWideChar(CP_ACP, 0, ansi_string.c_str(), -1, NULL, 0);
                if (len == 0) {
                    throw std::runtime_error("MultiByteToWideChar failed: Unable to convert ANSI to Unicode.");
                }
            
                // 分配缓冲区并进行转换
                std::wstring unicode_string(len, L'\0');
                MultiByteToWideChar(CP_ACP, 0, ansi_string.c_str(), -1, &unicode_string[0], len);
            
                // 去掉末尾的空字符
                unicode_string.resize(len - 1);
                return unicode_string;
            #else
                // 非 Windows 平台的实现
                std::wstring unicode_string;
                for (unsigned char c : ansi_string) {
                    unicode_string.push_back(static_cast<wchar_t>(c));
                }
                return unicode_string;
            #endif
            }

    std::string codec::unicode_to_ansi(const std::wstring &unicode_string) {
        #if defined(WIN32)
            // 获取转换所需的缓冲区大小
            int len = WideCharToMultiByte(CP_ACP, 0, unicode_string.c_str(), -1, NULL, 0, NULL, NULL);
            if (len == 0) {
                DWORD error = GetLastError();
                throw std::runtime_error("WideCharToMultiByte failed: Unable to convert Unicode to ANSI. Error code: " + std::to_string(error));
            }
        
            // 分配缓冲区并进行转换
            std::string ansi_string(len, '\0');
            if (WideCharToMultiByte(CP_ACP, 0, unicode_string.c_str(), -1, &ansi_string[0], len, NULL, NULL) == 0) {
                DWORD error = GetLastError();
                throw std::runtime_error("WideCharToMultiByte failed during conversion. Error code: " + std::to_string(error));
            }
        
            // 去掉末尾的空字符
            ansi_string.resize(len - 1);
            return ansi_string;
        #else
            // 非 Windows 平台假设 Unicode 转换为 ISO-8859-1
            std::string ansi_string;
            for (wchar_t wc : unicode_string) {
                if (wc > 255) {
                    std::cerr << "Warning: Unicode character " << wc << " cannot be represented in ANSI." << std::endl;
                    ansi_string.push_back('?'); // 替换无法转换的字符
                } else {
                    ansi_string.push_back(static_cast<char>(wc));
                }
            }
            return ansi_string;
        #endif
        }
    
        std::string codec::ansi_to_utf8(const std::string &ansi_string) {
            // 先将 ANSI 转换为 Unicode
            std::wstring unicode_string = ansi_to_unicode(ansi_string);
        
            // 再将 Unicode 转换为 UTF-8
            return unicode_to_utf8(unicode_string);
        }

    std::string codec::utf8_to_ansi(const std::string &utf8_string)
    {
        return unicode_to_ansi(utf8_to_unicode(utf8_string));
    }


    std::string codec::url_encode(const std::string &str) {
        const char hex[] = "0123456789ABCDEF"; // 十六进制字符表
        std::string encoded;
    
        for (unsigned char c : str) {
            if (isalnum(c) || c == '.' || c == '_' || c == '-' || c == '*') {
                // 保留字母、数字和安全字符
                encoded += c;
            } else {
                // 其他字符进行百分号编码
                encoded += '%';
                encoded += hex[c >> 4]; // 高 4 位
                encoded += hex[c & 0x0F]; // 低 4 位
            }
        }
    
        return encoded;
    }

    std::string codec::url_decode(const std::string &str) {
        std::string result;
        for (size_t i = 0; i < str.length(); ++i) {
            if (str[i] == '%' && i + 2 < str.length() && isxdigit(str[i + 1]) && isxdigit(str[i + 2])) {
                std::string hexStr = str.substr(i + 1, 2);
                char decoded_char = static_cast<char>(std::stoi(hexStr, nullptr, 16));
                result += decoded_char;
                i += 2;
            } else if (str[i] == '+') {
                result += ' ';
            } else {
                result += str[i];
            }
        }
        return result;
    }

    std::string codec::base64_encode(const std::string &str) {
        // Base64 字符表
        const char BASE64_CHARS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                    "abcdefghijklmnopqrstuvwxyz"
                                    "0123456789+/";

        std::string encoded;
        size_t len = str.size();
        int val = 0, valb = -6;

        for (unsigned char c : str) {
            val = (val << 8) + c;
            valb += 8;
            while (valb >= 0) {
                encoded.push_back(BASE64_CHARS[(val >> valb) & 0x3F]);
                valb -= 6;
            }
        }

        if (valb > -6) {
            encoded.push_back(BASE64_CHARS[((val << 8) >> (valb + 8)) & 0x3F]);
        }

        while (encoded.size() % 4) {
            encoded.push_back('=');
        }

        return encoded;
    }

    std::string codec::base64_decode(const std::string &str) {
        // Base64 解码表
        const int DECODE_TABLE[256] = {
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0-15
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 16-31
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, // 32-47
            52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, // 48-63
            -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,           // 64-79
            15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, // 80-95
            -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, // 96-111
            41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1, // 112-127
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 128-143
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 144-159
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 160-175
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 176-191
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 192-207
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 208-223
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 224-239
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1  // 240-255
        };

        std::string decoded;
        int val = 0, valb = -8;

        for (unsigned char c : str) {
            if (DECODE_TABLE[c] == -1) {
                if (c == '=') break; // Padding character
                throw std::invalid_argument("Invalid Base64 character");
            }
            val = (val << 6) + DECODE_TABLE[c];
            valb += 6;
            if (valb >= 0) {
                decoded.push_back((val >> valb) & 0xFF);
                valb -= 8;
            }
        }

        return decoded;
    }



    std::string codec::md5(const char* data, size_t length) {
        // 定义 MD5 常量
        const uint32_t INIT_STATE[4] = {
            0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476
        };
    
        const uint32_t S[64] = {
            7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
            5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
            4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
            6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
        };
    
        const uint32_t K[64] = {
            0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
            0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
            0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
            0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
            0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
            0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
            0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
            0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
            0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
            0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
            0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
            0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
            0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
            0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
            0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
            0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
        };
    
        // 初始化 MD5 状态
        uint32_t state[4];
        std::memcpy(state, INIT_STATE, sizeof(state));
    
        // 预处理输入
        std::vector<uint8_t> dataVec(data, data + length);
        size_t original_size = dataVec.size();
        dataVec.push_back(0x80);
    
        while ((dataVec.size() % 64) != 56) {
            dataVec.push_back(0x00);
        }
    
        uint64_t bit_length = original_size * 8;
        for (int i = 0; i < 8; ++i) {
            dataVec.push_back(static_cast<uint8_t>(bit_length >> (i * 8)));
        }
    
        // 主循环
        for (size_t i = 0; i < dataVec.size(); i += 64) {
            uint32_t M[16];
            for (int j = 0; j < 16; ++j) {
                M[j] = (dataVec[i + j * 4 + 0] << 0) |
                       (dataVec[i + j * 4 + 1] << 8) |
                       (dataVec[i + j * 4 + 2] << 16) |
                       (dataVec[i + j * 4 + 3] << 24);
            }
    
            uint32_t a = state[0];
            uint32_t b = state[1];
            uint32_t c = state[2];
            uint32_t d = state[3];
    
            for (int j = 0; j < 64; ++j) {
                uint32_t F, g;
                if (j < 16) {
                    F = (b & c) | (~b & d);
                    g = j;
                } else if (j < 32) {
                    F = (d & b) | (~d & c);
                    g = (5 * j + 1) % 16;
                } else if (j < 48) {
                    F = b ^ c ^ d;
                    g = (3 * j + 5) % 16;
                } else {
                    F = c ^ (b | ~d);
                    g = (7 * j) % 16;
                }
    
                F = F + a + K[j] + M[g];
                a = d;
                d = c;
                c = b;
                b = b + ((F << S[j]) | (F >> (32 - S[j])));
            }
    
            state[0] += a;
            state[1] += b;
            state[2] += c;
            state[3] += d;
        }
    
        // 输出结果
        std::ostringstream result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result << std::hex << std::setw(2) << std::setfill('0')
                       << ((state[i] >> (j * 8)) & 0xFF);
            }
        }
    
        return result.str();
    }
    
    std::string codec::md5(const std::string &str) {
        return md5(str.c_str(), str.length());
    }

    unsigned long long codec::hex_to_dec(const std::string &hex) {
        size_t startIndex = 0;
    
        // 跳过 '#' 或 '0x' 前缀
        if ((hex.size() >= 2 && (hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X'))) ||
            (hex.size() >= 1 && hex[0] == '#')) {
            startIndex = (hex[0] == '#' ? 1 : 2);
        }
    
        if (startIndex >= hex.size()) {
            return 0; // 返回 0 表示无效的十六进制字符串
        }
    
        unsigned long long dec = 0;
        unsigned long long base = 1;
    
        // 从后向前遍历字符串
        for (size_t i = hex.size(); i > startIndex; --i) {
            char c = hex[i - 1];
            if (!::isxdigit(c)) {
                return 0; // 返回 0 表示包含非十六进制字符
            }
            unsigned long long digitValue = ::tolower(c) - (c <= '9' ? '0' : ('a' - 10));
            dec += digitValue * base;
            base *= 16;
        }
    
        return dec;
    }

    std::string codec::dec_to_hex(const unsigned long long dec) {
        if (dec == 0) {
            return "0";
        }
    
        std::string hexString;
        const char hexDigits[] = "0123456789abcdef";
    
        unsigned long long uDec = dec;
        while (uDec > 0) {
            hexString.insert(hexString.begin(), hexDigits[uDec % 16]);
            uDec /= 16;
        }
    
        return hexString;
    }


} // namespace wlib