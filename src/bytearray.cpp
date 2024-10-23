#include "bytearray.h"
namespace wlib
{
	/// <summary>
	/// 默认构造函数
	/// </summary>
	bytearray::bytearray()
	{
		m_buffer = nullptr;
		m_size = 0;
	}

	bytearray::bytearray(const bytearray &data)
	{
		size_t size = data.size();
		m_buffer = (char *)::malloc(size);
		if (m_buffer)
		{
			::memcpy(m_buffer, data.data(), size);
			m_size = size;
		}
		else
		{
			m_size = 0;
		}
	}

	bytearray::bytearray(const char *data, size_t size)
	{
		if (!data || !size)
		{
			m_buffer = nullptr;
			m_size = 0;
			return;
		}
		m_buffer = (char *)::malloc(size);
		if (m_buffer)
		{
			::memcpy(m_buffer, data, size);
			m_size = size;
		}
		else
		{
			m_size = 0;
		}
	}

	bytearray::bytearray(const char *data)
	{
		if (!data)
		{
			m_buffer = nullptr;
			m_size = 0;
			return;
		}
		// '\0'
		size_t size = strlen(data); // + sizeof(char);// +sizeof('\0');
		m_buffer = (char *)::malloc(size);
		if (m_buffer)
		{
			::memcpy(m_buffer, data, size);
			m_size = size;
		}
		else
		{
			m_size = 0;
		}
	}

	bytearray::bytearray(const wchar_t *data)
	{
		if (!data)
		{
			m_buffer = nullptr;
			m_size = 0;
			return;
		}
		// '\0'
		size_t size = wcslen(data) * 2; //+ sizeof(wchar_t);// +sizeof('\0');
		m_buffer = (char *)::malloc(size);
		if (m_buffer)
		{
			::memcpy(m_buffer, data, size);
			m_size = size;
		}
		else
		{
			m_size = 0;
		}
	}

	bytearray::bytearray(size_t size, char c)
	{
		if (!size)
		{
			return;
		}
		m_buffer = (char *)::malloc(size);
		if (m_buffer)
		{
			::memset(m_buffer, c, size);
			m_size = size;
		}
		else
		{
			m_size = 0;
		}
	}

	bytearray::bytearray(const std::string &data)
	{
		if (!data.c_str())
		{
			m_buffer = nullptr;
			m_size = 0;
			return;
		}
		// '\0'
		size_t size = strlen(data.c_str()); // + sizeof(char);// +sizeof('\0');
		m_buffer = (char *)::malloc(size);
		if (m_buffer)
		{
			::memcpy(m_buffer, data.c_str(), size);
			m_size = size;
		}
		else
		{
			m_size = 0;
		}
	}

	bytearray::bytearray(const std::wstring &data)
	{
		if (!data.c_str())
		{
			m_buffer = nullptr;
			m_size = 0;
			return;
		}
		// '\0'
		size_t size = wcslen(data.c_str()); // + sizeof(char);// +sizeof('\0');
		m_buffer = (char *)::malloc(size);
		if (m_buffer)
		{
			::memcpy(m_buffer, data.c_str(), size);
			m_size = size;
		}
		else
		{
			m_size = 0;
		}
	}

	bytearray::~bytearray()
	{
		if (m_buffer)
		{
			::free(m_buffer);
		}
		m_size = 0;
	}

	/*
		重载函数
	*/
	bytearray &bytearray::operator=(const bytearray &value)
	{
		if (m_buffer)
		{
			::free(m_buffer);
			m_size = 0;
		}
		m_buffer = (char *)::malloc(value.size());
		if (m_buffer)
		{
			::memcpy(m_buffer, value.data(), value.size());
			m_size = value.size();
		}
		return *this;
	}

	bytearray bytearray::operator+(const bytearray &value) const
	{
		bytearray buffer;
		// 调整缓冲区大小
		buffer.resize(m_size + value.size());

		// 追加复制数据
		::memcpy(buffer.data(), this->data(), this->size());
		::memcpy(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(buffer.data()) + this->size()), value.data(), value.size());

