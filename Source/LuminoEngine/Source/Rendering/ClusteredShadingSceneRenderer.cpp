
#include "../Internal.h"
#include "ClusteredShadingSceneRenderer.h"

LN_NAMESPACE_BEGIN
namespace detail {

//==============================================================================
// ClusteredShadingSceneRenderer
//==============================================================================

ClusteredShadingSceneRenderer::ClusteredShadingSceneRenderer()
{
}

ClusteredShadingSceneRenderer::~ClusteredShadingSceneRenderer()
{
}

void ClusteredShadingSceneRenderer::initialize(GraphicsManager* manager)
{
	SceneRenderer::initialize(manager);
}

void ClusteredShadingSceneRenderer::prepare()
{
	SceneRenderer::prepare();
}

} // namespace detail
LN_NAMESPACE_END

