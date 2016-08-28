
#pragma once
#include "UIControl.h"
#include "UIElementCollection.h"

LN_NAMESPACE_BEGIN

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
	void SetItemsHostPanel(UIPanel* panel);

	// UIElement interface
	//virtual int GetVisualChildrenCount() const override;
	//virtual UIElement* GetVisualChildOrderd(int index) const override;
	//virtual SizeF MeasureOverride(const SizeF& constraint) override;
	//virtual SizeF ArrangeOverride(const SizeF& finalSize) override;

	// IUIElementCollectionOwner interface
	virtual void OnChildCollectionChanged(const tr::ChildCollectionChangedArgs& e) override;

private:
	RefPtr<UIElementCollection>	m_items;
	RefPtr<UIPanel>				m_itemsHostPanel;
};

LN_NAMESPACE_END
