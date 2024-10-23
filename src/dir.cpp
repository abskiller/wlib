#include "dir.h"

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <algorithm> // std::replace
#ifdef _WIN32
#include <Windows.h>
#include <direct.h>
#include <shlobj_core.h>
#include <algorithm>
#include <pathcch.h>
#include "codec.h"
#pragma comment(lib, "pathcch.lib")
#else
#include <unistd.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#ifdef _WIN32
#pragma comment(lib, "shell32.lib")
#endif

namespace wlib
{
	dir::dir()
	{
	}

	std::vector<std::string> dir::drives()
	{

#if defined(WIN32)
		std::vector<std::string> list;
		char driveBuffer[3];
		driveBuffer[2] = '\0';
		driveBuffer[1] = ':';

		// 遍历所有逻辑驱动器
		for (char letter = 'A'; letter <= 'Z'; ++letter)
		{
			driveBuffer[0] = letter;

			// 尝试打开驱动器根目录
			if (GetDriveTypeA(driveBuffer) == DRIVE_FIXED)
			{
				list.push_back(driveBuffer);
			}
		}
		return list;
#else
		std::vector<std::string> list;
		return list;
#endif
	}

	std::vector<std::string> dir::sub(const std::string &path, bool recursion, bool full)
	{
#ifdef _WIN32
		std::vector<std::string> subDirs;
		WIN32_FIND_DATAA findData;
		HANDLE hFind = FindFirstFileA((path + (path.back() == '\\' ? "*" : "\\*")).c_str(), &findData);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
				{
					if (std::string(findData.cFileName) != "." && std::string(findData.cFileName) != "..")
					{
						if (full)
						{
							subDirs.push_back(path + "\\" + findData.cFileName);
						}
						else
						{
							subDirs.push_back(findData.cFileName);
						}
						if (recursion)
						{
							std::string newPath = path + "\\" + findData.cFileName;
							auto childSubDirs = sub(newPath, recursion, full);
							subDirs.insert(subDirs.end(), childSubDirs.begin(), childSubDirs.end());
						}
					}
				}
			} while (FindNextFileA(hFind, &findData));

			FindClose(hFind);
		}
		return subDirs;
#else
		std::vector<std::string> subDirs;
		DIR *dir;
		struct dirent *ent;

		if ((dir = opendir(path.c_str())) != NULL)
		{
			while ((ent = readdir(dir)) != NULL)
			{
				if (ent->d_type == DT_DIR)
				{
					if (std::string(ent->d_name) != "." && std::string(ent->d_name) != "..")
					{
						if (full)
						{
							subDirs.push_back(path + "/" + ent->d_name);
						}
						else
						{
							subDirs.push_back(ent->d_name);
						}
						if (recursion)
						{
							std::string newPath = path + "/" + ent->d_name;
							auto childSubDirs = sub(newPath, recursion, full);
							subDirs.insert(subDirs.end(), childSubDirs.begin(), childSubDirs.end());
						}
					}
				}
			}
			closedir(dir);
		}

		return subDirs;
