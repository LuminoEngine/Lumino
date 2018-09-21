
#include "Internal.hpp"
#include <Lumino/Graphics/GraphicsContext.hpp>
#include <Lumino/Rendering/Material.hpp>
#include "RenderStage.hpp"
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

void SceneRendererPass::initialize()
{
}

void SceneRendererPass::overrideDefaultFrameBuffer(FrameBuffer* frameBuffer)
{
}

//==============================================================================
// SceneRenderer

SceneRenderer::SceneRenderer()
	: m_renderingRenderView(nullptr)
	, m_zSortDistanceBase(ZSortDistanceBase::CameraScreenDistance)
{
}

void SceneRenderer::render(
	GraphicsContext* graphicsContext,
	RenderView* renderView,
	const FrameBuffer& defaultFrameBuffer)
{
	m_renderingRenderView = renderView;
	m_defaultFrameBuffer = &defaultFrameBuffer;

	//detail::CoreGraphicsRenderFeature* coreRenderer = m_manager->getRenderer();
	//coreRenderer->begin();


	//if (clearColorBuffer)
	//{
	//	coreRenderer->setRenderTarget(0, defaultRenderTarget);
	//	coreRenderer->setDepthBuffer(defaultDepthBuffer);
	//	// TODO: 前回の最後のステートが残っていることがある。clear したいやつの弊害だけ、とりあえず暫定処置。シザーも必要になりそう。
	//	coreRenderer->setViewport(RectI(0, 0, defaultRenderTarget->getSize()));
	//	coreRenderer->clear(ClearFlags::All, clearColor);
	//}



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
	for (SceneRendererPass* pass : m_renderingPassList)
	{
		m_renderingActualPassList.add(pass);
	}

	for (SceneRendererPass* pass : m_renderingActualPassList)
	{
		renderPass(graphicsContext, pass);
	}

	//// Flush
	//{
	//	m_manager->getInternalContext()->flush();
	//}

	//if (diag != nullptr) diag->endDrawList();
	//if (diag != nullptr) diag->endRenderView();

	//coreRenderer->end();
}

void SceneRenderer::addPass(SceneRendererPass* pass)
{
	m_renderingPassList.add(pass);
}

