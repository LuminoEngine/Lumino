
#include "Internal.h"
#include <Lumino/UI/UIStyle.h>
#include <Lumino/UI/UITextElement.h>
#include "../Graphics/GraphicsManager.h"
#include "../Graphics/Text/FontManager.h"
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UITextElement
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UITextElement, UIElement);
LN_TR_PROPERTY2_IMPLEMENT(UITextElement, String, FontFamily, tr::PropertyMetadata(UITextElement::OnFontPropertyChanged));
LN_TR_PROPERTY2_IMPLEMENT(UITextElement, int, FontSize, tr::PropertyMetadata(UITextElement::OnFontPropertyChanged));
LN_TR_PROPERTY2_IMPLEMENT(UITextElement, bool, IsFontBold, tr::PropertyMetadata(UITextElement::OnFontPropertyChanged));
LN_TR_PROPERTY2_IMPLEMENT(UITextElement, bool, IsFontItalic, tr::PropertyMetadata(UITextElement::OnFontPropertyChanged));


//------------------------------------------------------------------------------
UITextElement::UITextElement()
	: FontFamily(String::GetEmpty())
	, FontSize(RawFont::DefaultSize)
	, IsFontBold(false)
	, IsFontItalic(false)
	, m_invalidateFont(true)
{
	//m_fontFamily.SetStaticListener(this, OnFontPropertyChanged);
	//m_fontSize.SetStaticListener(this, OnFontPropertyChanged);
	//m_fontBold.SetStaticListener(this, OnFontPropertyChanged);
	//m_fontItalic.SetStaticListener(this, OnFontPropertyChanged);
}

//------------------------------------------------------------------------------
UITextElement::~UITextElement()
{
}

//------------------------------------------------------------------------------
Size UITextElement::MeasureOverride(const Size& constraint)
{
	// フォント更新
	if (m_invalidateFont)
	{
		if (m_font == nullptr)
		{
			// TODO: GetActiveFont() でやったほうがいいかな
			m_font = RefPtr<Font>::MakeRef();
			m_font->Initialize(GetManager()->GetGraphicsManager(), nullptr);
		}
		m_font->SetFamily(FontFamily);
		m_font->SetSize(FontSize);
		m_font->SetBold(IsFontBold);
		m_font->SetItalic(IsFontItalic);
		m_font->SetAntiAlias(true);
		m_invalidateFont = false;
	}

	return UIElement::MeasureOverride(constraint);
}

//------------------------------------------------------------------------------
void UITextElement::OnUpdateStyle(UIStylePropertyTable* localStyle, detail::InvalidateFlags invalidateFlags)
{
	UIElement::OnUpdateStyle(localStyle, invalidateFlags);

	// TODO: 本来はStyle 用の優先順位で設定するべき。
	// https://msdn.microsoft.com/ja-jp/library/ms743230%28v=vs.110%29.aspx#Anchor_3
	// また、アニメーションする場合はここで起動する。
	//SetFontFamily(localStyle->m_fontFamily.value);
	//SetFontSize(localStyle->m_fontSize.value);
	//SetFontBold(localStyle->m_fontBold.value);
	//SetFontItalic(localStyle->m_fontItalic.value);
}

//------------------------------------------------------------------------------
void UITextElement::OnFontPropertyChanged(Object* obj)
//void UITextElement::OnFontPropertyChanged(tr::PropertyChangedEventArgs* e)
{
	static_cast<UITextElement*>(obj)->m_invalidateFont = true;
	// TODO: InvalidateMeasure
}

LN_NAMESPACE_END

