
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
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UITextElement, UIElement);


//------------------------------------------------------------------------------
UITextElement::UITextElement()
	: FontFamily(String::getEmpty())
	, FontSize(RawFont::DefaultSize)
	, isFontBold(false)
	, isFontItalic(false)
	, m_invalidateFont(true)
{
	//m_fontFamily.setStaticListener(this, onFontPropertyChanged);
	//m_fontSize.setStaticListener(this, onFontPropertyChanged);
	//m_fontBold.setStaticListener(this, onFontPropertyChanged);
	//m_fontItalic.setStaticListener(this, onFontPropertyChanged);
	printf("call onFontPropertyChanged not implemented.");
}

//------------------------------------------------------------------------------
UITextElement::~UITextElement()
{
}

//------------------------------------------------------------------------------
Size UITextElement::measureOverride(const Size& constraint)
{
	// フォント更新
	if (m_invalidateFont)
	{
		if (m_font == nullptr)
		{
			// TODO: getActiveFont() でやったほうがいいかな
			m_font = RefPtr<Font>::makeRef();
			m_font->initialize(getManager()->getGraphicsManager(), nullptr);
		}
		m_font->setFamily(FontFamily);
		m_font->setSize(FontSize);
		m_font->setBold(isFontBold);
		m_font->setItalic(isFontItalic);
		m_font->setAntiAlias(true);
		m_invalidateFont = false;
	}

	return UIElement::measureOverride(constraint);
}

//------------------------------------------------------------------------------
void UITextElement::onUpdateStyle(detail::UIStylePropertyTableInstance* localStyle, detail::InvalidateFlags invalidateFlags)
{
	UIElement::onUpdateStyle(localStyle, invalidateFlags);

	// TODO: 本来はStyle 用の優先順位で設定するべき。
	// https://msdn.microsoft.com/ja-jp/library/ms743230%28v=vs.110%29.aspx#Anchor_3
	// また、アニメーションする場合はここで起動する。
	//setFontFamily(localStyle->m_fontFamily.value);
	//setFontSize(localStyle->m_fontSize.value);
	//setFontBold(localStyle->m_fontBold.value);
	//setFontItalic(localStyle->m_fontItalic.value);
}

//------------------------------------------------------------------------------
void UITextElement::onFontPropertyChanged(Object* obj)
//void UITextElement::onFontPropertyChanged(tr::PropertyChangedEventArgs* e)
{
	static_cast<UITextElement*>(obj)->m_invalidateFont = true;
	// TODO: InvalidateMeasure
}

LN_NAMESPACE_END

