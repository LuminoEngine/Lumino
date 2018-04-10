
#include "Internal.hpp"
#include <algorithm>
#include <Lumino/Base/RefObject.hpp>
#include <Lumino/Base/Buffer.hpp>
#include "CRTHelper.h"

namespace ln {

//==============================================================================
// ByteBufferCore
//==============================================================================

ByteBuffer::ByteBufferCore::ByteBufferCore(size_t size)
	: m_buffer(LN_NEW byte_t[size])
	, m_refCount(1)
	, m_refMode(false)
{
}
ByteBuffer::ByteBufferCore::ByteBufferCore(byte_t* sharedBuffer)
	: m_buffer(sharedBuffer)
	, m_refCount(1)
	, m_refMode(true)
{
}
ByteBuffer::ByteBufferCore::~ByteBufferCore()
{
	if (!m_refMode) {
		if (m_buffer) {
			delete[] m_buffer;
		}
	}
}
bool ByteBuffer::ByteBufferCore::isShared() const
{
	return (m_refCount > 1);
}
void ByteBuffer::ByteBufferCore::addRef()
{
	++m_refCount;
}
void ByteBuffer::ByteBufferCore::release()
{
	--m_refCount;
	if (m_refCount <= 0)
	{
		if (this != &ByteBuffer::SharedCoreEmpty) {	// グローバル変数として定義されたインスタンスからの解放済み delete 対策
			delete this;
		}
	}
}

static byte_t g_sharedEmpty[1] = { 0 };
ByteBuffer::ByteBufferCore ByteBuffer::SharedCoreEmpty(g_sharedEmpty);

//==============================================================================
// ByteBuffer
//==============================================================================

// 0byte 確保しようとしたときに参照されるバッファ
//		C++ の new は 0byte 確保を許可しており、アドレスを返す。
//		実装は処理系依存で、一般的には 1byte のメモリを確保しているようだった。

//------------------------------------------------------------------------------
ByteBuffer::ByteBuffer()
	: m_core(NULL)
	, m_capacity(0)
	, m_size(0)
{
	LN_REFOBJ_SET(m_core, &SharedCoreEmpty);
}

//------------------------------------------------------------------------------
ByteBuffer::ByteBuffer(size_t size, bool zeroClear)
	: m_core(NULL)
	, m_capacity(0)
	, m_size(0)
{
	LN_REFOBJ_SET(m_core, &SharedCoreEmpty);
	resize(size, zeroClear);
}

//------------------------------------------------------------------------------
ByteBuffer::ByteBuffer(const void* data, size_t size)
	: m_core(NULL)
	, m_capacity(0)
	, m_size(0)
{
	LN_REFOBJ_SET(m_core, &SharedCoreEmpty);
	alloc(data, size);
}

//------------------------------------------------------------------------------
ByteBuffer::ByteBuffer(void* data, size_t size, bool refMode)
	: m_core(NULL)
	, m_capacity(0)
	, m_size(0)
{
	LN_REFOBJ_SET(m_core, &SharedCoreEmpty);
	if (refMode) {
		attach(data, size);
	}
	else {
		alloc(data, size);
	}
}

//------------------------------------------------------------------------------
ByteBuffer::ByteBuffer(const char* str)
	: m_core(NULL)
	, m_capacity(0)
	, m_size(0)
{
	LN_REFOBJ_SET(m_core, &SharedCoreEmpty);
	alloc(str, sizeof(char) * strlen(str));		// 終端 NULL はコピーしない (QByteArray と同じ動作)
}

//------------------------------------------------------------------------------
ByteBuffer::ByteBuffer(const wchar_t* str)
	: m_core(NULL)
	, m_capacity(0)
	, m_size(0)
{
	LN_REFOBJ_SET(m_core, &SharedCoreEmpty);
	alloc(str, sizeof(wchar_t) * wcslen(str));
}

//------------------------------------------------------------------------------
ByteBuffer::ByteBuffer(const ByteBuffer& buffer)
{
	m_core = NULL;
	LN_REFOBJ_SET(m_core, buffer.m_core);
	m_capacity = buffer.m_capacity;
	m_size = buffer.m_size;
}

//------------------------------------------------------------------------------
ByteBuffer::~ByteBuffer()
{
	free();
}

//------------------------------------------------------------------------------
void ByteBuffer::alloc(size_t size, bool zeroClear)
{
	free();
	if (size == 0) {
		m_core = &SharedCoreEmpty;
	}
	else {
		m_core = LN_NEW ByteBufferCore(size);
	}
	m_capacity = size;
	m_size = size;

	// 必要であれば 0 クリア
	if (zeroClear) {
		memset(m_core->m_buffer, 0, m_size);
	}
}

//------------------------------------------------------------------------------
void ByteBuffer::alloc(const void* data, size_t size)
{
	if (LN_REQUIRE(data != nullptr)) return;

	alloc(size, false);
	memcpy_s(m_core->m_buffer, size, data, size);
}

//------------------------------------------------------------------------------
void ByteBuffer::resize(size_t size, bool zeroClear)
{
	if (m_core == &SharedCoreEmpty)	// 未割り当てなら new するだけ。
	{
		alloc(size, zeroClear);
	}
	else
	{
		ByteBufferCore* newBuf = m_core;
		size_t newSize = size;
		size_t newCapacity = m_capacity;
		if (size > m_capacity ||	// サイズが capacity より大きくなるようであれば再確保
			m_core->isShared())		// 共有している場合も再確保
		{
			newBuf = LN_NEW ByteBufferCore(size);
			newCapacity = size;
			// 必要であれば 0 クリア
			if (zeroClear) {
				memset(newBuf->m_buffer, 0, newCapacity);
			}
		}

		// 元のバッファがあればコピー。その後元のバッファを破棄
		if (newBuf != m_core)
		{
			memcpy_s(newBuf->m_buffer, size, m_core->m_buffer, std::min(size, m_size));
			free();
		}

		// 新しいバッファに差し替え
		m_core = newBuf;
		m_capacity = newCapacity;
		m_size = newSize;
	}
}

//------------------------------------------------------------------------------
void ByteBuffer::attach(void* buffer, size_t size)
{
	free();
	m_core = LN_NEW ByteBufferCore((byte_t*)buffer);	// 参照モード
	m_capacity = size;
	m_size = size;
}

//------------------------------------------------------------------------------
void ByteBuffer::copy(const void* data, size_t size)
{
	if (LN_REQUIRE(size <= m_size)) return;
	checkDetachShared();
	memcpy_s(m_core->m_buffer, m_size, data, size);
}

//------------------------------------------------------------------------------
void ByteBuffer::copy(size_t offset, const void* data, size_t size)
{
	if (LN_REQUIRE(offset + size <= m_size)) return;
	checkDetachShared();
	memcpy_s(m_core->m_buffer + offset, m_size - offset, data, size);
}

//------------------------------------------------------------------------------
void ByteBuffer::clear()
{
	checkDetachShared();
	memset(m_core->m_buffer, 0, m_size);
}

//------------------------------------------------------------------------------
void ByteBuffer::free()
{
	LN_SAFE_RELEASE(m_core);
	m_capacity = 0;
	m_size = 0;
}

//------------------------------------------------------------------------------
bool ByteBuffer::equals(const ByteBuffer& buffer) const
{
	//if (m_core != buffer.m_core) return false;
	if (compare(*this, buffer.getConstData(), buffer.getSize()) != 0) return false;
	return true;
}

//------------------------------------------------------------------------------
int ByteBuffer::compare(const ByteBuffer& buf1, const void* buf2, size_t buf2Size, int count)
{
	// TODO: いろいろだめ。
	if (count >= 0)
	{
		if (buf1.getSize() < (size_t)count) return -1;
		if (buf2Size < (size_t)count) return 1;
	}
	else
	{
		count = buf1.getSize();
	}
	return memcmp(buf1.getConstData(), buf2, count);
}

//------------------------------------------------------------------------------
ByteBuffer& ByteBuffer::operator = (const ByteBuffer& right)
{
	LN_REFOBJ_SET(m_core, right.m_core);
	m_capacity = right.m_capacity;
	m_size = right.m_size;
	return (*this);
}

//------------------------------------------------------------------------------
byte_t& ByteBuffer::operator[] (size_t index)
{
	if (LN_REQUIRE(index < m_size)) return m_core->m_buffer[0];	// TODO:
	checkDetachShared();
	return m_core->m_buffer[index];
}
const byte_t& ByteBuffer::operator[](size_t index) const
{
	if (LN_REQUIRE(index < m_size)) return m_core->m_buffer[0];	// TODO:
	return m_core->m_buffer[index];
}

//------------------------------------------------------------------------------
void ByteBuffer::checkDetachShared()
{
	// TODO ↓これおかしい気がする・・・
	if (m_core == &SharedCoreEmpty) { return; }	// 空バッファなら処理する必要は無い

	if (m_core->isShared())
	{
		ByteBufferCore* newCore = LN_NEW ByteBufferCore(m_capacity);
		memcpy_s(newCore->m_buffer, m_capacity, m_core->m_buffer, m_size);	// copy するのは m_size 分だけでよい。余計な部分までする必要は無い
		LN_REFOBJ_SET(m_core, newCore);
		newCore->release();
	}
}

} // namespace ln