		// 多余
		// m_size += value.size();
		return buffer;
	}

	bytearray &bytearray::operator+=(const bytearray &value)
	{
		// 调整缓冲区大小
		resize(m_size + value.size());

		// 追加复制数据
		::memcpy(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(m_buffer) + m_size - value.size()), value.data(), value.size());

		// 多余
		// m_size += value.size();
		return *this;
	}

	char bytearray::operator[](const size_t i) const
	{
		return reinterpret_cast<char *>(m_buffer)[i];
	}

	const bool bytearray::operator==(const bytearray &value) const
	{
		if (this->m_size != value.m_size)
		{
			return false;
		}

		for (size_t i = 0; i < m_size; i++)
		{
			if (operator[](i) != value[i])
			{
				return false;
			}
		}

		return true;
	}

	const bool bytearray::operator!=(const bytearray &value) const
	{
		if (this->m_size != value.m_size)
		{
			return true;
		}

		for (size_t i = 0; i < m_size; i++)
		{
			if (operator[](i) != value[i])
			{
				return true;
			}
		}

		return false;
	}

	bytearray &bytearray::append(const bytearray &value)
	{
		if (value.size() == 0)
		{
			return *this;
		}
		return (*this += value);
	}

	bytearray &bytearray::append(const char *data, size_t size)
	{
		if (!data || !size)
		{
			return *this;
		}
		return *this += bytearray(data, size);
	}

	bytearray &bytearray::append(const char *data)
	{
		if (!data)
		{
			return *this;
		}
		return *this += bytearray(data);
	}

	bytearray &bytearray::append(const wchar_t *data)
	{
		if (!data)
		{
			return *this;
		}
		return *this += bytearray(data);
	}

	bytearray &bytearray::append(const bool data)
	{
		this->append((char *)&data, sizeof(data));
		return *this;
	}

	bytearray &bytearray::append(const char data)
	{
		this->append((char *)&data, sizeof(data));
		return *this;
	}

	bytearray &bytearray::append(const wchar_t data)
	{
		this->append((char *)&data, sizeof(data));
		return *this;
	}

	bytearray &bytearray::append(const short data)
	{
		this->append((char *)&data, sizeof(data));
		return *this;
	}

	bytearray &bytearray::append(const int32_t data)
	{
		this->append((char *)&data, sizeof(data));
		return *this;
	}

	bytearray &bytearray::append(const int64_t data)
	{
		this->append((char *)&data, sizeof(data));
		return *this;
	}

	bytearray &bytearray::append(const unsigned char data)
	{
		this->append((char *)&data, sizeof(data));
		return *this;
	}

	bytearray &bytearray::append(const unsigned short data)
	{
		this->append((char *)&data, sizeof(data));
		return *this;
	}

	bytearray &bytearray::append(const uint32_t data)
	{
		this->append((char *)&data, sizeof(data));
		return *this;
	}

	bytearray &bytearray::append(const uint64_t data)
	{
		this->append((char *)&data, sizeof(data));
		return *this;
	}

	bytearray &bytearray::prepend(const bytearray &value)
	{
		if (value.size() == 0)
		{
			return *this;
		}
		*this = value + *this;
		return *this;
	}

	bytearray &bytearray::prepend(const char *data, size_t size)
	{
		if (!data || !size)
		{
			return *this;
		}
		*this = bytearray(data, size) + *this;
		return *this;
	}

	bytearray &bytearray::prepend(const char *data)
	{
		if (!data)
		{
			return *this;
		}
		*this = bytearray(data) + *this;
		return *this;
	}

	bytearray &bytearray::prepend(const wchar_t *data)
	{
		if (!data)
		{
			return *this;
		}
		*this = bytearray(data) + *this;
		return *this;
	}

	bytearray &bytearray::prepend(const bool data)
	{
		this->prepend((char *)&data, sizeof(data));
		return *this;
	}

	bytearray &bytearray::prepend(const char data)
	{
		this->prepend((char *)&data, sizeof(data));
		return *this;
	}

	bytearray &bytearray::prepend(const wchar_t data)
	{
		this->prepend((char *)&data, sizeof(data));
		return *this;
	}

	bytearray &bytearray::prepend(const short data)
	{
		this->prepend((char *)&data, sizeof(data));
		return *this;
	}

	bytearray &bytearray::prepend(const int32_t data)
	{
		this->prepend((char *)&data, sizeof(data));
		return *this;
	}

	bytearray &bytearray::prepend(const int64_t data)
	{
		this->prepend((char *)&data, sizeof(data));
		return *this;
	}

	bytearray &bytearray::prepend(const unsigned char data)
	{
		this->prepend((char *)&data, sizeof(data));
		return *this;
	}

	bytearray &bytearray::prepend(const unsigned short data)
	{
		this->prepend((char *)&data, sizeof(data));
		return *this;
	}

	bytearray &bytearray::prepend(const uint32_t data)
	{
		this->prepend((char *)&data, sizeof(data));
		return *this;
	}

	bytearray &bytearray::prepend(const uint64_t data)
	{
		this->prepend((char *)&data, sizeof(data));
		return *this;
	}

	bytearray bytearray::left(const size_t &index, bool include) const
	{
		if (!include)
		{
			return bytearray(m_buffer, INDEXTOSIZE(index) - 1);
		}
		else
		{
			return bytearray(m_buffer, INDEXTOSIZE(index));
		}
	}

	bytearray bytearray::left(const bytearray &data, bool include) const
	{
		size_t index = 0;
		if (!this->indexOf(data, index))
		{
			return bytearray();
		}
		if (!include)
		{
			return this->left(index);
		}
		else
		{
			return this->left(index + SIZETOINDEX(data.size()), true);
		}
	}

	bytearray bytearray::right(const size_t index, bool include) const
	{
		if (!include)
		{
			return bytearray(reinterpret_cast<char *>(reinterpret_cast<uintptr_t>(m_buffer) + index + 1), m_size - INDEXTOSIZE(index) - 1); // 起始索引变化
		}
		else
		{
			return bytearray(reinterpret_cast<char *>(reinterpret_cast<uintptr_t>(m_buffer) + index), m_size - INDEXTOSIZE(index));
		}
	}

	bytearray bytearray::right(const bytearray &data, bool include) const
	{
		size_t index;
		if (!this->indexOf(data, index))
		{
			return bytearray();
		}
		if (!include)
		{
			return this->right(index + SIZETOINDEX(data.size()));
		}
		else
		{
			return this->right(index, true);
		}
	}

	bytearray bytearray::mid(const size_t &startIndex, const size_t &endIndex, bool include) const
	{
		if (startIndex > m_size || endIndex > m_size)
		{
			return bytearray();
		}
		if (!include)
		{
			return bytearray(reinterpret_cast<char *>(reinterpret_cast<uintptr_t>(m_buffer) + startIndex + 1), INDEXTOSIZE(endIndex - startIndex - 1 - 1)); // 起始位置改变
		}
		else
		{
			return bytearray(reinterpret_cast<char *>(reinterpret_cast<uintptr_t>(m_buffer) + startIndex), INDEXTOSIZE(endIndex - startIndex));
		}
	}

	bytearray bytearray::mid(const bytearray &start, const bytearray &end, bool include) const
	{
		size_t startIndex = 0;
		size_t endIndex = 0;
		if (!this->indexOf(start, startIndex) || !this->indexOf(end, endIndex, startIndex))
		{
			return bytearray();
		}

		if (include)
		{
			return this->mid(startIndex, endIndex + SIZETOINDEX(end.size()), true);
		}
		else
		{
			return this->mid(startIndex + SIZETOINDEX(start.size()), endIndex, false);
		}
	}

	bool bytearray::indexOf(const char c, size_t &index, size_t startIndex) const
	{
		if (m_size <= startIndex)
		{
			return false;
		}
		for (size_t i = startIndex; i < m_size; i++)
		{
			if (operator[](i) == c)
			{
				index = i;
				return true;
			}
		}
		return false;
	}

	bool bytearray::indexOf(const bytearray &data, size_t &index, size_t startIndex) const
	{
		if (m_size <= startIndex + SIZETOINDEX(data.size()))
		{
			return false;
		}
		for (size_t i = startIndex; i <= m_size - data.size(); i++)
		{
			bool found = true;
			for (size_t j = 0; j < data.size(); j++)
			{
				if (operator[](i + j) != data[j])
				{
					found = false;
					break;
				}
			}
			if (found)
			{
				index = i;
				return true;
			}
		}
		return false;
	}

	bool bytearray::lastIndexOf(const char c, size_t &index, size_t startIndex) const
	{
		if (m_size <= startIndex)
		{
			return false;
		}
		for (size_t i = startIndex ? startIndex : SIZETOINDEX(m_size); i > 0; i--)
		{
			if (operator[](i) == c)
			{
				index = i;
				return true;
			}
		}
		if (operator[](0) == c)
		{
			index = 0;
			return true;
		}
		return false;
	}

	bool bytearray::lastIndexOf(const bytearray &data, size_t &index, size_t startIndex) const
	{
		if (m_size <= startIndex + SIZETOINDEX(data.size()))
		{
			return false;
		}
		for (size_t i = startIndex ? startIndex : m_size - data.size(); i > 0; i--)
		{
			bool found = true;
			for (size_t j = 0; j < data.size(); j++)
			{
				if (operator[](i + j) != data[j])
				{
					found = false;
					break;
				}
			}
			if (found)
			{
				index = i;
				return true;
			}
		}

		for (size_t i = 0; i < data.size(); i++)
		{
			if (operator[](i) != data[i])
			{
				return false;
			}
		}
		index = 0;
		return true;
	}

	bytearray bytearray::repalce(const bytearray &src, const bytearray &dest, const bool &once) const
	{
		bytearray buffer = *this;
		size_t index = 0;
		size_t startIndex = 0;

		while (buffer.indexOf(src, index, startIndex))
		{
			buffer = buffer.left(index) + dest + buffer.right(index + src.size() - 1);
			startIndex += dest.size();
			if (once)
			{
				break;
			}
		}

		return buffer;
	}

	std::vector<bytearray> bytearray::split(const bytearray &separator) const
	{
		std::vector<bytearray> list;
		size_t index = 0;
		size_t startIndex = 0;
		while (this->indexOf(separator, index, startIndex))
		{
			list.push_back(this->mid(startIndex, index));
			startIndex = index + separator.size();
		}
		return list;
	}

	bytearray::operator std::string() const
	{
		return std::string(m_buffer, m_size);
	}

	std::string bytearray::toHex(char separator) const
	{
		if (!m_size)
		{
			return std::string();
		}

		const char HEX_NUMBERS[16] = {
			'0', '1', '2', '3',
			'4', '5', '6', '7',
			'8', '9', 'a', 'b',
			'c', 'd', 'e', 'f'};

		// 一个字节会形成两个字符,最终长度是原始数据的2倍
		const size_t length = separator ? (m_size * 3 - 1) : (m_size * 2);

		bytearray hex;
		hex.resize(length);
		unsigned char *data = (unsigned char *)hex.data();
		for (unsigned int i = 0, o = 0; i < m_size; ++i)
		{
			unsigned char t = reinterpret_cast<unsigned char *>(m_buffer)[i];
			unsigned char high = t / 16;
			unsigned char low = t % 16;
			data[o++] = HEX_NUMBERS[high];
			data[o++] = HEX_NUMBERS[low];
			if (separator && (o < length))
			{
				data[o++] = separator;
			}
		}
		return std::string(hex.data(), hex.size());
	}

