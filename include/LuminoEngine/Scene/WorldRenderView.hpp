
#pragma once
#include "Common.hpp"
#include "../Rendering/RenderView.hpp"

namespace ln {
class World;
namespace detail {
class SceneRenderingPipeline;
}

class WorldRenderView
	: public RenderView
{
public:
    void setTargetWorld(World* world);


    // TODO: internal
    void render(GraphicsContext* graphicsContext);

LN_CONSTRUCT_ACCESS:
    WorldRenderView();
	virtual ~WorldRenderView();
	void initialize();

private:
    Ref<detail::SceneRenderingPipeline> m_sceneRenderingPipeline;
    Ref<detail::DrawElementListCollector> m_drawElementListCollector;
    Ref<World> m_targetWorld;
};

} // namespace ln
