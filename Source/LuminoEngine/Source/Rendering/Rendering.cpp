
#include "../Internal.h"
#include <float.h>
#include <Lumino/IO/Console.h>
#include <Lumino/Graphics/Color.h>
#include <Lumino/Rendering/Rendering.h>
#include <Lumino/Rendering/SceneRenderer.h>	// TODO
#include <Lumino/Graphics/ContextInterface.h>
#include <Lumino/Mesh/Mesh.h>
#include "../Graphics/Device/GraphicsDriverInterface.h"
#include "../Graphics/GraphicsManager.h"
#include "../Graphics/CoreGraphicsRenderFeature.h"
#include "../Graphics/RenderTargetTextureCache.h"
#include "../Graphics/Text/SpriteTextRenderFeature.h"
#include "../Graphics/Text/VectorTextRenderFeature.h"
#include "../Graphics/Text/FontManager.h"
#include "../Graphics/Text/FontGlyphTextureCache.h"
#include "../Mesh/MeshFactory.h"
#include "PrimitiveRenderFeature.h"
#include "MeshRenderFeature.h"
#include "SpriteRenderFeature.h"
#include "ShapesRenderFeature.h"
#include "NanoVGRenderFeature.h"
#include "FrameRectRenderFeature.h"
#include "RenderStage.h"




LN_NAMESPACE_BEGIN

const DrawElementMetadata DrawElementMetadata::Default;

