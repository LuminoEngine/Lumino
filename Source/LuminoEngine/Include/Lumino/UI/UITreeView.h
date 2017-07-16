
#pragma once
#include "UIControl.h"

LN_NAMESPACE_BEGIN
namespace tr
{
class UITreeViewItem;
class UITreeView;
using UITreeViewItemPtr = Ref<UITreeViewItem>;
using UITreeViewPtr = Ref<UITreeView>;

/**
	@brief
*/
class UITreeViewItem
	: public UIControl
{
	LN_OBJECT();
public:
	void setHeader(UIElement* header);

protected:

	// UIElement interface
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

LN_CONSTRUCT_ACCESS:
	UITreeViewItem();
	virtual ~UITreeViewItem();
	void initialize();

private:
	Ref<UIElement>	m_header;
};

/**
	@brief
*/
class UITreeView
	: public UIControl
{
	LN_OBJECT();
public:
	static UITreeViewPtr create();

//public:
//	UITreeViewItemPtr addTextItem(const String& text);
//	UITreeViewItemPtr AddItem(UIElement* item);
//
LN_PROTECTED_INTERNAL_ACCESS:
	UITreeView();
	virtual ~UITreeView();
	void initialize();

private:
};

} // namespace tr
LN_NAMESPACE_END
