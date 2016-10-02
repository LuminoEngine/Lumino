
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN

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

private:
	detail::DeviceInputSource	m_source;
	float						m_minValidMThreshold;
	bool						m_negativeValue;
};


LN_NAMESPACE_END
