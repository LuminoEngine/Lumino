
#include "Internal.h"
#include "InputManager.h"
#include <Lumino/Input/InputController.h>
#include <Lumino/Input/Input.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// Input
//==============================================================================

//------------------------------------------------------------------------------
bool Input::isPressed(const StringRef& buttonName)
{
	return detail::InputManager::getInstance()->getVirtualPad(0)->isPressed(buttonName);
}

//------------------------------------------------------------------------------
bool Input::isTriggered(const StringRef& buttonName)
{
	return detail::InputManager::getInstance()->getVirtualPad(0)->isTriggered(buttonName);
}

//------------------------------------------------------------------------------
bool Input::isOffTriggered(const StringRef& buttonName)
{
	return detail::InputManager::getInstance()->getVirtualPad(0)->isOffTriggered(buttonName);
}

//------------------------------------------------------------------------------
bool Input::isRepeated(const StringRef& buttonName)
{
	return detail::InputManager::getInstance()->getVirtualPad(0)->isRepeated(buttonName);
}

//------------------------------------------------------------------------------
float Input::getAxisValue(const StringRef& buttonName)
{
	return detail::InputManager::getInstance()->getVirtualPad(0)->getAxisValue(buttonName);
}

//------------------------------------------------------------------------------
void Input::addBinding(const StringRef& buttonName, InputGesture* gesture)
{
	return detail::InputManager::getInstance()->getVirtualPad(0)->addBinding(buttonName, gesture);
}

//------------------------------------------------------------------------------
void Input::removeBinding(InputGesture* gesture)
{
	return detail::InputManager::getInstance()->getVirtualPad(0)->removeBinding(gesture);
}

//------------------------------------------------------------------------------
void Input::clearBindings(const StringRef& buttonName)
{
	return detail::InputManager::getInstance()->getVirtualPad(0)->clearBindings(buttonName);
}

//------------------------------------------------------------------------------
void Input::clearAllBindings()
{
	return detail::InputManager::getInstance()->getVirtualPad(0)->clearAllBindings();
}

//------------------------------------------------------------------------------
InputGesture* Input::getAnyActiveTriggered()
{
	return detail::InputManager::getInstance()->getAnyActiveTriggered();
}

//==============================================================================
// InputButton
//==============================================================================
const String InputButtons::Left = _T("left");
const String InputButtons::Right = _T("right");
const String InputButtons::Up = _T("up");
const String InputButtons::Down = _T("down");
const String InputButtons::Submit = _T("submit");
const String InputButtons::Cancel = _T("cancel");
const String InputButtons::Menu = _T("menu");
const String InputButtons::Shift = _T("shift");
const String InputButtons::PageUp = _T("pageup");
const String InputButtons::PageDown = _T("pagedown");
const String InputButtons::Any = String::getEmpty();

LN_NAMESPACE_END
