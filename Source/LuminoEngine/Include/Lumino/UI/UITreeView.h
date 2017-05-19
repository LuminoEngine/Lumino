
#pragma once
#include "UIControl.h"

LN_NAMESPACE_BEGIN
namespace tr
{
class UITreeViewItem;
class UITreeView;
using UITreeViewItemPtr = RefPtr<UITreeViewItem>;
using UITreeViewPtr = RefPtr<UITreeView>;

/**
	@brief
*/
class UITreeViewItem
	: public UIControl
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	void SetHeader(UIElement* header);

protected:

	// UIElement interface
	virtual Size MeasureOverride(const Size& constraint) override;
	virtual Size ArrangeOverride(const Size& finalSize) override;

LN_CONSTRUCT_ACCESS:
	UITreeViewItem();
	virtual ~UITreeViewItem();
	void Initialize();

private:
	RefPtr<UIElement>	m_header;
};

/**
	@brief
*/
class UITreeView
	: public UIControl
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	static UITreeViewPtr Create();

//public:
//	UITreeViewItemPtr AddTextItem(const String& text);
//	UITreeViewItemPtr AddItem(UIElement* item);
//
LN_PROTECTED_INTERNAL_ACCESS:
	UITreeView();
	virtual ~UITreeView();
	void Initialize();

private:
};

} // namespace tr
LN_NAMESPACE_END
