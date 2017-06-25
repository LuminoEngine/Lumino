
#include "../Internal.h"
#include <float.h>
#include <Lumino/Graphics/Color.h>
#include <Lumino/Rendering/Rendering.h>
#include <Lumino/Rendering/SceneRenderer.h>	// TODO
#include <Lumino/Graphics/ContextInterface.h>
#include <Lumino/Mesh/Mesh.h>
#include "../Graphics/Device/GraphicsDriverInterface.h"
#include "../Graphics/GraphicsManager.h"
#include "../Graphics/CoreGraphicsRenderFeature.h"
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




LN_NAMESPACE_BEGIN

const DrawElementMetadata DrawElementMetadata::Default;

namespace detail {

//==============================================================================
// DynamicLightInfo
//==============================================================================

//------------------------------------------------------------------------------
DynamicLightInfo::DynamicLightInfo()
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

	m_primitiveRenderer = RefPtr<PrimitiveRenderFeature>::makeRef();
	m_primitiveRenderer->initialize(manager);

	m_blitRenderer = RefPtr<BlitRenderer>::makeRef();
	m_blitRenderer->initialize(manager);

	m_meshRenderer = RefPtr<MeshRenderFeature>::makeRef();
	m_meshRenderer->initialize(manager);

	m_spriteRenderer = RefPtr<SpriteRenderFeature>::makeRef(manager, 2048);	// TODO

	m_textRenderer = RefPtr<TextRenderer>::makeRef();
	m_textRenderer->initialize(manager);

	m_vectorTextRenderer = RefPtr<VectorTextRenderer>::makeRef();
	m_vectorTextRenderer->initialize(manager);

	m_shapesRenderer = RefPtr<ShapesRenderFeature>::makeRef();
	m_shapesRenderer->initialize(manager);

	m_nanoVGRenderer = RefPtr<NanoVGRenderFeature>::makeRef();
	m_nanoVGRenderer->initialize(manager);

	m_frameRectRenderer = RefPtr<FrameRectRenderFeature>::makeRef();
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
	switchActiveRenderer(m_baseRenderer);
	return m_baseRenderer;
}

//------------------------------------------------------------------------------
PrimitiveRenderFeature* InternalContext::beginPrimitiveRenderer()
{
	switchActiveRenderer(m_primitiveRenderer);
	return m_primitiveRenderer;
}

//------------------------------------------------------------------------------
BlitRenderer* InternalContext::beginBlitRenderer()
{
	switchActiveRenderer(m_blitRenderer);
	return m_blitRenderer;
}

//------------------------------------------------------------------------------
MeshRenderFeature* InternalContext::beginMeshRenderer()
{
	switchActiveRenderer(m_meshRenderer);
	return m_meshRenderer;
}

//------------------------------------------------------------------------------
SpriteRenderFeature* InternalContext::beginSpriteRenderer()
{
	switchActiveRenderer(m_spriteRenderer);
	return m_spriteRenderer;
}

//------------------------------------------------------------------------------
TextRenderer* InternalContext::beginTextRenderer()
{
	switchActiveRenderer(m_textRenderer);
	return m_textRenderer;
}

//------------------------------------------------------------------------------
VectorTextRenderer* InternalContext::beginVectorTextRenderer()
{
	switchActiveRenderer(m_vectorTextRenderer);
	return m_vectorTextRenderer;
}

//------------------------------------------------------------------------------
ShapesRenderFeature* InternalContext::beginShapesRenderer()
{
	switchActiveRenderer(m_shapesRenderer);
	return m_shapesRenderer;
}

//------------------------------------------------------------------------------
NanoVGRenderFeature* InternalContext::beginNanoVGRenderer()
{
	switchActiveRenderer(m_nanoVGRenderer);
	return m_nanoVGRenderer;
}

