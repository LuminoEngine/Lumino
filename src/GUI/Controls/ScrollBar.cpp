/*
	・WPF
		ScrollBar
			Grid
				RepeatButton
				Track
				RepeatButton

		Track
			RepeatButton	上の余白
			Thumb
				ScrollChrome
			RepeatButton	下の余白

	つまみのサイズは Track.ArrangeOverride で決めていた。

	ScrollBar のスタイルとテンプレート
	https://msdn.microsoft.com/ja-jp/library/ms742173%28v=vs.110%29.aspx?f=255&MSPPError=-2147217396
	PART_Track がある。


	Track.Value は RangeBase.Value と TemplateBinding している。
	↓
	でも、そこまでしなくていい気がする。
	ScrollBar や Slider は "PART_Track" として子要素に Track があることを前提としている。
	(null は可能だが、実際そんな ScrollBar を作ることがあるのか・・・？)
	であれば、TemplateBinding しなくても ScrollBar 側から直接値を set してもそんなに問題ない。
*/
#include "../../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/Controls/ScrollBar.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// ScrollEventArgs
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Lumino::GUI::ScrollEventArgs, EventArgs);

//=============================================================================
// ScrollBar
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ScrollBar, Control);
LN_UI_ELEMENT_SUBCLASS_IMPL(ScrollBar);

const String ScrollBar::PART_TrackKeyName(_T("PART_Track"));

LN_DEFINE_PROPERTY_2(ScrollBar, float, ValueProperty, "Value", 0.0f, &ScrollBar::SetValue, &ScrollBar::GetValue);
LN_DEFINE_PROPERTY_ENUM_2(ScrollBar, Orientation, OrientationProperty, "Orientation", Orientation::Horizontal, &ScrollBar::SetOrientation, &ScrollBar::GetOrientation);

LN_DEFINE_ROUTED_EVENT(ScrollBar, ScrollEventArgs, ScrollEvent, "DragStarted", Scroll);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollBar* ScrollBar::Create(GUIManager* manager)
{
	auto obj = RefPtr<ScrollBar>::Create(manager);
	obj->InitializeComponent();
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollBar::ScrollBar(GUIManager* manager)
	: Control(manager)
	, m_value(0.0f)
	, m_minimum(0.0f)
	, m_maximum(1.0f)
	, m_orientation(Orientation::Horizontal)
	, m_scrollEvent()
	, m_track(NULL)
	, m_dragStartValue(0.0f)
{	
	// Register handler
	LN_REGISTER_ROUTED_EVENT_HANDLER(ScrollBar, DragEventArgs, Thumb::DragStartedEvent, Handler_Thumb_DragStarted);
	LN_REGISTER_ROUTED_EVENT_HANDLER(ScrollBar, DragEventArgs, Thumb::DragDeltaEvent, Handler_Thumb_DragDelta);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollBar::~ScrollBar()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollBar::PollingTemplateChildCreated(UIElement* newElement)
{
	// PART のキーに一致する名前を持つ要素が見つかったらメンバに保持しておく
	if (newElement->GetKeyName() == PART_TrackKeyName) {
		m_track = dynamic_cast<Track*>(newElement);
	}

	Control::PollingTemplateChildCreated(newElement);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollBar::UpdateValue(float horizontalDragDelta, float verticalDragDelta)
{
	float valueDelta = m_track->ValueFromDistance(horizontalDragDelta, verticalDragDelta);

	float newValue = m_dragStartValue/*GetValue()*/ + valueDelta;
	ChangeValue(newValue);
	RefPtr<ScrollEventArgs> args(m_manager->GetEventArgsPool()->Create<ScrollEventArgs>(newValue, ScrollEventType::ThumbTrack));
	RaiseEvent(ScrollEvent, this, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollBar::ChangeValue(float newValue/*, bool defer*/)
{
	if (m_isStandalone)
	{
		// × Track が TemplateBindingしたいので。
		// TODO: やっぱり SetValue() の中でやるようにしたほうが良いかも
		SetPropertyValue(ValueProperty, newValue);

		// TODO: OnValueChannged とか作って、そちらで行うべき。
		m_track->SetValue(newValue);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollBar::Handler_Thumb_DragStarted(DragEventArgs* e)
{
	m_dragStartValue = m_value;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollBar::Handler_Thumb_DragDelta(DragEventArgs* e)
{
	UpdateValue(e->XOffset, e->YOffset);
}

} // namespace GUI
} // namespace Lumino

