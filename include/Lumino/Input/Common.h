
#pragma once

LN_NAMESPACE_BEGIN

///// マウスのボタン番号
//enum MouseButton
//{
//	MouseButton_1 = 0,
//	MouseButton_2,
//	MouseButton_3,
//	MouseButton_4,
//	MouseButton_5,
//	MouseButton_6,
//	MouseButton_7,
//	MouseButton_8,
//
//	MouseButton_Max,
//};

/// 入力デバイスを識別する値
enum InputDeviceID
{
	InputDeviceID_Unknown = 0,
	InputDeviceID_Keyboard,
	InputDeviceID_Mouse,
	InputDeviceID_Joypad1,
	InputDeviceID_Joypad2,
	InputDeviceID_Joypad3,
	InputDeviceID_Joypad4,
};

/// 各仮想ボタンを示す値
enum VirtualButton
{
	VirtualButton_Unknown = 0,

	VirtualButton_Mouse1 = 256,		// GLFW と合わせて 1 スタート
	VirtualButton_Mouse2,
	VirtualButton_Mouse3,
	VirtualButton_Mouse4,
	VirtualButton_Mouse5,
	VirtualButton_Mouse6,
	VirtualButton_Mouse7,
	VirtualButton_Mouse8,

	VirtualButton_JoypadButton1,
	VirtualButton_JoypadButton2,
	VirtualButton_JoypadButton3,
	VirtualButton_JoypadButton4,
	VirtualButton_JoypadButton5,
	VirtualButton_JoypadButton6,
	VirtualButton_JoypadButton7,
	VirtualButton_JoypadButton8,
	VirtualButton_JoypadButton9,
	VirtualButton_JoypadButton10,
	VirtualButton_JoypadButton11,
	VirtualButton_JoypadButton12,
	VirtualButton_JoypadButton13,
	VirtualButton_JoypadButton14,
	VirtualButton_JoypadButton15,
	VirtualButton_JoypadButton16,

	VirtualButton_JoypadPOVUp,
	VirtualButton_JoypadPOVRight,
	VirtualButton_JoypadPOVDown,
	VirtualButton_JoypadPOVLeft,

	VirtualButton_JoypadAxis1Minus,		///< X1 -1.0
	VirtualButton_JoypadAxis1Plus,		///< X1  1.0
	VirtualButton_JoypadAxis2Minus,		///< Y1 -1.0
	VirtualButton_JoypadAxis2Plus,		///< Y1  1.0
	VirtualButton_JoypadAxis3Minus,		///< X2 -1.0
	VirtualButton_JoypadAxis3Plus,		///< X2  1.0
	VirtualButton_JoypadAxis4Minus,		///< Y2 -1.0
	VirtualButton_JoypadAxis4Plus,		///< Y2  1.0
	VirtualButton_JoypadAxis5Minus,		///< X3 -1.0
	VirtualButton_JoypadAxis5Plus,		///< X3  1.0     XInput LT
	VirtualButton_JoypadAxis6Minus,		///< Y3 -1.0
	VirtualButton_JoypadAxis6Plus,		///< Y3  1.0     XInput RT
	VirtualButton_JoypadAxis7Minus,		///< X4 -1.0
	VirtualButton_JoypadAxis7Plus,		///< X4  1.0
	VirtualButton_JoypadAxis8Minus,		///< Y4 -1.0
	VirtualButton_JoypadAxis8Plus,		///< Y4  1.0

	VirtualButton_IDLast,

	VirtualButton_Keyboard_Begin		= 1,        // 1～255 はとりあえずキーボード用
	VirtualButton_Keyboard_End			= 255,
	VirtualButton_Mouse_Begin			= VirtualButton_Mouse1,
	VirtualButton_Mouse_End				= VirtualButton_Mouse8,
	VirtualButton_JoypadButtonBegin		= VirtualButton_JoypadButton1,
	VirtualButton_JoypadButtonEnd		= VirtualButton_JoypadButton16,
	VirtualButton_JoypadPOV_Begin		= VirtualButton_JoypadPOVUp,
	VirtualButton_JoypadPOV_End			= VirtualButton_JoypadPOVLeft,
	VirtualButton_JoypadAxisBegin		= VirtualButton_JoypadAxis1Minus,
	VirtualButton_JoypadAxisEnd			= VirtualButton_JoypadAxis8Plus,
};

LN_NAMESPACE_END
