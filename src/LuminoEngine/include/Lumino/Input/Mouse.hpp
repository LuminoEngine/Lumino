
#pragma once
#include <array>
#include <Lumino/Platform/PlatformEvent.hpp>
#include <Lumino/Graphics/GeometryStructs.hpp>

namespace ln {


class Mouse
{
public:
	static bool isPressed(MouseButtons button);
	static bool isTriggered(MouseButtons button);
	static bool isOffTriggered(MouseButtons button);
	static bool isRepeated(MouseButtons button);
	static Point position();

private:
};

namespace detail {
class InputManager;

struct InputDeviceElement
{
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

class MouseInputDevice
	: public RefObject
{
public:
	MouseInputDevice();

	bool isPressed(MouseButtons button) const;
	bool isTriggered(MouseButtons button) const;
	bool isOffTriggered(MouseButtons button) const;
	bool isRepeated(MouseButtons button) const;
	Point position();

	void updateFrame(float elapsedTime);

private:
	static int MouseButtonToIndex(MouseButtons button);

	detail::InputManager* m_manager;
	std::array<InputDeviceElement, 5> m_elements;	// MouseButtons
};

} // namespace detail
} // namespace ln
