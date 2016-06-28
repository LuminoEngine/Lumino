
#include "../Internal.h"
#include <Lumino/Graphics/SpriteRenderer.h>
#include <Lumino/Scene/SceneGraphRenderingContext.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// SceneGraphRenderingContext
//==============================================================================

//------------------------------------------------------------------------------
SceneGraphRenderingContext::SceneGraphRenderingContext(GraphicsContext* graphicsContext)
	: m_graphicsContext(graphicsContext)
{
}

//------------------------------------------------------------------------------
SceneGraphRenderingContext::~SceneGraphRenderingContext()
{
}

void SceneGraphRenderingContext::Flush()
{
	return m_graphicsContext->Flush();
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