namespace detail {

//==============================================================================
// DynamicLightInfo
//==============================================================================

//------------------------------------------------------------------------------
DynamicLightInfo::DynamicLightInfo()
	: m_intensity(1)
	, m_range(10)
	, m_attenuation(1)
	, m_spotAngle(Math::PI / 3)
	, m_spotPenumbra(0)
	, m_shadowCasterPass(nullptr)
{
}

//==============================================================================
// CommandDataCache
//==============================================================================

//------------------------------------------------------------------------------
CommandDataCache::CommandDataCache()
	: m_dataList()
	, m_dataBuffer()
	, m_dataBufferUsed(0)
{
}

//------------------------------------------------------------------------------
CommandDataCache::~CommandDataCache()
{
}

//------------------------------------------------------------------------------
void CommandDataCache::reserve(size_t dataCount, size_t byteCount)
{
	clear();
	m_dataList.reserve(dataCount);
	m_dataBuffer.resize(byteCount, false);
}

//------------------------------------------------------------------------------
void CommandDataCache::clear()
{
	m_dataList.clear();
	m_dataBufferUsed = 0;
}

//------------------------------------------------------------------------------
CommandDataCache::DataHandle CommandDataCache::allocData(size_t byteCount, const void* data)
{
	// バッファが足りなければ拡張する
	if (m_dataBufferUsed + byteCount > m_dataBuffer.getSize())
	{
		size_t newSize = m_dataBuffer.getSize() + std::max(m_dataBuffer.getSize(), byteCount);	// 最低でも byteCount 分を拡張する
		m_dataBuffer.resize(newSize, false);
	}

	if (data != nullptr)
	{
		byte_t* ptr = &(m_dataBuffer.getData()[m_dataBufferUsed]);
		size_t size = m_dataBuffer.getSize() - m_dataBufferUsed;
		memcpy_s(ptr, size, data, byteCount);
	}

	size_t dataIdx = m_dataBufferUsed;
	m_dataList.add(dataIdx);
	m_dataBufferUsed += byteCount;
	return dataIdx;
}

//------------------------------------------------------------------------------
byte_t* CommandDataCache::getData(DataHandle handle)
{
	return &(m_dataBuffer.getData()[handle]);
}


//==============================================================================
// InternalContext
//==============================================================================

//------------------------------------------------------------------------------
InternalContext::InternalContext()
	: m_baseRenderer(nullptr)
	, m_primitiveRenderer(nullptr)
	, m_meshRenderer(nullptr)
	, m_spriteRenderer(nullptr)
	, m_nanoVGRenderer(nullptr)
	, m_current(nullptr)
	, m_currentStatePtr(nullptr)
{
}

//------------------------------------------------------------------------------
void InternalContext::initialize(detail::GraphicsManager* manager)
{
	m_baseRenderer = manager->getRenderer();

	m_primitiveRenderer = Ref<PrimitiveRenderFeature>::makeRef();
	m_primitiveRenderer->initialize(manager);

	m_blitRenderer = Ref<BlitRenderer>::makeRef();
	m_blitRenderer->initialize(manager);

	m_meshRenderer = Ref<MeshRenderFeature>::makeRef();
	m_meshRenderer->initialize(manager);

	m_spriteRenderer = Ref<SpriteRenderFeature>::makeRef(manager, 2048);	// TODO

	m_textRenderer = Ref<TextRenderer>::makeRef();
	m_textRenderer->initialize(manager);

	m_vectorTextRenderer = Ref<VectorTextRenderer>::makeRef();
	m_vectorTextRenderer->initialize(manager);

	m_shapesRenderer = Ref<ShapesRenderFeature>::makeRef();
	m_shapesRenderer->initialize(manager);

	m_nanoVGRenderer = Ref<NanoVGRenderFeature>::makeRef();
	m_nanoVGRenderer->initialize(manager);

	m_frameRectRenderer = Ref<FrameRectRenderFeature>::makeRef();
	m_frameRectRenderer->initialize(manager);
}

//------------------------------------------------------------------------------
detail::CoreGraphicsRenderFeature* InternalContext::getRenderStateManager()
{
	return m_baseRenderer;
}

//------------------------------------------------------------------------------
detail::CoreGraphicsRenderFeature* InternalContext::beginBaseRenderer()
{
	//switchActiveRenderer(m_baseRenderer);
	return m_baseRenderer;
}

//------------------------------------------------------------------------------
PrimitiveRenderFeature* InternalContext::beginPrimitiveRenderer()
{
	//switchActiveRenderer(m_primitiveRenderer);
	return m_primitiveRenderer;
}

//------------------------------------------------------------------------------
BlitRenderer* InternalContext::beginBlitRenderer()
{
	//switchActiveRenderer(m_blitRenderer);
	return m_blitRenderer;
}

//------------------------------------------------------------------------------
MeshRenderFeature* InternalContext::beginMeshRenderer()
{
	//switchActiveRenderer(m_meshRenderer);
	return m_meshRenderer;
}

//------------------------------------------------------------------------------
SpriteRenderFeature* InternalContext::beginSpriteRenderer()
{
	//switchActiveRenderer(m_spriteRenderer);
	return m_spriteRenderer;
}

//------------------------------------------------------------------------------
TextRenderer* InternalContext::beginTextRenderer()
{
	//switchActiveRenderer(m_textRenderer);
	return m_textRenderer;
}

//------------------------------------------------------------------------------
VectorTextRenderer* InternalContext::beginVectorTextRenderer()
{
	//switchActiveRenderer(m_vectorTextRenderer);
	return m_vectorTextRenderer;
}

//------------------------------------------------------------------------------
ShapesRenderFeature* InternalContext::beginShapesRenderer()
{
	//switchActiveRenderer(m_shapesRenderer);
	return m_shapesRenderer;
}

//------------------------------------------------------------------------------
NanoVGRenderFeature* InternalContext::beginNanoVGRenderer()
{
	//switchActiveRenderer(m_nanoVGRenderer);
	return m_nanoVGRenderer;
}

//------------------------------------------------------------------------------
FrameRectRenderFeature* InternalContext::beginFrameRectRenderer()
{
	//switchActiveRenderer(m_frameRectRenderer);
	return m_frameRectRenderer;
}

//------------------------------------------------------------------------------
void InternalContext::setViewInfo(const Size& viewPixelSize, const Matrix& viewMatrix, const Matrix& projMatrix)
{
	m_spriteRenderer->setViewInfo(viewPixelSize, viewMatrix, projMatrix);
	m_frameRectRenderer->setViewInfo(viewMatrix * projMatrix);
}

//------------------------------------------------------------------------------
SpriteRenderFeature* InternalContext::getSpriteRenderer()
{
	return m_spriteRenderer;
}

//------------------------------------------------------------------------------
void InternalContext::applyStatus(RenderStage* state, const DefaultStatus& defaultStatus)
{
	m_currentStatePtr = state;
	//m_currentStatePtr->applyStatus(this, defaultStatus);
	applyStatusInternal(state, defaultStatus);

	if (m_current != nullptr)
	{
		m_current->onSetState(m_currentStatePtr);
	}
}

//------------------------------------------------------------------------------
void InternalContext::flush()
{
	if (m_current != nullptr) m_current->flush();
}

//------------------------------------------------------------------------------
void InternalContext::switchActiveRenderer(detail::IRenderFeature* renderer)
{
	if (m_current != renderer)
	{
		if (m_current != nullptr)
		{
			m_current->onDeactivated();
		}

		m_current = renderer;

		if (m_current != nullptr)
		{
			m_current->onActivated();
			m_current->onSetState(m_currentStatePtr);
		}
	}
}

void InternalContext::applyStatusInternal(RenderStage* stage, const DefaultStatus& defaultStatus)
{
	auto* stateManager = getRenderStateManager();

	// RenderState
	{
		//BlendMode blendMode = (combinedMaterial->m_blendMode.isSet()) ? combinedMaterial->m_blendMode.get() : m_blendMode;
		//CullingMode cullingMode = (combinedMaterial->m_cullingMode.isSet()) ? combinedMaterial->m_cullingMode.get() : m_cullingMode;



		// TODO: Base
		RenderState state;
		ContextInterface::makeBlendMode(stage->getBlendModeFinal(), &state);
		state.Culling = stage->getCullingModeFinal();
		//state.AlphaTest = combinedMaterial->m_alphaTest;
		stateManager->setRenderState(state);

		// スプライトバッチ化のため (TODO: いらないかも。SpriteRenderer では State でそーとしなくなった)
		getSpriteRenderer()->setState(state);
	}
	// DepthStencilState
	{
		DepthStencilState state;
		state.DepthTestEnabled = stage->isDepthTestEnabledFinal();//(combinedMaterial->m_depthTestEnabled.isSet()) ? combinedMaterial->m_depthTestEnabled.get() : m_depthTestEnabled;
		state.DepthWriteEnabled = stage->isDepthWriteEnabledFinal();// (combinedMaterial->m_depthWriteEnabled.isSet()) ? combinedMaterial->m_depthWriteEnabled.get() : m_depthWriteEnabled;
		stateManager->setDepthStencilState(state);
	}
	// FrameBuffer
	RenderTargetTexture* renderTarget0 = nullptr;
	{
		//if (defaultRenderTarget == nullptr && defaultDepthBuffer == nullptr)
		//{
		//	// TODO: 子DrawListが実行された後のステート復帰。
		//	// もしかしたらオプションにはできないかもしれない。
		//	return;
		//}

		for (int i = 0; i < Graphics::MaxMultiRenderTargets; ++i)
		{
			//RenderTargetTexture* target = m_renderTargets[i];
			//if (!target) target = defaultStatus.defaultRenderTarget[i];
			//if (i == 0 && m_renderTargets[i] == nullptr)
			//{
			//	target = defaultStatus.defaultRenderTarget;
			//}
			//else
			//{
			//	target = m_renderTargets[i];
			//}
			//stateManager->setRenderTarget(i, target);
			RenderTargetTexture* target = stage->getRenderTargetFinal(i);
			if (!target) target = defaultStatus.defaultRenderTarget[i];

			stateManager->setRenderTarget(i, target);
			if (i == 0)
			{
				renderTarget0 = target;
			}
		}

		stateManager->setDepthBuffer(stage->getDepthBufferFinal());
		//if (m_depthBuffer == nullptr)
		//{
		//	stateManager->setDepthBuffer(defaultStatus.defaultDepthBuffer);
		//}
		//else
		//{
		//	stateManager->setDepthBuffer(m_depthBuffer);
		//}
	}
	// Viewport
	{
		const RectI& rect = stage->getViewportRectFinal();//getViewportRect();
		if (rect.width < 0)
		{
			stateManager->setViewport(RectI(0, 0, renderTarget0->getSize()));
		}
		else
		{
			stateManager->setViewport(rect);
		}
		// TODO: m_scissorRect
	}
}


//==============================================================================
// DrawElementBatch
//==============================================================================

//------------------------------------------------------------------------------
BatchState::BatchState()
{
	reset();
}

//------------------------------------------------------------------------------
//void BatchState::setBlendMode(BlendMode mode)
//{
//	if (m_blendMode != mode)
//	{
//		m_blendMode = mode;
//		m_hashDirty = true;
//	}
//}

//------------------------------------------------------------------------------
void BatchState::setRenderTarget(int index, RenderTargetTexture* renderTarget)
{
	if (m_renderTargets[index] != renderTarget)
	{
		m_renderTargets[index] = renderTarget;
		m_hashDirty = true;
	}
}

//------------------------------------------------------------------------------
void BatchState::setDepthBuffer(DepthBuffer* depthBuffer)
{
	if (m_depthBuffer != depthBuffer)
	{
		m_depthBuffer = depthBuffer;
		m_hashDirty = true;
	}
}

void BatchState::setViewportRect(const RectI& rect)
{
	if (m_viewportRect != rect)
	{
		m_viewportRect = rect;
		m_hashDirty = true;
	}
}

//------------------------------------------------------------------------------
void BatchState::setScissorRect(const RectI& scissorRect)
{
	if (m_scissorRect != scissorRect)
	{
		m_scissorRect = scissorRect;
		m_hashDirty = true;
	}
}

//------------------------------------------------------------------------------
void BatchState::setBlendMode(BlendMode mode)
{
	if (m_blendMode != mode)
	{
		m_blendMode = mode;
		m_hashDirty = true;
	}
}

//------------------------------------------------------------------------------
void BatchState::setCullingMode(CullingMode mode)
{
	if (m_cullingMode != mode)
	{
		m_cullingMode = mode;
		m_hashDirty = true;
	}
}

//------------------------------------------------------------------------------
void BatchState::setDepthTestEnabled(bool enabled)
{
	if (m_depthTestEnabled != enabled)
	{
		m_depthTestEnabled = enabled;
		m_hashDirty = true;
	}
}

//------------------------------------------------------------------------------
void BatchState::setDepthWriteEnabled(bool enabled)
{
	if (m_depthWriteEnabled != enabled)
	{
		m_depthWriteEnabled = enabled;
		m_hashDirty = true;
	}
}

//------------------------------------------------------------------------------
void BatchState::setBrush(Brush* brush)
{
	if (m_brush != brush)
	{
		m_brush = brush;
		m_hashDirty = true;
	}
}

//------------------------------------------------------------------------------
Brush* BatchState::getBrush() const
{
	return m_brush;
}

//------------------------------------------------------------------------------
void BatchState::setFont(Font* font)
{
	if (m_font != font)
	{
		m_font = font;
		m_hashDirty = true;
	}
}

//------------------------------------------------------------------------------
Font* BatchState::getFont() const
{
	return m_font;
}

//------------------------------------------------------------------------------
void BatchState::reset()
{
	for (int i = 0; i < Graphics::MaxMultiRenderTargets; ++i)
		m_renderTargets[i] = nullptr;
	m_depthBuffer = nullptr;

	m_blendMode = BlendMode::Normal;
	m_cullingMode = CullingMode::Back;
	m_depthTestEnabled = true;
	m_depthWriteEnabled = true;
	m_viewportRect.set(0, 0, -1, -1);

	m_brush = nullptr;
	m_font = nullptr;

	m_hashCode = 0;
	m_hashDirty = true;
}

//------------------------------------------------------------------------------
//void BatchState::applyStatus(InternalContext* context, CombinedMaterial* combinedMaterial, const DefaultStatus& defaultStatus)
//{
//	auto* stateManager = context->getRenderStateManager();
//
//	// RenderState
//	{
//		BlendMode blendMode = (combinedMaterial->m_blendMode.isSet()) ? combinedMaterial->m_blendMode.get() : m_blendMode;
//		CullingMode cullingMode = (combinedMaterial->m_cullingMode.isSet()) ? combinedMaterial->m_cullingMode.get() : m_cullingMode;
//
//
//
//		// TODO: Base
//		RenderState state;
//		ContextInterface::makeBlendMode(blendMode, &state);
//		state.Culling = cullingMode;
//		//state.AlphaTest = combinedMaterial->m_alphaTest;
//		stateManager->setRenderState(state);
//
//		// スプライトバッチ化のため (TODO: いらないかも。SpriteRenderer では State でそーとしなくなった)
//		context->getSpriteRenderer()->setState(state);
//	}
//	// DepthStencilState
//	{
//		DepthStencilState state;
//		state.DepthTestEnabled = (combinedMaterial->m_depthTestEnabled.isSet()) ? combinedMaterial->m_depthTestEnabled.get() : m_depthTestEnabled;
//		state.DepthWriteEnabled = (combinedMaterial->m_depthWriteEnabled.isSet()) ? combinedMaterial->m_depthWriteEnabled.get() : m_depthWriteEnabled;
//		stateManager->setDepthStencilState(state);
//	}
//	// FrameBuffer
//	RenderTargetTexture* renderTarget0 = nullptr;
//	{
//		//if (defaultRenderTarget == nullptr && defaultDepthBuffer == nullptr)
//		//{
//		//	// TODO: 子DrawListが実行された後のステート復帰。
//		//	// もしかしたらオプションにはできないかもしれない。
//		//	return;
//		//}
//
//		for (int i = 0; i < Graphics::MaxMultiRenderTargets; ++i)
//		{
//			RenderTargetTexture* target = m_renderTargets[i];
//			if (!target) target = defaultStatus.defaultRenderTarget[i];
//			//if (i == 0 && m_renderTargets[i] == nullptr)
//			//{
//			//	target = defaultStatus.defaultRenderTarget;
//			//}
//			//else
//			//{
//			//	target = m_renderTargets[i];
//			//}
//			stateManager->setRenderTarget(i, target);
//
//			if (i == 0)
//			{
//				renderTarget0 = target;
//			}
//		}
//
//		if (m_depthBuffer == nullptr)
//		{
//			stateManager->setDepthBuffer(defaultStatus.defaultDepthBuffer);
//		}
//		else
//		{
//			stateManager->setDepthBuffer(m_depthBuffer);
//		}
//	}
//	// Viewport
//	{
//		const RectI& rect = getViewportRect();
//		if (rect.width < 0)
//		{
//			stateManager->setViewport(RectI(0, 0, renderTarget0->getSize()));
//		}
//		else
//		{
//			stateManager->setViewport(rect);
//		}
//		// TODO: m_scissorRect
//	}
//}

//------------------------------------------------------------------------------
uint32_t BatchState::getHashCode() const
{
	if (m_hashDirty)
	{
		m_hashCode = 0;
		m_hashDirty = false;
		m_hashCode = Hash::calcHash(reinterpret_cast<const char*>(this), sizeof(BatchState));
	}
	return m_hashCode;
}


//==============================================================================
// DrawElementBatch
//==============================================================================

//------------------------------------------------------------------------------
DrawElementBatch::DrawElementBatch()
{
	reset();
}

//------------------------------------------------------------------------------
void DrawElementBatch::setTransfrom(const Matrix& value)
{
	if (m_transfrom != value)
	{
		m_transfrom = value;
		m_hashDirty = true;
	}
}

//------------------------------------------------------------------------------
void DrawElementBatch::setCombinedMaterial(CombinedMaterial* value)
{
	if (m_combinedMaterial != value)
	{
		m_combinedMaterial = value;
		m_hashDirty = true;
	}
}

//------------------------------------------------------------------------------
void DrawElementBatch::SetBuiltinEffect(const BuiltinEffectData& data)
{
	if (!m_builtinEffectData.equals(data))
	{
		m_builtinEffectData = data;
		m_hashDirty = true;
	}
}
//
////------------------------------------------------------------------------------
//void DrawElementBatch::SetStandaloneShaderRenderer(bool enabled)
//{
//	if (m_standaloneShaderRenderer != enabled)
//	{
//		m_standaloneShaderRenderer = enabled;
//		m_hashDirty = true;
//	}
//}
//
////------------------------------------------------------------------------------
//bool DrawElementBatch::IsStandaloneShaderRenderer() const
//{
//	return m_standaloneShaderRenderer;
//}

void DrawElementBatch::setRenderFeature(IRenderFeature* renderFeature)
{
	if (m_renderFeature != renderFeature)
	{
		m_renderFeature = renderFeature;
		m_hashDirty = true;
	}
}

//------------------------------------------------------------------------------
bool DrawElementBatch::Equal(const DrawElementBatch& state_, CommonMaterial* material, const BuiltinEffectData& effectData) const
{
	assert(m_combinedMaterial != nullptr);
	return
		state.getHashCode() == state_.state.getHashCode() &&
		m_combinedMaterial->getSourceHashCode() == material->getHashCode() &&
		m_transfrom == state_.m_transfrom &&
		m_builtinEffectData.getHashCode() == effectData.getHashCode();
//#if 
//	return GetHashCode() == obj.GetHashCode();
//#else
//	if (m_rendererId != obj.m_rendererId) return false;
//
//	if (m_alphaBlendEnabled != obj.m_alphaBlendEnabled) return false;
//	if (m_blendMode != obj.m_blendMode) return false;
//	if (m_cullingMode != obj.m_cullingMode) return false;
//	if (m_alphaTestEnabled != obj.m_alphaTestEnabled) return false;
//
//	if (m_depthTestEnabled != obj.m_depthTestEnabled) return false;
//	if (m_depthWriteEnabled != obj.m_depthWriteEnabled) return false;
//
//	if (m_shaderPass != obj.m_shaderPass) return false;
//	if (m_shaderValueList.GetCount() != obj.m_shaderValueList.GetCount()) return false;
//	for (int i = 0; i < m_shaderValueList.GetCount(); ++i)
//	{
//		if (m_shaderValueList[i].variable != obj.m_shaderValueList[i].variable) return false;
//		if (!m_shaderValueList[i].value.Equals(obj.m_shaderValueList[i].value)) return false;
//	}
//
//	for (int i = 0; i < Graphics::MaxMultiRenderTargets; ++i)
//	{
//		if (m_renderTargets[i] != obj.m_renderTargets[i]) return false;
//	}
//	if (m_depthBuffer != obj.m_depthBuffer) return false;
//	if (m_scissorRect != obj.m_scissorRect) return false;
//
//	return true;
//#endif
}

//------------------------------------------------------------------------------
void DrawElementBatch::reset()
{
	state.reset();

	m_transfrom = Matrix::Identity;
	m_combinedMaterial = nullptr;
	//m_standaloneShaderRenderer = false;
	m_renderFeature = nullptr;

	m_hashCode = 0;
	m_hashDirty = true;
}

//------------------------------------------------------------------------------
//void DrawElementBatch::applyStatus(InternalContext* context, const DefaultStatus& defaultStatus)
//{
//	state.applyStatus(context, m_combinedMaterial, defaultStatus);
//}

//------------------------------------------------------------------------------
size_t DrawElementBatch::getHashCode() const
{
	if (m_hashDirty || state.isHashDirty())
	{
		size_t hs = state.getHashCode();
		m_hashCode = 0;
		m_hashDirty = false;
		m_hashCode = Hash::calcHash(reinterpret_cast<const char*>(this), sizeof(DrawElementBatch));
		m_hashCode += hs;
	}
	return m_hashCode;
}


//==============================================================================
// DrawElement
//==============================================================================

//------------------------------------------------------------------------------
DrawElement::DrawElement()
	: /*m_transform()
	, */batchIndex(-1)
	, subsetIndex(0)
	//, zSortDistanceBase(ZSortDistanceBase::CameraDistance)
	, zDistance(0.0f)
	, m_stateFence(0)
{
	boundingSphere.center = Vector3::Zero;
	boundingSphere.radius = -1.0f;
}

//------------------------------------------------------------------------------
DrawElement::~DrawElement()
{
}

//------------------------------------------------------------------------------
const Matrix& DrawElement::getTransform(DrawElementList* oenerList) const
{
	return oenerList->getRenderStage(batchIndex)->getTransformFinal();
}

//------------------------------------------------------------------------------
void DrawElement::makeElementInfo(DrawElementList* oenerList, const CameraInfo& cameraInfo, RenderView* renderView, ElementInfo* outInfo)
{
	auto& matrix = getTransform(oenerList);
	outInfo->viewProjMatrix = &cameraInfo.viewProjMatrix;
	outInfo->WorldMatrix = matrix;
	renderView->filterWorldMatrix(&outInfo->WorldMatrix);

	// TODO: 呼び出し元でやるようにした
	outInfo->WorldViewProjectionMatrix = outInfo->WorldMatrix * cameraInfo.viewMatrix * cameraInfo.projMatrix;	// TODO: viewProj はまとめたい

	outInfo->affectedLights = getAffectedDynamicLightInfos();
}

//------------------------------------------------------------------------------
void DrawElement::makeSubsetInfo(DrawElementList* oenerList, RenderStage* stage, SubsetInfo* outInfo)
{
}

//------------------------------------------------------------------------------
void DrawElement::makeBoundingSphere(const Vector3& minPos, const Vector3& maxPos)
{
	Vector3 center = minPos + ((maxPos - minPos) / 2);
	boundingSphere.center = center;
	boundingSphere.radius = std::max(Vector3::distance(minPos, center), Vector3::distance(maxPos, center));
}

//------------------------------------------------------------------------------
DynamicLightInfo** DrawElement::getAffectedDynamicLightInfos()
{
	return nullptr;
}

//------------------------------------------------------------------------------
//void DrawElement::OnJoindDrawList(const Matrix& transform)
//{
//	m_transform = transform;
//}


//==============================================================================
// LightingDrawElement
//==============================================================================

//------------------------------------------------------------------------------
LightingDrawElement::LightingDrawElement()
{
	memset(m_affectedDynamicLightInfos, 0, sizeof(m_affectedDynamicLightInfos));
}


//==============================================================================
// DrawElementList
//==============================================================================

//------------------------------------------------------------------------------
DrawElementList::DrawElementList()
{
	m_commandDataCache.reserve(512, 4096);	// 適当に
	m_extDataCache.reserve(512, 4096);
	m_renderStageCache = std::make_unique<detail::RenderStageCache>();
	m_renderStageCache->initialize(64);
}

//------------------------------------------------------------------------------
void DrawElementList::clearCommands()
{
	for (int i = 0; i < getElementCount(); ++i)
	{
		DrawElement* cmd = getElement(i);
		cmd->~DrawElement();
	}

	m_commandDataCache.clear();
	m_extDataCache.clear();
	//m_batchList.clear();

	//m_combinedMaterialCache.releaseAll();
	m_dynamicLightList.clear();
	m_renderStageCache->clear();
	RenderStage* first = m_renderStageCache->request();	// 1個作っておく
	first->reset();
}

bool DrawElementList::checkChangeRenderStage(const RenderStage* stage) const
{
	return !m_renderStageCache->getLast()->equals(stage);
}

int DrawElementList::submitRenderStage(const RenderStage* stage)
{
	RenderStage* newstage = m_renderStageCache->request();
	newstage->copyFrom(stage);
	newstage->combineParameters();
	return newstage->getId();
}

RenderStage* DrawElementList::getRenderStage(int id) const
{
	return m_renderStageCache->get(id);
}

//------------------------------------------------------------------------------
//void DrawElementList::postAddCommandInternal(const DrawElementBatch& state, CommonMaterial* availableMaterial, const BuiltinEffectData& effectData, bool forceStateChange, DrawElement* element)
//{
//	if (forceStateChange || m_batchList.isEmpty() ||
//		!m_batchList.getLast().Equal(state, availableMaterial, effectData))
//	{
//		// CombinedMaterial を作る
//		CombinedMaterial* cm = m_combinedMaterialCache.queryCommandList();
//		cm->combine(/*nullptr, */availableMaterial, effectData);	// TODO
//
//		// 新しく DrawElementBatch を作る
//		m_batchList.add(DrawElementBatch());
//		m_batchList.getLast().state = state.state;
//		m_batchList.getLast().setRenderFeature(state.getRenderFeature());
//		m_batchList.getLast().setCombinedMaterial(cm);
//		m_batchList.getLast().setTransfrom(state.getTransfrom());
//		m_batchList.getLast().SetBuiltinEffect(effectData);
//	}
//	element->batchIndex = m_batchList.getCount() - 1;
//}

//------------------------------------------------------------------------------
//void DrawElementList::ResolveCombinedMaterials()
//{
//	for (DrawElementBatch& state : m_batchList)
//	{
//		if (state.m_material != nullptr)
//		{
//			state.m_material->ResolveCombinedMaterial();
//		}
//	}
//}

//------------------------------------------------------------------------------
void DrawElementList::addDynamicLightInfo(DynamicLightInfo* lightInfo)
{
	assert(lightInfo != nullptr);
	m_dynamicLightList.add(lightInfo);
}





} // namespace detail 



