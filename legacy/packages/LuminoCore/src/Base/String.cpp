/**
 * @private
 * 
 * String の内部エンコーディング
 * ----------
 * 
 * ### UTF-32
 * - https://ja.wikipedia.org/wiki/UTF-32
 *   - システムのメモリ上での管理や、データベースに保存するエンコーディングとして使用されることが多い。データ交換 (ファイル等) に使われることはほとんどない。
 *   - バイト数が固定であるため、最適なメモリ確保ができる。
 *   - メモリ消費が大きい。
 * - ゲームエンジンとしては
 *   - テキストレンダリング時に文字コードの変換が必要なくなるため高速。
 *   - C# や JS 等ほかの言語から呼び出すときにはエンコーディングの変換が必要になる。
 *     - これを毎フレーム行うのは直接描画で頑張るとき。
 *       しかしそれと比較しても、内部のテキストレンダリングは複数パスで行われることがありグリフの取得のため変換が都度行われる。
 *       UTF-32 の方が変換回数は少なくなるだろう。
 *
 */
#include "Internal.hpp"
#include <limits.h>
#include <iostream>
#include <memory>
#if defined(LN_OS_WIN32) && _MSC_VER >= 1900
#include <cuchar>
#endif
#include <LuminoCore/Base/Buffer.hpp>
#include <LuminoCore/Base/String.hpp>
#include <LuminoCore/Base/StringHelper.hpp>
#include <LuminoCore/Base/RefObject.hpp>
#include <LuminoCore/Base/CRCHash.hpp>
#include <LuminoCore/Base/UnicodeUtils.hpp>
#include <LuminoCore/Text/Encoding.hpp>

#ifdef LN_STRING_WITH_PATH
#include <LuminoCore/IO/Path.hpp>
#endif

