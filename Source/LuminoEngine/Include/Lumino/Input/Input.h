
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief		ゲームパッドやキーボードなどの入力システムのインターフェイス
*/
class Input
{
public:

	/**
		@brief		ボタンが現在押されているかを判定します。
	*/
	static bool isPressed(const StringRef& buttonName);

	/**
		@brief		ボタンが新しく押された瞬間を判定します。
	*/
	static bool isTriggered(const StringRef& buttonName);

	/**
		@brief		ボタンが離された瞬間を判定します。
	*/
	static bool isOffTriggered(const StringRef& buttonName);

	/**
		@brief		ボタンが新しく押された瞬間とリピート状態を判定します。
	*/
	static bool isRepeated(const StringRef& buttonName);

	/**
		@brief		指定した軸のアナログ値を取得します。
	*/
	static float getAxisValue(const StringRef& buttonName);

	/**
		@brief		ボタンのアクションマッピングを追加します。
	*/
	static void addButtonBinding(const StringRef& buttonName, InputBinding* binding);

	/**
		@brief		アクションマッピングを除外します。
	*/
	static void removeBinding(InputBinding* binding);

	/**
		@brief		全てのアクションマッピングを削除します。
	*/
	static void clearBindings();

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
	static const String OK;
	static const String Cancel;
};

LN_NAMESPACE_END
