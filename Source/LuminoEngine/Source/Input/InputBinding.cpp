
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
// KeyGesture
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(KeyGesture, InputGesture);

static const String& getKeyName(Keys key)
{
	struct NamePair
	{
		Keys	key;	// for verify
		String	name;
	};
	static const NamePair table[] =
	{
		{ Keys::Unknown,	_T("Unknown") },
		{ Keys::A,			_T("A") },
		{ Keys::B,			_T("B") },
		{ Keys::C,			_T("C") },
		{ Keys::D,			_T("D") },
		{ Keys::E,			_T("E") },
		{ Keys::F,			_T("F") },
		{ Keys::G,			_T("G") },
		{ Keys::H,			_T("H") },
		{ Keys::I,			_T("I") },
		{ Keys::J,			_T("J") },
		{ Keys::K,			_T("K") },
		{ Keys::L,			_T("L") },
		{ Keys::M,			_T("M") },
		{ Keys::N,			_T("N") },
		{ Keys::O,			_T("O") },
		{ Keys::P,			_T("P") },
		{ Keys::Q,			_T("Q") },
		{ Keys::R,			_T("R") },
		{ Keys::S,			_T("S") },
		{ Keys::T,			_T("T") },
		{ Keys::U,			_T("U") },
		{ Keys::V,			_T("V") },
		{ Keys::W,			_T("W") },
		{ Keys::X,			_T("X") },
		{ Keys::Y,			_T("Y") },
		{ Keys::Z,			_T("Z") },

		{ Keys::D0,			_T("0") },
		{ Keys::D1,			_T("1") },
		{ Keys::D2,			_T("2") },
		{ Keys::D3,			_T("3") },
		{ Keys::D4,			_T("4") },
		{ Keys::D5,			_T("5") },
		{ Keys::D6,			_T("6") },
		{ Keys::D7,			_T("7") },
		{ Keys::D8,			_T("8") },
		{ Keys::D9,			_T("9") },

		{ Keys::F1,			_T("F1") },
		{ Keys::F2,			_T("F2") },
		{ Keys::F3,			_T("F3") },
		{ Keys::F4,			_T("F4") },
		{ Keys::F5,			_T("F5") },
		{ Keys::F6,			_T("F6") },
		{ Keys::F7,			_T("F7") },
		{ Keys::F8,			_T("F8") },
		{ Keys::F9,			_T("F9") },
		{ Keys::F10,		_T("F10") },
		{ Keys::F11,		_T("F11") },
		{ Keys::F12,		_T("F12") },

		{ Keys::Space,		_T("Space") },
		{ Keys::Escape,		_T("Esc") },
		{ Keys::Up,			_T("Up") },
		{ Keys::Down,		_T("Down") },
		{ Keys::Left,		_T("Left") },
		{ Keys::Right,		_T("Right") },
		{ Keys::LShift,		_T("LeftShift") },
		{ Keys::RShift,		_T("RightShift") },
		{ Keys::LCtrl,		_T("LeftCtrl") },
		{ Keys::RCtrl,		_T("RightCtrl") },
		{ Keys::LAlt,		_T("LeftAlt") },
		{ Keys::RAlt,		_T("RightAlt") },
		{ Keys::Tab,		_T("Tab") },
		{ Keys::Enter,		_T("Enter") },
		{ Keys::BackSpace,	_T("BackSpace") },
		{ Keys::Insert,		_T("Insert") },
		{ Keys::Delete,		_T("Delete") },
		{ Keys::PageUp,		_T("PageUp") },
		{ Keys::PageDown,	_T("PageDown") },
		{ Keys::Home,		_T("Home") },
		{ Keys::End,		_T("End") },

		{ Keys::Colon,		_T(":") },
		{ Keys::Semicolon,	_T(";") },
		{ Keys::Comma,		_T(",") },
		{ Keys::Period,		_T(".") },
		{ Keys::Slash,		_T("/") },
		{ Keys::Minus,		_T("-") },
		{ Keys::Backslash,	_T("\\") },
		{ Keys::Yen,		_T("\\") },
		{ Keys::Caret,		_T("'") },
		{ Keys::LBracket,	_T("[") },
		{ Keys::RBracket,	_T("]") },

		{ Keys::AndroidPlus,	_T("+") },
	};

	int i = (int)key;
	LN_ASSERT(0 <= i && i < LN_ARRAY_SIZE_OF(table));
	LN_ASSERT(key == table[i].key);
	return table[i].name;
}

