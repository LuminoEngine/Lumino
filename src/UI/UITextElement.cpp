
#include "Internal.h"
#include <Lumino/UI/UIStyle.h>
#include <Lumino/UI/UITextElement.h>
#include "../AssetsManager.h"
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// UITextElement
//=============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UITextElement, UIElement);
LN_TR_PROPERTY_IMPLEMENT(UITextElement, String, FontFamilyProperty, "FontFamily", m_fontFamily, tr::PropertyMetadata(_T(""), &UITextElement::OnFontPropertyChanged));
LN_TR_PROPERTY_IMPLEMENT(UITextElement, int, FontSizeProperty, "FontSize", m_fontSize, tr::PropertyMetadata(_T(""), &UITextElement::OnFontPropertyChanged));
LN_TR_PROPERTY_IMPLEMENT(UITextElement, bool, IsFontBoldProperty, "IsFontBold", m_fontBold, tr::PropertyMetadata(_T(""), &UITextElement::OnFontPropertyChanged));
LN_TR_PROPERTY_IMPLEMENT(UITextElement, bool, IsFontItalicProperty, "IsFontItalic", m_fontItalic, tr::PropertyMetadata(_T(""), &UITextElement::OnFontPropertyChanged));


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UITextElement::UITextElement()
	: m_fontFamily(String::GetEmpty())
	, m_fontSize(Font::DefaultSize)
	, m_fontBold(false)
	, m_fontItalic(false)
	, m_invalidateFont(true)
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
	// フォント更新
	if (m_invalidateFont)
	{
		//m_font = Font::Create();
		//m_font->SetSize(20);
		m_font = GetManager()->GetAssetsManager()->LoadFont(m_fontFamily, m_fontSize, m_fontBold, m_fontItalic, true);
		m_invalidateFont = false;
	}

	return UIElement::MeasureOverride(constraint);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UITextElement::OnUpdateStyle(UIStyle* localStyle, detail::InvalidateFlags invalidateFlags)
{
	UIElement::OnUpdateStyle(localStyle, invalidateFlags);

	// TODO: 本来はStyle 用の優先順位で設定するべき。
	// https://msdn.microsoft.com/ja-jp/library/ms743230%28v=vs.110%29.aspx#Anchor_3
	// また、アニメーションする場合はここで起動する。
	SetFontFamily(localStyle->m_fontFamily.value);
	SetFontSize(localStyle->m_fontSize.value);
	SetFontBold(localStyle->m_fontBold.value);
	SetFontItalic(localStyle->m_fontItalic.value);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UITextElement::OnFontPropertyChanged(tr::PropertyChangedEventArgs* e)
{
	m_invalidateFont = true;
	// TODO: InvalidateMeasure
}

LN_NAMESPACE_END

