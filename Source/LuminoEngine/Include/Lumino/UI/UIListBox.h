
#pragma once
#include "UIItemsControl.h"
#include "UIContentControl.h"

LN_NAMESPACE_BEGIN
class UIListBoxItem;
class UIListBox;
using UIListBoxItemPtr = RefPtr<UIListBoxItem>;
using UIListBoxPtr = RefPtr<UIListBox>;

/**
	@brief
	@note		ListBoxItem の論理的な役目は、親の ListBox にマウス操作を伝えること。
				ListBox は ListBoxItem からの通知を受け取ったら、自分の設定 (複数選択可能か等) を考慮し、
				それぞれの Item の状態を更新する。
*/
class UIListBoxItem
	: public UIControl
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	virtual void onRender(DrawingContext* g) override
	{
		UIControl::onRender(g);
	}
LN_PROTECTED_INTERNAL_ACCESS:
	UIListBoxItem();
	virtual ~UIListBoxItem();
	void initialize();
};

/**
	@brief
*/
class UIListBox
	: public UIControl
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	static const String NormalState;

public:
	static UIListBoxPtr create();

public:
	UIListBoxItemPtr addTextItem(const String& text);
	UIListBoxItemPtr addItem(UIElement* item);

LN_PROTECTED_INTERNAL_ACCESS:
	UIListBox();
	virtual ~UIListBox();
	void initialize();

private:
};

LN_NAMESPACE_END