void SceneRenderer::renderPass(GraphicsContext* graphicsContext, SceneRendererPass* pass)
{
	m_renderingElementList.clear();

	FrameBuffer defaultFrameBuffer = *m_defaultFrameBuffer;

	detail::CameraInfo cameraInfo = m_renderingRenderView->cameraInfo;

	//pass->overrideCameraInfo(&cameraInfo);

	collect(/*pass, */cameraInfo);

	prepare();



	RenderStage* currentStage = nullptr;
	for (RenderDrawElement* element : m_renderingElementList)
	{
		RenderStage* stage = element->stage();
		// ステートの変わり目チェック
		if (currentStage == nullptr || !currentStage->equals(stage))
		{
			if (currentStage) {
				currentStage->flush();
			}
			currentStage = stage;
			applyFrameBufferStatus(graphicsContext, currentStage, defaultFrameBuffer);
		}

		// DrawElement drawing.
		{
			applyGeometryStatus(graphicsContext, currentStage, nullptr);

			AbstractMaterial* finalMaterial = currentStage->getMaterialFinal(nullptr, nullptr);

			ElementInfo elementInfo;
			elementInfo.viewProjMatrix = &cameraInfo.viewProjMatrix;
			elementInfo.WorldMatrix = element->combinedWorldMatrix();
			elementInfo.WorldViewProjectionMatrix = elementInfo.WorldMatrix * (*elementInfo.viewProjMatrix);
			//elementInfo.boneTexture = m_skinningMatricesTexture;
			//elementInfo.boneLocalQuaternionTexture = m_skinningLocalQuaternionsTexture;

			SubsetInfo subsetInfo;
			subsetInfo.materialTexture = (finalMaterial) ? finalMaterial->mainTexture() : nullptr;
			subsetInfo.opacity = currentStage->getOpacityFinal();
			subsetInfo.colorScale = currentStage->getColorScaleFinal();
			subsetInfo.blendColor = currentStage->getBlendColorFinal();
			subsetInfo.tone = currentStage->getToneFinal();

			ShaderTechnique* tech = pass->selectShaderTechnique(
				ShaderTechniqueClass_MeshProcess::StaticMesh,
				nullptr,
				ShadingModel::UnLighting);

			detail::ShaderSemanticsManager* semanticsManager = tech->shader()->semanticsManager();
			semanticsManager->updateCameraVariables(cameraInfo);
			semanticsManager->updateElementVariables(cameraInfo, elementInfo);
			semanticsManager->updateSubsetVariables(subsetInfo);

			for (ShaderPass* pass : tech->passes())
			{
				graphicsContext->setShaderPass(pass);

				element->onDraw(graphicsContext, currentStage->renderFeature);
			}
		}
	}

	if (currentStage) {
		currentStage->flush();
	}

#if 0

	//DefaultStatus defaultStatus;
	//defaultStatus.defaultRenderTarget[0] = defaultRenderTarget;
	//defaultStatus.defaultRenderTarget[1] = defaultStatus.defaultRenderTarget[2] = defaultStatus.defaultRenderTarget[3] = nullptr;
	//defaultStatus.defaultDepthBuffer = defaultDepthBuffer;

	//pass->onBeginPass(&defaultStatus, drawElementListSet);

	// DrawElement 描画
	//int currentBatchIndex = -1;
	//DrawElementBatch* currentState = nullptr;
	//Shader* currentShader = nullptr;
	RenderStage* currentStage = nullptr;
	for (RenderDrawElement* element : m_renderingElementList)
	{
		bool visible = true;
		//drawArgs.oenerList = element->m_ownerDrawElementList;

		//DrawElementBatch* batch = element->m_ownerDrawElementList->getBatch(element->batchIndex);
		RenderStage* stage = element->stage();//element->m_ownerDrawElementList->getRenderStage(element->batchIndex);

		// ステートの変わり目チェック
		//if (element->batchIndex != currentBatchIndex)
		if (currentStage == nullptr || !currentStage->equals(stage) /*currentState->getHashCode() != batch->getHashCode()*/)
		{
			if (currentStage) {
				currentStage->flush();
			}
			//context->flush();
			//currentBatchIndex = element->batchIndex;
			currentStage = stage;
			applyFrameBufferStatus(context, currentStage);
			//context->applyStatus(currentStage, defaultStatus);
			//context->switchActiveRenderer(currentState->getRenderFeature());
			//if (diag != nullptr) diag->changeRenderStage();
		}

		//context->switchActiveRenderer(element->renderFeature);

		// 固定の内部シェーダを使わない場合はいろいろ設定する
		//if (currentStage->getRenderFeature() == nullptr ||	// TODO: だめ。でもいまやらかしてる人がいるので、後で ASSERT 張って対応する
		//	!currentStage->getRenderFeature()->isStandaloneShader())
		//if (context->getCurrentRenderFeature() == nullptr ||	// TODO: だめ。でもいまやらかしてる人がいるので、後で ASSERT 張って対応する
		//	!context->getCurrentRenderFeature()->isStandaloneShader())
		//{
			if (visible)
			{
				ElementInfo elementInfo;
				elementInfo.viewProjMatrix = &cameraInfo.viewProjMatrix;
				elementInfo.WorldMatrix = element->combinedWorldMatrix();
				elementInfo.WorldViewProjectionMatrix = elementInfo.WorldMatrix * (*elementInfo.viewProjMatrix);
				//elementInfo.boneTexture = m_skinningMatricesTexture;
				//elementInfo.boneLocalQuaternionTexture = m_skinningLocalQuaternionsTexture;




				//for (int i = 0; i < element->subsetCount; i++)
				//{
					//CombinedMaterial* material = currentState->getCombinedMaterial();
					//RenderingPass2::RenderStageFinalData stageData;
					//stageData.stage = currentStage;
					//stageData.material = currentStage->getMaterialFinal(m_defaultMaterial, element->getPriorityMaterial(i));
					//stageData.shader = currentStage->getShaderFinal(stageData.material);
					//stageData.shadingModel = currentStage->getShadingModelFinal(stageData.material);

					AbstractMaterial* finalMaterial = currentStage->getMaterialFinal(m_defaultMaterial, nullptr);


					applyGeometryStatus(context, currentStage, nullptr);



					//ElementRenderingPolicy policy;
					//pass->selectElementRenderingPolicy(element, stageData, &policy);
					//visible = policy.visible;

					ShaderTechnique* tech =  pass->selectShaderTechnique();

					Shader* shader = policy.shader;


					SubsetInfo subsetInfo;
					subsetInfo.renderStage = currentStage;
					subsetInfo.finalMaterial = stageData.material;
					subsetInfo.materialTexture = (subsetInfo.finalMaterial != nullptr) ? subsetInfo.finalMaterial->getMaterialTexture(nullptr) : nullptr;
					element->makeSubsetInfo(element->m_ownerDrawElementList, currentStage, &subsetInfo);

					//currentState->IsStandaloneShaderRenderer

					if (context->getCurrentRenderFeature() != nullptr)
					{
						context->getCurrentRenderFeature()->onShaderSubsetInfoOverride(&subsetInfo);
					}

					shader->getSemanticsManager()->updateCameraVariables(cameraInfo);
					shader->getSemanticsManager()->updateElementVariables(cameraInfo, elementInfo);
					shader->getSemanticsManager()->updateSubsetVariables(subsetInfo);

					//material->applyUserShaderValeues(shader);
					stageData.material->applyUserShaderValeues(shader);


					auto* stateManager = context->getRenderStateManager();

					const List<ShaderPass*>& passes = policy.shaderTechnique->getPasses();
					for (ShaderPass* pass : passes)
					{
						onShaderPassChainging(pass);
						stateManager->setShaderPass(pass);

						if (diag != nullptr) element->reportDiag(diag);
						element->drawSubset(drawArgs, i);
					}
				//}
			}
		}
		//else
		//{
		//	// 描画実行
		//	if (visible)
		//	{
		//		if (diag != nullptr) element->reportDiag(diag);
		//		for (int i = 0; i < element->subsetCount; i++)
		//		{
		//			element->drawSubset(drawArgs, i);
		//		}
		//	}
		//}

	}

	//context->flush();
