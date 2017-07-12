
#include "Internal.h"
#include <Lumino/UI/UIRangeBase.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UIRangeBase
//------------------------------------------------------------------------------
/*
	onMaximumChanged, onMinimumChanged は用意するが、対応する Changed イベントは
	用意しない。WPF のインターフェイスと合わせたものだが、そもそもそれをハンドルする
	ようなこともないだろうし、キャンセルされたときに困る。
*/
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIRangeBase, UIControl);

//------------------------------------------------------------------------------
UIRangeBase::UIRangeBase()
	: m_value(0.0f)
	, m_minimum(0.0f)
	, m_maximum(1.0f)
{
}

//------------------------------------------------------------------------------
UIRangeBase::~UIRangeBase()
{
}

//------------------------------------------------------------------------------
void UIRangeBase::initialize()
{
	UIControl::initialize();
}

//------------------------------------------------------------------------------
void UIRangeBase::setValue(float value)
{
	float oldValue = m_value;
	m_value = value;
	if (m_value != oldValue)
	{
		onValueChanged(oldValue, m_value);
	}
}

//------------------------------------------------------------------------------
float UIRangeBase::getValue() const
{
	return m_value;
}

//------------------------------------------------------------------------------
void UIRangeBase::setMinimum(float value)
{
	float oldValue = m_minimum;
	m_minimum = value;
	if (m_minimum != oldValue)
	{
		onMinimumChanged(oldValue, m_minimum);
	}
}

//------------------------------------------------------------------------------
float UIRangeBase::getMinimum() const
{
	return m_minimum;
}

//------------------------------------------------------------------------------
void UIRangeBase::setMaximum(float value)
{
	float oldValue = m_maximum;
	m_maximum = value;
	if (m_maximum != oldValue)
	{
		onMaximumChanged(oldValue, m_maximum);
	}
}

//------------------------------------------------------------------------------
float UIRangeBase::getMaximum() const
{
	return m_maximum;
}

//------------------------------------------------------------------------------
void UIRangeBase::onValueChanged(float oldValue, float newValue)
{
}

//------------------------------------------------------------------------------
void UIRangeBase::onMinimumChanged(float oldMinimum, float newMinimum)
{
}

//------------------------------------------------------------------------------
void UIRangeBase::onMaximumChanged(float oldMaximum, float newMaximum)
{
}

LN_NAMESPACE_END
