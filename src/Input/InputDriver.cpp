
#include "../Internal.h"
#include "InputDriver.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// InputDriver
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputDriver::InputDriver()
	: m_keyStatus{}
	, m_mouseStatus{}
	, m_mousePoint()
	, m_mouseWheel(0)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputDriver::~InputDriver()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputDriver::UpdateFrame()
{
	m_keyStatus.fill(false);
	m_mouseStatus.fill(false);
	m_mousePoint.x = 0;
	m_mousePoint.y = 0;
	m_mouseWheel = 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputDriver::OnEvent(const PlatformEventArgs& e)
{
	switch (e.type)
	{
	case PlatformEventType::MouseDown:
		m_mouseStatus[e.mouse.button] = true;
		break;
	case PlatformEventType::MouseUp:
		m_mouseStatus[e.mouse.button] = false;
		break;
	case PlatformEventType::MouseMove:
		m_mousePoint.x = e.mouse.x;
		m_mousePoint.y = e.mouse.y;
		break;
	case PlatformEventType::MouseWheel:
		m_mouseWheel = e.wheel.delta;
		break;
	case PlatformEventType::KeyDown:
		m_keyStatus[(int)e.key.keyCode] = true;
		break;
	case PlatformEventType::KeyUp:
		m_keyStatus[(int)e.key.keyCode] = false;
		break;
	default:
		break;
	}
}

LN_NAMESPACE_END
