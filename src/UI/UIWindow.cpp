
#include "Internal.h"
#include <Lumino/UI/UIWindow.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UIWindow
//==============================================================================

//------------------------------------------------------------------------------
UIWindow::UIWindow()
	: m_manager(nullptr)
{
}

//------------------------------------------------------------------------------
UIWindow::~UIWindow()
{
}

//------------------------------------------------------------------------------
void UIWindow::InitializeDefault(detail::UIManager* manager)
{
	LN_CHECK_ARGS_RETURN(manager != nullptr);
	m_manager = manager;
}

LN_NAMESPACE_END
