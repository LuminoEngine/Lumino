
#include "Internal.h"
#include <Lumino/Rendering/DrawingContext.h>
#include <Lumino/UI/UILayoutPanel.h>
#include <Lumino/UI/UIWindow.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UIWindow
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIWindow, UIControl)

//------------------------------------------------------------------------------
Ref<UIWindow> UIWindow::create()
{
	return newObject<UIWindow>();
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
void UIWindow::initialize()
{
	UIControl::initialize();
}

//------------------------------------------------------------------------------
void UIWindow::onRoutedEvent(UIEventArgs* e)
{
	if (e->getType() == UIEvents::MouseDownEvent)
	{
		if (!m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			Point pos = mouseEvent->getPosition(getVisualParent());

			m_dragStartWindowPosition = getPosition();
			m_dragStartLocalPosition = pos;
			m_isDragging = true;
			captureMouse();

			e->handled = true;
			return;
		}
	}
	else if (e->getType() == UIEvents::MouseUpEvent)
	{
		if (m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			Point pos = mouseEvent->getPosition(getVisualParent());

			m_isDragging = false;
			releaseMouseCapture();

			e->handled = true;
			return;
		}
	}
	else if (e->getType() == UIEvents::MouseMoveEvent)
	{
		if (m_isDragging)
		{
			auto mouseEvent = static_cast<UIMouseEventArgs*>(e);
			Vector2 diff = mouseEvent->getPosition(getVisualParent()) - m_dragStartLocalPosition;

			setPosition(m_dragStartWindowPosition + diff);
			
			

			e->handled = true;
			return;
		}
	}

	UIElement::onRoutedEvent(e);
}

//------------------------------------------------------------------------------
void UIWindow::onLayoutPanelChanged(UILayoutPanel* newPanel)
{
	if (newPanel != nullptr)
	{
		newPanel->setStyleSubControlName(_T("UIWindow"), _T("ContentHost"));

		newPanel->setMargin(Thickness(0, 30, 0, 0));
	}

	UIControl::onLayoutPanelChanged(newPanel);
}

//------------------------------------------------------------------------------
void UIWindow::onRender(DrawingContext* g)
{
	UIControl::onRender(g);
	//g->setBlendMode(BlendMode::Alpha);
	////g->setBrush(Brush::White);
	//////g->drawBoxBorder(
	//////	RectF(10, 10, 300, 200), Thickness(3, 3, 3, 3), Color::Red, Color::Green, Color::Blue, Color::Cyan, 0, 10, 20, 30,
	//////	Color::Gray, 5, 5, false);	// TODO:

	////Color c = Color::LightSkyBlue;
	////Color c2 = Color::Gray;
	////c.a = 0.25;
	////c2.a = 0.25;
	//g->drawBoxBorder(
	//	RectF(10, 10, 200, 50), Thickness(1, 2, 4, 8), CornerRadius(0, 0, 0, 0),
	//	Color::LightSkyBlue, Color::LightSkyBlue, Color::LightSkyBlue, Color::LightSkyBlue,
	//	BorderDirection::Outside);	// TODO:
	//g->drawBoxBorder(
	//	RectF(250, 10, 200, 50), Thickness(1, 1, 1, 1), CornerRadius(10, 0, 10, 20),
	//	Color::LightSkyBlue, Color::LightSkyBlue, Color::LightSkyBlue, Color::LightSkyBlue,
	//	BorderDirection::Outside);	// TODO:

	//g->drawBoxShadow(
	//	RectF(10, 100, 200, 50), CornerRadius(0, 20, 0, 0),
	//	Color::LightSkyBlue, 5, 5,
	//	ShadowDirection::Outside);	// TODO:

	//g->drawBoxBorder(
	//	RectF(10, 100, 300, 50), Thickness(1, 1, 1, 1),
	//	Color::LawnGreen, Color::LightSkyBlue, Color::Gray, Color::OrangeRed,
	//	30, 0, 0, 30, BorderDirection::Inside,
	//	c2, 5, 5, ShadowDirection::Outside);	// TODO:

	//g->drawBoxBorder(
	//	RectF(10, 200, 300, 50), Thickness(1, 1, 1, 1),
	//	Color::OrangeRed, Color::OrangeRed, Color::OrangeRed, Color::OrangeRed,
	//	20, 20, 20, 20, BorderDirection::Inside,
	//	Color::OrangeRed.withAlpha(0.25), 5, 5, ShadowDirection::Outside);	// TODO:

	//g->drawBoxBorder(
	//	RectF(10, 70, 300, 50), Thickness(8, 2, 8, 8),
	//	Color::LightSkyBlue, Color::BlueViolet, Color::Orange, Color::GreenYellow,
	//	30, 0, 0, 30, BorderDirection::Inside,
	//	c2, 5, 5, ShadowDirection::Outside);	// TODO:

}

LN_NAMESPACE_END
