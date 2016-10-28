
#include "../Internal.h"
#include <Lumino/Graphics/Color.h>
#include <Lumino/Graphics/Rendering.h>
#include <Lumino/Graphics//ContextInterface.h>
#include "GraphicsManager.h"
#include "RendererImpl.h"
#include "SpriteRenderer.h"

LN_NAMESPACE_BEGIN

namespace detail {


//==============================================================================
// InternalContext
//==============================================================================

//------------------------------------------------------------------------------
InternalContext::InternalContext()
	: m_baseRenderer(nullptr)
	, m_spriteRenderer(nullptr)
	, m_current(nullptr)
{
}

//------------------------------------------------------------------------------
void InternalContext::Initialize(detail::GraphicsManager* manager)
{
	m_baseRenderer = manager->GetRenderer();

	m_spriteRenderer = RefPtr<detail::SpriteRenderer>::MakeRef(manager, 2048);	// TODO
}

//------------------------------------------------------------------------------
Details::Renderer* InternalContext::BeginBaseRenderer()
{
	SwitchActiveRenderer(m_baseRenderer);
	return m_baseRenderer;
}

//------------------------------------------------------------------------------
detail::SpriteRenderer* InternalContext::BeginSpriteRenderer()
{
	SwitchActiveRenderer(m_spriteRenderer);
	return m_spriteRenderer;
}

//------------------------------------------------------------------------------
void InternalContext::SwitchActiveRenderer(detail::IRendererPloxy* renderer)
{
	if (m_current != renderer)
	{
		if (m_current != nullptr) m_current->OnDeactivated();
		m_current = renderer;
		if (m_current != nullptr) m_current->OnActivated();
	}
}

//==============================================================================
// DrawElementBatch
//==============================================================================

//------------------------------------------------------------------------------
bool DrawElementBatch::Equal(const DrawElementBatch& obj) const
{
	if (m_rendererId != obj.m_rendererId) return false;

	if (m_alphaBlendEnabled != obj.m_alphaBlendEnabled) return false;
	if (m_blendMode != obj.m_blendMode) return false;
	if (m_cullingMode != obj.m_cullingMode) return false;
	if (m_alphaTestEnabled != obj.m_alphaTestEnabled) return false;

	if (m_depthTestEnabled != obj.m_depthTestEnabled) return false;
	if (m_depthWriteEnabled != obj.m_depthWriteEnabled) return false;

	if (m_shader != obj.m_shader) return false;
	if (m_shaderValueList.GetCount() != obj.m_shaderValueList.GetCount()) return false;
	for (int i = 0; i < m_shaderValueList.GetCount(); ++i)
	{
		if (m_shaderValueList[i].variable != obj.m_shaderValueList[i].variable) return false;
		if (!m_shaderValueList[i].value.Equals(obj.m_shaderValueList[i].value)) return false;
	}

	for (int i = 0; i < MaxMultiRenderTargets; ++i)
	{
		if (m_renderTargets[i] != obj.m_renderTargets[i]) return false;
	}
	if (m_depthBuffer != obj.m_depthBuffer) return false;
	if (m_scissorRect != obj.m_scissorRect) return false;

	return true;
}

//==============================================================================
// DrawElement
//==============================================================================

//------------------------------------------------------------------------------
DrawElement::DrawElement()
{
}

//------------------------------------------------------------------------------
DrawElement::~DrawElement()
{
}


//==============================================================================
// DrawElementList
//==============================================================================

//------------------------------------------------------------------------------
DrawElementList::DrawElementList()
	: m_commandList()
	, m_commandDataBuffer()
	, m_commandDataBufferUsed(0)
{
	m_commandList.Reserve(512);	// 適当に
	m_commandDataBuffer.Resize(4096, false);
	m_commandDataBufferUsed = 0;
}

//------------------------------------------------------------------------------
DrawElementList::CommandHandle DrawElementList::AllocCommand(size_t byteCount)
{
	// バッファが足りなければ拡張する
	if (m_commandDataBufferUsed + byteCount > m_commandDataBuffer.GetSize())
	{
		size_t newSize = m_commandDataBuffer.GetSize() + std::max(m_commandDataBuffer.GetSize(), byteCount);	// 最低でも byteCount 分を拡張する
		m_commandDataBuffer.Resize(newSize, false);
	}

	//if (copyData != nullptr)
	//{
	//	byte_t* ptr = &(m_commandDataBuffer.GetData()[m_commandDataBufferUsed]);
	//	size_t size = m_commandDataBuffer.GetSize() - m_commandDataBufferUsed;
	//	memcpy_s(ptr, size, copyData, byteCount);
	//}

	size_t dataIdx = m_commandDataBufferUsed;
	m_commandDataBufferUsed += byteCount;

	return dataIdx;
}

//------------------------------------------------------------------------------
detail::DrawElement* DrawElementList::GetCommand(CommandHandle handle)
{
	return (detail::DrawElement*)&(m_commandDataBuffer.GetData()[handle]);
}

//------------------------------------------------------------------------------
void DrawElementList::ClearCommands()
{
	for (CommandHandle handle : m_commandList)
	{
		DrawElement* cmd = GetCommand(handle);
		cmd->~DrawElement();
	}
	m_commandList.Clear();
	m_commandDataBufferUsed = 0;
}

//------------------------------------------------------------------------------
void DrawElementList::PostAddCommandInternal(const DrawElementBatch& state, detail::IRendererPloxy* renderer, DrawElement* element)
{
	if (m_batchList.IsEmpty() || !m_batchList.GetLast().Equal(state))
	{
		element->batchIndex = m_batchList.GetCount();
		m_batchList.Add(state);
		m_batchList.GetLast().m_rendererId = reinterpret_cast<intptr_t>(renderer);
	}
}


//==============================================================================
class ClearElement : public DrawElement
{
public:
	ClearFlags flags;
	Color color;
	float z;
	uint8_t stencil;