static String getModifierKeysName(ModifierKeys modifierKeyss)
{
	String name;
	if (modifierKeyss.TestFlag(ModifierKeys::Control))
	{
		name = _T("Ctrl");
	}
	if (modifierKeyss.TestFlag(ModifierKeys::Shift))
	{
		if (!name.isEmpty()) name += _T("+");
		name = _T("Shift");
	}
	if (modifierKeyss.TestFlag(ModifierKeys::Alt))
	{
		if (!name.isEmpty()) name += _T("+");
		name = _T("Alt");
	}
	return name;
}

//------------------------------------------------------------------------------
KeyboardBindingPtr KeyGesture::create(Keys key, ModifierKeys modifierKeys)
{
	auto ptr = KeyboardBindingPtr::makeRef(key, modifierKeys);
	return ptr;
}

//------------------------------------------------------------------------------
KeyGesture::KeyGesture(Keys key, ModifierKeys modifierKeys)
	: m_key(key)
	, m_modifierKeys(modifierKeys)
{
}

//------------------------------------------------------------------------------
KeyGesture::~KeyGesture()
{
}

//------------------------------------------------------------------------------
String KeyGesture::getDisplayName() const
{
	String name;
	if (m_modifierKeys != ModifierKeys::None)
	{
		name = getModifierKeysName(m_modifierKeys) + _T("+");
	}
	name += getKeyName(m_key);
	return name;
}

//------------------------------------------------------------------------------
detail::InputBindingType KeyGesture::getType() const
{
	return detail::InputBindingType::Keyboard;
}


//==============================================================================
// MouseGesture
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(MouseGesture, InputGesture);

static const String& getMouseActionName(MouseAction mouseAction)
{
	struct NamePair
	{
		MouseAction	mouseAction;	// for verify
		String		name;
	};
	static const NamePair table[] =
	{
		{ MouseAction::LeftClick,			_T("Mouse Left") },
		{ MouseAction::LeftDoubleClick,		_T("Mouse Left (Double)") },
		{ MouseAction::RightClick,			_T("Mouse Right") },
		{ MouseAction::RightDoubleClick,	_T("Mouse Right (Double)") },
		{ MouseAction::MiddleClick,			_T("Mouse Middle") },
		{ MouseAction::MiddleDoubleClick,	_T("Mouse Middle (Double)") },
		{ MouseAction::X1Click,				_T("Mouse X1") },
		{ MouseAction::X1DoubleClick,		_T("Mouse X1 (Double)") },
		{ MouseAction::X2Click,				_T("Mouse X2") },
		{ MouseAction::X2DoubleClick,		_T("Mouse X2 (Double)") },
		{ MouseAction::WheelPlus,			_T("Mouse Wheel +") },
		{ MouseAction::WheelMinus,			_T("Mouse Wheel -") },
	};

	int i = (int)mouseAction;
	LN_ASSERT(0 <= i && i < LN_ARRAY_SIZE_OF(table));
	LN_ASSERT(mouseAction == table[i].mouseAction);
	return table[i].name;
}

