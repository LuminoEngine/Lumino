
#include "../Internal.h"
#include <float.h>
#include <Lumino/Graphics/Color.h>
#include <Lumino/Rendering/Rendering.h>
#include <Lumino/Graphics/ContextInterface.h>
#include <Lumino/Mesh/Mesh.h>
#include "../Graphics/Device/GraphicsDriverInterface.h"
#include "../Graphics/GraphicsManager.h"
#include "../Graphics/RendererImpl.h"
#include "../Graphics/Text/TextRenderer.h"
#include "../Graphics/Text/FontManager.h"
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
Details::Renderer* InternalContext::GetRenderStateManager()
{
	return m_baseRenderer;
}

//------------------------------------------------------------------------------
Details::Renderer* InternalContext::BeginBaseRenderer()
{
	SwitchActiveRenderer(m_baseRenderer);
	return m_baseRenderer;
}

//------------------------------------------------------------------------------
PrimitiveRenderFeature* InternalContext::BeginPrimitiveRenderer()
{
	SwitchActiveRenderer(m_primitiveRenderer);
	return m_primitiveRenderer;
}

//------------------------------------------------------------------------------
BlitRenderer* InternalContext::BeginBlitRenderer()
{
	SwitchActiveRenderer(m_blitRenderer);
	return m_blitRenderer;
}

//------------------------------------------------------------------------------
MeshRenderFeature* InternalContext::BeginMeshRenderer()
{
	SwitchActiveRenderer(m_meshRenderer);
	return m_meshRenderer;
}

//------------------------------------------------------------------------------
SpriteRenderFeature* InternalContext::BeginSpriteRenderer()
{
	SwitchActiveRenderer(m_spriteRenderer);
	return m_spriteRenderer;
}

//------------------------------------------------------------------------------
TextRenderer* InternalContext::BeginTextRenderer()
{
	SwitchActiveRenderer(m_textRenderer);
	return m_textRenderer;
}

//------------------------------------------------------------------------------
VectorTextRenderer* InternalContext::BeginVectorTextRenderer()
{
	SwitchActiveRenderer(m_vectorTextRenderer);
	return m_vectorTextRenderer;
}

//------------------------------------------------------------------------------
ShapesRenderFeature* InternalContext::BeginShapesRenderer()
{
	SwitchActiveRenderer(m_shapesRenderer);
	return m_shapesRenderer;
}

//------------------------------------------------------------------------------
NanoVGRenderFeature* InternalContext::BeginNanoVGRenderer()
{
	SwitchActiveRenderer(m_nanoVGRenderer);
	return m_nanoVGRenderer;
}

//------------------------------------------------------------------------------
FrameRectRenderFeature* InternalContext::BeginFrameRectRenderer()
{
	SwitchActiveRenderer(m_frameRectRenderer);
	return m_frameRectRenderer;
}

//------------------------------------------------------------------------------
void InternalContext::SetViewInfo(const Size& viewPixelSize, const Matrix& viewMatrix, const Matrix& projMatrix)
{
	m_spriteRenderer->SetViewInfo(viewPixelSize, viewMatrix, projMatrix);
	m_textRenderer->SetViewInfo(viewMatrix * projMatrix, SizeI(viewPixelSize.width, viewPixelSize.height));
	m_frameRectRenderer->SetViewInfo(viewMatrix * projMatrix);
}

//------------------------------------------------------------------------------
SpriteRenderFeature* InternalContext::GetSpriteRenderer()
{
	return m_spriteRenderer;
}

