
#pragma once
#include "Common.hpp"
#include "../Rendering/RenderView.hpp"

namespace ln {
class World;
class Camera;
namespace detail {
class SceneRenderingPipeline;
}

class WorldRenderView
	: public RenderView
{
public:
    void setTargetWorld(World* world);
	void setCamera(Camera* camera);


    // TODO: internal
    virtual void render(GraphicsContext* graphicsContext) override;

LN_CONSTRUCT_ACCESS:
    WorldRenderView();
	virtual ~WorldRenderView();
	void initialize();

private:
    Ref<detail::SceneRenderingPipeline> m_sceneRenderingPipeline;
    Ref<detail::DrawElementListCollector> m_drawElementListCollector;
    Ref<World> m_targetWorld;
	Ref<Camera> m_camera;
    Ref<RenderViewPoint> m_viewPoint;
};

} // namespace ln
