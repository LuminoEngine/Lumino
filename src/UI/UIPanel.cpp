
#include "Internal.h"
#include <Lumino/UI/UIPanel.h>

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
	RectF childFinal(0, 0, finalSize);
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
void UIPanel::OnChildElementAdd(UIElement* element)
{
	LN_THROW(element->GetParent() == nullptr, InvalidOperationException, "the child elements of already other elements.");
	element->SetParent(this, GetOwnerLayoutView());
}

//------------------------------------------------------------------------------
void UIPanel::OnChildElementRemove(UIElement* element)
{
	element->SetParent(nullptr, nullptr);
}

LN_NAMESPACE_END
