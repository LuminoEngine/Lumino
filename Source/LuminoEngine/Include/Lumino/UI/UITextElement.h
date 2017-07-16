
#pragma once
#include "UIElement.h"

LN_NAMESPACE_BEGIN

/**
	@brief		テキスト描画に関係する色及びフォント管理機能を持つ要素のベースクラスです。
*/
class UITextElement
	: public UIElement
{
	LN_OBJECT;
public:
	String	FontFamily;
	int		FontSize;
	bool		isFontBold;
	bool		isFontItalic;

public:

	/** フォントファミリ名を設定します。*/
	void setFontFamily(const String& value) { FontFamily = value; }

	/** フォントファミリ名を取得します。*/
	String getFontFamily() const { return FontFamily; }

	/** フォントサイズを設定します。*/
	void setFontSize(int value) { FontSize = value; }

	/** フォントサイズを取得します。*/
	int getFontSize() const { return FontSize; }

	/** フォントの太字有無を設定します。*/
	void setFontBold(bool value) { isFontBold = value; }

	/** フォントの太字有無を取得します。*/
	//bool isFontBold() const { return tr::PropertyInfo::getPropertyValueDirect<bool>(this, IsFontBoldId; }

	/** フォントのイタリック体有無を設定します。*/
	void setFontItalic(bool value) { isFontItalic = value; }

	/** フォントのイタリック体有無を取得します。*/
	//bool isFontItalic() const { return tr::PropertyInfo::getPropertyValueDirect<bool>(this, IsFontItalicId); }

protected:
	UITextElement();
	virtual ~UITextElement();
	Font* getActiveFont() const { return m_font; }

	virtual Size measureOverride(const Size& constraint) override;
	virtual void onUpdateStyle(detail::UIStylePropertyTableInstance* localStyle, detail::InvalidateFlags invalidateFlags) override;

LN_INTERNAL_ACCESS:
	static void onFontPropertyChanged(Object* obj);

	//String		m_fontFamily;
	//int			m_fontSize;
	//bool			m_fontBold;
	//bool			m_fontItalic;

	FontPtr			m_font;
	bool			m_invalidateFont;
};

LN_NAMESPACE_END
