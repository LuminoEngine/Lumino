
#include "../../Internal.h"
#include <Lumino/Property.h>
#include <Lumino/GUI/Controls/Grid.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// Grid
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Grid);
LN_UI_ELEMENT_SUBCLASS_IMPL(Grid);

AttachedProperty*	Grid::ColumnProperty = NULL;
AttachedProperty*	Grid::ColumnSpanProperty = NULL;
AttachedProperty*	Grid::RowProperty = NULL;
AttachedProperty*	Grid::RowSpanProperty = NULL;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Grid::Grid(GUIManager* manager)
	: Panel(manager)
{
	// Register attached property
	LN_DEFINE_ATTACHED_PROPERTY(ColumnProperty,		"Column",		int, Grid, 0);
	LN_DEFINE_ATTACHED_PROPERTY(ColumnSpanProperty,	"ColumnSpan",	int, Grid, 0);
	LN_DEFINE_ATTACHED_PROPERTY(RowProperty,		"Row",			int, Grid, 0);
	LN_DEFINE_ATTACHED_PROPERTY(RowSpanProperty,	"RowSpan",		int, Grid, 0);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Grid::~Grid()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Grid::MeasureLayout(const SizeF& availableSize)
{
	// this ÇÃ m_desiredSize Çê›íËÇ∑ÇÈ
	//Panel::MeasureLayout(desiredSize);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Grid::ArrangeLayout(const RectF& finalRect)
{
	//Panel::ArrangeLayout(finalRect);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void	Grid::SetColumn(UIElement* element, int value)		{ LN_SET_ATTACHED_PROPERTY(element, ColumnProperty, value); }
int		Grid::GetColumn(UIElement* element)					{ LN_GET_ATTACHED_PROPERTY(element, ColumnProperty, int); }
void	Grid::SetColumnSpan(UIElement* element, int value)	{ LN_SET_ATTACHED_PROPERTY(element, ColumnSpanProperty, value); }
int		Grid::GetColumnSpan(UIElement* element)				{ LN_GET_ATTACHED_PROPERTY(element, ColumnSpanProperty, int); }
void	Grid::SetRow(UIElement* element, int value)			{ LN_SET_ATTACHED_PROPERTY(element, RowProperty, value); }
int		Grid::GetRow(UIElement* element)					{ LN_GET_ATTACHED_PROPERTY(element, RowProperty, int); }
void	Grid::SetRowSpan(UIElement* element, int value)		{ LN_SET_ATTACHED_PROPERTY(element, RowSpanProperty, value); }
int		Grid::GetRowSpan(UIElement* element)				{ LN_GET_ATTACHED_PROPERTY(element, RowSpanProperty, int); }

//=============================================================================
// ColumnDefinition
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ColumnDefinition);
LN_UI_ELEMENT_SUBCLASS_IMPL(ColumnDefinition);

const PropertyID	ColumnDefinition::WidthProperty(_T("Width"));
const PropertyID	ColumnDefinition::MinWidthProperty(_T("MinWidth"));
const PropertyID	ColumnDefinition::MaxWidthProperty(_T("MaxWidth"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ColumnDefinition::ColumnDefinition(GUIManager* manager)
	: ContentElement(manager)
{
	// Register property
	LN_DEFINE_PROPERTY(ColumnDefinition, float, WidthProperty, &ColumnDefinition::SetWidth, &ColumnDefinition::GetWidth, NAN);
	LN_DEFINE_PROPERTY(ColumnDefinition, float, MinWidthProperty, &ColumnDefinition::SetMinWidth, &ColumnDefinition::GetMinWidth, NAN);
	LN_DEFINE_PROPERTY(ColumnDefinition, float, MaxWidthProperty, &ColumnDefinition::SetMaxWidth, &ColumnDefinition::GetMaxWidth, NAN);
}

//=============================================================================
// RowDefinition
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(RowDefinition);
LN_UI_ELEMENT_SUBCLASS_IMPL(RowDefinition);

const PropertyID	RowDefinition::HeightProperty(_T("Height"));
const PropertyID	RowDefinition::MinHeightProperty(_T("MinHeight"));
const PropertyID	RowDefinition::MaxHeightProperty(_T("MaxHeight"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RowDefinition::RowDefinition(GUIManager* manager)
	: ContentElement(manager)
{	
	// Register property
	LN_DEFINE_PROPERTY(RowDefinition, float, HeightProperty, &RowDefinition::SetHeight, &RowDefinition::GetHeight, NAN);
	LN_DEFINE_PROPERTY(RowDefinition, float, MinHeightProperty, &RowDefinition::SetMinHeight, &RowDefinition::GetMinHeight, NAN);
	LN_DEFINE_PROPERTY(RowDefinition, float, MaxHeightProperty, &RowDefinition::SetMaxHeight, &RowDefinition::GetMaxHeight, NAN);

}

} // namespace GUI
} // namespace Lumino

