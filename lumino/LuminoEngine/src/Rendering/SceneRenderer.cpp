
#include "Internal.hpp"
#include <LuminoGraphics/RHI/ShaderDescriptor.hpp>
#include <LuminoGraphics/RHI/RenderPass.hpp>
#include <LuminoGraphics/RHI/GraphicsCommandBuffer.hpp>
#include <LuminoGraphics/RHI/SamplerState.hpp>
#include <LuminoGraphics/RHI/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/Kanata/KSceneRenderPass.hpp>
#include "../../../Graphics/src/GraphicsManager.hpp"
#include "../Mesh/MeshModelInstance.hpp"
#include "CommandListServer.hpp"
#include "RenderStage.hpp"
#include "RenderElement.hpp"
#include "RenderingPipeline.hpp"
#include "RenderingManager.hpp"
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
    , m_renderFeatureBatchList(detail::RenderingManager::instance())
    , m_renderingPipeline(nullptr)
    , m_drawCommandList(makeURef<kanata::DrawCommandList>(detail::RenderingManager::instance())) {
}

void SceneRenderer::init() {
    // m_renderPass = makeObject<RenderPass>();

    m_skinningMatricesTexture = makeObject<Texture2D>(4, 1024, TextureFormat::RGBA32F);
    m_skinningMatricesTexture->setResourceUsage(GraphicsResourceUsage::Dynamic);
    m_skinningLocalQuaternionsTexture = makeObject<Texture2D>(1, 1024, TextureFormat::RGBA32F);
    m_skinningLocalQuaternionsTexture->setResourceUsage(GraphicsResourceUsage::Dynamic);

    // skinning texture は毎フレーム更新されるものなので、デバイスリセット時に復元する必要はない
    m_skinningMatricesTexture->setResourcePool(GraphicsResourcePool::None);
    m_skinningLocalQuaternionsTexture->setResourcePool(GraphicsResourcePool::None);

    auto samperState = makeObject<SamplerState>();
    samperState->setFilterMode(TextureFilterMode::Point);
    m_skinningMatricesTexture->setSamplerState(samperState);
    m_skinningLocalQuaternionsTexture->setSamplerState(samperState);
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

    buildBatchList_Legacy(graphicsContext, culling);
}

//void SceneRenderer::renderPass(GraphicsCommandList* graphicsContext, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer, SceneRendererPass* pass) {
//#ifdef LN_USE_KANATA
//    renderPass_Kanata(graphicsContext, renderTarget, depthBuffer, pass);
//#else
//    renderPass_Legacy(graphicsContext, renderTarget, depthBuffer, pass);
//#endif // DEBUG
//}
//    //#define LN_PRINT_PROFILE 1

