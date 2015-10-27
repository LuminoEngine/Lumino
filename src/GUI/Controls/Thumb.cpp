
#include "../Internal.h"
#include "../UIManagerImpl.h"
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/Controls/Thumb.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

//=============================================================================
// DragEventArgs
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ln::DragEventArgs, RoutedEventArgs);

//=============================================================================
// Thumb
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Thumb, Control);
LN_UI_ELEMENT_SUBCLASS_IMPL(Thumb);

// Register property
LN_PROPERTY_IMPLEMENT(Thumb, bool, IsDraggingProperty, "IsDragging", m_isDragging, PropertyMetadata(false));

// Register routed event
LN_ROUTED_EVENT_IMPLEMENT(Thumb, DragEventArgs, DragStartedEvent, "DragStarted", DragStarted);
LN_ROUTED_EVENT_IMPLEMENT(Thumb, DragEventArgs, DragDeltaEvent, "DragDelta", DragDelta);
LN_ROUTED_EVENT_IMPLEMENT(Thumb, DragEventArgs, DragCompletedEvent, "DragCompleted", DragCompleted);
LN_ROUTED_EVENT_IMPLEMENT(Thumb, DragEventArgs, DragCanceledEvent, "DragCanceled", DragCanceled);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ThumbPtr Thumb::Create()
{
	return ThumbPtr(internalCreateInstance(GetUIManager()), false);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Thumb::Thumb(GUIManagerImpl* manager)
	: Control(manager)
	, m_isDragging(false)
{
	// Register handler
	LN_REGISTER_ROUTED_EVENT_HANDLER(Thumb, MouseEventArgs, UIElement::MouseMoveEvent, Handler_MouseMove);
	LN_REGISTER_ROUTED_EVENT_HANDLER(Thumb, MouseEventArgs, UIElement::MouseDownEvent, Handler_MouseDown);
	LN_REGISTER_ROUTED_EVENT_HANDLER(Thumb, MouseEventArgs, UIElement::MouseUpEvent, Handler_MouseUp);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Thumb::~Thumb()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Thumb::CancelDrag()
{
	if (m_isDragging)
	{
		m_isDragging = false;
		ReleaseMouseCapture();

		// ドラッグキャンセルイベント
		RefPtr<DragEventArgs> args(m_manager->GetEventArgsPool()->Create<DragEventArgs>(
			m_lastScreenPosition.X,
			m_lastScreenPosition.Y), false);
		OnDragDelta(args);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Thumb::Handler_MouseMove(MouseEventArgs* e)
{
	if (m_isDragging)
	{
		// ドラッグ中イベント
		RefPtr<DragEventArgs> args(m_manager->GetEventArgsPool()->Create<DragEventArgs>(
			e->X - m_lastScreenPosition.X,
			e->Y - m_lastScreenPosition.Y), false);
		OnDragDelta(args);

		e->Handled = true;
		return;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Thumb::Handler_MouseDown(MouseEventArgs* e)
{
	if (!m_isDragging)
	{
		m_lastScreenPosition.Set(e->X, e->Y);
		m_isDragging = true;
		CaptureMouse();

		// ドラッグ開始イベント
		RefPtr<DragEventArgs> args(m_manager->GetEventArgsPool()->Create<DragEventArgs>(
			e->X - m_lastScreenPosition.X,
			e->Y - m_lastScreenPosition.Y), false);
		OnDragStarted(args);

		e->Handled = true;
		return;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Thumb::Handler_MouseUp(MouseEventArgs* e)
{
	if (m_isDragging)
	{
		m_isDragging = false;
		ReleaseMouseCapture();

		// ドラッグ終了イベント
		RefPtr<DragEventArgs> args(m_manager->GetEventArgsPool()->Create<DragEventArgs>(
			e->X - m_lastScreenPosition.X,
			e->Y - m_lastScreenPosition.Y), false);
		OnDragCompleted(args);

		e->Handled = true;
		return;
	}
}

//=============================================================================
// ThumbChrome
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ThumbChrome, UIElement);
LN_UI_ELEMENT_SUBCLASS_IMPL(ThumbChrome);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ThumbChromePtr ThumbChrome::Create()
{
	return ThumbChromePtr(internalCreateInstance(GetUIManager()), false);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ThumbChrome::ThumbChrome(GUIManagerImpl* manager)
	: UIElement(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ThumbChrome::~ThumbChrome()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ThumbChrome::OnApplyTemplate(CombinedLocalResource* localResource)
{
	m_bgFrameBrush = static_cast<TextureBrush*>(localResource->GetItem(_T("ThumbChromeBackgroundFrameBrush")));
	m_bgInnerBrush = static_cast<TextureBrush*>(localResource->GetItem(_T("ThumbChromeBackgroundInnerBrush")));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ThumbChrome::OnRender(RenderingContext* painter)
{
	RectF rect = m_finalLocalRect;

	// 枠
	painter->SetBrush(m_bgFrameBrush);
	painter->DrawFrameRectangle(rect, 16);

	// 内側
	rect.Inflate(-16, -16);
	painter->SetBrush(m_bgInnerBrush);
	painter->DrawRectangle(rect);
}


LN_NAMESPACE_GUI_END
LN_NAMESPACE_END

