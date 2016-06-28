
#include "../Internal.h"
#include <Lumino/Graphics/SpriteRenderer.h>
#include <Lumino/Scene/SceneGraphRenderingContext.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// SceneGraphRenderingContext
//==============================================================================

//------------------------------------------------------------------------------
SceneGraphRenderingContext::SceneGraphRenderingContext(RenderingContext* renderingContext, GraphicsContext* internalContext)
	: m_renderingContext(renderingContext)
	, m_internalContext(internalContext)
{
}

//------------------------------------------------------------------------------
SceneGraphRenderingContext::~SceneGraphRenderingContext()
{
}

void SceneGraphRenderingContext::Flush()
{
	return m_internalContext->Flush();
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
