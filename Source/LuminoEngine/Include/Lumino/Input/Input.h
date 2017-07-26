
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
	static void addBinding(const StringRef& buttonName, InputGesture* gesture);

	/**
		@brief		アクションマッピングを除外します。
	*/
	static void removeBinding(InputGesture* gesture);
	
	/**
		@brief		指定したボタンのアクションマッピングをすべて削除します。
	*/
	static void clearBindings(const StringRef& buttonName);

	/**
		@brief		全てのアクションマッピングを削除します。
	*/
	static void clearAllBindings();
	
	/**
		@brief		サポートしているすべての入力のうち、現在入力中であるものを InputGesture として返します。
		@details	この機能はキーコンフィグ用のヘルパーです。新しく押された瞬間を判定します。それ以外は nullptr を返します。
	*/
	static InputGesture* getAnyActiveTriggered();

private:
};

/**
	@brief	デフォルトでマッピングされる仮想ボタンの名前を表します。
*/
class InputButtons
{
public:

	/** 左方向を表す仮想ボタンの名前 */
	static const String Left;

	/** 右方向を表す仮想ボタンの名前 */
	static const String Right;

	/** 上方向を表す仮想ボタンの名前 */
	static const String Up;

	/** 下方向を表す仮想ボタンの名前 */
	static const String Down;

	/** 決定操作を表す仮想ボタンの名前 */
	static const String Submit;
	
	/** キャンセル操作を表す仮想ボタンの名前 */
	static const String Cancel;

	/** メニュー操作を表す仮想ボタンの名前 */
	static const String Menu;

	/** 補助操作を表す仮想ボタンの名前 */
	static const String Shift;

	/** 前ページの表示操作を表す仮想ボタンの名前 */
	static const String PageUp;

	/** 次ページの表示操作を表す仮想ボタンの名前 */
	static const String PageDown;

	/** マッピングされているいずれかの操作を表す仮想ボタンの名前 */
	static const String Any;

};

LN_NAMESPACE_END
