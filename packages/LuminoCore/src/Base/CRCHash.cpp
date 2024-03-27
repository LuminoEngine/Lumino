
#include "Internal.hpp"
#include <string.h>
#include <LuminoCore/Base/CRCHash.hpp>

namespace ln {

uint32_t CRCHash::compute(const char* str, int len) {
    len = static_cast<int>((len < 0) ? strlen(str) : len);
    return calcCrcHash((const byte_t*)str, len);
}

uint32_t CRCHash::compute(const wchar_t* str, int len) {
    len = static_cast<int>(((len < 0) ? wcslen(str) : len) * sizeof(wchar_t));
    return calcCrcHash((const byte_t*)str, len);
}

uint32_t CRCHash::compute(const char16_t* str, int len) {
    len = static_cast<int>(((len < 0) ? std::char_traits<char16_t>::length(str) : len) * sizeof(char16_t));
    return calcCrcHash((const byte_t*)str, len);
}

uint32_t CRCHash::compute(const char32_t* str, int len) {
    len = static_cast<int>(((len < 0) ? std::char_traits<char32_t>::length(str) : len) * sizeof(char32_t));
    return calcCrcHash((const byte_t*)str, len);
}

} // namespace ln
