
#include "Internal.hpp"
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UIPropertyFields.hpp>

namespace ln {
	
//==============================================================================
// UISliderField

UISliderField::UISliderField()
	: m_value(0.0f)
	, m_minimum(0.0f)
	, m_maximum(1.0f)
	, m_dragging(false)
{
}

void UISliderField::init()
{
	UIControl::init(nullptr);

	setBackgroundColor(Color::White.withAlpha(0.5));
	setBorderColor(Color::Gray);
	setBorderThickness(1);
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

void UISliderField::onRoutedEvent(UIEventArgs* e)
{
	if (e->type() == UIEvents::MouseDownEvent)
	{
		if (!m_dragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			updateValue(mouseEvent->getPosition(this));

			m_dragging = true;
			retainCapture();
			e->handled = true;
			return;
		}
	}
	else if (e->type() == UIEvents::MouseUpEvent)
	{
		if (m_dragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);

			m_dragging = false;
			releaseCapture();
			e->handled = true;
			return;
		}
	}
	else if (e->type() == UIEvents::MouseMoveEvent)
	{
		if (m_dragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			updateValue(mouseEvent->getPosition(this));

			e->handled = true;
			return;
		}
	}

	UIElement::onRoutedEvent(e);
}

void UISliderField::onRender(UIRenderingContext* context)
{
	float r = (m_value - m_minimum) / (m_maximum - m_minimum);
	auto cr = clientRect();
	auto bar = cr.makeDeflate(2);
	bar.width = bar.width * r;
	context->drawSolidRectangle(bar, context->theme()->color(UIThemeConstantPalette::PrimaryMainColor));

	context->drawText(u"0.000", cr, TextAlignment::Center);
}

void UISliderField::updateValue(const Point& pos)
{
	auto cr = clientRect();
	//if (cr.contains(pos)) {
		float r = (pos.x - cr.x) / cr.width;
		m_value = Math::clamp(m_minimum + ((m_maximum - m_minimum) * r), m_minimum, m_maximum);
	//}
}


} // namespace ln

