
#include <algorithm>
#include "../Internal.h"
#include "../../include/Lumino/Text/EncodingConverter.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// EncodingConverter
//==============================================================================

//------------------------------------------------------------------------------
EncodingConverter::EncodingConverter()
	: m_dstEncoding(NULL)
	, m_srcEncoding(NULL)
	, m_dstEncoder(NULL)
	, m_srcDecoder(NULL)
	, m_encodingModified(false)
{
	m_options.NullTerminated = false;
}

//------------------------------------------------------------------------------
EncodingConverter::~EncodingConverter()
{
	LN_SAFE_DELETE(m_dstEncoder);
	LN_SAFE_DELETE(m_srcDecoder);
}

//------------------------------------------------------------------------------
void EncodingConverter::SetDestinationEncoding(Encoding* encoding)
{
	m_dstEncoding = encoding;
	m_encodingModified = true;
}

//------------------------------------------------------------------------------
Encoding* EncodingConverter::GetDestinationEncoding() const
{
	return m_dstEncoding;
}

//------------------------------------------------------------------------------
void EncodingConverter::SetSourceEncoding(Encoding* encoding)
{
	m_srcEncoding = encoding;
	m_encodingModified = true;
}

//------------------------------------------------------------------------------
Encoding* EncodingConverter::GetSourceEncoding() const
{
	return m_srcEncoding;
}

//------------------------------------------------------------------------------
void EncodingConverter::SetConversionOptions(const EncodingConversionOptions& options)
{
	m_options = options;
}

//------------------------------------------------------------------------------
const ByteBuffer& EncodingConverter::Convert(const void* data, size_t byteCount, EncodingConversionResult* outResult)
{
	CheckUpdateEncoderDecoder();

	// 変換するのに必要なバイト数で領域確保
	size_t size = Encoding::GetConversionRequiredByteCount(m_srcEncoding, m_dstEncoding, byteCount);
	if (m_options.NullTerminated) {
		size += m_dstEncoding->GetMinByteCount();
	}
	m_outputBuffer.Resize(size, false);

	if (m_options.NullTerminated) {
		m_outputBuffer.Clear();
	}

	if (m_srcDecoder->CanRemain())
	{
		//EncodingConversionResult localResult;
		//if (outResult == NULL) { outResult = &localResult; }	// outResult が NULL でも結果を受け取りたい

		ConvertDecoderRemain(data, byteCount, m_srcDecoder, m_outputBuffer.GetData(), size, m_dstEncoder, &m_lastResult);
		m_outputBuffer.Resize(m_lastResult.BytesUsed);	// 余分に確保されているので、見かけ上のサイズを実際に文字のあるサイズにする

		if (outResult != NULL) { *outResult = m_lastResult; }
	}
	// デコーダが変換状態を保持できない場合はやむを得ないので一時メモリを確保し、ソースバッファ全体を一度に変換する。
	else
	{
		// TCHAR を UTF16 へ全部変換するのに必要なバイト数で一時メモリ確保
		size_t totalByteCount = Encoding::GetConversionRequiredByteCount(m_srcEncoding, m_dstEncoding, byteCount);
		m_tmpBuffer.Resize(totalByteCount);

		// 後のコードがキャストだらけにならないように
		UTF16* utf16Buf = (UTF16*)m_tmpBuffer.GetData();
		int utf16ElementCount = m_tmpBuffer.GetSize() / sizeof(UTF16);

		// TCHAR を中間コード(UTF16) へ
		size_t outBytesUsed, outCharsUsed;
		m_srcDecoder->ConvertToUTF16((const byte_t*)data, byteCount, utf16Buf, utf16ElementCount, &outBytesUsed, &outCharsUsed);

		// 中間コード(UTF16)を出力コードへ
		m_dstEncoder->ConvertFromUTF16(utf16Buf, outCharsUsed, m_outputBuffer.GetData(), m_outputBuffer.GetSize(), &outBytesUsed, &outCharsUsed);

		// 余分に確保されているので、見かけ上のサイズを実際に文字のあるサイズにする
		m_outputBuffer.Resize(outBytesUsed);

		m_lastResult.BytesUsed = outBytesUsed;
		m_lastResult.CharsUsed = outCharsUsed;
		m_lastResult.UsedDefaultChar = (m_srcDecoder->UsedDefaultCharCount() > 0 || m_dstEncoder->UsedDefaultCharCount() > 0);
		if (outResult != NULL) 
		{
			outResult->BytesUsed = m_lastResult.BytesUsed;
			outResult->CharsUsed = m_lastResult.CharsUsed;
			outResult->UsedDefaultChar = m_lastResult.UsedDefaultChar;
		}
	}
	return m_outputBuffer;
}

//------------------------------------------------------------------------------
const ByteBuffer& EncodingConverter::GetLastBuffer() const
{
	return m_outputBuffer;
}

//------------------------------------------------------------------------------
const EncodingConversionResult& EncodingConverter::GetLastResult() const
{
	return m_lastResult;
}

//------------------------------------------------------------------------------
void EncodingConverter::CheckUpdateEncoderDecoder()
{
	if (m_encodingModified)
	{
		LN_THROW(m_dstEncoding != NULL, InvalidOperationException, "DestinationEncoding is not set.");
		LN_THROW(m_srcEncoding != NULL, InvalidOperationException, "SourceEncoding is not set.");

		LN_SAFE_DELETE(m_dstEncoder);
		LN_SAFE_DELETE(m_srcDecoder);

		m_dstEncoder = m_dstEncoding->CreateEncoder();
		m_srcDecoder = m_srcEncoding->CreateDecoder();
		m_encodingModified = false;
	}
}

//------------------------------------------------------------------------------
void EncodingConverter::ConvertDecoderRemain(
	const void* src_, size_t srcByteCount, Decoder* srcDecoder,
	void* dest_, size_t destByteCount, Encoder* destEncoder,
	EncodingConversionResult* outResult)
{
	if (LN_CHECK_ARG(srcDecoder != nullptr)) return;
	if (LN_CHECK_ARG(srcDecoder->CanRemain())) return;
	if (LN_CHECK_ARG(destEncoder != nullptr)) return;

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

	if (outResult)
	{
		outResult->BytesUsed = totalBytesUsed;
		outResult->CharsUsed = totalCharsUsed;
		outResult->UsedDefaultChar = (srcDecoder->UsedDefaultCharCount() > 0 || destEncoder->UsedDefaultCharCount() > 0);
	}
}

LN_NAMESPACE_END