//------------------------------------------------------------------------------
void InternalContext::ApplyStatus(DrawElementBatch* state, const DefaultStatus& defaultStatus)
{
	m_currentStatePtr = state;
	m_currentStatePtr->ApplyStatus(this, defaultStatus);

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
void InternalContext::SwitchActiveRenderer(detail::IRenderFeature* renderer)
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
void BatchState::SetScissorRect(const RectI& scissorRect)
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
void BatchState::SetFont(Font* font)
{
	if (m_font != font)
	{
		m_font = font;
		m_hashDirty = true;
	}
}

//------------------------------------------------------------------------------
Font* BatchState::GetFont() const
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
void BatchState::ApplyStatus(InternalContext* context, CombinedMaterial* combinedMaterial, const DefaultStatus& defaultStatus)
{
	auto* stateManager = context->GetRenderStateManager();

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
		context->GetSpriteRenderer()->SetState(state);
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
void DrawElementBatch::SetTransfrom(const Matrix& value)
{
	if (m_transfrom != value)
	{
		m_transfrom = value;
		m_hashDirty = true;
	}
}

//------------------------------------------------------------------------------
void DrawElementBatch::SetCombinedMaterial(CombinedMaterial* value)
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
void DrawElementBatch::ApplyStatus(InternalContext* context, const DefaultStatus& defaultStatus)
{
	state.ApplyStatus(context, m_combinedMaterial, defaultStatus);
}

//------------------------------------------------------------------------------
size_t DrawElementBatch::getHashCode() const
{
	if (m_hashDirty || state.IsHashDirty())
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
	return oenerList->GetBatch(batchIndex)->GetTransfrom();
}

//------------------------------------------------------------------------------
void DrawElement::MakeElementInfo(DrawElementList* oenerList, const CameraInfo& cameraInfo, ElementInfo* outInfo)
{
	outInfo->viewProjMatrix = &cameraInfo.viewProjMatrix;
	outInfo->WorldMatrix = getTransform(oenerList);
	outInfo->WorldViewProjectionMatrix = outInfo->WorldMatrix * cameraInfo.viewMatrix * cameraInfo.projMatrix;	// TODO: viewProj はまとめたい
	outInfo->affectedLights = GetAffectedDynamicLightInfos();
}

//------------------------------------------------------------------------------
void DrawElement::MakeSubsetInfo(DrawElementList* oenerList, CombinedMaterial* material, SubsetInfo* outInfo)
{
	outInfo->combinedMaterial = material;
	outInfo->materialTexture = (material != nullptr) ? material->m_mainTexture : nullptr;
}

//------------------------------------------------------------------------------
void DrawElement::MakeBoundingSphere(const Vector3& minPos, const Vector3& maxPos)
{
	Vector3 center = minPos + ((maxPos - minPos) / 2);
	boundingSphere.center = center;
	boundingSphere.radius = std::max(Vector3::distance(minPos, center), Vector3::distance(maxPos, center));
}

//------------------------------------------------------------------------------
DynamicLightInfo** DrawElement::GetAffectedDynamicLightInfos()
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
void DrawElementList::PostAddCommandInternal(const BatchState& state, Material* availableMaterial, const Matrix& transform, const BuiltinEffectData& effectData, DrawElement* element)
{
	if (m_batchList.isEmpty() || !m_batchList.getLast().Equal(state, availableMaterial, transform, effectData))
	{
		// CombinedMaterial を作る
		CombinedMaterial* cm = m_combinedMaterialCache.queryCommandList();
		cm->combine(nullptr, availableMaterial, effectData);	// TODO

		// 新しく DrawElementBatch を作る
		m_batchList.add(DrawElementBatch());
		m_batchList.getLast().state = state;
		m_batchList.getLast().SetCombinedMaterial(cm);
		m_batchList.getLast().SetTransfrom(transform);
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
void DrawElementList::AddDynamicLightInfo(DynamicLightInfo* lightInfo)
{
	assert(lightInfo != nullptr);
	m_dynamicLightList.add(lightInfo);
}

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
}

//------------------------------------------------------------------------------
void SceneRenderer::OnPreRender(DrawElementList* elementList)
{
}

//------------------------------------------------------------------------------
void SceneRenderer::AddPass(RenderingPass2* pass)
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
	RenderDiag* diag)
{
	if (diag != nullptr) diag->BeginRenderView();
	if (diag != nullptr) diag->BeginDrawList();

	InternalContext* context = m_manager->getInternalContext();
	const detail::CameraInfo& cameraInfo = drawElementListSet->m_cameraInfo;

	drawElementListSet->m_renderingElementList.clear();

	// Collect
	for (auto& elementList : drawElementListSet->m_lists)
	{
		elementList->SetDefaultRenderTarget(defaultRenderTarget);
		elementList->SetDefaultDepthBuffer(defaultDepthBuffer);

		OnPreRender(elementList);

		// 視点に関する情報の設定
		context->SetViewInfo(cameraInfo.viewPixelSize, cameraInfo.viewMatrix, cameraInfo.projMatrix);

		// ライブラリ外部への書き込み対応
		//context->BeginBaseRenderer()->Clear(ClearFlags::Depth/* | ClearFlags::Stencil*/, Color());

		// 視錘台カリング
		for (int i = 0; i < elementList->getElementCount(); ++i)
		{
			DrawElement* element = elementList->getElement(i);
			Sphere boundingSphere = element->GetBoundingSphere();

			if (boundingSphere.radius < 0 ||	// マイナス値なら視錐台と衝突判定しない
				cameraInfo.viewFrustum.intersects(boundingSphere.center, boundingSphere.radius))
			{
				// このノードは描画できる
				drawElementListSet->m_renderingElementList.add(element);

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
						transform.getFront());		// 平面と点の距離
													// TODO: ↑第2引数違くない？要確認
					break;
				default:
					element->zDistance = 0.0f;
					break;
				}
			}
		}
	}

	// Prepare
	{
		// 距離は降順。遠いほうを先に描画する
		// 優先度は昇順。高いほうを手前に描画する (UE4 ESceneDepthPriorityGroup)
		// フェンスID は昇順。高いほうを後に描画する
		std::stable_sort(
			drawElementListSet->m_renderingElementList.begin(), drawElementListSet->m_renderingElementList.end(),
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

	DrawElement::DrawArgs drawArgs;
	//drawArgs.oenerList = elementList;
	drawArgs.context = context;
	drawArgs.renderer = this;
	drawArgs.defaultRenderTarget = defaultRenderTarget;
	drawArgs.defaultDepthBuffer = defaultDepthBuffer;
	drawArgs.diag = diag;

	for (RenderingPass2* pass : m_renderingPassList)
	{
		// DrawElement 描画
		//int currentBatchIndex = -1;
		DrawElementBatch* currentState = nullptr;
		//Shader* currentShader = nullptr;
		for (DrawElement* element : drawElementListSet->m_renderingElementList)
		{
			bool visible = true;
			drawArgs.oenerList = element->m_ownerDrawElementList;

			DrawElementBatch* batch = element->m_ownerDrawElementList->GetBatch(element->batchIndex);

			// ステートの変わり目チェック
			//if (element->batchIndex != currentBatchIndex)
			if (currentState == nullptr || currentState->getHashCode() != batch->getHashCode())
			{
				context->flush();
				//currentBatchIndex = element->batchIndex;
				currentState = batch;
				context->ApplyStatus(currentState, { defaultRenderTarget, defaultDepthBuffer });
				if (diag != nullptr) diag->ChangeRenderStage();
			}

			// 固定の内部シェーダを使わない場合はいろいろ設定する
			if (!currentState->IsStandaloneShaderRenderer())
			{
				CombinedMaterial* material = currentState->GetCombinedMaterial();
				ElementRenderingPolicy policy;
				pass->SelectElementRenderingPolicy(element, material, &policy);
				visible = policy.visible;

				if (visible)
				{
					Shader* shader = policy.shader;

					ElementInfo elementInfo;
					element->MakeElementInfo(element->m_ownerDrawElementList, cameraInfo, &elementInfo);

					SubsetInfo subsetInfo;
					element->MakeSubsetInfo(element->m_ownerDrawElementList, material, &subsetInfo);

					shader->getSemanticsManager()->updateCameraVariables(cameraInfo);
					shader->getSemanticsManager()->updateElementVariables(elementInfo);
					shader->getSemanticsManager()->updateSubsetVariables(subsetInfo);

					material->applyUserShaderValeues(shader);

					auto* stateManager = context->GetRenderStateManager();
					ShaderPass* pass = shader->getTechniques().getAt(0)->getPasses().getAt(0);	// TODO: DrawList の実行者によって決定する
					stateManager->setShaderPass(pass);
				}
			}

			// 描画実行
			if (visible)
			{
				if (diag != nullptr) element->ReportDiag(diag);
				element->DrawSubset(drawArgs);
			}
		}

		context->flush();
	}

	// Flush
	{
		m_manager->getInternalContext()->flush();
	}

	if (diag != nullptr) diag->EndDrawList();
	if (diag != nullptr) diag->EndRenderView();
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

	auto pass = RefPtr<detail::NonShadingRenderingPass>::makeRef();
	pass->initialize(manager);
	AddPass(pass);
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

//------------------------------------------------------------------------------
Shader* NonShadingRenderingPass::GetDefaultShader() const
{
	return m_defaultShader;
}



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

	auto pass = RefPtr<detail::ForwardShadingRenderingPass>::makeRef();
	pass->initialize(manager);
	AddPass(pass);
}

//------------------------------------------------------------------------------
void ForwardShadingRenderer::OnPreRender(DrawElementList* elementList)
{
	auto& lights = elementList->GetDynamicLightList();
	m_selectingLights.clear();
	for (DynamicLightInfo* light : lights)
	{
		m_selectingLights.add(light);
	}

	for (int i = 0; i < elementList->getElementCount(); i++)
	{
		DrawElement* element = elementList->getElement(i);
		DynamicLightInfo** lightInfos = element->GetAffectedDynamicLightInfos();
		if (lightInfos != nullptr)
		{
			UpdateAffectLights(element, elementList);
		}
	}
}

//------------------------------------------------------------------------------
void ForwardShadingRenderer::UpdateAffectLights(DrawElement* element, DrawElementList* elementList)
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
			light->tempDistance = Vector3::distanceSquared(element->getTransform(elementList).getPosition(), light->transform.getPosition());
		}

		// ソート (昇順)
		std::stable_sort(m_selectingLights.begin(), m_selectingLights.end(), [](DynamicLightInfo* lhs, DynamicLightInfo* rhs) { return lhs->tempDistance < rhs->tempDistance; });
	}

	// 出力 (足りない分は nullptr で埋める)
	DynamicLightInfo** affectLightList = element->GetAffectedDynamicLightInfos();
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

