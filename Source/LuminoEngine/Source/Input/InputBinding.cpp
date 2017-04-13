
#include "../Internal.h"
#include <Lumino/Input/InputBinding.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// InputGesture
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(InputGesture, tr::ReflectionObject);

//------------------------------------------------------------------------------
InputGesture::InputGesture()
	: m_minValidMThreshold(0.2f)
{
}

//------------------------------------------------------------------------------
InputGesture::~InputGesture()
{
}

//==============================================================================
// KeyboardGesture
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(KeyboardGesture, InputGesture);

//------------------------------------------------------------------------------
KeyboardGesturePtr KeyboardGesture::Create(Keys key, ModifierKeys modifierKeys)
{
	auto ptr = KeyboardGesturePtr::MakeRef(key, modifierKeys);
	return ptr;
}

//------------------------------------------------------------------------------
KeyboardGesture::KeyboardGesture(Keys key, ModifierKeys modifierKeys)
	: m_key(key)
	, m_modifierKeys(modifierKeys)
{
}

//------------------------------------------------------------------------------
KeyboardGesture::~KeyboardGesture()
{
}

//------------------------------------------------------------------------------
detail::InputGestureType KeyboardGesture::GetType() const
{
	return detail::InputGestureType::Keyboard;
}


//==============================================================================
// MouseGesture
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(MouseGesture, InputGesture);

//------------------------------------------------------------------------------
MouseGesturePtr MouseGesture::Create(MouseAction mouseAction, ModifierKeys modifierKeys)
{
	auto ptr = MouseGesturePtr::MakeRef(mouseAction, modifierKeys);
	return ptr;
}

//------------------------------------------------------------------------------
MouseGesture::MouseGesture(MouseAction mouseAction, ModifierKeys modifierKeys)
	: m_mouseAction(mouseAction)
	, m_modifierKeys(modifierKeys)
{
}

//------------------------------------------------------------------------------
MouseGesture::~MouseGesture()
{
}

//------------------------------------------------------------------------------
detail::InputGestureType MouseGesture::GetType() const
{
	return detail::InputGestureType::Mouse;
}


//==============================================================================
// GamepadGesture
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(GamepadGesture, InputGesture);

//------------------------------------------------------------------------------
GamepadGesturePtr GamepadGesture::Create(GamepadElement element)
{
	auto ptr = GamepadGesturePtr::MakeRef(element);
	return ptr;
}

//------------------------------------------------------------------------------
GamepadGesture::GamepadGesture(GamepadElement element)
	: m_element(element)
{
}

//------------------------------------------------------------------------------
GamepadGesture::~GamepadGesture()
{
}

//------------------------------------------------------------------------------
detail::InputGestureType GamepadGesture::GetType() const
{
	return detail::InputGestureType::Gamepad;
}

LN_NAMESPACE_END
