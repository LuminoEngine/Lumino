
#include "Internal.h"
#include <Lumino/UI/UISlider.h>
#include <Lumino/UI/UIScrollViewer.h>	// TODO: Track(Primitive?)
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UISlider
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UISlider, UIRangeBase);

//------------------------------------------------------------------------------
RefPtr<UISlider> UISlider::create()
{
	return newObject<UISlider>();
}

//------------------------------------------------------------------------------
RefPtr<UISlider> UISlider::create(float value, float minimum, float maximum)
{
	return newObject<UISlider>(value, minimum, maximum);
}

//------------------------------------------------------------------------------
UISlider::UISlider()
	: m_dragStartValue(0)
{
}

//------------------------------------------------------------------------------
UISlider::~UISlider()
{
}

//------------------------------------------------------------------------------
void UISlider::initialize()
{
	UIRangeBase::initialize();

	// register VisualState
	auto* vsm = GetVisualStateManager();
	vsm->RegisterVisualState(UIVisualStates::OrientationGroup, UIVisualStates::HorizontalState);
	vsm->RegisterVisualState(UIVisualStates::OrientationGroup, UIVisualStates::VerticalState);

	m_track = newObject<UITrack>();
	m_track->SetStyleSubControlName(_T("UISlider"), _T("Track"));
	m_track->GetThumb()->SetStyleSubControlName(_T("UISlider"), _T("Thumb"));
	m_track->GetDecreaseButton()->SetStyleSubControlName(_T("UISlider"), _T("DecreaseButton"));
	m_track->GetIncreaseButton()->SetStyleSubControlName(_T("UISlider"), _T("IncreaseButton"));
	m_track->SetViewportSize(Math::NaN);	// 値の計算に slider モードを使用する
	AddVisualChild(m_track);
}

//------------------------------------------------------------------------------
void UISlider::initialize(float value, float minimum, float maximum)
{
	initialize();
	setValue(value);
	SetMinimum(minimum);
	SetMaximum(maximum);
}

//------------------------------------------------------------------------------
void UISlider::SetOrientation(Orientation orientation)
{
	m_track->SetOrientation(orientation);

	switch (orientation)
	{
	case Orientation::Horizontal:
		GoToVisualState(UIVisualStates::HorizontalState);
		break;
	case Orientation::Vertical:
		GoToVisualState(UIVisualStates::VerticalState);
		break;
	default:
		LN_NOTIMPLEMENTED();
		break;
	}
}

//------------------------------------------------------------------------------
Orientation UISlider::GetOrientation() const
{
	return m_track->GetOrientation();
}

//------------------------------------------------------------------------------
void UISlider::OnRoutedEvent(UIEventArgs* e)
{
	if (e->getType() == UIThumb::DragStartedEventId)
	{
		m_dragStartValue = m_track->getValue();
	}
	else if (e->getType() == UIThumb::DragDeltaEventId)
	{
		auto* e2 = static_cast<UIDragDeltaEventArgs*>(e);
		float newValue = m_dragStartValue + m_track->ValueFromDistance(e2->horizontalChange, e2->verticalChange);
		updateValue(Math::clamp(newValue, GetMinimum(), GetMaximum()));
	}
	else if (e->getType() == UIThumb::DragCompletedEventId)
	{
	}
	UIControl::OnRoutedEvent(e);
}

//------------------------------------------------------------------------------
Size UISlider::measureOverride(const Size& constraint)
{
	m_track->measureLayout(constraint);
	return UIControl::measureOverride(constraint);
}

//------------------------------------------------------------------------------
Size UISlider::arrangeOverride(const Size& finalSize)
{
	Orientation orientation = GetOrientation();

	switch (orientation)
	{
	case Orientation::Horizontal:
		m_track->arrangeLayout(Rect(0, 0, finalSize));
		break;
	case Orientation::Vertical:
		m_track->arrangeLayout(Rect(0, 0, finalSize));
		break;
	default:
		LN_NOTIMPLEMENTED();
		break;
	}

	return UIControl::arrangeOverride(finalSize);
}

//------------------------------------------------------------------------------
void UISlider::OnValueChanged(float oldValue, float newValue)
{
	m_track->setValue(newValue);
}

//------------------------------------------------------------------------------
void UISlider::OnMinimumChanged(float oldMinimum, float newMinimum)
{
	m_track->SetMinimum(newMinimum);
}

//------------------------------------------------------------------------------
void UISlider::OnMaximumChanged(float oldMaximum, float newMaximum)
{
	m_track->SetMaximum(newMaximum);
}

//------------------------------------------------------------------------------
void UISlider::updateValue(float value)
{
	float snappedValue = value;
	setValue(snappedValue);
}

LN_NAMESPACE_END
