
#pragma once
#include "Common.hpp"
#include "../Rendering/RenderView.hpp"
#include "TransformControls.hpp"

namespace ln {
class World;
class Camera;
class RenderingContext;
class MeshModel;
class MeshResource;
class PostEffect;
class FilmicPostEffect;
namespace detail {
class PostEffectRenderer;
class SceneRenderingPipeline;
class InternalSkyBox;
class InternalSkyDome;
}

/**
 * 3D シーンを描画するための RenderView です。
 * 
 * ClearMode のデフォルトは ColorAndDepth です。
 */
LN_CLASS()
class WorldRenderView
	: public RenderView
{
    LN_OBJECT;
public:
    void setTargetWorld(World* world);
	void setCamera(Camera* camera);

	void addPostEffect(PostEffect* effect);
	void removePostEffect(PostEffect* effect);


    /** この WorldRenderView が描画する 3D シーン上に、グリッドを表示するかどうかを設定します。 */
    LN_METHOD(Property)
	void setGuideGridEnabled(bool value) { m_visibleGridPlane = value; }

    /** この WorldRenderView が描画する 3D シーン上に、グリッドを表示するかどうかを取得します。 */
    LN_METHOD(Property)
	bool guideGridEnabled() const { return m_visibleGridPlane; }

    void setPhysicsDebugDrawEnabled(bool value) { m_physicsDebugDrawEnabled = value; }
	bool physicsDebugDrawEnabled() const { return m_physicsDebugDrawEnabled; }

    void setGizmoEnabled(bool value) { m_gizmoEnabled = value; }
    bool gizmoEnabled() const { return m_gizmoEnabled; }

    void setHDREnabled(bool value) { m_hdrEnabled = value; }
    bool isHDREnabled() const { return m_hdrEnabled; }

    const Ref<TransformControls>& transformControls() const{ return m_transformControls; }

    // TODO: internal
    virtual void render(GraphicsCommandList* graphicsContext, RenderTargetTexture* renderTarget) override;
    const Ref<detail::SceneRenderingPipeline>& sceneRenderingPipeline() const { return m_sceneRenderingPipeline; }
    const Ref<FilmicPostEffect>& finishingProcess() const { return m_finishingProcess; }

    WorldObject* findObjectInPoint(int x, int y);

protected:
	virtual void onUpdateFrame(float elapsedSeconds) override;
    virtual void onRoutedEvent(UIEventArgs* e) override;

LN_CONSTRUCT_ACCESS:
    WorldRenderView();
	virtual ~WorldRenderView();
	void init();

private:
    detail::PostEffectRenderer* acquirePostEffectPresenter();
    void createGridPlane();
    void renderGridPlane(RenderingContext* renderingContext, RenderView* renderView);
    void adjustGridPlane(const ViewFrustum& viewFrustum, RenderView* renderView);

    Ref<detail::SceneRenderingPipeline> m_sceneRenderingPipeline;
    Ref<World> m_targetWorld;
	Ref<Camera> m_camera;
    Ref<RenderViewPoint> m_viewPoint;
	Ref<detail::PostEffectRenderer> m_imageEffectRenderer;

    Ref<Material> m_clearMaterial;
	Ref<MeshModel> m_skyProjectionPlane;

    Ref<MeshResource> m_gridPlaneMesh;
    Ref<Material> m_gridPlaneMaterial;
    bool m_visibleGridPlane;
    bool m_physicsDebugDrawEnabled;
    bool m_gizmoEnabled;

	Ref<detail::InternalSkyBox> m_internalSkyBox;
    Ref<detail::InternalSkyDome> m_internalSkyDome;
    Ref<FilmicPostEffect> m_finishingProcess;
    Ref<TransformControls> m_transformControls; // TODO: gizmo でまとめる？

    // Unity だと HDR の設定は Camera につく。Viewport を使って画面を分割したとき、
    // HDR ON/OFF の画面を同時にバックバッファに書いたりできる。
    // 実際のところそんな用途があるかはわからないけど、ひとまず同じ実装にしておく。
    Ref<RenderTargetTexture> m_hdrRenderTarget;
    bool m_hdrEnabled = false;
};

} // namespace ln
