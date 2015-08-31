
#pragma once
#include "../Graphics/GraphicsManager.h"
#include "UIElement.h"
#include "VisualState.h"

namespace Lumino
{
namespace GUI
{
class ContentPresenter;

/**
	@brief		ユーザー操作によって特定の動作を行う UI 要素の基本クラスです。
*/
class Control
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Control);
public:
	LN_PROPERTY(Graphics::BrushPtr,		BackgroundProperty);			/**< Background プロパティの識別子 */
	LN_PROPERTY(ControlTemplate*,		TemplateProperty);
	LN_PROPERTY(VisualStateGroupList*,	VisualStateGroupsProperty);
	LN_PROPERTY(String,					FontFamilyProperty);			/**< FontFamily プロパティの識別子 */
	LN_PROPERTY(int,					FontSizeProperty);				/**< FontSize プロパティの識別子 */
	//LN_PROPERTY(int,					FontEdgeSizeProperty);			// TODO: Pen に移したい
	LN_PROPERTY(bool,					IsFontBoldProperty);			/**< IsFontBold プロパティの識別子 */
	LN_PROPERTY(bool,					IsFontItalicProperty);			/**< IsFontItalic プロパティの識別子 */
	LN_PROPERTY(bool,					IsFontAntiAliasProperty);		/**< IsFontAntiAlias プロパティの識別子 */

	LN_ROUTED_EVENT(MouseEventArgs,		MouseDoubleClickEvent);			/**< MouseDoubleClick ルーティングイベントの識別子 */

public:

	//-------------------------------------------------------------------------
	/** @name RoutedEvents */
	/** @{ */

	RoutedEventSlot<MouseEventArgs>		MouseDoubleClick;		/**< マウス ボタンが 2 回クリックされると発生します。*/

	/** @} */


	//-------------------------------------------------------------------------
	/** @name Properties */
	/** @{ */

	/** コントロールの背景の描画に使用するブラシを設定します。*/
	void SetBackground(Graphics::BrushPtr value) { SetTypedPropertyValue<Graphics::BrushPtr>(BackgroundProperty, value); }

	/** コントロールの背景の描画に使用するブラシを取得します。*/
	Graphics::BrushPtr GetBackground() const { return GetTypedPropertyValue<Graphics::BrushPtr>(BackgroundProperty); }

	/** コントロールのフォントファミリ名を設定します。*/
	void SetFontFamily(const String& value) { SetTypedPropertyValue<String>(FontFamilyProperty, value); }

	/** コントロールのフォントファミリ名を取得します。*/
	String GetFontFamily() const { return GetTypedPropertyValue<String>(FontFamilyProperty); }

	/** コントロールのフォントサイズを設定します。*/
	void SetFontSize(int value) { SetTypedPropertyValue<int>(FontSizeProperty, value); }

	/** コントロールのフォントサイズを取得します。*/
	int GetFontSize() const { return GetTypedPropertyValue<int>(FontSizeProperty); }

	/** コントロールのフォントの太字有無を設定します。*/
	void SetFontBold(bool value) { SetTypedPropertyValue<bool>(IsFontBoldProperty, value); }

	/** コントロールのフォントの太字有無を取得します。*/
	bool IsFontBold() const { return GetTypedPropertyValue<bool>(IsFontBoldProperty); }

	/** コントロールのフォントのイタリック体有無を設定します。*/
	void SetFontItalic(bool value) { SetTypedPropertyValue<bool>(IsFontItalicProperty, value); }

	/** コントロールのフォントのイタリック体有無を取得します。*/
	bool IsFontItalic() const { return GetTypedPropertyValue<bool>(IsFontItalicProperty); }

	/** コントロールのフォントのアンチエイリアス有無を設定します。*/
	void SetFontAntiAlias(bool value) { SetTypedPropertyValue<bool>(IsFontAntiAliasProperty, value); }

	/** コントロールのフォントのアンチエイリアス有無を取得します。*/
	bool IsFontAntiAlias() const { return GetTypedPropertyValue<bool>(IsFontAntiAliasProperty); }

	

	void SetTemplate(ControlTemplate* controlTemplate);	// TODO: OnTemplatePropertyChanged
	ControlTemplate* GetTemplate() const { return m_controlTemplate; }


	/** この Control に割り当てられている VisualStateGroup のリストを取得します。*/
	VisualStateGroupList* GetVisualStateGroups() const { return m_visualStateGroupList; }

	/** @} */

protected:
	Control(GUIManager* manager);
	virtual ~Control();

protected:


	// override UIElement
	virtual void OnApplyTemplate(CombinedLocalResource* localResource);
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);
	virtual void OnRender(Graphics::Painter* painter);

	//virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);

	//friend class UIElementFactory;

	//friend class ControlTemplate;
	//friend class DataTemplate;
	//virtual void AddVisualChild(UIElement* child) { LN_THROW(0, InvalidOperationException); }	// Contro 自体は子要素を持たない。サブクラスでさらにオーバーライドされる。

	//virtual void ApplyTemplate(CombinedLocalResource* parent);

	//	virtual bool ApplyTemplateInternal();
	//	bool ApplyTemplateInternalMain(const String& typeName);
	//
	//
	//protected:
	//	// override UIElement
	//	virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);

	virtual void OnMouseDoubleClick(MouseEventArgs* e);

	//virtual void OnFontUpdated();

private:
	void OnFontPropertyChanged(PropertyChangedEventArgs* e);
	void Handler_MouseDown(MouseEventArgs* e);

private:
	Graphics::BrushPtr	m_background;
	Graphics::GraphicsManager::FontData	m_fontData;
	RefPtr<ControlTemplate>			m_controlTemplate;
	RefPtr<VisualStateGroupList>	m_visualStateGroupList;
};


} // namespace GUI
} // namespace Lumino
