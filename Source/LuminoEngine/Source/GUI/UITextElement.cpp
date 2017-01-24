
#include "../Internal.h"
#include "UIManagerImpl.h"
#include <Lumino/GUI/UITextElement.h>
#include "GUIHelper.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

//=============================================================================
// TextBlock
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(UITextElement, UIElement);
LN_UI_ELEMENT_SUBCLASS_IMPL(UITextElement);

LN_PROPERTY_IMPLEMENT(UITextElement, Brush*, BackgroundProperty, "Background", m_background, PropertyMetadata(NULL, &UITextElement::OnFontPropertyChanged));
LN_PROPERTY_IMPLEMENT(UITextElement, Brush*, ForegroundProperty, "Foreground", m_foreground, PropertyMetadata(NULL, PropertyOptions::Inherits, &UITextElement::OnFontPropertyChanged));
LN_PROPERTY_IMPLEMENT(UITextElement, String, FontFamilyProperty, "FontFamily", m_fontData.Family, PropertyMetadata(String::GetEmpty(), PropertyOptions::Inherits, &UITextElement::OnFontPropertyChanged));
LN_PROPERTY_IMPLEMENT(UITextElement, int, FontSizeProperty, "FontSize", m_fontData.Size, PropertyMetadata(20, PropertyOptions::Inherits, &UITextElement::OnFontPropertyChanged));
LN_PROPERTY_IMPLEMENT(UITextElement, bool, IsFontBoldProperty, "IsFontBold", m_fontData.IsBold, PropertyMetadata(false, PropertyOptions::Inherits, &UITextElement::OnFontPropertyChanged));
LN_PROPERTY_IMPLEMENT(UITextElement, bool, IsFontItalicProperty, "IsFontItalic", m_fontData.IsItalic, PropertyMetadata(false, PropertyOptions::Inherits, &UITextElement::OnFontPropertyChanged));
LN_PROPERTY_IMPLEMENT(UITextElement, bool, IsFontAntiAliasProperty, "IsFontAntiAlias", m_fontData.IsAntiAlias, PropertyMetadata(true, PropertyOptions::Inherits, &UITextElement::OnFontPropertyChanged));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UITextElement::UITextElement(GUIManagerImpl* manager)
	: UIElement(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UITextElement::~UITextElement()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF UITextElement::MeasureOverride(const SizeF& constraint)
{
	if (GUIHelper::UIElement_GetInvalidateFlags(this).TestFlag(InvalidateFlags::Font))
	{
		OnUpdateFont(m_fontData);
	}
	return UIElement::MeasureOverride(constraint);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UITextElement::OnRender(RenderingContext* painter)
{
	// 背景を先に描く
	if (m_background != NULL) {
		painter->SetBrush(m_background);
		painter->DrawRectangle(RectF(0, 0, GetRenderSize()));
	}

	UIElement::OnRender(painter);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UITextElement::OnUpdateFont(const GraphicsManager::FontData& fontData)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UITextElement::OnFontPropertyChanged(PropertyChangedEventArgs* e)
{
	InvalidateFont();
	// TODO: InvalidateMeasure
}

LN_NAMESPACE_GUI_END
LN_NAMESPACE_END