//------------------------------------------------------------------------------
Shader* ForwardShadingRenderingPass::GetDefaultShader() const
{
	return m_defaultShader;
}


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
RefPtr<CombinedMaterial> CombinedMaterialCache::createObject()
{
	auto m = RefPtr<CombinedMaterial>::makeRef();
	return m;
}


//==============================================================================
// ScopedStateBlock2
//==============================================================================

//------------------------------------------------------------------------------
ScopedStateBlock2::ScopedStateBlock2(DrawList* renderer)
	: m_renderer(renderer)
	, m_state(renderer->getState())
{}

//------------------------------------------------------------------------------
ScopedStateBlock2::~ScopedStateBlock2()
{
	apply();
}

//------------------------------------------------------------------------------
void ScopedStateBlock2::apply()
{
	m_renderer->SetState(m_state);
}






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
void RenderingPass2::SelectElementRenderingPolicy(DrawElement* element, CombinedMaterial* material, ElementRenderingPolicy* outPolicy)
{
	outPolicy->shader = nullptr;
	if (material != nullptr && material->m_shader != nullptr)
	{
		outPolicy->shader = material->m_shader;
	}
	else
	{
		outPolicy->shader = GetDefaultShader();
	}

	// とありあえず全部可
	outPolicy->visible = true;
}

////------------------------------------------------------------------------------
//void RenderingPass2::initialize(GraphicsManager* manager)
//{
//	m_defaultShader = manager->GetDefaultShader(DefaultShader::NoLightingRendering);
//}
//
////------------------------------------------------------------------------------
//Shader* RenderingPass2::GetDefaultShader() const
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





//==============================================================================
class ClearElement : public DrawElement
{
public:
	ClearFlags flags;
	Color color;
	float z;
	uint8_t stencil;

	virtual void DrawSubset(const DrawArgs& e) override
	{
		e.context->BeginBaseRenderer()->clear(flags, color, z, stencil);
	}

