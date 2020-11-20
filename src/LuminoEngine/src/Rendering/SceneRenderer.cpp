
#include "Internal.hpp"
#include <LuminoEngine/Graphics/RenderPass.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/CommandList.hpp>
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include "../Graphics/GraphicsManager.hpp"
#include "../Mesh/MeshModelInstance.hpp"
#include "RenderStage.hpp"
#include "RenderElement.hpp"
#include "RenderingPipeline.hpp"
#include "RenderingManager.hpp"
#include "SceneRenderer.hpp"

namespace ln {
namespace detail {

//==============================================================================
// SceneRendererPass

SceneRendererPass::SceneRendererPass()
	: m_manager(detail::EngineDomain::renderingManager())
{
}

SceneRendererPass::~SceneRendererPass()
{
}

void SceneRendererPass::init()
{
}

void SceneRendererPass::onBeginRender(SceneRenderer* sceneRenderer, GraphicsContext* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer)
{
}

void SceneRendererPass::onEndRender(SceneRenderer* sceneRenderer)
{
}

bool SceneRendererPass::filterElement(RenderDrawElement* element) const
{
	if (element->flags().hasFlag(RenderDrawElementTypeFlags::LightDisc)) return false;

	return (element->flags() & (
		RenderDrawElementTypeFlags::Clear |
		RenderDrawElementTypeFlags::Opaque |
		RenderDrawElementTypeFlags::Transparent)) != RenderDrawElementTypeFlags::None;
}

//void SceneRendererPass::onBeginPass(GraphicsContext* context, FrameBuffer* frameBuffer)
//void SceneRendererPass::onBeginPass(SceneRenderer* sceneRenderer, GraphicsContext* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer)
//{
//}

//ShaderTechnique* SceneRendererPass::selectShaderTechniqueHelper(
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
//}

//}
//==============================================================================
// SceneRenderer

SceneRenderer::SceneRenderer()
	: m_manager(detail::EngineDomain::renderingManager())
	, m_renderFeatureBatchList(detail::EngineDomain::renderingManager())
	, m_renderingPipeline(nullptr)
	, m_zSortDistanceBase(ZSortDistanceBase::CameraScreenDistance)
	, m_renderPassPoolUsed(0)
{
}

void SceneRenderer::init()
{
	//m_renderPass = makeObject<RenderPass>();

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
	RenderingPipeline* renderingPipeline,
	//detail::CommandListServer* commandListServer,
	const detail::RenderViewInfo& mainRenderViewInfo,
	RenderPart targetPhase,
	ProjectionKind targetProjection,
	const detail::SceneGlobalRenderParams* sceneGlobalParams)
{
	m_renderingPipeline = renderingPipeline;
    m_mainRenderViewInfo = mainRenderViewInfo;
	m_sceneGlobalRenderParams = sceneGlobalParams;
	m_currentPart = targetPhase;
	m_currentProjection = targetProjection;

	m_renderingElementList.clear();
	collect(renderingPipeline, m_mainRenderViewInfo.cameraInfo, targetPhase);
	prepare();
}

#if 0
void SceneRenderer::render(
	GraphicsContext* graphicsContext,
    RenderingPipeline* renderingPipeline,
	RenderTargetTexture* renderTarget,
	DepthBuffer* depthBuffer,
    const CameraInfo& mainCameraInfo)
{

	//m_defaultFrameBuffer = &defaultFrameBuffer;
    //m_targetPhase = targetPhase;

	//detail::CoreGraphicsRenderFeature* coreRenderer = m_manager->getRenderer();
	//coreRenderer->begin();

	//m_renderPass->setRenderTarget(0, defaultFrameBuffer.renderTarget[0]);
	//m_renderPass->setDepthBuffer(defaultFrameBuffer.depthBuffer);
	//graphicsContext->setRenderPass(m_renderPass);

	//if (clearColorBuffer)
	{
        // TODO: 暫定。
        //graphicsContext->setRenderTarget(0, defaultFrameBuffer.renderTarget[0]);
        //graphicsContext->setDepthBuffer(defaultFrameBuffer.depthBuffer);
        //graphicsContext->clear(ClearFlags::Depth, Color::White);
		// TODO: 前回の最後のステートが残っていることがある。clear したいやつの弊害だけ、とりあえず暫定処置。シザーも必要になりそう。
		//coreRenderer->setViewport(RectI(0, 0, defaultRenderTarget->getSize()));
		//coreRenderer->clear(ClearFlags::All, clearColor);
	}
	



	//if (diag != nullptr) diag->beginRenderView();
	//if (diag != nullptr) diag->beginDrawList();

	//InternalContext* context = m_manager->getInternalContext();
	//const detail::CameraInfo& cameraInfo = drawElementListSet->m_cameraInfo;


	//DrawElement::DrawArgs drawArgs;
	////drawArgs.oenerList = elementList;
	//drawArgs.context = context;
	//drawArgs.renderer = this;
	//drawArgs.defaultRenderTarget = defaultRenderTarget;
	//drawArgs.defaultDepthBuffer = defaultDepthBuffer;
	//drawArgs.diag = diag;

	// TODO: 別 RT への描画は RenderStage 的な感じで分けて、メインの Pass でだけ行うようにしたほうが良い。
	// こういうものは ShadowCast のパスで実行したくない。無駄になる。


	//m_renderingActualPassList.clear();
	//m_renderingShadowCasterPassList.clear();
	//for (auto& elementList : m_renderingRenderView->m_lists)
	//{
	//	for (DynamicLightInfo* light : elementList->getDynamicLightList())
	//	{
	//		if (light->m_shadowCasterPass != nullptr)
	//		{
	//			m_renderingActualPassList.add(light->m_shadowCasterPass);
	//			m_renderingShadowCasterPassList.add(light->m_shadowCasterPass);
	//		}
	//	}
	//}
	//m_renderingActualPassList.addRange(m_renderingPassList);


	m_renderingActualPassList.clear();
	for (SceneRendererPass* pass : m_renderingPassList)
	{
		m_renderingActualPassList.add(pass);
	}


	//for (SceneRendererPass* pass : m_renderingActualPassList)
	//{
	//	pass->onBeginRender(this);
	//}

	for (SceneRendererPass* pass : m_renderingActualPassList)
	{
        renderPass(graphicsContext, renderTarget, depthBuffer, pass);
	}

	//for (SceneRendererPass* pass : m_renderingActualPassList)
	//{
	//	pass->onEndRender(this);
 //   }



	//// Flush
	//{
	//	m_manager->getInternalContext()->flush();
	//}

	//if (diag != nullptr) diag->endDrawList();
	//if (diag != nullptr) diag->endRenderView();

	//coreRenderer->end();
}
#endif

//void SceneRenderer::setDefaultMaterial(Material* material)
//{
//	m_defaultMaterial = material;
//}

//void SceneRenderer::addPass(SceneRendererPass* pass)
//{
//	m_renderingPassList.add(pass);
//}

void SceneRenderer::renderPass(GraphicsContext* graphicsContext, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer, SceneRendererPass* pass)
{
	graphicsContext->resetState();

	//m_renderingElementList.clear();

	//FrameBuffer defaultFrameBuffer = *m_defaultFrameBuffer;
	pass->onBeginRender(this, graphicsContext, renderTarget, depthBuffer);

	const detail::RenderViewInfo& cameraInfo = mainRenderViewInfo();

	//pass->overrideCameraInfo(&cameraInfo);

	//collect(/*pass, */cameraInfo);

	//prepare();

	for (auto& renderFeature : m_manager->renderFeatures()) {
		renderFeature->beginRendering();
	}

	m_renderFeatureBatchList.clear();

    // TODO: とりいそぎ
    m_renderFeatureBatchList.renderTarget = renderTarget;
    m_renderFeatureBatchList.depthBuffer = depthBuffer;
	m_renderFeatureBatchList.m_mainCameraInfo = &m_mainRenderViewInfo.cameraInfo;

	RenderPass* defaultRenderPass = pass->renderPass();
	assert(defaultRenderPass);

	// Create batch list.
	{
		RenderPass* currentRenderPass = defaultRenderPass;
		RenderStage* currentStage = nullptr;
		const Matrix* currentWorldMatrix = nullptr;
		Material* currentFinalMaterial = nullptr;
		SubsetInfo currentSubsetInfo;
        //int count = 0;
		for (RenderDrawElement* element : m_renderingElementList)
		{
			if (pass->filterElement(element)) {
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
					renderPass = getOrCreateRenderPass(currentRenderPass, stage, defaultRenderPass/*renderTarget, depthBuffer*//*, clearInfo*/);
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
						subsetInfo.roughnessMap = finalMaterial->roughnessMap();
					}
					else {
						subsetInfo.materialTexture = nullptr;
						subsetInfo.normalMap = nullptr;
						subsetInfo.roughnessMap = nullptr;
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
					submittedBatch->setWorldTransformPtr(currentWorldMatrix);
					submittedBatch->setFinalMaterial(currentFinalMaterial);
					submittedBatch->setSubsetInfo(currentSubsetInfo);
					submittedBatch->setRenderPass(submittedBatch->ensureRenderPassOutside ? nullptr : currentRenderPass);
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
			m_renderFeatureBatchList.lastBatch()->setWorldTransformPtr(currentWorldMatrix);
			m_renderFeatureBatchList.lastBatch()->setFinalMaterial(currentFinalMaterial);
			m_renderFeatureBatchList.lastBatch()->setSubsetInfo(currentSubsetInfo);
			m_renderFeatureBatchList.lastBatch()->setRenderPass(m_renderFeatureBatchList.lastBatch()->ensureRenderPassOutside ? nullptr : currentRenderPass);
		}
	}

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
		while (batch)
		{
			if (currentRenderPass != batch->renderPass()) {
				if (currentRenderPass) {
					graphicsContext->endRenderPass();
				}
				currentRenderPass = batch->renderPass();
                if (currentRenderPass) {
                    graphicsContext->beginRenderPass(currentRenderPass);
                }
			}

			const RenderStage* stage = batch->stage();
			const Material* finalMaterial = batch->finalMaterial();
			const SubsetInfo& subsetInfo = batch->subsetInfo();

			// ステートの変わり目チェック
			if (currentStage != stage)	// この時点ではポインタ比較だけでよい。中身が異なることは保証済み
			{
				currentStage = stage;
				//RenderStage::applyFrameBufferStatus(m_renderPass, currentStage, defaultFrameBuffer);
		        const RectI& scissorRect = stage->getScissorRectFinal();
		        if (scissorRect.width < 0) {
                    RenderPass* renderPass = (currentRenderPass) ? currentRenderPass : pass->renderPass();
                    auto renderTarget = renderPass->renderTarget(0);
                    graphicsContext->setScissorRect(Rect(0, 0, renderTarget->width(), renderTarget->height()));
		        }
                else {
                    graphicsContext->setScissorRect(scissorRect.toFloatRect());
                }

				const RectI& viewportRect = stage->getViewportRectFinal();
				if (viewportRect.width < 0) {
					RenderPass* renderPass = (currentRenderPass) ? currentRenderPass : pass->renderPass();
					auto renderTarget = renderPass->renderTarget(0);
					graphicsContext->setViewportRect(Rect(0, 0, renderTarget->width(), renderTarget->height()));
				}
				else {
					graphicsContext->setViewportRect(viewportRect.toFloatRect());
				}
			}

			if (!finalMaterial) {
				// Shader 使わない描画 (clear)
				RenderStage::applyGeometryStatus(graphicsContext, currentStage, nullptr);
				batch->render(graphicsContext);
			}
			else {
				ElementInfo elementInfo;
				elementInfo.objectId = stage->m_objectId;
				elementInfo.viewProjMatrix = &cameraInfo.cameraInfo.viewProjMatrix;
				elementInfo.WorldMatrix = (batch->worldTransformPtr()) ? *batch->worldTransformPtr() : Matrix::Identity;
				elementInfo.WorldViewProjectionMatrix = elementInfo.WorldMatrix * (*elementInfo.viewProjMatrix);
				elementInfo.boneLocalQuaternionTexture = nullptr;	// TODO: (MMD SDEF)
				if (batch->skeleton) {
					elementInfo.boneTexture = batch->skeleton->skinningMatricesTexture();
				}
				else {
					elementInfo.boneTexture = nullptr;
				}

				ShaderTechniqueRequestClasses requester = {
					(elementInfo.boneTexture) ? ShaderTechniqueClass_MeshProcess::SkinnedMesh : ShaderTechniqueClass_MeshProcess::StaticMesh,
					(batch->instancing) ? ShaderTechniqueClass_DrawMode::Instancing : ShaderTechniqueClass_DrawMode::Primitive,
					(subsetInfo.normalMap) ? ShaderTechniqueClass_Normal::NormalMap : ShaderTechniqueClass_Normal::Default,
					(subsetInfo.roughnessMap) ? ShaderTechniqueClass_Roughness::RoughnessMap : ShaderTechniqueClass_Roughness::Default,
				};
				ShaderTechnique* tech = pass->selectShaderTechnique(
					requester,
					finalMaterial->shader(),
					stage->getShadingModelFinal(finalMaterial));

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
				if (!localSubsetInfo.roughnessMap) {
					localSubsetInfo.roughnessMap = m_manager->graphicsManager()->blackTexture();
				}


				detail::ShaderTechniqueSemanticsManager* semanticsManager = tech->semanticsManager2();
				//semanticsManager->updateCameraVariables(cameraInfo);
				//semanticsManager->updateElementVariables(cameraInfo, elementInfo);
				//semanticsManager->updateSubsetVariables(subsetInfo);
				//if (stage->renderFeature) {
				//	stage->renderFeature->updateRenderParameters(element, tech, cameraInfo, elementInfo, subsetInfo);
				//}
				//else
				// TODO: ↑SpriteTextRenderer が Font 取るのに使ってる。これは Batch に持っていくべきだろう。
				{
					RenderFeature::updateRenderParametersDefault(tech, m_mainRenderViewInfo, m_mainSceneInfo, elementInfo, localSubsetInfo);
				}

				if (finalMaterial) {
					PbrMaterialData pbrMaterialData = finalMaterial->getPbrMaterialData();
					semanticsManager->updateSubsetVariables_PBR(pbrMaterialData);
					finalMaterial->updateShaderVariables(tech->shader());
					RenderStage::applyGeometryStatus(graphicsContext, currentStage, finalMaterial);
				}

				onSetAdditionalShaderPassVariables(tech);

				for (ShaderPass* pass2 : tech->passes())
				{
					graphicsContext->setShaderPass(pass2);
					batch->render(graphicsContext);
				}
			}


			batch = batch->next();
		}

		if (currentRenderPass) {
			graphicsContext->endRenderPass();
		}
	}

	for (auto& renderFeature : m_manager->renderFeatures()) {
		renderFeature->endRendering();
	}

    m_renderPassPoolUsed = 0;

	pass->onEndRender(this);
}

void SceneRenderer::collect(RenderingPipeline* renderingPipeline,/*SceneRendererPass* pass, */const detail::CameraInfo& cameraInfo, RenderPart targetPhase)
{
	//InternalContext* context = m_manager->getInternalContext();
	//const detail::CameraInfo& cameraInfo = m_renderingRenderView->m_cameraInfo;


    //for (auto& elementList : renderingPipeline->elementListCollector()->lists(/*RenderPart::Default*/))
    {
        for (auto& light : renderingPipeline->elementList()->dynamicLightInfoList())
        {
			if (light.mainLight) {
				m_mainLightInfo = &light;
			}
            onCollectLight(light);
        }
    }





    const auto& classifiedElements = renderingPipeline->elementList()->classifiedElementList(targetPhase);
	{
		RenderDrawElement* element = classifiedElements.headElement;
		while (element) {

#if 0		// TODO: 視錘台カリング
			const Matrix& transform = element->getTransform(elementList);

			Sphere boundingSphere = element->getLocalBoundingSphere();
			boundingSphere.center += transform.getPosition();

			if (boundingSphere.radius < 0 ||	// マイナス値なら視錐台と衝突判定しない
				cameraInfo.viewFrustum.intersects(boundingSphere.center, boundingSphere.radius))
			{
				// このノードは描画できる
				m_renderingElementList.add(element);
			}
#else
			m_renderingElementList.add(element);
#endif

			element = element->m_classifiedNext;
		}
	}

	for (auto& element : m_renderingElementList)
	{
		element->calculateActualPriority();

		auto& position = element->combinedWorldMatrix().position();

		// calculate distance for ZSort
		switch (m_zSortDistanceBase)
		{
		case ZSortDistanceBase::NodeZ:
			element->zDistance = position.z;
			break;
		case ZSortDistanceBase::CameraDistance:
			element->zDistance = (position - cameraInfo.viewPosition).lengthSquared();
			break;
		case ZSortDistanceBase::CameraScreenDistance:
			element->zDistance = Vector3::dot(
				position - cameraInfo.viewPosition,
				cameraInfo.viewDirection);		// 平面と点の距離
												// TODO: ↑第2引数違くない？要確認
			break;
		default:
			LN_UNREACHABLE();
			break;
		}
	}
}

void SceneRenderer::prepare()
{
	// 距離は降順。遠いほうを先に描画する
	// 優先度は昇順。高いほうを手前に描画する (UE4 ESceneDepthPriorityGroup)
	// フェンスID は昇順。高いほうを後に描画する
	std::stable_sort(
		m_renderingElementList.begin(), m_renderingElementList.end(),
		[](const RenderDrawElement* lhs, const RenderDrawElement* rhs)
		{
			if (lhs->commandFence == rhs->commandFence)
			{
				if (lhs->actualPriority() == rhs->actualPriority())
					return lhs->zDistance > rhs->zDistance;
				return lhs->actualPriority() < rhs->actualPriority();
			}
			else
			{
				return lhs->commandFence < rhs->commandFence;
			}
		});

	if (m_mainLightInfo) {
		m_mainSceneInfo.ambientColor = m_mainLightInfo->m_position;	// position フィールドを代用している
		m_mainSceneInfo.ambientSkyColor = m_mainLightInfo->m_color2;
		m_mainSceneInfo.ambientGroundColor = m_mainLightInfo->m_color3;
	}
}

void SceneRenderer::onCollectLight(const DynamicLightInfo& light)
{
}

void SceneRenderer::onSetAdditionalShaderPassVariables(ShaderTechnique* technique)
{
}

RenderPass* SceneRenderer::getOrCreateRenderPass(RenderPass* currentRenderPass, RenderStage* stage, RenderPass* defaultRenderPass/*RenderTargetTexture* defaultRenderTarget, DepthBuffer* defaultDepthBuffer*//*, const ClearInfo& clearInfo*/)
{
	assert(currentRenderPass);
	FrameBuffer fb;
	for (int i = 0; i < GraphicsContext::MaxMultiRenderTargets; i++) {
		//if (i == 0)
		//	fb.renderTarget[i] = stage->frameBufferStageParameters->m_renderTargets[i] ? stage->frameBufferStageParameters->m_renderTargets[i] : defaultRenderTarget;
		//else
			fb.renderTarget[i] = stage->frameBufferStageParameters->m_renderTargets[i];
	}
	//fb.depthBuffer = (stage->frameBufferStageParameters->m_depthBuffer) ? stage->frameBufferStageParameters->m_depthBuffer : defaultDepthBuffer;
	fb.depthBuffer = stage->frameBufferStageParameters->m_depthBuffer;

    //bool equalsClearInfo = 

	if (equalsFramebuffer(currentRenderPass, fb)) {
		return currentRenderPass;
	}

	if (!fb.renderTarget[0]) {
		return defaultRenderPass;
	}

	RenderPass* renderPass;
	if (m_renderPassPoolUsed < m_renderPassPool.size()) {
		renderPass = m_renderPassPool[m_renderPassPoolUsed];
	}
	else {
		auto r = makeObject<RenderPass>();
		m_renderPassPool.add(r);
		renderPass = r;
	}
	m_renderPassPoolUsed++;

	// reset
	renderPass->setClearValues(ClearFlags::Depth, Color::Transparency, 1.0f, 0x00);
    //renderPass->setClearValues(clearInfo.flags, clearInfo.color, clearInfo.depth, clearInfo.stencil);

	for (int i = 0; i < GraphicsContext::MaxMultiRenderTargets; i++) {
		renderPass->setRenderTarget(i, fb.renderTarget[i]);
	}
	renderPass->setDepthBuffer(fb.depthBuffer);

	// TODO: コマンド先読みして clear 値も考えたい。今は None (クリアしない)

	return renderPass;
}

bool SceneRenderer::equalsFramebuffer(RenderPass* renderPass, const FrameBuffer& fb)
{
	for (int i = 0; i < GraphicsContext::MaxMultiRenderTargets; i++) {
		if (renderPass->renderTarget(i) != fb.renderTarget[i]) {
			return false;
		}
	}
	return renderPass->depthBuffer() == fb.depthBuffer;
}

//void SceneRenderer::applyFrameBufferStatus(GraphicsContext* context, RenderStage* stage, const FrameBuffer& defaultFrameBufferInPass)
//{
//	RenderTargetTexture* renderTarget0 = nullptr;
//
//	// RenderTarget
//	{
//		for (int i = 0; i < MaxMultiRenderTargets; i++)
//		{
//			RenderTargetTexture* target = stage->getRenderTargetFinal(i);
//			if (!target) {
//				target = defaultFrameBufferInPass.renderTarget[i];
//			}
//
//			//context->setRenderTarget(i, target);
//			m_renderPass->setRenderTarget(i, target);
//
//			if (i == 0) {
//				renderTarget0 = target;
//			}
//		}
//	}
//
//	// DepthBuffer
//	{
//		DepthBuffer* depthBuffer = stage->getDepthBufferFinal();
//		if (depthBuffer) {
//			//context->setDepthBuffer(depthBuffer);
//			m_renderPass->setDepthBuffer(depthBuffer);
//		}
//		else {
//			//context->setDepthBuffer(defaultFrameBufferInPass.depthBuffer);
//			m_renderPass->setDepthBuffer(defaultFrameBufferInPass.depthBuffer);
//		}
//	}
//
//	// Viewport
//	{
//		//const RectI& rect = stage->getViewportRectFinal();//getViewportRect();
//		//if (rect.width < 0)
//		//{
//		//	stateManager->setViewport(RectI(0, 0, renderTarget0->getSize()));
//		//}
//		//else
//		//{
//		//	stateManager->setViewport(rect);
//		//}
//		// TODO:Viewport
//		// TODO: m_scissorRect
//	}
//}

//void SceneRenderer::applyGeometryStatus(GraphicsContext* context, RenderStage* stage, Material* priorityMaterial)
//{
//	// BlendState
//	{
//        BlendMode mode = stage->getBlendModeFinal(priorityMaterial);
//		BlendStateDesc state;
//		state.independentBlendEnable = false;
//		makeBlendMode(mode, &state.renderTargets[0]);
//		context->setBlendState(state);
//	}
//	// RasterizerState
//	{
//		RasterizerStateDesc state;
//		state.fillMode = FillMode::Solid;
//		state.cullMode = stage->getCullingModeFinal(priorityMaterial);
//		context->setRasterizerState(state);
//	}
//	// DepthStencilState
//	{
//		DepthStencilStateDesc state;
//		state.depthTestFunc = stage->getDepthTestFuncFinal(priorityMaterial);
//		state.depthWriteEnabled = stage->isDepthWriteEnabledFinal(priorityMaterial);
//		context->setDepthStencilState(state);
//	}
//}
//
//void SceneRenderer::makeBlendMode(BlendMode mode, RenderTargetBlendDesc* state)
//{
//	// もっといろいろ http://d.hatena.ne.jp/Ko-Ta/20070618/p1
//	// TODO: アルファも一緒のブレンド方式にしているので、個別指定で改善できそう
//	switch (mode)
//	{
//	case BlendMode::Normal:
//		state->blendEnable = false;
//		state->sourceBlend = BlendFactor::One;
//		state->destinationBlend = BlendFactor::Zero;
//		state->blendOp = BlendOp::Add;
//		state->sourceBlendAlpha = BlendFactor::One;
//		state->destinationBlendAlpha = BlendFactor::Zero;
//		state->blendOpAlpha = BlendOp::Add;
//		break;
//	case BlendMode::Alpha:
//		state->blendEnable = true;
//		state->sourceBlend = BlendFactor::SourceAlpha;
//		state->destinationBlend = BlendFactor::InverseSourceAlpha;
//		state->blendOp = BlendOp::Add;
//		state->sourceBlendAlpha = BlendFactor::SourceAlpha;
//		state->destinationBlendAlpha = BlendFactor::InverseSourceAlpha;
//		state->blendOpAlpha = BlendOp::Add;
//		break;
//	case BlendMode::Add:
//		state->blendEnable = true;
//		state->sourceBlend = BlendFactor::SourceAlpha;
//		state->destinationBlend = BlendFactor::One;
//		state->blendOp = BlendOp::Add;
//		state->sourceBlendAlpha = BlendFactor::SourceAlpha;
//		state->destinationBlendAlpha = BlendFactor::One;
//		state->blendOpAlpha = BlendOp::Add;
//		break;
//	case BlendMode::Subtract:
//		state->blendEnable = true;
//		state->sourceBlend = BlendFactor::SourceAlpha;
//		state->destinationBlend = BlendFactor::One;
//		state->blendOp = BlendOp::ReverseSubtract;
//		state->sourceBlendAlpha = BlendFactor::SourceAlpha;
//		state->destinationBlendAlpha = BlendFactor::One;
//		state->blendOpAlpha = BlendOp::Add;
//		break;
//	case BlendMode::Multiply:
//		state->blendEnable = true;
//		state->sourceBlend = BlendFactor::Zero;	// AlphaDisable (Alpha を別指定できない今の仕様では Alpha を考慮できない)
//		state->destinationBlend = BlendFactor::SourceColor;
//		state->blendOp = BlendOp::Add;
//		state->sourceBlendAlpha = BlendFactor::SourceAlpha;
//		state->destinationBlendAlpha = BlendFactor::One;
//		state->blendOpAlpha = BlendOp::Add;
//		break;
//	//case BlendMode_Screen:
//	//	m_dxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
//	//	m_dxDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
//	//	m_dxDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHASAT);
//	//	m_dxDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
//	//	m_dxDevice->SetRenderState(D3DRS_ALPHAREF, 255);
//	//	break;
//	//case BlendMode_Reverse:
//	//	m_dxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
//	//	m_dxDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
//	//	m_dxDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//	//	m_dxDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCCOLOR);
//	//	m_dxDevice->SetRenderState(D3DRS_ALPHAREF, 1);
//	//	break;
//	default:
//		assert(0);
//		break;
//	}
//}

} // namespace detail
} // namespace ln