//==============================================================================
// RenderDiag
//==============================================================================

//------------------------------------------------------------------------------
RenderDiagItem::RenderDiagItem()
	: m_name(nullptr)
	, m_subType(SubType::Command)
{
}

//------------------------------------------------------------------------------
String RenderDiagItem::toString() const
{
	return String(m_name);
}

//==============================================================================
// RenderDiag
//==============================================================================

//------------------------------------------------------------------------------
void RenderDiag::clear()
{
	for (auto& pair : m_cacheMap)
	{
		pair.second->clear();
	}
	m_items.clear();
}

//------------------------------------------------------------------------------
void RenderDiag::beginRenderView()
{
	instantiate<detail::RenderDiagItem_BeginRenderView>(_LT("BeginRenderView"))->m_subType = RenderDiagItem::SubType::ScopeBegin;
}

//------------------------------------------------------------------------------
void RenderDiag::endRenderView()
{
	instantiate<detail::RenderDiagItem_EndRenderView>(_LT("EndRenderView"))->m_subType = RenderDiagItem::SubType::ScopeEnd;
}

//------------------------------------------------------------------------------
void RenderDiag::beginDrawList()
{
	instantiate<detail::RenderDiagItem_BeginDrawList>(_LT("BeginDrawList"))->m_subType = RenderDiagItem::SubType::ScopeBegin;
}

