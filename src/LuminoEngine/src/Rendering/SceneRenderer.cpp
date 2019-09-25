
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include "../Graphics/GraphicsManager.hpp"
#include "RenderStage.hpp"
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

void SceneRendererPass::onBeginRender(SceneRenderer* sceneRenderer)
{
}

void SceneRendererPass::onEndRender(SceneRenderer* sceneRenderer)
{
}

void SceneRendererPass::onBeginPass(GraphicsContext* context, FrameBuffer* frameBuffer)
{
}

//==============================================================================
// SceneRenderer

SceneRenderer::SceneRenderer()
	: m_manager(detail::EngineDomain::renderingManager())
	, m_renderFeatureBatchList(detail::EngineDomain::renderingManager())
	, m_renderingPipeline(nullptr)
	, m_zSortDistanceBase(ZSortDistanceBase::CameraScreenDistance)
{
}

void SceneRenderer::init()
{
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

void SceneRenderer::render(
	GraphicsContext* graphicsContext,
    RenderingPipeline* renderingPipeline,
	const FrameBuffer& defaultFrameBuffer,
    const CameraInfo& mainCameraInfo,
    RendringPhase targetPhase)
{
	//if (LN_REQUIRE(m_defaultMaterial)) return;

	m_renderingPipeline = renderingPipeline;
	m_defaultFrameBuffer = &defaultFrameBuffer;
    m_mainCameraInfo = mainCameraInfo;
    m_targetPhase = targetPhase;

	//detail::CoreGraphicsRenderFeature* coreRenderer = m_manager->getRenderer();
	//coreRenderer->begin();


	//if (clearColorBuffer)
	{
        // TODO: 暫定。
        graphicsContext->setRenderTarget(0, defaultFrameBuffer.renderTarget[0]);
        graphicsContext->setDepthBuffer(defaultFrameBuffer.depthBuffer);
        graphicsContext->clear(ClearFlags::Depth, Color::White);
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


	for (SceneRendererPass* pass : m_renderingActualPassList)
	{
		pass->onBeginRender(this);
	}

	for (SceneRendererPass* pass : m_renderingActualPassList)
	{
        renderPass(graphicsContext, pass);
	}

	for (SceneRendererPass* pass : m_renderingActualPassList)
	{
		pass->onEndRender(this);
    }

	//// Flush
	//{
	//	m_manager->getInternalContext()->flush();
	//}

	//if (diag != nullptr) diag->endDrawList();
	//if (diag != nullptr) diag->endRenderView();

	//coreRenderer->end();
}

//void SceneRenderer::setDefaultMaterial(AbstractMaterial* material)
//{
//	m_defaultMaterial = material;
//}

void SceneRenderer::addPass(SceneRendererPass* pass)
{
	m_renderingPassList.add(pass);
}

void SceneRenderer::renderPass(GraphicsContext* graphicsContext, SceneRendererPass* pass)
{
	m_renderingElementList.clear();

	FrameBuffer defaultFrameBuffer = *m_defaultFrameBuffer;
	pass->onBeginPass(graphicsContext, &defaultFrameBuffer);


	const detail::CameraInfo& cameraInfo = mainCameraInfo();

	//pass->overrideCameraInfo(&cameraInfo);

	collect(/*pass, */cameraInfo);

	prepare();

#ifdef LN_RENDERING_MIGRATION
	m_renderFeatureBatchList.clear();

	// Create batch list.
	{
		RenderStage* currentStage = nullptr;
		const Matrix* currentWorldMatrix = nullptr;
		AbstractMaterial* currentFinalMaterial = nullptr;
		SubsetInfo currentSubsetInfo;
		for (RenderDrawElement* element : m_renderingElementList)
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

			// ShaderDescripter
			SubsetInfo subsetInfo;
			const Matrix* worldMatrix = nullptr;
			AbstractMaterial* finalMaterial = nullptr;
			if (element->elementType == RenderDrawElementType::Geometry) {
				if (stage->renderFeature->drawElementTransformNegate()) {
					worldMatrix = nullptr;
				}
				else {
					worldMatrix = &element->combinedWorldMatrix();
				}

				finalMaterial = stage->getMaterialFinal(nullptr, m_manager->builtinMaterials(BuiltinMaterial::Default));

				if (finalMaterial)
					subsetInfo.materialTexture = finalMaterial->mainTexture();
				else
					subsetInfo.materialTexture = nullptr;
				subsetInfo.opacity = stage->getOpacityFinal(element);
				subsetInfo.colorScale = stage->getColorScaleFinal(element);
				subsetInfo.blendColor = stage->getBlendColorFinal(element);
				subsetInfo.tone = stage->getToneFinal(element);
			}
			else {
				subsetInfo.clear();
			}

			// ShaderDescripter に関係するパラメータの変更チェック
			if (!submitRequested && m_renderFeatureBatchList.lastBatch() && !SubsetInfo::equals(currentSubsetInfo, subsetInfo)) {
				submitRequested = true;
			}

			// Batch 確定
			if (submitRequested) {
				if (!currentStage) {
					// 初回. 1つも draw 仕様としていないので submit は不要.
				}
				else {
					currentStage->renderFeature->submitBatch(graphicsContext, &m_renderFeatureBatchList);
					m_renderFeatureBatchList.lastBatch()->setWorldTransformPtr(currentWorldMatrix);
					m_renderFeatureBatchList.lastBatch()->setFinalMaterial(currentFinalMaterial);
					m_renderFeatureBatchList.lastBatch()->setSubsetInfo(currentSubsetInfo);
				}
				currentWorldMatrix = worldMatrix;
				currentFinalMaterial = finalMaterial;
				currentSubsetInfo = subsetInfo;
				currentStage = stage;
				m_renderFeatureBatchList.setCurrentStage(currentStage);
			}

			element->onDraw(graphicsContext, currentStage->renderFeature, &subsetInfo);
		}

		if (currentStage) {
			currentStage->renderFeature->submitBatch(graphicsContext, &m_renderFeatureBatchList);
			m_renderFeatureBatchList.lastBatch()->setWorldTransformPtr(currentWorldMatrix);
			m_renderFeatureBatchList.lastBatch()->setFinalMaterial(currentFinalMaterial);
			m_renderFeatureBatchList.lastBatch()->setSubsetInfo(currentSubsetInfo);
		}
	}

	// Render batch-list.
	{
		RenderFeatureBatch* batch = m_renderFeatureBatchList.firstBatch();
		while (batch)
		{
			const RenderStage* stage = batch->stage();
			const AbstractMaterial* finalMaterial = batch->finalMaterial();
			const SubsetInfo& subsetInfo = batch->subsetInfo();

			if (!finalMaterial) {
				// Shader 使わない描画 (clear)
				batch->render(graphicsContext);
			}
			else {
				ElementInfo elementInfo;
				elementInfo.viewProjMatrix = &cameraInfo.viewProjMatrix;
				elementInfo.WorldMatrix = (batch->worldTransformPtr()) ? *batch->worldTransformPtr() : Matrix::Identity;
				elementInfo.WorldViewProjectionMatrix = elementInfo.WorldMatrix * (*elementInfo.viewProjMatrix);
				elementInfo.boneTexture = nullptr;	// TODO:
				elementInfo.boneLocalQuaternionTexture = nullptr;	// TODO:

				ShaderTechniqueClass_MeshProcess meshProcess = ShaderTechniqueClass_MeshProcess::StaticMesh;	// TODO:
				ShaderTechnique* tech = pass->selectShaderTechnique(
					meshProcess,
					finalMaterial->shader(),
					stage->getShadingModelFinal(finalMaterial));

				SubsetInfo localSubsetInfo = subsetInfo;
				if (!localSubsetInfo.materialTexture) {
					localSubsetInfo.materialTexture = m_manager->graphicsManager()->whiteTexture();
				}


				detail::ShaderSemanticsManager* semanticsManager = ShaderHelper::semanticsManager(tech->shader());
				//semanticsManager->updateCameraVariables(cameraInfo);
				//semanticsManager->updateElementVariables(cameraInfo, elementInfo);
				//semanticsManager->updateSubsetVariables(subsetInfo);
				//if (stage->renderFeature) {
				//	stage->renderFeature->updateRenderParameters(element, tech, cameraInfo, elementInfo, subsetInfo);
				//}
				//else
				// TODO: ↑SpriteTextRenderer が Font 取るのに使ってる。これは Batch に持っていくべきだろう。
				{
					RenderFeature::updateRenderParametersDefault(tech, cameraInfo, elementInfo, subsetInfo);
				}

				if (finalMaterial) {
					PbrMaterialData pbrMaterialData;
					finalMaterial->translateToPBRMaterialData(&pbrMaterialData);
					semanticsManager->updateSubsetVariables_PBR(pbrMaterialData);
					finalMaterial->updateShaderVariables(tech->shader());
				}

				onSetAdditionalShaderPassVariables(tech->shader());

				for (ShaderPass* pass2 : tech->passes())
				{
					graphicsContext->setShaderPass(pass2);
					batch->render(graphicsContext);
				}
			}


			batch = batch->next();
		}
	}

#else
	RenderStage* currentStage = nullptr;
	for (RenderDrawElement* element : m_renderingElementList)
	{
		RenderStage* stage = element->stage();
		// ステートの変わり目チェック
		if (currentStage == nullptr || !currentStage->equals(stage))
		{
			if (currentStage) {
				currentStage->flush(graphicsContext);
			}
			currentStage = stage;
			RenderStage::applyFrameBufferStatus(graphicsContext, currentStage, defaultFrameBuffer);
		}

		// DrawElement drawing.
		{

			if (element->elementType == RenderDrawElementType::Geometry)
			{
				AbstractMaterial* finalMaterial = currentStage->getMaterialFinal(nullptr, m_manager->builtinMaterials(BuiltinMaterial::Default));
				RenderStage::applyGeometryStatus(graphicsContext, currentStage, finalMaterial);

                Texture* mainTexture = nullptr;
                if (finalMaterial) {
                    mainTexture = finalMaterial->mainTexture();
                }
                if (!mainTexture) {
                    mainTexture = m_manager->graphicsManager()->whiteTexture();
                }

                ShaderTechniqueClass_MeshProcess meshProcess = ShaderTechniqueClass_MeshProcess::StaticMesh;

				ElementInfo elementInfo;
				elementInfo.viewProjMatrix = &cameraInfo.viewProjMatrix;
				elementInfo.WorldMatrix = (currentStage->renderFeature && !currentStage->renderFeature->drawElementTransformNegate()) ? element->combinedWorldMatrix() : Matrix::Identity;
				elementInfo.WorldViewProjectionMatrix = elementInfo.WorldMatrix * (*elementInfo.viewProjMatrix);
				elementInfo.boneTexture = m_skinningMatricesTexture;
				elementInfo.boneLocalQuaternionTexture = m_skinningLocalQuaternionsTexture;
                //element->onElementInfoOverride(&elementInfo, &meshProcess);

				SubsetInfo subsetInfo;
				subsetInfo.materialTexture = mainTexture;
				subsetInfo.opacity = currentStage->getOpacityFinal(element);
				subsetInfo.colorScale = currentStage->getColorScaleFinal(element);
				subsetInfo.blendColor = currentStage->getBlendColorFinal(element);
				subsetInfo.tone = currentStage->getToneFinal(element);
				//element->onSubsetInfoOverride(&subsetInfo);

				ShaderTechnique* tech = pass->selectShaderTechnique(
                    meshProcess,
					finalMaterial->shader(),
					currentStage->getShadingModelFinal(finalMaterial));

                detail::ShaderSemanticsManager* semanticsManager = ShaderHelper::semanticsManager(tech->shader());
				//semanticsManager->updateCameraVariables(cameraInfo);
				//semanticsManager->updateElementVariables(cameraInfo, elementInfo);
				//semanticsManager->updateSubsetVariables(subsetInfo);
                if (currentStage->renderFeature) {
                    currentStage->renderFeature->updateRenderParameters(element, tech, cameraInfo, elementInfo, subsetInfo);
                }
                else {
                    RenderFeature::updateRenderParametersDefault(tech, cameraInfo, elementInfo, subsetInfo);
                }

				if (finalMaterial) {
					PbrMaterialData pbrMaterialData;
					finalMaterial->translateToPBRMaterialData(&pbrMaterialData);
					semanticsManager->updateSubsetVariables_PBR(pbrMaterialData);
					finalMaterial->updateShaderVariables(tech->shader());
				}

				onSetAdditionalShaderPassVariables(tech->shader());

				for (ShaderPass* pass2 : tech->passes())
				{
					graphicsContext->setShaderPass(pass2);

					element->onDraw(graphicsContext, currentStage->renderFeature, &subsetInfo);
				}
			}
			else if (element->elementType == RenderDrawElementType::Clear)
			{
				RenderStage::applyGeometryStatus(graphicsContext, currentStage, nullptr);
				element->onDraw(graphicsContext, currentStage->renderFeature, nullptr);
			}
			else
			{
				LN_UNREACHABLE();
			}
		}
	}

	if (currentStage) {
		currentStage->flush(graphicsContext);
	}
#endif


}

void SceneRenderer::collect(/*SceneRendererPass* pass, */const detail::CameraInfo& cameraInfo)
{
	//InternalContext* context = m_manager->getInternalContext();
	//const detail::CameraInfo& cameraInfo = m_renderingRenderView->m_cameraInfo;


	for (auto& elementListManager : *m_renderingPipeline->elementListManagers())
	{
		// TODO: とりあえず Default
		

		for (auto& elementList : elementListManager->lists(/*RendringPhase::Default*/))
		{
			//elementList->setDefaultRenderTarget(m_renderingDefaultRenderTarget);
			//elementList->setDefaultDepthBuffer(m_renderingDefaultDepthBuffer);

			for (auto& light : elementList->dynamicLightInfoList())
			{
				onCollectLight(light);
			}

			//onPreRender(elementList);

			// 視点に関する情報の設定
			//context->setViewInfo(cameraInfo.viewPixelSize, cameraInfo.viewMatrix, cameraInfo.projMatrix);

			// ライブラリ外部への書き込み対応
			//context->beginBaseRenderer()->Clear(ClearFlags::Depth/* | ClearFlags::Stencil*/, Color());

			//for (int i = 0; i < elementList->getElementCount(); ++i)
			//{
			//	DrawElement* element = elementList->getElement(i);

			RenderDrawElement* element = elementList->headElement();
			while (element)
			{
                // filter phase
                if (element->targetPhase == m_targetPhase) {


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
                }


				element = element->next();
			}
		}
	}


	for (auto& element : m_renderingElementList)
	{
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
				if (lhs->priority == rhs->priority)
					return lhs->zDistance > rhs->zDistance;
				return lhs->priority < rhs->priority;
			}
			else
			{
				return lhs->commandFence < rhs->commandFence;
			}
		});
}

void SceneRenderer::onCollectLight(const DynamicLightInfo& light)
{
}

void SceneRenderer::onSetAdditionalShaderPassVariables(Shader* shader)
{
}

} // namespace detail
} // namespace ln

