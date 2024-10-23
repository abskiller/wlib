#ifndef _CODEC_H_
#define _CODEC_H_



#include "wlib.h"
#include <string>

namespace wlib 
{
	class codec 
	{
	public:
		explicit codec();

	public:
		static char lower(char c);
		static char upper(char c);
		static wchar_t lowerw(wchar_t wc);
		static wchar_t upperw(wchar_t wc);

		static std::wstring ansi_to_unicode(const std::string& ansi_string);
		static std::string unicode_to_ansi(const std::wstring& unicode_string);

		static std::wstring utf8_to_unicode(const std::string& utf8_string);
		static std::string unicode_to_utf8(const std::wstring& unicode_string);



		static std::string ansi_to_utf8(const std::string& ansi_string);
		static std::string utf8_to_ansi(const std::string& utf8_string);

		static std::string url_encode(const std::string& str);
		static std::string url_decode(const std::string& str);

		static std::string base64_encode(const std::string& str);
		static std::string base64_decode(const std::string& str);

		static std::string md5(const std::string& str);

		static int hex_to_dec(const std::string& hex);
		static std::string dec_to_hex(const int dec);


	};
}

#endif