//------------------------------------------------------------------------------
void RenderDiag::endDrawList()
{
	instantiate<detail::RenderDiagItem_EndDrawList>(_LT("EndDrawList"))->m_subType = RenderDiagItem::SubType::ScopeEnd;
}

//------------------------------------------------------------------------------
void RenderDiag::changeRenderStage()
{
	instantiate<detail::RenderDiagItem_EndDrawList>(_LT("ChangeRenderStage"));
}

//------------------------------------------------------------------------------
void RenderDiag::callCommonElement(const Char* typeName)
{
	instantiate<detail::RenderDiagItem_Common>(typeName);
}

//------------------------------------------------------------------------------
void RenderDiag::print()
{
	int level = 0;
	for (RenderDiagItem* item : m_items)
	{
		if (item->m_subType == RenderDiagItem::SubType::ScopeEnd) level--;
		for (int i = 0; i < level; i++) printf("  ");
		if (item->m_subType == RenderDiagItem::SubType::ScopeBegin) level++;

		Console::writeLine(item->toString());
	}
}

//==============================================================================
// DrawList
//==============================================================================



//------------------------------------------------------------------------------
DrawList::DrawList()
	: /*m_currentSectionTopElement(nullptr)
	,*/ m_metadata(nullptr)
	, m_currentStateFence(0)
{
}

//------------------------------------------------------------------------------
DrawList::~DrawList()
{
	m_drawElementList.clearCommands();
}

//------------------------------------------------------------------------------
void DrawList::initialize(detail::GraphicsManager* manager)
{
	if (LN_REQUIRE(manager != nullptr)) return;
	m_manager = manager;
	//m_state.reset();

	//m_defaultMaterial = newObject<CommonMaterial>();


	// とりあえず 10 個くらい
	for (int i = 0; i < 10; i++)
	{
		m_freeStateStack.add(newObject<detail::RenderStage>(-1));
	}
	//m_aliveStateStack.add(Ref<StagingState>::makeRef());


	// Lumino はメインループの外側でも RenderingContext を使えるようにしている。
	// Editor モードでは GC のタイミングをシンプルにつかむことができない。
	// (最終書き込み先 RT が異なる RenderingContext 間で SetRenderTarget() などで指定する RT を使いまわすと描画順の問題が起こる)
	// そのため Unity とはことなり、Pool は RenderingContext につけている。
	m_renderTargetPool = Ref<detail::RenderTargetTextureCache>::makeRef(manager);
}

//------------------------------------------------------------------------------
void DrawList::pushState()
{
	Ref<detail::RenderStage> state;
	if (m_freeStateStack.isEmpty())
	{
		state = newObject<detail::RenderStage>(-1);
	}
	else
	{
		state = m_freeStateStack.getLast();
		m_freeStateStack.removeLast();
	}

	if (m_aliveStateStack.isEmpty())
	{
		// ルート用
	}
	else
	{
		// 親ステートの値をコピーしてくる
		state->copyFrom(m_aliveStateStack.getLast());
	}

	m_aliveStateStack.add(state);
}

//------------------------------------------------------------------------------
void DrawList::popState()
{
	auto state = m_aliveStateStack.getLast();
	m_aliveStateStack.removeLast();
	m_freeStateStack.add(state);
}

detail::RenderStage* DrawList::getCurrentState() const
{
	return m_aliveStateStack.getLast();
}

bool DrawList::checkSubmitRenderStage(int* stageId)
{
	if (m_drawElementList.checkChangeRenderStage(getCurrentState()))
	{
		*stageId = m_drawElementList.submitRenderStage(getCurrentState());
		return true;
	}
	else
	{
		*stageId = getCurrentState()->getId();
		return false;
	}
}

//------------------------------------------------------------------------------
void DrawList::setRenderTarget(int index, RenderTargetTexture* renderTarget)
{
	getCurrentState()->renderingContextParameters.setRenderTarget(index, renderTarget);
	//getCurrentState()->m_state.state.setRenderTarget(index, renderTarget);
	//if (index == 0 && renderTarget)	// TODO: default のサイズにするべき？
	//{
	//	setViewportRect(RectI(0, 0, renderTarget->getSize()));
	//}
	m_currentStateFence++;
}

//------------------------------------------------------------------------------
RenderTargetTexture* DrawList::getRenderTarget(int index) const
{
	return getCurrentState()->renderingContextParameters.getRenderTarget(index);
}

//------------------------------------------------------------------------------
void DrawList::setDepthBuffer(DepthBuffer* depthBuffer)
{
	getCurrentState()->renderingContextParameters.setDepthBuffer(depthBuffer);
	m_currentStateFence++;
}

//------------------------------------------------------------------------------
DepthBuffer* DrawList::getDepthBuffer() const
{
	return getCurrentState()->renderingContextParameters.getDepthBuffer();
}

