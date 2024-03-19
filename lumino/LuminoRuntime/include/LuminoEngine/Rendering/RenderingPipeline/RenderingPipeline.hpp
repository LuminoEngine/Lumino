#pragma once
#include "../../../../src/Rendering/RLIs/RLICulling.hpp"

namespace ln {
class GraphicsCommandList;
class RenderView;
namespace detail {
class ClusteredShadingSceneRenderer;
class UnLigitingSceneRenderer;
class UnLigitingSceneRendererPass;
} // namespace detail

/**
 * 描画リストや視点情報、ポストエフェクトなど描画に関わる全てのパラメータを受け取り、
 * RenderView から覗くことができる 1 枚の絵を描き上げる。
 */
class RenderingPipeline : public Object
{
public:
    void init();
    const SizeI& renderingFrameBufferSize() const { return m_renderingFrameBufferSize; }

protected:
    virtual void onPrepare(RenderView* renderView, RenderTargetTexture* renderTarget);
    virtual void onRender(
        GraphicsCommandList* graphicsContext,
        RenderingContext* renderingContext,
        RenderTargetTexture* renderTarget,
        const ClearInfo& mainPassClearInfo,
        const RenderView* renderView,
        detail::CommandListServer* commandListServer,
        const detail::SceneGlobalRenderParams* sceneGlobalParams) = 0;

    SizeI m_renderingFrameBufferSize; // render() の内側だけで使える
    detail::RLICulling m_cullingResult;

private:
    // TODO: renderTarget は RenderView::builtinRenderTarget(Current) で取りたい
    void prepare(RenderView* renderView, RenderTargetTexture* renderTarget);

    void render(
        GraphicsCommandList* graphicsContext,
        RenderingContext* renderingContext,
        RenderTargetTexture* renderTarget,
        const RenderView* renderView,
        const detail::SceneGlobalRenderParams* sceneGlobalParams);

    friend RenderView;
};

} // namespace ln

