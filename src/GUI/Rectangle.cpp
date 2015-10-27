
#include "Internal.h"
#include <Lumino/GUI/Rectangle.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

//=============================================================================
// Shape
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Shape, UIElement);
LN_UI_ELEMENT_SUBCLASS_IMPL(Shape);

// Register property
LN_PROPERTY_IMPLEMENT(Shape, Brush*, FillBrushProperty, "FillBrush", m_fillBrush, PropertyMetadata(NULL));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Shape::Shape(GUIManagerImpl* manager)
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
	return internalCreateInstance(GetUIManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Rectangle::Rectangle(GUIManagerImpl* manager)
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
void Rectangle::OnRender(RenderingContext* painter)
{
	if (m_fillBrush != NULL)
	{
		painter->SetBrush(m_fillBrush);
		painter->DrawRectangle(RectF(0, 0, GetRenderSize()));
	}
}

LN_NAMESPACE_GUI_END
LN_NAMESPACE_END

