
#pragma once
#include <unordered_map>
#include "Common.h"

LN_NAMESPACE_BEGIN
namespace parser
{

template<typename T>
class CacheBuffer
{
public:

	CacheBuffer()
		: m_buffer()
		, m_usedByteCount(0)
	{
	}

	void Reserve(size_t initSize)
	{
		m_buffer.Resize(sizeof(T) * initSize, false);
		m_usedByteCount = 0;
	}

	DataLocation Append(const T* dataArray, size_t count)
	{
		return AllocBuffer(sizeof(T) * count, dataArray);
	}

	T* GetData(DataLocation loc)
	{
		byte_t* buf = m_buffer.GetData();
		return (T*)&buf[loc];
	}

	const T* GetData(DataLocation loc) const
	{
		const byte_t* buf = m_buffer.GetConstData();
		return (T*)&buf[loc];
	}

private:
	ByteBuffer	m_buffer;
	size_t		m_usedByteCount;

	DataLocation AllocBuffer(size_t byteCount, const void* copyData = nullptr)
	{
		if (m_usedByteCount + byteCount > m_buffer.GetSize())
		{
			size_t newSize = m_buffer.GetSize() + std::max(m_buffer.GetSize(), byteCount);
			m_buffer.Resize(newSize, false);
		}

		if (copyData != nullptr)
		{
			byte_t* ptr = &(m_buffer.GetData()[m_usedByteCount]);
			size_t size = m_buffer.GetSize() - m_usedByteCount;
			memcpy_s(ptr, size, copyData, byteCount);
		}

		DataLocation dataLoc = m_usedByteCount;
		m_usedByteCount += byteCount;
		return dataLoc;
	}
};

} // namespace parser
LN_NAMESPACE_END
