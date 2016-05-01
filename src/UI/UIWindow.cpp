
#include "Internal.h"
#include <Lumino/UI/UIWindow.h>

LN_NAMESPACE_BEGIN

//=============================================================================
// UIWindow
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIWindow::UIWindow()
	: m_manager(nullptr)
	, m_platformWindow(nullptr)
	//, m_backBufferViewport(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIWindow::~UIWindow()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIWindow::InitializeDefault(detail::UIManager* manager, PlatformWindow* platformWindow)
{
	LN_CHECK_ARGS_RETURN(manager != nullptr);
	m_manager = manager;
	m_platformWindow = platformWindow;
}

LN_NAMESPACE_END
