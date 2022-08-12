
#include "../Internal.hpp"
#include <LuminoEngine/Input/InputBinding.hpp>

namespace ln {

//==============================================================================
// InputGesture
//==============================================================================
LN_OBJECT_IMPLEMENT(InputGesture, Object) {}

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
LN_OBJECT_IMPLEMENT(KeyGesture, InputGesture) {}

static const String& getKeyName(Keys key)
{
	struct NamePair
	{
		Keys	key;	// for verify
		String	name;
	};
	static const NamePair table[] =
	{
		{ Keys::Unknown,	_LT("Unknown") },
		{ Keys::A,			_LT("A") },
		{ Keys::B,			_LT("B") },
		{ Keys::C,			_LT("C") },
		{ Keys::D,			_LT("D") },
		{ Keys::E,			_LT("E") },
		{ Keys::F,			_LT("F") },
		{ Keys::G,			_LT("G") },
		{ Keys::H,			_LT("H") },
		{ Keys::I,			_LT("I") },
		{ Keys::J,			_LT("J") },
		{ Keys::K,			_LT("K") },
		{ Keys::L,			_LT("L") },
		{ Keys::M,			_LT("M") },
		{ Keys::N,			_LT("N") },
		{ Keys::O,			_LT("O") },
		{ Keys::P,			_LT("P") },
		{ Keys::Q,			_LT("Q") },
		{ Keys::R,			_LT("R") },
		{ Keys::S,			_LT("S") },
		{ Keys::T,			_LT("T") },
		{ Keys::U,			_LT("U") },
		{ Keys::V,			_LT("V") },
		{ Keys::W,			_LT("W") },
		{ Keys::X,			_LT("X") },
		{ Keys::Y,			_LT("Y") },
		{ Keys::Z,			_LT("Z") },

		{ Keys::D0,			_LT("0") },
		{ Keys::D1,			_LT("1") },
		{ Keys::D2,			_LT("2") },
		{ Keys::D3,			_LT("3") },
		{ Keys::D4,			_LT("4") },
		{ Keys::D5,			_LT("5") },
		{ Keys::D6,			_LT("6") },
		{ Keys::D7,			_LT("7") },
		{ Keys::D8,			_LT("8") },
		{ Keys::D9,			_LT("9") },

		{ Keys::F1,			_LT("F1") },
		{ Keys::F2,			_LT("F2") },
		{ Keys::F3,			_LT("F3") },
		{ Keys::F4,			_LT("F4") },
		{ Keys::F5,			_LT("F5") },
		{ Keys::F6,			_LT("F6") },
		{ Keys::F7,			_LT("F7") },
		{ Keys::F8,			_LT("F8") },
		{ Keys::F9,			_LT("F9") },
		{ Keys::F10,		_LT("F10") },
		{ Keys::F11,		_LT("F11") },
		{ Keys::F12,		_LT("F12") },

		{ Keys::Space,		_LT("Space") },
		{ Keys::Escape,		_LT("Esc") },
		{ Keys::Up,			_LT("Up") },
		{ Keys::Down,		_LT("Down") },
		{ Keys::Left,		_LT("Left") },
		{ Keys::Right,		_LT("Right") },
		{ Keys::LShift,		_LT("LeftShift") },
		{ Keys::RShift,		_LT("RightShift") },
		{ Keys::LCtrl,		_LT("LeftCtrl") },
		{ Keys::RCtrl,		_LT("RightCtrl") },
		{ Keys::LAlt,		_LT("LeftAlt") },
		{ Keys::RAlt,		_LT("RightAlt") },
		{ Keys::Tab,		_LT("Tab") },
		{ Keys::Enter,		_LT("Enter") },
		{ Keys::BackSpace,	_LT("BackSpace") },
		{ Keys::Insert,		_LT("Insert") },
		{ Keys::Delete,		_LT("Delete") },
		{ Keys::PageUp,		_LT("PageUp") },
		{ Keys::PageDown,	_LT("PageDown") },
		{ Keys::Home,		_LT("Home") },
		{ Keys::End,		_LT("End") },

		{ Keys::Colon,		_LT(":") },
		{ Keys::Semicolon,	_LT(";") },
		{ Keys::Comma,		_LT(",") },
		{ Keys::Period,		_LT(".") },
		{ Keys::Slash,		_LT("/") },
		{ Keys::Minus,		_LT("-") },
		{ Keys::Backslash,	_LT("\\") },
		{ Keys::Yen,		_LT("\\") },
		{ Keys::Caret,		_LT("'") },
		{ Keys::LBracket,	_LT("[") },
		{ Keys::RBracket,	_LT("]") },

		{ Keys::AndroidPlus,	_LT("+") },
	};

	int i = (int)key;
	LN_CHECK(0 <= i && i < LN_ARRAY_SIZE_OF(table));
	LN_CHECK(key == table[i].key);
	return table[i].name;
}

static String getModifierKeysName(ModifierKeys modifierKeyss)
{
	String name;
	if (testFlag(modifierKeyss, ModifierKeys::Control))
	{
		name = _LT("Ctrl");
	}
	if (testFlag(modifierKeyss, ModifierKeys::Shift))
	{
		if (!name.isEmpty()) name += _LT("+");
		name = _LT("Shift");
	}
	if (testFlag(modifierKeyss, ModifierKeys::Alt))
	{
		if (!name.isEmpty()) name += _LT("+");
		name = _LT("Alt");
	}
	return name;
}

//------------------------------------------------------------------------------
Ref<KeyGesture> KeyGesture::create(Keys key, ModifierKeys modifierKeys)
{
	return makeObject_deprecated<KeyGesture>(key, modifierKeys);
}

KeyGesture::KeyGesture()
	: m_key(Keys::Unknown)
	, m_modifierKeys(ModifierKeys::None)
{}

KeyGesture::~KeyGesture()
{
}

void KeyGesture::init()
{
	InputGesture::init();
}

void KeyGesture::init(Keys key)
{
	InputGesture::init();
	m_key = key;
}

void KeyGesture::init(Keys key, ModifierKeys modifierKeys)
{
	InputGesture::init();
	m_key = key;
	m_modifierKeys = modifierKeys;
}

//------------------------------------------------------------------------------
String KeyGesture::getDisplayName() const
{
	String name;
	if (m_modifierKeys != ModifierKeys::None)
	{
		name = getModifierKeysName(m_modifierKeys) + _LT("+");
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
LN_OBJECT_IMPLEMENT(MouseGesture, InputGesture) {}

static const String& getMouseActionName(MouseAction mouseAction)
{
	struct NamePair
	{
		MouseAction	mouseAction;	// for verify
		String		name;
	};
	static const NamePair table[] =
	{
		{ MouseAction::LeftClick,			_LT("Mouse Left") },
		{ MouseAction::LeftDoubleClick,		_LT("Mouse Left (Double)") },
		{ MouseAction::RightClick,			_LT("Mouse Right") },
		{ MouseAction::RightDoubleClick,	_LT("Mouse Right (Double)") },
		{ MouseAction::MiddleClick,			_LT("Mouse Middle") },
		{ MouseAction::MiddleDoubleClick,	_LT("Mouse Middle (Double)") },
		{ MouseAction::X1Click,				_LT("Mouse X1") },
		{ MouseAction::X1DoubleClick,		_LT("Mouse X1 (Double)") },
		{ MouseAction::X2Click,				_LT("Mouse X2") },
		{ MouseAction::X2DoubleClick,		_LT("Mouse X2 (Double)") },
		{ MouseAction::WheelPlus,			_LT("Mouse Wheel +") },
		{ MouseAction::WheelMinus,			_LT("Mouse Wheel -") },
	};

	int i = (int)mouseAction;
	LN_CHECK(0 <= i && i < LN_ARRAY_SIZE_OF(table));
	LN_CHECK(mouseAction == table[i].mouseAction);
	return table[i].name;
}

//------------------------------------------------------------------------------
Ref<MouseGesture> MouseGesture::create(MouseAction mouseAction, ModifierKeys modifierKeys)
{
	return makeObject_deprecated<MouseGesture>(mouseAction, modifierKeys);
}

//------------------------------------------------------------------------------
MouseGesture::MouseGesture()
	: m_mouseAction(MouseAction::None)
	, m_modifierKeys(ModifierKeys::None)
{
}

//------------------------------------------------------------------------------
MouseGesture::~MouseGesture()
{
}

void MouseGesture::init()
{
	InputGesture::init();
}

void MouseGesture::init(MouseAction mouseAction, ModifierKeys modifierKeys)
{
	InputGesture::init();
	m_mouseAction = mouseAction;
	m_modifierKeys = modifierKeys;
}

//------------------------------------------------------------------------------
String MouseGesture::getDisplayName() const
{
	String name;
	if (m_modifierKeys != ModifierKeys::None)
	{
		name = getModifierKeysName(m_modifierKeys) + _LT("+");
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
LN_OBJECT_IMPLEMENT(GamepadGesture, InputGesture) {}

static const String& getGamepadElementName(GamepadElement element)
{
	struct NamePair
	{
		GamepadElement	element;	// for verify
		String			name;
	};
	static const NamePair table[] =
	{
		{ GamepadElement::None,			_LT("") },
		{ GamepadElement::Button1,		_LT("Gamepad Button 1") },
		{ GamepadElement::Button2,		_LT("Gamepad Button 2") },
		{ GamepadElement::Button3,		_LT("Gamepad Button 3") },
		{ GamepadElement::Button4,		_LT("Gamepad Button 4") },
		{ GamepadElement::Button5,		_LT("Gamepad Button 5") },
		{ GamepadElement::Button6,		_LT("Gamepad Button 6") },
		{ GamepadElement::Button7,		_LT("Gamepad Button 7") },
		{ GamepadElement::Button8,		_LT("Gamepad Button 8") },
		{ GamepadElement::Button9,		_LT("Gamepad Button 9") },
		{ GamepadElement::Button10,		_LT("Gamepad Button 10") },
		{ GamepadElement::Button11,		_LT("Gamepad Button 11") },
		{ GamepadElement::Button12,		_LT("Gamepad Button 12") },
		{ GamepadElement::Button13,		_LT("Gamepad Button 13") },
		{ GamepadElement::Button14,		_LT("Gamepad Button 14") },
		{ GamepadElement::Button15,		_LT("Gamepad Button 15") },
		{ GamepadElement::Button16,		_LT("Gamepad Button 16") },
		{ GamepadElement::PovLeft,		_LT("Gamepad POV Left") },
		{ GamepadElement::PovRight,		_LT("Gamepad POV Right") },
		{ GamepadElement::PovUp,		_LT("Gamepad POV Up") },
		{ GamepadElement::PovDown,		_LT("Gamepad POV Down") },
		{ GamepadElement::Axis1,		_LT("Gamepad Axis 1") },
		{ GamepadElement::Axis2,		_LT("Gamepad Axis 2") },
		{ GamepadElement::Axis3,		_LT("Gamepad Axis 3") },
		{ GamepadElement::Axis4,		_LT("Gamepad Axis 4") },
		{ GamepadElement::Axis5,		_LT("Gamepad Axis 5") },
		{ GamepadElement::Axis6,		_LT("Gamepad Axis 6") },
		{ GamepadElement::Axis7,		_LT("Gamepad Axis 7") },
		{ GamepadElement::Axis8,		_LT("Gamepad Axis 8") },
		{ GamepadElement::Axis1Minus,	_LT("Gamepad Axis 1 -") },
		{ GamepadElement::Axis1Plus,	_LT("Gamepad Axis 1 +") },
		{ GamepadElement::Axis2Minus,	_LT("Gamepad Axis 2 -") },
		{ GamepadElement::Axis2Plus,	_LT("Gamepad Axis 2 +") },
		{ GamepadElement::Axis3Minus,	_LT("Gamepad Axis 3 -") },
		{ GamepadElement::Axis3Plus,	_LT("Gamepad Axis 3 +") },
		{ GamepadElement::Axis4Minus,	_LT("Gamepad Axis 4 -") },
		{ GamepadElement::Axis4Plus,	_LT("Gamepad Axis 4 +") },
		{ GamepadElement::Axis5Minus,	_LT("Gamepad Axis 5 -") },
		{ GamepadElement::Axis5Plus,	_LT("Gamepad Axis 5 +") },
		{ GamepadElement::Axis6Minus,	_LT("Gamepad Axis 6 -") },
		{ GamepadElement::Axis6Plus,	_LT("Gamepad Axis 6 +") },
		{ GamepadElement::Axis7Minus,	_LT("Gamepad Axis 7 -") },
		{ GamepadElement::Axis7Plus,	_LT("Gamepad Axis 7 +") },
		{ GamepadElement::Axis8Minus,	_LT("Gamepad Axis 8 -") },
		{ GamepadElement::Axis8Plus,	_LT("Gamepad Axis 8 +") },
	};

	int i = (int)element;
	LN_CHECK(0 <= i && i < LN_ARRAY_SIZE_OF(table));
	LN_CHECK(element == table[i].element);
	return table[i].name;
}

//------------------------------------------------------------------------------
Ref<GamepadGesture> GamepadGesture::create(GamepadElement element)
{
	return makeObject_deprecated<GamepadGesture>(element);
}

//------------------------------------------------------------------------------
GamepadGesture::GamepadGesture()
	: m_padNumber(0)
	, m_element(GamepadElement::None)
{
}

//------------------------------------------------------------------------------
GamepadGesture::~GamepadGesture()
{
}

void GamepadGesture::init()
{
	InputGesture::init();
}

void GamepadGesture::init(GamepadElement element)
{
	InputGesture::init();
	m_element = element;
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

} // namespace ln
