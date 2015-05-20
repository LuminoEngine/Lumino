
#include "../Internal.h"
#include "DebugRenderer.h"
#include "Camera.h"
#include "SceneGraphManager.h"
#include "InfomationRenderingPass.h"

namespace Lumino
{
namespace Scene
{

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
	DebugRenderer dr(params.GeometryRenderer);
	params.GeometryRenderer->SetViewProjTransform(params.CurrentCamera->GetViewProjectionMatrix());
	params.GeometryRenderer->BeginPass();
	m_manager->GetPhysicsManager()->DrawDebugShapes(&dr);
	params.GeometryRenderer->EndPass();
}

} // namespace Scene
} // namespace Lumino