//------------------------------------------------------------------------------
FrameRectRenderFeature* InternalContext::beginFrameRectRenderer()
{
	switchActiveRenderer(m_frameRectRenderer);
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
void InternalContext::applyStatus(DrawElementBatch* state, const DefaultStatus& defaultStatus)
{
	m_currentStatePtr = state;
	m_currentStatePtr->applyStatus(this, defaultStatus);

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

	m_brush = nullptr;
	m_font = nullptr;

	m_hashCode = 0;
	m_hashDirty = true;
}

//------------------------------------------------------------------------------
void BatchState::applyStatus(InternalContext* context, CombinedMaterial* combinedMaterial, const DefaultStatus& defaultStatus)
{
	auto* stateManager = context->getRenderStateManager();

	// RenderState
	{
		BlendMode blendMode = (combinedMaterial->m_blendMode.isSet()) ? combinedMaterial->m_blendMode.get() : m_blendMode;
		CullingMode cullingMode = (combinedMaterial->m_cullingMode.isSet()) ? combinedMaterial->m_cullingMode.get() : m_cullingMode;



		// TODO: Base
		RenderState state;
		ContextInterface::makeBlendMode(blendMode, &state);
		state.Culling = cullingMode;
		//state.AlphaTest = combinedMaterial->m_alphaTest;
		stateManager->setRenderState(state);

		// スプライトバッチ化のため (TODO: いらないかも。SpriteRenderer では State でそーとしなくなった)
		context->getSpriteRenderer()->setState(state);
	}
	// DepthStencilState
	{
		DepthStencilState state;
		state.DepthTestEnabled = (combinedMaterial->m_depthTestEnabled.isSet()) ? combinedMaterial->m_depthTestEnabled.get() : m_depthTestEnabled;
		state.DepthWriteEnabled = (combinedMaterial->m_depthWriteEnabled.isSet()) ? combinedMaterial->m_depthWriteEnabled.get() : m_depthWriteEnabled;
		stateManager->setDepthStencilState(state);
	}
	// FrameBuffer
	{
		//if (defaultRenderTarget == nullptr && defaultDepthBuffer == nullptr)
		//{
		//	// TODO: 子DrawListが実行された後のステート復帰。
		//	// もしかしたらオプションにはできないかもしれない。
		//	return;
		//}

		for (int i = 0; i < Graphics::MaxMultiRenderTargets; ++i)
		{
			if (i == 0 && m_renderTargets[i] == nullptr)
				stateManager->setRenderTarget(i, defaultStatus.defaultRenderTarget);
			else
				stateManager->setRenderTarget(i, m_renderTargets[i]);
		}
		if (m_depthBuffer == nullptr)
			stateManager->setDepthBuffer(defaultStatus.defaultDepthBuffer);
		else
			stateManager->setDepthBuffer(m_depthBuffer);
		// TODO: m_scissorRect
	}
}

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

//------------------------------------------------------------------------------
void DrawElementBatch::SetStandaloneShaderRenderer(bool enabled)
{
	if (m_standaloneShaderRenderer != enabled)
	{
		m_standaloneShaderRenderer = enabled;
		m_hashDirty = true;
	}
}

//------------------------------------------------------------------------------
bool DrawElementBatch::IsStandaloneShaderRenderer() const
{
	return m_standaloneShaderRenderer;
}

//------------------------------------------------------------------------------
bool DrawElementBatch::Equal(const BatchState& state_, Material* material, const Matrix& transfrom, const BuiltinEffectData& effectData) const
{
	assert(m_combinedMaterial != nullptr);
	return
		state.getHashCode() == state_.getHashCode() &&
		m_combinedMaterial->getSourceHashCode() == material->getHashCode() &&
		m_transfrom == transfrom &&
		m_builtinEffectData.getHashCode() == effectData.getHashCode();
//#if 1
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
	m_standaloneShaderRenderer = false;

	m_hashCode = 0;
	m_hashDirty = true;
}

//------------------------------------------------------------------------------
void DrawElementBatch::applyStatus(InternalContext* context, const DefaultStatus& defaultStatus)
{
	state.applyStatus(context, m_combinedMaterial, defaultStatus);
}

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
	, drawingSectionId(DrawingSectionId::None)
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
	return oenerList->getBatch(batchIndex)->getTransfrom();
}

