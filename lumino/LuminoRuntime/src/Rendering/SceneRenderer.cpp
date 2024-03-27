
#include "Internal.hpp"
#include <LuminoEngine/Graphics/detail/GraphicsManager.hpp>
#include <LuminoEngine/GPU/ShaderDescriptor.hpp>
#include <LuminoEngine/GPU/RenderPass.hpp>
#include <LuminoEngine/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/GPU/SamplerState.hpp>
#include <LuminoEngine/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/Kanata/KSceneRenderPass.hpp>
#include <LuminoEngine/Mesh/MeshModeEntity.hpp>
#include "CommandListServer.hpp"
#include <LuminoEngine/Rendering/detail/RenderStage.hpp>
#include "RenderElement.hpp"
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include "SceneRenderer.hpp"
#include "RLIs/RLIMaterial.hpp"
#include "RLIs/RLICulling.hpp"

namespace ln {
namespace detail {

//==============================================================================
// SceneRendererPass

SceneRendererPass::SceneRendererPass()
    : m_manager(detail::RenderingManager::instance()) {
}

SceneRendererPass::~SceneRendererPass() {
}

void SceneRendererPass::init() {
}

void SceneRendererPass::onBeginRender(SceneRenderer* sceneRenderer, GraphicsCommandList* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) {
}

void SceneRendererPass::onEndRender(SceneRenderer* sceneRenderer) {
}

bool SceneRendererPass::filterElement(RenderDrawElement* element) const {
    if (element->flags().hasFlag(RenderDrawElementTypeFlags::LightDisc)) return false;

    return (element->flags() & (RenderDrawElementTypeFlags::Clear |
                                RenderDrawElementTypeFlags::Opaque |
                                RenderDrawElementTypeFlags::Transparent)) != RenderDrawElementTypeFlags::None;
}

// void SceneRendererPass::onBeginPass(GraphicsCommandList* context, FrameBuffer* frameBuffer)
// void SceneRendererPass::onBeginPass(SceneRenderer* sceneRenderer, GraphicsCommandList* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer)
//{
// }

// ShaderTechnique* SceneRendererPass::selectShaderTechniqueHelper(
//	const ShaderTechniqueRequestClasses& requester,
//	Shader* requestedShader,
//	ShadingModel requestedShadingModel,
//	Shader* defaultShader,
//	ShaderTechnique* defaultTechnique,
//	ShaderTechniqueClass_Phase phase)
//{
//	const Shader* actualShader = (requestedShader) ? requestedShader : defaultShader;
//
//	ShaderTechniqueClass classSet;
//	classSet.defaultTechnique = false;
//	classSet.phase = phase;
//	classSet.meshProcess = requester.meshProcess;
//	classSet.shadingModel = tlanslateShadingModel(requestedShadingModel);
//	classSet.drawMode = requester.drawMode;
//	classSet.normalClass = requester.normal;
//	classSet.roughnessClass = requester.roughness;
//	ShaderTechnique* technique = ShaderHelper::findTechniqueByClass(requestedShader, classSet);
//	if (technique)
//		return technique;
//	else
//		return defaultTechnique;
// }

//}
//==============================================================================
// SceneRenderer

SceneRenderer::SceneRenderer()
    : m_manager(detail::RenderingManager::instance())
    , m_renderingPipeline(nullptr)
    , m_drawCommandList(makeURef<kanata::DrawCommandList>(detail::RenderingManager::instance())) {
}

void SceneRenderer::init() {
}

void SceneRenderer::prepare(
    GraphicsCommandList* graphicsContext,
    RenderingPipeline* renderingPipeline,
    RenderingContext* renderingContext,
    // detail::CommandListServer* commandListServer,
    const detail::RenderViewInfo& mainRenderViewInfo,
    RenderPart targetPhase,
    const detail::SceneGlobalRenderParams* sceneGlobalParams,
    const RLICulling* culling) {
    m_renderingPipeline = renderingPipeline;
    m_renderingContext = renderingContext;
    m_mainRenderViewInfo = mainRenderViewInfo;
    m_sceneGlobalRenderParams = sceneGlobalParams;
    m_currentPart = targetPhase;

    // m_renderingElementList.clear();
#ifdef LN_USE_KANATA
    m_drawCommandList->clear();
#endif

    collect(renderingPipeline, m_mainRenderViewInfo.cameraInfo, targetPhase);
    prepare();
}

void SceneRenderer::collect(RenderingPipeline* renderingPipeline, /*SceneRendererPass* pass, */ const detail::CameraInfo& cameraInfo, RenderPart targetPhase) {
    for (auto& light : m_renderingContext->dynamicLightInfoList()) {
        if (light.mainLight) {
            m_mainLightInfo = &light;
        }
        onCollectLight(light);
    }
}

void SceneRenderer::prepare() {

    if (m_mainLightInfo) {
        m_mainSceneInfo.ambientColor = m_mainLightInfo->m_position; // position フィールドを代用している
        m_mainSceneInfo.ambientSkyColor = m_mainLightInfo->m_color2;
        m_mainSceneInfo.ambientGroundColor = m_mainLightInfo->m_color3;
        m_mainSceneInfo.mainLightColor = m_mainLightInfo->m_color;
        m_mainSceneInfo.mainLightPos = Vector4(0, 1, 0, 1); // TODO:
        m_mainSceneInfo.mainLightDir = Vector4(m_mainLightInfo->m_direction, 1.0f);
        m_mainSceneInfo.mainLightMatrix = mainRenderViewInfo().mainLightViewProjection;
    }
    else {
        // TODO:
        m_mainSceneInfo = {};
    }
}

void SceneRenderer::onCollectLight(const DynamicLightInfo& light) {
}

void SceneRenderer::onSetAdditionalShaderPassVariables2(ShaderDescriptor* descriptor, ShaderPass* shaderPass) {
}

void SceneRenderer::buildBatchList_Kanata(
    GraphicsCommandList* descriptorAllocator,
    const RLICulling* culling,
    RenderTargetTexture* renderTarget,
    DepthBuffer* depthBuffer,
    SceneRendererPass* pass) {

    //graphicsContext->resetState();

    pass->onBeginRender(this, descriptorAllocator, renderTarget, depthBuffer);

    const detail::RenderViewInfo& cameraInfo = mainRenderViewInfo();

    RenderPass* defaultRenderPass = pass->renderPass();
    assert(defaultRenderPass);


    {
        auto* batchCollector = culling->batchCollector(m_currentPart);
        auto* internalRenderPass = pass->internalSceneRenderPass();
        internalRenderPass->buildDrawCommands(
            this,
            batchCollector,
            descriptorAllocator,
            defaultRenderPass,
            m_mainRenderViewInfo,
            m_mainSceneInfo,
            m_drawCommandList);
    }

    pass->onEndRender(this);
}

void SceneRenderer::render_Kanata(GraphicsCommandList* commandList) {
    m_drawCommandList->submitMeshDrawCommands(commandList);
}

} // namespace detail
} // namespace ln