namespace ln {

static bool checkOverRange(const String& this_, const Char* str, int len) {
    const Char* b1 = this_.c_str();
    const Char* e1 = b1 + this_.length();
    const Char* b2 = str;
    const Char* e2 = str + len;
    return !(e1 <= b2 || e2 <= b1);
}

namespace detail {
struct StringLockContext {
    detail::UStringCore* newCore;
    detail::UStringCore* oldCore;
};
}

//==============================================================================
// UStringCore

namespace detail {

UStringCore::UStringCore()
    : m_refCount(1)
    , m_str(nullptr)
    , m_capacity(0)
    , m_length(0) {
}

UStringCore::~UStringCore() {
    delete[] m_str;
}

bool UStringCore::isShared() const LN_NOEXCEPT {
    return (m_refCount > 1);
}

void UStringCore::retain() {
    ++m_refCount;
}

void UStringCore::release() {
    --m_refCount;
    if (m_refCount == 0) {
        delete this;
    }
}

void UStringCore::reserve(size_t length) {
    LN_DCHECK(length >= 0);
    size_t size = length + 1;
    if (m_capacity < size) {
        Char* oldStr = m_str;
        size_t oldLen = m_length;

        m_str = LN_NEW Char[size];
        m_capacity = length;

        if (oldStr != nullptr) {
            memcpy(m_str, oldStr, LN_MIN(length, oldLen) * sizeof(Char));
            delete oldStr;
        }
    }
}
void UStringCore::fixLength(size_t length) {
    m_str[length] = '\0';
    m_length = length;
}
void UStringCore::resize(size_t length) {
    reserve(length);
    fixLength(length);
}
void UStringCore::clear() {
    if (m_str != nullptr) {
        m_str[0] = '\0';
    }
    m_length = 0;
}

} // namespace detail

//==============================================================================
// String

const String String::Empty;

String::String() {
    init();
}

String::~String() {
    release();
}

String::String(const String& str)
    : String() {
    copy(str);
}

String::String(String&& str) LN_NOEXCEPT
    : String() {
    move(std::forward<String>(str));
}

String& String::operator=(const String& str) {
    copy(str);
    return *this;
}

String& String::operator=(String&& str) LN_NOEXCEPT {
    move(std::forward<String>(str));
    return *this;
}

String::String(const String& str, int begin)
    : String() {
    assign(str.c_str() + begin, str.length());
}

String::String(const String& str, int begin, int length)
    : String() {
    assign(str.c_str() + begin, length);
}

String::String(const Char* str)
    : String() {
    assign(str);
}

String::String(const Char* str, int length)
    : String() {
    assign(str, length);
}

String::String(const Char* begin, const Char* end)
    : String() {
    assign(begin, static_cast<int>(end - begin));
}

String::String(int count, Char ch)
    : String() {
    assign(count, ch);
}

String::String(const StringView& str)
    : String() {
    assign(str);
}

#ifdef LN_STRING_FUZZY_CONVERSION
String::String(const char* str)
    : String() {
    assignFromCStr(str);
}
#ifdef LN_USTRING16
String::String(const wchar_t* str)
    : String() {
    assignFromCStr(str);
}
#endif
#endif

bool String::isEmpty() const LN_NOEXCEPT {
    if (isSSO()) {
        return getSSOLength() == 0;
    }
    else {
        return !m_data.core || m_data.core->length() <= 0;
    }
}

void String::clear() {
    detail::StringLockContext context;
    lockBuffer(0, &context);
    unlockBuffer(0, &context);
}

void String::resize(int newLength) {
    resize(newLength, Char());
}

void String::resize(int newLength, Char ch) {
    int oldLen = length();
    detail::StringLockContext context;
    Char* buf = lockBuffer(newLength, &context);
    if (newLength > oldLen) {
        for (int i = oldLen; i < newLength; i++) {
            buf[i] = ch;
        }
    }
    unlockBuffer(newLength, &context);
}

void String::reserve(int size) {
    int cur = length();
    //reserveBuffer(size);
    detail::StringLockContext context;
    lockBuffer(size, &context);
    unlockBuffer(cur, &context);
}

bool String::contains(const StringView& str, CaseSensitivity cs) const {
    return indexOf(str, 0, cs) >= 0;
}

bool String::contains(Char ch, CaseSensitivity cs) const {
    return indexOf(ch, 0, cs) >= 0;
}

int String::indexOf(const StringView& str, int startIndex, CaseSensitivity cs) const {
    return StringHelper::indexOf(c_str(), length(), str.data(), str.length(), startIndex, cs);
}

int String::indexOf(Char ch, int startIndex, CaseSensitivity cs) const {
    return StringHelper::indexOf(c_str(), length(), &ch, 1, startIndex, cs);
}

int String::lastIndexOf(const StringView& str, int startIndex, int count, CaseSensitivity cs) const {
    return StringHelper::lastIndexOf(c_str(), length(), str.data(), str.length(), startIndex, count, cs);
}

int String::lastIndexOf(Char ch, int startIndex, int count, CaseSensitivity cs) const {
    return StringHelper::lastIndexOf(c_str(), length(), &ch, 1, startIndex, count, cs);
}

bool String::startsWith(const StringView& str, CaseSensitivity cs) const {
    return StringHelper::startsWith(c_str(), length(), str.data(), str.length(), cs);
}

bool String::startsWith(Char ch, CaseSensitivity cs) const {
    return StringHelper::startsWith(c_str(), length(), &ch, 1, cs);
}

bool String::endsWith(const StringView& str, CaseSensitivity cs) const {
    return StringHelper::endsWith(c_str(), length(), str.data(), str.length(), cs);
}

bool String::endsWith(Char ch, CaseSensitivity cs) const {
    return StringHelper::endsWith(c_str(), length(), &ch, 1, cs);
}

StringView String::substr(int start, int count) const {
    const Char* begin;
    const Char* end;
    StringHelper::substr(c_str(), length(), start, count, &begin, &end);
    return StringView(begin, end);
}

StringView String::left(int count) const {
    const Char* begin;
    const Char* end;
    StringHelper::left(c_str(), count, &begin, &end);
    return StringView(begin, end);
}

StringView String::right(int count) const {
    const Char* begin;
    const Char* end;
    StringHelper::right(c_str(), count, &begin, &end);
    return StringView(begin, end);
}

String String::trim() const {
    const Char* begin;
    int len;
    StringHelper::trim(c_str(), length(), &begin, &len);
    return String(begin, len);
}

String String::toUpper() const {
    int len = length();
    String result(c_str(), len);
    Char* buf = result.getBuffer();
    for (int i = 0; i < len; i++)
        buf[i] = StringHelper::toUpper<Char>(buf[i]);
    return result;
}

String String::toLower() const {
    int len = length();
    String result(c_str(), len);
    Char* buf = result.getBuffer();
    for (int i = 0; i < len; i++)
        buf[i] = StringHelper::toLower<Char>(buf[i]);
    return result;
}

String String::toTitleCase() const {
    int len = length();
    String result(c_str(), len);
    Char* buf = result.getBuffer();
    for (int i = 0; i < len; i++)
        buf[i] = StringHelper::toLower<Char>(buf[i]);
    if (len > 0) buf[0] = StringHelper::toUpper<Char>(buf[0]);
    return result;
}

String String::remove(const StringView& str, CaseSensitivity cs) const {
    String result;
    const Char* pos = c_str();
    const Char* end = pos + length();
    const Char* fs = str.data();
    int fsLen = str.length();

    detail::StringLockContext context;
    Char* buf = result.lockBuffer(end - pos, &context);
    Char* bufBegin = buf;

    if (fsLen > 0) {
        for (; pos < end;) {
            int index = StringHelper::indexOf(pos, static_cast<int32_t>(end - pos), fs, fsLen, 0, cs);
            if (index >= 0) {
                memcpy(buf, pos, index * sizeof(Char));
                buf += index;
                pos += index + fsLen;
            }
            else {
                break;
            }
        }
    }

    if (pos < end) {
        memcpy(buf, pos, (end - pos) * sizeof(Char));
        buf += (end - pos);
    }

    result.unlockBuffer(buf - bufBegin, &context);
    return result;
}

String String::replace(const StringView& from, const StringView& to, CaseSensitivity cs) const {
    String result;
    result.reserve(length());

    int pos = 0;
    const Char* src = c_str();
    int srcLen = length();
    const Char* fromStr = from.data();
    int fromLen = from.length();
    int start = 0;

    if (fromLen > 0) {
        do {
            pos = StringHelper::indexOf(src, srcLen, fromStr, fromLen, start, cs);
            if (pos >= 0) {
                result.append(src + start, pos - start);
                result.append(to.data(), to.length());
                start = pos + fromLen;
            }

        } while (pos >= 0);
    }

    result.append(src + start, srcLen - start);
    return result;
}

String String::insert(int startIndex, const StringView& value) const {
    if (value.isEmpty()) return *this;
    if (startIndex < 0) startIndex = 0;
    if (startIndex > length()) startIndex = length();

    String result;
    result.reserve(length() + value.length());

    const Char* src = c_str();
    int len = length();
    if (0 < startIndex) {
        result.append(src, startIndex);
    }

    result.append(value);

    if (startIndex < len) {
        result.append(src + startIndex, len - startIndex);
    }

    return result;
}

List<String> String::split(const StringView& delim, StringSplitOptions option) const {
    List<String> result;
    StringHelper::SplitHelper(
        c_str(), c_str() + length(), delim.data(), delim.length(), option, CaseSensitivity::CaseSensitive, [&result](const Char* begin, const Char* end) { result.add(String(begin, static_cast<int>(end - begin))); });
    return result;
}

Array<String> String::splitLines() const {
    if (isEmpty()) {
        return {};
    }

    const String& code = *this;
    const auto getNextLineLast = [&](int32_t lineHead, int32_t* last, int32_t* nlLen) {
        int32_t i = lineHead;
        int32_t end = code.length();
        bool lastCR = false;
        for (; i < end;) {
            if (lastCR) {
                if (code[i] == '\n') {
                    // CRLF
                    *last = i;
                    *nlLen = 2;
                    return;
                }
                else {
                    // CR
                    *last = i - 1;
                    *nlLen = 1;
                    return;
                }
            }
            else if (code[i] == '\r') {
                lastCR = true;
                i++;
            }
            else if (code[i] == '\n') { // LF
                *last = i;
                *nlLen = 1;
                return;
            }
            else {
                i++;
            }
        }
        if (lastCR) {
            *last = i - 1;
            *nlLen = 1;
        }
        else {
            *last = code.length(); // \0(EOF) の前
            *nlLen = 0;            // EOF
        }
    };


    Array<String> result;
    const Char* buf = code.c_str();
    int32_t pos = 0;
    int32_t end = code.length();
    while (pos <= end) {
        int32_t lineBegin = pos;
        int32_t lineEnd = 0;
        int32_t nlLen = 0;
        getNextLineLast(pos, &lineEnd, &nlLen);
        lineEnd += 1;

        result.push(code.substr(lineBegin, lineEnd - lineBegin - nlLen));

        pos = lineEnd;
    }

    return result;
}

#define TO_INT_DEF(type, func)                                   \
    const Char* begin;                                           \
    const Char* end;                                             \
    int len;                                                     \
    NumberConversionResult res;                                  \
    StringHelper::trim(c_str(), length(), &begin, &len);         \
    type num = StringHelper::func(begin, len, base, &end, &res); \
    if (res == NumberConversionResult::ArgsError) {              \
        LN_ENSURE(0);                                            \
    }                                                            \
    if (res == NumberConversionResult::FormatError) {            \
        LN_ENSURE(0);                                            \
    }                                                            \
    if (res == NumberConversionResult::Overflow) {               \
        LN_ENSURE(0);                                            \
    }                                                            \
    LN_ENSURE(end == begin + len);                               \
    return num;
int String::toInt(int base) const {
    TO_INT_DEF(int32_t, toInt32);
}
int8_t String::toInt8(int base) const {
    TO_INT_DEF(int8_t, toInt8);
}
int16_t String::toInt16(int base) const {
    TO_INT_DEF(int16_t, toInt16);
}
int32_t String::toInt32(int base) const {
    TO_INT_DEF(int32_t, toInt32);
}
int64_t String::toInt64(int base) const {
    TO_INT_DEF(int64_t, toInt64);
}
uint8_t String::toUInt8(int base) const {
    TO_INT_DEF(uint8_t, toUInt8);
}
uint16_t String::toUInt16(int base) const {
    TO_INT_DEF(uint16_t, toUInt16);
}
uint32_t String::toUInt32(int base) const {
    TO_INT_DEF(uint32_t, toUInt32);
}
uint64_t String::toUInt64(int base) const {
    TO_INT_DEF(uint64_t, toUInt64);
}
#undef TO_INT_DEF

#define TRY_TO_INT_DEF(type, func)                               \
    const Char* begin;                                           \
    const Char* end;                                             \
    int len;                                                     \
    NumberConversionResult res;                                  \
    StringHelper::trim(c_str(), length(), &begin, &len);         \
    type num = StringHelper::func(begin, len, base, &end, &res); \
    if (end != begin + len) {                                    \
        return false;                                            \
    }                                                            \
    if (res != NumberConversionResult::Success) {                \
        return false;                                            \
    }                                                            \
    if (outValue != nullptr) {                                   \
        *outValue = num;                                         \
    }                                                            \
    return true;
bool String::tryToInt(int* outValue, int base) const {
    TRY_TO_INT_DEF(int, toInt32);
}
bool String::tryToInt8(int8_t* outValue, int base) const {
    TRY_TO_INT_DEF(int8_t, toInt8);
}
bool String::tryToInt16(int16_t* outValue, int base) const {
    TRY_TO_INT_DEF(int16_t, toInt16);
}
bool String::tryToInt32(int32_t* outValue, int base) const {
    TRY_TO_INT_DEF(int32_t, toInt32);
}
bool String::tryToInt64(int64_t* outValue, int base) const {
    TRY_TO_INT_DEF(int64_t, toInt64);
}
bool String::tryToUInt8(uint8_t* outValue, int base) const {
    TRY_TO_INT_DEF(uint8_t, toUInt8);
}
bool String::tryToUInt16(uint16_t* outValue, int base) const {
    TRY_TO_INT_DEF(uint16_t, toUInt16);
}
bool String::tryToUInt32(uint32_t* outValue, int base) const {
    TRY_TO_INT_DEF(uint32_t, toUInt32);
}
bool String::tryToUInt64(uint64_t* outValue, int base) const {
    TRY_TO_INT_DEF(uint64_t, toUInt64);
}
#undef TRY_TO_INT_DEF

//#ifdef LN_STRING_WITH_ENCODING
//ByteBuffer String::convertTo(const String& str, const TextEncoding* encoding, bool* outUsedDefaultChar)
//{
//	TextEncoding* thisEncoding = TextEncoding::utf16Encoding();
//	if (encoding == thisEncoding)	// TODO: ポインタ比較ではダメ。結果は正しいが。
//	{
//		return ByteBuffer(str.c_str(), (str.length() + 1) * sizeof(Char));
//	}
//	else
//	{
//		EncodingConversionOptions options;
//		options.NullTerminated = true;
//
//		EncodingConversionResult result;
//		ByteBuffer buf = TextEncoding::convert(str.c_str(), str.length() * sizeof(Char), thisEncoding, encoding, options, &result);
//		if (outUsedDefaultChar != nullptr)
//		{
//			*outUsedDefaultChar = result.UsedDefaultChar;
//		}
//		return buf;
//	}
//}
//#endif

String String::remove(Char ch, CaseSensitivity cs) const {
    return remove(StringView(&ch, 1), cs);
}

String String::concat(const StringView& str1, const StringView& str2) {
    String s;
    s.reserve(str1.length() + str2.length());
    s.append(str1.data(), str1.length());
    s.append(str2.data(), str2.length());
    return s;
}

String String::concat(const StringView& str1, const StringView& str2, const StringView& str3) {
    String s;
    s.reserve(str1.length() + str2.length() + str3.length());
    s.append(str1.data(), str1.length());
    s.append(str2.data(), str2.length());
    s.append(str3.data(), str3.length());
    return s;
}
String String::concat(const StringView& str1, const StringView& str2, const StringView& str3, const StringView& str4) {
    String s;
    s.reserve(str1.length() + str2.length() + str3.length() + str4.length());
    s.append(str1.data(), str1.length());
    s.append(str2.data(), str2.length());
    s.append(str3.data(), str3.length());
    s.append(str4.data(), str4.length());
    return s;
}

String String::join(const Array<String>& list, const StringView& delim) {
    if (list.isEmpty()) return String();

    int len = delim.length() * (list.length() - 1);
    for (auto& str : list) {
        len += str.length();
    }

    String s;
    s.reserve(len);
    s.append(list[0]);
    for (int i = 1; i < list.length(); i++) {
        s.append(delim);
        s.append(list[i]);
    }
    return s;
}

int String::compare(const String& str1, const String& str2, CaseSensitivity cs) {
    return StringHelper::compare(str1.c_str(), str1.length(), str2.c_str(), str2.length(), LN_MAX(str1.length(), str2.length()), cs);
}

int String::compare(const StringView& str1, int index1, const StringView& str2, int index2, int length, CaseSensitivity cs) {
    const Char* s1 = str1.data() + index1;
    const Char* s2 = str2.data() + index2;
    return StringHelper::compare(s1, str1.length() - index1, s2, str2.length() - index2, length, cs);
}

std::string String::toUtf8() const {
    return toStdString(TextEncoding::utf8Encoding());
}

String String::fromUtf8(const std::string_view& s) {
    return fromStdString(s, TextEncoding::utf8Encoding());
}

std::u16string String::toUtf16() const {
    return UnicodeStringUtils::U32ToU16(c_str(), length());
}

String String::fromUtf16(const std::u16string_view& s) {
    String result;
    result.assignFromCStr(s.data(), static_cast<int>(s.length()), nullptr);
    return result;
}

std::filesystem::path String::toStdPath() const {
    return std::filesystem::path(c_str());
}

String String::fromCString(const char* str, int length, TextEncoding* encoding) {
    String result;
    result.assignFromCStr(str, length, nullptr, encoding);
    return result;
}

String String::fromCString(const wchar_t* str, int length) {
    String result;
    result.assignFromCStr(str, length, nullptr, nullptr);
    return result;
}

String String::fromStdString(const std::string_view& str, TextEncoding* encoding) {
    return fromCString(str.data(), static_cast<int>(str.length()), encoding);
}

String String::fromStdString(const std::wstring& str) {
    return fromCString(str.c_str(), static_cast<int>(str.length()));
}

String String::fromNarrow(std::string_view str, TextEncoding* encoding) {
    String result;
    result.assignFromCStr(str.data(), static_cast<int>(str.length()), nullptr, encoding);
    return result;
}

std::string String::toStdString(TextEncoding* encoding) const {
    if (!encoding) {
        encoding = TextEncoding::systemMultiByteEncoding();
    }
    std::vector<byte_t> bytes = encoding->encode(*this);
    return std::string(reinterpret_cast<const char*>(bytes.data()), bytes.size());
}

std::wstring String::toStdWString() const {
    std::vector<byte_t> bytes = TextEncoding::wideCharEncoding()->encode(*this);
    return std::wstring(reinterpret_cast<const wchar_t*>(bytes.data()), bytes.size() / sizeof(wchar_t));
}

String String::fromNumber(int32_t value, Char format) {
    return fromNumber((int64_t)value, format);
}

String String::fromNumber(int64_t value, Char format) {
    char buf[64];
    int len = StringHelper::int64ToString(value, static_cast<char>(format), buf, 64);
    return String::fromCString(buf, len);
}

String String::fromNumber(uint32_t value, Char format) {
    return fromNumber((uint64_t)value, format);
}

String String::fromNumber(uint64_t value, Char format) {
    char buf[64];
    int len = StringHelper::uint64ToString(value, static_cast<char>(format), buf, 64);
    return String::fromCString(buf, len);
}

String String::fromNumber(float value, Char format, int precision) {
    return fromNumber((double)value, format, precision);
}

String String::fromNumber(double value, Char format, int precision) {
    char buf[64];
    int len = StringHelper::doubleToString(value, static_cast<char>(format), precision, buf, 64);
    return String::fromCString(buf, len);
}

void String::init() LN_NOEXCEPT {
    m_data.core = nullptr;
    m_data.sso.length = 0;
}

void String::release() LN_NOEXCEPT {
    if (isNonSSO()) {
        LN_SAFE_RELEASE(m_data.core);
    }
}

void String::copy(const String& str) {
    if (this != &str) {
        if (isSSO()) {
            if (str.isSSO()) {
                // SSO -> SSO
                memcpy(&m_data, &str.m_data, sizeof(m_data));
            }
            else {
                // SSO -> NonSSO
                m_data.core = str.m_data.core;
                LN_SAFE_RETAIN(m_data.core);
                setNonSSO();
            }
        }
        else {
            if (str.isSSO()) {
                // NonSSO -> SSO
                release();
                memcpy(&m_data, &str.m_data, sizeof(m_data));
            }
            else {
                // NonSSO -> NonSSO
                release();
                m_data.core = str.m_data.core;
                LN_SAFE_RETAIN(m_data.core);
            }
        }
    }
}

void String::move(String&& str) LN_NOEXCEPT {
    if (isNonSSO()) {
        LN_SAFE_RELEASE(m_data.core);
    }

    memcpy(&m_data, &str.m_data, sizeof(m_data));
    str.init();
}

// 領域を確保し、sso かどうかのフラグをセットする。バッファサイズは変えるが長さ(length() で取れる値)は変えない。
// 中身は以前のものが維持され、新しい領域は不定値となる。
// 後で必ず unlockBuffer() を呼び出すこと。
// context は lockBuffer() と unlockBuffer() 間で渡しあうデータ。
// 今は自己代入回避のための細工に使っている。必ず unlockBuffer() に渡すこと。
// unlock するまでは \0 終端を保障しない。
// また、lock 前後でサイズが変わらなくても変更であるとみなし、COW 共有は解除され独立したバッファになる。
Char* String::lockBuffer(size_t requestSize, detail::StringLockContext* context) noexcept {
    context->newCore = nullptr;
    context->oldCore = nullptr;
    if (isSSO()) {
        if (requestSize < SSOCapacity) {
            // SSO -> SSO
        }
        else {
            // SSO -> NonSSO
            std::unique_ptr<detail::UStringCore> core(LN_NEW detail::UStringCore());
            core->reserve(static_cast<int>(requestSize));
            memcpy(core->get(), m_data.sso.buffer, LN_MIN(getSSOLength(), requestSize) * sizeof(Char));
            //core->get()[requestSize] = '\0';
            //m_data.core = core.get();
            context->newCore = core.get(); // 自己代入対策。unlock で メンバ変数に設定する
            core.release();
            setNonSSO();
            return context->newCore->get();
        }
    }
    else {
        if (m_data.core && !m_data.core->isShared() && requestSize <= m_data.core->capacity()) {
            // NonSSO で、共有されてもいないしサイズも間に合っているならいろいろ作り直す必要は無い
            //m_data.core->get()[requestSize] = '\0';
        }
        else {
            if (requestSize < SSOCapacity) {
                // NonSSO -> SSO
                if (m_data.core) {
                    detail::UStringCore* oldCore = m_data.core;
                    setSSO();
                    memcpy(m_data.sso.buffer, oldCore->get(), LN_MIN(oldCore->length(), requestSize) * sizeof(Char));
                    //oldCore->release();
                    context->oldCore = oldCore; // release old buffer (on unlock)
                }
                else {
                    setSSO();
                }
            }
            else {
                // NonSSO -> NonSSO
                if (m_data.core) {
                    if (m_data.core->isShared()) {
                        detail::UStringCore* oldCore = m_data.core;
                        m_data.core = LN_NEW detail::UStringCore();
                        m_data.core->reserve(static_cast<int>(requestSize));
                        memcpy(m_data.core->get(), oldCore->get(), LN_MIN(oldCore->length(), requestSize) * sizeof(Char));
                        //oldCore->release();
                        context->oldCore = oldCore; // release old buffer (on unlock)
                    }
                    else {
                        m_data.core->reserve(static_cast<int>(requestSize));
                    }
                }
                else {
                    m_data.core = LN_NEW detail::UStringCore();
                    m_data.core->reserve(static_cast<int>(requestSize));
                }
            }
        }
    }

    return getBuffer();
}

// 実際のサイズを確定する
void String::unlockBuffer(size_t confirmedSize, detail::StringLockContext* context) noexcept {
    if (context->oldCore) {
        context->oldCore->release();
    }
    if (context->newCore) {
        m_data.core = context->newCore;
    }

    if (isSSO()) {
        setSSOLength(confirmedSize);
    }
    else {
        m_data.core->get()[confirmedSize] = '\0';
        m_data.core->fixLength(confirmedSize);
    }
}

Char* String::getBuffer() {
    return (isSSO()) ? m_data.sso.buffer : m_data.core->get();
}

const Char* String::getBuffer() const LN_NOEXCEPT {
    return (isSSO()) ? m_data.sso.buffer : m_data.core->get();
}

void String::setSSOLength(size_t len) {
    m_data.sso.length = (len & 0x7F) << 1;
    m_data.sso.buffer[len] = '\0';
}

size_t String::getSSOLength() const LN_NOEXCEPT {
    return m_data.sso.length >> 1;
}

void String::setSSO() {
    m_data.sso.length = (static_cast<size_t>(m_data.sso.length) & 0x07) << 1;
}

void String::setNonSSO() {
    m_data.sso.length = 0x01;
}

void String::append(const Char* str, int len) {
    int firstLen = length();
    detail::StringLockContext context;
    Char* b = lockBuffer(firstLen + len, &context) + firstLen;
    memmove(b, str, len * sizeof(Char));
    unlockBuffer(firstLen + len, &context);
}

void String::append(const String& str) {
    append(str.c_str(), str.length());
}

void String::assign(const Char* str) {
    assign(str, static_cast<int>(detail::UStringHelper::strlen(str)));
}

void String::assign(const Char* str, int length) {
    if (str && *str) {
        //bool over = false;
        //if (checkOverRange(this, str, length))
        //{

        //	over = true;
        //}

        detail::StringLockContext context;
        Char* buf = lockBuffer(length, &context);
        memmove(buf, str, sizeof(Char) * length);
        unlockBuffer(length, &context);
    }
    else {
        clear();
    }
}

void String::assign(int count, Char ch) {
    if (count > 0) {
        detail::StringLockContext context;
        Char* buf = lockBuffer(count, &context);
        std::fill<Char*, Char>(buf, buf + count, ch);
        unlockBuffer(count, &context);
    }
    else {
        clear();
    }
}

void String::assign(const StringView& str) {
    // TODO: String 参照のときの特殊化
    assign(str.data(), str.length());
}

void String::assignFromCStr(const char* str, int length, bool* outUsedDefaultChar, TextEncoding* encoding) {
    int len = 0;
    bool ascii = true;

    if (str) { // Check if the string is ASCII only. Also, check the number of characters.
        length = (length < 0) ? INT_MAX : length;
        const char* pos = str;
        for (; *pos && len < length; ++pos, ++len) {
            if (isascii(*pos) == 0) {
                ascii = false;
            }
        }
    }

    if (ascii) {
        detail::StringLockContext context;
        Char* buf = lockBuffer(len, &context);
        for (int i = 0; i < len; ++i)
            buf[i] = str[i];
        unlockBuffer(len, &context);
    }
    else {
        assignFromCStrInternal((const byte_t*)str, len, 1, outUsedDefaultChar, encoding ? encoding : TextEncoding::systemMultiByteEncoding().get());
        //		TextEncoding* actualEncoding = encoding;
        //		if (!actualEncoding) actualEncoding = TextEncoding::systemMultiByteEncoding();
        //
        //        detail::StringLockContext context;
        //        size_t bufSize = TextEncoding::getConversionRequiredByteCount(actualEncoding, TextEncoding::tcharEncoding(), len) / sizeof(Char);
        //        Char* buf = lockBuffer(bufSize, &context);
        //
        //        TextDecodeResult result;
        //#if LN_USTRING32
        //        actualEncoding->convertToUTF32Stateless((const byte_t*)str, len, (UTF32*)buf, bufSize, &result);
        //#else
        //        actualEncoding->convertToUTF16Stateless((const byte_t*)str, len, (UTF16*)buf, bufSize, &result);
        //#endif
        //        unlockBuffer(result.outputByteCount / sizeof(Char), &context);
    }
}

void String::assignFromCStr(const wchar_t* str, int length, bool* outUsedDefaultChar, TextEncoding* encoding) {
    int len = 0;
    bool ascii = true;

    if (str) { // Check if the string is ASCII only. Also, check the number of characters.
        length = (length < 0) ? INT_MAX : length;
        const wchar_t* pos = str;
        for (; *pos && len < length; ++pos, ++len) {
            if (isascii(*pos) == 0) {
                ascii = false;
            }
        }
    }

    if (ascii) {
        detail::StringLockContext context;
        Char* buf = lockBuffer(len, &context);
        for (int i = 0; i < len; ++i)
            buf[i] = str[i];
        unlockBuffer(len, &context);
    }
    else {
        assignFromCStrInternal((const byte_t*)str, len, sizeof(wchar_t), outUsedDefaultChar, encoding ? encoding : TextEncoding::wideCharEncoding().get());
    }
}

void String::assignFromCStr(const char16_t* str, int length, bool* outUsedDefaultChar) {
    int len = 0;
    bool ascii = true;

    if (str) { // Check if the string is ASCII only. Also, check the number of characters.
        length = (length < 0) ? INT_MAX : length;
        const char16_t* pos = str;
        for (; *pos && len < length; ++pos, ++len) {
            if (isascii(*pos) == 0) {
                ascii = false;
            }
        }
    }

    if (ascii) {
        detail::StringLockContext context;
        Char* buf = lockBuffer(len, &context);
        for (int i = 0; i < len; ++i)
            buf[i] = str[i];
        unlockBuffer(len, &context);
    }
    else {
        assignFromCStrInternal((const byte_t*)str, len, sizeof(char16_t), outUsedDefaultChar, TextEncoding::utf16Encoding());
    }
}

void String::assignFromCStrInternal(const byte_t* bytes, int bytesCount, int elementSize, bool* outUsedDefaultChar, TextEncoding* encoding) {
    detail::StringLockContext context;
    size_t bufSize = TextEncoding::getConversionRequiredByteCount(encoding, TextEncoding::tcharEncoding(), bytesCount * elementSize) / sizeof(Char);
    Char* buf = lockBuffer(bufSize, &context);

    TextDecodeResult result;
#if LN_USTRING32
    encoding->convertToUTF32Stateless(bytes, bytesCount * elementSize, (UTF32*)buf, bufSize, &result);
#else
    encoding->convertToUTF16Stateless(bytes, bytesCount * elementSize, (UTF16*)buf, bufSize, &result);
#endif
    unlockBuffer(result.outputByteCount / sizeof(Char), &context);
}

void String::setAt(int index, Char ch) {
    int len = length();
    LN_CHECK(0 <= index && index < len);
    detail::StringLockContext context;
    Char* buf = lockBuffer(len, &context);
    buf[index] = ch;
    unlockBuffer(len, &context);
}

uint32_t String::getHashCode() const {
    if (isEmpty()) return 0;
    return ln::CRCHash::compute(c_str(), length());
}

const String& String::newLine() {
#ifdef LN_OS_WIN32
    static String nl(_TT("\r\n"));
    return nl;
#elif defined(LN_OS_MAC)
    static String nl(_TT("\r"));
    return nl;
#else
    static String nl(_TT("\n"));
    return nl;
#endif
}

#ifdef LN_STRING_WITH_PATH
String::String(const Path& path)
    : String(path.str()) {
}

String& String::operator=(const Path& rhs) {
    assign(rhs.str());
    return *this;
}
#endif

//namespace fmt {

////==============================================================================
//// GenericFormatStringBuilder
////==============================================================================
//
//template class GenericFormatStringBuilder<char16_t>;
//
//template<typename TChar>
//GenericFormatStringBuilder<TChar>::GenericFormatStringBuilder()
//	: m_buffer()
//	, m_bufferUsed(0)
//{
//}
//
//} // namespace fmt

//==============================================================================
// String globals
//==============================================================================

namespace detail {

//==============================================================================
// UStringConvert
//==============================================================================

//std::basic_string<TCHAR> UStringConvert::toStdTString(const Char* str)
//{
//	String t = str;
//#ifdef LN_UNICODE
//	return t.toStdWString();
//#else
//	return t.toStdString();
//#endif
//}

int UStringConvert::convertNativeString(const char* src, int srcLen, char* dst, int dstSize) {
    if (!dst || dstSize <= 0) return 0;
    if (src && srcLen >= 0) {
        int len = LN_MIN(srcLen, dstSize - 1);
        memcpy_s(dst, dstSize, src, len);
        dst[len] = '\0';
        return len;
    }
    else {
        dst[0] = '\0';
        return 0;
    }
}

int UStringConvert::convertNativeString(const char* src, int srcLen, wchar_t* dst, int dstSize) {
    if (!dst || dstSize <= 0) return 0;
    if (src && srcLen >= 0) {
        if (srcLen >= dstSize) return -1;
        size_t size;
        errno_t err = mbstowcs_s(&size, dst, dstSize, src, srcLen);
        if (err != 0) return -1;
        return (int)size - 1;
    }
    else {
        dst[0] = '\0';
        return 0;
    }
}

int UStringConvert::convertNativeString(const wchar_t* src, int srcLen, char* dst, int dstSize) {
    if (!dst || dstSize <= 0) return 0;
    if (src && srcLen >= 0) {
        if (srcLen >= dstSize) return -1;
        size_t size;
        errno_t err = wcstombs_s(&size, dst, dstSize, src, srcLen);
        if (err != 0) return -1;
        return (int)size - 1;
    }
    else {
        dst[0] = '\0';
        return 0;
    }
}

int UStringConvert::convertNativeString(const wchar_t* src, int srcLen, wchar_t* dst, int dstSize) {
    if (!dst || dstSize <= 0) return 0;
    if (src && srcLen >= 0) {
        int len = LN_MIN(srcLen, dstSize - 1);
        memcpy_s(dst, dstSize * sizeof(wchar_t), src, len * sizeof(wchar_t));
        dst[len] = '\0';
        return len;
    }
    else {
        dst[0] = '\0';
        return 0;
    }
}

int UStringConvert::convertNativeString(const char16_t* src, int srcLen, char* dst, int dstSize) {
#if defined(LN_OS_WIN32) && _MSC_VER >= 1900
    if (!dst || dstSize <= 0) return 0;
    if (src && srcLen >= 0) {
        mbstate_t state;
        char* p = dst;
        for (int n = 0; n < srcLen; ++n) {
            size_t rc = std::c16rtomb(p, src[n], &state);
            if (rc == (size_t)-1) break;
            p += rc;
        }
        return static_cast<int>(p - dst);
    }
    else {
        dst[0] = '\0';
        return 0;
    }
#else
    UTFConversionOptions options;
    options.ReplacementChar = '?';
    auto result = UnicodeUtils::convertUTF16toUTF8((const UTF16*)src, srcLen, (UTF8*)dst, dstSize, &options);
    if (result == UTFConversionResult_Success) {
        dst[options.ConvertedTargetLength] = '\0';
        return options.ConvertedTargetLength;
    }
    else {
        return 0;
    }
#endif
}

int UStringConvert::convertNativeString(const char16_t* src, int srcSize, wchar_t* dst, int dstSize) {
#ifdef LN_WCHAR_16
    return convertNativeString((const wchar_t*)src, srcSize, dst, dstSize);
#else
    LN_NOTIMPLEMENTED();
    return -1;
#endif
}

int UStringConvert::convertNativeString(const char32_t* src, int srcLen, char* dst, int dstSize) {
    UTFConversionOptions options;
    options.ReplacementChar = '?';
    auto result = UnicodeUtils::convertUTF32toUTF8(reinterpret_cast<const UTF32*>(src), srcLen, reinterpret_cast<UTF8*>(dst), dstSize, &options);
    if (result == UTFConversionResult_Success) {
        dst[options.ConvertedTargetLength] = '\0';
        return options.ConvertedTargetLength;
    }
    else {
        return 0;
    }
}

int UStringConvert::convertNativeString(const char32_t* src, int srcLen, wchar_t* dst, int dstSize) {
#ifdef LN_WCHAR_16
    UTFConversionOptions options;
    options.ReplacementChar = '?';
    auto result = UnicodeUtils::convertUTF32toUTF16(reinterpret_cast<const UTF32*>(src), srcLen, reinterpret_cast<UTF16*>(dst), dstSize, &options);
    if (result == UTFConversionResult_Success) {
        dst[options.ConvertedTargetLength] = '\0';
        return options.ConvertedTargetLength;
    }
    else {
        return 0;
    }
#else
    LN_NOTIMPLEMENTED();
    return -1;
#endif
}

//int UStringConvert::getMaxNativeStringConverLength(const char* src, int srcLen, const char* dst)
//{
//	return srcLen;
//}
//int UStringConvert::getMaxNativeStringConverLength(const char* src, int srcLen, const wchar_t* dst)
//{
//	int maxCodePoints = srcLen * 4;	// 最悪、UTF-8 と仮定
//	return maxCodePoints * 2;		// 最悪、全部サロゲートペアで構成されていると仮定
//}
//int UStringConvert::getMaxNativeStringConverLength(const wchar_t* src, int srcLen, const char* dst)
//{
//
//}
//int UStringConvert::getMaxNativeStringConverLength(const wchar_t* src, int srcLen, const wchar_t* dst);
//int UStringConvert::getMaxNativeStringConverLength(const char16_t* src, int srcLen, const char* dst);
//int UStringConvert::getMaxNativeStringConverLength(const char16_t* src, int srcLen, const wchar_t* dst);

void UStringConvert::convertToStdString(const char* src, int srcLen, std::string* outString) {
    outString->assign(src, srcLen);
}
void UStringConvert::convertToStdString(const char* src, int srcLen, std::wstring* outString) {
    auto str = String::fromCString(src, srcLen);
    *outString = str.toStdWString();
}
void UStringConvert::convertToStdString(const wchar_t* src, int srcLen, std::string* outString) {
    auto str = String::fromCString(src, srcLen);
    *outString = str.toStdString();
}
void UStringConvert::convertToStdString(const wchar_t* src, int srcLen, std::wstring* outString) {
    outString->assign(src, srcLen);
}
void UStringConvert::convertToStdString(const char16_t* src, int srcLen, std::string* outString) {
    LN_NOTIMPLEMENTED();
}
void UStringConvert::convertToStdString(const char16_t* src, int srcLen, std::wstring* outString) {
#ifdef LN_WCHAR_16
    outString->assign((const wchar_t*)(src), srcLen);
#else
    // UTF16 -> UTF32
    UTFConversionOptions options;
    options.ReplacementChar = '?';
    outString->resize(srcLen);
    UnicodeUtils::convertUTF16toUTF32(
        (const UTF16*)src, srcLen, (UTF32*)&((*outString)[0]), srcLen, &options);
    outString->shrink_to_fit();
#endif
}

void UStringConvert::convertToStdString(const char32_t* src, int srcLen, std::string* outString) {
#ifdef LN_OS_WIN32
    // SJIS へは変換したくない
    LN_NOTIMPLEMENTED();
#else
    *outString = UnicodeStringUtils::U32ToU8(src, srcLen);
#endif
}

void UStringConvert::convertToStdString(const char32_t* src, int srcLen, std::wstring* outString) {
    *outString = UnicodeStringUtils::U32ToWide(src, srcLen);
}

} // namespace detail
} // namespace ln

//==============================================================================
//
//==============================================================================
namespace std {

// for unordered_map key
std::size_t hash<ln::String>::operator()(const ln::String& key) const {
    return ln::CRCHash::compute(key.c_str(), key.length());
}

} // namespace std