//------------------------------------------------------------------------------
void DrawElement::makeElementInfo(DrawElementList* oenerList, const CameraInfo& cameraInfo, ElementInfo* outInfo)
{
	outInfo->viewProjMatrix = &cameraInfo.viewProjMatrix;
	outInfo->WorldMatrix = getTransform(oenerList);
	outInfo->WorldViewProjectionMatrix = outInfo->WorldMatrix * cameraInfo.viewMatrix * cameraInfo.projMatrix;	// TODO: viewProj はまとめたい
	outInfo->affectedLights = getAffectedDynamicLightInfos();
}

//------------------------------------------------------------------------------
void DrawElement::makeSubsetInfo(DrawElementList* oenerList, CombinedMaterial* material, SubsetInfo* outInfo)
{
	outInfo->combinedMaterial = material;
	outInfo->materialTexture = (material != nullptr) ? material->m_mainTexture : nullptr;
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
	m_batchList.clear();

	m_combinedMaterialCache.releaseAll();
	m_dynamicLightList.clear();
}

//------------------------------------------------------------------------------
void DrawElementList::postAddCommandInternal(const BatchState& state, Material* availableMaterial, const Matrix& transform, const BuiltinEffectData& effectData, bool forceStateChange, DrawElement* element)
{
	//if (forceStateChange || m_batchList.isEmpty() || !m_batchList.getLast().Equal(state, availableMaterial, transform, effectData))
	{
		// CombinedMaterial を作る
		CombinedMaterial* cm = m_combinedMaterialCache.queryCommandList();
		cm->combine(nullptr, availableMaterial, effectData);	// TODO

		// 新しく DrawElementBatch を作る
		m_batchList.add(DrawElementBatch());
		m_batchList.getLast().state = state;
		m_batchList.getLast().setCombinedMaterial(cm);
		m_batchList.getLast().setTransfrom(transform);
		m_batchList.getLast().SetBuiltinEffect(effectData);
	}
	element->batchIndex = m_batchList.getCount() - 1;
}

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




//==============================================================================
class ClearElement : public DrawElement
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

	virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement("Clear"); }
};

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
	instantiate<detail::RenderDiagItem_BeginRenderView>("BeginRenderView")->m_subType = RenderDiagItem::SubType::ScopeBegin;
}

//------------------------------------------------------------------------------
void RenderDiag::endRenderView()
{
	instantiate<detail::RenderDiagItem_EndRenderView>("EndRenderView")->m_subType = RenderDiagItem::SubType::ScopeEnd;
}

//------------------------------------------------------------------------------
void RenderDiag::beginDrawList()
{
	instantiate<detail::RenderDiagItem_BeginDrawList>("BeginDrawList")->m_subType = RenderDiagItem::SubType::ScopeBegin;
}

//------------------------------------------------------------------------------
void RenderDiag::endDrawList()
{
	instantiate<detail::RenderDiagItem_EndDrawList>("EndDrawList")->m_subType = RenderDiagItem::SubType::ScopeEnd;
}

//------------------------------------------------------------------------------
void RenderDiag::changeRenderStage()
{
	instantiate<detail::RenderDiagItem_EndDrawList>("ChangeRenderStage");
}

//------------------------------------------------------------------------------
void RenderDiag::callCommonElement(const TCHAR* typeName)
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
	: m_currentSectionTopElement(nullptr)
	, m_metadata(nullptr)
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
	if (LN_CHECK_ARG(manager != nullptr)) return;
	m_manager = manager;
	m_state.reset();

	m_defaultMaterial = RefPtr<Material>::makeRef();
	m_defaultMaterial->initialize();
}

