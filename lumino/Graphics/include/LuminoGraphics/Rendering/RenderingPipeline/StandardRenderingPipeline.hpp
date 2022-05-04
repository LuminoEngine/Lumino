#pragma once
#include "RenderingPipeline.hpp"

namespace ln {

class SceneRenderingPipeline
    : public RenderingPipeline
{
public:
    SceneRenderingPipeline();
    virtual ~SceneRenderingPipeline();

    void init();

    const Ref<RenderTargetTexture>& viweNormalAndDepthBuffer() const { return m_viweNormalAndDepthBuffer; }
    const Ref<RenderTargetTexture>& viweDepthBuffer() const { return m_viweDepthBuffer; }
    const Ref<RenderTargetTexture>& materialBuffer() const { return m_materialBuffer; }
    const Ref<RenderTargetTexture>& objectIdBuffer() const { return m_objectIdBuffer; }
    const Ref<RenderTargetTexture>& shadowMap() const { return m_shadowMap; }
    const Ref<DepthBuffer>& shadowMapDepthBuffer() const { return m_shadowMapDepthBuffer; }
    const Ref<RenderTargetTexture>& hdrRenderTarget() const { return m_hdrRenderTarget; }

    void setHDREnabled(bool value) { m_hdrEnabled = value; }
    bool isHDREnabled() const { return m_hdrEnabled; }

protected:
    void onPrepare(RenderView* renderView, RenderTargetTexture* renderTarget) override;
    void onRender(
        GraphicsCommandList* graphicsContext,
        RenderingContext* renderingContext,
        RenderTargetTexture* renderTarget,
        const ClearInfo& mainPassClearInfo,
        const RenderView* renderView,
        detail::CommandListServer* commandListServer,
        const detail::SceneGlobalRenderParams* sceneGlobalParams) override;

private:
    // Note: 複数の SceneRenderer を持っているのは、描画フェーズの大きなまとまりごとに、Element の扱いを変えることで最適化するため。
    // 例えば、http://cdn-ak.f.st-hatena.com/images/fotolife/h/hecomi/20160206/20160206190644.png や
    // https://docs.unity3d.com/ja/current/Manual/GraphicsCommandBuffers.html の図を考えるとき、
    // - DeferredRenderer では、Element を手前から奥にソートしたい。Sky, DebugDraw, PostEffect はここでは書きたくないのでソートの対象にもしたくない。
    // - ForwardRenderer では、Element を奥から手前にソートしたい。Sky, DebugDraw, PostEffect はここでは書きたくないのでソートの対象にもしたくない。
    // - 後段の統合 Renderer では基本的にソートはしたくない。ライティングも不要
    // - 最後の UIRenderer は 2D スプライトの考え方でソートしたい。(今はサポートしてないけど)

    Ref<detail::ClusteredShadingSceneRenderer> m_sceneRenderer;
    Ref<detail::SceneRenderer> m_sceneRenderer_PostEffectPhase;
    Ref<detail::UnLigitingSceneRendererPass> m_unlitRendererPass_Normal;
    Ref<detail::UnLigitingSceneRendererPass> m_unlitRendererPass_PostEffect;

    // rgb: Packed view space normal, a: depth (near=0.0 ~ far=1.0)
    // TODO: UE4 や Unity など、多くは WorldSpace の normal を G-Buffer に書き込んでいる。
    //       ここでは主にポストエフェクトに使う用に、view space normal を作っているが、必要になり次第、 world 用のも作ったほうがいいかも
    Ref<RenderTargetTexture> m_viweNormalAndDepthBuffer;

    // r: Clip space depth (near:0.0~far:1.0), g: View space linear depth (near:0.0~far:1.0)
    // r と g の違いは、プロジェクション変換されているかどうか。
    // r は変換済みで、対数グラフのようにカーブを描く。
    // g は View space Z をカメラの far - near で割っただけの値。 
    Ref<RenderTargetTexture> m_viweDepthBuffer;

    // 
    Ref<RenderTargetTexture> m_materialBuffer;

    // RT にオブジェクトIDを書いて読み出す手法は "GPU picking" というらしい
    // https://stackoverflow.com/questions/55462615/three-js-raycast-on-skinning-mesh
    // R32S を利用しているが、RGBでオブジェクトIDを作ることもできる。
    // https://riptutorial.com/three-js/example/17089/object-picking---gpu
    Ref<RenderTargetTexture> m_objectIdBuffer;

    Ref<RenderTargetTexture> m_shadowMap;
    Ref<DepthBuffer> m_shadowMapDepthBuffer;

    Ref<SamplerState> m_samplerState;	// TODO: 共通化
    Ref<RenderPass> m_renderPass;

    // Unity だと HDR の設定は Camera につく。Viewport を使って画面を分割したとき、
    // HDR ON/OFF の画面を同時にバックバッファに書いたりできる。
    // 実際のところそんな用途があるかはわからないけど、ひとまず同じ実装にしておく。
    Ref<RenderTargetTexture> m_hdrRenderTarget;
    bool m_hdrEnabled = false;
};

} // namespace ln