#endif
}

void SceneRenderer::collect(/*SceneRendererPass* pass, */const detail::CameraInfo& cameraInfo)
{
	//InternalContext* context = m_manager->getInternalContext();
	//const detail::CameraInfo& cameraInfo = m_renderingRenderView->m_cameraInfo;


	for (auto& elementListManager : m_renderingRenderView->elementListManagers())
	{
		// TODO: とりあえず Default
		

		for (auto& elementList : elementListManager->lists(RendringPhase::Default))
		{
			//elementList->setDefaultRenderTarget(m_renderingDefaultRenderTarget);
			//elementList->setDefaultDepthBuffer(m_renderingDefaultDepthBuffer);

			//for (DynamicLightInfo* light : elementList->getDynamicLightList())
			//{
			//	onCollectLight(light);
			//}

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
			//if (lhs->m_stateFence == rhs->m_stateFence)
			//{
			//	if (lhs->metadata.priority == rhs->metadata.priority)
					return lhs->zDistance > rhs->zDistance;
			//	return lhs->metadata.priority < rhs->metadata.priority;
			//}
			//else
			//{
			//	return lhs->m_stateFence < rhs->m_stateFence;
			//}
		});
}

void SceneRenderer::applyFrameBufferStatus(GraphicsContext* context, RenderStage* stage, const FrameBuffer& defaultFrameBufferInPass)
{
	RenderTargetTexture* renderTarget0 = nullptr;

	// RenderTarget
	{
		for (int i = 0; i < MaxMultiRenderTargets; i++)
		{
			RenderTargetTexture* target = stage->getRenderTargetFinal(i);
			if (!target) {
				target = defaultFrameBufferInPass.renderTarget[i];
			}

			context->setColorBuffer(i, target);

			if (i == 0) {
				renderTarget0 = target;
			}
		}
	}

	// DepthBuffer
	{
		DepthBuffer* depthBuffer = stage->getDepthBufferFinal();
		if (depthBuffer) {
			context->setDepthBuffer(depthBuffer);
		}
		else {
			context->setDepthBuffer(defaultFrameBufferInPass.depthBuffer);
		}
	}

	// Viewport
	{
		//const RectI& rect = stage->getViewportRectFinal();//getViewportRect();
		//if (rect.width < 0)
		//{
		//	stateManager->setViewport(RectI(0, 0, renderTarget0->getSize()));
		//}
		//else
		//{
		//	stateManager->setViewport(rect);
		//}
		// TODO:Viewport
		// TODO: m_scissorRect
	}
}

