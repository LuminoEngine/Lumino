#pragma once
#include <string>
#include <LuminoCore/Base/Common.hpp>

#ifndef _WIN32
#include <stdint.h>
#endif

#if WCHAR_MAX <= 0xffff
#define LN_UU_WCHAR_16 1
#elif WCHAR_MAX <= 0xffffffff
#define LN_UU_WCHAR_32 1
#else
#error "size of wchar_t is too large."
#endif

namespace ln {

/// 文字コード変換の処理結果
enum UTFConversionResult
{
    UTFConversionResult_Success = 0,     ///< 変換成功
    UTFConversionResult_SourceExhausted, ///< ソースバッファが不正 (バッファ終端でマルチバイト文字が途切れた)
    UTFConversionResult_TargetExhausted, ///< ターゲットバッファが足りない
    UTFConversionResult_SourceIllegal,   ///< ソースバッファ内に不正な文字が見つかった (UTF8の不正バイトシーケンスや、UTF16のサロゲートペア不一致(上位があるのに下位が無い)の時は、不正文字置換を有効にしていてもこのエラーで失敗する)
};

/// 文字コード変換の追加情報
struct UTFConversionOptions
{
    unsigned long ReplacementChar; ///< [in] 不正文字を見つけた時、この文字に置換する (ASCII で指定すること。'\0' を指定した状態で不正文字を見つけると、変換は失敗する)

    int ConvertedSourceLength; ///< [out] 変換に使用したソースバッファの要素数が格納される (バイト数や文字数ではない)
    int ConvertedTargetLength; ///< [out] 変換されたターゲットバッファの要素数が格納される (バイト数や文字数ではない)
    int CharCount;             ///< [out] 文字数が格納される (マルチバイト文字を1文字とみなした文字数)
    int IllegalCharCount;      ///< [out] 見つけた不正文字の数

    static UTFConversionOptions make(unsigned long replacementChar)
    {
        UTFConversionOptions v;
        v.ReplacementChar = replacementChar;
        v.ConvertedSourceLength = 0;
        v.ConvertedTargetLength = 0;
        v.CharCount = 0;
        v.IllegalCharCount = 0;
        return v;
    }
};

/**
	@brief		Unicode の相互変換等を行うモジュール
	@details	UTF-8 は最大 4 バイトとして扱います。
*/
class UnicodeUtils
{
public:
//#ifdef _WIN32
//    typedef unsigned long UTF32;
//    typedef wchar_t UTF16; // デバッガで見ることができるので wchar_t にしておく
//    typedef unsigned char UTF8;
//#else
//    typedef uint32_t UTF32;
//    typedef uint16_t UTF16;
//    typedef uint8_t UTF8;
//#endif

    static const UTF32 SurrogateHighStart = (UTF32)0xD800; ///< 上位サロゲート値の範囲 min
    static const UTF32 SurrogateHighEnd = (UTF32)0xDBFF;   ///< 上位サロゲート値の範囲 max
    static const UTF32 SurrogateLowStart = (UTF32)0xDC00;  ///< 下位サロゲート値の範囲 min
    static const UTF32 SurrogateLowEnd = (UTF32)0xDFFF;    ///< 下位サロゲート値の範囲 max
    static const int UTF16HalfShift = 10;                  ///< UTF16上位サロゲートに使用するシフト量
    static const UTF32 UTF16HalfBase = 0x0010000UL;        ///< UTF16下位サロゲートに使用するベース値
    static const UTF32 UTF16HalfMask = 0x3FFUL;            ///< UTF32→UTF16変換時の、下位マスク
    static const UTF32 MaxLegalUTF32 = 0x0010FFFF;         ///< この値以下が UTF32として有効
    static const UTF32 ReplacementChar = 0x0000FFFD;       ///< 不正文字が見つかった時、これに置換する
    static const UTF32 MaxBMP = 0x0000FFFF;                ///< Basic Multilingual Plane
    static const int UTF8MaxBytes = 4;                      

public:
    /**
		@brief			UTF8 から UTF16 への変換
		@param[in]		sourceStart		: 変換元のバッファの先頭アドレス
		@param[in]		sourceLength	: 変換元のバッファの長さ (文字単位(sourceStartの要素数)。終端 \0 は含まないようにすること)
		@param[in]		targetStart		: 変換結果を格納するバッファの先頭アドレス (終端 \0 は格納されない)
		@param[in]		targetLength	: 変換結果を格納するバッファの長さ (文字単位(targetStartの要素数))
		@param[in,out]	options			: 変換の追加情報
	*/
    static UTFConversionResult convertUTF8toUTF16(
        const UTF8* sourceStart,
        size_t sourceLength, // 終端 \0 等は含まない
        UTF16* targetStart,
        size_t targetLength,
        UTFConversionOptions* options);

