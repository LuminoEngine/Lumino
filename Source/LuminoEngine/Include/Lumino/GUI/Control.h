
#pragma once
#include "../Graphics/GraphicsManager.h"
#include "UITextElement.h"
#include "VisualState.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN
class ContentPresenter;

/**
	@brief		ユーザー操作によって特定の動作を行う UI 要素の基本クラスです。
*/
class Control
	: public UITextElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Control);
public:
	LN_PROPERTY(ControlTemplate*,		TemplateProperty);
	LN_PROPERTY(VisualStateGroupList*,	VisualStateGroupsProperty);
	LN_PROPERTY(HorizontalAlignment,	HorizontalContentAlignmentProperty);	/**< HorizontalContentAlignment プロパティの識別子 */
	LN_PROPERTY(VerticalAlignment,		VerticalContentAlignmentProperty);		/**< VerticalContentAlignment プロパティの識別子 */
	//LN_PROPERTY(int,					FontSizeProperty);				/**< FontSize プロパティの識別子 */
	////LN_PROPERTY(int,					FontEdgeSizeProperty);			// TODO: Pen に移したい
	//LN_PROPERTY(bool,					IsFontBoldProperty);			/**< IsFontBold プロパティの識別子 */
	//LN_PROPERTY(bool,					IsFontItalicProperty);			/**< IsFontItalic プロパティの識別子 */
	//LN_PROPERTY(bool,					IsFontAntiAliasProperty);		/**< IsFontAntiAlias プロパティの識別子 */

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

	/** コントロールのコンテンツの水平方向の配置方法を設定します。*/
	void SetHorizontalContentAlignment(HorizontalAlignment value) { SetTypedPropertyValue(HorizontalContentAlignmentProperty, value); }

	/** コントロールのコンテンツの水平方向の配置方法を取得します。*/
	HorizontalAlignment GetHorizontalContentAlignment() const { return GetTypedPropertyValue<HorizontalAlignment>(HorizontalContentAlignmentProperty); }
	
	/** コントロールのコンテンツの垂直方向の配置方法を設定します。*/
	void SetVerticalContentAlignment(VerticalAlignment value) { SetTypedPropertyValue(VerticalContentAlignmentProperty, value); }

	/** コントロールのコンテンツの垂直方向の配置方法を取得します。*/
	VerticalAlignment GetVerticalContentAlignment() const { return GetTypedPropertyValue<VerticalAlignment>(VerticalContentAlignmentProperty); }


	void SetTemplate(ControlTemplate* controlTemplate);	// TODO: OnTemplatePropertyChanged
	ControlTemplate* GetTemplate() const { return m_controlTemplate; }


	/** この Control に割り当てられている VisualStateGroup のリストを取得します。*/
	VisualStateGroupList* GetVisualStateGroups() const { return m_visualStateGroupList; }

	/** @} */

protected:
	Control(GUIManagerImpl* manager);
	virtual ~Control();

public:
	virtual bool IsFocusable() const { return true; }

protected:


	// override UIElement
	virtual void OnApplyTemplate(CombinedLocalResource* localResource);
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);
	//virtual void OnRender(Painter* painter);

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
	//void OnFontPropertyChanged(PropertyChangedEventArgs* e);
	void Handler_MouseDown(MouseEventArgs* e);

private:
	HorizontalAlignment				m_horizontalContentAlignment;
	VerticalAlignment				m_verticalContentAlignment;
	RefPtr<ControlTemplate>			m_controlTemplate;
	RefPtr<VisualStateGroupList>	m_visualStateGroupList;
};


LN_NAMESPACE_GUI_END
LN_NAMESPACE_END
