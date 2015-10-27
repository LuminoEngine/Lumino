
#include "../Internal.h"
#include "DebugRenderer.h"
#include <Lumino/Scene/Camera.h>
#include "SceneGraphManager.h"
#include "InfomationRenderingPass.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//=============================================================================
// RenderingPass
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InfomationRenderingPass::InfomationRenderingPass(SceneGraphManager* manager)
	: RenderingPass(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InfomationRenderingPass::~InfomationRenderingPass()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InfomationRenderingPass::PostRender(RenderingParams& params)
{
	if (m_manager->GetPhysicsManager() != NULL)
	{
		DebugRenderer dr(params.GeometryRenderer);
		params.GeometryRenderer->SetViewProjTransform(params.CurrentCamera->GetViewProjectionMatrix());
		params.GeometryRenderer->BeginPass();
		m_manager->GetPhysicsManager()->DrawDebugShapes(&dr);
		params.GeometryRenderer->EndPass();
	}
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
