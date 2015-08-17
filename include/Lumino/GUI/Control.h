
#pragma once
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

public:
	Control(GUIManager* manager);
	virtual ~Control();

	//-------------------------------------------------------------------------
	/** @name Properties */
	/** @{ */

	/** コントロールの背景の描画に使用するブラシを設定します。*/
	void SetBackground(Graphics::BrushPtr value) { SetTypedPropertyValue<Graphics::BrushPtr>(BackgroundProperty, value); }

	/** コントロールの背景の描画に使用するブラシを取得します。*/
	Graphics::BrushPtr GetBackground() const { return GetTypedPropertyValue<Graphics::BrushPtr>(BackgroundProperty); }



	

	void SetTemplate(ControlTemplate* controlTemplate);	// TODO: OnTemplatePropertyChanged
	ControlTemplate* GetTemplate() const { return m_controlTemplate; }


	/** この Control に割り当てられている VisualStateGroup のリストを取得します。*/
	VisualStateGroupList* GetVisualStateGroups() const { return m_visualStateGroupList; }

	/** @} */

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
private:
	Graphics::BrushPtr	m_background;
	RefPtr<ControlTemplate>	m_controlTemplate;
	RefPtr<VisualStateGroupList>	m_visualStateGroupList;
};


} // namespace GUI
} // namespace Lumino
