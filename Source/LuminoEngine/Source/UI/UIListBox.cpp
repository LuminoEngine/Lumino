
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
	setHContentAlignment(HAlignment::Left);
	setHAlignment(HAlignment::Stretch);
	goToVisualState(UIVisualStates::NormalState);

	// TODO:
	setMinHeight(16);
}

//------------------------------------------------------------------------------
void UIListBoxItem::onMouseDown(UIMouseEventArgs* e)
{
	if (e->getClickCount() == 2)
	{
		submit();
	}

	UIControl::onMouseDown(e);
}

//==============================================================================
// UIListBox
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIListBox, UIControl)

const String UIListBox::NormalState = _T("Normal");

//------------------------------------------------------------------------------
UIListBoxPtr UIListBox::create()
{
	auto ptr = UIListBoxPtr::makeRef();
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
	setHContentAlignment(HAlignment::Stretch);

	auto panel = newObject<UIStackPanel>();
	panel->setHAlignment(HAlignment::Stretch);
	panel->setVAlignment(VAlignment::Stretch);
	setLayoutPanel(panel);
	goToVisualState(NormalState);
}

//------------------------------------------------------------------------------
UIListBoxItemPtr UIListBox::addTextItem(const String& text)
{
	auto textBlock = newObject<UITextBlock>();
	textBlock->setText(text);
	return addItem(textBlock);
}

//------------------------------------------------------------------------------
UIListBoxItemPtr UIListBox::addItem(UIElement* item)
{
	if (LN_CHECK_ARG(item != nullptr)) return nullptr;

	// 受け取った item を UIListBoxItem でラップして、UIListBoxItem をリストに入れる
	auto listItem = newObject<UIListBoxItem>();
	listItem->addChild(item);
	addChild(listItem);
	return listItem;
}

LN_NAMESPACE_END
