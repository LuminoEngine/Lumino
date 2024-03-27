
#pragma once
#include "String.hpp"

namespace ln {

/**
 * 複合書式文字列と可変長引数リストから文字列を生成します。
 * 
 * 書式は std::fmt に従います。
 * https://fmt.dev/latest/index.html
 */
template<typename... TArgs>
inline String format(const StringView& format, TArgs&&... args) {
    try {
        std::basic_string_view<Char> view(format.data(), format.length());
        auto str = ::LN_FMT_NAMESPACE::format(view, std::forward<TArgs>(args)...);
        return String(str.c_str(), static_cast<int>(str.length()));
    }
    catch (::LN_FMT_NAMESPACE::format_error& e) {
        LN_ERROR("String::format errro: {}", e.what());
        return String::Empty;
    }
}

} // namespace ln
