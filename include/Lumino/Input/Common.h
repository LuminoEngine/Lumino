
#pragma once
#include <Lumino/Base/EnumExtension.h>
#include <Lumino/Reflection/ReflectionObject.h>
#include "../Common.h"

LN_NAMESPACE_BEGIN
class InputDriver;
class InputController;

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

namespace detail
{
class InputManager;
//
//
///** 入力デバイスから受け取る値の種別 */
//LN_ENUM(DeviceInputSource)
//{
//	Unknown = 0,
//
//	/* 255 以前はキーボード */
//
//	Mouse1 = 256,
//	Mouse2,
//	Mouse3,
//	Mouse4,
//	Mouse5,
//	Mouse6,
//	Mouse7,
//	Mouse8,
//
//	JoypadButton1,
//	JoypadButton2,
//	JoypadButton3,
//	JoypadButton4,
//	JoypadButton5,
//	JoypadButton6,
//	JoypadButton7,
//	JoypadButton8,
//	JoypadButton9,
//	JoypadButton10,
//	JoypadButton11,
//	JoypadButton12,
//	JoypadButton13,
//	JoypadButton14,
//	JoypadButton15,
//	JoypadButton16,
//
//	JoypadPOVUp,
//	JoypadPOVRight,
//	JoypadPOVDown,
//	JoypadPOVLeft,
//
//	JoypadAxis1Minus,		///< X1 -1.0
//	JoypadAxis1Plus,		///< X1  1.0
//	JoypadAxis2Minus,		///< Y1 -1.0
//	JoypadAxis2Plus,		///< Y1  1.0
//	JoypadAxis3Minus,		///< X2 -1.0
//	JoypadAxis3Plus,		///< X2  1.0
//	JoypadAxis4Minus,		///< Y2 -1.0
//	JoypadAxis4Plus,		///< Y2  1.0
//	JoypadAxis5Minus,		///< X3 -1.0
//	JoypadAxis5Plus,		///< X3  1.0     XInput LT
//	JoypadAxis6Minus,		///< Y3 -1.0
//	JoypadAxis6Plus,		///< Y3  1.0     XInput RT
//	JoypadAxis7Minus,		///< X4 -1.0
//	JoypadAxis7Plus,		///< X4  1.0
//	JoypadAxis8Minus,		///< Y4 -1.0
//	JoypadAxis8Plus,		///< Y4  1.0
//
//	IDLast,
//
//	Keyboard_Begin = 1,        // 1～255 はとりあえずキーボード用
//	Keyboard_End = 255,
//	Mouse_Begin = Mouse1,
//	Mouse_End = Mouse8,
//	JoypadButtonBegin = JoypadButton1,
//	JoypadButtonEnd = JoypadButton16,
//	JoypadPOV_Begin = JoypadPOVUp,
//	JoypadPOV_End = JoypadPOVLeft,
//	JoypadAxisBegin = JoypadAxis1Minus,
//	JoypadAxisEnd = JoypadAxis8Plus,
//};
//LN_ENUM_DECLARE(DeviceInputSource);

struct DeviceInputSource
{
	uint32_t	id;

	static const uint32_t KeyboardFlag			= 0x00010000;
	static const uint32_t MouseFlag				= 0x00020000;
	static const uint32_t JoystickButtonFlag	= 0x00040000;
	static const uint32_t JoystickAxisFlag		= 0x00080000;
	static const uint32_t JoystickPovFlag		= 0x00100000;
	static const uint32_t ValumeMask			= 0x0000FFFF;
	static const uint32_t JoystickNumberMask	= 0x0000F000;
	static const uint32_t JoystickValumeMask	= 0x00000FFF;
	// Motion
	// Gesture
	// Android
};

} // namespace detail
LN_NAMESPACE_END