	virtual void ReportDiag(RenderDiag* diag) override { diag->CallCommonElement("Clear"); }
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
void RenderDiag::BeginRenderView()
{
	Instantiate<detail::RenderDiagItem_BeginRenderView>("BeginRenderView")->m_subType = RenderDiagItem::SubType::ScopeBegin;
}

//------------------------------------------------------------------------------
void RenderDiag::EndRenderView()
{
	Instantiate<detail::RenderDiagItem_EndRenderView>("EndRenderView")->m_subType = RenderDiagItem::SubType::ScopeEnd;
}

//------------------------------------------------------------------------------
void RenderDiag::BeginDrawList()
{
	Instantiate<detail::RenderDiagItem_BeginDrawList>("BeginDrawList")->m_subType = RenderDiagItem::SubType::ScopeBegin;
}

//------------------------------------------------------------------------------
void RenderDiag::EndDrawList()
{
	Instantiate<detail::RenderDiagItem_EndDrawList>("EndDrawList")->m_subType = RenderDiagItem::SubType::ScopeEnd;
}

//------------------------------------------------------------------------------
void RenderDiag::ChangeRenderStage()
{
	Instantiate<detail::RenderDiagItem_EndDrawList>("ChangeRenderStage");
}

//------------------------------------------------------------------------------
void RenderDiag::CallCommonElement(const TCHAR* typeName)
{
	Instantiate<detail::RenderDiagItem_Common>(typeName);
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
	m_state.state.state.setRenderTarget(index, renderTarget);
	m_currentStateFence++;
}

//------------------------------------------------------------------------------
RenderTargetTexture* DrawList::getRenderTarget(int index) const
{
	return m_state.state.state.getRenderTarget(index);
}

//------------------------------------------------------------------------------
void DrawList::setDepthBuffer(DepthBuffer* depthBuffer)
{
	m_state.state.state.setDepthBuffer(depthBuffer);
	m_currentStateFence++;
}

//------------------------------------------------------------------------------
DepthBuffer* DrawList::getDepthBuffer() const
{
	return m_state.state.state.getDepthBuffer();
}

//------------------------------------------------------------------------------
void DrawList::setBrush(Brush* brush)
{
	brush = (brush != nullptr) ? brush : Brush::Black;
	m_state.state.state.setBrush(brush);
}

//------------------------------------------------------------------------------
Brush* DrawList::getBrush() const
{
	return m_state.state.state.getBrush();
}

//------------------------------------------------------------------------------
void DrawList::SetFont(Font* font)
{
	font = (font != nullptr) ? font : m_manager->getFontManager()->GetDefaultFont();

	m_state.state.state.SetFont(font);
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
	m_state.state.state.setBlendMode(mode);
}

//------------------------------------------------------------------------------
void DrawList::setCullingMode(CullingMode mode)
{
	m_state.state.state.setCullingMode(mode);
}

//------------------------------------------------------------------------------
//void DrawList::setOpacity(float opacity)
//{
//	m_state.state.
//}

//------------------------------------------------------------------------------
void DrawList::setDepthTestEnabled(bool enabled)
{
	m_state.state.state.setDepthTestEnabled(enabled);
}

//------------------------------------------------------------------------------
void DrawList::setDepthWriteEnabled(bool enabled)
{
	m_state.state.state.setDepthWriteEnabled(enabled);
}

//------------------------------------------------------------------------------
void DrawList::SetDefaultMaterial(Material* material)
{
	if (LN_CHECK_ARG(material != nullptr)) return;
	m_defaultMaterial = material;
}

//------------------------------------------------------------------------------
void DrawList::SetBuiltinEffectData(const detail::BuiltinEffectData& data)
{
	m_builtinEffectData = data;
}

//------------------------------------------------------------------------------
void DrawList::BeginMakeElements()
{
	m_drawElementList.clearCommands();
	m_state.reset();
	setBrush(nullptr);
	SetFont(nullptr);
	//m_state.state.state.SetFont(m_manager->getFontManager()->GetDefaultFont());
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
void DrawList::SetTransform(const Matrix& transform)
{
	m_state.state.SetTransfrom(transform);
}

//------------------------------------------------------------------------------
void DrawList::clear(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
	auto* ptr = m_drawElementList.addCommand<detail::ClearElement>(m_state.state.state, m_defaultMaterial, Matrix::Identity, m_builtinEffectData);
	ptr->flags = flags;
	ptr->color = color;
	ptr->z = z;
	ptr->stencil = stencil;
	m_currentStateFence++;
}

//------------------------------------------------------------------------------
void DrawList::DrawLinePrimitive(
	const Vector3& position1, const Color& color1,
	const Vector3& position2, const Color& color2)
{
	// TODO: キャッシュと LineStrip
	class DrawElement_DrawLine : public detail::DrawElement
	{
	public:
		Vector3 position1; Color color1;
		Vector3 position2; Color color2;

		virtual void DrawSubset(const DrawArgs& e) override
		{
			e.context->BeginPrimitiveRenderer()->DrawLine(
				position1, color1, position2, color2);
		}
		virtual void ReportDiag(RenderDiag* diag) override { diag->CallCommonElement("DrawLine"); }
	};
	auto* ptr = ResolveDrawElement<DrawElement_DrawLine>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_primitiveRenderer, nullptr);
	ptr->position1 = position1; ptr->color1 = color1;
	ptr->position2 = position2; ptr->color2 = color2;
	ptr->MakeBoundingSphere(Vector3::min(position1, position2), Vector3::max(position1, position2));
}

//------------------------------------------------------------------------------
void DrawList::DrawSquarePrimitive(
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

		virtual void DrawSubset(const DrawArgs& e) override
		{
			e.context->BeginPrimitiveRenderer()->DrawSquare(
				position[0], uv[0], color[0],
				position[1], uv[1], color[1],
				position[2], uv[2], color[2],
				position[3], uv[3], color[3]);
		}
		virtual void ReportDiag(RenderDiag* diag) override { diag->CallCommonElement("DrawSquarePrimitive"); }
	};
	auto* e = ResolveDrawElement<DrawSquarePrimitiveElement>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_primitiveRenderer, nullptr);
	e->position[0] = position1; e->uv[0] = uv1; e->color[0] = color1;
	e->position[1] = position2; e->uv[1] = uv2; e->color[1] = color2;
	e->position[2] = position3; e->uv[2] = uv3; e->color[2] = color3;
	e->position[3] = position4; e->uv[3] = uv4; e->color[3] = color4;
	e->MakeBoundingSphere(Vector3::min(e->position, 4), Vector3::max(e->position, 4));
}

