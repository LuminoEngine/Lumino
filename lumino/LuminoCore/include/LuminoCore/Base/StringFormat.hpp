
#pragma once
#include "String.hpp"

namespace ln {

/** 複合書式文字列と可変長引数リストから文字列を生成します。 */
template<typename... TArgs>
inline String format(const StringRef& format, TArgs&&... args)
{
    try {
        std::basic_string_view<Char> view(format.data(), format.length());
        auto str = ::fmt::format(view, std::forward<TArgs>(args)...);
        return String(str.c_str(), str.length());
    }
    catch (::fmt::format_error& e) {
        LN_ERROR("String::format errro: {}", e.what());
        return String::Empty;
    }
}

} // namespace ln
