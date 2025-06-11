#include "codec.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <sstream>
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

    std::wstring codec::utf8_to_unicode(const std::string &utf8_string)
    {
#if defined(WIN32)
        int len = MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), -1, NULL, 0);
        if (len == 0)
        {
            return L"";
        }
        std::wstring unicode_string(len, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), -1, &unicode_string[0], len);
        unicode_string.resize(len - 1); // 去掉末尾的空字符
        return unicode_string;
#else
        // 使用 utf8cpp 将 UTF-8 转换为 Unicode（std::wstring）
        std::wstring unicode_string;
        try
        {
            utf8::utf8to32(utf8_string.begin(), utf8_string.end(), std::back_inserter(unicode_string));
        }
        catch (const std::exception &e)
        {
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

} // namespace wlib