//------------------------------------------------------------------------------
void DrawList::setRenderTarget(int index, RenderTargetTexture* renderTarget)
{
	m_state.state.setRenderTarget(index, renderTarget);
	m_currentStateFence++;
}

//------------------------------------------------------------------------------
RenderTargetTexture* DrawList::getRenderTarget(int index) const
{
	return m_state.state.getRenderTarget(index);
}

//------------------------------------------------------------------------------
void DrawList::setDepthBuffer(DepthBuffer* depthBuffer)
{
	m_state.state.setDepthBuffer(depthBuffer);
	m_currentStateFence++;
}

//------------------------------------------------------------------------------
DepthBuffer* DrawList::getDepthBuffer() const
{
	return m_state.state.getDepthBuffer();
}

//------------------------------------------------------------------------------
void DrawList::setBrush(Brush* brush)
{
	brush = (brush != nullptr) ? brush : Brush::Black;
	m_state.state.setBrush(brush);
}

//------------------------------------------------------------------------------
Brush* DrawList::getBrush() const
{
	return m_state.state.getBrush();
}

//------------------------------------------------------------------------------
void DrawList::setFont(Font* font)
{
	font = (font != nullptr) ? font : m_manager->getFontManager()->getDefaultFont();

	m_state.state.setFont(font);
}

//------------------------------------------------------------------------------
void DrawList::setShader(Shader* shader)
{
	m_defaultMaterial->setShader(shader);
}

//------------------------------------------------------------------------------
Shader* DrawList::getShader() const
{
	return m_defaultMaterial->getShader();
}

//------------------------------------------------------------------------------
void DrawList::setBlendMode(BlendMode mode)
{
	m_state.state.setBlendMode(mode);
}

//------------------------------------------------------------------------------
void DrawList::setCullingMode(CullingMode mode)
{
	m_state.state.setCullingMode(mode);
}

//------------------------------------------------------------------------------
//void DrawList::setOpacity(float opacity)
//{
//	m_state.state.
//}

//------------------------------------------------------------------------------
void DrawList::setDepthTestEnabled(bool enabled)
{
	m_state.state.setDepthTestEnabled(enabled);
}

//------------------------------------------------------------------------------
void DrawList::setDepthWriteEnabled(bool enabled)
{
	m_state.state.setDepthWriteEnabled(enabled);
}

//------------------------------------------------------------------------------
void DrawList::setDefaultMaterial(Material* material)
{
	if (LN_CHECK_ARG(material != nullptr)) return;
	m_defaultMaterial = material;
}

//------------------------------------------------------------------------------
void DrawList::setBuiltinEffectData(const detail::BuiltinEffectData& data)
{
	m_builtinEffectData = data;
}

//------------------------------------------------------------------------------
void DrawList::beginMakeElements()
{
	m_drawElementList.clearCommands();
	m_state.reset();
	setBrush(nullptr);
	setFont(nullptr);
	//m_state.state.state.setFont(m_manager->getFontManager()->getDefaultFont());
	m_defaultMaterial->reset();
	m_builtinEffectData.reset();
	//m_defaultMaterial->cullingMode = CullingMode::None;
	m_currentSectionTopElement = nullptr;
	m_currentStateFence = 0;
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
	m_state.setTransfrom(transform);
}

//------------------------------------------------------------------------------
void DrawList::clear(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
	auto* ptr = m_drawElementList.addCommand<detail::ClearElement>(m_state.state, m_defaultMaterial, Matrix::Identity, m_builtinEffectData, false);
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
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement("DrawLine"); }
	};
	auto* ptr = resolveDrawElement<DrawElement_DrawLine>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_primitiveRenderer, nullptr);
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
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement("DrawSquarePrimitive"); }
	};
	auto* e = resolveDrawElement<DrawSquarePrimitiveElement>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_primitiveRenderer, nullptr);
	e->position[0] = position1; e->uv[0] = uv1; e->color[0] = color1;
	e->position[1] = position2; e->uv[1] = uv2; e->color[1] = color2;
	e->position[2] = position3; e->uv[2] = uv3; e->color[2] = color3;
	e->position[3] = position4; e->uv[3] = uv4; e->color[3] = color4;
	e->makeBoundingSphere(Vector3::min(e->position, 4), Vector3::max(e->position, 4));
}

