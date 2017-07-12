
#include "../Internal.h"
#include "InputDriver.h"

LN_NAMESPACE_BEGIN

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
{
}

//------------------------------------------------------------------------------
InputDriver::~InputDriver()
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
		m_mousePoint.x = e.mouse.x;
		m_mousePoint.y = e.mouse.y;
		break;
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
}

//------------------------------------------------------------------------------
void InputDriver::onKeyUp(Keys key)
{
	m_keyStatus[(int)key] = false;
}

//------------------------------------------------------------------------------
void InputDriver::onMouseButtonDown(MouseButtons button)
{
	m_mouseStatus[(int)button] = true;

	MouseClickTracker& tracker = m_mouseClickTrackers[(int)button];
	tracker.clickCount++;

	double curTime = 0.001 * Environment::getTickCount();
	float elapsed = (float)(curTime - tracker.lastTime);
	if (elapsed > m_mouseButtonClickTimeout ||
		tracker.clickCount > 3)
	{
		tracker.clickCount = 1;
	}
	tracker.lastTime = curTime;
}

//------------------------------------------------------------------------------
void InputDriver::onMouseButtonUp(MouseButtons button)
{
	m_mouseStatus[(int)button] = false;
}

LN_NAMESPACE_END
