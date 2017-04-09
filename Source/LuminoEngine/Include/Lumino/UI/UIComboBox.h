
#pragma once
#include "UIItemsControl.h"
#include "UIContentControl.h"

LN_NAMESPACE_BEGIN
namespace tr
{
class UIComboBoxItem;
class UIComboBox;
using UIComboBoxItemPtr = RefPtr<UIComboBoxItem>;
using UIComboBoxPtr = RefPtr<UIComboBox>;


/**
	@brief
*/
class UIPopup
	: public UIContentControl
{
	LN_UI_TYPEINFO_DECLARE();
public:

LN_PROTECTED_INTERNAL_ACCESS:
	UIPopup();
	virtual ~UIPopup();
	void Initialize(ln::detail::UIManager* manager);
};


/**
	@brief
*/
class UIComboBoxItem
	: public UIContentControl
{
	LN_UI_TYPEINFO_DECLARE();
public:

LN_PROTECTED_INTERNAL_ACCESS:
	UIComboBoxItem();
	virtual ~UIComboBoxItem();
	void Initialize(ln::detail::UIManager* manager);
};

/**
	@brief
*/
class UIComboBox
	: public UIControl
{
	LN_UI_TYPEINFO_DECLARE();
public:
	static UIComboBoxPtr Create();

public:
	UIComboBoxItemPtr AddTextItem(const String& text);
	UIComboBoxItemPtr AddItem(UIElement* item);

protected:
	virtual void OnMouseDown(UIMouseEventArgs* e) override;

LN_PROTECTED_INTERNAL_ACCESS:
	UIComboBox();
	virtual ~UIComboBox();
	void Initialize(ln::detail::UIManager* manager);

private:
};

} // namespace tr
LN_NAMESPACE_END