void DrawList::setViewportRect(const RectI& rect)
{
	getCurrentState()->renderingContextParameters.setViewportRect(rect);
}

const RectI& DrawList::getViewportRect() const
{
	return getCurrentState()->renderingContextParameters.getViewportRect();
}

//------------------------------------------------------------------------------
void DrawList::setBrush(Brush* brush)
{
	brush = (brush != nullptr) ? brush : Brush::Black;
	getCurrentState()->renderingContextParameters.setBrush(brush);
}

//------------------------------------------------------------------------------
Brush* DrawList::getBrush() const
{
	return getCurrentState()->renderingContextParameters.getBrush();
}

//------------------------------------------------------------------------------
void DrawList::setFont(Font* font)
{
	font = (font != nullptr) ? font : m_manager->getFontManager()->getDefaultFont();
	getCurrentState()->renderingContextParameters.setFont(font);
}

////------------------------------------------------------------------------------
//void DrawList::setShader(Shader* shader)
//{
//	getCurrentState()->m_defaultMaterialShader = shader;
//	m_defaultMaterial->setShader(shader);
//}

////------------------------------------------------------------------------------
//Shader* DrawList::getShader() const
//{
//	return getCurrentState()->m_defaultMaterialShader;
//}

//------------------------------------------------------------------------------
void DrawList::setBlendMode(Nullable<BlendMode> mode)
{
	getCurrentState()->renderingContextParameters.setBlendMode(mode);
}

//------------------------------------------------------------------------------
void DrawList::setCullingMode(Nullable<CullingMode> mode)
{
	getCurrentState()->renderingContextParameters.setCullingMode(mode);
}

//------------------------------------------------------------------------------
//void DrawList::setOpacity(float opacity)
//{
//	getCurrentState()->renderingContextParameters.
//}

//------------------------------------------------------------------------------
void DrawList::setDepthTestEnabled(Nullable<bool> enabled)
{
	getCurrentState()->renderingContextParameters.setDepthTestEnabled(enabled);
}

//------------------------------------------------------------------------------
void DrawList::setDepthWriteEnabled(Nullable<bool> enabled)
{
	getCurrentState()->renderingContextParameters.setDepthWriteEnabled(enabled);
}

////------------------------------------------------------------------------------
//void DrawList::setDefaultMaterial(CommonMaterial* material)
//{
//	if (LN_REQUIRE(material != nullptr)) return;
//	getCurrentState()->m_material = material;
//}

////------------------------------------------------------------------------------
//void DrawList::setBuiltinEffectData(const detail::BuiltinEffectData& data)
//{
//	getCurrentState()->m_builtinEffectData = data;
//}

void DrawList::setVisualNodeParameters(detail::VisualNodeParameters* params)
{
	getCurrentState()->visualNodeParameters = params;
}

//------------------------------------------------------------------------------
void DrawList::beginMakeElements()
{
	m_drawElementList.clearCommands();

	for (auto s : m_aliveStateStack)
	{
		m_freeStateStack.add(s);
	}
	m_aliveStateStack.clear();
	pushState();	// 1つスタックに積んでおく。コレがルートのステート
	getCurrentState()->reset();

	//m_defaultMaterial->reset();
	//getCurrentState()->m_state.reset();
	//m_state.state.state.setFont(m_manager->getFontManager()->getDefaultFont());
	//m_defaultMaterial->reset();
	//m_builtinEffectData.reset();
	//m_defaultMaterial->cullingMode = CullingMode::None;
	//m_currentSectionTopElement = nullptr;
	m_currentStateFence = 0;
	setBrush(nullptr);
	setFont(nullptr);
}

////------------------------------------------------------------------------------
//void DrawList::BeginFrame(RenderTargetTexture* defaultRenderTarget, DepthBuffer* defaultDepthBuffer)
//{
//	m_state.state.m_renderTargets[0] = defaultRenderTarget;
//	m_state.state.m_depthBuffer = defaultDepthBuffer;
//}

//------------------------------------------------------------------------------
void DrawList::setTransform(const Matrix& transform)
{
	getCurrentState()->renderingContextParameters.setTransform(transform);
}

//------------------------------------------------------------------------------
void DrawList::clear(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
	class ClearElement : public detail::DrawElement
	{
	public:
		ClearFlags flags;
		Color color;
		float z;
		uint8_t stencil;

		virtual void drawSubset(const DrawArgs& e) override
		{
			e.context->beginBaseRenderer()->clear(flags, color, z, stencil);
		}

		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement(_LT("Clear")); }
	};

	auto* ptr = resolveDrawElement<ClearElement>(m_manager->getInternalContext()->m_baseRenderer, nullptr);

	// TODO: これだけ他と独立していて変更が反映されない
	//auto* ptr = m_drawElementList.addCommand<detail::ClearElement>(getCurrentState()->m_state, m_defaultMaterial, getCurrentState()->m_builtinEffectData, false);
	ptr->flags = flags;
	ptr->color = color;
	ptr->z = z;
	ptr->stencil = stencil;
	m_currentStateFence++;
}

//------------------------------------------------------------------------------
void DrawList::drawLinePrimitive(
	const Vector3& position1, const Color& color1,
	const Vector3& position2, const Color& color2)
{
	// TODO: キャッシュと LineStrip
	class DrawElement_DrawLine : public detail::DrawElement
	{
	public:
		Vector3 position1; Color color1;
		Vector3 position2; Color color2;

		virtual void drawSubset(const DrawArgs& e) override
		{
			e.context->beginPrimitiveRenderer()->drawLine(
				position1, color1, position2, color2);
		}
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement(_LT("DrawLine")); }
	};
	auto* ptr = resolveDrawElement<DrawElement_DrawLine>(m_manager->getInternalContext()->m_primitiveRenderer, nullptr);
	ptr->position1 = position1; ptr->color1 = color1;
	ptr->position2 = position2; ptr->color2 = color2;
	ptr->makeBoundingSphere(Vector3::min(position1, position2), Vector3::max(position1, position2));
}

//------------------------------------------------------------------------------
void DrawList::drawSquarePrimitive(
	const Vector3& position1, const Vector2& uv1, const Color& color1,
	const Vector3& position2, const Vector2& uv2, const Color& color2,
	const Vector3& position3, const Vector2& uv3, const Color& color3,
	const Vector3& position4, const Vector2& uv4, const Color& color4/*,
	ShaderPass* shaderPass*/)
{
	class DrawSquarePrimitiveElement : public detail::LightingDrawElement	// TODO: LightingDrawElement は忘れやすい。デフォルトありでいいと思う
	{
	public:
		Vector3 position[4];
		Vector2 uv[4];
		Color color[4];

		virtual void drawSubset(const DrawArgs& e) override
		{
			e.context->beginPrimitiveRenderer()->drawSquare(
				position[0], uv[0], color[0],
				position[1], uv[1], color[1],
				position[2], uv[2], color[2],
				position[3], uv[3], color[3]);
		}
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement(_LT("DrawSquarePrimitive")); }
	};
	auto* e = resolveDrawElement<DrawSquarePrimitiveElement>(m_manager->getInternalContext()->m_primitiveRenderer, nullptr);
	e->position[0] = position1; e->uv[0] = uv1; e->color[0] = color1;
	e->position[1] = position2; e->uv[1] = uv2; e->color[1] = color2;
	e->position[2] = position3; e->uv[2] = uv3; e->color[2] = color3;
	e->position[3] = position4; e->uv[3] = uv4; e->color[3] = color4;
	e->makeBoundingSphere(Vector3::min(e->position, 4), Vector3::max(e->position, 4));
}

//------------------------------------------------------------------------------
void DrawList::drawSquare(float sizeX, float sizeZ, int slicesX, int slicesZ, const Color& color, const Matrix& localTransform, CommonMaterial* material)
{
	class DrawCylinderElement : public detail::LightingDrawElement	// TODO: LightingDrawElement は忘れやすい。デフォルトありでいいと思う
	{
	public:
		detail::PlaneMeshFactory3 factory;

		virtual void drawSubset(const DrawArgs& e) override
		{
			auto* r = e.context->beginPrimitiveRenderer();
			r->drawMeshFromFactory(factory, detail::PrimitiveRendererMode::TriangleList);
		}
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement(_LT("DrawCylinderElement")); }
	};
	auto* e = resolveDrawElement<DrawCylinderElement>(m_manager->getInternalContext()->m_primitiveRenderer, material);
	e->factory.initialize(Vector2(sizeX, sizeZ), slicesX, slicesZ, color, localTransform);
	e->boundingSphere.center = Vector3::Zero;
	e->boundingSphere.radius = Vector3(sizeX, sizeZ, 0).getLength();
}

