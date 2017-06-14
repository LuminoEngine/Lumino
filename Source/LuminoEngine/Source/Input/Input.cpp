
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
void Input::addButtonBinding(const StringRef& buttonName, InputBinding* binding)
{
	return detail::InputManager::getInstance()->getVirtualPad(0)->addBinding(buttonName, binding);
}

//------------------------------------------------------------------------------
void Input::removeBinding(InputBinding* binding)
{
	return detail::InputManager::getInstance()->getVirtualPad(0)->removeBinding(binding);
}

//------------------------------------------------------------------------------
void Input::clearBindings()
{
	return detail::InputManager::getInstance()->getVirtualPad(0)->clearBindings();
}


//==============================================================================
// InputButton
//==============================================================================
const String InputButtons::Any = String::getEmpty();
const String InputButtons::Left = _T("left");
const String InputButtons::Right = _T("right");
const String InputButtons::Up = _T("up");
const String InputButtons::Down = _T("down");
const String InputButtons::OK = _T("ok");
const String InputButtons::Cancel = _T("cancel");

LN_NAMESPACE_END
