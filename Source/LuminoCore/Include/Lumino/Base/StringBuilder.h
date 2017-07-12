
#pragma once
#include "ByteBuffer.h"
#include "String.h"
#include "Formatter.h"
#include "Locale.h"

LN_NAMESPACE_BEGIN

/**
	@brief
*/
template<typename TChar>
class GenericStringBuilderCore
{
public:
	GenericStringBuilderCore();
	~GenericStringBuilderCore();

public:
	bool isEmpty() const { return m_bufferUsed == 0; }
	void clear();
	void append(const TChar ch);
	void append(const TChar ch, int count);
	void append(const TChar* str, int length);
	void append(const TChar* str);
	//void Append(const GenericString<TChar>& str);		// TODO: GenericString 側が UTF16/32 対応できるまで
	void append(const byte_t* buffer, int byteCount);
	void append(const ByteBuffer& buffer);
	void append(const GenericStringRef<TChar>& str)
	{
		append(str.getBegin(), str.getLength());
	}

	void replace(int start, int length, const TChar* str, int strLength);

	const TChar* c_str() const { return (const TChar*)m_buffer.getConstData(); }
	int getLength() const { return m_bufferUsed / sizeof(TChar); }


private:
	void writeInternal(const TChar* str, int length);

protected:
	ByteBuffer	m_buffer;
	size_t		m_bufferUsed;	///< m_buffer 内の使用中バイト数
};


template<typename TChar>
class GenericStringBuilder
	: public GenericStringBuilderCore<TChar>
{
public:

	GenericString<TChar> toString() const
	{
		m_cache.assignCStr(
			(const TChar*)GenericStringBuilderCore<TChar>::m_buffer.getConstData(),
			GenericStringBuilderCore<TChar>::m_bufferUsed / sizeof(TChar));
		return m_cache;
	}

	//void Append(const GenericString<TChar>& str)
	//{
	//	GenericStringBuilderCore<TChar>::Append(str.c_str(), str.GetLength());
	//}


	template<typename... TArgs>
	void appendFormat(const Locale& locale, const GenericStringRef<TChar>& format, const TArgs&... args);

	void appendFormatInternal(const Locale& locale, const GenericStringRef<TChar>& format, typename detail::FormatList<TChar>* args);

private:
	mutable GenericString<TChar>	m_cache;
};






template<typename TChar>
template<typename... TArgs>
void GenericStringBuilder<TChar>::appendFormat(const Locale& locale, const GenericStringRef<TChar>& format, const TArgs&... args)
{
	auto list = detail::MakeArgList<TChar>(args...);
	appendFormatInternal(locale, format, &list);
}

