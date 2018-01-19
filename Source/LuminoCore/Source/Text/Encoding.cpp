
#include "../Internal.h"
#include <memory>
#include <algorithm>
#include <Lumino/Base/RefObject.h>
#include <Lumino/Base/ByteBuffer.h>
#include <Lumino/Base/String.h>
#include <Lumino/Text/UnicodeUtils.h>
#include <Lumino/Text/Encoding.h>
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
Encoding* Encoding::getSystemMultiByteEncoding()
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
Encoding* Encoding::getWideCharEncoding()
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
Encoding* Encoding::getTCharEncoding()
{
#ifdef LN_UNICODE
	return getWideCharEncoding();
#else
	return getSystemMultiByteEncoding();
#endif
}

//------------------------------------------------------------------------------
Encoding* Encoding::getUTF8Encoding()
{
	static UTF8Encoding encoding(false);
	return &encoding;
}

//------------------------------------------------------------------------------
Encoding* Encoding::getUTF16Encoding()
{
	static UTF16Encoding encoding(false, false);
	return &encoding;
}

//------------------------------------------------------------------------------
Encoding* Encoding::getUTF32Encoding()
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
		default:
			LN_UNREACHABLE();
			return nullptr;
	}
}

//------------------------------------------------------------------------------
template<>
Encoding* Encoding::getEncodingTemplate<char>()
{
	return getSystemMultiByteEncoding();
}
template<>
Encoding* Encoding::getEncodingTemplate<wchar_t>()
{
	return getWideCharEncoding();
}
template<>
Encoding* Encoding::getEncodingTemplate<char16_t>()
{
	return getUTF16Encoding();
}

//------------------------------------------------------------------------------
#ifdef LN_OS_WIN32
Encoding* Encoding::getWin32DefaultCodePageEncoding()
{
	static Win32CodePageEncoding encoding(CP_THREAD_ACP);
	return &encoding;
}
#endif

//------------------------------------------------------------------------------
size_t Encoding::getConversionRequiredByteCount(Encoding* from, Encoding* to, size_t fromByteCount)
{
	if (LN_REQUIRE(from)) return 0;
	if (LN_REQUIRE(to)) return 0;

	// from に入っている最悪パターンの文字数
	size_t srcMaxCharCount = fromByteCount / from->getMinByteCount();
	srcMaxCharCount += 1;	// Decoder・Encoder の状態保存により前回の余り文字が1つ追加されるかもしれない

	// 出力バッファに必要な最大バイト数
	return srcMaxCharCount * to->getMaxByteCount();
}

//------------------------------------------------------------------------------
ByteBuffer Encoding::convert(
	const void* src, size_t srcByteCount, const Encoding* srcEncoding,
	const Encoding* targetEncoding,
	const EncodingConversionOptions& options,
	EncodingConversionResult* result)
{
	std::unique_ptr<Decoder> decoder(srcEncoding->createDecoder());
	std::unique_ptr<Encoder> encoder(targetEncoding->createEncoder());
	return convert(src, srcByteCount, decoder.get(), encoder.get(), options, result);
}

//------------------------------------------------------------------------------
ByteBuffer Encoding::convert(
	const void* src, size_t srcByteCount, Decoder* decoder,
	Encoder* encoder,
	const EncodingConversionOptions& options,
	EncodingConversionResult* result)
{
	if (LN_REQUIRE(src)) return ByteBuffer();
	if (LN_REQUIRE(decoder)) return ByteBuffer();
	if (LN_REQUIRE(encoder)) return ByteBuffer();

	// src に入っている最悪パターンの文字数
	size_t srcMaxCharCount = srcByteCount / decoder->getMinByteCount();
	srcMaxCharCount += 1;									// Decoder・Encoder の状態保存により前回の余り文字が1つ追加されるかもしれない
	if (options.NullTerminated) { srcMaxCharCount += 1; }	// \0 の分

	// 中間バッファに必要な最大バイト数
	size_t utf16MaxByteCount = srcMaxCharCount * 4;	// UTF16 は1文字最大4バイト

	// 出力バッファに必要な最大バイト数
	size_t outputMaxByteCount = srcMaxCharCount * encoder->getMaxByteCount();

	// 中間バッファ作成
	ByteBuffer tmpBuf(utf16MaxByteCount + sizeof(uint16_t), false);	// 終端 \0 考慮 (mbstowcs_s は \0 を書き込もうとする)

	// 変換先バッファを、最大要素数で確保
	ByteBuffer targetBuf(outputMaxByteCount + encoder->getMaxByteCount(), false);	// 終端 \0 考慮 (mbstowcs_s は \0 を書き込もうとする)

	// 変換実行
	size_t bytesUsed;
	size_t charsUsed;
	// ソースフォーマットから中間フォーマットへ
	decoder->convertToUTF16(
		(const byte_t*)src,
		srcByteCount,
		(UTF16*)tmpBuf.getData(),
		utf16MaxByteCount / sizeof(UTF16),			// \0 強制格納に備え、1文字分余裕のあるサイズを指定する
		&bytesUsed,
		&charsUsed);
	// 中間フォーマットからターゲットフォーマットへ
	encoder->convertFromUTF16(
		(const UTF16*)tmpBuf.getData(),
		bytesUsed / sizeof(UTF16),
		(byte_t*)targetBuf.getData(),
		targetBuf.getSize(),		// \0 強制格納に備え、1文字分余裕のあるサイズを指定する
		&bytesUsed,
		&charsUsed);

	// \0 終端文字
	if (options.NullTerminated)
	{
		size_t nullBytes = encoder->getMinByteCount();
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
		result->UsedDefaultChar = (decoder->usedDefaultCharCount() > 0 || encoder->usedDefaultCharCount() > 0);
	}
	targetBuf.resize(bytesUsed);	// 出力バッファの見かけ上のサイズを、実際に使用したバイト数にする
	return targetBuf;
}

