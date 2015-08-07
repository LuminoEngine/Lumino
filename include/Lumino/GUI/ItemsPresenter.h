
#pragma once
#include "UIElement.h"
#include "Controls/Panel.h"

namespace Lumino
{
namespace GUI
{
	
/**
	@brief	ItemsControl のコンテンツを表示します。
*/
class ItemsPresenter
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ItemsPresenter);
public:

public:
	static ItemsPresenterPtr Create();

	ItemsPresenter(GUIManager* manager);
	virtual ~ItemsPresenter();

public:	// internal
	void AttachHostPanel(Panel* panel);

protected:
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);
	//virtual void AddVisualChild(UIElement* child) { LN_THROW(0, InvalidOperationException); }	// ItemsPresenter は論理的な子要素の配置先をマークするメタデータのようなものなので、子要素は持たない。
	
private:
	RefPtr<Panel>	m_hostPanel;
};

} // namespace GUI
} // namespace Lumino