//------------------------------------------------------------------------------
void DrawList::drawArc(float startAngle, float endAngle, float innerRadius, float outerRadius, int slices, const Color& color, const Matrix& localTransform, CommonMaterial* material)
{
	class DrawArcElement : public detail::LightingDrawElement	// TODO: LightingDrawElement は忘れやすい。デフォルトありでいいと思う
	{
	public:
		detail::ArcMeshFactory factory;

		virtual void drawSubset(const DrawArgs& e) override
		{
			auto* r = e.context->beginPrimitiveRenderer();
			r->drawMeshFromFactory(factory, detail::PrimitiveRendererMode::TriangleList);
		}
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement(_LT("DrawArcElement")); }
	};
	auto* e = resolveDrawElement<DrawArcElement>(m_manager->getInternalContext()->m_primitiveRenderer, material);
	e->factory.initialize(startAngle, endAngle, innerRadius, outerRadius, slices, color, localTransform);
	e->boundingSphere.center = Vector3::Zero;
	e->boundingSphere.radius = outerRadius;
}

//------------------------------------------------------------------------------
void DrawList::drawBox(const Box& box, const Color& color, const Matrix& localTransform, CommonMaterial* material)
{
	if (box.center != Vector3::Zero) LN_NOTIMPLEMENTED();

	class DrawBoxElement : public detail::LightingDrawElement	// TODO: LightingDrawElement は忘れやすい。デフォルトありでいいと思う
	{
	public:
		detail::RegularBoxMeshFactory factory;

		virtual void drawSubset(const DrawArgs& e) override
		{
			auto* r = e.context->beginPrimitiveRenderer();
			r->drawMeshFromFactory(factory, detail::PrimitiveRendererMode::TriangleList);
		}
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement(_LT("DrawBoxElement")); }
	};
	auto* e = resolveDrawElement<DrawBoxElement>(m_manager->getInternalContext()->m_primitiveRenderer, material);
	e->factory.initialize(Vector3(box.width, box.height, box.depth), color, localTransform);

	Vector3 min, max;
	box.getMinMax(&min, &max);
	e->makeBoundingSphere(min, max);
}

//------------------------------------------------------------------------------
void DrawList::drawSphere(float radius, int slices, int stacks, const Color& color, const Matrix& localTransform, CommonMaterial* material)
{
	class DrawSphereElement : public detail::LightingDrawElement	// TODO: LightingDrawElement は忘れやすい。デフォルトありでいいと思う
	{
	public:
		detail::RegularSphereMeshFactory factory;

		virtual void drawSubset(const DrawArgs& e) override
		{
			auto* r = e.context->beginPrimitiveRenderer();
			r->drawMeshFromFactory(factory, detail::PrimitiveRendererMode::TriangleList);
		}
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement(_LT("DrawSphereElement")); }
	};
	auto* e = resolveDrawElement<DrawSphereElement>(m_manager->getInternalContext()->m_primitiveRenderer, material);
	e->factory.initialize(radius, slices, stacks, color, localTransform);
	e->boundingSphere.center = Vector3::Zero;
	e->boundingSphere.radius = radius;
}

//------------------------------------------------------------------------------
void DrawList::drawCylinder(float radius, float	height, int slices, int stacks, const Color& color, const Matrix& localTransform)
{
	class DrawCylinderElement : public detail::LightingDrawElement	// TODO: LightingDrawElement は忘れやすい。デフォルトありでいいと思う
	{
	public:
		detail::RegularCylinderMeshFactory factory;

		virtual void drawSubset(const DrawArgs& e) override
		{
			auto* r = e.context->beginPrimitiveRenderer();
			r->drawMeshFromFactory(factory, detail::PrimitiveRendererMode::TriangleList);
		}
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement(_LT("DrawCylinder")); }
	};
	auto* e = resolveDrawElement<DrawCylinderElement>(m_manager->getInternalContext()->m_primitiveRenderer, nullptr);
	e->factory.initialize(radius, height, slices, stacks, color, localTransform);
	e->boundingSphere.center = Vector3::Zero;
	e->boundingSphere.radius = Vector3(radius, height, 0).getLength();
}

//------------------------------------------------------------------------------
void DrawList::drawCone(float radius, float height, int slices, const Color& color, const Matrix& localTransform)
{
	class DrawConeElement : public detail::LightingDrawElement	// TODO: LightingDrawElement は忘れやすい。デフォルトありでいいと思う
	{
	public:
		detail::RegularConeMeshFactory factory;

		virtual void drawSubset(const DrawArgs& e) override
		{
			auto* r = e.context->beginPrimitiveRenderer();
			r->drawMeshFromFactory(factory, detail::PrimitiveRendererMode::TriangleList);
		}
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement(_LT("DrawCone")); }
	};
	auto* e = resolveDrawElement<DrawConeElement>(m_manager->getInternalContext()->m_primitiveRenderer, nullptr);
	e->factory.initialize(radius, height, slices, color, localTransform);
	e->boundingSphere.center = Vector3::Zero;
	e->boundingSphere.radius = Vector3(radius, height, 0).getLength();
}

//------------------------------------------------------------------------------
void DrawList::drawMesh(MeshResource* mesh, int subsetIndex, CommonMaterial* material)
{
	drawMeshResourceInternal(mesh, subsetIndex, material);
}

//------------------------------------------------------------------------------
//void DrawList::drawMesh(StaticMeshModel* mesh, int subsetIndex, CommonMaterial* material)
//{
//	DrawMeshSubsetInternal(mesh, subsetIndex, material);
//}

//------------------------------------------------------------------------------
void DrawList::blit(Texture* source)
{
	blitInternal(source, nullptr, Matrix::Identity, nullptr);
}

//------------------------------------------------------------------------------
void DrawList::blit(Texture* source, const Matrix& transform)
{
	blitInternal(source, nullptr, transform, nullptr);
}

//------------------------------------------------------------------------------
void DrawList::blit(Texture* source, RenderTargetTexture* dest, const Matrix& transform)
{
	blitInternal(source, dest, transform, nullptr);
}

//------------------------------------------------------------------------------
void DrawList::blit(Texture* source, RenderTargetTexture* dest, CommonMaterial* material)
{
	blitInternal(source, dest, Matrix::Identity, material);
}

//------------------------------------------------------------------------------
void DrawList::drawGlyphRun(const Point& position, GlyphRun* glyphRun)
{
	class DrawElement_DrawGlyphRun : public detail::DrawElement
	{
	public:
		Ref<GlyphRun>	glyphRun;
		Point position;

		virtual void drawSubset(const DrawArgs& e) override
		{
			e.context->beginTextRenderer()->drawGlyphRun(getTransform(e.oenerList), position, glyphRun);
		}
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement(_LT("DrawGlyphRun")); }
	};


	// TODO: オーバーライドされる場合の動作はちゃんと考えておこう。
	// 3bd204b あたりの問題だけど、CombinedMaterial は 元 material のハッシュ値を覚えているので、それも変える必要がある。
	//Ref<Texture> old = m_defaultMaterial->getMaterialTexture(nullptr);
	//m_defaultMaterial->setMaterialTexture(m_state.state.getFont()->resolveRawFont()->GetGlyphTextureCache()->getGlyphsFillTexture());


	auto* e = resolveDrawElement<DrawElement_DrawGlyphRun>(m_manager->getInternalContext()->m_textRenderer, nullptr);
	e->glyphRun = glyphRun;
	e->position = position;
	//e->boundingSphere = ;	// TODO


	//m_defaultMaterial->setMaterialTexture(old);
}

//------------------------------------------------------------------------------
void DrawList::drawText_(const StringRef& text, const Point& position)
{
	drawText_(text, Rect(position, FLT_MAX, FLT_MAX), StringFormatFlags::LeftAlignment);
}

//------------------------------------------------------------------------------
void DrawList::drawText_(const StringRef& text, const Rect& rect, StringFormatFlags flags)
{
	class DrawElement_DrawText : public detail::DrawElement
	{
	public:
		String text;	// TODO: BlukData
		Rect rect;
		StringFormatFlags flags;

		virtual void drawSubset(const DrawArgs& e) override
		{
			e.context->beginTextRenderer()->drawString(getTransform(e.oenerList), text.c_str(), text.getLength(), rect, flags);
		}
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement(_LT("DrawText")); }
	};

	//BlendMode oldBlendMode = getCurrentState()->m_state.state.getBlendMode();
	//if (oldBlendMode == BlendMode::Normal)
	//{
	//	setBlendMode(BlendMode::Alpha);
	//}


	// TODO: オーバーライドされる場合の動作はちゃんと考えておこう。
	// 3bd204b あたりの問題だけど、CombinedMaterial は 元 material のハッシュ値を覚えているので、それも変える必要がある。
	//Ref<Texture> old = m_defaultMaterial->getMaterialTexture(nullptr);
	//m_defaultMaterial->setMaterialTexture(m_state.state.getFont()->resolveRawFont()->GetGlyphTextureCache()->getGlyphsFillTexture());

	//detail::PriorityBatchState priorityState;
	//priorityState.worldTransform = Matrix::Identity;
	//priorityState.mainTexture = getCurrentState()->m_state.state.getFont()->resolveRawFont()->GetGlyphTextureCache()->getGlyphsFillTexture();

	auto* e = resolveDrawElement<DrawElement_DrawText>(m_manager->getInternalContext()->m_textRenderer, nullptr/*, nullptr*/);
	e->text = text;
	e->rect = rect;
	e->flags = flags;
	//e->boundingSphere = ;	// TODO

	//m_defaultMaterial->setMaterialTexture(old);


	//setBlendMode(oldBlendMode);
}

