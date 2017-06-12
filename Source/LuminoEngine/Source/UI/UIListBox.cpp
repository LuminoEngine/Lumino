
#include "Internal.h"
#include <Lumino/UI/UITextBlock.h>
#include <Lumino/UI/UIListBox.h>
#include <Lumino/UI/UILayoutPanel.h>	// TODO: stackpanel にするかも
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIListBoxItem
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIListBoxItem, UIControl)

//------------------------------------------------------------------------------
UIListBoxItem::UIListBoxItem()
{
}

//------------------------------------------------------------------------------
UIListBoxItem::~UIListBoxItem()
{
}

//------------------------------------------------------------------------------
void UIListBoxItem::initialize()
{
	UIControl::initialize();
	SetHContentAlignment(HAlignment::Left);
	SetHAlignment(HAlignment::Stretch);
	GoToVisualState(UIVisualStates::NormalState);

	// TODO:
	SetMinHeight(16);
}

//==============================================================================
// UIListBox
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIListBox, UIControl)

const String UIListBox::NormalState = _T("Normal");

//------------------------------------------------------------------------------
UIListBoxPtr UIListBox::Create()
{
	auto ptr = UIListBoxPtr::MakeRef();
	ptr->initialize();
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
void UIListBox::initialize()
{
	UIControl::initialize();
	SetHContentAlignment(HAlignment::Stretch);

	auto panel = RefPtr<UIStackPanel>::MakeRef();
	panel->initialize();
	panel->SetHAlignment(HAlignment::Stretch);
	panel->SetVAlignment(VAlignment::Stretch);
	SetLayoutPanel(panel);
	GoToVisualState(NormalState);
}

//------------------------------------------------------------------------------
UIListBoxItemPtr UIListBox::AddTextItem(const String& text)
{
	auto textBlock = RefPtr<UITextBlock>::MakeRef();
	textBlock->initialize();
	textBlock->SetText(text);
	return AddItem(textBlock);
}

//------------------------------------------------------------------------------
UIListBoxItemPtr UIListBox::AddItem(UIElement* item)
{
	if (LN_CHECK_ARG(item != nullptr)) return nullptr;

	// 受け取った item を UIListBoxItem でラップして、UIListBoxItem をリストに入れる
	auto listItem = RefPtr<UIListBoxItem>::MakeRef();
	listItem->initialize();
	listItem->AddChild(item);
	AddChild(listItem);
	return listItem;
}

LN_NAMESPACE_END