#endif
	}

	std::string dir::current()
	{

		char buffer[FILENAME_MAX];
#ifdef _WIN32
		if (_getcwd(buffer, sizeof(buffer)))
#else
		if (getcwd(buffer, sizeof(buffer)))
#endif
		{
			return std::string(buffer);
		}
		return std::string();
	}

	std::string dir::parent(const std::string &path)
	{
		std::string result;

#ifdef _WIN32
		std::wstring path_wide = codec::ansi_to_unicode(path);
		if (path_wide.empty())
		{
			return std::string();
		}

		wchar_t parentPath[MAX_PATH] = {0};
		if (wcscpy_s(parentPath, MAX_PATH, path_wide.c_str()) != 0)
		{
			return std::string();
		}
		// 使用 PathCchRemoveFileSpec 移除文件名部分
		if (FAILED(PathCchRemoveFileSpec(parentPath, MAX_PATH)))
		{
			return std::string();
		}

		// 移除结尾的反斜杠（如果存在）
		size_t len = wcslen(parentPath);
		if (len > 0 && (parentPath[len - 1] == L'\\' || parentPath[len - 1] == L'/'))
		{
			parentPath[len - 1] = L'\0';
		}

		result = codec::unicode_to_ansi(parentPath);
#else
		// Use dirname on Linux
		char *temp = strdup(path.c_str());
		if (temp == NULL)
		{
			return std::string();
		}

		char *parentDir = dirname(temp);
		if (parentDir == NULL)
		{
			free(temp);
			return std::string();
		}

		result = std::string(parentDir);
		free(temp);

		// Remove trailing slash if it exists (for consistency)
		if (!result.empty() && result.back() == '/')
		{
			result.pop_back();
		}
#endif
		return result;
	}

	bool dir::create(const std::string &filepath, bool recursion)
	{
#ifdef _WIN32
		std::wstring path_wide = codec::ansi_to_unicode(filepath);
		if (path_wide.empty())
		{
			return false;
		}

		DWORD attributes = GetFileAttributesW(path_wide.c_str());
		if (attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			// 目录已存在
			return true;
		}

		if (recursion)
		{
			// 创建父目录
			std::string parent = dir::parent(filepath);
			if (!parent.empty())
			{
				if (!dir::create(parent, true))
				{
					return false;
				}
			}
		}

		// 创建目录
		if (!CreateDirectoryW(path_wide.c_str(), NULL))
		{
			return false;
		}

		return true;
#elif __linux__
		if (mkdir(filepath.c_str(), 0777) == 0)
		{
			return true;
		}
		else if (recursion && errno == ENOENT)
		{
			// 创建父目录
			std::string parent = dir::parent(filepath);
			if (!parent.empty())
			{
				if (!dir::create(parent, true))
				{
					return false;
				}
			}
			// 再次尝试创建目录
			if (mkdir(filepath.c_str(), 0777) == 0)
			{
				return true;
			}
		}
		return false;
#else
		return false;
#endif
	}

	std::string dir::format(const std::string &path, bool tailSlash)
	{
		std::string result = path;

		// 在Windows上，将正斜杠替换为反斜杠（如果需要的话）
		// 在Linux上，这一步可以省略，但保留以确保跨平台一致性（尽管通常不需要）
		std::replace(result.begin(), result.end(), '/', '\\');

		// 如果路径为空，直接返回
		if (result.empty())
		{
			return result;
		}

		// 根据tailSlash参数添加或删除尾随的反斜杠
		if (tailSlash)
		{
			if (result.back() != '\\')
			{
				result += '\\';
			}
		}
		else
		{
			if (result.back() == '\\')
			{
				result.pop_back();
			}
		}

		return result;
	}

	std::string dir::temp()
	{
#ifdef _WIN32
		wchar_t tempPath[MAX_PATH] = {0};
		if (GetTempPathW(MAX_PATH, tempPath) == NULL)
		{
			return std::string();
		}

		// 使用GetLongPathNameW获取长文件名
		wchar_t longPath[MAX_PATH] = {0};
		if (GetLongPathNameW(tempPath, longPath, MAX_PATH) == 0)
		{
			// 如果转换失败，返回原始的短文件名路径
			return codec::unicode_to_ansi(tempPath);
		}
		std::string result = codec::unicode_to_ansi(longPath);
#elif __linux__
		const char *tempDir = getenv("TMPDIR");
		if (tempDir == NULL)
		{
			tempDir = "/tmp"; // 默认的临时目录
		}
		std::string result = tempDir;
#else
#endif
		if (!result.empty() && (result.back() == '/' || result.back() == '\\'))
		{
			result.pop_back();
		}
		return result;
	}

	bool dir::remove(const std::string &path)
	{
#ifdef _WIN32
		std::string searchPath = path;
		// 确保searchPath以斜杠结尾，除非它已经是根目录
		if (!searchPath.empty() && searchPath.back() != '\\')
		{
			searchPath += "\\";
		}
		searchPath += "*"; // 添加通配符以搜索目录中的所有文件和子目录

		WIN32_FIND_DATAA ffd;
		HANDLE hFind = FindFirstFileA(searchPath.c_str(), &ffd);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			// 检查是否是“目录不存在”的错误
			if (GetLastError() == ERROR_FILE_NOT_FOUND || GetLastError() == ERROR_NO_MORE_FILES)
			{
				return true; // 如果目录不存在，则视为成功删除
			}
			return false; // 其他错误，返回失败
		}

		do
		{
			if (std::strcmp(ffd.cFileName, ".") != 0 && std::strcmp(ffd.cFileName, "..") != 0)
			{
				std::string subPath = path;
				if (!subPath.empty() && subPath.back() != '\\')
				{
					subPath = subPath + "\\" + ffd.cFileName;
				}
				else
				{
					subPath += ffd.cFileName;
				}
				// std::string subPath = path + "\\" + ffd.cFileName; // 总是添加斜杠，因为我们已经确保了path以斜杠结尾

				if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (!remove(subPath))
					{
						FindClose(hFind);
						return false;
					}
				}
				else
				{
					if (!DeleteFileA(subPath.c_str()))
					{
						FindClose(hFind);
						return false;
					}
				}
			}
		} while (FindNextFileA(hFind, &ffd));

		FindClose(hFind);

		// 尝试删除目录本身，此时它应该为空
		return RemoveDirectoryA(path.c_str()) != 0;
#else
		return false;
#endif
	}

	std::string dir::desktop()
	{
#ifdef _WIN32
		wchar_t path[MAX_PATH] = {0};
		if (S_OK != SHGetFolderPathW(NULL, CSIDL_DESKTOP, NULL, 0, path))
		{
			return std::string();
		}

		return codec::unicode_to_ansi(path);
#elif __linux__
		char *desktopPath = getenv("HOME");
		if (desktopPath == NULL)
		{
			return std::string();
		}
		std::string result = desktopPath;
		result += "/Desktop/";
		return result;
#else
		return std::string();
#endif
	}

}
