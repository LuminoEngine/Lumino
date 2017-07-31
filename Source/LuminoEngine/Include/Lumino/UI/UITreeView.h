
#pragma once
#include "UIControl.h"

LN_NAMESPACE_BEGIN
class UIToggleButton;
class UITreeViewItem;
class UITreeView;

/**
	@brief
*/
class UITreeViewItem
	: public UIControl
{
	LN_OBJECT;
public:
	void setHeader(UIElement* header);

	void setExpanded(bool expand);
	bool isExpanded() const;

	UITreeViewItem* addTextItem(const String& text);
	UITreeViewItem* addItem(UIElement* item);

protected:

	// UIElement interface
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

LN_CONSTRUCT_ACCESS:
	UITreeViewItem();
	virtual ~UITreeViewItem();
	void initialize();

private:
	void expander_OnChecked(UIEventArgs* e);
	void expander_OnUnchecked(UIEventArgs* e);

	Ref<UIElement>		m_header;
	Ref<UIToggleButton>	m_expander;
};

/**
	@brief
*/
class UITreeView
	: public UIControl
{
	LN_OBJECT;
public:
	static Ref<UITreeView> create();

public:
	UITreeViewItem* addTextItem(const String& text);
	UITreeViewItem* addItem(UIElement* item);

LN_CONSTRUCT_ACCESS:
	UITreeView();
	virtual ~UITreeView();
	void initialize();

private:
};

LN_NAMESPACE_END
