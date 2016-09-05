
#include "../Internal.h"
#include <Lumino/Scene/SceneGraphRenderingContext.h>
#include "../Graphics/SpriteRenderer.h"
#include "SceneGraphManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// SceneGraphRenderingContext
//==============================================================================

//------------------------------------------------------------------------------
SceneGraphRenderingContext::SceneGraphRenderingContext()
{
}

//------------------------------------------------------------------------------
SceneGraphRenderingContext::~SceneGraphRenderingContext()
{
}

//------------------------------------------------------------------------------
void SceneGraphRenderingContext::Initialize(SceneGraphManager* manager)
{
	RenderingContext::Initialize(manager->GetGraphicsManager());
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
