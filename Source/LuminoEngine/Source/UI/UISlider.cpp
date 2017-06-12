
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
RefPtr<UISlider> UISlider::Create()
{
	return NewObject<UISlider>();
}

//------------------------------------------------------------------------------
RefPtr<UISlider> UISlider::Create(float value, float minimum, float maximum)
{
	return NewObject<UISlider>(value, minimum, maximum);
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

	m_track = NewObject<UITrack>();
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
	SetValue(value);
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
	if (e->GetType() == UIThumb::DragStartedEventId)
	{
		m_dragStartValue = m_track->GetValue();
	}
	else if (e->GetType() == UIThumb::DragDeltaEventId)
	{
		auto* e2 = static_cast<UIDragDeltaEventArgs*>(e);
		float newValue = m_dragStartValue + m_track->ValueFromDistance(e2->horizontalChange, e2->verticalChange);
		UpdateValue(Math::Clamp(newValue, GetMinimum(), GetMaximum()));
	}
	else if (e->GetType() == UIThumb::DragCompletedEventId)
	{
	}
	UIControl::OnRoutedEvent(e);
}

//------------------------------------------------------------------------------
Size UISlider::MeasureOverride(const Size& constraint)
{
	m_track->MeasureLayout(constraint);
	return UIControl::MeasureOverride(constraint);
}

//------------------------------------------------------------------------------
Size UISlider::ArrangeOverride(const Size& finalSize)
{
	Orientation orientation = GetOrientation();

	switch (orientation)
	{
	case Orientation::Horizontal:
		m_track->ArrangeLayout(Rect(0, 0, finalSize));
		break;
	case Orientation::Vertical:
		m_track->ArrangeLayout(Rect(0, 0, finalSize));
		break;
	default:
		LN_NOTIMPLEMENTED();
		break;
	}

	return UIControl::ArrangeOverride(finalSize);
}

//------------------------------------------------------------------------------
void UISlider::OnValueChanged(float oldValue, float newValue)
{
	m_track->SetValue(newValue);
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
void UISlider::UpdateValue(float value)
{
	float snappedValue = value;
	SetValue(snappedValue);
}

LN_NAMESPACE_END