//------------------------------------------------------------------------------
void DrawList::DrawSquare(float sizeX, float sizeZ, int slicesX, int slicesZ, const Color& color, const Matrix& localTransform, Material* material)
{
	class DrawCylinderElement : public detail::LightingDrawElement	// TODO: LightingDrawElement は忘れやすい。デフォルトありでいいと思う
	{
	public:
		detail::PlaneMeshFactory3 factory;

		virtual void DrawSubset(const DrawArgs& e) override
		{
			auto* r = e.context->BeginPrimitiveRenderer();
			r->DrawMeshFromFactory(factory, detail::PrimitiveRendererMode::TriangleList);
		}
		virtual void ReportDiag(RenderDiag* diag) override { diag->CallCommonElement("DrawCylinderElement"); }
	};
	auto* e = ResolveDrawElement<DrawCylinderElement>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_primitiveRenderer, material);
	e->factory.initialize(Vector2(sizeX, sizeZ), slicesX, slicesZ, color, localTransform);
	e->boundingSphere.center = Vector3::Zero;
	e->boundingSphere.radius = Vector3(sizeX, sizeZ, 0).getLength();
}

//------------------------------------------------------------------------------
void DrawList::DrawArc(float startAngle, float endAngle, float innerRadius, float outerRadius, int slices, const Color& color, const Matrix& localTransform, Material* material)
{
	class DrawArcElement : public detail::LightingDrawElement	// TODO: LightingDrawElement は忘れやすい。デフォルトありでいいと思う
	{
	public:
		detail::ArcMeshFactory factory;

		virtual void DrawSubset(const DrawArgs& e) override
		{
			auto* r = e.context->BeginPrimitiveRenderer();
			r->DrawMeshFromFactory(factory, detail::PrimitiveRendererMode::TriangleList);
		}
		virtual void ReportDiag(RenderDiag* diag) override { diag->CallCommonElement("DrawArcElement"); }
	};
	auto* e = ResolveDrawElement<DrawArcElement>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_primitiveRenderer, material);
	e->factory.initialize(startAngle, endAngle, innerRadius, outerRadius, slices, color, localTransform);
	e->boundingSphere.center = Vector3::Zero;
	e->boundingSphere.radius = outerRadius;
}

//------------------------------------------------------------------------------
void DrawList::DrawBox(const Box& box, const Color& color, const Matrix& localTransform, Material* material)
{
	if (box.center != Vector3::Zero) LN_NOTIMPLEMENTED();

	class DrawBoxElement : public detail::LightingDrawElement	// TODO: LightingDrawElement は忘れやすい。デフォルトありでいいと思う
	{
	public:
		detail::RegularBoxMeshFactory factory;

		virtual void DrawSubset(const DrawArgs& e) override
		{
			auto* r = e.context->BeginPrimitiveRenderer();
			r->DrawMeshFromFactory(factory, detail::PrimitiveRendererMode::TriangleList);
		}
		virtual void ReportDiag(RenderDiag* diag) override { diag->CallCommonElement("DrawBoxElement"); }
	};
	auto* e = ResolveDrawElement<DrawBoxElement>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_primitiveRenderer, material);
	e->factory.initialize(Vector3(box.width, box.height, box.depth), color, localTransform);

	Vector3 min, max;
	box.getMinMax(&min, &max);
	e->MakeBoundingSphere(min, max);
}

//------------------------------------------------------------------------------
void DrawList::DrawSphere(float radius, int slices, int stacks, const Color& color, const Matrix& localTransform)
{
	class DrawSphereElement : public detail::LightingDrawElement	// TODO: LightingDrawElement は忘れやすい。デフォルトありでいいと思う
	{
	public:
		detail::RegularSphereMeshFactory factory;

		virtual void DrawSubset(const DrawArgs& e) override
		{
			auto* r = e.context->BeginPrimitiveRenderer();
			r->DrawMeshFromFactory(factory, detail::PrimitiveRendererMode::TriangleList);
		}
		virtual void ReportDiag(RenderDiag* diag) override { diag->CallCommonElement("DrawSphereElement"); }
	};
	auto* e = ResolveDrawElement<DrawSphereElement>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_primitiveRenderer, nullptr);
	e->factory.initialize(radius, slices, stacks, color, localTransform);
	e->boundingSphere.center = Vector3::Zero;
	e->boundingSphere.radius = radius;
}

//------------------------------------------------------------------------------
void DrawList::DrawCylinder(float radius, float	height, int slices, int stacks, const Color& color, const Matrix& localTransform)
{
	class DrawCylinderElement : public detail::LightingDrawElement	// TODO: LightingDrawElement は忘れやすい。デフォルトありでいいと思う
	{
	public:
		detail::RegularCylinderMeshFactory factory;

		virtual void DrawSubset(const DrawArgs& e) override
		{
			auto* r = e.context->BeginPrimitiveRenderer();
			r->DrawMeshFromFactory(factory, detail::PrimitiveRendererMode::TriangleList);
		}
		virtual void ReportDiag(RenderDiag* diag) override { diag->CallCommonElement("DrawCylinder"); }
	};
	auto* e = ResolveDrawElement<DrawCylinderElement>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_primitiveRenderer, nullptr);
	e->factory.initialize(radius, height, slices, stacks, color, localTransform);
	e->boundingSphere.center = Vector3::Zero;
	e->boundingSphere.radius = Vector3(radius, height, 0).getLength();
}

