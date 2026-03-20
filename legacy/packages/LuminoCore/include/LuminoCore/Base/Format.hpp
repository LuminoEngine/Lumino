#pragma once

// FMT_BEGIN_NAMESPACE / FMT_END_NAMESPACE を定義する方法ではコンパイルエラーを回避できなかったため、
// namespace そのものを変更する方法を取っています。
#define fmt lnfmt
#define LN_FMT_NAMESPACE lnfmt::v11

// namespace を変更した影響で標準の FMT_ASSERT 無いでコンパイルエラーが発生するため、
// 代わりの local_fmt_assert_fail を使うように定義しています。
#define LN_FMT_ASSERT(condition, message)                                                                              \
    ((condition)                                          \
         ? (void)0                                                                                                     \
         : ln::local_fmt_assert_fail(__FILE__, __LINE__, (message)))
namespace ln {
void local_fmt_assert_fail(const char* file, int line, const char* message);
}

#include "detail/fmt/format.h"
#include "detail/fmt/xchar.h"

#undef fmt
