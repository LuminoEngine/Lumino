
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief	
*/
class InputBinding
	: public tr::ReflectionObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/**
		@brief		InputBinding オブジェクトを作成します。
	*/
	static InputBindingPtr Create(const String& bindingName, Key key, ModifierKeys modifier = ModifierKeys::None);

public:

	const String& GetName() const { return m_bindingName; }

	float GetMinValidMThreshold() const { return m_minValidMThreshold; }

	bool IsNegativeValue() const { return m_negativeValue; }

	bool EqualKeyInput(Key keyCode, ModifierKeys modifier);

LN_INTERNAL_ACCESS:
	InputBinding();
	InputBinding(const String& bindingName, Key key, ModifierKeys modifier);
	virtual ~InputBinding();
	void Initialize(detail::InputManager* manager);
	const detail::DeviceInputSource& GetDeviceInputSource() const { return m_source; }

private:
	String						m_bindingName;
	detail::DeviceInputSource	m_source;
	float						m_minValidMThreshold;
	bool						m_negativeValue;
};

LN_NAMESPACE_END