void SceneRenderer::buildBatchList_Legacy(GraphicsCommandList* graphicsContext, const RLICulling* culling) {

    m_renderFeatureBatchList.clear();

    // TODO: とりいそぎ
    m_renderFeatureBatchList.m_mainCameraInfo = &m_mainRenderViewInfo.cameraInfo;

    for (auto& renderFeature : m_manager->renderFeatures()) {
        renderFeature->beginRendering();
    }

#ifdef LN_PRINT_PROFILE
    ElapsedTimer pt;
    pt.start();
#endif
    // Create batch list.
    {
#ifdef LN_RLI_BATCH
        RLIBatchState batchState;
        const std::vector<RenderDrawElement*>& renderingElementList = culling->visibleElements(m_currentPart);
        for (RenderDrawElement* element : renderingElementList) {
            RenderStage* stage = element->stage();
            assert(stage);
            assert(stage->renderFeature);

            m_renderFeatureBatchList.setCurrentStage(stage);

            // nullptr の場合、render 時の規定値を使う
            RenderPass* renderPass = stage->getRenderPassFinal();

            // ShaderDescripter
            SubsetInfo subsetInfo;
            const Matrix* worldMatrix = nullptr;
            Material* finalMaterial = nullptr;
            if (element->flags().hasFlag(RenderDrawElementTypeFlags::Clear)) {
                subsetInfo.clear();
            }
            else {
                if (stage->renderFeature->drawElementTransformNegate()) {
                    worldMatrix = nullptr;
                }
                else {
                    worldMatrix = &element->combinedWorldMatrix();
                }

                finalMaterial = stage->getMaterialFinal(nullptr, m_manager->builtinMaterials(BuiltinMaterial::Default));

                if (finalMaterial) {
                    subsetInfo.materialTexture = finalMaterial->mainTexture();
                    subsetInfo.normalMap = finalMaterial->normalMap();
                    subsetInfo.metallicRoughnessTexture = finalMaterial->metallicRoughnessTexture();
                    subsetInfo.occlusionTexture = finalMaterial->occlusionTexture();
                }
                else {
                    subsetInfo.materialTexture = nullptr;
                    subsetInfo.normalMap = nullptr;
                    subsetInfo.metallicRoughnessTexture = nullptr;
                    subsetInfo.occlusionTexture = nullptr;
                }
                subsetInfo.opacity = stage->getOpacityFinal(element);
                subsetInfo.colorScale = stage->getColorScaleFinal(element);
                subsetInfo.blendColor = stage->getBlendColorFinal(element);
                subsetInfo.tone = stage->getToneFinal(element);
            }

            // setup()
            {
                batchState.m_worldTransform = worldMatrix;
                batchState.m_subsetInfo = subsetInfo;
                batchState.m_renderPass = renderPass;
                batchState.material = finalMaterial;
                batchState.mergeFrom(stage->geometryStageParameters, finalMaterial);
            }

            element->onRequestBatch(&m_renderFeatureBatchList, graphicsContext, stage->renderFeature, &batchState);
        }
#else
        RenderPass* currentRenderPass = nullptr;
        RenderStage* currentStage = nullptr;
        const Matrix* currentWorldMatrix = nullptr;
        Material* currentFinalMaterial = nullptr;
        SubsetInfo currentSubsetInfo;
        // int count = 0;
        const std::vector<RenderDrawElement*>& renderingElementList = culling->visibleElements(m_currentPart);
        for (RenderDrawElement* element : renderingElementList) {
            // if (pass->filterElement(element))
            {
                bool submitRequested = false;
                RenderStage* stage = element->stage();
                assert(stage->renderFeature);

                if (!stage->renderFeature->drawElementTransformNegate()) {
                    // バッチ機能を持たない RenderFeature であるので、毎回 flush する。
                    // 実際のところ Mesh とかを無理やりバッチ(Descripterを変更しないで連続描画)しようとしても、
                    // ほとんどの場合は WorldMatrix の変更が必要なので頑張ってやってもあまり恩恵がない。
                    submitRequested = true;
                }
                else {
                    // ステートの変わり目チェック
                    if (!currentStage || !currentStage->equals(stage)) {
                        submitRequested = true;
                    }
                }

                RenderPass* renderPass = nullptr;
                if (submitRequested) {
                    renderPass = getOrCreateRenderPass(currentRenderPass, stage);
                }
                else {
                    renderPass = currentRenderPass;
                }

                // ShaderDescripter
                SubsetInfo subsetInfo;
                const Matrix* worldMatrix = nullptr;
                Material* finalMaterial = nullptr;
                if (element->flags().hasFlag(RenderDrawElementTypeFlags::Clear)) {
                    subsetInfo.clear();
                }
                else {
                    if (stage->renderFeature->drawElementTransformNegate()) {
                        worldMatrix = nullptr;
                    }
                    else {
                        worldMatrix = &element->combinedWorldMatrix();
                    }

                    finalMaterial = stage->getMaterialFinal(nullptr, m_manager->builtinMaterials(BuiltinMaterial::Default));

                    if (finalMaterial) {
                        subsetInfo.materialTexture = finalMaterial->mainTexture();
                        subsetInfo.normalMap = finalMaterial->normalMap();
                        subsetInfo.metallicRoughnessTexture = finalMaterial->metallicRoughnessTexture();
                        subsetInfo.occlusionTexture = finalMaterial->occlusionTexture();
                    }
                    else {
                        subsetInfo.materialTexture = nullptr;
                        subsetInfo.normalMap = nullptr;
                        subsetInfo.metallicRoughnessTexture = nullptr;
                        subsetInfo.occlusionTexture = nullptr;
                    }
                    subsetInfo.opacity = stage->getOpacityFinal(element);
                    subsetInfo.colorScale = stage->getColorScaleFinal(element);
                    subsetInfo.blendColor = stage->getBlendColorFinal(element);
                    subsetInfo.tone = stage->getToneFinal(element);
                }

                // ShaderDescripter に関係するパラメータの変更チェック
                if (!submitRequested && m_renderFeatureBatchList.lastBatch() && !SubsetInfo::equals(currentSubsetInfo, subsetInfo)) {
                    submitRequested = true;
                }

                // Batch 確定
                RenderFeatureBatch* submittedBatch = nullptr;
                if (submitRequested) {
                    if (!currentStage) {
                        // 初回. 1つも draw 仕様としていないので submit は不要.
                    }
                    else {
                        currentStage->renderFeature->submitBatch(graphicsContext, &m_renderFeatureBatchList);
                        submittedBatch = m_renderFeatureBatchList.lastBatch();
                    }
                }

                auto result = element->onRequestBatch(&m_renderFeatureBatchList, graphicsContext, stage->renderFeature, &subsetInfo);
                if (result == RequestBatchResult::Submitted) {
                    if (submittedBatch) {
                        // もし↑の submitBatch() でも新しい Batch が作られたなら、onRequestBatch では新しい Batch を作ってはならない
                        // (実際空なので意味はなく、もし作ってしまうと↓でいろいろ set しているものが submitBatch の Batch に掛からなくなってしまう)
                        LN_CHECK(submittedBatch == m_renderFeatureBatchList.lastBatch());
                    }
                    submittedBatch = m_renderFeatureBatchList.lastBatch();
                }

                if (submittedBatch) {
                    submittedBatch->setup(
                        currentWorldMatrix, currentFinalMaterial, currentSubsetInfo, submittedBatch->ensureRenderPassOutside ? nullptr : currentRenderPass);
                }

                if (submitRequested) {
                    currentWorldMatrix = worldMatrix;
                    currentFinalMaterial = finalMaterial;
                    currentSubsetInfo = subsetInfo;
                    currentStage = stage;
                    currentRenderPass = renderPass;
                    m_renderFeatureBatchList.setCurrentStage(currentStage);
                }
            }
        }

        if (currentStage) {
            currentStage->renderFeature->submitBatch(graphicsContext, &m_renderFeatureBatchList);
            m_renderFeatureBatchList.lastBatch()->setup(
                currentWorldMatrix, currentFinalMaterial, currentSubsetInfo, m_renderFeatureBatchList.lastBatch()->ensureRenderPassOutside ? nullptr : currentRenderPass);
        }
#endif
    }

#ifdef LN_PRINT_PROFILE
    double tt = pt.elapsed();
    std::cout << "buildBatchList: " << tt << std::endl;
#endif
}

