
#include "file.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#include <map>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <shlwapi.h>
#include "codec.h"
#pragma comment(lib, "shlwapi.lib")

#else
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#endif

namespace wlib
{
    file::file()
    {
        m_file = 0;
    }

    file::~file()
    {
        // 析构自动关闭文件
        this->close();
    }

    bool file::open(const std::string &path, const std::string type)
    {
        m_file = fopen(path.data(), type.c_str());
        if (!m_file) // 成功返回0 失败返回错误代码
        {
            return false;
        }
        seek(0);
        return true;
    }

    bool file::close()
    {

        if (m_file == NULL)
        {
            return true;
        }

        if (fclose(this->m_file) != 0)
        {
            this->m_file = NULL;
            return false;
        }

        this->m_file = NULL;
        this->m_path = "";
        return true;
    }

    bool file::flush()
    {
        if (m_file == nullptr)
        {
            return false;
        }
        return fflush(this->m_file) == 0;
    }

    bool file::clear()
    {
        // 返回0文件尾部
        return feof(m_file) == 0;
    }

    bytearray file::read(size_t size) const
    {
        // 判断文件句柄是否打开
        if (this->m_file == NULL)
        {
            std::cout << "文件句柄未打开" << std::endl;
            return bytearray();
        }
        // 读取1个size的大小
        bytearray buffer;
        buffer.resize(0, size);
        size_t read_size;
        read_size = fread(buffer.data(), 1, size, this->m_file);
        if (read_size != size)
        {
            std::cout << "文件读取大小: " << read_size << std::endl;
            std::cout << "文件实际大小: " << size << std::endl;
            // return dog::bytearray();
        }
        return buffer;
    }

    bool file::write(const char *buffer, size_t size)
    {
        if (this->m_file == NULL)
        {
            return false;
        }

        if (fwrite(buffer, size, 1, this->m_file) == 1)
        {
            this->flush();
            return true;
        }
        else
        {
            return false;
        }
    }

    bool file::write(const bytearray &value)
    {
        return write(value.data(), (size_t)value.length());
    }

