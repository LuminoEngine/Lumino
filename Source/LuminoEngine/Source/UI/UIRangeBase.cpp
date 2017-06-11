
#include "Internal.h"
#include <Lumino/UI/UIRangeBase.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UIRangeBase
//------------------------------------------------------------------------------
/*
	OnMaximumChanged, OnMinimumChanged は用意するが、対応する Changed イベントは
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
void UIRangeBase::Initialize()
{
	UIControl::Initialize();
}

//------------------------------------------------------------------------------
void UIRangeBase::SetValue(float value)
{
	float oldValue = m_value;
	m_value = value;
	if (m_value != oldValue)
	{
		OnValueChanged(oldValue, m_value);
	}
}

//------------------------------------------------------------------------------
float UIRangeBase::GetValue() const
{
	return m_value;
}

//------------------------------------------------------------------------------
void UIRangeBase::SetMinimum(float value)
{
	float oldValue = m_minimum;
	m_minimum = value;
	if (m_minimum != oldValue)
	{
		OnMinimumChanged(oldValue, m_minimum);
	}
}

//------------------------------------------------------------------------------
float UIRangeBase::GetMinimum() const
{
	return m_minimum;
}

//------------------------------------------------------------------------------
void UIRangeBase::SetMaximum(float value)
{
	float oldValue = m_maximum;
	m_maximum = value;
	if (m_maximum != oldValue)
	{
		OnMaximumChanged(oldValue, m_maximum);
	}
}

//------------------------------------------------------------------------------
float UIRangeBase::GetMaximum() const
{
	return m_maximum;
}

//------------------------------------------------------------------------------
void UIRangeBase::OnValueChanged(float oldValue, float newValue)
{
}

//------------------------------------------------------------------------------
void UIRangeBase::OnMinimumChanged(float oldMinimum, float newMinimum)
{
}

//------------------------------------------------------------------------------
void UIRangeBase::OnMaximumChanged(float oldMaximum, float newMaximum)
{
}

LN_NAMESPACE_END
