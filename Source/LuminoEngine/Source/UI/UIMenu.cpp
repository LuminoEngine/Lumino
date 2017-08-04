
#include "Internal.h"
#include <Lumino/UI/UIMenu.h>
#include <Lumino/UI/UILayoutPanel.h>
#include <Lumino/UI/UIComboBox.h>
#include <Lumino/UI/UITextBlock.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UIMenuItem
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIMenuItem, UIHeaderedItemsControl)

UIMenuItem::UIMenuItem()
{
}

UIMenuItem::~UIMenuItem()
{
}

void UIMenuItem::initialize()
{
	UIHeaderedItemsControl::initialize();
}

Size UIMenuItem::measureOverride(const Size& constraint)
{
	return UIHeaderedItemsControl::measureOverride(constraint);
}

Size UIMenuItem::arrangeOverride(const Size& finalSize)
{
	return UIHeaderedItemsControl::arrangeOverride(finalSize);
}

//==============================================================================
// UIMenuBase
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIMenuBase, UIItemsControl)

UIMenuBase::UIMenuBase()
{
}

UIMenuBase::~UIMenuBase()
{
}

void UIMenuBase::initialize()
{
	UIItemsControl::initialize();
}

UIMenuItem* UIMenuBase::addMenuItem(const StringRef& text, const Delegate<void()>& handler)
{
	auto textBlock = newObject<UITextBlock>();
	textBlock->setText(text);

	auto item = newObject<UIMenuItem>();
	item->setHeader(textBlock);
	item->m_handler = handler;
	addItem(item);

	return item;
}

//==============================================================================
// UIContextMenu
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIContextMenu, UIMenuBase)

UIContextMenu::UIContextMenu()
	: m_popup(nullptr)
{
}

UIContextMenu::~UIContextMenu()
{
}

void UIContextMenu::initialize()
{
	UIMenuBase::initialize();

	m_popup = newObject<UIPopup>();	// TODO: Manager でキャッシュとかしておきたい

	writeCoreFlag(detail::UICoreFlags_LogicalChildrenPresenterAutoManagement, false);
	auto panel = newObject<UIStackPanel>();
	setLogicalChildrenPresenter(panel);
}

void UIContextMenu::open(UIElement* owner)
{
	m_popup->setSize(Size(50, 50));		// TODO:
	m_popup->setBackground(Brush::Red);	// TODO:
	m_popup->setContent(getLogicalChildrenPresenter());
	m_popup->open(owner);
}

LN_NAMESPACE_END
