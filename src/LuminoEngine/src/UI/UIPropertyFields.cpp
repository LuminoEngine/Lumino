
#include "Internal.hpp"
#include <LuminoEngine/UI/UIPropertyFields.hpp>

namespace ln {
	
//==============================================================================
// UISliderField

UISliderField::UISliderField()
	: m_value(0.0f)
	, m_minimum(0.0f)
	, m_maximum(1.0f)
{
}

void UISliderField::init()
{
	UIElement::init();
}

void UISliderField::setValue(float value)
{
	float oldValue = m_value;
	m_value = value;
	if (m_value != oldValue) {
		onValueChanged(oldValue, m_value);
	}
}

float UISliderField::value() const
{
	return m_value;
}

void UISliderField::setMinimum(float value)
{
	float oldValue = m_minimum;
	m_minimum = value;
	if (m_minimum != oldValue) {
		onMinimumChanged(oldValue, m_minimum);
	}
}

float UISliderField::minimum() const
{
	return m_minimum;
}

void UISliderField::setMaximum(float value)
{
	float oldValue = m_maximum;
	m_maximum = value;
	if (m_maximum != oldValue) {
		onMaximumChanged(oldValue, m_maximum);
	}
}

float UISliderField::maximum() const
{
	return m_maximum;
}

void UISliderField::onValueChanged(float oldValue, float newValue)
{
}

void UISliderField::onMinimumChanged(float oldMinimum, float newMinimum)
{
}

void UISliderField::onMaximumChanged(float oldMaximum, float newMaximum)
{
}


} // namespace ln