//------------------------------------------------------------------------------
void DrawList::drawSquare(float sizeX, float sizeZ, int slicesX, int slicesZ, const Color& color, const Matrix& localTransform, Material* material)
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
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement("DrawCylinderElement"); }
	};
	auto* e = resolveDrawElement<DrawCylinderElement>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_primitiveRenderer, material);
	e->factory.initialize(Vector2(sizeX, sizeZ), slicesX, slicesZ, color, localTransform);
	e->boundingSphere.center = Vector3::Zero;
	e->boundingSphere.radius = Vector3(sizeX, sizeZ, 0).getLength();
}

//------------------------------------------------------------------------------
void DrawList::drawArc(float startAngle, float endAngle, float innerRadius, float outerRadius, int slices, const Color& color, const Matrix& localTransform, Material* material)
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
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement("DrawArcElement"); }
	};
	auto* e = resolveDrawElement<DrawArcElement>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_primitiveRenderer, material);
	e->factory.initialize(startAngle, endAngle, innerRadius, outerRadius, slices, color, localTransform);
	e->boundingSphere.center = Vector3::Zero;
	e->boundingSphere.radius = outerRadius;
}

//------------------------------------------------------------------------------
void DrawList::drawBox(const Box& box, const Color& color, const Matrix& localTransform, Material* material)
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
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement("DrawBoxElement"); }
	};
	auto* e = resolveDrawElement<DrawBoxElement>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_primitiveRenderer, material);
	e->factory.initialize(Vector3(box.width, box.height, box.depth), color, localTransform);

	Vector3 min, max;
	box.getMinMax(&min, &max);
	e->makeBoundingSphere(min, max);
}

//------------------------------------------------------------------------------
void DrawList::drawSphere(float radius, int slices, int stacks, const Color& color, const Matrix& localTransform, Material* material)
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
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement("DrawSphereElement"); }
	};
	auto* e = resolveDrawElement<DrawSphereElement>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_primitiveRenderer, material);
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
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement("DrawCylinder"); }
	};
	auto* e = resolveDrawElement<DrawCylinderElement>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_primitiveRenderer, nullptr);
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
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement("DrawCone"); }
	};
	auto* e = resolveDrawElement<DrawConeElement>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_primitiveRenderer, nullptr);
	e->factory.initialize(radius, height, slices, color, localTransform);
	e->boundingSphere.center = Vector3::Zero;
	e->boundingSphere.radius = Vector3(radius, height, 0).getLength();
}

//------------------------------------------------------------------------------
void DrawList::drawMesh(MeshResource* mesh, int subsetIndex, Material* material)
{
	drawMeshResourceInternal(mesh, subsetIndex, material);
}

//------------------------------------------------------------------------------
//void DrawList::drawMesh(StaticMeshModel* mesh, int subsetIndex, Material* material)
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
void DrawList::blit(Texture* source, RenderTargetTexture* dest, Material* material)
{
	blitInternal(source, dest, Matrix::Identity, material);
}

//------------------------------------------------------------------------------
void DrawList::drawGlyphRun(const PointF& position, GlyphRun* glyphRun)
{
	class DrawElement_DrawGlyphRun : public detail::DrawElement
	{
	public:
		RefPtr<GlyphRun>	glyphRun;
		PointF position;

		virtual void drawSubset(const DrawArgs& e) override
		{
			e.context->beginTextRenderer()->drawGlyphRun(getTransform(e.oenerList), position, glyphRun);
		}
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement("DrawGlyphRun"); }
	};

	auto* e = resolveDrawElement<DrawElement_DrawGlyphRun>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_textRenderer, nullptr);
	e->glyphRun = glyphRun;
	e->position = position;
	//e->boundingSphere = ;	// TODO
}

