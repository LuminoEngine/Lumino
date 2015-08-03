
#include "Internal.h"
#include <Lumino/Application.h>
#include "ApplicationContext.h"

namespace Lumino
{

//=============================================================================
// ApplicationContext
//=============================================================================

static Application* g_app = NULL;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ApplicationContext::SetCurrent(Application* app)
{
	g_app = app;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Application* ApplicationContext::GetCurrent()
{
	return g_app;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Graphics::GraphicsManager* ApplicationContext::GetGraphicsManager()
{
	LN_CHECK_STATE_RETURNV(g_app != NULL, NULL);
	return g_app->GetGraphicsManager();
}

} // namespace Lumino
