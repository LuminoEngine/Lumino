
#include "Internal.h"
#include "InputManager.h"
#include <Lumino/Input/InputController.h>
#include <Lumino/Input/Input.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// Input
//==============================================================================

//------------------------------------------------------------------------------
bool Input::IsPressed(const StringRef& buttonName)
{
	return detail::InputManager::getInstance()->GetVirtualPad(0)->IsPressed(buttonName);
}

//------------------------------------------------------------------------------
bool Input::IsTriggered(const StringRef& buttonName)
{
	return detail::InputManager::getInstance()->GetVirtualPad(0)->IsTriggered(buttonName);
}

//------------------------------------------------------------------------------
bool Input::IsOffTriggered(const StringRef& buttonName)
{
	return detail::InputManager::getInstance()->GetVirtualPad(0)->IsOffTriggered(buttonName);
}

//------------------------------------------------------------------------------
bool Input::IsRepeated(const StringRef& buttonName)
{
	return detail::InputManager::getInstance()->GetVirtualPad(0)->IsRepeated(buttonName);
}

//------------------------------------------------------------------------------
float Input::GetAxisValue(const StringRef& buttonName)
{
	return detail::InputManager::getInstance()->GetVirtualPad(0)->GetAxisValue(buttonName);
}

//------------------------------------------------------------------------------
void Input::AddButtonBinding(const StringRef& buttonName, InputBinding* binding)
{
	return detail::InputManager::getInstance()->GetVirtualPad(0)->AddBinding(buttonName, binding);
}

//------------------------------------------------------------------------------
void Input::RemoveBinding(InputBinding* binding)
{
	return detail::InputManager::getInstance()->GetVirtualPad(0)->RemoveBinding(binding);
}

//------------------------------------------------------------------------------
void Input::ClearBindings()
{
	return detail::InputManager::getInstance()->GetVirtualPad(0)->ClearBindings();
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
