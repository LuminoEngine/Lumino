
#ifdef _WIN32

#include "Internal.hpp"
#include <Windows.h>
#include <vector>
#include "UnicodeUtils.hpp"
#include "Win32CodePageEncoding.hpp"

namespace ln {

//==============================================================================
// Win32CodePageEncoding

Win32CodePageEncoding::Win32CodePageEncoding(UINT codePage)
{
    BOOL r = ::GetCPInfoEx(codePage, 0, &m_cpInfo);
    if (LN_ENSURE(r != FALSE)) {
        return;
    }

    char buf[32];
    sprintf_s(buf, "cp%u", m_cpInfo.CodePage);

    wchar_t buf2[32];
    for (int i = 0; i < 32; i++) {
        buf2[i] = buf[i];
    }
    m_name = buf2;
}

int Win32CodePageEncoding::getCharacterCount(const void* buffer, size_t bufferSize) const
{
    int count = 0;
    const byte_t* pos = (const byte_t*)buffer;
    const byte_t* end = pos + bufferSize;
    while (pos < end) {
        if (::IsDBCSLeadByteEx(m_cpInfo.CodePage, *pos)) {
            pos++;
        }
        pos++;
        count++;
    }
    return count;
}

int Win32CodePageEncoding::getLeadExtraLength(const void* buffer, size_t bufferSize) const
{
    return (::IsDBCSLeadByteEx(m_cpInfo.CodePage, *((const byte_t*)buffer))) ? 1 : 0;
}

bool Win32CodePageEncoding::convertToUTF16Stateless(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, TextDecodeResult* outResult)
{
    Win32CodePageDecoder decoder(this, m_cpInfo);
    return decoder.convertToUTF16(input, inputByteSize, output, outputElementSize, outResult);
}

//==============================================================================
// Win32CodePageEncoding::Win32CodePageDecoder

Win32CodePageEncoding::Win32CodePageDecoder::Win32CodePageDecoder(TextEncoding* encoding, const CPINFOEX& cpInfo)
    : TextDecoder(encoding)
    , m_codePage(cpInfo.CodePage)
    , m_lastLeadByte(0)
    , m_usedDefaultCharCount(0)
    , m_canRemain(false)
{
    // 2文字マルチバイト文字コードだが、先行バイトの分かるものであれば状態を保持しながら変換ができる。
    if (cpInfo.MaxCharSize == 2 && cpInfo.LeadByte[0] != '\0') {
        m_canRemain = true;
    }
    reset();
}

bool Win32CodePageEncoding::Win32CodePageDecoder::convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, TextDecodeResult* outResult)
{
    outResult->usedByteCount = 0;
    outResult->outputByteCount = 0;
    outResult->outputCharCount = 0;

    if (outputElementSize > 0) {
        output[0] = '\0';
    }

    // 入力が 0 文字の場合は何もしない
    // (MultiByteToWideChar の戻り値がエラーなのか成功なのかわからなくなる)
    if (inputByteSize == 0) {
        return true;
    }

    int convertedWideCount;
    if (m_canRemain) {
        // 前回の先行バイトが保存されていれば、入力バッファの先頭に先行バイトを埋め込んだ新しいバッファを作る。
        // されていなければそのまま input をコピーでバッファを作る。
        std::vector<byte_t> tmpInBuffer;
        if (m_lastLeadByte != '\0') {
            tmpInBuffer.resize(1 + inputByteSize);
            tmpInBuffer[0] = m_lastLeadByte;
            std::copy(input, input + inputByteSize, tmpInBuffer.begin() + 1);
        } else {
            tmpInBuffer.resize(inputByteSize);
            std::copy(input, input + inputByteSize, tmpInBuffer.begin());
        }

        // バッファ全体をチェック。末尾が先行バイトであれば defectCount が 1 でループ終了する。
        size_t defectCount = 0;
        for (size_t i = 0; i < tmpInBuffer.size(); i++) {
            if (defectCount > 0) {
                // ひとつ前の文字が LeadByte だった。2つで1文字。
                defectCount = 0;
            } else {
                if (::IsDBCSLeadByteEx(m_codePage, tmpInBuffer[i]) != FALSE) {
                    // 先行バイト文字である
                    defectCount = 1;
                }
            }
        }

        // バッファ終端が先行バイトだった。先行バイトを保存する
        if (defectCount > 0) {
            m_lastLeadByte = tmpInBuffer[tmpInBuffer.size() - 1];
        } else {
            m_lastLeadByte = '\0';
        }

        // バッファ全てが先行バイトだった。何もしない
        if (defectCount == tmpInBuffer.size()) {
            return true;
        }

        // 変換
        convertedWideCount = ::MultiByteToWideChar(m_codePage, MB_ERR_INVALID_CHARS, (LPCSTR)tmpInBuffer.data(), tmpInBuffer.size() - defectCount, (LPWSTR)output, outputElementSize);
        if (LN_ENSURE(convertedWideCount > 0))
            return false;
    } else {
        convertedWideCount = ::MultiByteToWideChar(m_codePage, MB_ERR_INVALID_CHARS, (LPCSTR)input, inputByteSize, (LPWSTR)output, outputElementSize);
        if (LN_ENSURE(convertedWideCount > 0))
            return false;
    }

    // MultiByteToWideChar じゃ文字数カウントはできないので UnicodeUtils を使う
    int count;
    UTFConversionResult r = UnicodeUtils::getUTF16CharCount((UTF16*)output, convertedWideCount, true, &count);
    if (LN_ENSURE(r == UTFConversionResult_Success))
        return false;

    outResult->usedByteCount = inputByteSize;
    outResult->outputByteCount = convertedWideCount * sizeof(wchar_t);
    outResult->outputCharCount = count;
    return true;
}

