
#include "../Internal.h"
#include <memory>
#include <algorithm>
#include "../../include/Lumino/Base/RefObject.h"
#include "../../include/Lumino/Base/ByteBuffer.h"
#include "../../include/Lumino/Text/UnicodeUtils.h"
#include "../../include/Lumino/Text/Encoding.h"
#include "ASCIIEncoding.h"
#include "UTF8Encoding.h"
#include "UTF16Encoding.h"
#include "UTF32Encoding.h"
#include "DBCSEncoding.h"
#include "EUCJPEncoding.h"
#if defined(LN_OS_WIN32)
#include "Win32CodePageEncoding.h"
#endif

LN_NAMESPACE_BEGIN

//==============================================================================
// Encoding
//==============================================================================

//static const size_t CommonMaxBytes = 6;	///< 全Encoding中、最大の文字バイト数 (UTF8)

//------------------------------------------------------------------------------
Encoding* Encoding::GetSystemMultiByteEncoding()
{
#ifdef LN_OS_WIN32
	static Win32CodePageEncoding systemEncoding(CP_THREAD_ACP);
	return &systemEncoding;
#else
	static UTF8Encoding systemEncoding(false);
	return &systemEncoding;
#endif
}

//------------------------------------------------------------------------------
Encoding* Encoding::GetWideCharEncoding()
{
#if defined(LN_WCHAR_16)
	static UTF16Encoding wideEncoding(false, false);
	return &wideEncoding;
#elif defined(LN_WCHAR_32)
	static UTF32Encoding wideEncoding(false, false);
	return &wideEncoding;
#else
#error "Invalid wchar_t size."
#endif
}

//------------------------------------------------------------------------------
Encoding* Encoding::GetTCharEncoding()
{
#ifdef LN_UNICODE
	return GetWideCharEncoding();
#else
	return GetSystemMultiByteEncoding();
#endif
}

//------------------------------------------------------------------------------
Encoding* Encoding::GetUTF8Encoding()
{
	static UTF8Encoding encoding(false);
	return &encoding;
}

//------------------------------------------------------------------------------
Encoding* Encoding::GetUTF16Encoding()
{
	static UTF16Encoding encoding(false, false);
	return &encoding;
}

//------------------------------------------------------------------------------
Encoding* Encoding::GetUTF32Encoding()
{
	static UTF32Encoding encoding(false, false);
	return &encoding;
}

//------------------------------------------------------------------------------
Encoding* Encoding::getEncoding(EncodingType type)
{
	switch (type)
	{
		case EncodingType::ASCII:
		{
			static ASCIIEncoding asciiEncoding;
			return &asciiEncoding;
		}
		case EncodingType::SJIS:
		{
			static DBCSEncoding sjisEncoding(EncodingType::SJIS);
			return &sjisEncoding;
		}
		case EncodingType::EUCJP:
		{
			static EUCJPEncoding eucjpEncoding;
			return &eucjpEncoding;
		}
		case EncodingType::GB2312:
		{
			static DBCSEncoding gb2312Encoding(EncodingType::GB2312);
			return &gb2312Encoding;
		}
		case EncodingType::EUCKR:
		{
			static DBCSEncoding euckrEncoding(EncodingType::EUCKR);
			return &euckrEncoding;
		}
		case EncodingType::BIG5:
		{
			static DBCSEncoding big5Encoding(EncodingType::BIG5);
			return &big5Encoding;
		}
		case EncodingType::UTF8:
		{
			static UTF8Encoding utf8BOMEncoding(true);
			return &utf8BOMEncoding;
		}
	}
	LN_THROW(0, ArgumentException);
}

//------------------------------------------------------------------------------
template<>
Encoding* Encoding::GetEncodingTemplate<char>()
{
	return GetSystemMultiByteEncoding();
}
template<>
Encoding* Encoding::GetEncodingTemplate<wchar_t>()
{
	return GetWideCharEncoding();
}

