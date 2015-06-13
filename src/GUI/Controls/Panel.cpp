
#include "../../Internal.h"
#include <Lumino/GUI/Controls/Panel.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// Panel
//=============================================================================

LN_CORE_OBJECT_TYPE_INFO_IMPL(Panel);
LN_UI_ELEMENT_SUBCLASS_IMPL(Panel);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Panel::Panel(GUIManager* manager)
	: UIElement(manager)
	, m_children(LN_NEW UIElementCollection(this))
{
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
bool Panel::OnEvent(EventType type, EventArgs* args)
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


} // namespace GUI
} // namespace Lumino

