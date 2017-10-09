
#pragma once
#include <Lumino/Rendering/SceneRenderer.h>

LN_NAMESPACE_BEGIN
namespace detail {


class ClusteredShadingGeometryRenderingPass
	: public RenderingPass2
{
public:
	ClusteredShadingGeometryRenderingPass();
	virtual ~ClusteredShadingGeometryRenderingPass();
	void initialize(GraphicsManager* manager);

private:
};

class ClusteredShadingSceneRenderer
	: public SceneRenderer
{
public:
	ClusteredShadingSceneRenderer();
	virtual ~ClusteredShadingSceneRenderer();
	void initialize(GraphicsManager* manager);

protected:
	virtual void prepare() override;
};

} // namespace detail
LN_NAMESPACE_END

