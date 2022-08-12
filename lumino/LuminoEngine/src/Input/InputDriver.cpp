
#include "Internal.hpp"
#include <LuminoPlatform/PlatformWindow.hpp>
#include "InputDriver.hpp"

namespace ln {

//==============================================================================
// InputDriver
//==============================================================================

//------------------------------------------------------------------------------
InputDriver::InputDriver()
	: m_keyStatus{}
	, m_mouseStatus{}
	, m_mouseClickTrackers{}
	, m_mouseButtonClickTimeout(0.3f)
	, m_mousePoint()
	, m_mouseWheel(0)
	, m_pressedAnyKey(makeObject_deprecated<KeyGesture>(Keys::Unknown, ModifierKeys::None))
	, m_pressedAnyMouseButton(makeObject_deprecated<MouseGesture>(MouseAction::None, ModifierKeys::None))
	, m_pressedAnyGamepadElement(makeObject_deprecated<GamepadGesture>(GamepadElement::None))
{
	for (int i = 0; i < m_mouseClickTrackers.size(); i++) {
		m_mouseClickTrackers[i].timer.start();
	}
}

//------------------------------------------------------------------------------
InputDriver::~InputDriver()
{
}

void InputDriver::dispose()
{
}

//------------------------------------------------------------------------------
bool InputDriver::queryKeyState(Keys key)
{
	return m_keyStatus[(int)key];
}

//------------------------------------------------------------------------------
bool InputDriver::queryMouseState(MouseAction action)
{
	switch (action)
	{
	case ln::MouseAction::LeftClick:
		return (m_mouseStatus[(int)MouseButtons::Left] && m_mouseClickTrackers[(int)MouseButtons::Left].clickCount == 1);
	case ln::MouseAction::LeftDoubleClick:
		return (m_mouseStatus[(int)MouseButtons::Left] && m_mouseClickTrackers[(int)MouseButtons::Left].clickCount == 2);
	case ln::MouseAction::RightClick:
		return (m_mouseStatus[(int)MouseButtons::Right] && m_mouseClickTrackers[(int)MouseButtons::Right].clickCount == 1);
	case ln::MouseAction::RightDoubleClick:
		return (m_mouseStatus[(int)MouseButtons::Right] && m_mouseClickTrackers[(int)MouseButtons::Right].clickCount == 2);
	case ln::MouseAction::MiddleClick:
		return (m_mouseStatus[(int)MouseButtons::Middle] && m_mouseClickTrackers[(int)MouseButtons::Middle].clickCount == 1);
	case ln::MouseAction::MiddleDoubleClick:
		return (m_mouseStatus[(int)MouseButtons::Middle] && m_mouseClickTrackers[(int)MouseButtons::Middle].clickCount == 2);
	case ln::MouseAction::X1Click:
		return (m_mouseStatus[(int)MouseButtons::X1] && m_mouseClickTrackers[(int)MouseButtons::X1].clickCount == 1);
	case ln::MouseAction::X1DoubleClick:
		return (m_mouseStatus[(int)MouseButtons::X1] && m_mouseClickTrackers[(int)MouseButtons::X1].clickCount == 2);
	case ln::MouseAction::X2Click:
		return (m_mouseStatus[(int)MouseButtons::X2] && m_mouseClickTrackers[(int)MouseButtons::X2].clickCount == 1);
	case ln::MouseAction::X2DoubleClick:
		return (m_mouseStatus[(int)MouseButtons::X2] && m_mouseClickTrackers[(int)MouseButtons::X2].clickCount == 2);
	case ln::MouseAction::WheelPlus:
		return (m_mouseWheel > 0);
	case ln::MouseAction::WheelMinus:
		return (m_mouseWheel < 0);
	default:
		break;
	}
	return false;
}

//------------------------------------------------------------------------------
void InputDriver::preUpdateFrame()
{
	//m_keyStatus.fill(false);
	//m_mouseStatus.fill(false);
	//m_mousePoint.x = 0;
	//m_mousePoint.y = 0;
	m_mouseWheel = 0;
}

int InputDriver::getJoystickCount()
{
	return 0;
}

void InputDriver::getJoystickState(int joystickNumber, JoystickDeviceState* state)
{
	state->reset();
}

//------------------------------------------------------------------------------
void InputDriver::onEvent(const PlatformEventArgs& e)
{
	switch (e.type)
	{
	case PlatformEventType::MouseDown:
		onMouseButtonDown(e.mouse.button);
		break;
	case PlatformEventType::MouseUp:
		onMouseButtonUp(e.mouse.button);
		break;
	case PlatformEventType::MouseMove:
	{
		PointI pt = e.sender->pointFromScreen(PointI(e.mouseMove.screenX, e.mouseMove.screenY));
		m_mousePoint.x = pt.x;
		m_mousePoint.y = pt.y;
		break;
	}
	case PlatformEventType::MouseWheel:
		m_mouseWheel = e.wheel.delta;
		break;
	case PlatformEventType::KeyDown:
		onKeyDown(e.key.keyCode);
		break;
	case PlatformEventType::KeyUp:
		onKeyUp(e.key.keyCode);
		break;
	default:
		break;
	}
}

//------------------------------------------------------------------------------
void InputDriver::onKeyDown(Keys key)
{
	m_keyStatus[(int)key] = true;

	// any key の処理
	if (m_pressedAnyKey->getKey() == Keys::Unknown)
	{
		m_pressedAnyKey->setKey(key);
	}
}

//------------------------------------------------------------------------------
void InputDriver::onKeyUp(Keys key)
{
	m_keyStatus[(int)key] = false;

	// any key の処理
	if (m_pressedAnyKey->getKey() == key)
	{
		m_pressedAnyKey->setKey(Keys::Unknown);
	}
}

//------------------------------------------------------------------------------
void InputDriver::onMouseButtonDown(MouseButtons button)
{
	m_mouseStatus[(int)button] = true;

	MouseClickTracker& tracker = m_mouseClickTrackers[(int)button];
	tracker.clickCount++;

	//double curTime = 0.001 * Environment::getTickCount();
	//float elapsed = (float)(curTime - tracker.lastTime);
	float elapsedSec = 0.001f * tracker.timer.elapsedMilliseconds();
	if (elapsedSec > m_mouseButtonClickTimeout ||
		tracker.clickCount > 3)
	{
		tracker.clickCount = 1;
	}
	tracker.timer.start();

	// any key の処理
	if (m_pressedAnyMouseButton->getMouseAction() == MouseAction::None)
	{
		m_pressedAnyMouseButton->setMouseAction(toMouseActionSimple(button));
	}
}

//------------------------------------------------------------------------------
void InputDriver::onMouseButtonUp(MouseButtons button)
{
	m_mouseStatus[(int)button] = false;

	// any key の処理
	if (m_pressedAnyMouseButton->getMouseAction() == toMouseActionSimple(button))
	{
		m_pressedAnyMouseButton->setMouseAction(MouseAction::None);
	}
}

void InputDriver::updatePressedAnyGamepadElement()
{
	int newPadIndex;
	GamepadElement newElement;
	if (getPressedAnyGamepadElementHelper(&newPadIndex, &newElement))
	{
		m_pressedAnyGamepadElement->setGamepadNumber(newPadIndex);
		m_pressedAnyGamepadElement->setElement(newElement);
	}
	else
	{
		m_pressedAnyGamepadElement->setElement(GamepadElement::None);
	}
}

KeyGesture* InputDriver::getPressedAnyKey() const
{
	if (m_pressedAnyKey->getKey() == Keys::Unknown) return nullptr;
	return m_pressedAnyKey;
}

MouseGesture* InputDriver::getPressedAnyMouseButton() const
{
	if (m_pressedAnyMouseButton->getMouseAction() == MouseAction::None) return nullptr;
	return m_pressedAnyMouseButton;
}

GamepadGesture* InputDriver::getPressedAnyGamepadElement() const
{
	if (m_pressedAnyGamepadElement->getElement() == GamepadElement::None) return nullptr;
	return m_pressedAnyGamepadElement;
}

MouseAction InputDriver::toMouseActionSimple(MouseButtons button)
{
	MouseAction mouseAction = MouseAction::None;
	switch (button)
	{
	case MouseButtons::None:
		break;
	case MouseButtons::Left:
		mouseAction = MouseAction::LeftClick;
		break;
	case MouseButtons::Right:
		mouseAction = MouseAction::RightClick;
		break;
	case MouseButtons::Middle:
		mouseAction = MouseAction::MiddleClick;
		break;
	case MouseButtons::X1:
		mouseAction = MouseAction::X1Click;
		break;
	case MouseButtons::X2:
		mouseAction = MouseAction::X2Click;
		break;
	default:
		break;
	}
	return mouseAction;
}

bool InputDriver::getPressedAnyGamepadElementHelper(int* outPadNumber, GamepadElement* outElement)
{
	int joyCount = getJoystickCount();
	for (int i = 0; i < joyCount; i++)
	{
		*outPadNumber = i;

		JoystickDeviceState state;
		getJoystickState(i, &state);

		// buttons
		for (int iButton = 0; iButton < JoystickDeviceState::MaxButtons; iButton++)
		{
			if (state.Buttons[iButton] != 0)
			{
				*outElement = (GamepadElement)(((int)GamepadElement::Button1) + iButton);
				return true;
			}
		}

		// pov
		if (state.POV & PovDirFlags_Left)
		{
			*outElement = GamepadElement::PovLeft;
			return true;
		}
		if (state.POV & PovDirFlags_Right)
		{
			*outElement = GamepadElement::PovRight;
			return true;
		}
		if (state.POV & PovDirFlags_Up)
		{
			*outElement = GamepadElement::PovUp;
			return true;
		}
		if (state.POV & PovDirFlags_Down)
		{
			*outElement = GamepadElement::PovDown;
			return true;
		}

		// axis
		for (int iAxis = 0; iAxis < JoystickDeviceState::MaxAxis; iAxis++)
		{
			if (abs(state.Axes[iAxis]) > 0.5f)	// あそび固定。この処理の目的はキーとして扱いたいような要素を検出することだから問題ないだろう
			{
				int index = iAxis * 2 + (int)GamepadElement::Axis1Minus;
				if (state.Axes[iAxis] > 0.0) index += 1;
				*outElement = (GamepadElement)index;
				return true;

			}
		}
	}
	return false;
}

} // namespace ln