	virtual void Execute(InternalContext* context)
	{
		context->BeginBaseRenderer()->Clear(flags, color, z, stencil);
	}
};

//==============================================================================
class DrawSpriteElement : public DrawElement
{
public:
	Vector3 position;
	Vector2 size;
	Vector2 anchorRatio;
	RefPtr<Texture> texture;
	RectF srcRect;
	const Color* colorTable;

	virtual void Execute(InternalContext* context)
	{
		context->BeginSpriteRenderer()->DrawRequest2D(position, size, anchorRatio, texture, srcRect, colorTable);
	}
};

} // namespace detail 

//==============================================================================
// DrawList
//==============================================================================

//------------------------------------------------------------------------------
DrawList::DrawList()
{
}

//------------------------------------------------------------------------------
DrawList::~DrawList()
{
}

//------------------------------------------------------------------------------
void DrawList::Initialize(detail::GraphicsManager* manager)
{
	LN_CHECK_ARG(manager != nullptr);
	m_internalContext.Initialize(manager);
}

//------------------------------------------------------------------------------
void DrawList::SetTransform(const Matrix& transform)
{
	m_state.transfrom = transform;
}

//------------------------------------------------------------------------------
void DrawList::Clear(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
	auto* ptr = m_drawElementList.AddCommand<detail::ClearElement>(m_state.state, m_internalContext.m_baseRenderer);
	ptr->flags = flags;
	ptr->color = color;
	ptr->z = z;
	ptr->stencil = stencil;
}

//------------------------------------------------------------------------------
void DrawList::DrawSprite2D(
	const SizeF& size,
	Texture* texture,
	const RectF& srcRect,
	const Color& color)
{
	//auto* ptr = m_drawElementList.AddCommand<detail::DrawSpriteElement>(m_state.state, m_internalContext.m_spriteRenderer);
	//ptr->position;
	//ptr->size = size;
	//ptr->anchorRatio;
	//ptr->texture = texture;
	//ptr->srcRect = srcRect;
	//ptr->colorTable;
}

LN_NAMESPACE_END

