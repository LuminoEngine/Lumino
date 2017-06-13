
#pragma once
#include "UIElement.h"

LN_NAMESPACE_BEGIN

/**
	@brief		テキスト描画に関係する色及びフォント管理機能を持つ要素のベースクラスです。
*/
class UITextElement
	: public UIElement
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	LN_TR_PROPERTY(String, FontFamily);		/**< FontFamily プロパティの識別子 */
	LN_TR_PROPERTY(int, FontSize);			/**< FontSize プロパティの識別子 */
	LN_TR_PROPERTY(bool, isFontBold);		/**< isFontBold プロパティの識別子 */
	LN_TR_PROPERTY(bool, isFontItalic);		/**< isFontItalic プロパティの識別子 */
	tr::Property<String>	FontFamily;
	tr::Property<int>		FontSize;
	tr::Property<bool>		isFontBold;
	tr::Property<bool>		isFontItalic;

public:

	/** フォントファミリ名を設定します。*/
	void setFontFamily(const String& value) { tr::PropertyInfo::setPropertyValueDirect<String>(this, FontFamilyId, value); }

	/** フォントファミリ名を取得します。*/
	String getFontFamily() const { return tr::PropertyInfo::getPropertyValueDirect<String>(this, FontFamilyId); }

	/** フォントサイズを設定します。*/
	void setFontSize(int value) { tr::PropertyInfo::setPropertyValueDirect<int>(this, FontSizeId, value); }

	/** フォントサイズを取得します。*/
	int getFontSize() const { return tr::PropertyInfo::getPropertyValueDirect<int>(this, FontSizeId); }

	/** フォントの太字有無を設定します。*/
	void setFontBold(bool value) { tr::PropertyInfo::setPropertyValueDirect<bool>(this, isFontBoldId, value); }

	/** フォントの太字有無を取得します。*/
	//bool isFontBold() const { return tr::PropertyInfo::getPropertyValueDirect<bool>(this, IsFontBoldId); }

	/** フォントのイタリック体有無を設定します。*/
	void setFontItalic(bool value) { tr::PropertyInfo::setPropertyValueDirect<bool>(this, isFontItalicId, value); }

	/** フォントのイタリック体有無を取得します。*/
	//bool isFontItalic() const { return tr::PropertyInfo::getPropertyValueDirect<bool>(this, IsFontItalicId); }

protected:
	UITextElement();
	virtual ~UITextElement();
	Font* GetActiveFont() const { return m_font; }

	virtual Size measureOverride(const Size& constraint) override;
	virtual void OnUpdateStyle(detail::UIStylePropertyTableInstance* localStyle, detail::InvalidateFlags invalidateFlags) override;

LN_INTERNAL_ACCESS:
	static void OnFontPropertyChanged(Object* obj);

	//tr::Property<String>		m_fontFamily;
	//tr::Property<int>			m_fontSize;
	//tr::Property<bool>			m_fontBold;
	//tr::Property<bool>			m_fontItalic;

	FontPtr			m_font;
	bool			m_invalidateFont;
};

LN_NAMESPACE_END
