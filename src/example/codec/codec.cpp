#include <iostream>
#include "codec.h"

#if defined(_WIN32)
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif

void set_console_output() {
#if defined(_WIN32)
    // 设置控制台为 UTF-8 编码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::wcout.imbue(std::locale("en_US.UTF-8"));
    std::cout << "Console output encoding set to UTF-8." << std::endl;
#endif
}

int main() {
    set_console_output(); // 初始化控制台为 UTF-8 编码

    try {
        // 创建 codec 对象
        wlib::codec codec;

        // 示例字符串
        std::string utf8_str = u8"Hello, 世界!";
        std::wstring unicode_str = L"Hello, 世界!";
        std::string ansi_str;

        // 测试 utf8_to_unicode
        try {
            std::cout << "Testing utf8_to_unicode..." << std::endl;
            std::wstring unicode_result = codec.utf8_to_unicode(utf8_str);
            std::wcout << L"UTF-8 to Unicode: " << unicode_result << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Error in utf8_to_unicode: " << e.what() << std::endl;
        }

        // 测试 unicode_to_utf8
        try {
            std::cout << "\nTesting unicode_to_utf8..." << std::endl;
            std::string utf8_result = codec.unicode_to_utf8(unicode_str);
            std::cout << "Unicode to UTF-8: " << utf8_result << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Error in unicode_to_utf8: " << e.what() << std::endl;
        }

        // 测试 ansi_to_unicode
        try {
            std::cout << "\nTesting ansi_to_unicode..." << std::endl;
            ansi_str = codec.unicode_to_ansi(unicode_str); // 先将 Unicode 转换为 ANSI
            std::wstring unicode_from_ansi = codec.ansi_to_unicode(ansi_str);
            std::wcout << L"ANSI to Unicode: " << unicode_from_ansi << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Error in ansi_to_unicode: " << e.what() << std::endl;
        }

        // 测试 unicode_to_ansi
        try {
            std::cout << "\nTesting unicode_to_ansi..." << std::endl;
            ansi_str = codec.unicode_to_ansi(unicode_str);
            std::cout << "Unicode to ANSI: " << codec.unicode_to_utf8(codec.ansi_to_unicode(ansi_str)) << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Error in unicode_to_ansi: " << e.what() << std::endl;
        }

        // 测试 ansi_to_utf8
        try {
            std::cout << "\nTesting ansi_to_utf8..." << std::endl;
            std::string utf8_from_ansi = codec.ansi_to_utf8(ansi_str);
            std::cout << "ANSI to UTF-8: " << utf8_from_ansi << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Error in ansi_to_utf8: " << e.what() << std::endl;
        }

        // 测试 utf8_to_ansi
        try {
            std::cout << "\nTesting utf8_to_ansi..." << std::endl;
            std::string ansi_from_utf8 = codec.utf8_to_ansi(utf8_str);
            std::cout << "UTF-8 to ANSI: " << codec.unicode_to_utf8(codec.ansi_to_unicode(ansi_from_utf8)) << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Error in utf8_to_ansi: " << e.what() << std::endl;
        }

        // 测试 url_encode 和 url_decode
        try {
            std::cout << "\nTesting url_encode and url_decode..." << std::endl;
            std::string url_encoded = codec.url_encode(utf8_str);
            std::string url_decoded = codec.url_decode(url_encoded);
            std::cout << "URL Encoded: " << url_encoded << std::endl;
            std::cout << "URL Decoded: " << url_decoded << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Error in url_encode/url_decode: " << e.what() << std::endl;
        }

        // 测试 base64_encode 和 base64_decode
        try {
            std::cout << "\nTesting base64_encode and base64_decode..." << std::endl;
            std::string base64_encoded = codec.base64_encode(utf8_str);
            std::string base64_decoded = codec.base64_decode(base64_encoded);
            std::cout << "Base64 Encoded: " << base64_encoded << std::endl;
            std::cout << "Base64 Decoded: " << base64_decoded << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Error in base64_encode/base64_decode: " << e.what() << std::endl;
        }

        // 测试 md5
        try {
            std::cout << "\nTesting md5..." << std::endl;
            std::string md5_hash = codec.md5(utf8_str);
            std::cout << "MD5 Hash: " << md5_hash << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Error in md5: " << e.what() << std::endl;
        }

        // 测试 hex_to_dec 和 dec_to_hex
        try {
            std::cout << "\nTesting hex_to_dec and dec_to_hex..." << std::endl;
            std::string hex_value = "1A3F4B5E6F";
            int decimal_value = codec.hex_to_dec(hex_value);
            std::string hex_converted_back = codec.dec_to_hex(decimal_value);
            std::cout << "Hex to Decimal: " << decimal_value << std::endl;
            std::cout << "Decimal to Hex: " << hex_converted_back << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Error in hex_to_dec/dec_to_hex: " << e.what() << std::endl;
        }

    } catch (const std::exception &e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }

    return 0;
}