//------------------------------------------------------------------------------
void DrawList::drawText_(const StringRef& text, const PointF& position)
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

		virtual void makeElementInfo(detail::DrawElementList* oenerList, const detail::CameraInfo& cameraInfo, detail::ElementInfo* outInfo) override
		{
			// ワールド行列は作らない。一連の Glyphs を描画する方に任せる。
			// (スプライトと同じく、できるだけ一度に描画する)
			outInfo->viewProjMatrix = &cameraInfo.viewProjMatrix;
			outInfo->WorldMatrix = Matrix::Identity;//getTransform(oenerList);
			outInfo->WorldViewProjectionMatrix = cameraInfo.viewMatrix * cameraInfo.projMatrix;// outInfo->WorldMatrix * cameraInfo.viewMatrix * cameraInfo.projMatrix;	// TODO: viewProj はまとめたい
			outInfo->affectedLights = getAffectedDynamicLightInfos();
		}

		//virtual void makeSubsetInfo(detail::DrawElementList* oenerList, detail::CombinedMaterial* material, detail::SubsetInfo* outInfo) override
		//{
		//	DrawElement::makeSubsetInfo(oenerList, material, outInfo);

		//	// MaterialTexture を上書きする
		//	outInfo->materialTexture = oenerList->getBatch(batchIndex)->state.getFont()->resolveRawFont()->GetGlyphTextureCache()->getGlyphsFillTexture();
		//}

		virtual void drawSubset(const DrawArgs& e) override
		{
			e.context->beginTextRenderer()->drawString(getTransform(e.oenerList), text.c_str(), text.getLength(), rect, flags);
		}
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement("DrawText"); }
	};

	RefPtr<Texture> old = m_defaultMaterial->getMaterialTexture(nullptr);
	m_defaultMaterial->setMaterialTexture(m_state.state.getFont()->resolveRawFont()->GetGlyphTextureCache()->getGlyphsFillTexture());

	auto* e = resolveDrawElement<DrawElement_DrawText>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_textRenderer, nullptr);
	e->text = text;
	e->rect = rect;
	e->flags = flags;
	//e->boundingSphere = ;	// TODO

	m_defaultMaterial->setMaterialTexture(old);


	//m_drawElementList.getBatch(e->batchIndex)->getCombinedMaterial()->m_mainTexture = m_state.state.getFont()->resolveRawFont()->GetGlyphTextureCache()->getGlyphsFillTexture();
}

//------------------------------------------------------------------------------
void DrawList::drawChar(TCHAR ch, const PointF& position)
{
	class DrawElement_DrawChar : public detail::DrawElement
	{
	public:
		TCHAR ch;
		PointF position;

		virtual void drawSubset(const DrawArgs& e) override
		{
			e.context->beginVectorTextRenderer()->drawChar(getTransform(e.oenerList), ch, Rect(position, 0, 0), TextLayoutOptions::None);
		}
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement("DrawChar"); }
	};

	// TODO: UTF32 変換

	auto* e = resolveDrawElement<DrawElement_DrawChar>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_vectorTextRenderer, nullptr);
	e->ch = ch;
	e->position = position;
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
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement("DrawString"); }
	};

	const ByteBuffer& utf32Data = m_manager->getFontManager()->getTCharToUTF32Converter()->convert(text.getBegin(), text.getLength() * sizeof(TCHAR));

	auto* e = resolveDrawElement<DrawElement_DrawString>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_vectorTextRenderer, nullptr);
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
	Material* material)
{
	class DrawElement_DrawSprite : public detail::DrawElement
	{
	public:
		Vector3 position;
		Vector2 size;
		Vector2 anchorRatio;
		RefPtr<Texture> texture;
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
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement("DrawSprite"); }
	};

	auto* ptr = resolveDrawElement<DrawElement_DrawSprite>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_spriteRenderer, material);
	ptr->position = position;
	ptr->size.set(size.width, size.height);
	ptr->anchorRatio = anchor;
	ptr->texture = texture;
	ptr->srcRect = srcRect;
	ptr->color = color;
	ptr->baseDirection = baseDirection;
	ptr->billboardType = billboardType;
	detail::SpriteRenderFeature::makeBoundingSphere(ptr->size, baseDirection, &ptr->boundingSphere);
	ptr->boundingSphere.center += m_state.getTransfrom().getPosition();	// TODO: 他と共通化
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
	virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement("DrawNanoVGCommands"); }
};

