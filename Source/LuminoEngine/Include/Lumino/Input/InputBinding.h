
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
class InputBinding;
class KeyboardBinding;
class MouseBinding;
class GamepadBinding;
using InputBindingPtr = RefPtr<InputBinding>;
using KeyboardBindingPtr = RefPtr<KeyboardBinding>;
using MouseBindingPtr = RefPtr<MouseBinding>;
using GamepadBindingPtr = RefPtr<GamepadBinding>;

/** マウスによって実行されるアクション */
enum class MouseAction
{
	LeftClick,				/**< 左ボタンのクリック */
	LeftDoubleClick,		/**< 左ボタンのダブルクリック */
	RightClick,				/**< 右ボタンのクリック */
	RightDoubleClick,		/**< 右ボタンのダブルクリック */
	MiddleClick,			/**< 中央ボタンのクリック */
	MiddleDoubleClick,		/**< 中央ボタンのダブルクリック */
	X1Click,				/**< 拡張ボタン1のクリック */
	X1DoubleClick,			/**< 拡張ボタン1のダブルクリック */
	X2Click,				/**< 拡張ボタン2のクリック */
	X2DoubleClick,			/**< 拡張ボタン2のダブルクリック */
	WheelPlus,				/**< プラス方向へのホイール回転 */
	WheelMinus,				/**< マイナス方向へのホイール回転 */
};

/** ゲームパッドの入力要素 */
enum class GamepadElement
{
	Button1,
	Button2,
	Button3,
	Button4,
	Button5,
	Button6,
	Button7,
	Button8,
	Button9,
	Button10,
	Button11,
	Button12,
	Button13,
	Button14,
	Button15,
	Button16,
	//
	PovLeft,
	PovRight,
	PovUp,
	PovDown,
	//
	Axis1,			/**< -1.0 .. 1.0 */
	Axis2,			/**< -1.0 .. 1.0 */
	Axis3,			/**< -1.0 .. 1.0 */
	Axis4,			/**< -1.0 .. 1.0 */
	Axis5,			/**< -1.0 .. 1.0 */
	Axis6,			/**< -1.0 .. 1.0 */
	Axis7,			/**< -1.0 .. 1.0 */
	Axis8,			/**< -1.0 .. 1.0 */
	//
	Axis1Minus,		/**< X1 -1.0 */
	Axis1Plus,		/**< X1  1.0 */
	Axis2Minus,		/**< Y1 -1.0 */
	Axis2Plus,		/**< Y1  1.0 */
	Axis3Minus,		/**< X2 -1.0 */
	Axis3Plus,		/**< X2  1.0 */
	Axis4Minus,		/**< Y2 -1.0 */
	Axis4Plus,		/**< Y2  1.0 */
	Axis5Minus,		/**< X3 -1.0 */
	Axis5Plus,		/**< X3  1.0 (XInput LT) */
	Axis6Minus,		/**< Y3 -1.0 */
	Axis6Plus,		/**< Y3  1.0 (XInput RT) */
	Axis7Minus,		/**< X4 -1.0 */
	Axis7Plus,		/**< X4  1.0 */
	Axis8Minus,		/**< Y4 -1.0 */
	Axis8Plus,		/**< Y4  1.0 */
};

/**
	@brief	ユーザー入力となる入力デバイス操作を定義するためのベースクラスです。
*/
class InputBinding
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	float GetMinValidMThreshold() const { return m_minValidMThreshold; }
	float getScale() const { return 1.0f; }	// TODO

LN_PROTECTED_INTERNAL_ACCESS:
	InputBinding();
	virtual ~InputBinding();
	virtual detail::InputBindingType getType() const = 0;

private:
	float						m_minValidMThreshold;
};

/**
	@brief	ユーザー入力となるキーボード操作の組み合わせを定義します。
*/
class KeyboardBinding
	: public InputBinding
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/**
		@brief		MouseBinding オブジェクトを作成します。
		@param[in]	key				: 関連付けられるキー
		@param[in]	modifierKeys	: 関連付けられる修飾キー
	*/
	static KeyboardBindingPtr create(Keys key, ModifierKeys modifierKeys = ModifierKeys::None);

public:

	/** 関連付けられるキー */
	Keys getKey() const { return m_key; }

	/** 関連付けられる修飾キー */
	ModifierKeys getModifierKeys() const { return m_modifierKeys; }

LN_PROTECTED_INTERNAL_ACCESS:
	KeyboardBinding(Keys key, ModifierKeys modifierKeys);
	virtual ~KeyboardBinding();
	virtual detail::InputBindingType getType() const override;

private:
	Keys			m_key;
	ModifierKeys	m_modifierKeys;
};

/**
	@brief	ユーザー入力となるマウス操作の組み合わせを定義します。
*/
class MouseBinding
	: public InputBinding
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/**
		@brief		MouseBinding オブジェクトを作成します。
		@param[in]	mouseAction		: 関連付けられるマウス操作
		@param[in]	modifierKeys	: 関連付けられる修飾キー
	*/
	static MouseBindingPtr create(MouseAction mouseAction, ModifierKeys modifierKeys = ModifierKeys::None);

public:

	/** 関連付けられるマウス操作 */
	MouseAction getMouseAction() const { return m_mouseAction; }

	/** 関連付けられる修飾キー */
	ModifierKeys getModifierKeys() const { return m_modifierKeys; }

LN_PROTECTED_INTERNAL_ACCESS:
	MouseBinding(MouseAction mouseAction, ModifierKeys modifierKeys);
	virtual ~MouseBinding();
	virtual detail::InputBindingType getType() const override;

private:
	MouseAction		m_mouseAction;
	ModifierKeys	m_modifierKeys;
};

/**
	@brief	ユーザー入力となるゲームパッド操作の組み合わせを定義します。
*/
class GamepadBinding
	: public InputBinding
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/**
		@brief		GamepadBinding オブジェクトを作成します。
		@param[in]	element		: 関連付けられるゲームパッド操作
	*/
	static GamepadBindingPtr create(GamepadElement element);

public:

	/** 関連付けられるゲームパッド操作 */
	GamepadElement getElement() const { return m_element; }

LN_PROTECTED_INTERNAL_ACCESS:
	GamepadBinding(GamepadElement element);
	virtual ~GamepadBinding();
	virtual detail::InputBindingType getType() const override;

private:
	GamepadElement		m_element;
};

LN_NAMESPACE_END