    /**
		@brief			UTF8 から UTF32 への変換
		@param[in]		sourceStart		: 変換元のバッファの先頭アドレス
		@param[in]		sourceLength	: 変換元のバッファの長さ (文字単位(sourceStartの要素数)。終端 \0 は含まないようにすること)
		@param[in]		targetStart		: 変換結果を格納するバッファの先頭アドレス (終端 \0 は格納されない)
		@param[in]		targetLength	: 変換結果を格納するバッファの長さ (文字単位(targetStartの要素数))
		@param[in,out]	options			: 変換の追加情報
	*/
    static UTFConversionResult convertUTF8toUTF32(
        const UTF8* sourceStart,
        size_t sourceLength,
        UTF32* targetStart,
        size_t targetLength,
        UTFConversionOptions* options);

    /**
		@brief			UTF16 から UTF8 への変換
		@param[in]		sourceStart		: 変換元のバッファの先頭アドレス
		@param[in]		sourceLength	: 変換元のバッファの長さ (文字単位(sourceStartの要素数)。終端 \0 は含まないようにすること)
		@param[in]		targetStart		: 変換結果を格納するバッファの先頭アドレス (終端 \0 は格納されない)
		@param[in]		targetLength	: 変換結果を格納するバッファの長さ (文字単位(targetStartの要素数))
		@param[in,out]	options			: 変換の追加情報
	*/
    static UTFConversionResult convertUTF16toUTF8(
        const UTF16* sourceStart,
        size_t sourceLength,
        UTF8* targetStart,
        size_t targetLength,
        UTFConversionOptions* options);

    /**
		@brief			UTF16 から UTF32 への変換
		@param[in]		sourceStart		: 変換元のバッファの先頭アドレス
		@param[in]		sourceLength	: 変換元のバッファの長さ (文字単位(sourceStartの要素数)。終端 \0 は含まないようにすること)
		@param[in]		targetStart		: 変換結果を格納するバッファの先頭アドレス (終端 \0 は格納されない)
		@param[in]		targetLength	: 変換結果を格納するバッファの長さ (文字単位(targetStartの要素数))
		@param[in,out]	options			: 変換の追加情報
	*/
    static UTFConversionResult convertUTF16toUTF32(
        const UTF16* sourceStart,
        size_t sourceLength,
        UTF32* targetStart,
        size_t targetLength,
        UTFConversionOptions* options);

    /**
		@brief			UTF32 から UTF8 への変換
		@param[in]		sourceStart		: 変換元のバッファの先頭アドレス
		@param[in]		sourceLength	: 変換元のバッファの長さ (文字単位(sourceStartの要素数)。終端 \0 は含まないようにすること)
		@param[in]		targetStart		: 変換結果を格納するバッファの先頭アドレス (終端 \0 は格納されない)
		@param[in]		targetLength	: 変換結果を格納するバッファの長さ (文字単位(targetStartの要素数))
		@param[in,out]	options			: 変換の追加情報
	*/
    static UTFConversionResult convertUTF32toUTF8(
        const UTF32* sourceStart,
        size_t sourceLength,
        UTF8* targetStart,
        size_t targetLength,
        UTFConversionOptions* options);

    /**
		@brief			UTF32 から UTF16 への変換
		@param[in]		sourceStart		: 変換元のバッファの先頭アドレス
		@param[in]		sourceLength	: 変換元のバッファの長さ (文字単位(sourceStartの要素数)。終端 \0 は含まないようにすること)
		@param[in]		targetStart		: 変換結果を格納するバッファの先頭アドレス (終端 \0 は格納されない)
		@param[in]		targetLength	: 変換結果を格納するバッファの長さ (文字単位(targetStartの要素数))
		@param[in,out]	options			: 変換の追加情報
	*/
    static UTFConversionResult convertUTF32toUTF16(
        const UTF32* sourceStart,
        size_t sourceLength,
        UTF16* targetStart,
        size_t targetLength,
        UTFConversionOptions* options);

    /**
		@brief			UTF8 先行バイトを考慮した文字数を取得する
		@param[in]		sourceStart		: カウント元のバッファの先頭アドレス
		@param[in]		sourceLength	: カウント元のバッファの長さ (文字単位(sourceStartの要素数)。終端 \0 は含まないようにすること)
	*/
    static UTFConversionResult getUTF8CharCount(
        const UTF8* sourceStart,
        size_t sourceLength,
        bool isStrict,
        int* count);