//------------------------------------------------------------------------------
void Encoding::convert(
	const void* src, size_t srcByteCount, Encoding* srcEncoding,
	void* dest, size_t destByteCount, Encoding* destEncoding,
	EncodingConversionResult* result)
{
	// TODO: できればメモリ確保はしたくないが…
	std::unique_ptr<Decoder> decoder(srcEncoding->createDecoder());
	std::unique_ptr<Encoder> encoder(destEncoding->createEncoder());
	return convert(src, srcByteCount, decoder.get(), dest, destByteCount, encoder.get(), result);
}

//------------------------------------------------------------------------------
void Encoding::convert(
	const void* src_, size_t srcByteCount, Decoder* srcDecoder,
	void* dest_, size_t destByteCount, Encoder* destEncoder,
	EncodingConversionResult* result)
{
	if (LN_REQUIRE(srcDecoder != nullptr)) return;
	if (LN_REQUIRE(srcDecoder->canRemain())) return;
	if (LN_REQUIRE(destEncoder != nullptr)) return;
	if (LN_REQUIRE(destEncoder->canRemain())) return;

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
		srcDecoder->convertToUTF16(
			&src[srcPos],
			srcBytes,
			utf16,
			BufferingElements,
			&bytesUsed,
			&charsUsed);
		srcPos += srcBytes;

		// UTF16 文字をターゲットへ
		destEncoder->convertFromUTF16(
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
		result->UsedDefaultChar = (srcDecoder->usedDefaultCharCount() > 0 || destEncoder->usedDefaultCharCount() > 0);
	}
}

String Encoding::fromBytes(const char* bytes, int size, Encoding* encoding, bool* outUsedDefaultChar)
{
	encoding = (encoding) ? encoding : getSystemMultiByteEncoding();

	Encoding* thisTypeEncoding = getTCharEncoding();	// TODO: 名前は StringEncoding とかのほうがいいと思う

	// 全く同じエンコーディングなら変換の必要は無い
	if (thisTypeEncoding == encoding)	// TODO: ポインタ比較はよくない
	{
		int byteCount = (size < 0) ? strlen((const char*)bytes) : size;
		return String((const Char*)bytes, byteCount);
	}
	else
	{
		EncodingConversionOptions options;
		options.NullTerminated = false;

		EncodingConversionResult result;
		const ByteBuffer tmpBuffer = Encoding::convert(bytes, size, encoding, thisTypeEncoding, options, &result);
		if (outUsedDefaultChar != nullptr) {
			*outUsedDefaultChar = result.UsedDefaultChar;
		}

		return String((const Char*)tmpBuffer.getData(), result.BytesUsed / sizeof(Char));
	}
}

String Encoding::fromBytes(const byte_t* bytes, int size, Encoding* encoding, bool* outUsedDefaultChar)
{
	return fromBytes((const char*)bytes, size, encoding, outUsedDefaultChar);
}

Encoding* Encoding::detectEncodingSimple(const char* str, int length, bool strict)
{
	if (LN_REQUIRE(str)) return nullptr;

	// check UTF8 BOM
	if (length >= 3)
	{
		const byte_t bom[] = { 0xEF, 0xBB, 0xBF };
		if (memcmp(str, bom, 3) == 0)
		{
			return getEncoding(EncodingType::UTF8);
		}
	}

	int utf8pt = 0;
	int localPt = 0;
	const uint8_t* s = (const uint8_t*)str;
	const uint8_t* end = (const uint8_t*)str + length;
	int extra = 0;
	while (s < end)
	{
		if (*s == 0x00)
		{
			// 有効範囲内に 0 があった。バイナリかもしれない
			if (strict)
				return nullptr;
			else
				return getEncoding(EncodingType::ASCII);
		}
		else if (*s <= 0x7F)
		{
			// ASCII
			s++;
		}
		else
		{
			if (UnicodeUtils::checkUTF8TrailingBytes((const UTF8*)s, (const UTF8*)end, true, &extra) == UTFConversionResult_Success)
			{
				// UTF8
				utf8pt += 1;
				//s += extra;
			}
			
			{
				// それ以外 (SJIS など)
				localPt++;
			}
			s++;
		}
	}

	if (utf8pt == 0 && localPt == 0)
	{
		return getEncoding(EncodingType::ASCII);
	}
	else if (utf8pt >= localPt)
	{
		return getEncoding(EncodingType::UTF8N);
	}
	else
	{
		return getSystemMultiByteEncoding();
	}
}

//------------------------------------------------------------------------------
size_t Encoding::checkPreamble(const void* buffer, size_t bufferSize) const
{
	if (LN_REQUIRE(buffer == nullptr)) return 0;

	const char* bom = (const char*)getPreamble();
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
