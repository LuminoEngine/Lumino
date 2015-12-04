
#include "../Internal.h"
#include "DebugRenderer.h"
#include <Lumino/Scene/SceneGraphRenderingContext.h>
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
void InfomationRenderingPass::RenderNode(SceneGraphRenderingContext* dc, SceneNode* node)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InfomationRenderingPass::RenderSubset(SceneGraphRenderingContext* dc, VisualNode* node, int subset)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InfomationRenderingPass::PostRender(SceneGraphRenderingContext* dc)
{
	if (m_manager->GetPhysicsManager() != NULL)
	{
		DebugRenderer dr(dc->GeometryRenderer);
		dc->GeometryRenderer->SetViewProjTransform(dc->CurrentCamera->GetViewProjectionMatrix());
		dc->GeometryRenderer->BeginPass();
		m_manager->GetPhysicsManager()->DrawDebugShapes(&dr);
		dc->GeometryRenderer->EndPass();
	}
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
