
#include "../Internal.h"
#include <Lumino/GUI/Controls/Panel.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

//=============================================================================
// Panel
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Panel, UIElement);
LN_UI_ELEMENT_SUBCLASS_IMPL(Panel);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Panel::Panel(GUIManagerImpl* manager)
	: UIElement(manager)
	, m_children(LN_NEW UIElementCollection(), false)
{
	m_children->ItemAdded += CreateDelegate(this, &Panel::Children_ItemAdded);
	m_children->ItemRemoved += CreateDelegate(this, &Panel::Children_ItemRemoved);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Panel::~Panel()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Panel::AddChild(const Variant& value)
{
	UIElement* item = Variant::Cast<UIElement*>(value);	// TODO: Cast() 無いで型チェックしたほうが良いかも
	m_children->Add(item);
	//LN_THROW(item->GetParent() == NULL, InvalidOperationException);	// 既に親要素があった
	//m_children->Add(item);
	//m_visualChildren.Add(item);
	//item->SetParent(this);
	//item->SetTemplateModified(true);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Panel::Children_ItemAdded(UIElement* item)
{
	// TODO: ここは AddVisualChild ではなくて、GeVisualChild() をオーバーライドする。WPF はそうなっている。
	AddVisualChild(item);
	m_orderdVisualChildren.Add(item);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Panel::Children_ItemRemoved(UIElement* item)
{
	RemoveVisualChild(item);
	m_orderdVisualChildren.Remove(item);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* Panel::GetVisualChildOrderd(int index) const
{
	return m_orderdVisualChildren[index];
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Panel::ActivateInternal(UIElement* child)
{
	// オーダーの末尾へ　TODO: Zオーダー考慮
	if (m_orderdVisualChildren.Remove(child)) {
		m_orderdVisualChildren.Add(child);
	}

	UIElement::ActivateInternal(child);
}

#if 0
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* Panel::CheckMouseHoverElement(const PointF& globalPt)
{
	// 子要素を優先
	LN_FOREACH(UIElement* child, *m_children) {
		UIElement* e = child->CheckMouseHoverElement(globalPt);
		if (e != NULL) { return e; }
	}

	return UIElement::CheckMouseHoverElement(globalPt);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Panel::Render()
{
	OnRender();
	LN_FOREACH(UIElement* child, *m_children) {
		child->Render();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Panel::OnEvent(EventType type, RoutedEventArgs* args)
{
	LN_FOREACH(UIElement* child, *m_children) {
		if (child->OnEvent(type, args)) { return true; }
	}
	return UIElement::OnEvent(type, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Panel::ApplyTemplateHierarchy(CombinedLocalResource* parent)
{
	UIElement::ApplyTemplateHierarchy(parent);
	LN_FOREACH(UIElement* child, *m_children) {
		child->ApplyTemplateHierarchy(m_combinedLocalResource);	// 再帰的に更新する
	}
}
#endif


//=============================================================================
// PilePanel
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(PilePanel, Panel);
LN_UI_ELEMENT_SUBCLASS_IMPL(PilePanel);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PilePanel* PilePanel::Create()
{
	return internalCreateInstance(GetUIManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PilePanel::PilePanel(GUIManagerImpl* manager)
	: Panel(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PilePanel::~PilePanel()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF PilePanel::MeasureOverride(const SizeF& constraint)
{
	SizeF desiredSize = Panel::MeasureOverride(constraint);
	for (UIElement* child : *m_children)
	{
		child->MeasureLayout(constraint);
		const SizeF& childDesiredSize = child->GetDesiredSize();

		desiredSize.Width = std::max(desiredSize.Width, childDesiredSize.Width);
		desiredSize.Height = std::max(desiredSize.Height, childDesiredSize.Height);
	}
	return desiredSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF PilePanel::ArrangeOverride(const SizeF& finalSize)
{
	RectF childFinal(0, 0, finalSize);
	for (UIElement* child : *m_children)
	{
		SizeF childDesiredSize = child->GetDesiredSize();
		childDesiredSize.Width = std::max(finalSize.Width, childDesiredSize.Width);
		childDesiredSize.Height = std::max(finalSize.Height, childDesiredSize.Height);
		child->ArrangeLayout(RectF(0, 0, childDesiredSize));
	}
	return finalSize;
}

LN_NAMESPACE_GUI_END
LN_NAMESPACE_END

