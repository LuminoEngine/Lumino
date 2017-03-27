
#include "../Internal.h"
#include <algorithm>
#include <Lumino/Base/RefObject.h>
#include <Lumino/Base/ByteBuffer.h>

LN_NAMESPACE_BEGIN

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
		LN_SAFE_DELETE_ARRAY(m_buffer);
	}
}
bool ByteBuffer::ByteBufferCore::IsShared() const
{
	return (m_refCount > 1);
}
void ByteBuffer::ByteBufferCore::AddRef()
{
	++m_refCount;
}
void ByteBuffer::ByteBufferCore::Release()
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
	Resize(size, zeroClear);
}

//------------------------------------------------------------------------------
ByteBuffer::ByteBuffer(const void* data, size_t size)
	: m_core(NULL)
	, m_capacity(0)
	, m_size(0)
{
	LN_REFOBJ_SET(m_core, &SharedCoreEmpty);
	Alloc(data, size);
}

//------------------------------------------------------------------------------
ByteBuffer::ByteBuffer(void* data, size_t size, bool refMode)
	: m_core(NULL)
	, m_capacity(0)
	, m_size(0)
{
	LN_REFOBJ_SET(m_core, &SharedCoreEmpty);
	if (refMode) {
		Attach(data, size);
	}
	else {
		Alloc(data, size);
	}
}

//------------------------------------------------------------------------------
ByteBuffer::ByteBuffer(const char* str)
	: m_core(NULL)
	, m_capacity(0)
	, m_size(0)
{
	LN_REFOBJ_SET(m_core, &SharedCoreEmpty);
	Alloc(str, sizeof(char) * strlen(str));		// 終端 NULL はコピーしない (QByteArray と同じ動作)
}

//------------------------------------------------------------------------------
ByteBuffer::ByteBuffer(const wchar_t* str)
	: m_core(NULL)
	, m_capacity(0)
	, m_size(0)
{
	LN_REFOBJ_SET(m_core, &SharedCoreEmpty);
	Alloc(str, sizeof(wchar_t) * wcslen(str));
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
	Dispose();
}

//------------------------------------------------------------------------------
void ByteBuffer::Alloc(size_t size, bool zeroClear)
{
	Dispose();
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
void ByteBuffer::Alloc(const void* data, size_t size)
{
	if (LN_CHECK_ARG(data != nullptr)) return;

	Alloc(size, false);
	memcpy_s(m_core->m_buffer, size, data, size);
}

//------------------------------------------------------------------------------
void ByteBuffer::Resize(size_t size, bool zeroClear)
{
	if (m_core == &SharedCoreEmpty)	// 未割り当てなら new するだけ。
	{
		Alloc(size, zeroClear);
	}
	else
	{
		ByteBufferCore* newBuf = m_core;
		size_t newSize = size;
		size_t newCapacity = m_capacity;
		if (size > m_capacity ||	// サイズが capacity より大きくなるようであれば再確保
			m_core->IsShared())		// 共有している場合も再確保
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
			Dispose();
		}

		// 新しいバッファに差し替え
		m_core = newBuf;
		m_capacity = newCapacity;
		m_size = newSize;
	}
}

//------------------------------------------------------------------------------
void ByteBuffer::Attach(void* buffer, size_t size)
{
	Dispose();
	m_core = LN_NEW ByteBufferCore((byte_t*)buffer);	// 参照モード
	m_capacity = size;
	m_size = size;
}

//------------------------------------------------------------------------------
void ByteBuffer::Copy(const void* data, size_t size)
{
	LN_THROW(size <= m_size, ArgumentException);
	CheckDetachShared();
	memcpy_s(m_core->m_buffer, m_size, data, size);
}

//------------------------------------------------------------------------------
void ByteBuffer::Copy(size_t offset, const void* data, size_t size)
{
	LN_THROW(offset + size <= m_size, ArgumentException);
	CheckDetachShared();
	memcpy_s(m_core->m_buffer + offset, m_size - offset, data, size);
}

//------------------------------------------------------------------------------
void ByteBuffer::Clear()
{
	CheckDetachShared();
	memset(m_core->m_buffer, 0, m_size);
}

//------------------------------------------------------------------------------
void ByteBuffer::Release()
{
	Dispose();
}

//------------------------------------------------------------------------------
bool ByteBuffer::Equals(const ByteBuffer& buffer) const
{
	//if (m_core != buffer.m_core) return false;
	if (Compare(*this, buffer.GetConstData(), buffer.GetSize()) != 0) return false;
	return true;
}

//------------------------------------------------------------------------------
int ByteBuffer::Compare(const ByteBuffer& buf1, const void* buf2, size_t buf2Size, int count)
{
	// TODO: いろいろだめ。
	if (count >= 0)
	{
		if (buf1.GetSize() < count) return -1;
		if (buf2Size < count) return 1;
		//if (buf1.GetSize() > count) return 1;
		//if (buf1.GetSize() == 0) return 0;			// 両方 0
	}
	else
	{
		count = buf1.GetSize();
	}
	return memcmp(buf1.GetConstData(), buf2, count);
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
	LN_THROW(index < m_size, ArgumentException);
	CheckDetachShared();
	return m_core->m_buffer[index];
}
const byte_t& ByteBuffer::operator[](size_t index) const
{
	LN_THROW(index < m_size, ArgumentException);
	return m_core->m_buffer[index];
}

//------------------------------------------------------------------------------
void ByteBuffer::Dispose()
{
	LN_SAFE_RELEASE(m_core);
	m_capacity = 0;
	m_size = 0;
}

//------------------------------------------------------------------------------
void ByteBuffer::CheckDetachShared()
{
	// TODO ↓これおかしい気がする・・・
	if (m_core == &SharedCoreEmpty) { return; }	// 空バッファなら処理する必要は無い

	if (m_core->IsShared())
	{
		ByteBufferCore* newCore = LN_NEW ByteBufferCore(m_capacity);
		memcpy_s(newCore->m_buffer, m_capacity, m_core->m_buffer, m_size);	// copy するのは m_size 分だけでよい。余計な部分までする必要は無い
		LN_REFOBJ_SET(m_core, newCore);
		newCore->Release();
	}
}

LN_NAMESPACE_END
