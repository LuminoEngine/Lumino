// Copyright (c) 2018+ lriki. Distributed under the MIT license..

#pragma once

namespace ln {

template<typename TChar>
static inline int my_strncmp(const TChar* str1, const TChar* str2, size_t count) LN_NOEXCEPT {
    for (; 0 < count; --count, ++str1, ++str2) {
        if (*str1 != *str2) {
            return (*str1 < *str2) ? -1 : 1;
        }
    }
    return 0;
}

template<typename TChar>
static inline int my_strnicmp(const TChar* str1, const TChar* str2, size_t count) LN_NOEXCEPT {
    for (; 0 < count; --count, ++str1, ++str2) {
        TChar c1 = StringHelper::toUpper(*str1);
        TChar c2 = StringHelper::toUpper(*str2);
        if (c1 != c2) {
            return (c1 < c2) ? -1 : 1;
        }
    }
    return 0;
}

template<typename TChar>
inline int StringHelper::indexOf(const TChar* str1, int str1Len, const TChar* str2, int str2Len, int startIndex, CaseSensitivity cs) noexcept {
    if (LN_ENSURE(str1 && str2)) return -1;

    if (*str1 == 0) {
        return -1;
    }

    str1Len = (str1Len < 0) ? ((int)strlen(str1)) : str1Len;
    if (str1Len <= startIndex) {
        return -1;
    }

    str2Len = (str2Len < 0) ? ((int)strlen(str2)) : str2Len;
    if (str2Len <= 0) {
        return -1;
    }

    const TChar* pos = str1 + startIndex;
    const TChar* end = str1 + str1Len;

    // 大文字小文字を区別する
    if (cs == CaseSensitivity::CaseSensitive) {
        for (; pos < end && *pos; ++pos) {
            if (*pos == *str2) {
                if (my_strncmp(pos, str2, str2Len) == 0) {
                    return (int)(pos - str1);
                }
            }
        }
    }
    // 大文字小文字を区別しない
    else {
        for (; pos < end && *pos; ++pos) {
            if (my_strnicmp(pos, str2, str2Len) == 0) {
                return (int)(pos - str1);
            }
        }
    }

    return -1;
}

template<typename TChar>
inline int StringHelper::lastIndexOf(const TChar* str1, int str1Len, const TChar* str2, int str2Len, int startIndex, int count, CaseSensitivity cs) noexcept {
    const TChar nullStr[] = { '\0' };

    str1 = (str1 == nullptr) ? nullStr : str1;
    str2 = (str2 == nullptr) ? nullStr : str2;
    str1Len = static_cast<int>((str1Len < 0) ? strlen(str1) : str1Len);
    str2Len = static_cast<int>((str2Len < 0) ? strlen(str2) : str2Len);
    startIndex = (startIndex < 0) ? (str1Len - 1) : startIndex;

    // 検索対象文字列の長さが 0 の場合は特別な前提処理
    if (str1Len == 0 && (startIndex == -1 || startIndex == 0)) {
        return (str2Len == 0) ? 0 : -1;
    }

    if (LN_ENSURE(startIndex >= 0)) return -1;      // startIndex は 0 以上でなければならない。
    if (LN_ENSURE(startIndex < str1Len)) return -1; // startIndex は str1 の長さを超えてはならない。

    // 検索文字数が 0 の場合は必ず検索開始位置でヒットする (strstr と同じ動作)
    if (str2Len == 0 && count >= 0 && startIndex - count + 1 >= 0) {
        return startIndex;
    }

    const TChar* pos = str1 + startIndex;                      // 検索範囲の末尾の文字を指す。
    const TChar* end = (count < 0) ? str1 : pos - (count - 1); // 検索範囲の先頭の文字を指す。
    if (LN_ENSURE(end <= pos)) return -1;                      // 末尾と先頭が逆転してないこと。

    if (pos - end < (str2Len - 1)) {
        return -1; // 検索範囲が検索文字数よりも少ない場合は見つかるはずがない
    }

    pos -= (str2Len - 1);

    // 大文字小文字を区別する
    if (cs == CaseSensitivity::CaseSensitive) {
        // 後ろから前へ見ていく
        while (pos >= end) {
            if (my_strncmp(pos, str2, str2Len) == 0) {
                return (int)(pos - str1);
            }
            --pos;
        }
    }
    // 大文字小文字を区別しない
    else {
        // 後ろから前へ見ていく
        while (pos >= end) {
            if (my_strnicmp(pos, str2, str2Len) == 0) {
                return (int)(pos - str1);
            }
            --pos;
        }
    }
    return -1;
}

template<typename TChar>
inline int StringHelper::compare(const TChar* str1, int str1Len, const TChar* str2, int str2Len, int count, CaseSensitivity cs) noexcept {
    if (str1 == nullptr || str2 == nullptr) {
        if (str1 == nullptr && str2 == nullptr) {
            return 0;
        }
        else if (str1 == nullptr) {
            return -1; // NULL < ""
        }
        else {
            return 1; // "" > NULL
        }
    }

    // 必要があれば文字数カウント
    str1Len = (str1Len < 0) ? static_cast<int>(strlen(str1)) : str1Len;
    str2Len = (str2Len < 0) ? static_cast<int>(strlen(str2)) : str2Len;
    int minCount = std::min(str1Len, str2Len);
    int maxCount = std::max(str1Len, str2Len);

    // チェックする文字数
    if (count < 0) {
        count = maxCount;
    }
    else {
        maxCount = count;
        count = LN_MIN(minCount, count);
    }

    if (cs == CaseSensitivity::CaseSensitive) {
        while (count > 0) {
            if (*str1 != *str2) {
                return ((unsigned int)(*str1)) - ((unsigned int)(*str2));
            }
            //if (*str1 == 0) {
            //	break;
            //}
            ++str1;
            ++str2;
            --count;
        }
    }
    else {
        while (count > 0) {
            if (toUpper(*str1) != toUpper(*str2)) {
                return ((unsigned int)(toUpper(*str1))) - ((unsigned int)(toUpper(*str2)));
            }
            //if (*str1 == 0) {
            //	break;
            //}
            ++str1;
            ++str2;
            --count;
        }
    }

    if (minCount < maxCount) {
        return str1Len - str2Len;
    }
    return 0;
}

template<typename TChar>
inline int StringHelper::compare(const TChar* str1, const TChar* str2, int count, CaseSensitivity cs) noexcept {
    return compare(str1, -1, str2, -1, count, cs);
}

template<typename TChar>
inline int StringHelper::compare(TChar ch1, TChar ch2, CaseSensitivity cs) noexcept {
    if (cs == CaseSensitivity::CaseSensitive) {
        return ((unsigned int)(ch1)) - ((unsigned int)(ch2));
    }
    else {
        return ((unsigned int)(toUpper(ch1))) - ((unsigned int)(toUpper(ch2)));
    }
}

template<typename TChar>
inline TChar StringHelper::toUpper(TChar ch) noexcept {
    if ('a' <= ch && ch <= 'z')
        return 'A' + (ch - 'a');
    return ch;
}

template<typename TChar>
inline TChar StringHelper::toLower(TChar ch) noexcept {
    if ('A' <= ch && ch <= 'Z')
        return 'a' + (ch - 'A');
    return ch;
}

} // namespace ln
