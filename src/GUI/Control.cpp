
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
// TODO: フォント設定は子要素へ継承する
LN_PROPERTY_IMPLEMENT(Control, String, FontFamilyProperty, "FontFamily", m_fontData.Family, String::GetEmpty(), NULL);
LN_PROPERTY_IMPLEMENT(Control, int, FontSizeProperty, "FontSize", m_fontData.Size, 20, NULL);
//LN_PROPERTY_IMPLEMENT(TextElement, int, FontEdgeSizeProperty, "FontEdgeSize", m_fontData.EdgeSize, 20, NULL);
LN_PROPERTY_IMPLEMENT(Control, bool, IsFontBoldProperty, "IsFontBold", m_fontData.IsBold, false, NULL);
LN_PROPERTY_IMPLEMENT(Control, bool, IsFontItalicProperty, "IsFontItalic", m_fontData.IsItalic, false, NULL);
LN_PROPERTY_IMPLEMENT(Control, bool, IsFontAntiAliasProperty, "IsFontAntiAlias", m_fontData.IsAntiAlias, true, NULL);

// Register routed event
LN_ROUTED_EVENT_IMPLEMENT(Control, MouseEventArgs, MouseDoubleClickEvent, "MouseDoubleClick", MouseDoubleClick);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Control::Control(GUIManager* manager)
	: UIElement(manager)
{
	m_visualStateGroupList.Attach(LN_NEW VisualStateGroupList());

	LN_REGISTER_ROUTED_EVENT_HANDLER(Control, MouseEventArgs, UIElement::MouseDownEvent, Handler_MouseDown);
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


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Control::OnMouseDoubleClick(MouseEventArgs* e)
{
	LN_CHECK_ARGS_RETURN(e != NULL);
	if (!e->Handled) {
		RaiseEvent(MouseDoubleClickEvent, this, e);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Control::Handler_MouseDown(MouseEventArgs* e)
{
	if (e->ClickCount == 2) {
		OnMouseDoubleClick(e);
	}
	e->Handled = true;
}


} // namespace GUI
} // namespace Lumino
