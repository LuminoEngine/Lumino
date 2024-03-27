#pragma once

#define LN_FMT_NAMESPACE ln::fmt
//#define FMT_HEADER_ONLY 1
//
//#define FMT_BEGIN_NAMESPACE \
//    namespace lnfmt {        \
//    namespace v7 {
//
//#define FMT_END_NAMESPACE \
//    }                     \
//using namespace v7; \
//    }
//
//#ifndef FMT_ASSERT
//#ifdef NDEBUG
//// FMT_ASSERT is not empty to avoid -Werror=empty-body.
//#define FMT_ASSERT(condition, message) ((void)0)
//#else
//#define FMT_ASSERT(condition, message)                                      \
//    ((condition) /* void() fails with -Winvalid-constexpr on clang 4.0.1 */ \
//         ? (void)0                                                          \
//         : ::lnfmt::detail::assert_fail(__FILE__, __LINE__, (message)))
//#endif
//#endif

// v7.1.3 では FMT_BEGIN_NAMESPACE がうまく機能しなかったため、強制的に上書きしてみる
//#define fmt lnfmt

//#define LN_FMT_BEGIN_NAMESPACE \
//    namespace LN_FMT_NAMESPACE {            \
//    inline namespace v9 {
//#define LN_FMT_END_NAMESPACE \
//    }                        \
//using namespace v9; \
//    }

#include "detail/fmt/format.h"
#include "detail/fmt/xchar.h"
//#if FMT_VERSION > 80000
//#include "detail/fmt/include/fmt/xchar.h"
//#endif
//#undef fmt

namespace ln {

} // namespace ln