void SceneRenderer::applyGeometryStatus(GraphicsContext* context, RenderStage* stage, AbstractMaterial* priorityMaterial)
{
	// BlendState
	{
		BlendStateDesc state;
		state.independentBlendEnable = false;
		makeBlendMode(stage->getBlendModeFinal(priorityMaterial), &state.renderTargets[0]);
		context->setBlendState(state);
	}
	// RasterizerState
	{
		RasterizerStateDesc state;
		state.fillMode = FillMode::Solid;
		state.cullMode = stage->getCullingModeFinal(priorityMaterial);
		context->setRasterizerState(state);
	}
	// DepthStencilState
	{
		DepthStencilStateDesc state;
		state.depthTestEnabled = stage->isDepthTestEnabledFinal(priorityMaterial);
		state.depthWriteEnabled = stage->isDepthWriteEnabledFinal(priorityMaterial);
		context->setDepthStencilState(state);
	}
}

void SceneRenderer::makeBlendMode(BlendMode mode, RenderTargetBlendDesc* state)
{
	// もっといろいろ http://d.hatena.ne.jp/Ko-Ta/20070618/p1
	// TODO: アルファも一緒のブレンド方式にしているので、個別指定で改善できそう
	switch (mode)
	{
	case BlendMode::Normal:
		state->blendEnable = false;
		state->sourceBlend = BlendFactor::One;
		state->destinationBlend = BlendFactor::Zero;
		state->blendOp = BlendOp::Add;
		state->sourceBlendAlpha = BlendFactor::One;
		state->destinationBlendAlpha = BlendFactor::Zero;
		state->blendOpAlpha = BlendOp::Add;
		break;
	case BlendMode::Alpha:
		state->blendEnable = true;
		state->sourceBlend = BlendFactor::SourceAlpha;
		state->destinationBlend = BlendFactor::InverseSourceAlpha;
		state->blendOp = BlendOp::Add;
		state->sourceBlendAlpha = BlendFactor::SourceAlpha;
		state->destinationBlendAlpha = BlendFactor::InverseSourceAlpha;
		state->blendOpAlpha = BlendOp::Add;
		break;
	case BlendMode::Add:
		state->blendEnable = true;
		state->sourceBlend = BlendFactor::SourceAlpha;
		state->destinationBlend = BlendFactor::One;
		state->blendOp = BlendOp::Add;
		state->sourceBlendAlpha = BlendFactor::SourceAlpha;
		state->destinationBlendAlpha = BlendFactor::One;
		state->blendOpAlpha = BlendOp::Add;
		break;
	case BlendMode::Subtract:
		state->blendEnable = true;
		state->sourceBlend = BlendFactor::SourceAlpha;
		state->destinationBlend = BlendFactor::One;
		state->blendOp = BlendOp::ReverseSubtract;
		state->sourceBlendAlpha = BlendFactor::SourceAlpha;
		state->destinationBlendAlpha = BlendFactor::One;
		state->blendOpAlpha = BlendOp::Add;
		break;
	case BlendMode::Multiply:
		state->blendEnable = true;
		state->sourceBlend = BlendFactor::Zero;	// AlphaDisable (Alpha を別指定できない今の仕様では Alpha を考慮できない)
		state->destinationBlend = BlendFactor::SourceColor;
		state->blendOp = BlendOp::Add;
		state->sourceBlendAlpha = BlendFactor::SourceAlpha;
		state->destinationBlendAlpha = BlendFactor::One;
		state->blendOpAlpha = BlendOp::Add;
		break;
	//case BlendMode_Screen:
	//	m_dxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//	m_dxDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//	m_dxDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHASAT);
	//	m_dxDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
	//	m_dxDevice->SetRenderState(D3DRS_ALPHAREF, 255);
	//	break;
	//case BlendMode_Reverse:
	//	m_dxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//	m_dxDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//	m_dxDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//	m_dxDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCCOLOR);
	//	m_dxDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	//	break;
	default:
		assert(0);
		break;
	}
}

} // namespace detail
} // namespace ln

