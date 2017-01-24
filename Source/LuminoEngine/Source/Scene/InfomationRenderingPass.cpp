
#if 0
#include "../Internal.h"
#include "DebugRenderer.h"
#include <Lumino/Scene/Camera.h>
#include "SceneGraphManager.h"
#include "InfomationRenderingPass.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// RenderingPass
//==============================================================================

//------------------------------------------------------------------------------
InfomationRenderingPass::InfomationRenderingPass(SceneGraphManager* manager)
	: RenderingPass(manager)
{
}

//------------------------------------------------------------------------------
InfomationRenderingPass::~InfomationRenderingPass()
{
}

//------------------------------------------------------------------------------
void InfomationRenderingPass::RenderNode(SceneGraphRenderingContext* dc, SceneNode* node)
{
}

//------------------------------------------------------------------------------
void InfomationRenderingPass::RenderSubset(SceneGraphRenderingContext* dc, VisualNode* node, int subset)
{
}

//------------------------------------------------------------------------------
void InfomationRenderingPass::PostRender(SceneGraphRenderingContext* dc)
{
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
#endif