//------------------------------------------------------------------------------
#ifdef LN_OS_WIN32
Encoding* Encoding::GetWin32DefaultCodePageEncoding()
{
	static Win32CodePageEncoding encoding(CP_THREAD_ACP);
	return &encoding;
}
#endif

//------------------------------------------------------------------------------
size_t Encoding::GetConversionRequiredByteCount(Encoding* from, Encoding* to, size_t fromByteCount)
{
	LN_THROW(from != NULL, ArgumentException);
	LN_THROW(to != NULL, ArgumentException);

	// from に入っている最悪パターンの文字数
	size_t srcMaxCharCount = fromByteCount / from->GetMinByteCount();
	srcMaxCharCount += 1;	// Decoder・Encoder の状態保存により前回の余り文字が1つ追加されるかもしれない

	// 出力バッファに必要な最大バイト数
	return srcMaxCharCount * to->GetMaxByteCount();
}

//------------------------------------------------------------------------------
ByteBuffer Encoding::Convert(
	const void* src, size_t srcByteCount, const Encoding* srcEncoding,
	const Encoding* targetEncoding,
	const EncodingConversionOptions& options,
	EncodingConversionResult* result)
{
	std::unique_ptr<Decoder> decoder(srcEncoding->CreateDecoder());
	std::unique_ptr<Encoder> encoder(targetEncoding->CreateEncoder());
	return Convert(src, srcByteCount, decoder.get(), encoder.get(), options, result);
}

//------------------------------------------------------------------------------
ByteBuffer Encoding::Convert(
	const void* src, size_t srcByteCount, Decoder* decoder,
	Encoder* encoder,
	const EncodingConversionOptions& options,
	EncodingConversionResult* result)
{
	LN_THROW(src != NULL, ArgumentException);
	LN_THROW(decoder != NULL, ArgumentException);
	LN_THROW(encoder != NULL, ArgumentException);

	// src に入っている最悪パターンの文字数
	size_t srcMaxCharCount = srcByteCount / decoder->GetMinByteCount();
	srcMaxCharCount += 1;									// Decoder・Encoder の状態保存により前回の余り文字が1つ追加されるかもしれない
	if (options.NullTerminated) { srcMaxCharCount += 1; }	// \0 の分

	// 中間バッファに必要な最大バイト数
	size_t utf16MaxByteCount = srcMaxCharCount * 4;	// UTF16 は1文字最大4バイト

	// 出力バッファに必要な最大バイト数
	size_t outputMaxByteCount = srcMaxCharCount * encoder->GetMaxByteCount();

	// 中間バッファ作成
	ByteBuffer tmpBuf(utf16MaxByteCount + sizeof(uint16_t), false);	// 終端 \0 考慮 (mbstowcs_s は \0 を書き込もうとする)

	// 変換先バッファを、最大要素数で確保
	ByteBuffer targetBuf(outputMaxByteCount + encoder->GetMaxByteCount(), false);	// 終端 \0 考慮 (mbstowcs_s は \0 を書き込もうとする)

	// 変換実行
	size_t bytesUsed;
	size_t charsUsed;
	// ソースフォーマットから中間フォーマットへ
	decoder->ConvertToUTF16(
		(const byte_t*)src,
		srcByteCount,
		(UTF16*)tmpBuf.getData(),
		utf16MaxByteCount / sizeof(UTF16),			// \0 強制格納に備え、1文字分余裕のあるサイズを指定する
		&bytesUsed,
		&charsUsed);
	// 中間フォーマットからターゲットフォーマットへ
	encoder->ConvertFromUTF16(
		(const UTF16*)tmpBuf.getData(),
		bytesUsed / sizeof(UTF16),
		(byte_t*)targetBuf.getData(),
		targetBuf.getSize(),		// \0 強制格納に備え、1文字分余裕のあるサイズを指定する
		&bytesUsed,
		&charsUsed);

	// \0 終端文字
	if (options.NullTerminated)
	{
		size_t nullBytes = encoder->GetMinByteCount();
		byte_t* buf = (byte_t*)targetBuf.getData();
		for (size_t i = 0; i < nullBytes; ++i) {
			buf[bytesUsed + i] = 0x00;
		}
		bytesUsed += nullBytes;
	}

	if (result)
	{
		result->BytesUsed = bytesUsed;
		result->CharsUsed = charsUsed;
		result->UsedDefaultChar = (decoder->UsedDefaultCharCount() > 0 || encoder->UsedDefaultCharCount() > 0);
	}
	targetBuf.resize(bytesUsed);	// 出力バッファの見かけ上のサイズを、実際に使用したバイト数にする
	return targetBuf;
}

