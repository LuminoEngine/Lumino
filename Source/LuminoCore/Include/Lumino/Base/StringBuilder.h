
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
	bool IsEmpty() const { return m_bufferUsed == 0; }
	void clear();
	void Append(const TChar ch);
	void Append(const TChar ch, int count);
	void Append(const TChar* str, int length);
	void Append(const TChar* str);
	//void Append(const GenericString<TChar>& str);		// TODO: GenericString 側が UTF16/32 対応できるまで
	void Append(const byte_t* buffer, int byteCount);
	void Append(const ByteBuffer& buffer);
	void Append(const GenericStringRef<TChar>& str)
	{
		Append(str.GetBegin(), str.GetLength());
	}

	void Replace(int start, int length, const TChar* str, int strLength);

	const TChar* c_str() const { return (const TChar*)m_buffer.getConstData(); }
	int GetLength() const { return m_bufferUsed / sizeof(TChar); }


private:
	void WriteInternal(const TChar* str, int length);

protected:
	ByteBuffer	m_buffer;
	size_t		m_bufferUsed;	///< m_buffer 内の使用中バイト数
};


template<typename TChar>
class GenericStringBuilder
	: public GenericStringBuilderCore<TChar>
{
public:

	GenericString<TChar> ToString() const
	{
		m_cache.AssignCStr(
			(const TChar*)GenericStringBuilderCore<TChar>::m_buffer.getConstData(),
			GenericStringBuilderCore<TChar>::m_bufferUsed / sizeof(TChar));
		return m_cache;
	}

	//void Append(const GenericString<TChar>& str)
	//{
	//	GenericStringBuilderCore<TChar>::Append(str.c_str(), str.GetLength());
	//}


	template<typename... TArgs>
	void AppendFormat(const Locale& locale, const GenericStringRef<TChar>& format, const TArgs&... args);

	void AppendFormatInternal(const Locale& locale, const GenericStringRef<TChar>& format, typename detail::FormatList<TChar>* args);

private:
	mutable GenericString<TChar>	m_cache;
};






template<typename TChar>
template<typename... TArgs>
void GenericStringBuilder<TChar>::AppendFormat(const Locale& locale, const GenericStringRef<TChar>& format, const TArgs&... args)
{
	auto list = detail::MakeArgList<TChar>(args...);
	AppendFormatInternal(locale, format, &list);
}

template<typename TChar>
void GenericStringBuilder<TChar>::AppendFormatInternal(const Locale& locale, const GenericStringRef<TChar>& format, typename detail::FormatList<TChar>* args)
{
	const TChar* pos = format.GetBegin();
	const TChar* end = format.GetEnd();
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

			GenericStringBuilderCore<TChar>::Append(ch);
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
		LN_THROW(index < args->GetCount(), InvalidFormatException);

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

		GenericString<TChar> str = args->GetArg(index).DoFormat(locale.GetStdLocale(), GenericStringRef<TChar>(fmtBegin, fmtEnd), GenericStringRef<TChar>(fmtEnd, fmtParamEnd));

		int pad = width - str.GetLength();
		if (!leftJustify && pad > 0) GenericStringBuilderCore<TChar>::Append(' ', pad);
		GenericStringBuilderCore<TChar>::Append(str.c_str(), str.GetLength());
		if (leftJustify && pad > 0) GenericStringBuilderCore<TChar>::Append(' ', pad);

		++pos;
	}
}

typedef GenericStringBuilder<TCHAR>		StringBuilder;
typedef GenericStringBuilder<char>		StringBuilderA;
typedef GenericStringBuilder<wchar_t>	StringBuilderW;



//------------------------------------------------------------------------------
template<typename TChar>
template<typename... TArgs>
GenericString<TChar> GenericString<TChar>::Format(const GenericStringRef<TChar>& format, const TArgs&... args)
{
	GenericStringBuilder<TChar> sb;
	sb.AppendFormat(Locale::GetC(), format, args...);
	return sb.ToString();
}

//------------------------------------------------------------------------------
template<typename TChar>
template<typename... TArgs>
GenericString<TChar> GenericString<TChar>::Format(const Locale& locale, const GenericStringRef<TChar>& format, const TArgs&... args)
{
	GenericStringBuilder<TChar> sb;
	sb.AppendFormat(locale, format, args...);
	return sb.ToString();
}

LN_NAMESPACE_END
