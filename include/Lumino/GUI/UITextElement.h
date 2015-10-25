
#pragma once
#include "UIElement.h"

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN

/**
	@brief		テキスト描画に関係する色及びフォント管理機能を持つ要素のベースクラスです。
*/
class UITextElement
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(UITextElement);
public:
	LN_PROPERTY(Brush*,		BackgroundProperty);		/**< Background プロパティの識別子 */
	LN_PROPERTY(Brush*,		ForegroundProperty);		/**< Foreground プロパティの識別子 */
	LN_PROPERTY(String,		FontFamilyProperty);		/**< FontFamily プロパティの識別子 */
	LN_PROPERTY(int,		FontSizeProperty);			/**< FontSize プロパティの識別子 */
	LN_PROPERTY(bool,		IsFontBoldProperty);		/**< IsFontBold プロパティの識別子 */
	LN_PROPERTY(bool,		IsFontItalicProperty);		/**< IsFontItalic プロパティの識別子 */
	LN_PROPERTY(bool,		IsFontAntiAliasProperty);	/**< IsFontAntiAlias プロパティの識別子 */

public:

	/** コントロールの背景の描画に使用するブラシを設定します。*/
	void SetBackground(Brush* value) { SetTypedPropertyValue<Brush*>(BackgroundProperty, value); }

	/** コントロールの背景の描画に使用するブラシを取得します。*/
	Brush* GetBackground() const { return GetTypedPropertyValue<Brush*>(BackgroundProperty); }

	/** コントロールの前景の描画に使用するブラシを設定します。*/
	void SetForeground(Brush* value) { SetTypedPropertyValue<Brush*>(ForegroundProperty, value); }

	/** コントロールの前景の描画に使用するブラシを取得します。*/
	Brush* GetForeground() const { return GetTypedPropertyValue<Brush*>(ForegroundProperty); }

	/** フォントファミリ名を設定します。*/
	void SetFontFamily(const String& value) { SetTypedPropertyValue<String>(FontFamilyProperty, value); }

	/** フォントファミリ名を取得します。*/
	String GetFontFamily() const { return GetTypedPropertyValue<String>(FontFamilyProperty); }

	/** フォントサイズを設定します。*/
	void SetFontSize(int value) { SetTypedPropertyValue<int>(FontSizeProperty, value); }

	/** フォントサイズを取得します。*/
	int GetFontSize() const { return GetTypedPropertyValue<int>(FontSizeProperty); }

	/** フォントの太字有無を設定します。*/
	void SetFontBold(bool value) { SetTypedPropertyValue<bool>(IsFontBoldProperty, value); }

	/** フォントの太字有無を取得します。*/
	bool IsFontBold() const { return GetTypedPropertyValue<bool>(IsFontBoldProperty); }

	/** フォントのイタリック体有無を設定します。*/
	void SetFontItalic(bool value) { SetTypedPropertyValue<bool>(IsFontItalicProperty, value); }

	/** フォントのイタリック体有無を取得します。*/
	bool IsFontItalic() const { return GetTypedPropertyValue<bool>(IsFontItalicProperty); }

	/** フォントのアンチエイリアス有無を設定します。*/
	void SetFontAntiAlias(bool value) { SetTypedPropertyValue<bool>(IsFontAntiAliasProperty, value); }

	/** フォントのアンチエイリアス有無を取得します。*/
	bool IsFontAntiAlias() const { return GetTypedPropertyValue<bool>(IsFontAntiAliasProperty); }

protected:
	UITextElement(GUIManagerImpl* manager);
	virtual ~UITextElement();
	virtual void OnRender(RenderingContext* painter);

private:
	void OnTextPropertyChanged(PropertyChangedEventArgs* e);
	void OnFontPropertyChanged(PropertyChangedEventArgs* e);

private:
	BrushPtr	m_background;
	BrushPtr	m_foreground;
	GraphicsManager::FontData	m_fontData;	// TODO: ポインタにするなりししないと、GraphicsManager を include することになる…
};

LN_NAMESPACE_GUI_END
} // namespace Lumino
