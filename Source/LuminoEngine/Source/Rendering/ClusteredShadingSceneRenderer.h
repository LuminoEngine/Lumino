
#pragma once
#include <Lumino/Rendering/SceneRenderer.h>

LN_NAMESPACE_BEGIN
namespace detail {

class ClusteredShadingSceneRenderer
	: public SceneRenderer
{
public:
	ClusteredShadingSceneRenderer();
	virtual ~ClusteredShadingSceneRenderer();
	void initialize(GraphicsManager* manager);
};

} // namespace detail
LN_NAMESPACE_END

