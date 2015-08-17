
#include "../Internal.h"
#include <Lumino/GUI/Control.h>
#include "GUIHelper.h"

namespace Lumino
{
namespace GUI
{

//=============================================================================
// Control
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Control, UIElement);
LN_UI_ELEMENT_SUBCLASS_IMPL(Control);

// Register property
LN_PROPERTY_IMPLEMENT(Control, Graphics::BrushPtr, BackgroundProperty, "Background", m_background, NULL, NULL);
LN_PROPERTY_IMPLEMENT(Control, ControlTemplate*, TemplateProperty, "Template", m_controlTemplate, NULL, NULL);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Control::Control(GUIManager* manager)
	: UIElement(manager)
{
	m_visualStateGroupList.Attach(LN_NEW VisualStateGroupList());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Control::~Control()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Control::SetTemplate(ControlTemplate* controlTemplate)
{
	m_controlTemplate = controlTemplate;
	SetTemplateModified(true);
}

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//bool Control::ApplyTemplateInternal()
//{
//	return ApplyTemplateInternalMain(ControlTemplateTypeName);
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//bool Control::ApplyTemplateInternalMain(const String& typeName)
//{
//	ControlTemplate* t;
//	if (m_combinedLocalResource->TryGetControlTemplate(typeName, &t))
//	{
//		t->Apply(this);
//		return true;
//	}
//	return false;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Control::OnApplyTemplate(CombinedLocalResource* localResource)
{
	// ControlTemplate の適用処理 (WPF では ApplyTemplate で行っていた)
	if (m_controlTemplate != NULL) {
		m_controlTemplate->Apply(this);
	}

	//ControlTemplate* t;
	//if (localResource->TryGetControlTemplate(GetTypeID(), &t)) {
	//	t->Apply(this);
	//}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF Control::MeasureOverride(const SizeF& constraint)
{
	SizeF desiredSize;
	if (GUIHelper::SimpleMeasureOverrideSingleVisual(this, constraint, &desiredSize)) {
		return desiredSize;
	}
	return UIElement::MeasureOverride(constraint);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF Control::ArrangeOverride(const SizeF& finalSize)
{
	if (GUIHelper::SimpleArrangeOverrideSingleVisual(this, finalSize)) {
		return finalSize;
	}
	return UIElement::ArrangeOverride(finalSize);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Control::OnRender(Graphics::Painter* painter)
{
	UIElement::OnRender(painter);

	if (m_background != NULL) {
		painter->SetBrush(m_background);
		painter->DrawRectangle(RectF(0, 0, GetRenderSize()));
	}
}

//void Control::ApplyTemplateHierarchy(CombinedLocalResource* parent)
//{
//	// ローカルリソースを更新する
//	if (m_combinedLocalResource != NULL && parent != m_combinedLocalResource) {
//		m_combinedLocalResource->Combine(parent, m_localResource);
//	}
//	else {
//		LN_REFOBJ_SET(m_combinedLocalResource, parent);
//	}
//
//	// ControlTemplate の適用処理
//	ControlTemplate* t;
//	if (m_combinedLocalResource->TryGetControlTemplate(GetTypeID(), &t)) {
//		t->Apply(this);
//	}
//
//	OnApplyTemplate(m_combinedLocalResource);
//
//	// 子要素
//	if (m_templateChild != NULL) {
//		m_templateChild->ApplyTemplateHierarchy(m_combinedLocalResource);	// 再帰的に更新する
//	}
//}
//


} // namespace GUI
} // namespace Lumino
