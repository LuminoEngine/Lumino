
#include "../Internal.h"
#include <Lumino/Rendering/Rendering.h>
#include <Lumino/Rendering/SceneRenderer.h>
#include "../Graphics/GraphicsManager.h"
#include "../Graphics/CoreGraphicsRenderFeature.h"
#include "ClusteredShadingSceneRenderer.h"
#include "RenderStage.h"

LN_NAMESPACE_BEGIN
namespace detail {


//==============================================================================
// SceneRenderer
//==============================================================================

//------------------------------------------------------------------------------
SceneRenderer::SceneRenderer()
{
}

//------------------------------------------------------------------------------
SceneRenderer::~SceneRenderer()
{
}

//------------------------------------------------------------------------------
void SceneRenderer::initialize(GraphicsManager* manager)
{
	m_manager = manager;
	m_defaultMaterial = CommonMaterial::create();
}

//------------------------------------------------------------------------------
void SceneRenderer::onPreRender(DrawElementList* elementList)
{
}

void SceneRenderer::onCollectLight(DynamicLightInfo* light)
{
}

void SceneRenderer::onShaderPassChainging(ShaderPass* pass)
{
	Shader* shader = pass->getOwnerShader();

	ShaderVariable* v;

	// TODO: SceneParam として設定したい
	if (!m_renderingShadowCasterPassList.isEmpty())
	{
		v = shader->findVariable(_T("ln_DirectionalShadowMap"));
		if (v) v->setTexture(m_renderingShadowCasterPassList[0]->m_shadowMap);

		v = shader->findVariable(_T("ln_DirectionalShadowMapPixelSize"));
		if (v) v->setVector(Vector4(m_renderingShadowCasterPassList[0]->m_shadowMap->getWidth(), m_renderingShadowCasterPassList[0]->m_shadowMap->getHeight(), 0, 0));	//TODO: implicit

		v = shader->findVariable(_T("ln_ViewProjection_Light0"));
		if (v) v->setMatrix(m_renderingShadowCasterPassList[0]->view.viewProjMatrix);
	}
}

//ShaderTechnique* SceneRenderer::selectShaderTechnique(Shader* shader)
//{
//	return shader->getTechniques().getAt(0);
//}

//------------------------------------------------------------------------------
void SceneRenderer::addPass(RenderingPass2* pass)
{
	m_renderingPassList.add(pass);
}

//------------------------------------------------------------------------------
void SceneRenderer::render(
	RenderView* drawElementListSet,
	//DrawElementList* elementList,
	//const detail::CameraInfo& cameraInfo,
	RenderTargetTexture* defaultRenderTarget,
	DepthBuffer* defaultDepthBuffer,
	RenderDiag* diag,
	bool clearColorBuffer,
	const Color& clearColor)
{
	m_renderingRenderView = drawElementListSet;
	m_renderingDefaultRenderTarget = defaultRenderTarget;
	m_renderingDefaultDepthBuffer = defaultDepthBuffer;

	detail::CoreGraphicsRenderFeature* coreRenderer = m_manager->getRenderer();
	coreRenderer->begin();


	if (clearColorBuffer)
	{
		coreRenderer->setRenderTarget(0, defaultRenderTarget);
		coreRenderer->setDepthBuffer(defaultDepthBuffer);
		coreRenderer->clear(ClearFlags::All, clearColor);
	}



	if (diag != nullptr) diag->beginRenderView();
	if (diag != nullptr) diag->beginDrawList();

	InternalContext* context = m_manager->getInternalContext();
	//const detail::CameraInfo& cameraInfo = drawElementListSet->m_cameraInfo;


	DrawElement::DrawArgs drawArgs;
	//drawArgs.oenerList = elementList;
	drawArgs.context = context;
	drawArgs.renderer = this;
	drawArgs.defaultRenderTarget = defaultRenderTarget;
	drawArgs.defaultDepthBuffer = defaultDepthBuffer;
	drawArgs.diag = diag;

	// TODO: 別 RT への描画は RenderStage 的な感じで分けて、メインの Pass でだけ行うようにしたほうが良い。
	// こういうものは ShadowCast のパスで実行したくない。無駄になる。


	m_renderingActualPassList.clear();
	m_renderingShadowCasterPassList.clear();
	for (auto& elementList : m_renderingRenderView->m_lists)
	{
		for (DynamicLightInfo* light : elementList->getDynamicLightList())
		{
			if (light->m_shadowCasterPass != nullptr)
			{
				m_renderingActualPassList.add(light->m_shadowCasterPass);
				m_renderingShadowCasterPassList.add(light->m_shadowCasterPass);
			}
		}
	}
	//m_renderingActualPassList.addRange(m_renderingPassList);
	for (RenderingPass2* pass : m_renderingPassList)
	{
		m_renderingActualPassList.add(pass);
	}

	for (RenderingPass2* pass : m_renderingActualPassList)
	{
		m_renderingElementList.clear();

		detail::CameraInfo cameraInfo = drawElementListSet->m_cameraInfo;

		pass->overrideCameraInfo(&cameraInfo);

		collect(pass, cameraInfo);

		prepare();


		DefaultStatus defaultStatus;
		defaultStatus.defaultRenderTarget[0] = defaultRenderTarget;
		defaultStatus.defaultRenderTarget[1] = defaultStatus.defaultRenderTarget[2] = defaultStatus.defaultRenderTarget[3] = nullptr;
		defaultStatus.defaultDepthBuffer = defaultDepthBuffer;

		pass->onBeginPass(&defaultStatus);

		// DrawElement 描画
		//int currentBatchIndex = -1;
		//DrawElementBatch* currentState = nullptr;
		//Shader* currentShader = nullptr;
		RenderStage* currentStage = nullptr;
		for (DrawElement* element : m_renderingElementList)
		{
			bool visible = true;
			drawArgs.oenerList = element->m_ownerDrawElementList;

			//DrawElementBatch* batch = element->m_ownerDrawElementList->getBatch(element->batchIndex);
			RenderStage* stage = element->m_ownerDrawElementList->getRenderStage(element->batchIndex);

			// ステートの変わり目チェック
			//if (element->batchIndex != currentBatchIndex)
			if (currentStage == nullptr || !currentStage->equals(stage) /*currentState->getHashCode() != batch->getHashCode()*/)
			{
				context->flush();
				//currentBatchIndex = element->batchIndex;
				currentStage = stage;
				context->applyStatus(currentStage, defaultStatus);
				//context->switchActiveRenderer(currentState->getRenderFeature());
				if (diag != nullptr) diag->changeRenderStage();
			}

			context->switchActiveRenderer(element->renderFeature);

			// 固定の内部シェーダを使わない場合はいろいろ設定する
			//if (currentStage->getRenderFeature() == nullptr ||	// TODO: だめ。でもいまやらかしてる人がいるので、後で ASSERT 張って対応する
			//	!currentStage->getRenderFeature()->isStandaloneShader())
			if (context->getCurrentRenderFeature() == nullptr ||	// TODO: だめ。でもいまやらかしてる人がいるので、後で ASSERT 張って対応する
				!context->getCurrentRenderFeature()->isStandaloneShader())
			{
				//CombinedMaterial* material = currentState->getCombinedMaterial();
				RenderingPass2::RenderStageFinalData stageData;
				stageData.stage = currentStage;
				stageData.material = currentStage->getMaterialFinal(m_defaultMaterial, element->priorityMaterial);
				stageData.shader = currentStage->getShaderFinal(stageData.material);
				stageData.shadingModel = currentStage->getShadingModelFinal(stageData.material);

				ElementRenderingPolicy policy;
				pass->selectElementRenderingPolicy(element, stageData, &policy);
				visible = policy.visible;

				if (visible)
				{
					Shader* shader = policy.shader;

					ElementInfo elementInfo;
					element->makeElementInfo(element->m_ownerDrawElementList, cameraInfo, drawElementListSet, &elementInfo);
					//drawElementListSet->filterWorldMatrix(&elementInfo.WorldMatrix);
					//elementInfo.WorldViewProjectionMatrix = elementInfo.WorldMatrix * cameraInfo.viewMatrix * cameraInfo.projMatrix;	// TODO: viewProj はまとめたい



					SubsetInfo subsetInfo;
					subsetInfo.renderStage = currentStage;
					subsetInfo.finalMaterial = stageData.material;
					subsetInfo.materialTexture = (subsetInfo.finalMaterial != nullptr) ? subsetInfo.finalMaterial->getMaterialTexture(nullptr) : nullptr;
					element->makeSubsetInfo(element->m_ownerDrawElementList, currentStage, &subsetInfo);

					//currentState->IsStandaloneShaderRenderer

					if (context->getCurrentRenderFeature() != nullptr)
					{
						context->getCurrentRenderFeature()->onShaderElementInfoOverride(&elementInfo);
						context->getCurrentRenderFeature()->onShaderSubsetInfoOverride(&subsetInfo);
					}

					shader->getSemanticsManager()->updateCameraVariables(cameraInfo);
					shader->getSemanticsManager()->updateElementVariables(cameraInfo, elementInfo);
					shader->getSemanticsManager()->updateSubsetVariables(subsetInfo);

					//material->applyUserShaderValeues(shader);
					stageData.material->applyUserShaderValeues(shader);

					onShaderPassChainging(policy.shaderPass);

					auto* stateManager = context->getRenderStateManager();
					stateManager->setShaderPass(policy.shaderPass);
				}
			}

			// 描画実行
			if (visible)
			{
				if (diag != nullptr) element->reportDiag(diag);
				element->drawSubset(drawArgs);
			}
		}

		context->flush();
	}

	// Flush
	{
		m_manager->getInternalContext()->flush();
	}

	if (diag != nullptr) diag->endDrawList();
	if (diag != nullptr) diag->endRenderView();

	coreRenderer->end();
}

void SceneRenderer::collect(RenderingPass2* pass, const detail::CameraInfo& cameraInfo)
{
	InternalContext* context = m_manager->getInternalContext();
	//const detail::CameraInfo& cameraInfo = m_renderingRenderView->m_cameraInfo;

	for (auto& elementList : m_renderingRenderView->m_lists)
	{
		elementList->setDefaultRenderTarget(m_renderingDefaultRenderTarget);
		elementList->setDefaultDepthBuffer(m_renderingDefaultDepthBuffer);

		for (DynamicLightInfo* light : elementList->getDynamicLightList())
		{
			onCollectLight(light);
		}

		onPreRender(elementList);

		// 視点に関する情報の設定
		context->setViewInfo(cameraInfo.viewPixelSize, cameraInfo.viewMatrix, cameraInfo.projMatrix);

		// ライブラリ外部への書き込み対応
		//context->beginBaseRenderer()->Clear(ClearFlags::Depth/* | ClearFlags::Stencil*/, Color());

		// 視錘台カリング
		for (int i = 0; i < elementList->getElementCount(); ++i)
		{
			DrawElement* element = elementList->getElement(i);
			Sphere boundingSphere = element->getBoundingSphere();

			if (boundingSphere.radius < 0 ||	// マイナス値なら視錐台と衝突判定しない
				cameraInfo.viewFrustum.intersects(boundingSphere.center, boundingSphere.radius))
			{
				// このノードは描画できる
				m_renderingElementList.add(element);

				// calculate distance for ZSort
				const Matrix& transform = element->getTransform(elementList);
				switch (cameraInfo.zSortDistanceBase)
				{
				case ZSortDistanceBase::NodeZ:
					element->zDistance = transform.getPosition().z;
					break;
				case ZSortDistanceBase::CameraDistance:
					element->zDistance = (transform.getPosition() - cameraInfo.viewPosition).getLengthSquared();
					break;
				case ZSortDistanceBase::CameraScreenDistance:
					element->zDistance = Vector3::dot(
						transform.getPosition() - cameraInfo.viewPosition,
						cameraInfo.viewDirection);		// 平面と点の距離
														// TODO: ↑第2引数違くない？要確認
					break;
				default:
					element->zDistance = 0.0f;
					break;
				}
			}
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
		[](const DrawElement* lhs, const DrawElement* rhs)
	{
		if (lhs->m_stateFence == rhs->m_stateFence)
		{
			if (lhs->metadata.priority == rhs->metadata.priority)
				return lhs->zDistance > rhs->zDistance;
			return lhs->metadata.priority < rhs->metadata.priority;
		}
		else
		{
			return lhs->m_stateFence < rhs->m_stateFence;
		}
	}
	);
}


//==============================================================================
// NonShadingRenderer
//==============================================================================

//------------------------------------------------------------------------------
NonShadingRenderer::NonShadingRenderer()
{
}

//------------------------------------------------------------------------------
NonShadingRenderer::~NonShadingRenderer()
{
}

//------------------------------------------------------------------------------
void NonShadingRenderer::initialize(GraphicsManager* manager)
{
	SceneRenderer::initialize(manager);

	auto pass = Ref<detail::NonShadingRenderingPass>::makeRef();
	pass->initialize(manager);
	addPass(pass);
}


//==============================================================================
// NonShadingRenderingPass
//==============================================================================
//------------------------------------------------------------------------------
NonShadingRenderingPass::NonShadingRenderingPass()
{
}

//------------------------------------------------------------------------------
NonShadingRenderingPass::~NonShadingRenderingPass()
{
}

//------------------------------------------------------------------------------
void NonShadingRenderingPass::initialize(GraphicsManager* manager)
{
	m_defaultShader = manager->getBuiltinShader(BuiltinShader::Sprite);
}

void NonShadingRenderingPass::selectElementRenderingPolicy(DrawElement* element, const RenderStageFinalData& stageData, ElementRenderingPolicy* outPolicy)
{
	outPolicy->shaderPass = selectShaderPassHelperSimple(stageData.shader, m_defaultShader);
	outPolicy->shader = outPolicy->shaderPass->getOwnerShader();
	outPolicy->visible = true;
}

////------------------------------------------------------------------------------
//Shader* NonShadingRenderingPass::getDefaultShader() const
//{
//	return m_defaultShader;
//}



//==============================================================================
// ForwardShadingRenderer
//==============================================================================

//------------------------------------------------------------------------------
ForwardShadingRenderer::ForwardShadingRenderer()
{
}

//------------------------------------------------------------------------------
ForwardShadingRenderer::~ForwardShadingRenderer()
{
}

//------------------------------------------------------------------------------
void ForwardShadingRenderer::initialize(GraphicsManager* manager)
{
	SceneRenderer::initialize(manager);

	auto pass = Ref<detail::ForwardShadingRenderingPass>::makeRef();
	pass->initialize(manager);
	addPass(pass);
}

//------------------------------------------------------------------------------
void ForwardShadingRenderer::onPreRender(DrawElementList* elementList)
{
	auto& lights = elementList->getDynamicLightList();
	m_selectingLights.clear();
	for (DynamicLightInfo* light : lights)
	{
		m_selectingLights.add(light);
	}

	for (int i = 0; i < elementList->getElementCount(); i++)
	{
		DrawElement* element = elementList->getElement(i);
		DynamicLightInfo** lightInfos = element->getAffectedDynamicLightInfos();
		if (lightInfos != nullptr)
		{
			updateAffectLights(element, elementList);
		}
	}
}

//------------------------------------------------------------------------------
void ForwardShadingRenderer::updateAffectLights(DrawElement* element, DrawElementList* elementList)
{
	/*
	まず全てのライトに、このノードとの距離をセットする。
	その後近い順にソートして、ソート結果の先頭から必要な数だけ取りだしている。
	ライトの数が少なければどんなアルゴリズムにしても大差はないと思うが、
	ノード単位でソートが実行されるので速度的に少し心配。
	先頭数個が確定したときにソートを終了する等、最適化の余地はあるが…。
	*/

	if (m_selectingLights.getCount() > DynamicLightInfo::MaxLights)
	{
		// ソート基準値の計算
		for (DynamicLightInfo* light : m_selectingLights)
		{
			light->tempDistance = Vector3::distanceSquared(element->getTransform(elementList).getPosition(), light->m_position);
		}

		// ソート (昇順)
		std::stable_sort(m_selectingLights.begin(), m_selectingLights.end(), [](DynamicLightInfo* lhs, DynamicLightInfo* rhs) { return lhs->tempDistance < rhs->tempDistance; });
	}

	// 出力 (足りない分は nullptr で埋める)
	DynamicLightInfo** affectLightList = element->getAffectedDynamicLightInfos();
	int count = std::min(m_selectingLights.getCount(), DynamicLightInfo::MaxLights);
	int i = 0;
	for (; i < count; ++i)
	{
		affectLightList[i] = m_selectingLights[i];
	}
	for (; i < DynamicLightInfo::MaxLights; ++i)
	{
		affectLightList[i] = nullptr;
	}
}

//==============================================================================
// ForwardShadingRenderingPass
//==============================================================================
//------------------------------------------------------------------------------
ForwardShadingRenderingPass::ForwardShadingRenderingPass()
{
}

//------------------------------------------------------------------------------
ForwardShadingRenderingPass::~ForwardShadingRenderingPass()
{
}

//------------------------------------------------------------------------------
void ForwardShadingRenderingPass::initialize(GraphicsManager* manager)
{
	m_defaultShader = manager->getBuiltinShader(BuiltinShader::LegacyDiffuse);
}

void ForwardShadingRenderingPass::selectElementRenderingPolicy(DrawElement* element, const RenderStageFinalData& stageData, ElementRenderingPolicy* outPolicy)
{
	outPolicy->shaderPass = selectShaderPassHelperSimple(stageData.shader, m_defaultShader);
	outPolicy->shader = outPolicy->shaderPass->getOwnerShader();
	outPolicy->visible = true;
}

////------------------------------------------------------------------------------
//Shader* ForwardShadingRenderingPass::getDefaultShader() const
//{
//	return m_defaultShader;
//}


//==============================================================================
// InfomationRenderingPass
//==============================================================================

//------------------------------------------------------------------------------
InfomationRenderingPass::InfomationRenderingPass()
{
}

//------------------------------------------------------------------------------
InfomationRenderingPass::~InfomationRenderingPass()
{
}

//------------------------------------------------------------------------------
void InfomationRenderingPass::initialize(GraphicsManager* manager)
{
	NonShadingRenderingPass::initialize(manager);
}


//==============================================================================
// CombinedMaterialCache
//==============================================================================

//------------------------------------------------------------------------------
Ref<CombinedMaterial> CombinedMaterialCache::createObject()
{
	auto m = Ref<CombinedMaterial>::makeRef();
	return m;
}


////==============================================================================
//// ScopedStateBlock2
////==============================================================================
//
////------------------------------------------------------------------------------
//ScopedStateBlock2::ScopedStateBlock2(DrawList* renderer)
//	: m_renderer(renderer)
//	, m_state(renderer->getState())
//{}
//
////------------------------------------------------------------------------------
//ScopedStateBlock2::~ScopedStateBlock2()
//{
//	apply();
//}
//
////------------------------------------------------------------------------------
//void ScopedStateBlock2::apply()
//{
//	m_renderer->setState(m_state);
//}
//
//




//==============================================================================
// RenderingPass
//==============================================================================
//------------------------------------------------------------------------------
RenderingPass2::RenderingPass2()
{

}

//------------------------------------------------------------------------------
RenderingPass2::~RenderingPass2()
{
}

//------------------------------------------------------------------------------
//void RenderingPass2::selectElementRenderingPolicy(DrawElement* element, CombinedMaterial* material, ElementRenderingPolicy* outPolicy)
//{
//	outPolicy->shader = nullptr;
//	if (material != nullptr && material->m_shader != nullptr)
//	{
//		outPolicy->shader = material->m_shader;
//	}
//	else
//	{
//		outPolicy->shader = getDefaultShader();
//	}
//
//	if (outPolicy->shader)
//	{
//		outPolicy->shaderPass = selectShaderPass(outPolicy->shader);
//	}
//
//	// とありあえず全部可
//	outPolicy->visible = true;
//}

//ShaderTechnique* RenderingPass2::selectShaderTechnique(Shader* shader)
//{
//	return shader->getTechniques().getAt(0)->getPasses().getAt(0);
//}

void RenderingPass2::onBeginPass(DefaultStatus* defaultStatus)
{

}

void RenderingPass2::overrideCameraInfo(detail::CameraInfo* cameraInfo)
{

}
//
//ShaderPass* RenderingPass2::selectShaderPass(Shader* shader)
//{
//	// TODO: DrawList の実行者によって決定する
//	return shader->getTechniques().getAt(0)->getPasses().getAt(0);
//}

ShaderPass* RenderingPass2::selectShaderPassHelper(Shader* materialShader, const String& techniqueName, const String& passName, ShaderPass* defaultPass)
{
	if (materialShader)
	{
		ShaderTechnique* tech = materialShader->findTechnique(techniqueName);
		if (tech)
		{
			ShaderPass* pass = tech->getPass(passName.c_str());	// TODO:
			if (pass)
			{
				return pass;
			}
		}
	}

	return defaultPass;
}

ShaderPass* RenderingPass2::selectShaderPassHelperSimple(Shader* materialShader, Shader* defaultShader)
{
	Shader* shader = (materialShader) ? materialShader : defaultShader;
	return shader->getTechniques().getAt(0)->getPasses().getAt(0);
}

////------------------------------------------------------------------------------
//void RenderingPass2::initialize(GraphicsManager* manager)
//{
//	m_defaultShader = manager->getDefaultShader(DefaultShader::NoLightingRendering);
//}
//
////------------------------------------------------------------------------------
//Shader* RenderingPass2::getDefaultShader() const
//{
//	return m_defaultShader;
//}

////------------------------------------------------------------------------------
//void RenderingPass2::RenderElement(DrawList* renderer, DrawElement* element)
//{
//}
//
////------------------------------------------------------------------------------
//void RenderingPass2::RenderElementSubset(DrawList* renderer, DrawElement* element, int subsetIndex)
//{
//}

} // namespace detail
LN_NAMESPACE_END

