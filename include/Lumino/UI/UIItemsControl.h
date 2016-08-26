
#pragma once
#include "UIElement.h"
#include "UIElementCollection.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class UIItemsControl
	: public UIElement
	, public tr::IUIElementCollectionOwner
{
	LN_UI_TYPEINFO_DECLARE();
public:

	UIElementCollection* GetItems() const;

protected:
	UIItemsControl();
	virtual ~UIItemsControl();
	void Initialize(detail::UIManager* manager);
	void SetItemsHostPanel(UIPanel* panel);

	// UIElement interface
	virtual int GetVisualChildrenCount() const override;
	virtual UIElement* GetVisualChildOrderd(int index) const override;

	// IUIElementCollectionOwner interface
	virtual void OnChildCollectionChanged(const tr::ChildCollectionChangedArgs& e) override;

private:
	RefPtr<UIElementCollection>	m_items;
	RefPtr<UIPanel>				m_itemsHostPanel;
};

LN_NAMESPACE_END
