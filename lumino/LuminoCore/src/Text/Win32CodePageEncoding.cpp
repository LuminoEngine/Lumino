
#ifdef _WIN32

#include "Internal.hpp"
#include <Windows.h>
#include <vector>
#include <LuminoCore/Base/UnicodeUtils.hpp>
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
    m_name = String::fromCString(buf2);
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
#if LN_USTRING32
bool Win32CodePageEncoding::convertToUTF32Stateless(const byte_t* input, size_t inputByteSize, UTF32* output, size_t outputElementSize, TextDecodeResult* outResult)
{
    Win32CodePageDecoder decoder(this, &m_cpInfo);
    return decoder.convertToUTF32(input, inputByteSize, output, outputElementSize, outResult);
}
#else
bool Win32CodePageEncoding::convertToUTF16Stateless(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, TextDecodeResult* outResult)
{
    Win32CodePageDecoder decoder(this, m_cpInfo);
    return decoder.convertToUTF16(input, inputByteSize, output, outputElementSize, outResult);
}
#endif

//==============================================================================
// Win32CodePageEncoding::Win32CodePageDecoder

Win32CodePageEncoding::Win32CodePageDecoder::Win32CodePageDecoder(TextEncoding* encoding, const CPINFOEX* cpInfo)
    : ASCIIDecoder(encoding)
    , m_cpInfo(cpInfo)
    , m_lastLeadByte(0)
    , m_usedDefaultCharCount(0)
    , m_canRemain(false)
{
    if (LN_REQUIRE(cpInfo->MaxCharSize <= 2)) return;

    // 2文字マルチバイト文字コードだが、先行バイトの分かるものであれば状態を保持しながら変換ができる。
    if (cpInfo->MaxCharSize == 2 && cpInfo->LeadByte[0] != '\0') {
        m_canRemain = true;
    }
    reset();
}
#if LN_USTRING32
static bool checkLeadByte(const CPINFOEX* cpInfo, char ch)
{
    for (int i = 0; i < MAX_LEADBYTES; i++) {
        if (cpInfo->LeadByte[i] == ch) return true;
    }
    return false;
}

