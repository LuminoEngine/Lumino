
#pragma once
#include <Lumino/Base/EnumExtension.h>
#include <Lumino/Reflection/ReflectionObject.h>
#include "../Common.h"

LN_NAMESPACE_BEGIN
class InputDriver;
class InputController;
class InputBinding;
typedef RefPtr<InputBinding>	InputBindingPtr;

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


enum class JoystickPovAxis
{
	X,
	Y,
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

enum class DeviceInputSourceType
{
	Keyboard,
	Mouse,
	JoystickButton,
	JoystickAxis,
	JoystickPov,
	// Motion,
	// Gesture,
	// Android,
};

struct DeviceInputSource
{
	DeviceInputSourceType	type;

	union
	{
		struct
		{
			Key				key;
			ModifierKeys::enum_type	modifierKeys;

		} Keyboard;

		struct
		{
			MouseButton::enum_type		buttonNumber;

		} Mouse;

		struct
		{
			int				buttonNumber;

		} JoystickButton;

		struct
		{
			int				axizNumber;

		} JoystickAxis;

		struct
		{
			JoystickPovAxis		povAxis;

		} JoystickPov;
	};
};

enum InputDeviceID
{
	InputDeviceID_None = 0x0000,
	InputDeviceID_Keyboard = 0x0001,
	InputDeviceID_Mouse = 0x0002,
	InputDeviceID_Joystick0 = 0x0004,
	InputDeviceID_Joystick1 = 0x0008,
	InputDeviceID_Joystick2 = 0x0010,
	InputDeviceID_Joystick3 = 0x0020,
};

} // namespace detail
LN_NAMESPACE_END