//------------------------------------------------------------------------------
void DrawList::DrawCone(float radius, float height, int slices, const Color& color, const Matrix& localTransform)
{
	class DrawConeElement : public detail::LightingDrawElement	// TODO: LightingDrawElement は忘れやすい。デフォルトありでいいと思う
	{
	public:
		detail::RegularConeMeshFactory factory;

		virtual void DrawSubset(const DrawArgs& e) override
		{
			auto* r = e.context->BeginPrimitiveRenderer();
			r->DrawMeshFromFactory(factory, detail::PrimitiveRendererMode::TriangleList);
		}
		virtual void ReportDiag(RenderDiag* diag) override { diag->CallCommonElement("DrawCone"); }
	};
	auto* e = ResolveDrawElement<DrawConeElement>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_primitiveRenderer, nullptr);
	e->factory.initialize(radius, height, slices, color, localTransform);
	e->boundingSphere.center = Vector3::Zero;
	e->boundingSphere.radius = Vector3(radius, height, 0).getLength();
}

//------------------------------------------------------------------------------
void DrawList::DrawMesh(MeshResource* mesh, int subsetIndex, Material* material)
{
	DrawMeshResourceInternal(mesh, subsetIndex, material);
}

//------------------------------------------------------------------------------
//void DrawList::DrawMesh(StaticMeshModel* mesh, int subsetIndex, Material* material)
//{
//	DrawMeshSubsetInternal(mesh, subsetIndex, material);
//}

//------------------------------------------------------------------------------
void DrawList::blit(Texture* source)
{
	BlitInternal(source, nullptr, Matrix::Identity, nullptr);
}

//------------------------------------------------------------------------------
void DrawList::blit(Texture* source, const Matrix& transform)
{
	BlitInternal(source, nullptr, transform, nullptr);
}

//------------------------------------------------------------------------------
void DrawList::blit(Texture* source, RenderTargetTexture* dest, const Matrix& transform)
{
	BlitInternal(source, dest, transform, nullptr);
}

//------------------------------------------------------------------------------
void DrawList::blit(Texture* source, RenderTargetTexture* dest, Material* material)
{
	BlitInternal(source, dest, Matrix::Identity, material);
}

//------------------------------------------------------------------------------
void DrawList::DrawGlyphRun(const PointF& position, GlyphRun* glyphRun)
{
	class DrawElement_DrawGlyphRun : public detail::DrawElement
	{
	public:
		RefPtr<GlyphRun>	glyphRun;
		PointF position;

		virtual void DrawSubset(const DrawArgs& e) override
		{
			e.context->BeginTextRenderer()->DrawGlyphRun(getTransform(e.oenerList), position, glyphRun);
		}
		virtual void ReportDiag(RenderDiag* diag) override { diag->CallCommonElement("DrawGlyphRun"); }
	};

	auto* e = ResolveDrawElement<DrawElement_DrawGlyphRun>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_textRenderer, nullptr);
	e->glyphRun = glyphRun;
	e->position = position;
	//e->boundingSphere = ;	// TODO
}

//------------------------------------------------------------------------------
void DrawList::DrawText_(const StringRef& text, const PointF& position)
{
	DrawText_(text, Rect(position, FLT_MAX, FLT_MAX), StringFormatFlags::LeftAlignment);
}

//------------------------------------------------------------------------------
void DrawList::DrawText_(const StringRef& text, const Rect& rect, StringFormatFlags flags)
{
	class DrawElement_DrawText : public detail::DrawElement
	{
	public:
		String text;	// TODO: BlukData
		Rect rect;
		StringFormatFlags flags;

		virtual void DrawSubset(const DrawArgs& e) override
		{
			e.context->BeginTextRenderer()->DrawString(getTransform(e.oenerList), text.c_str(), text.getLength(), rect, flags);
		}
		virtual void ReportDiag(RenderDiag* diag) override { diag->CallCommonElement("DrawText"); }
	};

	auto* e = ResolveDrawElement<DrawElement_DrawText>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_textRenderer, nullptr);
	e->text = text;
	e->rect = rect;
	e->flags = flags;
	//e->boundingSphere = ;	// TODO
}

//------------------------------------------------------------------------------
void DrawList::DrawChar(TCHAR ch, const PointF& position)
{
	class DrawElement_DrawChar : public detail::DrawElement
	{
	public:
		TCHAR ch;
		PointF position;

		virtual void DrawSubset(const DrawArgs& e) override
		{
			e.context->BeginVectorTextRenderer()->DrawChar(getTransform(e.oenerList), ch, Rect(position, 0, 0), TextLayoutOptions::None);
		}
		virtual void ReportDiag(RenderDiag* diag) override { diag->CallCommonElement("DrawChar"); }
	};

	// TODO: UTF32 変換

	auto* e = ResolveDrawElement<DrawElement_DrawChar>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_vectorTextRenderer, nullptr);
	e->ch = ch;
	e->position = position;
	//e->boundingSphere = ;	// TODO
}

