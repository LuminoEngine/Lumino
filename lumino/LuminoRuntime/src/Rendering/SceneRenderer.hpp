#pragma once
#include <LuminoEngine/Graphics/RenderState.hpp>
#include <LuminoEngine/GPU/Shader.hpp>
#include <LuminoEngine/Rendering/Common.hpp>
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/Rendering/Kanata/KDrawCommand.hpp>
#include "RLIs/Common.hpp"
#include "RLIs/RLIBatchList.hpp"

namespace ln {
class Material;
namespace detail {
class RenderStage;
class RenderDrawElement;


class SceneRendererPass
    : public RefObject {
public:
    SceneRendererPass();
    virtual ~SceneRendererPass();
    void init();

    void setClearInfo(const ClearInfo& value) { m_clearInfo = value; }
    const ClearInfo& clearInfo() const { return m_clearInfo; }

    virtual void onBeginRender(SceneRenderer* sceneRenderer, GraphicsCommandList* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer);
    virtual void onEndRender(SceneRenderer* sceneRenderer);

    // このパスのデフォルトの RenderPass を構築する。
    // renderTarget と depthBuffer は、シーンレンダリングの最終出力先。
    // もし G-Buffer を作るときなど、内部的な RenderTarget に書きたい場合はこれらは使用せずに派生側で各種バッファを用意する。
    // virtual void onBeginPass(SceneRenderer* sceneRenderer) = 0;

    // このパスを実行するときのデフォルトの RenderPass を取得する。
    // インスタンスは、このパスの実行側の onBeginPass() で作っておく。
    virtual RenderPass* renderPass() const = 0;

    virtual bool filterElement(RenderDrawElement* element) const;

    virtual void overrideFinalMaterial(RLIMaterial* material) {}

    // Element の情報と派生 Pass から、最終的に使いたい ShaderTechnique を求める
    virtual ShaderTechnique* selectShaderTechnique(
        const ShaderTechniqueRequestClasses& requester,
        Shader* requestedShader,
        ShadingModel requestedShadingModel) = 0;

    RenderingManager* manager() const { return m_manager; }

    static kokage::ShaderTechniqueClass_ShadingModel tlanslateShadingModel(ShadingModel value) { return (kokage::ShaderTechniqueClass_ShadingModel)value; }

    virtual kanata::SceneRenderPass* internalSceneRenderPass() const {
        LN_UNREACHABLE();
        return nullptr;
    }

protected:
    // static ShaderTechnique* selectShaderTechniqueHelper(
    //	const ShaderTechniqueRequestClasses& requester,
    //	Shader* requestedShader,
    //	ShadingModel requestedShadingModel,
    //	Shader* defaultShader,
    //	ShaderTechnique* defaultTechnique,
    //	ShaderTechniqueClass_Phase phase);

private:
    RenderingManager* m_manager;
    ClearInfo m_clearInfo;
};

class SceneRenderer
    : public RefObject {
public:
    // render の前準備として、効率的な描画を行うためのZソートなどを実施した Element リストを作成する。
    void prepare(
        GraphicsCommandList* graphicsContext,
        RenderingPipeline* renderingPipeline,
        RenderingContext* renderingContext,
        // detail::CommandListServer* commandListServer,
        const detail::RenderViewInfo& mainRenderViewInfo,
        RenderPart targetPhase,
        const detail::SceneGlobalRenderParams* sceneGlobalParams,
        const RLICulling* culling);

    void buildBatchList_Kanata(
        GraphicsCommandList* descriptorAllocator,
        const RLICulling* culling,
        RenderTargetTexture* renderTarget,
        DepthBuffer* depthBuffer,
        SceneRendererPass* pass);

    void render_Kanata(GraphicsCommandList* commandList);

    virtual SceneRendererPass* mainRenderPass() const { return nullptr; }

    RenderingPipeline* renderingPipeline() const { return m_renderingPipeline; }
    const detail::SceneGlobalRenderParams* sceneGlobalParams() const { return m_sceneGlobalRenderParams; }
    const detail::RenderViewInfo& mainRenderViewInfo() const { return m_mainRenderViewInfo; }
    const detail::DynamicLightInfo* mainLightInfo() const { return m_mainLightInfo; }

    SceneRenderer();
    void init();

public: // TODO:
    // void setDefaultMaterial(Material* material);

    // void addPass(SceneRendererPass* pass);

    // レンダリング準備として、描画に関係する各種オブジェクト (DrawElement や Light) を収集するフェーズ
    virtual void collect(RenderingPipeline* renderingPipeline, const CameraInfo& cameraInfo, RenderPart targetPhase);

    // レンダリング準備として、効率的な描画を行うために収集した各種オブジェクトのソートなどを行う
    void prepare();

    // virtual void onBeginRender() = 0;
    // virtual void onEndRender() = 0;

    virtual void onCollectLight(const DynamicLightInfo& light);
    virtual void onSetAdditionalShaderPassVariables2(ShaderDescriptor* descriptor, ShaderPass* shaderPass);

private:
    detail::RenderingManager* m_manager;

    RenderingPipeline* m_renderingPipeline;
    RenderingContext* m_renderingContext;
    const detail::SceneGlobalRenderParams* m_sceneGlobalRenderParams;
    RenderPart m_currentPart;

    // 1つのパイプラインの別フェーズで SceneRenderer を使うとき、
    // viewproj 行列を分けたいことがある (Default と PostEffect など) ため、SceneRenderer 側に実態で持つ
    RenderViewInfo m_mainRenderViewInfo;
    SceneInfo m_mainSceneInfo;
    const DynamicLightInfo* m_mainLightInfo = nullptr;

    URef<kanata::DrawCommandList> m_drawCommandList;
};

} // namespace detail
} // namespace ln