void DrawList::drawRectangle(const Rect& rect)
{
	if (m_state.state.getBrush() != nullptr &&
		(m_state.state.getBrush()->getImageDrawMode() == BrushImageDrawMode::BoxFrame || m_state.state.getBrush()->getImageDrawMode() == BrushImageDrawMode::BorderFrame))
	{
		drawFrameRectangle(rect);
		return;
	}

	auto* ptr = resolveDrawElement<DrawElement_DrawNanoVGCommands>(detail::DrawingSectionId::NanoVG, m_manager->getInternalContext()->m_nanoVGRenderer, nullptr);
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
	if (LN_CHECK_STATE(m_metadata == nullptr)) return;
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
//TElement* DrawList::resolveDrawElement(detail::DrawingSectionId sectionId, detail::IRendererPloxy* renderer, Material* userMaterial)
//{
//	Material* availableMaterial = (userMaterial != nullptr) ? userMaterial : m_defaultMaterial.Get();
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
void DrawList::drawMeshResourceInternal(MeshResource* mesh, int subsetIndex, Material* material)
{
	class DrawElement_DrawMeshResourceInternal : public detail::LightingDrawElement
	{
	public:
		RefPtr<MeshResource>	mesh;
		int startIndex;
		int primitiveCount;
		PrimitiveType primitiveType;

		virtual void drawSubset(const DrawArgs& e) override
		{
			e.context->beginMeshRenderer()->drawMesh(mesh, startIndex, primitiveCount, primitiveType);
		}
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement("DrawMeshResourceInternal"); }
	};

	MeshAttribute attr;
	mesh->getMeshAttribute(subsetIndex, &attr);
	if (attr.PrimitiveNum == 0) return;		// not need draw

	auto* e = resolveDrawElement<DrawElement_DrawMeshResourceInternal>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_meshRenderer, material);
	e->subsetIndex = subsetIndex;
	e->mesh = mesh;
	e->startIndex = attr.StartIndex;
	e->primitiveCount = attr.PrimitiveNum;
	e->primitiveType = attr.primitiveType;
	//e->boundingSphere = ;	// TODO
}

//------------------------------------------------------------------------------
//void DrawList::DrawMeshSubsetInternal(StaticMeshModel* mesh, int subsetIndex, Material* material)
//{
//	/* 
//	 * この時点では MeshResource ではなく StaticMeshModel が必要。
//	 * LOD リソースがある場合、実際に書くときの視点情報を元に、描画する LOD リソースを選択する必要がある。
//	 */
//
//	class DrawElement_DrawMeshInternal : public detail::LightingDrawElement
//	{
//	public:
//		RefPtr<StaticMeshModel>	mesh;
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
//	auto* e = resolveDrawElement<DrawElement_DrawMeshInternal>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_meshRenderer, material);
//	e->subsetIndex = subsetIndex;
//	e->mesh = mesh;
//	e->startIndex = attr.StartIndex;
//	e->primitiveCount = attr.PrimitiveNum;
//	e->primitiveType = attr.primitiveType;
//	//e->boundingSphere = ;	// TODO
//}

