
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
Ref<UISlider> UISlider::create()
{
	return newObject<UISlider>();
}

//------------------------------------------------------------------------------
Ref<UISlider> UISlider::create(float value, float minimum, float maximum)
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
	auto* vsm = getVisualStateManager();
	vsm->registerVisualState(UIVisualStates::OrientationGroup, UIVisualStates::HorizontalState);
	vsm->registerVisualState(UIVisualStates::OrientationGroup, UIVisualStates::VerticalState);

	m_track = newObject<UITrack>();
	m_track->setStyleSubControlName(_LT("UISlider"), _LT("Track"));
	m_track->getThumb()->setStyleSubControlName(_LT("UISlider"), _LT("Thumb"));
	m_track->getDecreaseButton()->setStyleSubControlName(_LT("UISlider"), _LT("DecreaseButton"));
	m_track->getIncreaseButton()->setStyleSubControlName(_LT("UISlider"), _LT("IncreaseButton"));
	m_track->setViewportSize(Math::NaN);	// 値の計算に slider モードを使用する
	addVisualChild(m_track);
}

//------------------------------------------------------------------------------
void UISlider::initialize(float value, float minimum, float maximum)
{
	initialize();
	setValue(value);
	setMinimum(minimum);
	setMaximum(maximum);
}

//------------------------------------------------------------------------------
void UISlider::setOrientation(Orientation orientation)
{
	m_track->setOrientation(orientation);

	switch (orientation)
	{
	case Orientation::Horizontal:
		goToVisualState(UIVisualStates::HorizontalState);
		break;
	case Orientation::Vertical:
		goToVisualState(UIVisualStates::VerticalState);
		break;
	default:
		LN_NOTIMPLEMENTED();
		break;
	}
}

//------------------------------------------------------------------------------
Orientation UISlider::getOrientation() const
{
	return m_track->getOrientation();
}

//------------------------------------------------------------------------------
void UISlider::onRoutedEvent(UIEventArgs* e)
{
	if (e->getType() == UIThumb::DragStartedEventId)
	{
		m_dragStartValue = m_track->getValue();
	}
	else if (e->getType() == UIThumb::DragDeltaEventId)
	{
		auto* e2 = static_cast<UIDragDeltaEventArgs*>(e);
		float newValue = m_dragStartValue + m_track->valueFromDistance(e2->horizontalChange, e2->verticalChange);
		updateValue(Math::clamp(newValue, getMinimum(), getMaximum()));
	}
	else if (e->getType() == UIThumb::DragCompletedEventId)
	{
	}
	UIControl::onRoutedEvent(e);
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
	Orientation orientation = getOrientation();

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
void UISlider::onValueChanged(float oldValue, float newValue)
{
	m_track->setValue(newValue);
}

//------------------------------------------------------------------------------
void UISlider::onMinimumChanged(float oldMinimum, float newMinimum)
{
	m_track->setMinimum(newMinimum);
}

//------------------------------------------------------------------------------
void UISlider::onMaximumChanged(float oldMaximum, float newMaximum)
{
	m_track->setMaximum(newMaximum);
}

//------------------------------------------------------------------------------
void UISlider::updateValue(float value)
{
	float snappedValue = value;
	setValue(snappedValue);
}

LN_NAMESPACE_END