bytearray bytearray::fromHex(std::string hex, char separator)
{
    bytearray result;
    for (unsigned int i = 0; i < hex.length(); i += 2)
    {
        std::string byteString = hex.substr(i, 2);
        char byte = (char)strtol(byteString.c_str(), NULL, 16);
        result.append(byte);
        if (separator!= '\0' && (i + 2 < hex.length()))
        {
            // 跳过分隔符
            i++;
        }
    }
    return result;
}

	void bytearray::resize(size_t size)
	{
		char *buffer = (char *)::malloc(size);
		if (!buffer)
		{
			return;
		}
		::memset(buffer, 0, size);
		if (m_buffer)
		{

			::memcpy(buffer, m_buffer, size > m_size ? m_size : size); // MIN
			::free(m_buffer);
		}
		m_buffer = buffer;
		m_size = size;
	}

	void bytearray::resize(char padding, size_t size)
	{
		char *buffer = (char *)::malloc(size);
		if (!buffer)
		{
			return;
		}
		::memset(buffer, padding, size);
		if (m_buffer)
		{
			::memcpy(buffer, m_buffer, size > m_size ? m_size : size); // MIN

			::free(m_buffer);
		}
		m_buffer = buffer;
		m_size = size;
	}

	const size_t bytearray::size() const
	{
		return m_size;
	}

	const size_t bytearray::length() const
	{
		return m_size;
	}

	char *bytearray::data() const
	{
		return m_buffer;
	}
	void bytearray::clear()
	{
		if (m_buffer)
		{
			::free(m_buffer);
			m_size = 0;
			m_buffer = nullptr;
		}
	}
}