//------------------------------------------------------------------------------
void DrawList::DrawText2(const StringRef& text, const Rect& rect)
{
	class DrawElement_DrawString : public detail::DrawElement
	{
	public:
		detail::CommandDataCache::DataHandle utf32DataHandle;
		int length;
		Rect rect;

		virtual void DrawSubset(const DrawArgs& e) override
		{
			e.context->BeginVectorTextRenderer()->DrawString(
				getTransform(e.oenerList), 
				(const UTF32*)e.oenerList->getExtData(utf32DataHandle),
				length,
				rect,
				TextLayoutOptions::None);
		}
		virtual void ReportDiag(RenderDiag* diag) override { diag->CallCommonElement("DrawString"); }
	};

	const ByteBuffer& utf32Data = m_manager->getFontManager()->getTCharToUTF32Converter()->convert(text.getBegin(), text.getLength() * sizeof(TCHAR));

	auto* e = ResolveDrawElement<DrawElement_DrawString>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_vectorTextRenderer, nullptr);
	e->utf32DataHandle = m_drawElementList.allocExtData(utf32Data.getSize());
	e->length = utf32Data.getSize() / sizeof(UTF32);
	e->rect = rect;
	//e->boundingSphere = ;	// TODO

	memcpy(m_drawElementList.getExtData(e->utf32DataHandle), utf32Data.getConstData(), utf32Data.getSize());
}

//------------------------------------------------------------------------------
void DrawList::DrawSprite(
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

		virtual void DrawSubset(const DrawArgs& e) override
		{
			auto* r = e.context->BeginSpriteRenderer();
			r->SetTransform(getTransform(e.oenerList));
			r->DrawRequest(position, size, anchorRatio, texture, srcRect, color, baseDirection, billboardType);
		}
		virtual void ReportDiag(RenderDiag* diag) override { diag->CallCommonElement("DrawSprite"); }
	};

	auto* ptr = ResolveDrawElement<DrawElement_DrawSprite>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_spriteRenderer, material);
	ptr->position = position;
	ptr->size.set(size.width, size.height);
	ptr->anchorRatio = anchor;
	ptr->texture = texture;
	ptr->srcRect = srcRect;
	ptr->color = color;
	ptr->baseDirection = baseDirection;
	ptr->billboardType = billboardType;
	detail::SpriteRenderFeature::MakeBoundingSphere(ptr->size, baseDirection, &ptr->boundingSphere);
	ptr->boundingSphere.center += m_state.state.GetTransfrom().getPosition();	// TODO: 他と共通化
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

	virtual void DrawSubset(const DrawArgs& e) override
	{
		auto* r = e.context->BeginNanoVGRenderer();
		//auto cl = r->TakeCommandList();
		//detail::NanoVGCommandHelper::nvgRect(cl, rect.x, rect.y, rect.width, rect.height);
		//detail::NanoVGCommandHelper::nvgFill(cl);
		r->ExecuteCommand(m_commandList);
		m_commandList = nullptr;
	}
	virtual void ReportDiag(RenderDiag* diag) override { diag->CallCommonElement("DrawNanoVGCommands"); }
};

void DrawList::DrawRectangle(const Rect& rect)
{
	if (m_state.state.state.getBrush() != nullptr &&
		(m_state.state.state.getBrush()->getImageDrawMode() == BrushImageDrawMode::BoxFrame || m_state.state.state.getBrush()->getImageDrawMode() == BrushImageDrawMode::BorderFrame))
	{
		DrawFrameRectangle(rect);
		return;
	}

	auto* ptr = ResolveDrawElement<DrawElement_DrawNanoVGCommands>(detail::DrawingSectionId::NanoVG, m_manager->getInternalContext()->m_nanoVGRenderer, nullptr);
	auto* list = ptr->GetGCommandList(this);
	detail::NanoVGCommandHelper::nvgBeginPath(list);
	detail::NanoVGCommandHelper::nvgRect(list, rect.x, rect.y, rect.width, rect.height);
	detail::NanoVGCommandHelper::nvgFill(list);
	//ptr->rect = rect;
	// TODO: カリング
	
}

void DrawList::DrawScreenRectangle()
{
	blit(nullptr);
}

//------------------------------------------------------------------------------
void DrawList::AddDynamicLightInfo(detail::DynamicLightInfo* lightInfo)
{
	m_drawElementList.AddDynamicLightInfo(lightInfo);
}

//------------------------------------------------------------------------------
void DrawList::PushMetadata(const DrawElementMetadata* metadata)
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
void DrawList::PopMetadata()
{
	m_metadata = nullptr;
}
//
////------------------------------------------------------------------------------
//template<typename TElement>
//TElement* DrawList::ResolveDrawElement(detail::DrawingSectionId sectionId, detail::IRendererPloxy* renderer, Material* userMaterial)
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
//		m_drawElementList.GetBatch(m_currentSectionTopElement->batchIndex)->Equal(m_state.state.state, availableMaterial, m_state.state.GetTransfrom()))
//	{
//		return static_cast<TElement*>(m_currentSectionTopElement);
//	}
//
//	// DrawElement を新しく作る
//	TElement* element = m_drawElementList.addCommand<TElement>(m_state.state.state, availableMaterial, m_state.state.GetTransfrom());
//	//element->OnJoindDrawList(m_state.transfrom);
//	element->drawingSectionId = sectionId;
//	element->metadata = *metadata;
//	m_currentSectionTopElement = element;
//	return element;
//}

