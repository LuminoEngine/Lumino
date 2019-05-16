
#pragma once
#include "Common.hpp"
#include "../Rendering/RenderView.hpp"

namespace ln {
class World;
class Camera;
class RenderingContext;
class StaticMeshModel;
namespace detail {
class SceneRenderingPipeline;
}

class WorldRenderView
	: public RenderView
{
public:
    void setTargetWorld(World* world);
	void setCamera(Camera* camera);

    void setPhysicsDebugDrawEnabled(bool value) { m_physicsDebugDrawEnabled = value; }

    // TODO: internal
    virtual void render(GraphicsContext* graphicsContext) override;

protected:
    //virtual void onRoutedEvent(UIEventArgs* e) override;

LN_CONSTRUCT_ACCESS:
    WorldRenderView();
	virtual ~WorldRenderView();
	void init();

private:
    void createGridPlane();
    void renderGridPlane(RenderingContext* renderingContext, RenderView* renderView);
    void adjustGridPlane(const ViewFrustum& viewFrustum, RenderView* renderView);

    Ref<detail::SceneRenderingPipeline> m_sceneRenderingPipeline;
    Ref<detail::DrawElementListCollector> m_drawElementListCollector;
    Ref<World> m_targetWorld;
	Ref<Camera> m_camera;
    Ref<RenderViewPoint> m_viewPoint;

    Ref<StaticMeshModel> m_gridPlane;
    bool m_visibleGridPlane;
    bool m_physicsDebugDrawEnabled;
};

} // namespace ln
