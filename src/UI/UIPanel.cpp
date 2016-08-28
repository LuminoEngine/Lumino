
#include "Internal.h"
#include <Lumino/UI/UIPanel.h>
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIPanel
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIPanel, UIElement)

//------------------------------------------------------------------------------
UIPanel::UIPanel()
{
}

//------------------------------------------------------------------------------
UIPanel::~UIPanel()
{
}

//------------------------------------------------------------------------------
void UIPanel::Initialize(detail::UIManager* manager)
{
	UIElement::Initialize(manager);
	m_children = RefPtr<UIElementCollection>::MakeRef(this);
}

//------------------------------------------------------------------------------
void UIPanel::AddChild(UIElement* element)
{
	m_children->Add(element);
}

//------------------------------------------------------------------------------
void UIPanel::RemoveChild(UIElement* element)
{
	m_children->Remove(element);
}

//------------------------------------------------------------------------------
int UIPanel::GetVisualChildrenCount() const
{
	return m_children->GetCount();
}

//------------------------------------------------------------------------------
UIElement* UIPanel::GetVisualChildOrderd(int index) const
{
	return m_children->GetAt(index);
}

//------------------------------------------------------------------------------
SizeF UIPanel::MeasureOverride(const SizeF& constraint)
{
	SizeF desiredSize = UIElement::MeasureOverride(constraint);
	for (UIElement* child : *m_children)
	{
		child->MeasureLayout(constraint);
		const SizeF& childDesiredSize = child->GetDesiredSize();

		desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
		desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
	}
	return desiredSize;
}

//------------------------------------------------------------------------------
SizeF UIPanel::ArrangeOverride(const SizeF& finalSize)
{
	for (UIElement* child : *m_children)
	{
		SizeF childDesiredSize = child->GetDesiredSize();
		childDesiredSize.width = std::max(finalSize.width, childDesiredSize.width);
		childDesiredSize.height = std::max(finalSize.height, childDesiredSize.height);
		child->ArrangeLayout(RectF(0, 0, childDesiredSize));
	}
	return finalSize;
}

//------------------------------------------------------------------------------
void UIPanel::OnChildCollectionChanged(const tr::ChildCollectionChangedArgs& e)
{
	// êVÇµÇ≠í«â¡Ç≥ÇÍÇΩÇ‡ÇÃÇΩÇø
	for (UIElement* element : e.newItems)
	{
		element->SetParent(this);
	}

	// çÌèúÇ≥ÇÍÇΩÇ‡ÇÃÇΩÇø
	for (UIElement* element : e.oldItems)
	{
		element->SetParent(nullptr);
	}
}


//==============================================================================
// UIStackPanel
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIStackPanel, UIPanel)

//------------------------------------------------------------------------------
UIStackPanel::UIStackPanel()
{
}

//------------------------------------------------------------------------------
UIStackPanel::~UIStackPanel()
{
}

//------------------------------------------------------------------------------
void UIStackPanel::Initialize(detail::UIManager* manager)
{
	UIPanel::Initialize(manager);
}

//------------------------------------------------------------------------------
SizeF UIStackPanel::MeasureOverride(const SizeF& constraint)
{
	SizeF size = constraint;

	if (m_orientation == Orientation::Horizontal)
	{
		// â°Ç…ï¿Ç◊ÇÈèÍçáÅAïùÇÃêßå¿Çê›ÇØÇ»Ç¢
		size.width = std::numeric_limits<float>::infinity();
	}
	else
	{
		// ècÇ…ï¿Ç◊ÇÈèÍçáÅAçÇÇ≥ÇÃêßå¿Çê›ÇØÇ»Ç¢
		size.height = std::numeric_limits<float>::infinity();
	}

	SizeF desiredSize;
	for (UIElement* child : *GetChildren())
	{
		child->MeasureLayout(size);

		const SizeF& childDesiredSize = child->GetDesiredSize();
		if (m_orientation == Orientation::Horizontal)
		{
			desiredSize.width += childDesiredSize.width;
			desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
		}
		else
		{
			desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
			desiredSize.height += child->GetDesiredSize().height;
		}
	}

	return desiredSize;
}

//------------------------------------------------------------------------------
SizeF UIStackPanel::ArrangeOverride(const SizeF& finalSize)
{
	float prevChildSize = 0;
	RectF childRect;
	for (UIElement* child : *GetChildren())
	{
		const SizeF& childDesiredSize = child->GetDesiredSize();
		if (m_orientation == Orientation::Horizontal)
		{
			childRect.x += prevChildSize;
			prevChildSize = childDesiredSize.width;
			childRect.width = prevChildSize;
			childRect.height = finalSize.height;//std::min(finalSize.Height, childDesiredSize.Height);
		}
		else
		{
			childRect.y += prevChildSize;
			prevChildSize = childDesiredSize.height;
			childRect.height = prevChildSize;
			childRect.width = finalSize.width;// std::min(finalSize.Width, childDesiredSize.Width);
		}

		child->ArrangeLayout(childRect);
	}

	return finalSize;
}


//==============================================================================
// UICanvas
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UICanvas, UIPanel)

//------------------------------------------------------------------------------
UICanvasPtr UICanvas::Create()
{
	auto ptr = UICanvasPtr::MakeRef();
	ptr->Initialize(detail::UIManager::GetInstance());
	return ptr;
}

//------------------------------------------------------------------------------
UICanvas::UICanvas()
{
}

//------------------------------------------------------------------------------
UICanvas::~UICanvas()
{
}

//------------------------------------------------------------------------------
void UICanvas::Initialize(detail::UIManager* manager)
{
	UIPanel::Initialize(manager);
}

//------------------------------------------------------------------------------
SizeF UICanvas::MeasureOverride(const SizeF& constraint)
{
	return UIPanel::MeasureOverride(constraint);
}

//------------------------------------------------------------------------------
SizeF UICanvas::ArrangeOverride(const SizeF& finalSize)
{
	for (UIElement* child : *GetChildren())
	{
		RectF childRect(child->GetPositionInternal(), child->GetSizeInternal());
		child->ArrangeLayout(childRect);
	}

	return finalSize;
}

LN_NAMESPACE_END
