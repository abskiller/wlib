// #include "codec.h"

// #include <iostream>
// #include <stdio.h>
// #include <string>
// #include <vector>

// // #include "base64.h"
// // #include "md5.h"
// // #include <wstring_convert>

// // #include "utfcpp/source/utf8.h"

// #if defined(WIN32)
// #include <Windows.h>
// #elif defined(__unix__)
// #else
// #endif

// namespace wlib
// {

// 	codec::codec()
// 	{
// 	}

// 	char codec::lower(char c)
// 	{
// 		if (c >= 'A' && c <= 'Z')
// 		{
// 			return c - 'A' + 'a';
// 		}
// 		return c;
// 	}

// 	char codec::upper(char c)
// 	{
// 		if (c >= 'a' && c <= 'z')
// 		{
// 			return c - 'a' + 'A';
// 		}
// 		return c;
// 	}

// 	wchar_t codec::lowerw(wchar_t wc)
// 	{
// 		if (wc >= L'A' && wc <= L'Z')
// 		{
// 			return wc - L'A' + L'a';
// 		}
// 		return wc;
// 	}

// 	wchar_t codec::upperw(wchar_t wc)
// 	{
// 		if (wc >= L'a' && wc <= L'z')
// 		{
// 			return wc - L'a' + L'A';
// 		}
// 		return wc;
// 	}
// 	std::wstring codec::utf8_to_unicode(const std::string &utf8_string)
// 	{
// #if defined(WIN32)
// 		int len = MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), -1, NULL, 0);
// 		if(len == 0)
// 		{
// 			return L"";
// 		}
// 		wchar_t* buffer = new wchar_t[len];
// 		if(!buffer)
// 		{
// 			return L"";
// 		}
// 		MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), -1, buffer, len);
// 		std::wstring unicode_string(buffer);
// 		delete[] buffer;
// 		return unicode_string;
// #elif defined(__unix__)

// #endif
// 	}
// 	std::string codec::unicode_to_utf8(const std::wstring &unicode_String)
// 	{
// #if defined(WIN32)
// 		int len = WideCharToMultiByte(CP_UTF8, 0, unicode_String.c_str(), -1, NULL, 0, NULL, NULL);
// 		if(len == 0)
// 		{
// 			return "";
// 		}
// 		char* buffer = new char[len];
// 		if(!buffer)
// 		{
// 			return "";
// 		}
// 		WideCharToMultiByte(CP_UTF8, 0, unicode_String.c_str(), -1,buffer, len, NULL, NULL);
// 		std::string utf8_string(buffer);
// 		delete[] buffer;
// 		return utf8_string;
// #elif defined(__unix__)

// #endif
// 	}
// 	std::wstring codec::ansi_to_unicode(const std::string &ansi_string)
// 	{
// #if defined(WIN32)
// 		int len = MultiByteToWideChar(CP_ACP, 0, ansi_string.c_str(), -1, NULL, 0);
// 		if(len == 0)
// 		{
// 			return L"";
// 		}
// 		wchar_t* buffer = new wchar_t[len];
// 		if(!buffer)
// 		{
// 			return L"";
// 		}
// 		MultiByteToWideChar(CP_ACP, 0, ansi_string.c_str(), -1, buffer, len);
// 		std::wstring unicode_string(buffer);
// 		delete[] buffer;
// 		return unicode_string;

// #elif defined(__unix__)

// #endif
// 	}

// 	std::string codec::unicode_to_ansi(const std::wstring &unicode_string)
// 	{
// #if defined(WIN32)
// 		int len = WideCharToMultiByte(CP_ACP, 0, unicode_string.c_str(), -1, NULL, 0, NULL, NULL);
// 		if(len == 0)
// 		{
// 			return "";
// 		}
// 		char* buffer = new char[len];
// 		if(!buffer)
// 		{
// 			return "";
// 		}
// 		WideCharToMultiByte(CP_ACP, 0, unicode_string.c_str(), -1,buffer, len, NULL, NULL);
// 		std::string ansi_string(buffer);
// 		delete[] buffer;
// 		return ansi_string;
// #elif defined(__unix__)

// #endif
// 	}

// 	std::string codec::ansi_to_utf8(const std::string &ansi_string)
// 	{
// 		std::wstring unicode_string;
// 		std::string utf8_string;

// 		unicode_string = codec::ansi_to_unicode(ansi_string);
// 		utf8_string = codec::unicode_to_utf8(unicode_string);
// 		return utf8_string;
// 	}

// 	std::string codec::utf8_to_ansi(const std::string &utf8_string)
// 	{
// 		std::wstring unicode_string;
// 		std::string ansi_string;

// 		unicode_string = codec::utf8_to_unicode(utf8_string);
// 		ansi_string = codec::unicode_to_ansi(unicode_string);
// 		return ansi_string;
// 	}

//     std::string codec::url_encode(const std::string &str)
//     {
// 		std::string src = str;
// 		char hex[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','\0' };// "0123456789ABCDEF"
// 		std::string dst;


