/*
	[2015/11/6]
		WriteLine() のオーバーロードは、WriteLineString() や WriteLineInt() のように型で名前を分けない。
		WriteLine() は 与えられたデータの文字列表現を書き込むだけで、
		数値の場合はプレフィックスをつけるような型別の特殊な処理は行わない。
		あくまで ToString() のような余計な呼び出しを書きたくないためのユーティリティ。
		ちなみに Qt は QTextStream::operator<<。
*/
#include "../Internal.h"
#include <Lumino/Base/String.h>
#include <Lumino/IO/TextWriter.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// TextWriter
//==============================================================================

//------------------------------------------------------------------------------
TextWriter::TextWriter()
	: m_newLine(String::getNewLine())
	, m_locale()
	//, m_utf16Buffer(BufferSize, false)
	, m_writtenPreamble(true)
{
	// String を中間文字コード (UTF16) に変換するためのデコーダ
	//m_decoder.Attach(Encoding::GetTCharEncoding()->CreateDecoder());

	// デフォルト Encoding
	m_converter.SetSourceEncoding(Encoding::GetTCharEncoding());
	m_converter.SetDestinationEncoding(Encoding::GetTCharEncoding());
}

//------------------------------------------------------------------------------
TextWriter::~TextWriter()
{
}

//------------------------------------------------------------------------------
void TextWriter::setEncoding(Encoding* encoding)
{
	m_converter.SetDestinationEncoding(encoding);
#if 0
	m_encoding = encoding;

	if (m_encoding != NULL)
	{
		m_encoder.Attach(m_encoding->CreateEncoder());

		// 中間バッファを指定エンコーディングに全て変換したときに必要になる最大バッファサイズを計算し、メモリ確保
		size_t maxSize = Encoding::GetConversionRequiredByteCount(Text::Encoding::GetUTF16Encoding(), m_encoding, m_utf16Buffer.GetSize());
		m_outputBuffer.Resize(maxSize, false);

		// TCHAR → 中間バッファ (UTF16) 時、中間バッファに納められる TCHAR 文字数
		m_safeTCharCount = m_utf16Buffer.GetSize() / Encoding::GetTCharEncoding()->GetMaxByteCount();

		// BOM が必要か
		if (m_encoding->GetPreamble() != NULL) {
			m_writtenPreamble = false;
		}
		else {
			m_writtenPreamble = true;	// 既に BOM を書き込んだことにする
		}
		
	}
#endif
}

//------------------------------------------------------------------------------
Encoding* TextWriter::getEncoding() const
{
	return m_converter.GetDestinationEncoding();
}

//------------------------------------------------------------------------------
void TextWriter::setNewLine(const String& newLine)
{
	m_newLine = newLine;
}

//------------------------------------------------------------------------------
void TextWriter::setFormatLocale(const Locale& locale)
{
	m_locale = locale;
}

////------------------------------------------------------------------------------
//void TextWriter::Write(const TCHAR* str, int len)
//{
//	WriteInternal(str, (len < 0) ? StringTraits::StrLen(str) : len);
//}
//
////------------------------------------------------------------------------------
//void TextWriter::Write(const String& str)
//{
//	WriteInternal(str.c_str(), str.GetLength());
//}
//
//
//	/**
//		@brief		書式を指定して文字列を書き込みます。
//		@param[in]	str		: 書式指定文字列
//		@param[in]	...		: 引数リスト
//		@details	注意点等は String::Format() を参照してください。
//	*/
////	void WriteFormat(const TCHAR* format, ...);
////void TextWriter::WriteFormat(const TCHAR* format, ...)
////{
////	LN_THROW(0, NotImplementedException);
////}

//------------------------------------------------------------------------------
void TextWriter::write(TCHAR ch)
{
	writeInternal(&ch, 1);
}
void TextWriter::write(int16_t value)
{
	TCHAR buf[64];
	int len = StringTraits::sprintf(buf, 64, _T("%d"), value);
	writeInternal(buf, len);
}
void TextWriter::write(int32_t value)
{
	TCHAR buf[64];
	int len = StringTraits::sprintf(buf, 64, _T("%d"), value);
	writeInternal(buf, len);
}
void TextWriter::write(int64_t value)
{
	TCHAR buf[64];
	int len = StringTraits::sprintf(buf, 64, _T("%lld"), value);
	writeInternal(buf, len);
}
//void Write(byte_t value);
//void TextWriter::WriteByte(byte_t value)
//{
//	TCHAR buf[64];
//	int len = StringTraits::SPrintf(buf, 64, _T("%u"), value);
//	WriteInternal(buf, len);
//}
void TextWriter::write(uint16_t value)
{
	TCHAR buf[64];
	int len = StringTraits::sprintf(buf, 64, _T("%u"), value);
	writeInternal(buf, len);
}
void TextWriter::write(uint32_t value)
{
	TCHAR buf[64];
	int len = StringTraits::sprintf(buf, 64, _T("%u"), value);
	writeInternal(buf, len);
}
void TextWriter::write(uint64_t value)
{
	TCHAR buf[64];
	int len = StringTraits::sprintf(buf, 64, _T("%llu"), value);
	writeInternal(buf, len);
}
void TextWriter::write(float value)
{
	TCHAR buf[64];
	int len = StringTraits::tsnprintf_l(buf, 64, _T("%f"), m_locale.getNativeLocale(), value);
	writeInternal(buf, len);
}
void TextWriter::write(double value)
{
	// TODO: 64桁以上だと失敗する
	TCHAR buf[64];
	int len = StringTraits::tsnprintf_l(buf, 64, _T("%lf"), m_locale.getNativeLocale(), value);
	if (LN_CHECK_STATE(len > 0)) return;
	writeInternal(buf, len);
}

