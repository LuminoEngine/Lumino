
#pragma once
#include <Lumino/Reflection/ReflectionObject.h>
#include "../Common.h"

LN_NAMESPACE_BEGIN
class InputController;

namespace detail
{
class InputManager;

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

enum class InputBindingType
{
	Keyboard,
	Mouse,
	Gamepad,
};

} // namespace detail
LN_NAMESPACE_END
