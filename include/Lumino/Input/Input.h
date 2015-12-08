
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
	static bool IsPress(const StringRef& bindingName);

	/**
		@brief		ボタンが新しく押された瞬間を判定します。
	*/
	static bool IsOnTrigger(const StringRef& bindingName);

	/**
		@brief		ボタンが離された瞬間を判定します。
	*/
	static bool IsOffTrigger(const StringRef& bindingName);

	/**
		@brief		ボタンが新しく押された瞬間とリピート状態を判定します。
	*/
	static bool IsRepeat(const StringRef& bindingName);

	/**
		@brief		指定した軸のアナログ値を取得します。(0.0～1.0)
	*/
	static float GetAxisValue(const StringRef& bindingName);

private:
};

/**
	@brief	
*/
class InputButtons
{
public:
	static const String& Left;
	static const String& Right;
	static const String& Up;
	static const String& Down;
};

LN_NAMESPACE_END
