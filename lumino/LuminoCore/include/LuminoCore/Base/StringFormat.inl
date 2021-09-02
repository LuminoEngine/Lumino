
#include <array>
#include <sstream>
#include <iomanip>
#include "Locale.hpp"
#include "StringHelper.hpp"

#if defined(_MSC_VER) && _MSC_VER <= 1800 // VS2013
#define LN_FORMAT_BRACED_INIT_WORKAROUND(x) (x)
#else
#define LN_FORMAT_BRACED_INIT_WORKAROUND(x) \
    {                                       \
        x                                   \
    }
#endif

namespace ln {

template<typename TChar, typename TKind, typename TValue>
struct Formatter;

namespace fmt {

template<typename TChar>
GenericFormatStringRef<TChar>::GenericFormatStringRef()
{
    m_str = nullptr;
    m_length = 0;
}

template<typename TChar>
GenericFormatStringRef<TChar>::GenericFormatStringRef(const TChar* begin, const TChar* end)
{
    m_str = begin;
    m_length = end - begin;
}
template<typename TChar>
GenericFormatStringRef<TChar>::GenericFormatStringRef(const GenericFormatStringRef& str)
{
    m_str = str.m_str;
    m_length = str.m_length;
}

//==============================================================================
// GenericFormatStringBuilder
//==============================================================================

template<typename TChar>
GenericFormatStringBuilder<TChar>::GenericFormatStringBuilder()
    : m_buffer()
    , m_bufferUsed(0)
    , m_fixedBuffer(nullptr)
    , m_fixedBufferSize(0)
    , m_fixedBufferOver(false)
{
    ln::detail::GenericBufferHelper::setAutoClear(&m_buffer, false);
}

template<typename TChar>
GenericFormatStringBuilder<TChar>::GenericFormatStringBuilder(TChar* buffer, size_t bufferSize)
    : GenericFormatStringBuilder()
{
    assert(buffer);
    m_fixedBuffer = (byte_t*)buffer;
    m_fixedBufferSize = bufferSize * sizeof(TChar);
}

template<typename TChar>
void GenericFormatStringBuilder<TChar>::clear()
{
    m_bufferUsed = 0;
}

template<typename TChar>
void GenericFormatStringBuilder<TChar>::appendChar(TChar ch)
{
    appendIntenal(&ch, 1);
}

template<typename TChar>
void GenericFormatStringBuilder<TChar>::appendChar(TChar ch, int count)
{
    for (int i = 0; i < count; ++i) {
        appendChar(ch);
    }
}

template<typename TChar>
void GenericFormatStringBuilder<TChar>::appendString(const TChar* str)
{
    appendIntenal(str, ln::detail::UStringHelper::strlen(str));
}

template<typename TChar>
void GenericFormatStringBuilder<TChar>::appendString(const TChar* str, size_t length)
{
    appendIntenal(str, length);
}

template<typename TChar>
void GenericFormatStringBuilder<TChar>::appendString(const String& str)
{
    appendIntenal(str.c_str(), str.length());
}

template<typename TChar>
const TChar* GenericFormatStringBuilder<TChar>::c_str() const
{
    return (const TChar*)m_buffer.data();
}

template<typename TChar>
size_t GenericFormatStringBuilder<TChar>::length() const
{
    return m_bufferUsed / sizeof(TChar);
}

template<typename TChar>
void GenericFormatStringBuilder<TChar>::appendIntenal(const TChar* str, size_t length)
{
    size_t byteCount = sizeof(TChar) * length;
    byte_t* writeBegin = nullptr;
    size_t writeSize = 0;
    if (m_fixedBuffer) {
        // バッファが足りなければエラー
        if (m_fixedBufferOver || m_bufferUsed + byteCount > m_fixedBufferSize) {
            m_fixedBufferOver = true;
            return;
        }

        writeBegin = m_fixedBuffer + m_bufferUsed;
        writeSize = m_fixedBufferSize - m_bufferUsed;
    } else {
        // バッファが足りなければ拡張する
		size_t bufferSize = static_cast<size_t>(m_buffer.size());
        if (m_bufferUsed + byteCount > bufferSize) {
            size_t newSize = m_buffer.size() + LN_MAX(bufferSize, byteCount); // 最低でも byteCount 分を拡張する
            m_buffer.resize(static_cast<int>(newSize));
        }

        writeBegin = &(m_buffer.data()[m_bufferUsed]);
        writeSize = m_buffer.size() - m_bufferUsed;
    }

    memcpy(writeBegin, str, byteCount);
    m_bufferUsed += byteCount;
}

namespace detail {

//==============================================================================
// global
//==============================================================================

#if defined(_MSC_VER) && _MSC_VER <= 1800 // VS2013
#define LN_FMT_BRACED_INIT_WORKAROUND(x) (x)
#else
#define LN_FMT_BRACED_INIT_WORKAROUND(x) \
    {                                    \
        x                                \
    }
#endif

typedef void (*FormatFunc)(void* formatter, const void* value);

template<typename TChar, typename TValue>
void formatInternal_Numeric(GenericStringFormatter<TChar>& formatter, const TValue value)
{
    char buf[64];
    ln::detail::StdCharArrayBuffer<char> b(buf, 64);
    std::basic_ostream<char, std::char_traits<char>> os(&b);
    os.imbue(*formatter.locale());

    auto& formatParam = formatter.precision();
    int32_t precision = -1;
    if (!formatParam.isEmpty()) {
        NumberConversionResult result;
        const TChar* dummy;
        precision = StringHelper::toInt32(formatParam.begin(), static_cast<int>(formatParam.length()), 10, &dummy, &result);
        if (LN_ENSURE(result == NumberConversionResult::Success)) return;
    }

    auto& format = formatter.formatString();
    if (format.isEmpty()) {
    } else if (format.length() == 1) {
        if (format[0] == 'd' || format[0] == 'D') {
            if (precision >= 0) {
                // 0埋め
                os << std::setfill('0') << std::setw(precision);
            }
        } else if (format[0] == 'x' || format[0] == 'X') {
            os << std::hex;
            if (format[0] == 'X') {
                os << std::uppercase;
            }

            if (precision >= 0) {
                // 0埋め
                os << std::setfill('0') << std::setw(precision);
            }
        } else if (format[0] == 'f' || format[0] == 'F') {
            os << std::fixed;

            if (precision >= 0) {
                // 小数点以下の精度
                os << std::setprecision(precision);
            }
        } else if (format[0] == 'e' || format[0] == 'E') {
            os << std::scientific;
            if (format[0] == 'E') {
                os << std::uppercase;
            }

            if (precision >= 0) {
                // 小数点以下の精度
                os << std::setprecision(precision);
            }
        }
    } else {
        LN_ENSURE(0);
        return;
    }
    // http://sla0.jp/2012/04/cpp%E3%81%AEiostream%E3%83%95%E3%82%A9%E3%83%BC%E3%83%9E%E3%83%83%E3%83%88%E6%8C%87%E5%AE%9A%E6%97%A9%E8%A6%8B%E8%A1%A8/

    os << value;

    // convert
    {
        size_t len = b.length();
        for (size_t i = 0; i < len; i++) {
            formatter.getSB().appendChar(buf[i]);
        }
    }
}

template<typename TChar>
class FormatArg
{
public:
#define LN_FMT_MAKE_NUMERIC_VALUE(type)             \
    FormatArg(const type& value)                    \
        : m_value(static_cast<const void*>(&value)) \
        , m_format(&formatArg_##type)               \
    {                                               \
    }                                               \
    static void formatArg_##type(void* formatter, const void* value) { formatInternal_Numeric<TChar, type>(*static_cast<GenericStringFormatter<TChar>*>(formatter), *static_cast<const type*>(value)); }

    LN_FMT_MAKE_NUMERIC_VALUE(int16_t);
    LN_FMT_MAKE_NUMERIC_VALUE(int32_t);
    LN_FMT_MAKE_NUMERIC_VALUE(int64_t);
    LN_FMT_MAKE_NUMERIC_VALUE(uint16_t);
    LN_FMT_MAKE_NUMERIC_VALUE(uint32_t);
    LN_FMT_MAKE_NUMERIC_VALUE(uint64_t);
    LN_FMT_MAKE_NUMERIC_VALUE(float);
    LN_FMT_MAKE_NUMERIC_VALUE(double);

    // bool
    FormatArg(const bool& value)
        : m_value(reinterpret_cast<const void*>(value))
        , m_format(&formatArg_bool)
    {
    }
    static void formatArg_bool(void* formatter, const void* value)
    {
        auto& f = *static_cast<GenericStringFormatter<TChar>*>(formatter);

        if (value) {
            const TChar str[] = {'T', 'r', 'u', 'e'};
            f.getSB().appendString(str, 4);
        } else {
            const TChar str[] = {'F', 'a', 'l', 's', 'e'};
            f.getSB().appendString(str, 5);
        }
    }

    // TChar
    FormatArg(const TChar& value)
        : m_value(static_cast<const void*>(&value))
        , m_format(&formatArg_Char)
    {
    }
    static void formatArg_Char(void* formatter, const void* value)
    {
        auto& f = *static_cast<GenericStringFormatter<TChar>*>(formatter);
        f.getSB().appendChar(*static_cast<const TChar*>(value));
    }

    // TChar*
    // const TChar*
    FormatArg(TChar* value)
        : m_value(static_cast<const void*>(value))
        , m_format(&formatArg_CStr)
    {
    }
    FormatArg(const TChar* value)
        : m_value(static_cast<const void*>(value))
        , m_format(&formatArg_CStr)
    {
    }
    static void formatArg_CStr(void* formatter, const void* value)
    {
        auto& f = *static_cast<GenericStringFormatter<TChar>*>(formatter);
        f.getSB().appendString(static_cast<const TChar*>(value));
    }

    // String
    FormatArg(const String& value)
        : m_value(static_cast<const void*>(&value))
        , m_format(&formatArg_UString)
    {
    }
    static void formatArg_UString(void* formatter, const void* value)
    {
        auto& f = *static_cast<GenericStringFormatter<TChar>*>(formatter);
        f.getSB().appendString(*static_cast<const String*>(value));
    }

    // StringRef
    FormatArg(const StringRef& value)
        : m_value(static_cast<const void*>(&value))
        , m_format(&formatArg_UStringRef)
    {
    }
    static void formatArg_UStringRef(void* formatter, const void* value)
    {
        auto& f = *static_cast<GenericStringFormatter<TChar>*>(formatter);
        const StringRef* ref = static_cast<const StringRef*>(value);
        f.getSB().appendString(ref->data(), ref->length());
    }

    // user-defined type
    template<typename T>
    FormatArg(const T& value)
        : m_value(static_cast<const void*>(&value))
        , m_format(&formatArg_custom<T>)
    {
    }

    template<typename T>
    static void formatArg_custom(void* formatter, const void* value)
    {
        formatArg(
            *static_cast<GenericStringFormatter<TChar>*>(formatter),
            *reinterpret_cast<const T*>(value));
    }

    void doFormat(GenericStringFormatter<TChar>* formatter) const
    {
        return m_format(formatter, m_value);
    }

private:
    const void* m_value;
    FormatFunc m_format;
};

// base class of variadic args
template<typename TChar>
class FormatList
{
public:
    FormatList()
        : m_argList(nullptr)
        , m_count(0)
    {
    }

