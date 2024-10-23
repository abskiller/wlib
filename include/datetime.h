#ifndef _DATETIME_H_
#define _DATETIME_H_

#include "wlib.h"
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>
#ifdef _WIN32
#else
#include <chrono>
#endif
namespace wlib
{
	class datetime
	{
	public:
		explicit datetime();
	public:
		/*
			当前时间戳
			返回:1714787791611
		*/
		static std::time_t current_timestamp();

		/*
			当前时间
		*/
		static std::string nowTime(const std::string format = "%Y-%m-%d %H:%M:%S");

		/*
			文本时间转时间戳
		*/
		static time_t textToTimeStamp(const std::string time);

		/*
			文本时间转指定格式
		*/
		static std::string format(const std::string time, const std::string format = "%Y-%m-%d %H:%M:%S");
	};
}



#endif // !_DATETIME_HEAD