    /**
		@brief			UTF16 のサロゲートペアを考慮した文字数を取得する
		@param[in]		sourceStart		: カウント元のバッファの先頭アドレス
		@param[in]		sourceLength	: カウント元のバッファの長さ (文字単位(sourceStartの要素数)。終端 \0 は含まないようにすること)
	*/
    static UTFConversionResult getUTF16CharCount(
        const UTF16* sourceStart,
        size_t sourceLength,
        bool isStrict,
        int* count);

    /**
		@brief			UTF8 文字の後続バイト数を取得する
		@param[in]		sourceStart			: チェック対象の先頭文字のポインタ
		@param[in]		sourceEnd			: バッファの終端文字のポインタ (有効な領域の次のアドレスを指す (\0 終端文字列なら \0 を指す))
		@param[in]		strict				: true の場合、不正文字のチェックを行う (不正文字が見つかった場合は UTFConversionResult_SourceIllegal を返す)
		@param[out]		outExtraByteCount	: 後続バイト数 (ASCII 文字等、後続が無い場合は 0 が格納される)
		@details		戻り値がエラーでも、outExtraByteCount には後続バイト数が格納されます。
	*/
    static UTFConversionResult checkUTF8TrailingBytes(const UTF8* sourceStart, const UTF8* sourceEnd, bool strict, int* outExtraByteCount);

    /**
		@brief			UTF16 のサロゲートペアであるかを確認する
		@param[in]		sourceStart		: チェック対象の先頭文字のポインタ
		@param[in]		sourceEnd		: バッファの終端文字のポインタ (有効な領域の次のアドレスを指す (\0 終端文字列なら \0 を指す))
		@param[in]		strict			: true の場合、不正文字のチェックを行う (不正文字が見つかった場合は UTFConversionResult_SourceIllegal を返す)
		@param[out]		outSurrogate	: true が格納された場合、sourceStart と その次の文字はサロゲートペアである
	*/
    static UTFConversionResult checkUTF16Surrogate(const UTF16* sourceStart, const UTF16* sourceEnd, bool strict, bool* outSurrogate);

    /// ワードが、UTF16 の上位サロゲートであるかを確認する
    static bool checkUTF16HighSurrogate(UTF16 ch) { return (SurrogateHighStart <= ch && ch <= SurrogateHighEnd); }

    /// ワードが、UTF16 の下位サロゲートであるかを確認する
    static bool checkUTF16LowSurrogate(UTF16 ch) { return (SurrogateLowStart <= ch && ch <= SurrogateLowEnd); }

    /// 1文字分の変換 (UTF8 → UTF32)
    static UTFConversionResult convertCharUTF8toUTF32(
        const UTF8** sourceStart,
        const UTF8* sourceEnd,
        UTFConversionOptions* options,
        UTF32* outChar);

    /// 1文字分の変換 (UTF32 → UTF8)
    static UTFConversionResult convertCharUTF32toUTF8(
        UTF32 ch,
        UTF8** targetStart,
        UTF8* targetEnd,
        UTFConversionOptions* options);

    /// 1文字分の変換 (UTF16 → UTF32)
    static UTFConversionResult convertCharUTF16toUTF32(
        const UTF16** sourceStart,
        const UTF16* sourceEnd,
        UTFConversionOptions* options,
        UTF32* outChar);

    /// 1文字分の変換 (UTF32 → UTF16)
    static UTFConversionResult convertCharUTF32toUTF16(
        UTF32 ch,
        UTF16** targetStart,
        UTF16* targetEnd,
        UTFConversionOptions* options);

private:
    /// 正規の UTF8 バイト列かをチェックする
    static bool isLegalUTF8(const UTF8* source, size_t length);

    /// 不正文字を許容するか？
    static bool isStrictConversion(const UTFConversionOptions* options) { return (options->ReplacementChar == 0); }
};

class UnicodeStringUtils
{
public:
    template<typename TChar>
    [[deprecated("use std::char_traits<char32_t>::length")]]
    inline static size_t strlen(const TChar* str) noexcept
    {
        assert(str);
        const TChar* s;
        for (s = str; *s; ++s);
        return (s - str);
    }

    template<typename TChar>
    inline static char* strncpy(TChar* dst, const TChar* src, size_t n) noexcept
    {
        if (n != 0) {
            TChar* d = dst;
            const TChar* s = src;
            do {
                if ((*d++ = *s++) == 0) {
                    /* NUL pad the remaining n-1 bytes */
                    while (--n != 0)
                        *d++ = 0;
                    break;
                }
            } while (--n != 0);
        }
        return (dst);
    }