//------------------------------------------------------------------------------
void DrawList::DrawMeshResourceInternal(MeshResource* mesh, int subsetIndex, Material* material)
{
	class DrawElement_DrawMeshResourceInternal : public detail::LightingDrawElement
	{
	public:
		RefPtr<MeshResource>	mesh;
		int startIndex;
		int primitiveCount;
		PrimitiveType primitiveType;

		virtual void DrawSubset(const DrawArgs& e) override
		{
			e.context->BeginMeshRenderer()->DrawMesh(mesh, startIndex, primitiveCount, primitiveType);
		}
		virtual void ReportDiag(RenderDiag* diag) override { diag->CallCommonElement("DrawMeshResourceInternal"); }
	};

	MeshAttribute attr;
	mesh->GetMeshAttribute(subsetIndex, &attr);
	if (attr.PrimitiveNum == 0) return;		// not need draw

	auto* e = ResolveDrawElement<DrawElement_DrawMeshResourceInternal>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_meshRenderer, material);
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
//		virtual void DrawSubset(detail::DrawElementList* oenerList, detail::InternalContext* context) override
//		{
//			context->BeginMeshRenderer()->DrawMesh(mesh->GetMeshResource(), startIndex, primitiveCount, primitiveType);
//		}
//	};
//	MeshAttribute attr;
//	mesh->GetMeshResource()->GetMeshAttribute(subsetIndex, &attr);
//	if (attr.PrimitiveNum == 0) return;		// not need draw
//
//	auto* e = ResolveDrawElement<DrawElement_DrawMeshInternal>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_meshRenderer, material);
//	e->subsetIndex = subsetIndex;
//	e->mesh = mesh;
//	e->startIndex = attr.StartIndex;
//	e->primitiveCount = attr.PrimitiveNum;
//	e->primitiveType = attr.primitiveType;
//	//e->boundingSphere = ;	// TODO
//}

//------------------------------------------------------------------------------
void DrawList::BlitInternal(Texture* source, RenderTargetTexture* dest, const Matrix& transform, Material* material)
{

	class DrawElement_BlitInternal : public detail::DrawElement
	{
	public:
		Matrix			overrideTransform;
		RefPtr<Texture>	source;

		virtual void MakeElementInfo(detail::DrawElementList* oenerList, const detail::CameraInfo& cameraInfo, detail::ElementInfo* outInfo) override
		{
			DrawElement::MakeElementInfo(oenerList, cameraInfo, outInfo);
			outInfo->WorldViewProjectionMatrix = overrideTransform;
		}
		virtual void MakeSubsetInfo(detail::DrawElementList* oenerList, detail::CombinedMaterial* material, detail::SubsetInfo* outInfo) override
		{
			DrawElement::MakeSubsetInfo(oenerList, material, outInfo);

			// MaterialTexture を上書きする
			outInfo->materialTexture = source;
		}

		virtual void DrawSubset(const DrawArgs& e) override
		{
			e.context->BeginBlitRenderer()->blit();
		}
		virtual void ReportDiag(RenderDiag* diag) override { diag->CallCommonElement("BlitInternal"); }
	};

	if (dest != nullptr)
	{
		setRenderTarget(0, dest);
	}

	auto* e = ResolveDrawElement<DrawElement_BlitInternal>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_blitRenderer, material);
	e->overrideTransform = transform;
	e->source = source;
}

//------------------------------------------------------------------------------
void DrawList::DrawFrameRectangle(const Rect& rect)
{
	class DrawElement_DrawFrameRectangle : public detail::DrawElement
	{
	public:
		Rect rect;

		virtual void MakeSubsetInfo(detail::DrawElementList* oenerList, detail::CombinedMaterial* material, detail::SubsetInfo* outInfo) override
		{
			DrawElement::MakeSubsetInfo(oenerList, material, outInfo);

			// MaterialTexture を上書きする
			outInfo->materialTexture = oenerList->GetBatch(batchIndex)->state.getBrush()->getTexture();
		}
		virtual void DrawSubset(const DrawArgs& e) override
		{
			auto* r = e.context->BeginFrameRectRenderer();
			r->Draw(getTransform(e.oenerList), rect);
		}
		virtual void ReportDiag(RenderDiag* diag) override { diag->CallCommonElement("DrawFrameRectangle"); }
	};
	auto* ptr = ResolveDrawElement<DrawElement_DrawFrameRectangle>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_frameRectRenderer, nullptr);
	ptr->rect = rect;
	// TODO: カリング
}

//------------------------------------------------------------------------------
void DrawList::RenderSubView(RenderView* listSet, detail::SceneRenderer* renderer, RenderTargetTexture* defaultRenderTarget, DepthBuffer* defaultDepthBuffer)
{
	class DrawElement_RenderSubView : public detail::DrawElement
	{
	public:
		RenderView* listSet;
		detail::SceneRenderer* renderer;
		RefPtr<RenderTargetTexture>	defaultRenderTarget;
		RefPtr<DepthBuffer> defaultDepthBuffer;

		virtual void DrawSubset(const DrawArgs& e) override
		{
			// それぞれ、省略されていれば親のを使う
			detail::SceneRenderer* primRenderer = (renderer != nullptr) ? renderer : e.renderer;
			RenderTargetTexture* primRenderTarget = (defaultRenderTarget != nullptr) ? defaultRenderTarget : e.defaultRenderTarget;
			DepthBuffer* primDepthBuffer = (defaultDepthBuffer != nullptr) ? defaultDepthBuffer : e.defaultDepthBuffer;

			// TODO: scoped change block
			auto* status = e.context->GetCurrentStatus();

			primRenderer->render(listSet, primRenderTarget, primDepthBuffer, e.diag);

			// ステート復帰
			e.context->ApplyStatus(status, { e.defaultRenderTarget, e.defaultDepthBuffer });
		}
		virtual void ReportDiag(RenderDiag* diag) override { diag->CallCommonElement("RenderSubView"); }
	};

	// TODO: m_frameRectRenderer は違う気がする・・・
	auto* e = ResolveDrawElement<DrawElement_RenderSubView>(detail::DrawingSectionId::None, m_manager->getInternalContext()->m_frameRectRenderer, nullptr);
	//e->elementList = listSet->m_lists[0];
	//e->cameraInfo = listSet->m_cameraInfo;
	e->listSet = listSet;
	e->renderer = renderer;
	e->defaultRenderTarget = defaultRenderTarget;
	e->defaultDepthBuffer = defaultDepthBuffer;
}

LN_NAMESPACE_END