//------------------------------------------------------------------------------
void DrawList::drawChar(uint32_t codePoint, const Rect& rect, StringFormatFlags flags)
{
	class DrawElement_DrawChar : public detail::DrawElement
	{
	public:
		uint32_t ch;
		Rect rect;
		StringFormatFlags flags;
		//Point position;

		virtual void drawSubset(const DrawArgs& e) override
		{
			e.context->beginTextRenderer()->drawChar(getTransform(e.oenerList), ch, rect, flags);
			//e.context->beginVectorTextRenderer()->drawChar(getTransform(e.oenerList), ch, Rect(position, 0, 0), TextLayoutOptions::None);
		}
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement(_LT("DrawChar")); }
	};

	auto* e = resolveDrawElement<DrawElement_DrawChar>(m_manager->getInternalContext()->m_textRenderer, nullptr);
	e->ch = codePoint;
	e->rect = rect;
	e->flags = flags;
	//e->boundingSphere = ;	// TODO
}

//------------------------------------------------------------------------------
void DrawList::drawText2(const StringRef& text, const Rect& rect)
{
	class DrawElement_DrawString : public detail::DrawElement
	{
	public:
		detail::CommandDataCache::DataHandle utf32DataHandle;
		int length;
		Rect rect;

		virtual void drawSubset(const DrawArgs& e) override
		{
			e.context->beginVectorTextRenderer()->drawString(
				getTransform(e.oenerList), 
				(const UTF32*)e.oenerList->getExtData(utf32DataHandle),
				length,
				rect,
				TextLayoutOptions::None);
		}
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement(_LT("DrawString")); }
	};

	const ByteBuffer& utf32Data = m_manager->getFontManager()->getTCharToUTF32Converter()->convert(text.getBegin(), text.getLength() * sizeof(Char));

	auto* e = resolveDrawElement<DrawElement_DrawString>(m_manager->getInternalContext()->m_vectorTextRenderer, nullptr);
	e->utf32DataHandle = m_drawElementList.allocExtData(utf32Data.getSize());
	e->length = utf32Data.getSize() / sizeof(UTF32);
	e->rect = rect;
	//e->boundingSphere = ;	// TODO

	memcpy(m_drawElementList.getExtData(e->utf32DataHandle), utf32Data.getConstData(), utf32Data.getSize());
}

//------------------------------------------------------------------------------
void DrawList::drawSprite(
	const Vector3& position,
	const Size& size,
	const Vector2& anchor,
	Texture* texture,
	const Rect& srcRect,
	const Color& color,
	SpriteBaseDirection baseDirection,
	BillboardType billboardType,
	CommonMaterial* material)
{
	class DrawElement_DrawSprite : public detail::DrawElement
	{
	public:
		Vector3 position;
		Vector2 size;
		Vector2 anchorRatio;
		Ref<Texture> texture;
		Rect srcRect;
		Color color;
		SpriteBaseDirection baseDirection;
		BillboardType billboardType;

		virtual void drawSubset(const DrawArgs& e) override
		{
			auto* r = e.context->beginSpriteRenderer();
			r->setTransform(getTransform(e.oenerList));
			r->drawRequest(position, size, anchorRatio, texture, srcRect, color, baseDirection, billboardType);
		}
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement(_LT("DrawSprite")); }
	};

	auto* ptr = resolveDrawElement<DrawElement_DrawSprite>(m_manager->getInternalContext()->m_spriteRenderer, material);
	ptr->position = position;
	ptr->size.set(size.width, size.height);
	ptr->anchorRatio = anchor;
	ptr->texture = texture;
	ptr->srcRect = srcRect;
	ptr->color = color;
	ptr->baseDirection = baseDirection;
	ptr->billboardType = billboardType;
	detail::SpriteRenderFeature::makeBoundingSphere(ptr->size, baseDirection, &ptr->boundingSphere);
	ptr->boundingSphere.center = Vector3::Zero;
}


//------------------------------------------------------------------------------
class DrawElement_DrawNanoVGCommands : public detail::DrawElement
{
public:
	detail::NanoVGCommandList*	m_commandList = nullptr;

	detail::NanoVGCommandList* GetGCommandList(DrawList* owner)
	{
		if (m_commandList == nullptr)
		{
			m_commandList = owner->getManager()->getNanoVGCommandListCache()->queryCommandList();
		}
		return m_commandList;
	}

	virtual void drawSubset(const DrawArgs& e) override
	{
		auto* r = e.context->beginNanoVGRenderer();
		//auto cl = r->TakeCommandList();
		//detail::NanoVGCommandHelper::nvgRect(cl, rect.x, rect.y, rect.width, rect.height);
		//detail::NanoVGCommandHelper::nvgFill(cl);
		r->executeCommand(m_commandList);
		m_commandList = nullptr;
	}
	virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement(_LT("DrawNanoVGCommands")); }
};

void DrawList::drawRectangle(const Rect& rect)
{
	if (getCurrentState()->renderingContextParameters.getBrush() != nullptr &&
		(getCurrentState()->renderingContextParameters.getBrush()->getImageDrawMode() == BrushImageDrawMode::BoxFrame || getCurrentState()->renderingContextParameters.getBrush()->getImageDrawMode() == BrushImageDrawMode::BorderFrame))
	{
		drawFrameRectangle(rect);
		return;
	}

	auto* ptr = resolveDrawElement<DrawElement_DrawNanoVGCommands>(m_manager->getInternalContext()->m_nanoVGRenderer, nullptr, true);
	auto* list = ptr->GetGCommandList(this);
	detail::NanoVGCommandHelper::nvgBeginPath(list);
	detail::NanoVGCommandHelper::nvgRect(list, rect.x, rect.y, rect.width, rect.height);
	detail::NanoVGCommandHelper::nvgFill(list);
	//ptr->rect = rect;
	// TODO: カリング
	
}

void DrawList::drawScreenRectangle()
{
	blit(nullptr);
}

//------------------------------------------------------------------------------
void DrawList::addDynamicLightInfo(detail::DynamicLightInfo* lightInfo)
{
	m_drawElementList.addDynamicLightInfo(lightInfo);
}

//------------------------------------------------------------------------------
void DrawList::pushMetadata(const DrawElementMetadata* metadata)
{
	if (LN_REQUIRE(m_metadata == nullptr)) return;
	m_metadata = metadata;
	// TODO: stack
}

//------------------------------------------------------------------------------
const DrawElementMetadata* DrawList::getMetadata()
{
	return m_metadata;
}

//------------------------------------------------------------------------------
void DrawList::popMetadata()
{
	m_metadata = nullptr;
}
//
////------------------------------------------------------------------------------
//template<typename TElement>
//TElement* DrawList::resolveDrawElement(detail::DrawingSectionId sectionId, detail::IRendererPloxy* renderer, CommonMaterial* userMaterial)
//{
//	CommonMaterial* availableMaterial = (userMaterial != nullptr) ? userMaterial : m_defaultMaterial.Get();
//
//	// これを決定してから比較を行う
//	m_state.state.SetStandaloneShaderRenderer(renderer->isStandaloneShader());
//
//	m_state.state.m_rendererId = reinterpret_cast<intptr_t>(renderer);
//
//	const DrawElementMetadata* userMetadata = getMetadata();
//	const DrawElementMetadata* metadata = (userMetadata != nullptr) ? userMetadata : &DrawElementMetadata::Default;
//
//	// 何か前回追加された DrawElement があり、それと DrawingSectionId、State が一致するならそれに対して追記できる
//	if (sectionId != detail::DrawingSectionId::None &&
//		m_currentSectionTopElement != nullptr &&
//		m_currentSectionTopElement->drawingSectionId == sectionId &&
//		m_currentSectionTopElement->metadata.Equals(*metadata) &&
//		m_drawElementList.getBatch(m_currentSectionTopElement->batchIndex)->Equal(m_state.state.state, availableMaterial, m_state.state.getTransfrom()))
//	{
//		return static_cast<TElement*>(m_currentSectionTopElement);
//	}
//
//	// DrawElement を新しく作る
//	TElement* element = m_drawElementList.addCommand<TElement>(m_state.state.state, availableMaterial, m_state.state.getTransfrom());
//	//element->OnJoindDrawList(m_state.transfrom);
//	element->drawingSectionId = sectionId;
//	element->metadata = *metadata;
//	m_currentSectionTopElement = element;
//	return element;
//}

