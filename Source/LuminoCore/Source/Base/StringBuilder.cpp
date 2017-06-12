
#include "../Internal.h"
#include <Lumino/Base/String.h>
#include <Lumino/Base/StringBuilder.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// GenericStringBuilderCore
//==============================================================================

const int DefaultCapacity = 512;

//------------------------------------------------------------------------------
template<typename TChar>
GenericStringBuilderCore<TChar>::GenericStringBuilderCore()
	: m_buffer()
	, m_bufferUsed(0)
{
	m_buffer.resize(DefaultCapacity, false);
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericStringBuilderCore<TChar>::~GenericStringBuilderCore()
{
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericStringBuilderCore<TChar>::clear()
{
	m_bufferUsed = 0;
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericStringBuilderCore<TChar>::append(const TChar ch)
{
	append(&ch, 1);
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericStringBuilderCore<TChar>::append(const TChar ch, int count)
{
	for (int i = 0; i < count; ++i) {
		append(ch);
	}
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericStringBuilderCore<TChar>::append(const TChar* str, int length)
{
	//if (str == NULL || length <= 0) { return; }	// コピーの必要無し
	writeInternal(str, (length < 0) ? StringTraits::tcslen(str) : length);
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericStringBuilderCore<TChar>::append(const TChar* str)
{
	writeInternal(str, StringTraits::tcslen(str));
}

//------------------------------------------------------------------------------
//template<typename TChar>
//void GenericStringBuilderCore<TChar>::Append(const GenericString<TChar>& str)
//{
//	Append(str.c_str(), str.GetLength());
//}
//
//------------------------------------------------------------------------------
template<typename TChar>
void GenericStringBuilderCore<TChar>::append(const byte_t* buffer, int byteCount)
{
	append((const TChar*)buffer, byteCount / sizeof(TChar));
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericStringBuilderCore<TChar>::append(const ByteBuffer& buffer)
{
	append(buffer.getData(), buffer.getSize());
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericStringBuilderCore<TChar>::replace(int start, int length, const TChar* str, int strLength)
{
	// 置換したら何 byte 増える？減る？
	int diff = sizeof(TChar) * (strLength - length);

	// バッファが足りなければ拡張する
	if (diff > 0 && m_bufferUsed + diff > m_buffer.getSize())
	{
		size_t newSize = m_buffer.getSize() + std::max(m_buffer.getSize(), (size_t)diff);
		m_buffer.resize(newSize, false);
	}

	int diffChars = (strLength - length);
	// before は置換しない部分
	TChar* beforeBegin = (TChar*)m_buffer.getData();
	TChar* beforeEnd = beforeBegin + start;				// この1つ前までが before の文字。全て置換する場合は beforeBegin と同じ。
	
	// oldAfter は残す部分
	TChar* oldAfterBegin = beforeEnd + length;
	TChar* oldAfterEnd = beforeBegin + getLength();		// この1つ前までが after の文字
	int afterCount = oldAfterEnd - oldAfterBegin;

	// newAfter は oldAfter の移動先 (文字数は oldAfter と同じ)
	TChar* newAfterBegin = oldAfterBegin + diffChars;
	//TChar* newAfterEnd = newAfterBegin + afterCount;	// この1つ前までが after の文字

	if (afterCount > 0) {
		memmove(newAfterBegin, oldAfterBegin, sizeof(TChar) * afterCount);
	}
	if (strLength > 0) {
		memcpy(beforeEnd, str, sizeof(TChar) * strLength);
	}

	m_bufferUsed += diff;
}

//------------------------------------------------------------------------------
//template<typename TChar>
//GenericString<TChar> GenericStringBuilderCore<TChar>::ToString() const
//{
//	return GenericString<TChar>((const TChar*)m_buffer.GetConstData(), m_bufferUsed / sizeof(TChar));
//}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericStringBuilderCore<TChar>::writeInternal(const TChar* str, int length)
{
	LN_ASSERT(str != NULL);

	size_t byteCount = sizeof(TChar) * length;

	// バッファが足りなければ拡張する
	if (m_bufferUsed + byteCount > m_buffer.getSize())
	{
		size_t newSize = m_buffer.getSize() + std::max(m_buffer.getSize(), byteCount);	// 最低でも byteCount 分を拡張する
		m_buffer.resize(newSize, false);
	}

	// 末尾にコピー
	byte_t* ptr = &(m_buffer.getData()[m_bufferUsed]);
	size_t size = m_buffer.getSize() - m_bufferUsed;
	memcpy_s(ptr, size, str, byteCount);

	m_bufferUsed += byteCount;
}

// テンプレートのインスタンス化
template class GenericStringBuilderCore<char>;
template class GenericStringBuilderCore<wchar_t>;
#ifdef LN_WCHAR_16	// GCC で↑の wchar_t のと多重定義になる対策
template class GenericStringBuilderCore<UTF32>;
#endif




LN_NAMESPACE_END
