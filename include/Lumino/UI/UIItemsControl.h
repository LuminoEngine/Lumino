
#pragma once
#include "UIControl.h"
#include "UIElementCollection.h"

LN_NAMESPACE_BEGIN
class UILayoutPanel;

/**
	@brief		
*/
class UIItemsControl
	: public UIControl
	, public tr::IUIElementCollectionOwner
{
	LN_UI_TYPEINFO_DECLARE();
public:

	UIElementCollection* GetItems() const;

protected:
	UIItemsControl();
	virtual ~UIItemsControl();
	void Initialize(detail::UIManager* manager);
	void SetItemsHostPanel(UILayoutPanel* panel);

	// UIElement interface
	//virtual int GetVisualChildrenCount() const override;
	//virtual UIElement* GetVisualChildOrderd(int index) const override;
	//virtual Size MeasureOverride(const Size& constraint) override;
	//virtual Size ArrangeOverride(const Size& finalSize) override;

	// IUIElementCollectionOwner interface
	virtual void OnChildCollectionChanged(const tr::ChildCollectionChangedArgs& e) override;

private:
	RefPtr<UIElementCollection>	m_items;
	RefPtr<UILayoutPanel>			m_itemsHostPanel;
};

LN_NAMESPACE_END