// 		for (size_t i = 0; i < src.size(); ++i)
// 		{
// 			unsigned char cc = src[i];
// 			if (cc >= 'A' && cc <= 'Z'
// 				|| cc >= 'a' && cc <= 'z'
// 				|| cc >= '0' && cc <= '9'
// 				|| cc == '.'
// 				|| cc == '_'
// 				|| cc == '-'
// 				|| cc == '*')
// 			{
// 				if (cc == ' ')
// 				{
// 					dst += "+";
// 				}
// 				else
// 					dst += cc;
// 			}
// 			else
// 			{
// 				unsigned char c = static_cast<unsigned char>(src[i]);
// 				dst += '%';
// 				dst += hex[c / 16];
// 				dst += hex[c % 16];
// 			}
// 		}
// 		return dst;
//     }

//     std::string codec::url_decode(const std::string &str)
//     {
// 		std::string result;
// 		int hex = 0;
// 		for (size_t i = 0; i < str.length(); ++i)
// 		{
// 			switch (str[i])
// 			{
// 			case '+':
// 				result += ' ';
// 				break;
// 			case '%':
// 				if (isxdigit(str[i + 1]) && isxdigit(str[i + 2]))
// 				{
// 					std::string hexStr = str.substr(i + 1, 2);
// 					hex = strtol(hexStr.c_str(), 0, 16);
// 					//字母和数字[0-9a-zA-Z]、一些特殊符号[$-_.+!*'(),] 、以及某些保留字[$&+,/:;=?@]
// 					//可以不经过编码直接用于URL
// 					if (!((hex >= 48 && hex <= 57) ||	//0-9
// 						(hex >= 97 && hex <= 122) ||	//a-z
// 						(hex >= 65 && hex <= 90) ||	//A-Z
// 						//一些特殊符号及保留字[$-_.+!*'(),]  [$&+,/:;=?@]
// 						hex == 0x21 || hex == 0x24 || hex == 0x26 || hex == 0x27 || hex == 0x28 || hex == 0x29
// 						|| hex == 0x2a || hex == 0x2b || hex == 0x2c || hex == 0x2d || hex == 0x2e || hex == 0x2f
// 						|| hex == 0x3A || hex == 0x3B || hex == 0x3D || hex == 0x3f || hex == 0x40 || hex == 0x5f
// 						))
// 					{
// 						result += char(hex);
// 						i += 2;
// 					}
// 					else result += '%';
// 				}
// 				else {
// 					result += '%';
// 				}
// 				break;
// 			default:
// 				result += str[i];
// 				break;
// 			}
// 		}
// 		return result;
//     }

//     std::string codec::base64_encode(const std::string &str)
// 	{
// 		if (str.size() == 0)
// 		{
// 			return std::string();
// 		}

// 		size_t outSize = BASE64_ENCODE_OUT_SIZE(str.size());
// 		if (outSize == 0)
// 		{
// 			return std::string();
// 		}
// 		std::string result;
// 		result.resize(outSize);
// 		size_t size = ::base64_encode((const unsigned char*)str.data(), (unsigned int)str.size(), (char*)&result[0]);
// 		return result;
//     }
//     std::string codec::base64_decode(const std::string &str)
//     {
// 		if(str.size() == 0)
// 		{
// 			return std::string();
// 		}
// 		size_t outSize = BASE64_DECODE_OUT_SIZE(str.size());
// 		if (outSize == 0)
// 		{
// 			return std::string();
// 		}
// 		std::string result;
// 		result.resize(outSize);
// 		size_t size = ::base64_decode(str.data(), (unsigned int)str.size(), (unsigned char*)&result[0]);
// 		return result;
//     }

//     std::string codec::md5(const std::string &str)
//     {
// 		// MD5 md5(str);
// 		// return md5.toStr();
//     }

//     int codec::hex_to_dec(const std::string &hex)
//     {
// 		// 跳过 '#' 或 '0x' 前缀
// 		size_t startIndex = 0;
// 		if ((hex.size() >= 2 && (hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X'))) ||
// 			(hex.size() >= 1 && hex[0] == '#')) {
// 			startIndex = (hex[0] == '#' ? 1 : 2); 
// 		}

// 		int dec = 0;
// 		int base = 1;
// 		for (size_t i = hex.size() - 1; i >= startIndex; --i) {
// 			char c = hex[i];
// 			if (!::isxdigit(c)) {
// 				// 非法字符，可以抛出异常或返回错误代码  
// 				throw std::invalid_argument("Invalid hexadecimal string");
// 			}
// 			int digitValue = ::tolower(c) - (c <= '9' ? '0' : ('a' - 10));
// 			dec += digitValue * base;
// 			base *= 16;
// 		}
// 		return dec;

//     }

// 	std::string codec::dec_to_hex(const int dec)
// 	{
// 		std::vector<char> hexChars;
// 		char hexDigits[] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','\0' };
// 		// char hexDigits[] = "0123456789abcdef";

// 		unsigned int uDec = dec; // 避免负数情况  
// 		while (uDec > 0) {
// 			hexChars.push_back(hexDigits[uDec % 16]);
// 			uDec /= 16;
// 		}

// 		// 如果结果为 0，直接返回 "0"  
// 		if (hexChars.empty()) {
// 			hexChars.push_back('0');
// 		}

// 		// 逆序拼接字符  
// 		std::reverse(hexChars.begin(), hexChars.end());

// 		// 转换为字符串并返回  
// 		return std::string(hexChars.begin(), hexChars.end());
// 	}

// }
