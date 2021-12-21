
#include "Internal.hpp"
#include <LuminoCore/Base/String.hpp>
#include <LuminoCore/Text/Encoding.hpp>
#include <LuminoCore/IO/Path.hpp>

namespace ln {

StringRef::StringRef() LN_NOEXCEPT
    : m_string(nullptr)
    , m_str(nullptr)
    , m_len(0)
    , m_localAlloc(false)
{
}

StringRef::StringRef(const StringRef& str) LN_NOEXCEPT
    : StringRef()
{
    m_str = str.data();
    m_len = str.length();
}

StringRef::StringRef(const String& str)
    : StringRef()
{
    m_string = &str;
    m_str = str.c_str();
    m_len = str.length();
}

StringRef::StringRef(const String& str, int len)
    : StringRef()
{
    m_string = &str;
    m_str = str.c_str();
    m_len = len;
}

StringRef::StringRef(const String& str, int startIndex, int len)
    : StringRef()
{
    m_str = str.c_str() + startIndex;
    m_len = len;
}

StringRef::StringRef(const Char* str)
    : StringRef()
{
    m_str = str;
    m_len = detail::UStringHelper::strlen(str);
}

StringRef::StringRef(const Char* str, int len)
    : StringRef()
{
    m_str = str;
    m_len = len;
}

StringRef::StringRef(const Char* begin, const Char* end)
    : StringRef()
{
    m_str = begin;
    m_len = end - begin;
}

StringRef::StringRef(const Path& path)
    : StringRef(path.str())
{
}

#ifdef LN_STRING_FUZZY_CONVERSION
StringRef::StringRef(const char* str)
    : StringRef()
{
    m_string = new String(str);
    m_str = m_string->c_str();
    m_len = m_string->length();
    m_localAlloc = true;
}
#ifdef LN_USTRING16
StringRef::StringRef(const wchar_t* str)
    : StringRef()
{
    m_string = new String(str);
    m_str = m_string->c_str();
    m_len = m_string->length();
    m_localAlloc = true;
}
#endif
#endif

StringRef StringRef::substr(int start, int count) const
{
    const Char* begin;
    const Char* end;
    StringHelper::substr(data(), length(), start, count, &begin, &end);
    return StringRef(begin, end);
    //// TODO: Ref で返していいよね？
    ////return StringHelper::mid(getBegin(), start, count);
    //return String(data(), count).substr(start, count);
}

bool StringRef::endsWith(const StringRef& str, CaseSensitivity cs) const
{
    if (isEmpty() || str.isEmpty()) {
        return false;
    }
    return StringHelper::endsWith(data(), length(), str.data(), str.length(), cs);
}

int StringRef::indexOf(const StringRef& str, int startIndex, CaseSensitivity cs) const
{
    return StringHelper::indexOf(data(), length(), str.data(), str.length(), startIndex, cs);
}

int StringRef::indexOf(Char ch, int startIndex, CaseSensitivity cs) const
{
    return StringHelper::indexOf(data(), length(), &ch, 1, startIndex, cs);
}

int StringRef::lastIndexOf(const StringRef& str, int startIndex, int count, CaseSensitivity cs) const
{
    return StringHelper::lastIndexOf(data(), length(), str.data(), str.length(), startIndex, count, cs);
}

int StringRef::lastIndexOf(Char ch, int startIndex, int count, CaseSensitivity cs) const
{
    return StringHelper::lastIndexOf(data(), length(), &ch, 1, startIndex, count, cs);
}

#define TO_INT_DEF(type, func)                                   \
    const Char* begin;                                           \
    const Char* end;                                             \
    int len;                                                     \
    NumberConversionResult res;                                  \
    StringHelper::trim(data(), length(), &begin, &len);          \
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
int StringRef::toInt(int base) const
{
    TO_INT_DEF(int32_t, toInt32);
}
#undef TO_INT_DEF;

std::string StringRef::toStdString() const
{
    std::vector<byte_t> bytes = TextEncoding::systemMultiByteEncoding()->encode(data(), length());
    return std::string(reinterpret_cast<const char*>(bytes.data()), bytes.size());
}

std::wstring StringRef::toStdWString() const
{
    std::vector<byte_t> bytes = TextEncoding::wideCharEncoding()->encode(data(), length());
    return std::wstring(reinterpret_cast<const wchar_t*>(bytes.data()), bytes.size() / sizeof(wchar_t));
}

std::string StringRef::toUtf8() const {
    std::vector<byte_t> bytes = TextEncoding::utf8Encoding()->encode(data(), length());
    return std::string(reinterpret_cast<const char*>(bytes.data()), bytes.size());
}

size_t StringRef::getHashCode() const
{
    if (isEmpty()) return 0;
    return CRCHash::compute(data(), length());
}

void StringRef::clear()
{
    if (m_localAlloc) {
        delete m_string;
        m_localAlloc = false;
    }
    m_string = nullptr;
    m_str = nullptr;
    m_len = 0;
}

StringRef& StringRef::operator=(const StringRef& str)
{
    if (this != &str) {
        clear();
        m_str = str.data();
        m_len = str.length();
    }
    return *this;
}

StringRef& StringRef::operator=(StringRef&& str)
{
    clear();
    m_string = str.m_string;
    m_str = str.m_str;
    m_len = str.m_len;
    m_localAlloc = str.m_localAlloc;
    str.m_string = nullptr;
    str.m_str = nullptr;
    str.m_len = 0;
    str.m_localAlloc = false;
    return *this;
}

} // namespace ln
