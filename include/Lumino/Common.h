/**
	@file	Common.h
*/
#pragma once
#include <Lumino/Base/Common.h>

LN_NAMESPACE_BEGIN

class EngineManager;

/** マウスボタン */
LN_ENUM(MouseButton)
{
	None = 0,		/**< 無効または押されていないことを示す */
	Left,			/**< 左ボタン */
	Right,			/**< 右ボタン */
	Middle,			/**< 中央ボタン */

	X1,				/**< 拡張ボタン1 */
	X2,				/**< 拡張ボタン2 */

	TERMINATOR,
};
LN_ENUM_DECLARE(MouseButton);

/** キーコード (NumPad はサポートしていない) */
enum class Key
{
	Unknown = 0,

	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,

	D0,
	D1,
	D2,
	D3,
	D4,
	D5,
	D6,
	D7,
	D8,
	D9,

	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,

	Space,	// 0x20 = ' ' (スペース)
	Escape,
	Up,
	Down,
	Left,
	Right,
	LShift,
	RShift,
	LCtrl,
	RCtrl,
	LAlt,
	RAlt,
	Tab,
	Enter,
	BackSpace,
	Insert,
	Delete,
	PageUp,
	PageDown,
	Home,
	End,

	Colon,		///< :	[非推奨 GLFWでは;]
	SemiColon,	///< ;	[非推奨 GLFWでは=]
	Comma,		///< ,
	Period,		///< .
	Slash,		///< /
	Minus,		///< -
	BackSlash,	///< \(バックスラッシュ) [非推奨 GLFWではNOWORD2]
	Yen,		///< \(BaskSpaceキーの左の\キー) [非推奨 GLFWではI]
	Caret,		///< ^ [非推奨 GLFWではGLFW_KEY_APOSTROPHE(')]
	LBracket,	///< [
	RBracket,	///< ]

	AndroidPlus,		///< Android '+'

	TERMINATOR,
};

/** 修飾キー */
LN_ENUM_FLAGS(ModifierKeys)
{
	None	= 0x0000,
	Alt		= 0x0001,	/**< Alt キー */
	Shift	= 0x0002,	/**< Shift キー */
	Control	= 0x0004,	/**< Ctrl キー */
};
LN_ENUM_FLAGS_DECLARE(ModifierKeys);

LN_NAMESPACE_END
