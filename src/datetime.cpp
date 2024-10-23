#include "datetime.h"


// #include <string>

namespace wlib
{
    datetime::datetime()
    {
    }

    std::time_t datetime::current_timestamp()
    {
        std::time_t now = std::time(nullptr);
        return static_cast<long long>(now) * 1000;
    }

    std::string datetime::nowTime(const std::string format)
    {
#ifdef _WIN32
        time_t tNowTime;
        time(&tNowTime);
        // struct tm t;
        tm tLocalTime;
        localtime_s(&tLocalTime, &tNowTime);
        char szTime[30] = {'\0'};
        strftime(szTime, 30, format.c_str(), &tLocalTime);
        std::string strTime = szTime;
        return strTime;
#else
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), format.c_str());
        return ss.str();
#endif
    }

    time_t datetime::textToTimeStamp(const std::string time)
    {
        std::istringstream iss(time);
        std::tm tm;

        // 分隔符 根据实际需要调整
        char delimiter = ' ';

        // 使用strptime解析文本时间到tm结构体中
        iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        // 检查解析是否成功
        if (iss.fail())
        {
            std::cerr << "时间解析失败" << std::endl;
            return 0;
        }

        // 使用mktime将tm结构体转换为time_t时间戳
        time_t timeSinceEpoch = std::mktime(&tm);

        // 检查转换是否成功
        if (timeSinceEpoch == -1)
        {
            std::cerr << "时间转换失败" << std::endl;
            return 0;
        }
        return static_cast<long long>(timeSinceEpoch) * 1000;
    }

    std::string datetime::format(const std::string time, const std::string format)
    {
        std::istringstream iss(time);
        std::tm tm;

        // 分隔符 根据实际需要调整
        char delimiter = ' ';

        // 使用strptime解析文本时间到tm结构体中
        iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        // 检查解析是否成功
        if (iss.fail())
        {
            std::cerr << "时间解析失败" << std::endl;
            return "";
        }

        char szTime[30] = {'\0'};
        strftime(szTime, 30, format.c_str(), &tm);
        std::string strTime = szTime;
        return strTime;
    }

}