//------------------------------------------------------------------------------
void Encoding::Convert(
	const void* src, size_t srcByteCount, Encoding* srcEncoding,
	void* dest, size_t destByteCount, Encoding* destEncoding,
	EncodingConversionResult* result)
{
	// TODO: できればメモリ確保はしたくないが…
	std::unique_ptr<Decoder> decoder(srcEncoding->CreateDecoder());
	std::unique_ptr<Encoder> encoder(destEncoding->CreateEncoder());
	return Convert(src, srcByteCount, decoder.get(), dest, destByteCount, encoder.get(), result);
}

//------------------------------------------------------------------------------
void Encoding::Convert(
	const void* src_, size_t srcByteCount, Decoder* srcDecoder,
	void* dest_, size_t destByteCount, Encoder* destEncoder,
	EncodingConversionResult* result)
{
	if (LN_CHECK_ARG(srcDecoder != nullptr)) return;
	if (LN_CHECK_ARG(srcDecoder->CanRemain())) return;
	if (LN_CHECK_ARG(destEncoder != nullptr)) return;
	if (LN_CHECK_ARG(destEncoder->CanRemain())) return;

	const size_t BufferingElements = 512;
	UTF16 utf16[BufferingElements];
	size_t totalBytesUsed = 0;
	size_t totalCharsUsed = 0;
	size_t bytesUsed;
	size_t charsUsed;
	const byte_t* src = (const byte_t*)src_;
	byte_t* dest = (byte_t*)dest_;
	size_t srcPos = 0;
	size_t destPos = 0;

	for (;;)
	{
		if (srcPos >= srcByteCount || destPos >= destByteCount) {
			break;
		}

		// UTF16 へ
		size_t srcBytes = std::min(srcByteCount - srcPos, BufferingElements);
		srcDecoder->ConvertToUTF16(
			&src[srcPos],
			srcBytes,
			utf16,
			BufferingElements,
			&bytesUsed,
			&charsUsed);
		srcPos += srcBytes;

		// UTF16 文字をターゲットへ
		destEncoder->ConvertFromUTF16(
			utf16,
			bytesUsed / sizeof(UTF16),
			&dest[destPos],
			destByteCount - destPos,
			&bytesUsed,
			&charsUsed);
		destPos += bytesUsed;

		totalBytesUsed += bytesUsed;
		totalCharsUsed += charsUsed;
	}

	if (result)
	{
		result->BytesUsed = totalBytesUsed;
		result->CharsUsed = totalCharsUsed;
		result->UsedDefaultChar = (srcDecoder->UsedDefaultCharCount() > 0 || destEncoder->UsedDefaultCharCount() > 0);
	}
}

//------------------------------------------------------------------------------
size_t Encoding::CheckPreamble(const void* buffer, size_t bufferSize) const
{
	if (LN_CHECK_ARG(buffer == nullptr)) return 0;

	const char* bom = (const char*)GetPreamble();
	size_t bomLen = strlen(bom);
	if (bufferSize < bomLen) {
		return 0;
	}

	if (memcmp(buffer, bom, bomLen) == 0) {
		return bomLen;
	}
	return 0;
}

LN_NAMESPACE_END