    bool file::seek(size_t index) const
    {
#ifdef _WIN64
        if (_fseeki64(m_file, index, SEEK_SET) == 0)
#else
        if (fseek(m_file, index, SEEK_SET) == 0)
#endif
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    size_t file::tell() const
    {
        if (m_file == NULL)
        {
            return 0;
        }
#ifdef _WIN64
        return _ftelli64(m_file);
#else
        return ftell(m_file);
#endif
    }

    char file::operator[](size_t index) const
    {

        if (!seek(index) == false)
        {
            return '\0';
        }
        bytearray result = read(1);
        return result[0];
    }

    size_t file::size(const std::string &filepath)
    {
#ifdef _WIN32
        FILE *file = NULL;
        file = fopen(filepath.data(), "r+");
        if (!file)
        {
            // std::cout << "打开文件失败" << std::endl;
            return 0;
        }
#ifdef _WIN64
        size_t size = _filelengthi64(_fileno(file));
#else
        size_t size = (size_t)_filelength(_fileno(file));
#endif
        fclose(file);
        return size;
#else
        struct stat f_stat;
        if (stat(filepath.c_str(), &f_stat) == -1)
        {
            return 0;
        }
        return f_stat.st_size;
#endif
    }

    bool file::exist(const std::string &filepath)
    {
#ifdef _WIN32
        return _access(filepath.data(), 0) != -1;
#else
        return access(filepath.data(), F_OK) == 0;
#endif
    }

    bool file::remove(const std::string &filepath)
    {
        if (::remove(filepath.data()) == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool file::rename(const std::string &src, const std::string &dst)
    {
        return false;
    }

    void file::list(const std::string &path, std::vector<std::string> &list, bool recursion)
    {
#ifdef _WIN32
        // 文件句柄
        intptr_t handle = 0;

        // 文件信息
        struct _finddata_t fileinfo;

        std::string searchPattern = path;
        // 如果路径不以\结尾，则添加一个\  
    if (searchPattern.back()!= '\\') {
        searchPattern += "\\";
    }
    searchPattern += "*";

    if ((handle = _findfirst(searchPattern.c_str(), &fileinfo)) != -1)
    {
        do
        {
            // 如果是目录，并且需要递归，则递归之
            if ((fileinfo.attrib & _A_SUBDIR) != 0)
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                {
                    if (recursion)
                    {
                        std::string newPath = path;
                        // 如果路径不以\结尾，则添加一个\  
                        if (newPath.back()!= '\\') {
                        newPath += "\\";
                    }
                    newPath += fileinfo.name;
                    file::list(newPath, list, recursion);
                }
            }
        }
        else
        {
            // 如果是文件，则添加到列表中
            list.push_back(fileinfo.name);
        }
    }
    while (_findnext(handle, &fileinfo) == 0)
        ;

    _findclose(handle);
}
#else
        DIR *dir;
        struct dirent *ent;

        if ((dir = opendir(path.c_str())) != NULL)
        {
            while ((ent = readdir(dir)) != NULL)
            {
                if (ent->d_type == DT_DIR)
                {
                    if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
                    {
                        if (recursion)
                        {
                            std::string newPath = path;
                            newPath += "/";
                            newPath += ent->d_name;
                            file::list(newPath, list, recursion);
                        }
                    }
                }
                else
                {
                    list.push_back(ent->d_name);
                }
            }
            closedir(dir);
        }
#endif
}

bool file::write(const std::string &filepath, const char *data, size_t len)
{

    std::string _filepath = filepath;
    if (exist(_filepath))
    {
        if (!file::remove(_filepath.data()))
        {
            return false;
        }
    }
    file file;
    if (!file.open(_filepath, "wb"))
    {
        return false;
    }

    if (!file.write(data, len))
    {
        return false;
    }

    file.close();
    return true;
}
bool file::write(const std::string &filepath, const bytearray &data)
{
    return file::write(filepath, data.data(), data.size());
}
bool file::write(const std::string &filepath, const std::string &data)
{
    return file::write(filepath, data.c_str(), data.size());
}
bool file::append(const std::string &filepath, const bytearray &data)
{

    std::ofstream file(filepath, std::ios::binary | std::ios::app);

    // 检查文件流状态
    if (file.good())
    {
        // 输出结尾字符串
        // file << content << "\n";
        // 关闭文件流
        file.close();
        return true;
    }
    else
    {
        return false;
    }
}
bytearray file::read(const std::string &filepath)
{
    file file;
    size_t size = 0;
    size = file::size(filepath);
    if (size == 0)
    {
        return bytearray();
    }
    bytearray buffer;
    buffer.resize(size);
    if (!file.open(filepath, "rb"))
    {
        return buffer;
    }

    if (size == 0)
    {
        file.close();
        return buffer;
    }
    buffer = file.read(size);
    file.close();
    return buffer;
}

bool file::copy(const std::string &src, const std::string &dst, bool cover)
{
#ifdef _WIN32
    if (CopyFileA(src.data(), dst.data(), !cover))
    {
        return true;
    }
    return false;
#else
        if (cover)
        {
            return rename(src.c_str(), dst.c_str()) == 0;
        }
        else
        {
            struct stat st;
            if (stat(dst.c_str(), &st) == 0)
            {
                return false;
            }
            return rename(src.c_str(), dst.c_str()) == 0;
        }
#endif
}

std::string file::dir(const std::string &path)
{
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos)
    {
        return std::string();
    }
    return path.substr(0, pos);
}

std::string file::filename(const std::string &path)
{
    // 找到最后一个斜杠或反斜杠的位置
    size_t pos = path.find_last_of("/\\");
    if (pos == std::string::npos)
    {
        // 如果没有找到斜杠或反斜杠，返回整个路径作为文件名
        return path;
    }

    // 找到文件名的起始位置
    size_t start = pos + 1;

    // 找到最后一个点的位置
    size_t dot_pos = path.rfind('.');

    // 如果没有找到点，或者点在斜杠或反斜杠之后，返回从起始位置到字符串末尾的子串
    if (dot_pos == std::string::npos || dot_pos < start)
    {
        return path.substr(start);
    }

    // 否则，返回从起始位置到点之前的子串
    return path.substr(start, dot_pos - start);
}

std::string file::suffix(const std::string &path)
{
    // 从最后一个点（'.'）开始反向搜索
    auto dot_pos = path.rfind('.');

    // 如果没有找到点，则没有后缀
    if (dot_pos == std::string::npos)
    {
        return "";
    }

    // 否则，返回点之后的部分作为后缀
    return path.substr(dot_pos + 1);
}

bytearray file::icon(const std::string &exePath, int iconIndex)
{
    return bytearray();
}
std::string file::tempPath()
{
#ifdef _WIN32
    char tempPath[MAX_PATH];
    GetTempPathA(MAX_PATH, tempPath);
    char tempFile[MAX_PATH];
    GetTempFileNameA(tempPath, "TMP", 0, tempFile);

    // 在这里，tempFile是一个存在的临时文件的路径。如果你只需要文件名，你可以使用PathFindFileName函数来提取它。
    // 但是，请注意，这个函数创建了一个真实的临时文件，所以你可能需要删除它。
    DeleteFileA(tempFile);
    return std::string(tempFile);
#else
        char tempPath[PATH_MAX];
        const char *tmpDir = getenv("TMPDIR");
        if (tmpDir == nullptr)
        {
            tmpDir = "/tmp";
        }
        snprintf(tempPath, sizeof(tempPath), "%s/%sXXXXXX", tmpDir, "TMP");
        int fd = mkstemp(tempPath);
        if (fd == -1)
        {
            perror("Failed to create temporary file");
            return "";
        }
        ::close(fd);
        unlink(tempPath);
        return std::string(tempPath);
#endif
}
bytearray file::resources(char *type, char *index)
{
    return bytearray();
}
std::string file::description(const std::string &path)
{
    return std::string();
}
std::string file::company(const std::string &path)
{

    return std::string();
}

bool file::move(const std::string &src, const std::string &dst)
{
#ifdef _WIN32
    return MoveFileExA(src.c_str(), dst.c_str(), MOVEFILE_REPLACE_EXISTING) == TRUE;
#else
        return rename(src.c_str(), dst.c_str()) == 0;
#endif
}
}
