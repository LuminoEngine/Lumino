
#include "Internal.h"
#include <Lumino/UI/UITextBlock.h>
#include <Lumino/UI/UIListBox.h>
#include <Lumino/UI/UIPanel.h>	// TODO: stackpanel にするかも
#include "UIManager.h"

LN_NAMESPACE_BEGIN
namespace tr
{

//==============================================================================
// UIListBoxItem
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIListBoxItem, UIContentControl)

const String UIListBoxItem::NormalState = _T("Normal");
const String UIListBoxItem::MouseOverState = _T("MouseOver");

//------------------------------------------------------------------------------
UIListBoxItem::UIListBoxItem()
{
}

//------------------------------------------------------------------------------
UIListBoxItem::~UIListBoxItem()
{
}

//------------------------------------------------------------------------------
void UIListBoxItem::Initialize(ln::detail::UIManager* manager)
{
	UIContentControl::Initialize(manager);
	SetHorizontalAlignment(HorizontalAlignment::Left);
	SetVerticalAlignment(VerticalAlignment::Center);
	GoToVisualState(NormalState);
}

//------------------------------------------------------------------------------
void UIListBoxItem::OnRoutedEvent(const UIEventInfo* ev, UIEventArgs* e)
{
	if (ev == UIElement::MouseEnterEvent)
	{
		GoToVisualState(MouseOverState);
	}
	else if (ev == UIElement::MouseLeaveEvent)
	{
		GoToVisualState(NormalState);
	}
}

//==============================================================================
// UIListBox
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIListBox, UIItemsControl)

//------------------------------------------------------------------------------
UIListBoxPtr UIListBox::Create()
{
	auto ptr = UIListBoxPtr::MakeRef();
	ptr->Initialize(ln::detail::UIManager::GetInstance());
	return ptr;
}

//------------------------------------------------------------------------------
UIListBox::UIListBox()
{
}

//------------------------------------------------------------------------------
UIListBox::~UIListBox()
{
}

//------------------------------------------------------------------------------
void UIListBox::Initialize(ln::detail::UIManager* manager)
{
	UIItemsControl::Initialize(manager);

	auto panel = RefPtr<UIStackPanel>::MakeRef();
	panel->Initialize(manager);
	SetItemsHostPanel(panel);
}

//------------------------------------------------------------------------------
UIListBoxItemPtr UIListBox::AddTextItem(const String& text)
{
	auto textBlock = RefPtr<UITextBlock>::MakeRef();
	textBlock->Initialize(GetManager());
	textBlock->SetText(text);
	return AddItem(textBlock);
}

//------------------------------------------------------------------------------
UIListBoxItemPtr UIListBox::AddItem(UIElement* item)
{
	LN_CHECK_ARG(item != nullptr);

	// 受け取った item を UIListBoxItem でラップして、UIListBoxItem をリストに入れる
	auto listItem = RefPtr<UIListBoxItem>::MakeRef();
	listItem->Initialize(GetManager());
	listItem->SetContent(item);
	GetItems()->Add(listItem);
	return listItem;
}

} // namespace tr
LN_NAMESPACE_END
