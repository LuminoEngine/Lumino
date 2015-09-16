
#include "Internal.h"
#include "ApplicationImpl.h"
#include "ApplicationContext.h"

namespace Lumino
{

//=============================================================================
// ApplicationContext
//=============================================================================

static ApplicationImpl* g_app = NULL;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ApplicationContext::SetCurrent(ApplicationImpl* app)
{
	g_app = app;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ApplicationImpl* ApplicationContext::GetCurrent()
{
	return g_app;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsManager* ApplicationContext::GetGraphicsManager()
{
	LN_CHECK_STATE_RETURNV(g_app != NULL, NULL);
	return g_app->GetGraphicsManager();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GUIManagerImpl* ApplicationContext::GetGUIManager()
{
	LN_CHECK_STATE_RETURNV(g_app != NULL, NULL);
	return g_app->GetGUIManager();
}

} // namespace Lumino
