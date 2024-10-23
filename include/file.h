#ifndef _FILE_H
#define _FILE_H

#include <string>
#include <map>
#include <list>

#include "wlib.h"
#include "bytearray.h"

namespace wlib
{

	class file
	{
	// enum OPEN_TYPE {
	// 	ReadText,
	// 	ReadAndWriteText,
	// 	ReadBytes,
	// 	ReadAndWriteBytes,
	// 	CreateClearReadText,
	// 	CreateClearReadAndWriteText,
	// 	CreateClearReadBytes,
	// 	CreateClearReadAndWriteBytes,
	// 	CreateWriteTextTail,
	// 	CreateWriteBytesTail,
	// 	CreateReadAndWriteBytesTail
	// };
	public:
		file();
		~file();
		bool open(const std::string& filepath, const std::string type = "rb+");
		bytearray read(size_t len) const;
		bool write(const char* buffer, size_t size);
		bool write(const bytearray& value);
		// 重定位文件指针
		bool seek(size_t index) const;
		size_t tell() const;
		bool close();
		bool flush();
		bool clear();
		char operator[](size_t index) const;
		bool is_open() { return m_file != NULL; }
	private:
		std::string m_path;
		FILE* m_file;
	public:
		static size_t size(const std::string& filepath);
		static bool exist(const std::string& filepath);
		static bool remove(const std::string& filepath);
		static bool rename(const std::string& src,const std::string& dst);
		static void list(const std::string& path, std::vector<std::string>& list,bool recursion = false);
		static bool write(const std::string& filepath, const char* data, size_t len);
		static bool write(const std::string& filepath, const bytearray& data);
		static bool write(const std::string& filepath, const std::string& data);
		static bool append(const std::string& filepath, const bytearray& data);
		static bytearray read(const std::string& filepath);
		static bool copy(const std::string& src, const std::string& dst, bool cover = true);
		static std::string dir(const std::string& path);
		static std::string filename(const std::string& path);

		/*
			取文件后缀
		*/
		static std::string suffix(const std::string& path);
		static bytearray icon(const std::string& exePath, int iconIndex = 0);

		/*
			取临时文件名
		*/
		static std::string tempPath();

		static bytearray resources(char* type, char* index);

		static std::string description(const std::string& path);//utf8
		static std::string company(const std::string& path);//utf8

		static bool move(const std::string& src, const std::string& dst);

	};
}

#endif