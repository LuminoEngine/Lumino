
#include "../Internal.h"
#include <Lumino/Base/ByteBuffer.h>
#include "../../include/Lumino/Text/UnicodeUtils.h"
#include "Win32CodePageEncoding.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// Win32CodePageEncoding
//==============================================================================

//------------------------------------------------------------------------------
// この関数の中で String のエンコーディング変換系の処理が呼ばれると無限再帰することがあるので注意
Win32CodePageEncoding::Win32CodePageEncoding(UINT codePage)
{
	BOOL r = ::GetCPInfoEx(codePage, 0, &m_cpInfo);
	if (LN_ENSURE(r != FALSE)) return;
	
	char buf[32];
	sprintf_s(buf, "cp%u", codePage);

	Char buf2[32];
	for (int i = 0; i < 32; i++) buf2[i] = buf[i];
	m_name = buf2;
}

//------------------------------------------------------------------------------
int Win32CodePageEncoding::getCharacterCount(const void* buffer, size_t bufferSize) const
{
	int count = 0;
	const byte_t* pos = (const byte_t*)buffer;
	const byte_t* end = pos + bufferSize;
	while (pos < end)
	{
		if (::IsDBCSLeadByteEx(m_cpInfo.CodePage, *pos)) {
			pos++;
		}
		pos++;
		count++;
	}
	return count;
}

//------------------------------------------------------------------------------
int Win32CodePageEncoding::getLeadExtraLength(const void* buffer, size_t bufferSize) const
{
	return (::IsDBCSLeadByteEx(m_cpInfo.CodePage, *((const byte_t*)buffer))) ? 1 : 0;
}

//==============================================================================
// Win32CodePageEncoding::Win32CodePageDecoder
//==============================================================================

//------------------------------------------------------------------------------
Win32CodePageEncoding::Win32CodePageDecoder::Win32CodePageDecoder(const CPINFOEX& cpInfo)
	: m_codePage(cpInfo.CodePage)
	, m_maxByteCount(cpInfo.MaxCharSize)
	, m_lastLeadByte(0)
	, m_usedDefaultCharCount(0)
	, m_canRemain(false)
{
	// 2文字マルチバイト文字コードだが、先行バイトの分かるものであれば状態を保持しながら変換ができる。
	if (m_maxByteCount == 2 && cpInfo.LeadByte[0] != '\0') {
		m_canRemain = true;
	}
	reset();
}

//------------------------------------------------------------------------------
void Win32CodePageEncoding::Win32CodePageDecoder::convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, size_t* outBytesUsed, size_t* outCharsUsed)
{
	if (outputElementSize > 0) {
		output[0] = '\0';
	}
	*outBytesUsed = 0;
	*outCharsUsed = 0;

	// 入力が 0 文字の場合は何もしない
	// (MultiByteToWideChar の戻り値がエラーなのか成功なのかわからなくなる)
	if (inputByteSize == 0) {
		return;
	}

	int convertedWideCount;
	if (m_canRemain)
	{
		// 前回の先行バイトが保存されていれば、入力バッファの先頭に先行バイトを埋め込んだ新しいバッファを作る。
		// されていなければそのまま input をコピーでバッファを作る。
		ByteBuffer tmpInBuffer;
		if (m_lastLeadByte != '\0')
		{
			tmpInBuffer.resize(1 + inputByteSize);
			tmpInBuffer.copy(0, &m_lastLeadByte, 1);
			tmpInBuffer.copy(1, input, inputByteSize);
		}
		else
		{
			tmpInBuffer.resize(inputByteSize);
			tmpInBuffer.copy(0, input, inputByteSize);
		}

		// バッファ全体をチェック。末尾が先行バイトであれば defectCount が 1 でループ終了する。
		size_t defectCount = 0;
		for (size_t i = 0; i < tmpInBuffer.getSize(); i++)
		{
			if (defectCount > 0) {
				// ひとつ前の文字が LeadByte だった。2つで1文字。
				defectCount = 0;
			}
			else
			{
				if (::IsDBCSLeadByteEx(m_codePage, tmpInBuffer[i]) != FALSE) {
					// 先行バイト文字である
					defectCount = 1;
				}
			}
		}

		// バッファ終端が先行バイトだった。先行バイトを保存する
		if (defectCount > 0) {
			m_lastLeadByte = tmpInBuffer[tmpInBuffer.getSize() - 1];
		}
		else {
			m_lastLeadByte = '\0';
		}

		// バッファ全てが先行バイトだった。何もしない
		if (defectCount == tmpInBuffer.getSize()) {
			return;
		}

		// 変換
		convertedWideCount = ::MultiByteToWideChar(m_codePage, MB_ERR_INVALID_CHARS, (LPCSTR)tmpInBuffer.getConstData(), tmpInBuffer.getSize() - defectCount, (LPWSTR)output, outputElementSize);
		if (LN_ENSURE_ENCODING(convertedWideCount > 0)) return;
	}
	else
	{
		convertedWideCount = ::MultiByteToWideChar(m_codePage, MB_ERR_INVALID_CHARS, (LPCSTR)input, inputByteSize, (LPWSTR)output, outputElementSize);
		if (LN_ENSURE_ENCODING(convertedWideCount > 0)) return;
	}

	// MultiByteToWideChar じゃ文字数カウントはできないので UnicodeUtils を使う
	int count;
	UTFConversionResult r = UnicodeUtils::getUTF16CharCount((UnicodeUtils::UTF16*)output, convertedWideCount, true, &count);
	if (LN_ENSURE_ENCODING(r == UTFConversionResult_Success)) return;

	*outBytesUsed = convertedWideCount * sizeof(wchar_t);
	*outCharsUsed = count;
}