template<typename TChar>
void GenericStringBuilder<TChar>::appendFormatInternal(const Locale& locale, const GenericStringRef<TChar>& format, typename detail::FormatList<TChar>* args)
{
	const TChar* pos = format.getBegin();
	const TChar* end = format.getEnd();
	TChar ch;
	while (pos < end)
	{
		// { を見つけるまで回るループ
		while (pos < end)
		{
			ch = *pos;
			++pos;

			if (ch == '}')
			{
				if (pos < end && *pos == '}') {	// } のエスケープ "}}"
					++pos;
				}
				else {
					LN_THROW(0, InvalidFormatException);	// 単発の } が現れてはならない
				}
			}
			if (ch == '{')
			{
				if (pos < end && *pos == '{') { 	// { のエスケープ "{{"
					++pos;
				}
				else if (pos >= end) {
					LN_THROW(0, InvalidFormatException);	// { で終わった
				}
				else {
					break;
				}
			}

			GenericStringBuilderCore<TChar>::append(ch);
		}
		// この時点で pos は { の次を指している

		if (pos >= end) { break; }

		// 次の文字は必ず数字でなければならない
		if ('0' <= *pos && *pos <= '9') {
		}
		else {
			LN_THROW(0, InvalidFormatException);
		}

		//-----------------------------------------------------------
		// Index コンポーネント (型引数のインデックスを取り出す)
		int index = 0;
		do
		{
			index = (index * 10) + ((*pos) - '0');
			++pos;
			LN_THROW(pos < end, InvalidFormatException);	// インデックス解析中に \0 になった

		} while ((*pos) >= '0' && (*pos) <= '9');
		LN_THROW(index < args->getCount(), InvalidFormatException);

		//-----------------------------------------------------------
		// Alignment コンポーネント
		while (pos < end && *pos == ' ') { pos++; }	// 先頭の空白を無視
		bool leftJustify = false;					// 左詰めにするか？
		int width = 0;
		if (*pos == ',')	// , であれば解析開始。無ければ省略されている
		{
			pos++;
			while (pos < end && *pos == ' ') { pos++; }
			LN_THROW(pos < end, InvalidFormatException);

			if (*pos == '-')	// 符号が - なら左詰め
			{
				leftJustify = true;
				++pos;
				LN_THROW(pos < end, InvalidFormatException);
			}

			// 次は絶対数字
			LN_THROW('0' <= *pos && *pos <= '9', InvalidFormatException);
			do
			{
				width = (width * 10) + ((*pos) - '0');
				++pos;
				LN_THROW(pos < end, InvalidFormatException);

			} while ((*pos) >= '0' && (*pos) <= '9');
		}

		//-----------------------------------------------------------
		// FormatString コンポーネント
		while (pos < end && *pos == ' ') { pos++; }	// 先頭の空白を無視
		const TChar* fmtBegin = nullptr;
		const TChar* fmtEnd = nullptr;
		const TChar* fmtParamEnd = nullptr;
		if (*pos == ':')
		{
			pos++;
			fmtBegin = pos;
			while (true)
			{
				LN_THROW(pos < end, InvalidFormatException);
				if (*pos == '}') {
					break;
				}
				++pos;
				fmtEnd = pos;
			}

			// "D4" のように後ろが整数のパラメータになっているとき、ここで整数部分を取り出してしまう (変換側が楽できるように)
			ch = *(fmtEnd - 1);
			if (ch >= '0' && ch <= '9')
			{
				fmtParamEnd = fmtEnd;
				do
				{
					--fmtEnd;
					ch = *(fmtEnd - 1);

				} while (ch >= '0' && ch <= '9');
			}
		}

		// 最後は } でなければならない
		LN_THROW(*pos == '}', InvalidFormatException);

		GenericString<TChar> str = args->GetArg(index).DoFormat(locale.getStdLocale(), GenericStringRef<TChar>(fmtBegin, fmtEnd), GenericStringRef<TChar>(fmtEnd, fmtParamEnd));

		int pad = width - str.getLength();
		if (!leftJustify && pad > 0) GenericStringBuilderCore<TChar>::append(' ', pad);
		GenericStringBuilderCore<TChar>::append(str.c_str(), str.getLength());
		if (leftJustify && pad > 0) GenericStringBuilderCore<TChar>::append(' ', pad);

		++pos;
	}
}

typedef GenericStringBuilder<TCHAR>		StringBuilder;
typedef GenericStringBuilder<char>		StringBuilderA;
typedef GenericStringBuilder<wchar_t>	StringBuilderW;



//------------------------------------------------------------------------------
template<typename TChar>
template<typename... TArgs>
GenericString<TChar> GenericString<TChar>::format(const GenericStringRef<TChar>& format, const TArgs&... args)
{
	GenericStringBuilder<TChar> sb;
	sb.appendFormat(Locale::getC(), format, args...);
	return sb.toString();
}

//------------------------------------------------------------------------------
template<typename TChar>
template<typename... TArgs>
GenericString<TChar> GenericString<TChar>::format(const Locale& locale, const GenericStringRef<TChar>& format, const TArgs&... args)
{
	GenericStringBuilder<TChar> sb;
	sb.appendFormat(locale, format, args...);
	return sb.toString();
}

LN_NAMESPACE_END
