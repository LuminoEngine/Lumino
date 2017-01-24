
#pragma once
#include "UIElement.h"

LN_NAMESPACE_BEGIN

/**
	@brief		テキスト描画に関係する色及びフォント管理機能を持つ要素のベースクラスです。
*/
class UITextElement
	: public UIElement
{
	LN_UI_TYPEINFO_DECLARE();
public:
	LN_TR_PROPERTY(String, FontFamily);		/**< FontFamily プロパティの識別子 */
	LN_TR_PROPERTY(int, FontSize);			/**< FontSize プロパティの識別子 */
	LN_TR_PROPERTY(bool, IsFontBold);		/**< IsFontBold プロパティの識別子 */
	LN_TR_PROPERTY(bool, IsFontItalic);		/**< IsFontItalic プロパティの識別子 */

public:

	/** フォントファミリ名を設定します。*/
	void SetFontFamily(const String& value) { tr::PropertyInfo::SetPropertyValueDirect<String>(this, FontFamilyId, value); }

	/** フォントファミリ名を取得します。*/
	String GetFontFamily() const { return tr::PropertyInfo::GetPropertyValueDirect<String>(this, FontFamilyId); }

	/** フォントサイズを設定します。*/
	void SetFontSize(int value) { tr::PropertyInfo::SetPropertyValueDirect<int>(this, FontSizeId, value); }

	/** フォントサイズを取得します。*/
	int GetFontSize() const { return tr::PropertyInfo::GetPropertyValueDirect<int>(this, FontSizeId); }

	/** フォントの太字有無を設定します。*/
	void SetFontBold(bool value) { tr::PropertyInfo::SetPropertyValueDirect<bool>(this, IsFontBoldId, value); }

	/** フォントの太字有無を取得します。*/
	//bool IsFontBold() const { return tr::PropertyInfo::GetPropertyValueDirect<bool>(this, IsFontBoldId); }

	/** フォントのイタリック体有無を設定します。*/
	void SetFontItalic(bool value) { tr::PropertyInfo::SetPropertyValueDirect<bool>(this, IsFontItalicId, value); }

	/** フォントのイタリック体有無を取得します。*/
	//bool IsFontItalic() const { return tr::PropertyInfo::GetPropertyValueDirect<bool>(this, IsFontItalicId); }

protected:
	UITextElement();
	virtual ~UITextElement();
	Font* GetActiveFont() const { return m_font; }

	virtual Size MeasureOverride(const Size& constraint) override;
	virtual void OnUpdateStyle(UIStylePropertyTable* localStyle, detail::InvalidateFlags invalidateFlags) override;

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
