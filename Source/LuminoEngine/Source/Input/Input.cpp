
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
const String InputButtons::Left = _LT("left");
const String InputButtons::Right = _LT("right");
const String InputButtons::Up = _LT("up");
const String InputButtons::Down = _LT("down");
const String InputButtons::Submit = _LT("submit");
const String InputButtons::Cancel = _LT("cancel");
const String InputButtons::Menu = _LT("menu");
const String InputButtons::Shift = _LT("shift");
const String InputButtons::PageUp = _LT("pageup");
const String InputButtons::PageDown = _LT("pagedown");
const String InputButtons::Any = String::getEmpty();

LN_NAMESPACE_END
