#ifndef _DIR_H_
#define _DIR_H_

#include "wlib.h"
#include <vector>
#include <string>

namespace wlib
{
	class dir
	{
	public:
		dir();
	public:
		/*
			枚举驱动器
			例:
				输出:
					C:\\
					D:\\
					E:\\
					F:\\

		*/
		static std::vector<std::string> drives();

		/*
			枚举子目录(仅目录名称)
			例:
				输入:C:\\
				输出:
					$Recycle.Bin
					boot
					config
					Documents and Settings
					Program Files
					Program Files (x86)
					ProgramData
					Recovery
					Remote Desktop Connection
					System Volume Information
					Users
					Windows
					Windows.old
		*/
		static std::vector<std::string> sub(const std::string& path, bool recursion = false, bool full = false);

		/*
			取运行目录
			例:
				输出:C:\\System32
		*/
		static std::string current();
		/*
			取父目录
			例:
				输入:C:\\System32\\a.exe
				输出:C:\\System32
		*/
		static std::string parent(const std::string& path);

		/*
			创建目录
		*/
		static bool create(const std::string& filepath, bool recursion = true);

		/*
			格式化目录路径(默认以'\'结尾)
		*/
		static std::string format(const std::string& path, bool tailSlash = true);

		/*
			删除目录(递归删除目录下文件)
		*/
		static bool remove(const std::string& path);

		/*
			取临时文件夹路径(以'\'结尾)
			返回:C:\\Users\\Administrator\\AppData\\Local\\Temp\\
		*/
		static std::string temp();

		/*
			功能:取桌面路径(以'\'结尾)
			返回:C:\\Users\\Administrator\\Desktop\\
		*/
		static std::string desktop();
	};

}

#endif