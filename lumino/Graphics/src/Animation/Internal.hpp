#pragma once
//#include "../Internal.hpp"
//#include <LuminoEngine/Rendering/Common.hpp>

namespace ln {
namespace detail {

#if 0
template<typename T>
class CacheBuffer
{
public:
	CacheBuffer()
		: m_buffer()
		, m_capacity(0)
		, m_count(0)
	{
	}

	void clearAndReserve(int count)
	{
		m_buffer.resize(sizeof(T) * count);
		m_capacity = count;
		m_count = 0;
	}

	void add(const T& value)
	{
		tryGlow(m_count + 1);
		memcpy(&(m_buffer.data()[sizeof(T) * m_count]), &value, sizeof(T));
		m_count++;
	}

	T* request(int count)
	{
		tryGlow(m_count + count);
		size_t begin = getBufferUsedByteCount();
		m_count += count;
		return reinterpret_cast<T*>(m_buffer.data() + begin);
	}

	void clear()
	{
		m_count = 0;
	}

	T& getAt(int index) { return ((T*)(m_buffer.data()))[index]; }
	T& getLast() { return getAt(m_count - 1); }

	int getCount() const { return m_count; }
	byte_t* getBuffer() { return m_buffer.data(); }
	size_t getBufferUsedByteCount() { return m_count * sizeof(T); }

private:
	void tryGlow(int requestCount)
	{
		if (/*m_count + */requestCount > m_capacity)
		{
			clearAndReserve(m_capacity * 2);
		}
	}

	ByteBuffer	m_buffer;
	int			m_capacity;
	int			m_count;
};
#endif

} // namespace detail
} // namespace ln
