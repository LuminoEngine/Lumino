
#include "Internal.hpp"
#include <LuminoEngine/UI/UIScrollView.hpp>

namespace ln {

//==============================================================================
// UIScrollView

UIScrollView::UIScrollView()
{
}

void UIScrollView::init()
{
	UIControl::init();
}


//==============================================================================
// UIThumb

UIThumb::UIThumb()
	: m_lastScreenPosition()
	, m_isDragging(false)
{
}

UIThumb::~UIThumb()
{
}

void UIThumb::init()
{
	UIElement::init();
}

void UIThumb::onRoutedEvent(UIEventArgs* e)
{
	if (e->type() == UIEvents::MouseDownEvent)
	{
		if (!m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			Point pos = mouseEvent->getPosition();

			m_lastScreenPosition = pos;
			m_isDragging = true;
			retainCapture();

			auto args = UIDragDeltaEventArgs::create(this, UIEvents::ScrollDragStartedEvent, pos.x - m_lastScreenPosition.x, pos.y - m_lastScreenPosition.y);
			onDragStarted(args);

			e->handled = true;
			return;
		}
	}
	else if (e->type() == UIEvents::MouseUpEvent)
	{
		if (m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			Point pos = mouseEvent->getPosition();

			m_isDragging = false;
			releaseCapture();

			auto args = UIDragDeltaEventArgs::create(this, UIEvents::ScrollDragCompletedEvent, pos.x - m_lastScreenPosition.x, pos.y - m_lastScreenPosition.y);
			onDragCompleted(args);

			e->handled = true;
			return;
		}
	}
	else if (e->type() == UIEvents::MouseMoveEvent)
	{
		if (m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			Point pos = mouseEvent->getPosition();

			auto args = UIDragDeltaEventArgs::create(this, UIEvents::ScrollDragDeltaEvent, pos.x - m_lastScreenPosition.x, pos.y - m_lastScreenPosition.y);
			onDragDelta(args);

			e->handled = true;
			return;
		}
	}

	UIElement::onRoutedEvent(e);
}

void UIThumb::onDragStarted(UIDragDeltaEventArgs* e)
{
	raiseEvent(e);
}

void UIThumb::onDragDelta(UIDragDeltaEventArgs* e)
{
	raiseEvent(e);
}

void UIThumb::onDragCompleted(UIDragDeltaEventArgs* e)
{
	raiseEvent(e);
}

void UIThumb::onDragCanceled(UIDragDeltaEventArgs* e)
{
	raiseEvent(e);
}

} // namespace ln