//------------------------------------------------------------------------------
void TextWriter::write(const StringRef& str)
{
	writeInternal(str.getBegin(), str.getLength());
}

//------------------------------------------------------------------------------
void TextWriter::write(const TCHAR* str, int length)
{
	writeInternal(str, length);
}

//------------------------------------------------------------------------------
void TextWriter::writeLine()
{
	writeInternal(m_newLine.c_str(), m_newLine.getLength());
}
//void TextWriter::WriteLine(const StringRef& str)
//{
//	Write(str.GetBegin(), str.GetLength());
//	WriteLine();
//}
//void TextWriter::WriteLine(const String& str)
//{
//	Write(str);
//	WriteLine();
//}

//------------------------------------------------------------------------------

	/**
		@brief		書式を指定して文字列を書き込み、続けて改行を書き込みます。
		@param[in]	str		: 書式指定文字列
		@param[in]	...		: 引数リスト
		@details	注意点等は String::Format() を参照してください。
	*/
//	void WriteLineFormat(const TCHAR* format, ...);
//void TextWriter::WriteLineFormat(const TCHAR* format, ...)
//{
//	LN_THROW(0, NotImplementedException);
//}

//------------------------------------------------------------------------------
void TextWriter::writeLine(TCHAR value)
{
	write(value);
	writeLine();
}
void TextWriter::writeLine(int16_t value)
{
	write(value);
	writeLine();
}
void TextWriter::writeLine(int32_t value)
{
	write(value);
	writeLine();
}
void TextWriter::writeLine(int64_t value)
{
	write(value);
	writeLine();
}
//void WriteLine(byte_t value);
//void TextWriter::WriteLine(uint8_t value)
//{
//	Write(value);
//	WriteLine();
//}
void TextWriter::writeLine(uint16_t value)
{
	write(value);
	writeLine();
}
void TextWriter::writeLine(uint32_t value)
{
	write(value);
	writeLine();
}
void TextWriter::writeLine(uint64_t value)
{
	write(value);
	writeLine();
}
void TextWriter::writeLine(float value)
{
	write(value);
	writeLine();
}
void TextWriter::writeLine(double value)
{
	write(value);
	writeLine();
}

//------------------------------------------------------------------------------
void TextWriter::writeInternal(const TCHAR* str, int len)
{
	// BOM の書き込みが必要であればここで書き込む
	if (!m_writtenPreamble)
	{
		const byte_t* bom = m_converter.GetDestinationEncoding()->GetPreamble();
		size_t len = strlen((char*)bom);
		writeOverride(bom, len);
		m_writtenPreamble = true;
	}

	// 入力が空なら特にすることは無い
	if (str == NULL || len == 0) {
		return;
	}

	const ByteBuffer buf = m_converter.Convert(str, len * sizeof(TCHAR));

	writeOverride(buf.getConstData(), buf.getSize());

#if 0

	if (m_decoder != NULL && m_encoder != NULL)
	{
		// 変換状態を保持できる Encoding であれば余分にメモリを確保しないで変換できる。
		if (m_decoder->CanRemain()/* && m_encoder->CanRemain()*/)	// encoder 側は状態保存できなくても良い
		{
			// 後のコードがキャストだらけにならないように
			UTF16* utf16Buf = (Text::UTF16*)m_utf16Buffer.GetData();
			int utf16ElementCount = m_utf16Buffer.GetSize() / sizeof(UTF16);

			int convCount = 0;
			while (convCount < len)
			{
				int charCount = std::min(len - convCount, m_safeTCharCount);

				// TCHAR を中間コードへ
				size_t outBytesUsed, outCharsUsed;
				m_decoder->ConvertToUTF16((byte_t*)&str[convCount], charCount * sizeof(TCHAR), utf16Buf, utf16ElementCount, &outBytesUsed, &outCharsUsed);

				// 中間コードを出力コードへ
				m_encoder->ConvertFromUTF16(utf16Buf, outCharsUsed, m_outputBuffer.GetData(), m_outputBuffer.GetSize(), &outBytesUsed, &outCharsUsed);

				// 出力
				WriteOverride(m_outputBuffer.GetData(), outBytesUsed);

				convCount += charCount;
			}
		}
		// デコーダが変換状態を保持できない場合はやむを得ないので一時メモリを確保し、ソースバッファ全体を一度に変換する。
		else
		{
			// 出力
			WriteOverride(m_outputBuffer.GetData(), outBytesUsed);
		}
	}
	// エンコーダが無い (SetEncoding() されていない) 場合は、文字コード変換を行わず TCHAR をそのまま出力する
	else if (m_encoder == NULL)
	{
		WriteOverride(str, len * sizeof(TCHAR));
	}
	else {
		LN_THROW(0, NotImplementedException);
	}
#endif
}


LN_NAMESPACE_END
