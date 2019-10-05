#pragma once
#include <LuminoEngine/Rendering/RenderView.hpp>

namespace ln {
class GraphicsContext;
namespace detail {
class ClusteredShadingSceneRenderer;
class UnLigitingSceneRenderer;

/*
 * 描画リストやポストエフェクトなど描画に関わる全てのパラメータを受け取り、
 * RenderView から覗くことができる 1 枚の絵を描き上げる。
 *
 * 将来的な話だが、Deffered と Forward 混在のような複数の SceneRenderer をコントロールするのはこのクラスの役目。
 */
class RenderingPipeline
	: public RefObject
{
public:
    void init();

    const SizeI& renderingFrameBufferSize() const { return m_renderingFrameBufferSize; }
    //const Ref<detail::FrameBufferCache>& frameBufferCache() const { return m_frameBufferCache; }

    const List<detail::DrawElementListCollector*>* elementListManagers() const { return m_elementListManagers; }

protected:
    void clear(GraphicsContext* graphicsContext, RenderTargetTexture* renderTarget, const ClearInfo& clearInfo);

    //Ref<detail::FrameBufferCache> m_frameBufferCache;
    SizeI m_renderingFrameBufferSize;	// render() の内側だけで使える
    const List<detail::DrawElementListCollector*>* m_elementListManagers;
    Ref<RenderPass> m_clearRenderPass;
};

class SceneRenderingPipeline
    : public RenderingPipeline
{
public:
    SceneRenderingPipeline();
    virtual ~SceneRenderingPipeline();

    void init();
    void render(
        GraphicsContext* graphicsContext,
		RenderTargetTexture* renderTarget,
        const ClearInfo& clearInfo,
        const detail::CameraInfo* mainCameraInfo,
        const List<detail::DrawElementListCollector*>* elementListManagers);


private:
    Ref<detail::ClusteredShadingSceneRenderer> m_sceneRenderer;
    Ref<detail::UnLigitingSceneRenderer> m_sceneRenderer_ImageEffectPhase;
};

// ライティングしない Pipeline。UI で使う。
class FlatRenderingPipeline
    : public RenderingPipeline
{
public:
	FlatRenderingPipeline();
	void init();
    void render(
        GraphicsContext* graphicsContext,
        RenderTargetTexture* renderTarget,
        const ClearInfo& clearInfo,
		const detail::CameraInfo* mainCameraInfo,
		const List<detail::DrawElementListCollector*>* elementListManagers);

private:
	Ref<detail::UnLigitingSceneRenderer> m_sceneRenderer;
    Ref<detail::UnLigitingSceneRenderer> m_sceneRenderer_ImageEffectPhase;
};

} // namespace detail
} // namespace ln

