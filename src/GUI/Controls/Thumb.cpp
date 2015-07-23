
#include "../../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/Controls/Thumb.h>

namespace Lumino
{
namespace GUI
{
//=============================================================================
// DragEventArgs
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Lumino::GUI::DragEventArgs, EventArgs);

//=============================================================================
// Thumb
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Thumb, Control);
LN_UI_ELEMENT_SUBCLASS_IMPL(Thumb);

// Register property
LN_DEFINE_PROPERTY_2(Thumb, bool, IsDraggingProperty, "IsDragging", false, NULL, &Thumb::IsDragging);
//PropertyID	Thumb::IsDraggingProperty(_T("IsDragging"));

//EventID	 	Thumb::DragStartedEvent(_T("DragStarted"));
//EventID		Thumb::DragDeltaEvent(_T("DragDelta"));
//EventID		Thumb::DragCompletedEvent(_T("DragCompleted"));
//EventID		Thumb::DragCanceledEvent(_T("DragCanceled"));
LN_DEFINE_ROUTED_EVENT(Thumb, DragEventArgs, DragStartedEvent, "DragStarted", DragStarted);
LN_DEFINE_ROUTED_EVENT(Thumb, DragEventArgs, DragDeltaEvent, "DragDelta", DragDelta);
LN_DEFINE_ROUTED_EVENT(Thumb, DragEventArgs, DragCompletedEvent, "DragCompleted", DragCompleted);
LN_DEFINE_ROUTED_EVENT(Thumb, DragEventArgs, DragCanceledEvent, "DragCanceled", DragCanceled);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Thumb* Thumb::Create(GUIManager* manager)
{
	auto obj = RefPtr<Thumb>::Create(manager);
	obj->InitializeComponent();
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Thumb::Thumb(GUIManager* manager)
	: Control(manager)
	, m_isDragging(false)
{
	// Register property
	//LN_DEFINE_PROPERTY(Thumb, bool, IsDraggingProperty, NULL, &Thumb::IsDragging, false);

	// Register event
	//LN_DEFINE_ROUTED_EVENT(Thumb, DragEventArgs, DragStartedEvent, &Thumb::OnDragStarted, [](UIElement* t, CoreObject* s, DragEventArgs* e){ t->MouseMove.Raise(s, e); });
	//LN_DEFINE_ROUTED_EVENT(Thumb, DragEventArgs, DragDeltaEvent, &Thumb::OnDragDelta, [](UIElement* t, CoreObject* s, DragEventArgs* e){ t->MouseDown.Raise(s, e); });

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
			m_lastScreenPosition.Y));
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
			e->Y - m_lastScreenPosition.Y));
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
			e->Y - m_lastScreenPosition.Y));
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
			e->Y - m_lastScreenPosition.Y));
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
ThumbChrome* ThumbChrome::Create(GUIManager* manager)
{
	auto obj = RefPtr<ThumbChrome>::Create(manager);
	obj->InitializeComponent();
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ThumbChrome::ThumbChrome(GUIManager* manager)
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
	m_bgFrameBrush = static_cast<Graphics::TextureBrush*>(localResource->GetItem(_T("ThumbChromeBackgroundFrameBrush")));
	m_bgInnerBrush = static_cast<Graphics::TextureBrush*>(localResource->GetItem(_T("ThumbChromeBackgroundInnerBrush")));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ThumbChrome::OnRender(Graphics::Painter* painter)
{
	RectF rect = m_finalGlobalRect;

	// 枠
	painter->SetBrush(m_bgFrameBrush);
	painter->DrawFrameRectangle(rect, 16);

	// 内側
	rect.Inflate(-16, -16);
	painter->SetBrush(m_bgInnerBrush);
	painter->DrawRectangle(rect);
}


} // namespace GUI
} // namespace Lumino

