
#include "Internal.h"
#include <Lumino/UI/UITextBlock.h>
#include <Lumino/UI/UIListBox.h>
#include <Lumino/UI/UILayoutPanel.h>	// TODO: stackpanel にするかも
#include "UIManager.h"

LN_NAMESPACE_BEGIN
namespace tr
{

//==============================================================================
// UIListBoxItem
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIListBoxItem, UIContentControl)

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
	SetHContentAlignment(HAlignment::Left);
	SetHAlignment(HAlignment::Stretch);
	GoToVisualState(NormalState);
}

//==============================================================================
// UIListBox
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIListBox, UIControl)

const String UIListBox::NormalState = _T("Normal");

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
	UIControl::Initialize(manager);
	SetHContentAlignment(HAlignment::Stretch);

	auto panel = RefPtr<UIStackPanel>::MakeRef();
	panel->Initialize(manager);
	panel->SetHAlignment(HAlignment::Stretch);
	panel->SetVAlignment(VAlignment::Stretch);
	SetLayoutPanel(panel);
	GoToVisualState(NormalState);
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
