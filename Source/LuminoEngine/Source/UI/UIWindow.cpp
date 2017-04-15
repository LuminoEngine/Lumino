
#include "Internal.h"
#include <Lumino/UI/UILayoutPanel.h>
#include <Lumino/UI/UIWindow.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UIWindow
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIWindow, UIControl)

//------------------------------------------------------------------------------
RefPtr<UIWindow> UIWindow::Create()
{
	return NewObject<UIWindow>();
}

//------------------------------------------------------------------------------
UIWindow::UIWindow()
	: m_dragStartWindowPosition()
	, m_dragStartLocalPosition()
	, m_isDragging(false)
{
}

//------------------------------------------------------------------------------
UIWindow::~UIWindow()
{
}

//------------------------------------------------------------------------------
void UIWindow::Initialize()
{
	UIControl::Initialize();
}

//------------------------------------------------------------------------------
void UIWindow::OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e)
{
	if (ev == UIElement::MouseDownEvent)
	{
		if (!m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			PointF pos = mouseEvent->GetPosition(GetVisualParent());

			m_dragStartWindowPosition = GetPosition();
			m_dragStartLocalPosition = pos;
			m_isDragging = true;
			CaptureMouse();

			e->handled = true;
			return;
		}
	}
	else if (ev == UIElement::MouseUpEvent)
	{
		if (m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			PointF pos = mouseEvent->GetPosition(GetVisualParent());

			m_isDragging = false;
			ReleaseMouseCapture();

			e->handled = true;
			return;
		}
	}
	else if (ev == UIElement::MouseMoveEvent)
	{
		if (m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			Vector2 diff = mouseEvent->GetPosition(GetVisualParent()) - m_dragStartLocalPosition;

			SetPosition(m_dragStartWindowPosition + diff);
			
			

			e->handled = true;
			return;
		}
	}

	UIElement::OnRoutedEvent(ev, e);
}

//------------------------------------------------------------------------------
void UIWindow::OnLayoutPanelChanged(UILayoutPanel* newPanel)
{
	if (newPanel != nullptr)
	{
		newPanel->SetStyleSubControlName(_T("UIWindow"), _T("ContentHost"));

		//newPanel->margin = ThicknessF(0, 30, 0, 0);
	}

	UIControl::OnLayoutPanelChanged(newPanel);
}

LN_NAMESPACE_END
