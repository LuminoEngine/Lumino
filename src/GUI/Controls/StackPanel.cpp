
#include "../Internal.h"
#include <Lumino/Property.h>
#include <Lumino/GUI/Controls/StackPanel.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

//=============================================================================
// StackPanel
//=============================================================================

LN_CORE_OBJECT_TYPE_INFO_IMPL(StackPanel, Panel);
LN_UI_ELEMENT_SUBCLASS_IMPL(StackPanel);

LN_PROPERTY_IMPLEMENT(StackPanel, Orientation, OrientationProperty, "Orientation", m_orientation, PropertyMetadata(Orientation::Horizontal));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
StackPanel* StackPanel::Create()
{
	return internalCreateInstance(GetUIManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
StackPanel::StackPanel(GUIManagerImpl* manager)
	: Panel(manager)
{
	std::is_class<int>();
	// プロパティの登録
//	LN_DEFINE_PROPERTY_ENUM(StackPanel, Orientation, OrientationProperty, &StackPanel::SetOrientation, &StackPanel::GetOrientation, Orientation::Horizontal);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
StackPanel::~StackPanel()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF StackPanel::MeasureOverride(const SizeF& constraint)
{
	SizeF size = constraint;

	if (m_orientation == Orientation::Horizontal)
	{
		// 横に並べる場合、幅の制限を設けない
		size.Width = std::numeric_limits<float>::infinity();
	}
	else
	{
		// 縦に並べる場合、高さの制限を設けない
		size.Height = std::numeric_limits<float>::infinity();
	}

	SizeF desiredSize;
	for (UIElement* child : *m_children)
	{
		child->MeasureLayout(size);

		const SizeF& childDesiredSize = child->GetDesiredSize();
		if (m_orientation == Orientation::Horizontal)
		{
			desiredSize.Width += childDesiredSize.Width;
			desiredSize.Height = std::max(desiredSize.Height, childDesiredSize.Height);
		}
		else
		{
			desiredSize.Width = std::max(desiredSize.Width, childDesiredSize.Width);
			desiredSize.Height += child->GetDesiredSize().Height;
		}
	}

	return desiredSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF StackPanel::ArrangeOverride(const SizeF& finalSize)
{
	float prevChildSize = 0;
	RectF childRect;
	for (UIElement* child : *m_children)
	{
		const SizeF& childDesiredSize = child->GetDesiredSize();
		if (m_orientation == Orientation::Horizontal)
		{
			childRect.X += prevChildSize;
			prevChildSize = childDesiredSize.Width;
			childRect.Width = prevChildSize;
			childRect.Height = std::min(finalSize.Height, childDesiredSize.Height);
		}
		else
		{
			childRect.Y += prevChildSize;
			prevChildSize = childDesiredSize.Height;
			childRect.Height = prevChildSize;
			childRect.Width = std::min(finalSize.Width, childDesiredSize.Width);
		}

		child->ArrangeLayout(childRect);
	}

	return finalSize;
}

LN_NAMESPACE_GUI_END
LN_NAMESPACE_END

