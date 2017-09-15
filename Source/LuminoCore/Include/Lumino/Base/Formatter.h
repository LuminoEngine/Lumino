
#pragma once
#include <array>
#include <strstream>
#include <iomanip>
#include "Common.h"

#if defined(_MSC_VER) && _MSC_VER <= 1800 // VS2013
	#define LN_FORMAT_BRACED_INIT_WORKAROUND(x) (x)
#else
	#define LN_FORMAT_BRACED_INIT_WORKAROUND(x) {x}
#endif

LN_NAMESPACE_BEGIN

template<typename TChar, typename TKind, typename TValue>
struct Formatter;


// https://msdn.microsoft.com/ja-jp/library/txafckwd(v=vs.110).aspx#Anchor_1

namespace detail
{


// ostream が直接文字列配列に書き出せるようにする
template<typename TChar>
class StdCharArrayBuffer : public std::basic_streambuf<TChar, std::char_traits<TChar> >
{
public:
	TChar* m_begin;

	StdCharArrayBuffer(TChar* buffer, size_t bufferLength)
	{
		m_begin = buffer;
		TChar* bufferEnd = buffer + bufferLength;
		std::basic_streambuf<TChar, std::char_traits<TChar> >::setp(buffer, bufferEnd);
		std::basic_streambuf<TChar, std::char_traits<TChar> >::setg(buffer, buffer, bufferEnd);
	}

	int getLength() const
	{
		return std::basic_streambuf<TChar, std::char_traits<TChar> >::pptr() - m_begin;
	}

	const TChar* GetCStr()
	{
		*std::basic_streambuf<TChar, std::char_traits<TChar> >::pptr() = '\0';
		return m_begin;
	}
};

} // namespace detail
LN_NAMESPACE_END
