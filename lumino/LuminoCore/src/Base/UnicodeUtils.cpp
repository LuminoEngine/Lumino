
#include <stdio.h>
#include <LuminoCore/Base/UnicodeUtils.hpp>

namespace ln {

// UTF-8 のある先頭バイトに対する1文字分の後続バイト数
// (現在本ソースでは 4 バイトまでしか扱えないが、覚書として残しておく)
static const char TrailingBytesForUTF8[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5,
};

// UTF-8 → UTF-32 の変換時に使用し、UTF-8 のバイトマークを打ち消す
static const UTF32 OffsetsFromUTF8[6] = {
    0x00000000UL,
    0x00003080UL,
    0x000E2080UL,
    0x03C82080UL,
    0xFA082080UL,
    0x82082080UL,
};

// UTF-8 の先行バイトに付くバイトマーク
// 00000000, 00000000, 11000000, 11100000, 11110000, 11111000, 11111100
static const UTF8 UTF8FirstByteMark[7] =
    {0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC};

UTFConversionResult UnicodeUtils::convertUTF8toUTF16(
    const UTF8* sourceStart,
    size_t sourceLength,
    UTF16* targetStart,
    size_t targetLength,
    UTFConversionOptions* options) {
    UTFConversionResult result = UTFConversionResult_Success;
    const UTF8* source = sourceStart;
    const UTF8* sourceEnd = source + sourceLength;
    UTF16* target = targetStart;
    UTF16* targetEnd = target + targetLength;

    while (source < sourceEnd) {
        // 一度 UTF32 文字へ
        UTF32 ch;
        result = convertCharUTF8toUTF32(&source, sourceEnd, options, &ch);
        if (result != UTFConversionResult_Success) {
            break;
        }

        // UTF32 から UTF16 へ
        result = convertCharUTF32toUTF16(ch, &target, targetEnd, options);
        if (result != UTFConversionResult_Success) {
            break;
        }

        // 文字数カウント
        options->CharCount++;
    }

    options->ConvertedSourceLength = static_cast<int32_t>(source - sourceStart);
    options->ConvertedTargetLength = static_cast<int32_t>(target - targetStart);
    return result;
}

UTFConversionResult UnicodeUtils::convertUTF8toUTF32(
    const UTF8* sourceStart,
    size_t sourceLength,
    UTF32* targetStart,
    size_t targetLength,
    UTFConversionOptions* options) {
    UTFConversionResult result = UTFConversionResult_Success;
    const UTF8* source = sourceStart;
    const UTF8* sourceEnd = source + sourceLength;
    UTF32* target = targetStart;
    UTF32* targetEnd = target + targetLength;

    while (source < sourceEnd) {
        const UTF8* oldSource = source; // backup

        // UTF32 文字へ
        UTF32 ch;
        result = convertCharUTF8toUTF32(&source, sourceEnd, options, &ch);
        if (result != UTFConversionResult_Success) {
            break;
        }

        // target バッファの範囲チェック
        if (target >= targetEnd) {
            source = oldSource;
            result = UTFConversionResult_TargetExhausted;
            break;
        }

        bool isIllegal = false;
        if (ch <= MaxLegalUTF32) {
            // UTF16サロゲート値は UTF32 では不正文字 (面17 (>0x10FFFF)
            // からは不正文字)
            if (SurrogateHighStart <= ch && ch <= SurrogateLowEnd) {
                isIllegal = true;
            }
        }
        // UTF32 の範囲外
        else {
            isIllegal = true;
        }
        // 不正文字
        if (isIllegal) {
            if (isStrictConversion(options)) {
                // エラーとする
                source = oldSource;
                result = UTFConversionResult_SourceIllegal;
                break;
            }
            else {
                // 置換する
                *target = options->ReplacementChar;
                target++;
                options->IllegalCharCount++;
            }
        }
        // 正規文字
        else {
            *target++ = ch;
        }

        // 文字数カウント
        options->CharCount++;
    }

    options->ConvertedSourceLength = static_cast<int32_t>(source - sourceStart);
    options->ConvertedTargetLength = static_cast<int32_t>(target - targetStart);
    return result;
}

UTFConversionResult UnicodeUtils::convertUTF16toUTF8(
    const UTF16*
        sourceStart, // 変換できない文字が見つかった場合、そこを指している
    size_t sourceLength,
    UTF8* targetStart,
    size_t targetLength,
    UTFConversionOptions* options) {
    UTFConversionResult result = UTFConversionResult_Success;
    const UTF16* source = sourceStart;
    const UTF16* sourceEnd = source + sourceLength;
    UTF8* target = targetStart;
    UTF8* targetEnd = target + targetLength;

    while (source < sourceEnd) {
        const UTF16* oldSource = source;

        // 一度 UTF-32 文字へ
        UTF32 ch;
        result = convertCharUTF16toUTF32(&source, sourceEnd, options, &ch);
        if (result != UTFConversionResult_Success) {
            break;
        }

        // UTF-8 文字へ
        result = convertCharUTF32toUTF8(ch, &target, targetEnd, options);
        if (result != UTFConversionResult_Success) {
            source = oldSource;
            break;
        }

        // 文字数カウント
        options->CharCount++;
    }

    options->ConvertedSourceLength = static_cast<int32_t>(source - sourceStart);
    options->ConvertedTargetLength = static_cast<int32_t>(target - targetStart);
    return result;
}

UTFConversionResult UnicodeUtils::convertUTF16toUTF32(
    const UTF16* sourceStart,
    size_t sourceLength,
    UTF32* targetStart,
    size_t targetLength,
    UTFConversionOptions* options) {
    UTFConversionResult result = UTFConversionResult_Success;
    const UTF16* source = sourceStart;
    const UTF16* sourceEnd = source + sourceLength;
    UTF32* target = targetStart;
    UTF32* targetEnd = target + targetLength;

    while (source < sourceEnd) {
        const UTF16* oldSource = source;

        // UTF-32 文字へ (成功すると、source は次の文字へ移動する)
        UTF32 ch;
        result = convertCharUTF16toUTF32(&source, sourceEnd, options, &ch);
        if (result != UTFConversionResult_Success) {
            break;
        }

        // 出力バッファのサイズチェック
        if (target >= targetEnd) {
            source = oldSource;
            result = UTFConversionResult_TargetExhausted;
            break;
        }

        // 格納
        *target = ch;
        target++;

        // 文字数カウント
        options->CharCount++;
    }

    options->ConvertedSourceLength = static_cast<int32_t>(source - sourceStart);
    options->ConvertedTargetLength = static_cast<int32_t>(target - targetStart);
    return result;
}

UTFConversionResult UnicodeUtils::convertUTF32toUTF8(
    const UTF32* sourceStart,
    size_t sourceLength,
    UTF8* targetStart,
    size_t targetLength,
    UTFConversionOptions* options) {
    UTFConversionResult result = UTFConversionResult_Success;
    const UTF32* source = sourceStart;
    const UTF32* sourceEnd = source + sourceLength;
    UTF8* target = targetStart;
    UTF8* targetEnd = target + targetLength;

    while (source < sourceEnd) {
        const UTF32* oldSource = source; // backup
        UTF32 ch = *source;
        source++;

        // UTF16 サロゲートは UTF32 では不正文字
        if (SurrogateHighStart <= ch && ch <= SurrogateLowEnd) {
            if (isStrictConversion(options)) { // エラーとする
                source = oldSource;
                result = UTFConversionResult_SourceIllegal;
                break;
            }
            else { // 置換する
                ch = options->ReplacementChar;
                options->IllegalCharCount++;
            }
        }

        // UTF8 へ変換
        result = convertCharUTF32toUTF8(ch, &target, targetEnd, options);
        if (result != UTFConversionResult_Success) {
            break;
        }

        // 文字数カウント
        options->CharCount++;
    }

    options->ConvertedSourceLength = static_cast<int32_t>(source - sourceStart);
    options->ConvertedTargetLength = static_cast<int32_t>(target - targetStart);
    return result;
}

UTFConversionResult UnicodeUtils::convertUTF32toUTF16(
    const UTF32* sourceStart,
    size_t sourceLength,
    UTF16* targetStart,
    size_t targetLength,
    UTFConversionOptions* options) {
    UTFConversionResult result = UTFConversionResult_Success;
    const UTF32* source = sourceStart;
    const UTF32* sourceEnd = source + sourceLength;
    UTF16* target = targetStart;
    UTF16* targetEnd = target + targetLength;
    options->CharCount = 0;

    while (source < sourceEnd) {
        // 出力バッファのサイズチェック
        if (target >= targetEnd) {
            result = UTFConversionResult_TargetExhausted;
            break;
        }

        // UTF32 読み取り
        UTF32 ch = *source;
        source++;

        // UTF16 へ (成功すると、target は次の格納先を指す)
        result = convertCharUTF32toUTF16(ch, &target, targetEnd, options);
        if (result != UTFConversionResult_Success) {
            break;
        }

        // 文字数カウント
        options->CharCount++;
    }

    options->ConvertedSourceLength = static_cast<int32_t>(source - sourceStart);
    options->ConvertedTargetLength = static_cast<int32_t>(target - targetStart);
    return result;
}

UTFConversionResult UnicodeUtils::getUTF8CharCount(
    const UTF8* sourceStart,
    size_t sourceLength,
    bool isStrict,
    int* outCount) {
    UTFConversionResult result = UTFConversionResult_Success;
    const UTF8* source = sourceStart;
    const UTF8* sourceEnd = source + sourceLength;
    int count = 0;

    while (source < sourceEnd) {
        int extra;
        result = checkUTF8TrailingBytes(source, sourceEnd, isStrict, &extra);
        if (result != UTFConversionResult_Success) {
            return result;
        }

        source += (extra + 1);
        count++;
    }

    if (outCount != NULL) {
        *outCount = count;
    }
    return result;
}

UTFConversionResult UnicodeUtils::getUTF16CharCount(
    const UTF16* sourceStart,
    size_t sourceLength,
    bool isStrict,
    int* count) {
    const UTF16* source = sourceStart;
    const UTF16* sourceEnd = source + sourceLength;
    UTF16 ch1, ch2;
    (*count) = 0;

    while (source < sourceEnd) {
        ch1 = *source;
        source++;

        // サロゲートペアを持っている場合
        if (SurrogateHighStart <= ch1 && ch1 <= SurrogateHighEnd) {
            if (source < sourceEnd) // バッファ終端チェック
            {
                ch2 = *source; // 2文字目読み取り
                // 2文字目が下位サロゲートである
                if (ch2 >= SurrogateLowStart && ch2 <= SurrogateLowEnd) {
                    ++source;
                }
                else if (isStrict) {
                    // 上位サロゲートがあるのに、下位サロゲートが見つからなかった
                    return UTFConversionResult_SourceIllegal;
                }
            }
            else {
                // 下位サロゲートを探している途中にバッファ終端に到達してしまった
                return UTFConversionResult_SourceExhausted;
            }
        }
        // サロゲートペアではない場合
        else {
            // いきなり下位サロゲート値が見つかった
            if (ch1 >= SurrogateLowStart && ch1 <= SurrogateLowEnd) {
                return UTFConversionResult_SourceIllegal;
            }
        }

        // 文字数カウント
        (*count)++;
    }

    return UTFConversionResult_Success;
}

UTFConversionResult UnicodeUtils::checkUTF8TrailingBytes(
    const UTF8* sourceStart,
    const UTF8* sourceEnd,
    bool strict,
    int* outExtraByteCount) {
    *outExtraByteCount = 0;
    int extra = TrailingBytesForUTF8[*sourceStart];
    *outExtraByteCount = extra;

    // 範囲チェック
    if (sourceStart + extra >= sourceEnd) {
        return UTFConversionResult_SourceExhausted;
    }

    // 正常な UTF8 文字であるか？
    if (strict && !isLegalUTF8(sourceStart, extra + 1)) {
        return UTFConversionResult_SourceIllegal;
    }

    return UTFConversionResult_Success;
}

UTFConversionResult UnicodeUtils::checkUTF16Surrogate(
    const UTF16* sourceStart,
    const UTF16* sourceEnd,
    bool strict,
    bool* outSurrogate) {
    *outSurrogate = false;

    UTF32 ch = *sourceStart;
    // サロゲートペアを持っている場合は UTF32 への変換処理を行う
    if (SurrogateHighStart <= ch && ch <= SurrogateHighEnd) {
        if (sourceStart + 1 < sourceEnd) // バッファ終端チェック
        {
            UTF32 ch2 = *(sourceStart + 1); // 2文字目読み取り
            // 下位サロゲートチェック
            if (SurrogateLowStart <= ch2 && ch2 <= SurrogateLowEnd) {
                *outSurrogate = true;
            }
            else if (strict) {
                // 上位サロゲートがあるのに、下位サロゲートが見つからなかった
                return UTFConversionResult_SourceIllegal;
            }
        }
        else {
            // 下位サロゲートを探している途中にバッファ終端に到達してしまった
            return UTFConversionResult_SourceExhausted;
        }
    }
    // いきなり下位サロゲート値が見つかった
    else if (strict && ch >= SurrogateLowStart && ch <= SurrogateLowEnd) {
        return UTFConversionResult_SourceIllegal;
    }
    return UTFConversionResult_Success;
}

UTFConversionResult UnicodeUtils::convertCharUTF8toUTF32(
    const UTF8** sourceStart,
    const UTF8* sourceEnd,
    UTFConversionOptions* options,
    UTF32* outChar) {
    const UTF8* source = *sourceStart;
    UTF32 ch = 0;

    // 追加で読むバイト数
    unsigned short extraBytesToRead = TrailingBytesForUTF8[*source];
    if (source + extraBytesToRead >= sourceEnd) {
        return UTFConversionResult_SourceExhausted;
    }
    // 正規の UTF-8 文字のバイトシーケンスであるかを確認
    if (!isLegalUTF8(source, extraBytesToRead + 1)) {
        return UTFConversionResult_SourceIllegal;
    }
    // 追加バイト数に応じてフォールスルーで流れつつ読んでいく
    switch (extraBytesToRead) {
        case 5:
            ch += *source++;
            ch <<= 6; // 旧式
        case 4:
            ch += *source++;
            ch <<= 6; // 旧式
        case 3:
            ch += *source++;
            ch <<= 6;
        case 2:
            ch += *source++;
            ch <<= 6;
        case 1:
            ch += *source++;
            ch <<= 6;
        case 0:
            ch += *source++;
    }
    ch -= OffsetsFromUTF8[extraBytesToRead];

    *sourceStart = source;
    *outChar = ch;
    return UTFConversionResult_Success;
}

UTFConversionResult UnicodeUtils::convertCharUTF32toUTF8(
    UTF32 ch,
    UTF8**
        targetStart, // 成否にかかわらず、ここのひとつ前まで文字が書かれている。
    UTF8* targetEnd,
    UTFConversionOptions* options) {
    UTFConversionResult result = UTFConversionResult_Success;
    UTF8* target = *targetStart;

    // 変換後、何バイト必要か
    unsigned short bytesToWrite = 0;
    if (ch < (UTF32)0x80) {
        bytesToWrite = 1;
    }
    else if (ch < (UTF32)0x800) {
        bytesToWrite = 2;
    }
    else if (ch < (UTF32)0x10000) {
        bytesToWrite = 3;
    }
    else if (ch <= MaxLegalUTF32) {
        bytesToWrite = 4;
    }
    else {
        // 不正文字が見つかった
        bytesToWrite = 3;
        if (isStrictConversion(options)) {
            return UTFConversionResult_SourceIllegal;
        }
        else {
            ch = options->ReplacementChar;
            options->IllegalCharCount++;
        }
    }

    if (target + bytesToWrite > targetEnd) {
        return UTFConversionResult_TargetExhausted;
    }
    else {
        const UTF32 byteMask = 0xBF;
        const UTF32 byteMark = 0x80;

        // target は書き込み範囲の一番後ろを指しておき、
        // switch フォールスルーで後ろから前へ文字を書き込んでいく
        target += bytesToWrite;
        switch (bytesToWrite) {
            case 4:
                target[-1] = (UTF8)((ch | byteMark) & byteMask);
                target--;
                ch >>= 6;
            case 3:
                target[-1] = (UTF8)((ch | byteMark) & byteMask);
                target--;
                ch >>= 6;
            case 2:
                target[-1] = (UTF8)((ch | byteMark) & byteMask);
                target--;
                ch >>= 6;
            case 1:
                target[-1] = (UTF8)(ch | UTF8FirstByteMark[bytesToWrite]);
                target--;
        }
        target += bytesToWrite;
    }

    *targetStart = target;
    return result;
}

UTFConversionResult UnicodeUtils::convertCharUTF16toUTF32(
    const UTF16** sourceStart, // 失敗した場合は変化しない
    const UTF16* sourceEnd,
    UTFConversionOptions* options,
    UTF32* outChar) {
    UTFConversionResult result = UTFConversionResult_Success;
    const UTF16* source = *sourceStart;
    UTF32 ch, ch2;

    ch = *source;
    source++;

    // サロゲートペアを持っている場合は UTF32 への変換処理を行う
    if (SurrogateHighStart <= ch && ch <= SurrogateHighEnd) {
        if (source < sourceEnd) // バッファ終端チェック
        {
            ch2 = *source; // 2文字目読み取り
            // 2文字目が下位サロゲートであれば、UTF-32に変換する
            if (ch2 >= SurrogateLowStart && ch2 <= SurrogateLowEnd) {
                ch = ((ch - SurrogateHighStart) << UTF16HalfShift) +
                     (ch2 - SurrogateLowStart) + UTF16HalfBase;
                ++source;
            }
            else if (isStrictConversion(options)) {
                // 上位サロゲートがあるのに、下位サロゲートが見つからなかった
                result = UTFConversionResult_SourceIllegal;
            }
        }
        else {
            // 下位サロゲートを探している途中にバッファ終端に到達してしまった
            result = UTFConversionResult_SourceExhausted;
        }
    }
    // サロゲートペアではない場合
    else {
        // いきなり下位サロゲート値が見つかった
        if (ch >= SurrogateLowStart && ch <= SurrogateLowEnd) {
            result = UTFConversionResult_SourceIllegal;
        }
    }

    // 成功していればそれぞれ更新 (失敗した場合、sourceStart
    // は不正文字を指している)
    if (result == UTFConversionResult_Success) {
        *sourceStart = source;
        *outChar = ch;
    }
    return result;
}

UTFConversionResult UnicodeUtils::convertCharUTF32toUTF16(
    UTF32 ch,
    UTF16** targetStart,
    UTF16* targetEnd,
    UTFConversionOptions* options) {
    UTF16* target = *targetStart;

    // 0xFFFF 以下。普通の UTF-16 構成文字
    if (ch <= MaxBMP) {
        // UTF-16のサロゲートは、UTF-32では無効な文字である
        if (SurrogateHighStart <= ch && ch <= SurrogateLowEnd) {
            // 不正文字の処理
            if (isStrictConversion(options)) {
                return UTFConversionResult_SourceExhausted;
            }
            else {
                *target = (UTF16)options->ReplacementChar;
                target++;
                options->IllegalCharCount++;
            }
        }
        else {
            // サロゲートでなければ普通に格納
            *target = (UTF16)ch;
            target++;
        }
    }
    // 規格外
    else if (ch > MaxLegalUTF32) {
        // 不正文字の処理
        if (isStrictConversion(options)) {
            return UTFConversionResult_SourceExhausted;
        }
        else {
            *target = (UTF16)options->ReplacementChar;
            target++;
            options->IllegalCharCount++;
        }
    }
    // 0xFFFF ～ 0x10FFFF の間 (サロゲート使用)
    else {
        if (target + 1 >= targetEnd) {
            return UTFConversionResult_TargetExhausted; // バッファオーバー
        }
        ch -= UTF16HalfBase;
        *target = (UTF16)((ch >> UTF16HalfShift) + SurrogateHighStart);
        target++;
        *target = (UTF16)((ch & UTF16HalfMask) + SurrogateLowStart);
        target++;
    }

    *targetStart = target;
    return UTFConversionResult_Success;
}

bool UnicodeUtils::isLegalUTF8(const UTF8* source, size_t length) {
    // length は UTF-8 の先行バイトによって事前に決定される。
    // length > 4 の場合は false を返す。

    UTF8 a;

    // 最初に一番後ろを指し、フォールスルーでひとつずつ前へ見ていく
    const UTF8* srcptr = source + length;
    switch (length) {
        default:
            return false;
        case 4:
            if ((a = (*--srcptr)) < 0x80 || a > 0xBF)
                return false;
        case 3:
            if ((a = (*--srcptr)) < 0x80 || a > 0xBF)
                return false;
        case 2:
            if ((a = (*--srcptr)) > 0xBF)
                return false;

            switch (*source) {
                // ここはフォールスルーしないので注意
                case 0xE0:
                    if (a < 0xA0)
                        return false;
                    break;
                case 0xED:
                    if (a > 0x9F)
                        return false;
                    break;
                case 0xF0:
                    if (a < 0x90)
                        return false;
                    break;
                case 0xF4:
                    if (a > 0x8F)
                        return false;
                    break;
                default:
                    if (a < 0x80)
                        return false;
            }

        case 1:
            if (*source >= 0x80 && *source < 0xC2)
                return false;
    }
    if (*source > 0xF4)
        return false;
    return true;
}

} // namespace ln
