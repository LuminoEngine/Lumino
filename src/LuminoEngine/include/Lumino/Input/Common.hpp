
#pragma once

namespace ln {
class InputController;

namespace detail {
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

class InputDeviceElement
	: public RefObject
{
public:
	String name;
	float	current;
	int		state;		// -1: pressed in current frame, 0: released, 1~:repeat
	float	repeatTime;
	int		ref;		// reference count (referenced BindingSlot)

	InputDeviceElement()
	{
		current = 0;
		state = 0;
		repeatTime = 0;
		ref = 0;
	}

	void updateFrame(float elapsedTime, float repeatIntervalStartTime, float repeatIntervalStepTime)
	{
		if (current > 0.0f) {
			state++;
		}
		else {
			if (state > 0) {
				state = -1;
			}
			else {
				state = 0;
			}
		}

		if (isPressed()) {
			if (repeatTime < repeatIntervalStartTime + repeatIntervalStepTime) {
				repeatTime += elapsedTime;
			}
			else {
				repeatTime = repeatIntervalStartTime;
			}
		}
		else {
			repeatTime = 0;
		}
	}

	bool isPressed() const { return state > 0; }
	bool isTriggered() const { return state == 1; }
	bool isOffTriggered() const { return state < 0; }
	bool isRepeated(float repeatIntervalStartTime, float repeatIntervalStepTime) const { return isTriggered() || repeatTime >= repeatIntervalStartTime + repeatIntervalStepTime; }
};

} // namespace detail
} // namespace ln 
