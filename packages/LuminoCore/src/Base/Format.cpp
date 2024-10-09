#include <LuminoCore/Base/Format.hpp>
#define fmt lnfmt
#include <LuminoCore/Base/detail/fmt/format-inl.h>

//------------------------------------------------------------------------------
// see: LN_FMT_ASSERT
//------------------------------------------------------------------------------
namespace ln {
void local_fmt_assert_fail(const char* file, int line, const char* message) {
    // Use unchecked std::fprintf to avoid triggering another assertion when
    // writing to stderr fails
    std::fprintf(stderr, "%s:%d: assertion failed: %s", file, line, message);
    // Chosen instead of std::abort to satisfy Clang in CUDA mode during device
    // code pass.
    std::terminate();
}
} // namespace ln

//------------------------------------------------------------------------------
// 以下の内容は fmt.cc と同様です。
//------------------------------------------------------------------------------

// Formatting library for C++
//
// Copyright (c) 2012 - 2016, Victor Zverovich
// All rights reserved.
//
// For the license information refer to format.h.

LN_FMT_BEGIN_NAMESPACE
namespace detail {

template LN_FMT_API auto dragonbox::to_decimal(float x) noexcept -> dragonbox::decimal_fp<float>;
template LN_FMT_API auto dragonbox::to_decimal(double x) noexcept -> dragonbox::decimal_fp<double>;

#ifndef LN_FMT_STATIC_THOUSANDS_SEPARATOR
template LN_FMT_API locale_ref::locale_ref(const std::locale& loc);
template LN_FMT_API auto locale_ref::get<std::locale>() const -> std::locale;
#endif

// Explicit instantiations for char.

template LN_FMT_API auto thousands_sep_impl(locale_ref) -> thousands_sep_result<char>;
template LN_FMT_API auto decimal_point_impl(locale_ref) -> char;

template LN_FMT_API void buffer<char>::append(const char*, const char*);

template LN_FMT_API void vformat_to(buffer<char>&, string_view, typename vformat_args<>::type, locale_ref);

// Explicit instantiations for wchar_t.

template LN_FMT_API auto thousands_sep_impl(locale_ref) -> thousands_sep_result<wchar_t>;
template LN_FMT_API auto decimal_point_impl(locale_ref) -> wchar_t;

template LN_FMT_API void buffer<wchar_t>::append(const wchar_t*, const wchar_t*);

} // namespace detail
LN_FMT_END_NAMESPACE