//------------------------------------------------------------------------------
void DrawList::drawMeshResourceInternal(MeshResource* mesh, int subsetIndex, CommonMaterial* material)
{
	class DrawElement_DrawMeshResourceInternal : public detail::LightingDrawElement
	{
	public:
		Ref<MeshResource>	mesh;
		int startIndex;
		int primitiveCount;
		PrimitiveType primitiveType;

		virtual void drawSubset(const DrawArgs& e) override
		{
			e.context->beginMeshRenderer()->drawMesh(mesh, startIndex, primitiveCount, primitiveType);
		}
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement(_LT("DrawMeshResourceInternal")); }
	};

	MeshAttribute attr;
	mesh->getMeshAttribute(subsetIndex, &attr);
	if (attr.PrimitiveNum == 0) return;		// not need draw

	auto* e = resolveDrawElement<DrawElement_DrawMeshResourceInternal>(m_manager->getInternalContext()->m_meshRenderer, material);
	e->subsetIndex = subsetIndex;
	e->mesh = mesh;
	e->startIndex = attr.StartIndex;
	e->primitiveCount = attr.PrimitiveNum;
	e->primitiveType = attr.primitiveType;
	//e->boundingSphere = ;	// TODO
}

//------------------------------------------------------------------------------
//void DrawList::DrawMeshSubsetInternal(StaticMeshModel* mesh, int subsetIndex, CommonMaterial* material)
//{
//	/* 
//	 * この時点では MeshResource ではなく StaticMeshModel が必要。
//	 * LOD リソースがある場合、実際に書くときの視点情報を元に、描画する LOD リソースを選択する必要がある。
//	 */
//
//	class DrawElement_DrawMeshInternal : public detail::LightingDrawElement
//	{
//	public:
//		Ref<StaticMeshModel>	mesh;
//		int startIndex;
//		int primitiveCount;
//		PrimitiveType primitiveType;
//
//		virtual void drawSubset(detail::DrawElementList* oenerList, detail::InternalContext* context) override
//		{
//			context->beginMeshRenderer()->drawMesh(mesh->getMeshResource(), startIndex, primitiveCount, primitiveType);
//		}
//	};
//	MeshAttribute attr;
//	mesh->getMeshResource()->getMeshAttribute(subsetIndex, &attr);
//	if (attr.PrimitiveNum == 0) return;		// not need draw
//
//	auto* e = resolveDrawElement<DrawElement_DrawMeshInternal>(m_manager->getInternalContext()->m_meshRenderer, material);
//	e->subsetIndex = subsetIndex;
//	e->mesh = mesh;
//	e->startIndex = attr.StartIndex;
//	e->primitiveCount = attr.PrimitiveNum;
//	e->primitiveType = attr.primitiveType;
//	//e->boundingSphere = ;	// TODO
//}

//------------------------------------------------------------------------------
void DrawList::blitInternal(Texture* source, RenderTargetTexture* dest, const Matrix& transform, CommonMaterial* material)
{

	class DrawElement_BlitInternal : public detail::DrawElement
	{
	public:
		Matrix			overrideTransform;
		Ref<Texture>	source;

		virtual void makeElementInfo(detail::DrawElementList* oenerList, const detail::CameraInfo& cameraInfo, RenderView* renderView, detail::ElementInfo* outInfo) override
		{
			DrawElement::makeElementInfo(oenerList, cameraInfo, renderView, outInfo);
			outInfo->WorldViewProjectionMatrix = overrideTransform;
		}
		virtual void makeSubsetInfo(detail::DrawElementList* oenerList, detail::RenderStage* stage, detail::SubsetInfo* outInfo) override
		{
			DrawElement::makeSubsetInfo(oenerList, stage, outInfo);

			// MaterialTexture を上書きする
			outInfo->materialTexture = source;
		}

		virtual void drawSubset(const DrawArgs& e) override
		{
			e.context->beginBlitRenderer()->blit();
		}
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement(_LT("BlitInternal")); }
	};

	if (dest != nullptr)
	{
		setRenderTarget(0, dest);
	}

	auto* e = resolveDrawElement<DrawElement_BlitInternal>(m_manager->getInternalContext()->m_blitRenderer, material);
	e->overrideTransform = transform;
	e->source = source;
}

//------------------------------------------------------------------------------
void DrawList::drawFrameRectangle(const Rect& rect)
{
	class DrawElement_DrawFrameRectangle : public detail::DrawElement
	{
	public:
		Rect rect;

		virtual void makeSubsetInfo(detail::DrawElementList* oenerList, detail::RenderStage* stage, detail::SubsetInfo* outInfo) override
		{
			DrawElement::makeSubsetInfo(oenerList, stage, outInfo);

			// MaterialTexture を上書きする
			//outInfo->materialTexture = oenerList->getBatch(batchIndex)->state.getBrush()->getTexture();
			outInfo->materialTexture = oenerList->getRenderStage(batchIndex)->getBrushFinal()->getTexture();
		}
		virtual void drawSubset(const DrawArgs& e) override
		{
			auto* r = e.context->beginFrameRectRenderer();
			r->draw(getTransform(e.oenerList), rect);
		}
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement(_LT("DrawFrameRectangle")); }
	};
	auto* ptr = resolveDrawElement<DrawElement_DrawFrameRectangle>(m_manager->getInternalContext()->m_frameRectRenderer, nullptr);
	ptr->rect = rect;
	// TODO: カリング
}

//------------------------------------------------------------------------------
//void DrawList::renderSubView(RenderView* listSet, detail::SceneRenderer* renderer, RenderTargetTexture* defaultRenderTarget, DepthBuffer* defaultDepthBuffer)
//{
//	class DrawElement_RenderSubView : public detail::DrawElement
//	{
//	public:
//		RenderView* listSet;
//		detail::SceneRenderer* renderer;
//		Ref<RenderTargetTexture>	defaultRenderTarget;
//		Ref<DepthBuffer> defaultDepthBuffer;
//
//		virtual void drawSubset(const DrawArgs& e) override
//		{
//			// それぞれ、省略されていれば親のを使う
//			detail::SceneRenderer* primRenderer = (renderer != nullptr) ? renderer : e.renderer;
//			RenderTargetTexture* primRenderTarget = (defaultRenderTarget != nullptr) ? defaultRenderTarget : e.defaultRenderTarget;
//			DepthBuffer* primDepthBuffer = (defaultDepthBuffer != nullptr) ? defaultDepthBuffer : e.defaultDepthBuffer;
//
//			// TODO: scoped change block
//			auto* status = e.context->getCurrentStatus();
//
//			primRenderer->render(listSet, primRenderTarget, primDepthBuffer, e.diag);
//
//			// ステート復帰
//			e.context->applyStatus(status, { e.defaultRenderTarget, e.defaultDepthBuffer });
//		}
//		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement(_LT("RenderSubView")); }
//	};
//
//	// TODO: m_frameRectRenderer は違う気がする・・・
//	auto* e = resolveDrawElement<DrawElement_RenderSubView>(m_manager->getInternalContext()->m_frameRectRenderer, nullptr);
//	//e->elementList = listSet->m_lists[0];
//	//e->cameraInfo = listSet->m_cameraInfo;
//	e->listSet = listSet;
//	e->renderer = renderer;
//	e->defaultRenderTarget = defaultRenderTarget;
//	e->defaultDepthBuffer = defaultDepthBuffer;
//}



//==============================================================================
// RenderView
//==============================================================================
//------------------------------------------------------------------------------
RenderView::RenderView()
	: m_viewSize()
	, m_sceneRenderer(nullptr)
{
}

//------------------------------------------------------------------------------
RenderView::~RenderView()
{
}

//------------------------------------------------------------------------------
void RenderView::filterWorldMatrix(Matrix* outMatrix)
{
}

//==============================================================================
// StagingState
//==============================================================================
//------------------------------------------------------------------------------
//DrawList::StagingState::StagingState()
//{
//	//m_defaultMaterial = Ref<CommonMaterial>::makeRef();
//	//m_defaultMaterial->initialize();
//	reset();
//}
//
////------------------------------------------------------------------------------
//void DrawList::StagingState::reset()
//{
//	m_state.reset();
//	m_material = nullptr;//->reset();
//	m_builtinEffectData.reset();
//	m_defaultMaterialShader = nullptr;
//}
//
////------------------------------------------------------------------------------
//void DrawList::StagingState::copyFrom(const StagingState* state)
//{
//	m_state = state->m_state;
//	m_material = state->m_material;
//	m_builtinEffectData = state->m_builtinEffectData;
//	m_defaultMaterialShader = state->m_defaultMaterialShader;
//}

LN_NAMESPACE_END

