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

    const detail::DrawElementListCollector* elementListCollector() const { return m_elementListCollector; }

protected:
    //void clear(GraphicsContext* graphicsContext, RenderTargetTexture* renderTarget, const ClearInfo& clearInfo);

    //Ref<detail::FrameBufferCache> m_frameBufferCache;
    SizeI m_renderingFrameBufferSize;	// render() の内側だけで使える
    detail::DrawElementListCollector* m_elementListCollector;
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
        const ClearInfo& mainPassClearInfo,
        const detail::CameraInfo* mainCameraInfo,
        detail::DrawElementListCollector* elementListCollector,
		const detail::SceneGlobalRenderParams* sceneGlobalParams);


private:
    // Note: 複数の SceneRenderer を持っているのは、描画フェーズの大きなまとまりごとに、Element の扱いを変えることで最適化するため。
    // 例えば、http://cdn-ak.f.st-hatena.com/images/fotolife/h/hecomi/20160206/20160206190644.png や
    // https://docs.unity3d.com/ja/current/Manual/GraphicsCommandBuffers.html の図を考えるとき、
    // - DeferredRenderer では、Element を手前から奥にソートしたい。Sky, DebugDraw, PostEffect はここでは書きたくないのでソートの対象にもしたくない。
    // - ForwardRenderer では、Element を奥から手前にソートしたい。Sky, DebugDraw, PostEffect はここでは書きたくないのでソートの対象にもしたくない。
    // - 後段の統合 Renderer では基本的にソートはしたくない。ライティングも不要
    // - 最後の UIRenderer は 2D スプライトの考え方でソートしたい。(今はサポートしてないけど)

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
        const ClearInfo& mainPassClearInfo,
		const detail::CameraInfo* mainCameraInfo,
        detail::DrawElementListCollector* elementListCollector);

private:
	Ref<detail::UnLigitingSceneRenderer> m_sceneRenderer;
    Ref<detail::UnLigitingSceneRenderer> m_sceneRenderer_ImageEffectPhase;
};

} // namespace detail
} // namespace ln