//------------------------------------------------------------------------------
void DrawList::blitInternal(Texture* source, RenderTargetTexture* dest, const Matrix& transform, Material* material)
{

	class DrawElement_BlitInternal : public detail::DrawElement
	{
	public:
		Matrix			overrideTransform;
		RefPtr<Texture>	source;

		virtual void makeElementInfo(detail::DrawElementList* oenerList, const detail::CameraInfo& cameraInfo, detail::ElementInfo* outInfo) override
		{
			DrawElement::makeElementInfo(oenerList, cameraInfo, outInfo);
			outInfo->WorldViewProjectionMatrix = overrideTransform;
		}
		virtual void makeSubsetInfo(detail::DrawElementList* oenerList, detail::CombinedMaterial* material, detail::SubsetInfo* outInfo) override
		{
			DrawElement::makeSubsetInfo(oenerList, material, outInfo);

			// MaterialTexture を上書きする
			outInfo->materialTexture = source;
		}

		virtual void drawSubset(const DrawArgs& e) override
		{
			e.context->beginBlitRenderer()->blit();
		}
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement("BlitInternal"); }
	};

	if (dest != nullptr)
	{
		setRenderTarget(0, dest);
	}

	auto* e = resolveDrawElement<DrawElement_BlitInternal>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_blitRenderer, material);
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

		virtual void makeSubsetInfo(detail::DrawElementList* oenerList, detail::CombinedMaterial* material, detail::SubsetInfo* outInfo) override
		{
			DrawElement::makeSubsetInfo(oenerList, material, outInfo);

			// MaterialTexture を上書きする
			outInfo->materialTexture = oenerList->getBatch(batchIndex)->state.getBrush()->getTexture();
		}
		virtual void drawSubset(const DrawArgs& e) override
		{
			auto* r = e.context->beginFrameRectRenderer();
			r->draw(getTransform(e.oenerList), rect);
		}
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement("DrawFrameRectangle"); }
	};
	auto* ptr = resolveDrawElement<DrawElement_DrawFrameRectangle>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_frameRectRenderer, nullptr);
	ptr->rect = rect;
	// TODO: カリング
}

//------------------------------------------------------------------------------
void DrawList::renderSubView(RenderView* listSet, detail::SceneRenderer* renderer, RenderTargetTexture* defaultRenderTarget, DepthBuffer* defaultDepthBuffer)
{
	class DrawElement_RenderSubView : public detail::DrawElement
	{
	public:
		RenderView* listSet;
		detail::SceneRenderer* renderer;
		RefPtr<RenderTargetTexture>	defaultRenderTarget;
		RefPtr<DepthBuffer> defaultDepthBuffer;

		virtual void drawSubset(const DrawArgs& e) override
		{
			// それぞれ、省略されていれば親のを使う
			detail::SceneRenderer* primRenderer = (renderer != nullptr) ? renderer : e.renderer;
			RenderTargetTexture* primRenderTarget = (defaultRenderTarget != nullptr) ? defaultRenderTarget : e.defaultRenderTarget;
			DepthBuffer* primDepthBuffer = (defaultDepthBuffer != nullptr) ? defaultDepthBuffer : e.defaultDepthBuffer;

			// TODO: scoped change block
			auto* status = e.context->getCurrentStatus();

			primRenderer->render(listSet, primRenderTarget, primDepthBuffer, e.diag);

			// ステート復帰
			e.context->applyStatus(status, { e.defaultRenderTarget, e.defaultDepthBuffer });
		}
		virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement("RenderSubView"); }
	};

	// TODO: m_frameRectRenderer は違う気がする・・・
	auto* e = resolveDrawElement<DrawElement_RenderSubView>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_frameRectRenderer, nullptr);
	//e->elementList = listSet->m_lists[0];
	//e->cameraInfo = listSet->m_cameraInfo;
	e->listSet = listSet;
	e->renderer = renderer;
	e->defaultRenderTarget = defaultRenderTarget;
	e->defaultDepthBuffer = defaultDepthBuffer;
}

LN_NAMESPACE_END