bool Win32CodePageEncoding::Win32CodePageDecoder::convertToUTF32(const byte_t* input, size_t inputByteSize, UTF32* output, size_t outputElementSize, TextDecodeResult* outResult)
{
    auto options = UTFConversionOptions::make(encoding()->fallbackReplacementChar());
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

    if (m_canRemain) {
        const size_t CHAR_COUNT = 256;
        const size_t WORDS_COUNT = 256;
        char buffer[CHAR_COUNT + 1];
        wchar_t words[WORDS_COUNT + 1];

        const byte_t* iItr = input;
        const byte_t* iEnd = iItr + inputByteSize;
        UTF32* oItr = output;
        UTF32* oEnd = oItr + outputElementSize;

        while (iItr < iEnd) {
            char* bItr = buffer;
            char* bEnd = buffer + CHAR_COUNT;

            // 前回の先行バイトを詰める
            if (m_lastLeadByte != '\0') {
                *bItr = m_lastLeadByte;
                bItr++;
            }

            while (iItr < iEnd && bItr < bEnd) {

                //if (checkLeadByte(m_cpInfo, *iItr) != FALSE) {
                if (m_lastLeadByte == '\0' && ::IsDBCSLeadByteEx(m_cpInfo->CodePage, *iItr) != FALSE) {
                    m_lastLeadByte = *iItr;
                }
                else {
                    m_lastLeadByte = '\0';
                }

                *bItr = *iItr;
                iItr++;
                bItr++;
            }

            if (m_lastLeadByte) {
                // input の終端が LeadByte だった。
                // その分は変換しないようにひとつ戻す。
                bItr--;
            }

            const size_t bufferSize = bItr - buffer;

            if (bufferSize > 0) {
                // TODO: MB_PRECOMPOSED ではなく MB_ERR_INVALID_CHARS の方がセキュリティ的にはよい。
                // オプションで変更できるようにしたい。
                // 今は主に Process クラスで標準出力を受け取るために使っておりデフォルトは OS のコードページだが、
                // その外部プロセスが UTF-8 でデータを送ってくることもある。（clang に UTF-8 コードを入力した時のエラーメッセージ等）
                int convertedWideCount = ::MultiByteToWideChar(
                    m_cpInfo->CodePage, MB_PRECOMPOSED,
                    (LPCSTR)buffer, bufferSize,
                    (LPWSTR)words, WORDS_COUNT);
                if (LN_ENSURE(convertedWideCount > 0)) {
                    return false;
                }


                auto result = UnicodeUtils::convertUTF16toUTF32(reinterpret_cast<const UTF16*>(words), convertedWideCount, oItr, oEnd - oItr, &options);
                if (result != UTFConversionResult_Success) {
                    return false;
                }

                oItr += options.ConvertedTargetLength;
                outResult->outputCharCount += options.CharCount;
            }
            else {
                // input に LeadByte しか入っていなかった
            }
        }

        outResult->usedByteCount = iItr - input;
        outResult->outputByteCount = (oItr - output) * sizeof(UTF32);

    }
    else {
        return ASCIIDecoder::convertToUTF32(input, inputByteSize,  output, outputElementSize, outResult);
    }



    //int convertedWideCount;
    //if (m_canRemain) {
    //    // 前回の先行バイトが保存されていれば、入力バッファの先頭に先行バイトを埋め込んだ新しいバッファを作る。
    //    // されていなければそのまま input をコピーでバッファを作る。
    //    std::vector<byte_t> tmpInBuffer;
    //    if (m_lastLeadByte != '\0') {
    //        tmpInBuffer.resize(1 + inputByteSize);
    //        tmpInBuffer[0] = m_lastLeadByte;
    //        std::copy(input, input + inputByteSize, tmpInBuffer.begin() + 1);
    //    }
    //    else {
    //        tmpInBuffer.resize(inputByteSize);
    //        std::copy(input, input + inputByteSize, tmpInBuffer.begin());
    //    }

    //    // バッファ全体をチェック。末尾が先行バイトであれば defectCount が 1 でループ終了する。
    //    size_t defectCount = 0;
    //    for (size_t i = 0; i < tmpInBuffer.size(); i++) {
    //        if (defectCount > 0) {
    //            // ひとつ前の文字が LeadByte だった。2つで1文字。
    //            defectCount = 0;
    //        }
    //        else {
    //            if (::IsDBCSLeadByteEx(m_codePage, tmpInBuffer[i]) != FALSE) {
    //                // 先行バイト文字である
    //                defectCount = 1;
    //            }
    //        }
    //    }

    //    // バッファ終端が先行バイトだった。先行バイトを保存する
    //    if (defectCount > 0) {
    //        m_lastLeadByte = tmpInBuffer[tmpInBuffer.size() - 1];
    //    }
    //    else {
    //        m_lastLeadByte = '\0';
    //    }

    //    // バッファ全てが先行バイトだった。何もしない
    //    if (defectCount == tmpInBuffer.size()) {
    //        return true;
    //    }

    //    // 変換
    //    convertedWideCount = ::MultiByteToWideChar(m_codePage, MB_ERR_INVALID_CHARS, (LPCSTR)tmpInBuffer.data(), tmpInBuffer.size() - defectCount, (LPWSTR)output, outputElementSize);
    //    if (LN_ENSURE(convertedWideCount > 0))
    //        return false;
    //}
    //else {
    //    convertedWideCount = ::MultiByteToWideChar(m_codePage, MB_ERR_INVALID_CHARS, (LPCSTR)input, inputByteSize, (LPWSTR)output, outputElementSize);
    //    if (LN_ENSURE(convertedWideCount > 0))
    //        return false;
    //}

    //// MultiByteToWideChar じゃ文字数カウントはできないので UnicodeUtils を使う
    //int count;
    //UTFConversionResult r = UnicodeUtils::getUTF16CharCount((UTF16*)output, convertedWideCount, true, &count);
    //if (LN_ENSURE(r == UTFConversionResult_Success))
    //    return false;

    return true;
}
#else
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
#endif

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
#if LN_USTRING32

static bool UTF16ToUTF32(const void* utf16Buf, size_t bytes)
{

}

bool Win32CodePageEncoding::Win32CodePageEncoder::convertFromUTF32(const UTF32* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, TextEncodeResult* outResult)
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

    // Covertion settings for WideCharToMultiByte()
    DWORD dwFlags = 0;
    char chDefault = (char)encoding()->fallbackReplacementChar();
    BOOL bUsedDefaultChar = FALSE;
    LPCSTR pDefault = NULL;
    if (chDefault != '\0') {
        dwFlags = WC_NO_BEST_FIT_CHARS;
        pDefault = &chDefault;
    }

    const UTF32* iItr = input;
    const UTF32* iEnd = input + inputElementSize;
    char* oItr = reinterpret_cast<char*>(output);
    char* oEnd = oItr + outputByteSize;

    const size_t WORD_COUNT = 256;
    wchar_t words[WORD_COUNT + 1];
    auto options = UTFConversionOptions::make(chDefault);
    
    while (iItr < iEnd)
    {
        // 
        const size_t csize = std::min((size_t)(iEnd - iItr), WORD_COUNT / 2);  // 一度に変換できる UTF32 文字数
        if (UnicodeUtils::convertUTF32toUTF16(iItr, csize, words, WORD_COUNT, &options) != UTFConversionResult_Success) {
            return false;
        }

        const size_t convertedWords = options.ConvertedTargetLength;

        const int convertedByteCount = ::WideCharToMultiByte(
            m_codePage,
            dwFlags,
            words,
            convertedWords,
            (LPSTR)oItr,
            oEnd - oItr,
            pDefault,
            &bUsedDefaultChar);
        if (convertedByteCount <= 0) {
            return false;
        }

        iItr += options.ConvertedSourceLength;
        oItr += convertedByteCount;
        outResult->outputCharCount += options.CharCount;
    }

    outResult->usedElementCount = inputElementSize;
    outResult->outputByteCount = oItr - reinterpret_cast<char*>(output);
    return true;
}
#else
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
#endif

} // namespace ln

#endif
