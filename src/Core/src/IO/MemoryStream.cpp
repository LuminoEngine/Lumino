
#include "Internal.hpp"
#include <LuminoCore/IO/MemoryStream.hpp>
#include <LuminoCore/IO/FileSystem.hpp>
//#include "../../Include/Lumino/IO/FileSystem.h"

namespace ln {

//==============================================================================
// MemoryStream
//==============================================================================

////------------------------------------------------------------------------------
//MemoryStreamPtr MemoryStream::create()
//{
//	MemoryStreamPtr ptr(LN_NEW MemoryStream(), false);
//	return ptr;
//}
//
////------------------------------------------------------------------------------
//MemoryStreamPtr MemoryStream::create(void* buffer, size_t size)
//{
//	MemoryStreamPtr ptr(LN_NEW MemoryStream(buffer, size), false);
//	return ptr;
//}
//
////------------------------------------------------------------------------------
//MemoryStreamPtr MemoryStream::create(const void* buffer, size_t size, bool copy)
//{
//	MemoryStreamPtr ptr(LN_NEW MemoryStream(buffer, size, copy), false);
//	return ptr;
//}

//------------------------------------------------------------------------------
MemoryStream::MemoryStream()
	: m_seekPos(0)
	, m_fixedBuffer(NULL)
	, m_constfixedBuffer(NULL)
	, m_fixedBufferSize(0)
	, m_autoDelete(false)
{
	// 要素が1つ以上無いと [0] にもアクセスできない (assert が発生する)
	m_buffer.resize(1);
}

//------------------------------------------------------------------------------
MemoryStream::MemoryStream(void* buffer, size_t size)
	: m_seekPos(0)
	, m_fixedBuffer(NULL)
	, m_constfixedBuffer(NULL)
	, m_fixedBufferSize(0)
	, m_autoDelete(false)
{
	reset(buffer, size);
}

//------------------------------------------------------------------------------
MemoryStream::MemoryStream(const void* buffer, size_t size, bool copy)
	: m_seekPos(0)
	, m_fixedBuffer(NULL)
	, m_constfixedBuffer(NULL)
	, m_fixedBufferSize(0)
	, m_autoDelete(false)
{
	reset(buffer, size, copy);
}

//------------------------------------------------------------------------------
MemoryStream::~MemoryStream()
{
	if (m_autoDelete) {
		// TODO: AutoDelete は危険。void* にキャストして delete することになってしまう。
		LN_SAFE_DELETE_ARRAY(m_fixedBuffer);
	}
}

//------------------------------------------------------------------------------
void MemoryStream::initialize(size_t uSize)
{
	m_buffer.resize(uSize);
	m_seekPos = 0;
}

//------------------------------------------------------------------------------
void MemoryStream::reset(void* buffer, size_t size)
{
	m_fixedBuffer = buffer;
	m_fixedBufferSize = size;
}

//------------------------------------------------------------------------------
void MemoryStream::reset(const void* buffer, size_t size, bool copy)
{
	if (copy)
	{
		m_fixedBuffer = LN_NEW byte_t[size];
		memcpy_s(m_fixedBuffer, size, buffer, size);
		m_fixedBufferSize = size;
		m_autoDelete = true;
	}
	else
	{
		m_constfixedBuffer = buffer;
		m_fixedBufferSize = size;
	}
}

void* MemoryStream::data() const
{
	byte_t* buf = NULL;
	size_t size = 0;
	if (m_fixedBuffer != NULL) {
		buf = (byte_t*)m_fixedBuffer;
		size = m_fixedBufferSize;
	}
	else {
		buf = (byte_t*)&(m_buffer[0]);
		size = m_buffer.size();
	}

	return buf;
}


const void* MemoryStream::head() const
{
	return static_cast<const byte_t*>(m_constfixedBuffer) + m_seekPos;
}

//------------------------------------------------------------------------------
bool MemoryStream::canRead() const
{
	return true;
}

//------------------------------------------------------------------------------
bool MemoryStream::canWrite() const
{
	return (m_constfixedBuffer == NULL);
}

//------------------------------------------------------------------------------
int64_t MemoryStream::length() const
{
	if (m_fixedBuffer || m_constfixedBuffer) {
		return m_fixedBufferSize;
	}
	else {
		return m_buffer.size();
	}
}

//------------------------------------------------------------------------------
int64_t MemoryStream::position() const
{
	return m_seekPos;
}

//------------------------------------------------------------------------------
size_t MemoryStream::read(void* buffer, size_t byteCount)
{
	// const 固定長のバッファを使用している場合
	if (m_constfixedBuffer != NULL)
	{
		size_t readSize = LN_MIN(byteCount, m_fixedBufferSize - m_seekPos);
		memcpy_s(buffer, byteCount, &(((byte_t*)m_constfixedBuffer)[m_seekPos]), readSize);
		m_seekPos += readSize;
		return readSize;
	}
	// 固定長のバッファを使用している場合
	else  if (m_fixedBuffer != NULL)
	{
		size_t readSize = LN_MIN(byteCount, m_fixedBufferSize - m_seekPos);
		memcpy_s(buffer, byteCount, &(((byte_t*)m_fixedBuffer)[m_seekPos]), readSize);
		m_seekPos += readSize;
		return readSize;
	}
	// 可変長のバッファを使用している場合
	else
	{
		size_t readSize = LN_MIN(byteCount, m_buffer.size());
		void* p = m_buffer.data() + m_seekPos;//&(m_buffer[m_seekPos]);
		memcpy_s(buffer, byteCount, p, readSize);
		m_seekPos += readSize;
		return readSize;
	}
}

//------------------------------------------------------------------------------
void MemoryStream::write(const void* data, size_t byteCount)
{
	size_t newPos = m_seekPos + byteCount;

	// const 固定長のバッファを使用している場合
	if (m_constfixedBuffer != NULL)
	{
		LN_REQUIRE(0);
		return;
	}
	// 固定長のバッファを使用している場合
	else if (m_fixedBuffer != NULL)
	{
		if (newPos > m_fixedBufferSize)
		{
			LN_REQUIRE(0);
			return;
		}
		memcpy_s(static_cast<byte_t*>(m_fixedBuffer) + m_seekPos, m_fixedBufferSize - m_seekPos, data, byteCount);
	}
	// 可変長のバッファを使用している場合
	else
	{
		// 必要があれば拡張
		if (newPos >= m_buffer.size()) {
			m_buffer.resize(newPos + 1);
		}
		void* p = m_buffer.data() + m_seekPos;//&(m_buffer[m_seekPos]);
		memcpy_s(p, m_buffer.size(), data, byteCount);
	}
	m_seekPos = newPos;
}

//------------------------------------------------------------------------------
void MemoryStream::seek(int64_t offset, SeekOrigin origin)
{
	size_t s = (m_fixedBufferSize != 0) ? m_fixedBufferSize : m_buffer.size();
	m_seekPos = (size_t)detail::FileSystemInternal::calcSeekPoint(m_seekPos, s, offset, (int)origin);
}

void MemoryStream::flush()
{
}

} // namespace ln