void SceneRenderer::renderPass_Legacy(GraphicsCommandList* graphicsContext, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer, SceneRendererPass* pass) {
#ifdef LN_PRINT_PROFILE
    ElapsedTimer pt;
    pt.start();
#endif

    // TODO: とりいそぎ
    m_renderFeatureBatchList.renderTarget = renderTarget;
    m_renderFeatureBatchList.depthBuffer = depthBuffer;

    graphicsContext->resetState();

    pass->onBeginRender(this, graphicsContext, renderTarget, depthBuffer);

    const detail::RenderViewInfo& cameraInfo = mainRenderViewInfo();

    RenderPass* defaultRenderPass = pass->renderPass();
    assert(defaultRenderPass);

    // Render batch-list.
    {
        RenderPass* currentRenderPass = defaultRenderPass;
        if (currentRenderPass) {
            // Batch が1つもないときでも、RenderPass は開始しておく。
            // そうしないと画面がクリアされず、Vulkan バックエンドを使っているときに RenderTarget が不正な状態なままになる。
            graphicsContext->beginRenderPass(currentRenderPass);
        }

        const RenderStage* currentStage = nullptr;
        RenderFeatureBatch* batch = m_renderFeatureBatchList.firstBatch();
        while (batch) {

            // 新しい RenderPass を取得する。nullptr の場合は Pass のデフォルトを使う。
            // (バックバッファ以外にも、シャドウマップなどへ書き込む Pass もある)
            RenderPass* batchRenderPass = batch->renderPass();
            if (!batchRenderPass) {
                batchRenderPass = defaultRenderPass;
            }

            if (currentRenderPass != batchRenderPass) {
                if (currentRenderPass) {
                    graphicsContext->endRenderPass();
                }

                currentRenderPass = batchRenderPass;

                if (currentRenderPass) {
                    graphicsContext->beginRenderPass(currentRenderPass);
                }
            }

            const RenderStage* stage = batch->stage();
            const RLIBatchState* finalMaterial = batch->finalMaterial();
            const SubsetInfo& subsetInfo = batch->subsetInfo();

            // ステートの変わり目チェック
            if (currentStage != stage) // この時点ではポインタ比較だけでよい。中身が異なることは保証済み
            {
                currentStage = stage;
                // RenderStage::applyFrameBufferStatus(m_renderPass, currentStage, defaultFrameBuffer);
                const RectI& scissorRect = stage->getScissorRectFinal();
                if (scissorRect.width < 0) {
                    RenderPass* renderPass = (currentRenderPass) ? currentRenderPass : pass->renderPass();
                    graphicsContext->setScissorRect(Rect(0, 0, renderPass->width(), renderPass->height()));
                }
                else {
                    graphicsContext->setScissorRect(scissorRect.toFloatRect());
                }

                const RectI& viewportRect = stage->getViewportRectFinal();
                if (viewportRect.width < 0) {
                    RenderPass* renderPass = (currentRenderPass) ? currentRenderPass : pass->renderPass();
                    graphicsContext->setViewportRect(Rect(0, 0, renderPass->width(), renderPass->height()));
                }
                else {
                    graphicsContext->setViewportRect(viewportRect.toFloatRect());
                }
            }

            if (!finalMaterial->material) {
                // Shader 使わない描画 (clear)
                RLIBatchState m1;
                m1.mergeFrom(currentStage->geometryStageParameters, nullptr);
                RLIMaterial rm(m1);
                rm.applyRenderStates(graphicsContext);
                batch->render(graphicsContext);
            }
            else {

                ElementInfo elementInfo;
                elementInfo.objectId = stage->m_objectId;
                elementInfo.viewProjMatrix = &cameraInfo.cameraInfo.viewProjMatrix;
                elementInfo.WorldMatrix = (batch->worldTransformPtr()) ? *batch->worldTransformPtr() : Matrix::Identity;
                elementInfo.WorldViewProjectionMatrix = elementInfo.WorldMatrix * (*elementInfo.viewProjMatrix);
                elementInfo.boneLocalQuaternionTexture = nullptr; // TODO: (MMD SDEF)
                if (batch->skeleton) {
                    elementInfo.boneTexture = batch->skeleton->skinningMatricesTexture();
                }
                else {
                    elementInfo.boneTexture = nullptr;
                }

                ShaderTechniqueRequestClasses requester = {
                    (elementInfo.boneTexture) ? kokage::ShaderTechniqueClass_MeshProcess::SkinnedMesh : kokage::ShaderTechniqueClass_MeshProcess::StaticMesh,
                    (batch->instancing) ? kokage::ShaderTechniqueClass_DrawMode::Instancing : kokage::ShaderTechniqueClass_DrawMode::Primitive,
                    (subsetInfo.normalMap) ? kokage::ShaderTechniqueClass_Normal::NormalMap : kokage::ShaderTechniqueClass_Normal::Default,
                    (subsetInfo.metallicRoughnessTexture) ? kokage::ShaderTechniqueClass_Roughness::RoughnessMap : kokage::ShaderTechniqueClass_Roughness::Default,
                };

                const ShadingModel shadingModel = finalMaterial->shadingModel;
                ShaderTechnique* tech = pass->selectShaderTechnique(
                    requester,
                    finalMaterial->material->shader(),
                    shadingModel);

                SubsetInfo localSubsetInfo = subsetInfo;
                if (batch->overrideTexture) {
                    localSubsetInfo.materialTexture = batch->overrideTexture;

                    // TODO: テキスト描画でAAをサンプラーステートでやるために使っている。
                    // ただ、設定しっぱなしになってしまうためあまりよくないかも。今はテキストでしか使ってないからいいけど。
                    if (batch->overrideSamplerState) {
                        localSubsetInfo.materialTexture->setSamplerState(batch->overrideSamplerState);
                    }
                }
                if (!localSubsetInfo.materialTexture) {
                    localSubsetInfo.materialTexture = m_manager->graphicsManager()->whiteTexture();
                }
                if (!localSubsetInfo.normalMap) {
                    localSubsetInfo.normalMap = m_manager->graphicsManager()->defaultNormalMap();
                }
                if (!localSubsetInfo.metallicRoughnessTexture) {
                    // ダミーは WhiteTexture でよい。
                    //   R=遮蔽率の逆。(1.0 に近づくほど、完全な光の下、オリジナルに近い色になる)
                    //   G=Metallic. マテリアル自体の Metallic と乗算されるため、1.0 でよい。
                    //   B=Roughness. マテリアル自体の Roughness と乗算されるため、1.0 でよい。
                    //   A=未使用
                    localSubsetInfo.metallicRoughnessTexture = m_manager->graphicsManager()->whiteTexture();
                }
                if (!localSubsetInfo.occlusionTexture) {
                    localSubsetInfo.occlusionTexture = m_manager->graphicsManager()->whiteTexture();
                }

                const auto& commandList = graphicsContext;

                // if (batch->morph) {
                //	const auto& shader = detail::RenderingManager::instance()->blendShapeShader;
                //	ShaderSecondaryDescriptor* descriptor = commandList->acquireShaderDescriptor(shader.shader);

                //	descriptor->setst

                //	batch->morph->getMorphWeights(&elementInfo.morphWeights);

                //}
                // else {
                //	std::fill(elementInfo.morphWeights.begin(), elementInfo.morphWeights.end(), 0.0f);
                //}

                detail::ShaderTechniqueSemanticsManager* semanticsManager = tech->semanticsManager2();

                ShaderSecondaryDescriptor* descriptor = commandList->acquireShaderDescriptor(tech->shader());
                descriptor->reallocFromSemanticsManager(commandList, semanticsManager);

                // semanticsManager->updateCameraVariables(cameraInfo);
                // semanticsManager->updateElementVariables(cameraInfo, elementInfo);
                // semanticsManager->updateSubsetVariables(subsetInfo);
                // if (stage->renderFeature) {
                //	stage->renderFeature->updateRenderParameters(element, tech, cameraInfo, elementInfo, subsetInfo);
                // }
                // else
                //  TODO: ↑SpriteTextRenderer が Font 取るのに使ってる。これは Batch に持っていくべきだろう。
                {
                    RenderFeature::updateRenderParametersDefault(descriptor, tech, m_mainRenderViewInfo, m_mainSceneInfo, elementInfo, localSubsetInfo);
                }

                // Commit final material
                {
                    PbrMaterialData pbrMaterialData = finalMaterial->material->getPbrMaterialData();
                    semanticsManager->updateSubsetVariables_PBR(descriptor, pbrMaterialData);
                    finalMaterial->material->updateShaderVariables(commandList, descriptor);

                    RLIMaterial rm(*finalMaterial);
                    pass->overrideFinalMaterial(&rm);
                    rm.applyRenderStates(graphicsContext);

                    onSetAdditionalShaderPassVariables(descriptor, tech);
                }

                for (ShaderPass* pass2 : tech->passes()) {
                    graphicsContext->setShaderPass(pass2);
                    graphicsContext->setShaderDescriptor(descriptor);
                    batch->render(graphicsContext);
                    graphicsContext->setShaderDescriptor(nullptr);
                }
            }

            batch = batch->next();
        }

        if (currentRenderPass) {
            graphicsContext->endRenderPass();
        }
    }

    // for (auto& renderFeature : m_manager->renderFeatures()) {
    //	renderFeature->endRendering();
    // }

    //m_renderPassPoolUsed = 0;

    pass->onEndRender(this);

#ifdef LN_PRINT_PROFILE
    double tt = pt.elapsed();
    std::cout << "renderPass: " << tt << std::endl;
#endif
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

void SceneRenderer::onSetAdditionalShaderPassVariables(ShaderSecondaryDescriptor* descriptor, ShaderTechnique* technique) {
}

bool SceneRenderer::equalsFramebuffer(RenderPass* renderPass, const FrameBuffer& fb) {
    for (int i = 0; i < GraphicsCommandList::MaxMultiRenderTargets; i++) {
        if (renderPass->renderTarget(i) != fb.renderTarget[i]) {
            return false;
        }
    }
    return renderPass->depthBuffer() == fb.depthBuffer;
}

void SceneRenderer::buildBatchList_Kanata(
    GraphicsCommandList* descriptorAllocator,
    const RLICulling* culling,
    RenderTargetTexture* renderTarget,
    DepthBuffer* depthBuffer,
    SceneRendererPass* pass) {

    // TODO: とりいそぎ
    m_renderFeatureBatchList.renderTarget = renderTarget;
    m_renderFeatureBatchList.depthBuffer = depthBuffer;

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
