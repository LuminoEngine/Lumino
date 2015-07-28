
#pragma once
#include "UIElement.h"
#include "ItemsPresenter.h"
#include "Controls/Panel.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief
*/
class ItemsControl
	: public Control
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ItemsControl);
public:
	static const Property*	ItemsPanelTemplateProperty;

public:
	static ItemsControl* Create(GUIManager* manager);

	ItemsControl(GUIManager* manager);
	virtual ~ItemsControl();

	void SetItemsPanelTemplate(ControlTemplate* controlTemplate) { m_itemsPanelTemplate = controlTemplate; }
	ControlTemplate* GetItemsPanelTemplate() const { return m_itemsPanelTemplate; }

private:

	virtual void OnApplyTemplate(CombinedLocalResource* localResource);
	virtual void PollingTemplateChildCreated(UIElement* newElement);
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);

private:
	RefPtr<ControlTemplate>	m_itemsPanelTemplate;
	ItemsPresenter*			m_visualItemsPresenter;	///< VisualTree 内の ItemsPresenter
	RefPtr<Panel>			m_hostPanel;			///< アイテムコントロールの追加先
};


} // namespace GUI
} // namespace Lumino
