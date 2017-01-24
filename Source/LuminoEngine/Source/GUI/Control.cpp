
#include "../Internal.h"
#include <Lumino/GUI/Control.h>
#include "GUIHelper.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

//=============================================================================
// Control
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Control, UITextElement);
LN_UI_ELEMENT_SUBCLASS_IMPL(Control);

// Register property
LN_PROPERTY_IMPLEMENT(Control, HorizontalAlignment, HorizontalContentAlignmentProperty, "HorizontalContentAlignment", m_horizontalContentAlignment, PropertyMetadata(HorizontalAlignment::Left));
LN_PROPERTY_IMPLEMENT(Control, VerticalAlignment, VerticalContentAlignmentProperty, "VerticalContentAlignment", m_verticalContentAlignment, PropertyMetadata(VerticalAlignment::Top));
//LN_PROPERTY_IMPLEMENT(Control, Brush*, BackgroundProperty, "Background", m_background, PropertyMetadata(NULL, &Control::OnFontPropertyChanged));
//LN_PROPERTY_IMPLEMENT(Control, Brush*, ForegroundProperty, "Foreground", m_foreground, PropertyMetadata(NULL, &Control::OnFontPropertyChanged));
LN_PROPERTY_IMPLEMENT(Control, ControlTemplate*, TemplateProperty, "Template", m_controlTemplate, PropertyMetadata(NULL/*, &Control::OnFontPropertyChanged*/));
LN_PROPERTY_IMPLEMENT(Control, VisualStateGroupList*, VisualStateGroupsProperty, "VisualStateGroups", m_visualStateGroupList, PropertyMetadata(NULL));

// TODO: フォント設定は子要素へ継承する
//LN_PROPERTY_IMPLEMENT(Control, String, FontFamilyProperty, "FontFamily", m_fontData.Family, PropertyMetadata(String::GetEmpty(), PropertyOptions::Inherits, &Control::OnFontPropertyChanged));
//LN_PROPERTY_IMPLEMENT(Control, int, FontSizeProperty, "FontSize", m_fontData.Size, PropertyMetadata(20, PropertyOptions::Inherits, &Control::OnFontPropertyChanged));
////LN_PROPERTY_IMPLEMENT(TextElement, int, FontEdgeSizeProperty, "FontEdgeSize", m_fontData.EdgeSize, 20, &Control::OnFontPropertyChanged);
//LN_PROPERTY_IMPLEMENT(Control, bool, IsFontBoldProperty, "IsFontBold", m_fontData.IsBold, PropertyMetadata(false, PropertyOptions::Inherits, &Control::OnFontPropertyChanged));
//LN_PROPERTY_IMPLEMENT(Control, bool, IsFontItalicProperty, "IsFontItalic", m_fontData.IsItalic, PropertyMetadata(false, PropertyOptions::Inherits, &Control::OnFontPropertyChanged));
//LN_PROPERTY_IMPLEMENT(Control, bool, IsFontAntiAliasProperty, "IsFontAntiAlias", m_fontData.IsAntiAlias, PropertyMetadata(true, PropertyOptions::Inherits, &Control::OnFontPropertyChanged));

// Register routed event
LN_ROUTED_EVENT_IMPLEMENT(Control, MouseEventArgs, MouseDoubleClickEvent, "MouseDoubleClick", MouseDoubleClick);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Control::Control(GUIManagerImpl* manager)
	: UITextElement(manager)
	//, m_background()
	//, m_foreground()
	//, m_fontData()
	, m_controlTemplate()
	, m_visualStateGroupList()
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
	// 必要があればフォントを再構築する。継承も考慮。
	// TextBlock 等はテキストが要素のサイズとなるため、Measure で行う必要がある。
	//if (m_fontDataModified)
	//{
	//	if (m_parent != NULL)
	//	{
	//		Control* parentControl = dynamic_cast<Control*>(m_parent);
	//		if (parentControl != NULL)
	//		{
	//			if (!HasLocalPropertyValue(FontFamilyProperty)) { m_fontData.Family = parentControl->m_fontData.Family; }
	//			if (!HasLocalPropertyValue(FontSizeProperty)) { m_fontData.Size = parentControl->m_fontData.Size; }
	//			//if (!HasLocalPropertyValue(FontEdgeSizeProperty)) { m_fontData.EdgeSize = parentControl->m_fontData.EdgeSize; }
	//			if (!HasLocalPropertyValue(IsFontBoldProperty)) { m_fontData.IsBold = parentControl->m_fontData.IsBold; }
	//			if (!HasLocalPropertyValue(IsFontItalicProperty)) { m_fontData.IsItalic = parentControl->m_fontData.IsItalic; }
	//			if (!HasLocalPropertyValue(IsFontAntiAliasProperty)) { m_fontData.IsAntiAlias = parentControl->m_fontData.IsAntiAlias; }
	//		}
	//		OnFontUpdated();
	//	}
	//	m_fontDataModified = false;
	//}

	
	SizeF desiredSize;
	if (GUIHelper::SimpleMeasureOverrideSingleVisual(this, constraint, &desiredSize)) {
		//return desiredSize;
	}
	SizeF thisDesiredSize = UIElement::MeasureOverride(constraint);

	desiredSize.Width = std::max(desiredSize.Width, thisDesiredSize.Width);
	desiredSize.Height = std::max(desiredSize.Height, thisDesiredSize.Height);
	return desiredSize;
	//return UIElement::MeasureOverride(constraint);
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
//void Control::OnRender(Painter* painter)
//{
//	// 背景を先に描く
//	if (m_background != NULL) {
//		painter->SetBrush(m_background);
//		painter->DrawRectangle(RectF(0, 0, GetRenderSize()));
//	}
//
//	UIElement::OnRender(painter);
//}

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
//void Control::OnFontUpdated()
//{
//	//int count = GetVisualChildrenCount();
//	//if (count > 0)
//	//{
//	//	for (int i = 0; i < count; i++)
//	//	{
//	//		UIElement* child = GetVisualChild(0);
//	//		if (child != NULL)
//	//		{
//	//			child->inva
//	//		}
//	//	}
//	//}
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void Control::OnFontPropertyChanged(PropertyChangedEventArgs* e)
//{
//	InvalidateFont();
//	// TODO: InvalidateMeasure
//}

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


LN_NAMESPACE_GUI_END
LN_NAMESPACE_END
