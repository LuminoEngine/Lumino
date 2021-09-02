
#pragma once
#include <vector>
#include <string>
#include <atomic>
#include <locale>
#include <assert.h>
#include "Common.hpp"
#include "List.hpp"
#include "Buffer.hpp"
#include "StringHelper.hpp"

namespace ln {
class TextEncoding;
class Locale;
class CharRef;
class StringRef;
class Path;
namespace detail {
class UStringCore;
struct StringLockContext;
}

/**
 */
class String2
{
public:
    static const String Empty;

    using value_type = Char;
    //size_t size() const { return length(); }

    String2();

};

} // namespace ln

