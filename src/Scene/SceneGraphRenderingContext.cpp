
#include "../Internal.h"
#include <Lumino/Scene/SceneGraphRenderingContext.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//=============================================================================
// SceneGraphRenderingContext
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SceneGraphRenderingContext::SceneGraphRenderingContext(GraphicsManager* manager)
	: DrawingContext()
{
	Initialize(manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SceneGraphRenderingContext::~SceneGraphRenderingContext()
{
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
