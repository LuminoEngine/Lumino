
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
	LN_TR_PROPERTY(String, FontFamilyProperty);		/**< FontFamily プロパティの識別子 */
	LN_TR_PROPERTY(int, FontSizeProperty);			/**< FontSize プロパティの識別子 */
	LN_TR_PROPERTY(bool, IsFontBoldProperty);		/**< IsFontBold プロパティの識別子 */
	LN_TR_PROPERTY(bool, IsFontItalicProperty);		/**< IsFontItalic プロパティの識別子 */

public:

	/** フォントファミリ名を設定します。*/
	void SetFontFamily(const String& value) { tr::PropertyInfo::SetPropertyValueDirect<String>(this, FontFamilyProperty, value); }

	/** フォントファミリ名を取得します。*/
	String GetFontFamily() const { return tr::PropertyInfo::GetPropertyValueDirect<String>(this, FontFamilyProperty); }

	/** フォントサイズを設定します。*/
	void SetFontSize(int value) { tr::PropertyInfo::SetPropertyValueDirect<int>(this, FontSizeProperty, value); }

	/** フォントサイズを取得します。*/
	int GetFontSize() const { return tr::PropertyInfo::GetPropertyValueDirect<int>(this, FontSizeProperty); }

	/** フォントの太字有無を設定します。*/
	void SetFontBold(bool value) { tr::PropertyInfo::SetPropertyValueDirect<bool>(this, IsFontBoldProperty, value); }

	/** フォントの太字有無を取得します。*/
	bool IsFontBold() const { return tr::PropertyInfo::GetPropertyValueDirect<bool>(this, IsFontBoldProperty); }

	/** フォントのイタリック体有無を設定します。*/
	void SetFontItalic(bool value) { tr::PropertyInfo::SetPropertyValueDirect<bool>(this, IsFontItalicProperty, value); }

	/** フォントのイタリック体有無を取得します。*/
	bool IsFontItalic() const { return tr::PropertyInfo::GetPropertyValueDirect<bool>(this, IsFontItalicProperty); }

protected:
	UITextElement();
	virtual ~UITextElement();
	Font* GetActiveFont() const { return m_font; }

	virtual Size MeasureOverride(const Size& constraint) override;
	virtual void OnUpdateStyle(UIStylePropertyTable* localStyle, detail::InvalidateFlags invalidateFlags) override;

LN_INTERNAL_ACCESS:
	static void OnFontPropertyChanged(Object* obj, tr::PropertyChangedEventArgs* e);

	tr::Property<String>		m_fontFamily;
	tr::Property<int>			m_fontSize;
	tr::Property<bool>			m_fontBold;
	tr::Property<bool>			m_fontItalic;

	FontPtr			m_font;
	bool			m_invalidateFont;
};

LN_NAMESPACE_END
