// Copyright (c) 2018+ lriki. Distributed under the MIT license.

#pragma once

namespace ln {

/** ハッシュ値の計算を行うクラスです。 */
class CRCHash
{
public:

	/** 指定した文字列のハッシュ値を求めます。 */
	static uint32_t compute(const char* str, int len = -1);

	/** 指定した文字列のハッシュ値を求めます。 */
	static uint32_t compute(const wchar_t* str, int len = -1);

	/** 指定した文字列のハッシュ値を求めます。 */
	static uint32_t compute(const char16_t* str, int len = -1);
};

namespace detail {
struct CRCHashInternal
{
    static const unsigned* getCRCTable();
};
} // namespace detail
} // namespace ln
