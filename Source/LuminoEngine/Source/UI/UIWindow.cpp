
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
	if (LN_CHECK_ARG(manager != nullptr)) return;
	m_manager = manager;
}

LN_NAMESPACE_END
