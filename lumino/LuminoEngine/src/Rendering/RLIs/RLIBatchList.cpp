
#include "Internal.hpp"
#include "../RenderingManager.hpp"
#include "RLIBatch.hpp"
#include "RLIBatchList.hpp"

namespace ln {
namespace detail {
	
//==============================================================================
// RenderFeatureBatchList

RenderFeatureBatchList::RenderFeatureBatchList(detail::RenderingManager* manager)
	: m_manager(manager)
	, m_allocator(makeRef<detail::LinearAllocator>(manager->stageDataPageManager()))
	, m_currentStage(nullptr)
	, m_head(nullptr)
	, m_tail(nullptr)
{
}

void RenderFeatureBatchList::clear()
{
	m_allocator->cleanup();
	m_currentStage = nullptr;
	m_head = nullptr;
	m_tail = nullptr;
}

void RenderFeatureBatchList::setCurrentStage(detail::RenderStage* stage)
{
	m_currentStage = stage;
}
//
//void RenderFeatureBatchList::addClearBatch(ClearFlags flags, const Color& color, float depth, uint8_t stencil)
//{
//
//	addNewBatch<ClearBatch>(nullptr);
//}

void RenderFeatureBatchList::add(RenderFeatureBatch* batch, RenderFeature* owner, const RLIBatchState& state)
{
	if (LN_REQUIRE(batch)) return;
	if (LN_REQUIRE(m_currentStage)) return;

	batch->m_owner = owner;
	batch->m_stage = m_currentStage;
	batch->m_material = state;

	if (!m_head) {
		m_head = m_tail = batch;
	}
	else {
		m_tail->m_next = batch;
		m_tail = batch;
	}
}

//bool RenderFeatureBatchList::lastBatchRenderFeatureIs(RenderFeature* renderFeature) const
//{
//	if (m_tail) {
//		return m_tail->renderFeature() == renderFeature;
//	}
//	else {
//		return false;
//	}
//}

bool RenderFeatureBatchList::equalsLastBatchState(RenderFeature* renderFeature, const RLIBatchState& state)
{
	if (!m_tail) return false;
	if (m_tail->renderFeature() != renderFeature) return false;
	if (!m_tail->finalMaterial()->equals(&state)) return false;
	return true;
}

//void RenderFeatureBatchList::prepareState(const CameraInfo& cameraInfo, RenderStage* stage, RenderDrawElement* element)
//{
//	Material* finalMaterial = stage->getMaterialFinal(nullptr, m_manager->builtinMaterials(BuiltinMaterial::Default));
//	Texture* mainTexture = nullptr;
//	if (finalMaterial) {
//		mainTexture = finalMaterial->mainTexture();
//	}
//	if (!mainTexture) {
//		mainTexture = m_manager->graphicsManager()->whiteTexture();
//	}
//
//	ElementInfo elementInfo;
//	elementInfo.viewProjMatrix = &cameraInfo.viewProjMatrix;
//	elementInfo.WorldMatrix = (stage->renderFeature && !stage->renderFeature->drawElementTransformNegate()) ? element->combinedWorldMatrix() : Matrix::Identity;
//	elementInfo.WorldViewProjectionMatrix = elementInfo.WorldMatrix * (*elementInfo.viewProjMatrix);
//	elementInfo.boneTexture = nullptr;	// TODO:
//	elementInfo.boneLocalQuaternionTexture = nullptr;	// TODO:
//
//	SubsetInfo subsetInfo;
//	subsetInfo.materialTexture = mainTexture;
//	subsetInfo.opacity = stage->getOpacityFinal(element);
//	subsetInfo.colorScale = stage->getColorScaleFinal(element);
//	subsetInfo.blendColor = stage->getBlendColorFinal(element);
//	subsetInfo.tone = stage->getToneFinal(element);
//}
//
//void RenderFeatureBatchList::render(GraphicsContext* graphicsContext, SceneRendererPass* pass, const FrameBuffer& defaultFrameBuffer, const CameraInfo& cameraInfo)
//{
//#if 1
//	const detail::RenderStage* currentStage = nullptr;
//	RenderFeatureBatch* batch = m_head;
//	while (batch)
//	{
//		const detail::RenderStage *stage = batch->stage();
//
//		// ステートの変わり目チェック
//		if (currentStage == nullptr || !currentStage->equals(stage))
//		{
//			currentStage = stage;
//			detail::RenderStage::applyFrameBufferStatus(graphicsContext, currentStage, defaultFrameBuffer);
//		}
//
//		// DrawElement drawing.
//		{
//			if (batch->type() == RenderDrawElementType::Geometry)
//			{
//				Material* finalMaterial = currentStage->getMaterialFinal(nullptr, m_manager->builtinMaterials(detail::BuiltinMaterial::Default));
//				detail::RenderStage::applyGeometryStatus(graphicsContext, currentStage, finalMaterial);
//
//				Texture* mainTexture = nullptr;
//				if (finalMaterial) {
//					mainTexture = finalMaterial->mainTexture();
//				}
//				if (!mainTexture) {
//					mainTexture = m_manager->graphicsManager()->whiteTexture();
//				}
//
//				ShaderTechniqueClass_MeshProcess meshProcess = ShaderTechniqueClass_MeshProcess::StaticMesh;
//
//				ElementInfo elementInfo;
//				elementInfo.viewProjMatrix = &cameraInfo.viewProjMatrix;
//				elementInfo.WorldMatrix = (batch->worldTransformPtr()) ? *batch->worldTransformPtr() : Matrix::Identity;
//				elementInfo.WorldViewProjectionMatrix = elementInfo.WorldMatrix * (*elementInfo.viewProjMatrix);
//				elementInfo.boneTexture = nullptr;// TODO: m_skinningMatricesTexture;
//				elementInfo.boneLocalQuaternionTexture = nullptr; // TODO:m_skinningLocalQuaternionsTexture;
//				//element->onElementInfoOverride(&elementInfo, &meshProcess);
//
//				SubsetInfo subsetInfo;
//				subsetInfo.materialTexture = mainTexture;
//				subsetInfo.opacity = currentStage->getOpacityFinal(element);
//				subsetInfo.colorScale = currentStage->getColorScaleFinal(element);
//				subsetInfo.blendColor = currentStage->getBlendColorFinal(element);
//				subsetInfo.tone = currentStage->getToneFinal(element);
//				element->onSubsetInfoOverride(&subsetInfo);
//
//				ShaderTechnique* tech = pass->selectShaderTechnique(
//					meshProcess,
//					finalMaterial->shader(),
//					currentStage->getShadingModelFinal(finalMaterial));
//
//				detail::ShaderSemanticsManager* semanticsManager = ShaderHelper::semanticsManager(tech->shader());
//				//semanticsManager->updateCameraVariables(cameraInfo);
//				//semanticsManager->updateElementVariables(cameraInfo, elementInfo);
//				//semanticsManager->updateSubsetVariables(subsetInfo);
//				if (currentStage->renderFeature) {
//					currentStage->renderFeature->updateRenderParameters(element, tech, cameraInfo, elementInfo, subsetInfo);
//				}
//				else {
//					RenderFeature::updateRenderParametersDefault(tech, cameraInfo, elementInfo, subsetInfo);
//				}
//
//				if (finalMaterial) {
//					PbrMaterialData pbrMaterialData;
//					finalMaterial->translateToPBRMaterialData(&pbrMaterialData);
//					semanticsManager->updateSubsetVariables_PBR(pbrMaterialData);
//					finalMaterial->updateShaderVariables(tech->shader());
//				}
//
//				onSetAdditionalShaderPassVariables(tech->shader());
//
//				for (ShaderPass* pass2 : tech->passes())
//				{
//					graphicsContext->setShaderPass(pass2);
//					batch->render(graphicsContext);
//				}
//			}
//			else if (batch->type() == RenderDrawElementType::Clear)
//			{
//				RenderStage::applyGeometryStatus(graphicsContext, currentStage, nullptr);
//				batch->render(graphicsContext);
//			}
//			else
//			{
//				LN_UNREACHABLE();
//			}
//		}
//
//		batch = batch->m_next;
//	}
//#endif
//}


} // namespace detail
} // namespace ln