//==============================================================================
// Win32CodePageEncoding::Win32CodePageEncoder
//==============================================================================

//------------------------------------------------------------------------------
Win32CodePageEncoding::Win32CodePageEncoder::Win32CodePageEncoder(const CPINFOEX& cpInfo)
	: m_codePage(cpInfo.CodePage)
	, m_maxByteCount(cpInfo.MaxCharSize)
	, m_usedDefaultCharCount(0)
	, m_canRemain(false)
{
	// TODO: 実装の時間無いので今は flase
	m_canRemain = false;
	reset();
}

//------------------------------------------------------------------------------
void Win32CodePageEncoding::Win32CodePageEncoder::convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, size_t* outBytesUsed, size_t* outCharsUsed)
{
	if (outputByteSize > 0) {
		output[0] = '\0';
	}
	*outBytesUsed = 0;
	*outCharsUsed = 0;

	// 入力が 0 文字の場合は何もしない
	// (WideCharToMultiByte の戻り値がエラーなのか成功なのかわからなくなる)
	if (inputElementSize == 0) {
		return;
	}

	// マッピングできない文字をデフォルト文字に変換する設定をいろいろ
	DWORD dwFlags = 0;
	char chDefault = (char)mFallbackReplacementChar;
	BOOL bUsedDefaultChar = FALSE;
	LPCSTR pDefault = NULL;
	if (chDefault != '\0') {
		dwFlags = WC_NO_BEST_FIT_CHARS;
		pDefault = &chDefault;
	}

	int convertedByteCount = 0;
	if (m_canRemain)
	{
		LN_NOTIMPLEMENTED();
	}
	else
	{
		// 変換
		convertedByteCount = ::WideCharToMultiByte(
			m_codePage,
			dwFlags,
			(const wchar_t*)input,
			inputElementSize,
			(LPSTR)output,
			outputByteSize,
			pDefault,
			&bUsedDefaultChar);
		if (LN_ENSURE_ENCODING(convertedByteCount > 0)) return;
	}

	// WideCharToMultiByte じゃ文字数カウントはできないので UnicodeUtils を使う
	int count;
	UTFConversionResult r = UnicodeUtils::getUTF16CharCount((UnicodeUtils::UTF16*)input, inputElementSize, true, &count);
	if (LN_ENSURE_ENCODING(r == UTFConversionResult_Success)) return;

	*outBytesUsed = convertedByteCount;
	*outCharsUsed = count;
}

LN_NAMESPACE_END
