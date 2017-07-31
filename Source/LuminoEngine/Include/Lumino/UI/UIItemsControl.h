
#pragma once
#include "UIControl.h"
#include "UIElementCollection.h"

LN_NAMESPACE_BEGIN

/**
	@brief	意味のある連続した子要素を管理する機能を持ったコントロールです。
*/
class UIItemsControl
	: public UIControl
{
	LN_OBJECT;
public:
	void addItem(UIElement* item);

LN_CONSTRUCT_ACCESS:
	UIItemsControl();
	virtual ~UIItemsControl();
	void initialize();

private:
};

/**
	@brief
*/
class UIHeaderedItemsControl
	: public UIItemsControl
{
	LN_OBJECT;
public:
	void setHeader(UIElement* header);

LN_CONSTRUCT_ACCESS:
	UIHeaderedItemsControl();
	virtual ~UIHeaderedItemsControl();
	void initialize();

protected:
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

private:
	Ref<UIControl>	m_headerContainer;
	Ref<UIElement>	m_headerContent;
};

#if 0
/**
	@brief		
*/
class UIItemsControl
	: public UIControl
	, public tr::IUIElementCollectionOwner
{
	LN_OBJECT;
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
	Ref<UIElementCollection>	m_items;
	Ref<UILayoutPanel>			m_itemsHostPanel;
};
#endif

LN_NAMESPACE_END