//==============================================================================
// Win32CodePageEncoding::Win32CodePageEncoder

Win32CodePageEncoding::Win32CodePageEncoder::Win32CodePageEncoder(TextEncoding* encoding, const CPINFOEX& cpInfo)
    : TextEncoder(encoding)
    , m_codePage(cpInfo.CodePage)
    , m_usedDefaultCharCount(0)
    , m_canRemain(false)
{
    // TODO: 実装の時間無いので今は flase
    m_canRemain = false;
    reset();
}

bool Win32CodePageEncoding::Win32CodePageEncoder::convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, TextEncodeResult* outResult)
{
    outResult->usedElementCount = 0;
    outResult->outputByteCount = 0;
    outResult->outputCharCount = 0;

    if (outputByteSize > 0) {
        output[0] = '\0';
    }

    // 入力が 0 文字の場合は何もしない
    // (WideCharToMultiByte の戻り値がエラーなのか成功なのかわからなくなる)
    if (inputElementSize == 0) {
        return true;
    }

    // マッピングできない文字をデフォルト文字に変換する設定をいろいろ
    DWORD dwFlags = 0;
    char chDefault = (char)encoding()->fallbackReplacementChar();
    BOOL bUsedDefaultChar = FALSE;
    LPCSTR pDefault = NULL;
    if (chDefault != '\0') {
        dwFlags = WC_NO_BEST_FIT_CHARS;
        pDefault = &chDefault;
    }

    int convertedByteCount = 0;
    if (m_canRemain) {
        LN_NOTIMPLEMENTED();
    } else {
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
        if (LN_ENSURE(convertedByteCount > 0))
            return false;
    }

    // WideCharToMultiByte じゃ文字数カウントはできないので UnicodeUtils を使う
    int count;
    UTFConversionResult r = UnicodeUtils::getUTF16CharCount((UTF16*)input, inputElementSize, true, &count);
    if (LN_ENSURE(r == UTFConversionResult_Success))
        return false;

    outResult->usedElementCount = inputElementSize;
    outResult->outputByteCount = convertedByteCount;
    outResult->outputCharCount = count;
    return true;
}

} // namespace ln

#endif
