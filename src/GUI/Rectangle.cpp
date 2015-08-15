
#include "../Internal.h"
#include <Lumino/GUI/Rectangle.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// Shape
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Shape, UIElement);
LN_UI_ELEMENT_SUBCLASS_IMPL(Shape);

// Register property
LN_PROPERTY_IMPLEMENT(Shape, Graphics::Brush*, FillBrushProperty, "FillBrush", m_fillBrush, NULL, NULL);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Shape::Shape(GUIManager* manager)
	: UIElement(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Shape::~Shape()
{
}


//=============================================================================
// Rectangle
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Rectangle, Shape);
LN_UI_ELEMENT_SUBCLASS_IMPL(Rectangle);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Rectangle* Rectangle::Create()
{
	return internalCreateInstance(ApplicationContext::GetGUIManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Rectangle::Rectangle(GUIManager* manager)
	: Shape(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Rectangle::~Rectangle()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Rectangle::OnRender(Graphics::Painter* painter)
{
	if (m_fillBrush != NULL)
	{
		painter->SetBrush(m_fillBrush);
		painter->DrawRectangle(RectF(0, 0, GetRenderSize()));
	}
}

} // namespace GUI
} // namespace Lumino

