/**
	@file	Hash.h
*/
#pragma once

LN_NAMESPACE_BEGIN

/**
	@brief		ハッシュ値の計算を行うクラスです。
*/
class Hash
{
public:

	/**
		@brief		指定した文字列のハッシュ値を求めます。
	*/
	static uint32_t calcHash(const char* str, int len = -1);
	static uint32_t calcHash(const wchar_t* str, int len = -1);
};

LN_NAMESPACE_END
