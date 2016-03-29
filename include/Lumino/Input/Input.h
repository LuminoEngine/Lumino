
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
	static bool IsPressed(const StringRef& bindingName);

	/**
		@brief		ボタンが新しく押された瞬間を判定します。
	*/
	static bool IsTriggered(const StringRef& bindingName);

	/**
		@brief		ボタンが離された瞬間を判定します。
	*/
	static bool IsOffTriggered(const StringRef& bindingName);

	/**
		@brief		ボタンが新しく押された瞬間とリピート状態を判定します。
	*/
	static bool IsRepeated(const StringRef& bindingName);

	/**
		@brief		指定した軸のアナログ値を取得します。(0.0～1.0)
	*/
	static float GetAxisValue(const StringRef& bindingName);

	static VirtualPad* GetController(int index);

private:
};

/**
	@brief	
*/
class InputButtons
{
public:
	static const String Any;
	static const String Left;
	static const String Right;
	static const String Up;
	static const String Down;
	static const String Ok;
	static const String Cancel;
};

LN_NAMESPACE_END
