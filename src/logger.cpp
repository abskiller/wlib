#include "logger.h"

namespace wlib
{
	logger* logger::m_instance = new logger();

	logger::logger()
	{
		m_pattern = "{}";
		m_outFile = true;
		setLevel(LogLevel::Debug);

	}

	logger* logger::instance()
	{
		return m_instance;
	}

	void logger::setLevel(LogLevel level)
	{
		std::lock_guard<std::mutex> guard(m_ioMutex);
		m_level = level;
	}

	void logger::setPattern(std::string pattern)
	{
		std::lock_guard<std::mutex> guard(m_ioMutex);
		m_pattern = pattern;
	}

	void logger::clear()
	{
		std::lock_guard<std::mutex> guard(m_ioMutex);
		m_fileMutex.clear();
	}

	bool logger::writeToFile(std::string filename, std::string content)
	{
		std::ofstream file(filename, std::ios::binary | std::ios::app);


		if (file.good())
		{

			file << content << "\n";

			file.close();
			return true;
		}
		else
		{
			return false;
		}
	}


	std::vector<std::string> logger::split(std::string str, std::string pattern)
	{
		std::string::size_type pos;
		std::vector<std::string> result;
		str += pattern;
		size_t size = str.size();

		for (size_t i = 0; i < size; i++)
		{
			pos = str.find(pattern, i);
			if (pos < size)
			{
				std::string s = str.substr(i, pos - i);
				result.push_back(s);
				i = pos + pattern.size() - 1;
			}
		}
		return result;
	}



	std::ostream& logger::red(std::ostream& s)
	{
#ifdef _WIN32
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
#else
		s << BOLDRED;
#endif
		return s;
	}

	std::ostream& logger::yellow(std::ostream& s)
	{
#ifdef _WIN32
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
#else
		s << BOLDYELLOW;
#endif
		return s;
	}

	std::ostream& logger::blue(std::ostream& s)
	{
#ifdef _WIN32
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
#else
		s << BOLDBLUE;
#endif
		return s;
	}

	std::ostream& logger::green(std::ostream& s)
	{
#ifdef _WIN32
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
#else
		s << BOLDGREEN;
#endif
		return s;
	}

	std::ostream& logger::white(std::ostream& s)
	{
#ifdef _WIN32
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
#else
		s << BOLDWHITE;
#endif
		return s;
	}

	std::ostream& logger::reset(std::ostream& s)
	{
#ifdef _WIN32
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
#else
		s << RESET;
#endif
		return s;
	}


}


