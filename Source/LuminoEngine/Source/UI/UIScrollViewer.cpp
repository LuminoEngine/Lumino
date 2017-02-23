
#include "Internal.h"
#include <Lumino/UI/UIScrollViewer.h>
#include "UIManager.h"
#include "EventArgsPool.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIDragDeltaEventArgs
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIDragDeltaEventArgs, UIEventArgs)

//==============================================================================
// UIThumb
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIThumb, UIElement)
LN_ROUTED_EVENT_IMPLEMENT(UIThumb, UIDragDeltaEventArgs, DragStartedEvent, "DragStarted", DragStarted);
LN_ROUTED_EVENT_IMPLEMENT(UIThumb, UIDragDeltaEventArgs, DragDeltaEvent, "DragDelta", DragDelta);
LN_ROUTED_EVENT_IMPLEMENT(UIThumb, UIDragDeltaEventArgs, DragCompletedEvent, "DragCompleted", DragCompleted);
LN_ROUTED_EVENT_IMPLEMENT(UIThumb, UIDragDeltaEventArgs, DragCanceledEvent, "DragCanceled", DragCanceled);

//------------------------------------------------------------------------------
RefPtr<UIThumb> UIThumb::Create()
{
	auto ptr = RefPtr<UIThumb>::MakeRef();
	ptr->Initialize(ln::detail::UIManager::GetInstance());
	return ptr;
}

//------------------------------------------------------------------------------
UIThumb::UIThumb()
	: m_lastScreenPosition()
	, m_isDragging(false)
{
}

//------------------------------------------------------------------------------
UIThumb::~UIThumb()
{
}

//------------------------------------------------------------------------------
void UIThumb::Initialize(detail::UIManager* manager)
{
	UIElement::Initialize(manager);
}

//------------------------------------------------------------------------------
void UIThumb::OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e)
{
	if (ev == UIElement::MouseDownEvent)
	{
		if (!m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			PointF pos = mouseEvent->GetPosition(GetParent());

			m_lastScreenPosition = pos;
			m_isDragging = true;
			CaptureMouse();

			// ドラッグ開始イベント
			detail::EventArgsPool* pool = GetManager()->GetEventArgsPool();
			RefPtr<UIDragDeltaEventArgs> args(pool->Create<UIDragDeltaEventArgs>(), false);
			args->horizontalChange = pos.x - m_lastScreenPosition.x;
			args->verticalChange = pos.y - m_lastScreenPosition.y;
			OnDragStarted(args);

			e->handled = true;
			return;
		}
	}
	else if (ev == UIElement::MouseUpEvent)
	{
		if (m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			PointF pos = mouseEvent->GetPosition(GetParent());

			m_isDragging = false;
			ReleaseMouseCapture();

			// ドラッグ終了イベント
			detail::EventArgsPool* pool = GetManager()->GetEventArgsPool();
			RefPtr<UIDragDeltaEventArgs> args(pool->Create<UIDragDeltaEventArgs>(), false);
			args->horizontalChange = pos.x - m_lastScreenPosition.x;
			args->verticalChange = pos.y - m_lastScreenPosition.y;
			OnDragCompleted(args);

			e->handled = true;
			return;
		}
	}
	else if (ev == UIElement::MouseMoveEvent)
	{
		if (m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			PointF pos = mouseEvent->GetPosition(GetParent());

			// ドラッグ中イベント
			detail::EventArgsPool* pool = GetManager()->GetEventArgsPool();
			RefPtr<UIDragDeltaEventArgs> args(pool->Create<UIDragDeltaEventArgs>(), false);
			args->horizontalChange = pos.x - m_lastScreenPosition.x;
			args->verticalChange = pos.y - m_lastScreenPosition.y;
			OnDragDelta(args);

			e->handled = true;
			return;
		}
	}

	UIElement::OnRoutedEvent(ev, e);
}

//==============================================================================
// UITrack
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UITrack, UIElement)

//------------------------------------------------------------------------------
RefPtr<UITrack> UITrack::Create()
{
	auto ptr = RefPtr<UITrack>::MakeRef();
	ptr->Initialize(ln::detail::UIManager::GetInstance());
	return ptr;
}

//------------------------------------------------------------------------------
UITrack::UITrack()
{
}

//------------------------------------------------------------------------------
UITrack::~UITrack()
{
}

//------------------------------------------------------------------------------
void UITrack::Initialize(detail::UIManager* manager)
{
	UIElement::Initialize(manager);
}

//==============================================================================
// UIScrollBar
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIScrollBar, UIElement)

//------------------------------------------------------------------------------
RefPtr<UIScrollBar> UIScrollBar::Create()
{
	auto ptr = RefPtr<UIScrollBar>::MakeRef();
	ptr->Initialize(ln::detail::UIManager::GetInstance());
	return ptr;
}

//------------------------------------------------------------------------------
UIScrollBar::UIScrollBar()
{
}

//------------------------------------------------------------------------------
UIScrollBar::~UIScrollBar()
{
}

//------------------------------------------------------------------------------
void UIScrollBar::Initialize(detail::UIManager* manager)
{
	UIElement::Initialize(manager);
}

LN_NAMESPACE_END
