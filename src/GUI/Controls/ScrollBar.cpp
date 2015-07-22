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

	float newValue = GetValue() + valueDelta;
	ChangeValue(newValue);
	RefPtr<ScrollEventArgs> args(m_manager->GetEventArgsPool()->Create<ScrollEventArgs>(newValue));
	RaiseEvent(ScrollEvent, this, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollBar::ChangeValue(float newValue/*, bool defer*/)
{
	//if (IsStandalone)
	{
		SetValue(newValue);
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

