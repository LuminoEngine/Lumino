
#pragma once
#include "UIItemsControl.h"
#include "UIContentControl.h"

LN_NAMESPACE_BEGIN
namespace tr
{
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
	: public UIContentControl
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static const String NormalState;
	static const String MouseOverState;

LN_PROTECTED_INTERNAL_ACCESS:
	UIListBoxItem();
	virtual ~UIListBoxItem();
	void Initialize(ln::detail::UIManager* manager);

protected:
	virtual void OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e) override;
};

/**
	@brief
*/
class UIListBox
	: public UIItemsControl
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static UIListBoxPtr Create();

public:
	UIListBoxItemPtr AddTextItem(const String& text);
	UIListBoxItemPtr AddItem(UIElement* item);

LN_PROTECTED_INTERNAL_ACCESS:
	UIListBox();
	virtual ~UIListBox();
	void Initialize(ln::detail::UIManager* manager);

private:
};

} // namespace tr
LN_NAMESPACE_END