    static inline std::wstring NarrowToWide(const char* str, size_t len) noexcept
    {
        std::wstring result;
        result.reserve(len);

        const size_t BUF_SIZE = 50;
        char bufC[BUF_SIZE];
        wchar_t bufW[BUF_SIZE];

        const char* p = str;
        while (*p != '\0') {
            int L = mblen(p, BUF_SIZE);
            if (L <= 0)
                break;

            strncpy(bufC, p, L);
            bufC[L] = '\0';
#ifdef _WIN32
            size_t tmpsize;
            mbstowcs_s(&tmpsize, bufW, BUF_SIZE, bufC, BUF_SIZE);
#else
            mbstowcs(bufW, bufC, BUF_SIZE);
#endif
            result += bufW;

            p += L;
        }

        return result;
    }

    static inline std::u16string WideToU16(const wchar_t* str, size_t len) noexcept
    {
#ifdef LN_UU_WCHAR_16
        return std::u16string(reinterpret_cast<const char16_t*>(str), len);
#else
        std::u16string result(len * 2, u'\0');
        UTFConversionOptions options;
        options.ReplacementChar = '?';
        UnicodeUtils::convertUTF32toUTF16(reinterpret_cast<const UTF32*>(str), len, reinterpret_cast<UTF16*>(result.data()), result.length(), &options);
        return result;
#endif
    }

    static inline std::string U32ToU8(const char32_t* str, size_t len) noexcept
    {
        std::string result(len * UnicodeUtils::UTF8MaxBytes, '\0');
        UTFConversionOptions options;
        options.ReplacementChar = '?';
        UnicodeUtils::convertUTF32toUTF8(reinterpret_cast<const UTF32*>(str), len, reinterpret_cast<UTF8*>(result.data()), result.length(), &options);
        return result;
    }

    static inline std::u16string U32ToU16(const char32_t* str, size_t len) noexcept {
        std::u16string result(len * 2, '\0');
        UTFConversionOptions options;
        options.ReplacementChar = '?';
        UnicodeUtils::convertUTF32toUTF16(reinterpret_cast<const UTF32*>(str), len, reinterpret_cast<UTF16*>(result.data()), result.length(), &options);
        return result;
    }

    static inline std::u32string U16ToU32(const char16_t* str, size_t len) noexcept
    {
        std::u32string result(len * 2, U'\0');
        UTFConversionOptions options;
        options.ReplacementChar = '?';
        UnicodeUtils::convertUTF16toUTF32(reinterpret_cast<const UTF16*>(str), len, reinterpret_cast<UTF32*>(result.data()), result.length(), &options);
        return result;
    }

    static inline std::u32string WideToU32(const wchar_t* str, size_t len) noexcept
    {
#ifdef LN_UU_WCHAR_32
        return std::u32string(reinterpret_cast<const char32_t*>(str), len);
#else
        return U16ToU32(reinterpret_cast<const char16_t*>(str), len);
#endif
    }

    static inline std::wstring U32ToWide(const char32_t* str, size_t len) noexcept
    {
#ifdef LN_UU_WCHAR_32
        return std::wstring(reinterpret_cast<const wchar_t*>(str), len);
#else
        std::wstring result(len * 2, U'\0');
        UTFConversionOptions options;
        options.ReplacementChar = '?';
        UnicodeUtils::convertUTF32toUTF16(reinterpret_cast<const UTF32*>(str), len, reinterpret_cast<UTF16*>(result.data()), result.length(), &options);
        return result;
#endif
    }

    static inline std::u16string NarrowToU16(const char* str, size_t len) noexcept
    {
        const auto s = NarrowToWide(str, len);
        return WideToU16(s.c_str(), s.length());
    }

    static inline std::u32string NarrowToU32(const char* str, size_t len) noexcept
    {
        const auto s = NarrowToWide(str, len);
        return WideToU32(s.c_str(), s.length());
    }

    static inline UStdString NarrowToUString(const char* str, size_t len) noexcept
    {
#if LN_USTRING32
        return NarrowToU32(str, len);
#else
        return NarrowToU16(str, len);
#endif
    }

    static inline UStdString WideToUString(const wchar_t* str, size_t len) noexcept
    {
#if LN_USTRING32
        return WideToU32(str, len);
#else
        return WideToU16(str, len);
#endif
    }
};

} // namespace ln