//------------------------------------------------------------------------------
MouseBindingPtr MouseGesture::create(MouseAction mouseAction, ModifierKeys modifierKeys)
{
	auto ptr = MouseBindingPtr::makeRef(mouseAction, modifierKeys);
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
String MouseGesture::getDisplayName() const
{
	String name;
	if (m_modifierKeys != ModifierKeys::None)
	{
		name = getModifierKeysName(m_modifierKeys) + _T("+");
	}
	name += getMouseActionName(m_mouseAction);
	return name;
}

//------------------------------------------------------------------------------
detail::InputBindingType MouseGesture::getType() const
{
	return detail::InputBindingType::Mouse;
}


//==============================================================================
// GamepadGesture
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(GamepadGesture, InputGesture);

static const String& getGamepadElementName(GamepadElement element)
{
	struct NamePair
	{
		GamepadElement	element;	// for verify
		String			name;
	};
	static const NamePair table[] =
	{
		{ GamepadElement::None,			nullptr },
		{ GamepadElement::Button1,		_T("Gamepad Button 1") },
		{ GamepadElement::Button2,		_T("Gamepad Button 2") },
		{ GamepadElement::Button3,		_T("Gamepad Button 3") },
		{ GamepadElement::Button4,		_T("Gamepad Button 4") },
		{ GamepadElement::Button5,		_T("Gamepad Button 5") },
		{ GamepadElement::Button6,		_T("Gamepad Button 6") },
		{ GamepadElement::Button7,		_T("Gamepad Button 7") },
		{ GamepadElement::Button8,		_T("Gamepad Button 8") },
		{ GamepadElement::Button9,		_T("Gamepad Button 9") },
		{ GamepadElement::Button10,		_T("Gamepad Button 10") },
		{ GamepadElement::Button11,		_T("Gamepad Button 11") },
		{ GamepadElement::Button12,		_T("Gamepad Button 12") },
		{ GamepadElement::Button13,		_T("Gamepad Button 13") },
		{ GamepadElement::Button14,		_T("Gamepad Button 14") },
		{ GamepadElement::Button15,		_T("Gamepad Button 15") },
		{ GamepadElement::Button16,		_T("Gamepad Button 16") },
		{ GamepadElement::PovLeft,		_T("Gamepad POV Left") },
		{ GamepadElement::PovRight,		_T("Gamepad POV Right") },
		{ GamepadElement::PovUp,		_T("Gamepad POV Up") },
		{ GamepadElement::PovDown,		_T("Gamepad POV Down") },
		{ GamepadElement::Axis1,		_T("Gamepad Axis 1") },
		{ GamepadElement::Axis2,		_T("Gamepad Axis 2") },
		{ GamepadElement::Axis3,		_T("Gamepad Axis 3") },
		{ GamepadElement::Axis4,		_T("Gamepad Axis 4") },
		{ GamepadElement::Axis5,		_T("Gamepad Axis 5") },
		{ GamepadElement::Axis6,		_T("Gamepad Axis 6") },
		{ GamepadElement::Axis7,		_T("Gamepad Axis 7") },
		{ GamepadElement::Axis8,		_T("Gamepad Axis 8") },
		{ GamepadElement::Axis1Minus,	_T("Gamepad Axis 1 -") },
		{ GamepadElement::Axis1Plus,	_T("Gamepad Axis 1 +") },
		{ GamepadElement::Axis2Minus,	_T("Gamepad Axis 2 -") },
		{ GamepadElement::Axis2Plus,	_T("Gamepad Axis 2 +") },
		{ GamepadElement::Axis3Minus,	_T("Gamepad Axis 3 -") },
		{ GamepadElement::Axis3Plus,	_T("Gamepad Axis 3 +") },
		{ GamepadElement::Axis4Minus,	_T("Gamepad Axis 4 -") },
		{ GamepadElement::Axis4Plus,	_T("Gamepad Axis 4 +") },
		{ GamepadElement::Axis5Minus,	_T("Gamepad Axis 5 -") },
		{ GamepadElement::Axis5Plus,	_T("Gamepad Axis 5 +") },
		{ GamepadElement::Axis6Minus,	_T("Gamepad Axis 6 -") },
		{ GamepadElement::Axis6Plus,	_T("Gamepad Axis 6 +") },
		{ GamepadElement::Axis7Minus,	_T("Gamepad Axis 7 -") },
		{ GamepadElement::Axis7Plus,	_T("Gamepad Axis 7 +") },
		{ GamepadElement::Axis8Minus,	_T("Gamepad Axis 8 -") },
		{ GamepadElement::Axis8Plus,	_T("Gamepad Axis 8 +") },
	};

	int i = (int)element;
	LN_ASSERT(0 <= i && i < LN_ARRAY_SIZE_OF(table));
	LN_ASSERT(element == table[i].element);
	return table[i].name;
}

//------------------------------------------------------------------------------
GamepadBindingPtr GamepadGesture::create(GamepadElement element)
{
	auto ptr = GamepadBindingPtr::makeRef(element);
	return ptr;
}

//------------------------------------------------------------------------------
GamepadGesture::GamepadGesture(GamepadElement element)
	: m_padNumber(0)
	, m_element(element)
{
}

//------------------------------------------------------------------------------
GamepadGesture::~GamepadGesture()
{
}

//------------------------------------------------------------------------------
String GamepadGesture::getDisplayName() const
{
	return getGamepadElementName(m_element);
}

//------------------------------------------------------------------------------
detail::InputBindingType GamepadGesture::getType() const
{
	return detail::InputBindingType::Gamepad;
}

LN_NAMESPACE_END
