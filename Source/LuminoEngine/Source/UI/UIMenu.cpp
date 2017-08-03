
#include "Internal.h"
#include <Lumino/UI/UIMenu.h>
#include <Lumino/UI/UIComboBox.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UIMenuItem
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIMenuItem, UIHeaderedItemsControl)

//------------------------------------------------------------------------------
UIMenuItem::UIMenuItem()
{
}

//------------------------------------------------------------------------------
UIMenuItem::~UIMenuItem()
{
}

//------------------------------------------------------------------------------
void UIMenuItem::initialize()
{
	UIHeaderedItemsControl::initialize();
}

Size UIMenuItem::measureOverride(const Size& constraint)
{
	return UIItemsControl::measureOverride(constraint);
}

Size UIMenuItem::arrangeOverride(const Size& finalSize)
{
	return UIItemsControl::arrangeOverride(finalSize);
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

//==============================================================================
// UIContextMenu
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIContextMenu, Object)

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
}

LN_NAMESPACE_END
