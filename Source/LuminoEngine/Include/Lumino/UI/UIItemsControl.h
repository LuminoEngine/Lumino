
#pragma once
#include "UIControl.h"
#include "UIElementCollection.h"

LN_NAMESPACE_BEGIN
class UILayoutPanel;

#if 0
/**
	@brief		
*/
class UIItemsControl
	: public UIControl
	, public tr::IUIElementCollectionOwner
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	UIElementCollection* GetItems() const;

	void AddChild(UIElement* element);
	void RemoveChild(UIElement* element);

	void SetLayoutPanel(UILayoutPanel* panel);

protected:
	UIItemsControl();
	virtual ~UIItemsControl();
	void initialize(detail::UIManager* manager);

	virtual void OnLayoutPanelChanged(UILayoutPanel* newPanel);

	// UIElement interface
	//virtual int getVisualChildrenCount() const override;
	//virtual UIElement* getVisualChild(int index) const override;
	//virtual Size measureOverride(const Size& constraint) override;
	//virtual Size arrangeOverride(const Size& finalSize) override;

	// IUIElementCollectionOwner interface
	virtual void OnChildCollectionChanged(const tr::ChildCollectionChangedArgs& e) override;

private:
	RefPtr<UIElementCollection>	m_items;
	RefPtr<UILayoutPanel>			m_itemsHostPanel;
};
#endif

LN_NAMESPACE_END
