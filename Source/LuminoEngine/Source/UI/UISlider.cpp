
#include "Internal.h"
#include <Lumino/UI/UISlider.h>
#include <Lumino/UI/UIScrollViewer.h>	// TODO: Track(Primitive?)
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIRangeBase
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIRangeBase, UIControl);

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
	OnValueChanged(oldValue, m_value);
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
	OnMaximumChanged(oldValue, m_minimum);
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
	OnMinimumChanged(oldValue, m_maximum);
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
void UIRangeBase::OnMaximumChanged(float oldMaximum, float newMaximum)
{
}
//------------------------------------------------------------------------------
void UIRangeBase::OnMinimumChanged(float oldMinimum, float newMinimum)
{
}

//==============================================================================
// UISlider
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UISlider, UIRangeBase);

//------------------------------------------------------------------------------
RefPtr<UISlider> UISlider::Create()
{
	return NewObject<UISlider>();
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
void UISlider::Initialize()
{
	UIRangeBase::Initialize();

	// register VisualState
	auto* vsm = GetVisualStateManager();
	vsm->RegisterVisualState(UIVisualStates::OrientationGroup, UIVisualStates::HorizontalState);
	vsm->RegisterVisualState(UIVisualStates::OrientationGroup, UIVisualStates::VerticalState);

	m_track = NewObject<UITrack>();
	m_track->SetBackground(Brush::Red);		// TODO:
	m_track->GetThumb()->SetSize(Size(16, 20));
	m_track->GetThumb()->SetBackground(Brush::Green);
	//m_track->SetViewportSize(Math::NaN);
	AddVisualChild(m_track);
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
		UpdateValue(newValue);
		//auto* e2 = static_cast<UIDragDeltaEventArgs*>(e);
		//UpdateValue(e2->horizontalChange, e2->verticalChange);

		//auto args = UIScrollEventArgs::Create(this, m_track->GetValue(), ScrollEventType::ThumbTrack);
		//RaiseEvent(ScrollEventId, this, args);

		//switch (m_track->GetOrientation())
		//{
		//case Orientation::Horizontal:

		//	break;
		//case Orientation::Vertical:
		//	break;
		//case Orientation::ReverseHorizontal:
		//case Orientation::ReverseVertical:
		//default:
		//	LN_NOTIMPLEMENTED();
		//	break;
		//}
	}
	else if (e->GetType() == UIThumb::DragCompletedEventId)
	{
		//auto args = UIScrollEventArgs::Create(this, m_track->GetValue(), ScrollEventType::EndScroll);
		//RaiseEvent(ScrollEventId, this, args);
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
void UISlider::UpdateValue(float value)
{
	float snappedValue = value;// SnapToTick(value);
	//float valueDelta = m_track->ValueFromDistance(horizontalDragDelta, verticalDragDelta);

	//float newValue = m_dragStartValue + valueDelta;
	SetValue(snappedValue);
	m_track->SetValue(snappedValue);
}

LN_NAMESPACE_END