    const FormatArg<TChar>& GetArg(int index) const { return m_argList[index]; }
    int getCount() const { return m_count; }

protected:
    const FormatArg<TChar>* m_argList;
    int m_count;
};

// variadic args (N > 0)
template<typename TChar, int N>
class FormatListN : public FormatList<TChar>
{
public:
    template<typename... Args>
    FormatListN(const Args&... args)
        : FormatList<TChar>()
        , m_argListInstance LN_FMT_BRACED_INIT_WORKAROUND({FormatArg<TChar>(args)...}) // extract to -> {FormatArg(e1), FormatArg(e2), FormatArg(e3)} http://en.cppreference.com/w/cpp/language/parameter_pack
    {
        static_assert(sizeof...(args) == N, "Invalid args count.");
        FormatList<TChar>::m_argList = &m_argListInstance[0];
        FormatList<TChar>::m_count = N;
    }

private:
    std::array<FormatArg<TChar>, N> m_argListInstance;
};

// variadic args (N = 0)
template<typename TChar>
class FormatListN<TChar, 0> : public FormatList<TChar>
{
public:
    FormatListN() {}
};

template<typename TChar, typename... TArgs>
static FormatListN<TChar, sizeof...(TArgs)> makeArgList(const TArgs&... args)
{
    return FormatListN<TChar, sizeof...(args)>(args...);
}

template<typename TChar>
bool formatInternal(const Locale& locale, GenericFormatStringBuilder<TChar>* outStr, const TChar* format, size_t formatLen, const FormatList<TChar>& args)
{
    enum NumberingState
    {
        Prologue,
        Manual,
        Auto,
    };

    NumberingState numberingState = NumberingState::Prologue;
    int autoNumberingIndex = -1;

    GenericStringFormatter<TChar> formatter;
    formatter.setLocale(&locale.stdLocale());
    const TChar* pos = format;
    const TChar* end = format + formatLen;
    TChar ch;
    while (pos < end) {
        // find '{' roop
        while (pos < end) {
            ch = *pos;
            ++pos;

            if (ch == '}') {
                if (pos < end && *pos == '}') // "}}" escape
                {
                    ++pos;
                } else {
                    // Error: 単発の } が現れてはならない
                    formatter.reportError("parse error '}'", static_cast<int>(format - pos));
                    return false;
                }
            }
            if (ch == '{') {
                if (pos < end && *pos == '{') // "{{" escape
                {
                    ++pos;
                } else if (pos >= end) {
                    // Error: 単発の { で終わった
                    formatter.reportError("parse error '{'", static_cast<int>(format - pos));
                    return false;
                } else {
                    break;
                }
            }

            outStr->appendChar(ch);
        }
        // この時点で pos は { の次を指している

        if (pos >= end) {
            break;
        }

        // 次の文字は必ず数字でなければならない
        NumberingState requireNumberingState;
        if ('0' <= *pos && *pos <= '9') {
            requireNumberingState = NumberingState::Manual;
        } else {
            requireNumberingState = NumberingState::Auto;
        }

        if (numberingState == NumberingState::Prologue) {
            numberingState = requireNumberingState;
        } else if (numberingState == NumberingState::Manual && requireNumberingState == NumberingState::Auto) {
            formatter.reportError("cannot switch from manual field specification to automatic field numbering", static_cast<int>(format - pos));
            return false;
        } else if (numberingState == NumberingState::Auto && requireNumberingState == NumberingState::Manual) {
            formatter.reportError("cannot switch from automatic field numbering to manual field specification", static_cast<int>(format - pos));
            return false;
        } else {
            // numberingState equals requireNumberingState
        }

        //-----------------------------------------------------------
        // Index component
        int index = 0;
        if (numberingState == NumberingState::Manual) {
            do {
                index = (index * 10) + ((*pos) - '0');
                ++pos;
                if (pos >= end) {
                    // Error: インデックス解析中に \0 になった
                    formatter.reportError("expected index number", static_cast<int>(format - pos));
                    return false;
                }

            } while ((*pos) >= '0' && (*pos) <= '9');

            if (index >= args.getCount()) {
                // Error: 引数の数よりも大きいインデックスがある
                formatter.reportError("index is out of args range", static_cast<int>(format - pos));
                return false;
            }
        } else {
            autoNumberingIndex++;
            index = autoNumberingIndex;
        }

        //-----------------------------------------------------------
        // Alignment component
        while (pos < end && *pos == ' ') {
            pos++;
        }                         // 先頭の空白を無視
        bool leftJustify = false; // 左詰めにするか？
        int width = 0;
        if (*pos == ',') // , であれば解析開始。無ければ省略されている
        {
            pos++;
            while (pos < end && *pos == ' ') {
                pos++;
            }
            if (pos >= end) {
                // Error: EOF
                formatter.reportError("parse error alignment component", static_cast<int>(format - pos));
                return false;
            }

            if (*pos == '-') // 符号が - なら左詰め
            {
                leftJustify = true;
                ++pos;
                if (pos >= end) {
                    // Error: EOF
                    formatter.reportError("parse error alignment component", static_cast<int>(format - pos));
                    return false;
                }
            }

            // 次は絶対数字
            if ('0' <= *pos && *pos <= '9') {
            } else {
                formatter.reportError("expected number", static_cast<int>(format - pos));
                return false;
            }

            do {
                width = (width * 10) + ((*pos) - '0');
                ++pos;
                if (pos >= end) {
                    // Error: EOF
                    formatter.reportError("parse error alignment component", static_cast<int>(format - pos));
                    return false;
                }

            } while ((*pos) >= '0' && (*pos) <= '9');
        }

        //-----------------------------------------------------------
        // FormatString component
        while (pos < end && *pos == ' ') {
            pos++;
        } // ignore head space
        const TChar* fmtBegin = nullptr;
        const TChar* fmtEnd = nullptr;
        const TChar* fmtParamEnd = nullptr;
        if (*pos == ':') {
            pos++;
            fmtBegin = pos;
            while (true) {
                if (pos >= end) {
                    // Error: EOF
                    formatter.reportError("parse error FormatString component", static_cast<int>(format - pos));
                    return false;
                }

                if (*pos == '}') {
                    break;
                }
                ++pos;
                fmtEnd = pos;
            }

            fmtParamEnd = fmtEnd;
            // "D4" のように後ろが整数のパラメータになっているとき、ここで整数部分を取り出してしまう (変換側が楽できるように)
            ch = *(fmtEnd - 1);
            if (ch >= '0' && ch <= '9') {
                fmtParamEnd = fmtEnd;
                do {
                    --fmtEnd;
                    ch = *(fmtEnd - 1);

                } while (ch >= '0' && ch <= '9');
            }
        }

        // 最後は } でなければならない
        if (*pos != '}') {
            formatter.reportError("expected number", static_cast<int>(format - pos));
            return false;
        }

        formatter.getSB().clear();
        formatter.setFormatString(GenericFormatStringRef<TChar>(fmtBegin, fmtEnd));
        formatter.setPrecision(GenericFormatStringRef<TChar>(fmtEnd, fmtParamEnd));
        args.GetArg(index).doFormat(&formatter);
        if (formatter.hasError()) {
            return false;
        }

        auto& str = formatter.getSB();
        int pad = static_cast<int>(width - str.length());
        if (!leftJustify && pad > 0) outStr->appendChar(' ', pad);
        outStr->appendString(str.c_str(), str.length());
        if (leftJustify && pad > 0) outStr->appendChar(' ', pad);

        if (outStr->isFixedBufferOver()) {
            return false;
        }

        ++pos;
    }

    return true;
}

template<typename TChar, typename... TArgs>
static inline int formatFixed(TChar* buffer, size_t bufferSize, const Locale& locale, const StringRef& format, TArgs&&... args)
{
    auto argList = fmt::detail::makeArgList<Char>(std::forward<TArgs>(args)...);
    fmt::GenericFormatStringBuilder<Char> sb(buffer, bufferSize - 1);
    if (fmt::detail::formatInternal<Char>(locale, &sb, format.data(), format.length(), argList)) {
        buffer[sb.length()] = '\0';
        return sb.length();
    }
    return -1;
}

} // namespace detail
} // namespace fmt

//==============================================================================
// String

template<typename... TArgs>
inline String String::format(const StringRef& format, TArgs&&... args)
{
    auto argList = fmt::detail::makeArgList<Char>(std::forward<TArgs>(args)...);
    fmt::GenericFormatStringBuilder<Char> sb;
    if (fmt::detail::formatInternal<Char>(Locale::getC(), &sb, format.data(), format.length(), argList)) {
        return String(sb.c_str(), static_cast<int>(sb.length()));
    } else {
        return String();
    }
}

template<typename... TArgs>
inline String String::format(const Locale& locale, const StringRef& format, TArgs&&... args)
{
    auto argList = fmt::detail::makeArgList<Char>(std::forward<TArgs>(args)...);
    fmt::GenericFormatStringBuilder<Char> sb;
    if (fmt::detail::formatInternal<Char>(locale, &sb, format.data(), format.length(), argList)) {
        return String(sb.c_str(), sb.length());
    } else {
        return String();
    }
}

} // namespace ln
