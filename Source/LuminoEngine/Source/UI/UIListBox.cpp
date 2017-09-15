
#include "Internal.h"
#include <Lumino/UI/UITextBlock.h>
#include <Lumino/UI/UIListBox.h>
#include <Lumino/UI/UILayoutPanel.h>	// TODO: stackpanel にするかも
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIListBoxItem
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIListBoxItem, UIContentsControl)

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
	UIContentsControl::initialize();
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

	UIContentsControl::onMouseDown(e);
}

//==============================================================================
// UIListBox
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIListBox, UIContentsControl)

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
	UIContentsControl::initialize();
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
	if (LN_REQUIRE(item != nullptr)) return nullptr;

	// 受け取った item を UIListBoxItem でラップして、UIListBoxItem をリストに入れる
	auto listItem = newObject<UIListBoxItem>();
	listItem->addChild(item);
	addChild(listItem);
	return listItem;
}

LN_NAMESPACE_END
