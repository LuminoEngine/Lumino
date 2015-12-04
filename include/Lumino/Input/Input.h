
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief	
*/
class Input
{
public:

	/**
		@brief		ボタンが現在押されているかを判定します。
	*/
	static bool IsPress(int bindingIndex) const;
	static bool IsPress(const TCHAR* bindingName) const;

	/**
		@brief		ボタンが新しく押された瞬間を判定します。
	*/
	static bool IsOnTrigger(int bindingIndex) const;
	static bool IsOnTrigger(const TCHAR* bindingName) const;

	/**
		@brief		ボタンが離された瞬間を判定します。
	*/
	static bool IsOffTrigger(int bindingIndex) const;
	static bool IsOffTrigger(const TCHAR* bindingName) const;

	/**
		@brief		ボタンが新しく押された瞬間とリピート状態を判定します。
	*/
	static bool IsRepeat(int bindingIndex) const;
	static bool IsRepeat(const TCHAR* bindingName) const;

	/**
		@brief		指定した軸のアナログ値を取得します。(0.0～1.0)
	*/
	static float GetAxisValue(int bindingIndex) const;
	static float GetAxisValue(const TCHAR* bindingName) const;

private:
};

LN_NAMESPACE_END
