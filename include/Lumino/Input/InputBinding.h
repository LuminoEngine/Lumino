
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
class GamepadInputBinding;
using GamepadInputBindingPtr = RefPtr<GamepadInputBinding>;

enum class GamepadInputElement
{
	Button1,
	Button2,
	Button3,
	Button4,
	Button5,
	Button6,
	Button7,
	Button8,
	Button9,
	Button10,
	Button11,
	Button12,
	Button13,
	Button14,
	Button15,
	Button16,
	//
	PovLeft,
	PovRight,
	PovUp,
	PovDown,
	//
	Axis1,			/**< -1.0 .. 1.0 */
	Axis2,			/**< -1.0 .. 1.0 */
	Axis3,			/**< -1.0 .. 1.0 */
	Axis4,			/**< -1.0 .. 1.0 */
	Axis5,			/**< -1.0 .. 1.0 */
	Axis6,			/**< -1.0 .. 1.0 */
	Axis7,			/**< -1.0 .. 1.0 */
	Axis8,			/**< -1.0 .. 1.0 */
	//
	Axis1Minus,		///< X1 -1.0
	Axis1Plus,		///< X1  1.0
	Axis2Minus,		///< Y1 -1.0
	Axis2Plus,		///< Y1  1.0
	Axis3Minus,		///< X2 -1.0
	Axis3Plus,		///< X2  1.0
	Axis4Minus,		///< Y2 -1.0
	Axis4Plus,		///< Y2  1.0
	Axis5Minus,		///< X3 -1.0
	Axis5Plus,		///< X3  1.0     XInput LT
	Axis6Minus,		///< Y3 -1.0
	Axis6Plus,		///< Y3  1.0     XInput RT
	Axis7Minus,		///< X4 -1.0
	Axis7Plus,		///< X4  1.0
	Axis8Minus,		///< Y4 -1.0
	Axis8Plus,		///< Y4  1.0
};

namespace detail {

enum class InputBindingType
{
	Keyboard,
	Gamepad,
};

} // namespace detail

/**
	@brief	
*/
class InputBinding
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/**
		@brief		InputBinding オブジェクトを作成します。
	*/
	static InputBindingPtr CreateKeyboardBinding(Key key, ModifierKeys modifier = ModifierKeys::None);

	static InputBindingPtr CreateJoystickButtonBinding(int buttonNumber);
	static InputBindingPtr CreateJoystickAxisBinding(int axisNumber);
	static InputBindingPtr CreateJoystickPovBinding(JoystickPovAxis direction);

public:
	float GetMinValidMThreshold() const { return m_minValidMThreshold; }

	bool IsNegativeValue() const { return m_negativeValue; }

	bool EqualKeyInput(Key keyCode, ModifierKeys modifier);

LN_INTERNAL_ACCESS:
	InputBinding();
	virtual ~InputBinding();
	void InitializeKeyboardBinding(Key key, ModifierKeys modifier);
	void InitializeJoystickButtonBinding(int buttonNumber);
	void InitializeJoystickAxisBinding(int axisNumber);
	void InitializeJoystickPovBinding(JoystickPovAxis direction);
	const detail::DeviceInputSource& GetDeviceInputSource() const { return m_source; }

LN_PROTECTED_INTERNAL_ACCESS:
	virtual detail::InputBindingType GetType() const;

private:
	detail::DeviceInputSource	m_source;
	float						m_minValidMThreshold;
	bool						m_negativeValue;
};


/**
	@brief	
*/
class GamepadInputBinding
	: public InputBinding
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	static GamepadInputBindingPtr Create(GamepadInputElement element);

public:
	GamepadInputElement GetElement() const { return m_element; }

LN_INTERNAL_ACCESS:
	GamepadInputBinding(GamepadInputElement element);
	virtual ~GamepadInputBinding();

LN_PROTECTED_INTERNAL_ACCESS:
	virtual detail::InputBindingType GetType() const;

private:
	GamepadInputElement		m_element;
};

LN_NAMESPACE_END
