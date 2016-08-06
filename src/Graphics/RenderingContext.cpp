
#include "Internal.h"
#include "GraphicsManager.h"
#include <Lumino/Graphics/RenderingContext.h>
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/Material.h>
#include "RendererImpl.h"
#include "PrimitiveRenderer.h"
#include "MeshRendererProxy.h"

LN_NAMESPACE_BEGIN

class RenderingContext::RenderStateBlock2
{
public:
	RenderStateBlock2(RenderingContext* context)
		: m_context(context)
		, m_state(context->m_state)
	{}
	~RenderStateBlock2()
	{
		Apply(m_context);
	}

	void Apply(RenderingContext* context)
	{
		context->SetBasicContextState(m_state);
	}

private:
	RenderingContext*			m_context;
	detail::BasicContextState	m_state;
};

//==============================================================================
// RenderingContext
//==============================================================================

//------------------------------------------------------------------------------
RenderingContext::RenderingContext()
	: m_primitiveRenderer(nullptr)
	, m_meshRendererProxy(nullptr)
{
}

//------------------------------------------------------------------------------
RenderingContext::~RenderingContext()
{
	LN_SAFE_RELEASE(m_meshRendererProxy);
	LN_SAFE_RELEASE(m_primitiveRenderer);
}

//------------------------------------------------------------------------------
void RenderingContext::Initialize(GraphicsManager* manager)
{
	detail::ContextInterface::Initialize(manager);

	m_primitiveRenderer = LN_NEW detail::PrimitiveRenderer();
	m_primitiveRenderer->Initialize(manager);
	m_primitiveRenderer->SetUseInternalShader(true);	// TODO

	m_meshRendererProxy = LN_NEW detail::MeshRendererProxy();
	m_meshRendererProxy->Initialize(manager);
}












//------------------------------------------------------------------------------
void RenderingContext::SetRenderState(const RenderState& state)
{
	if (!m_state.renderState.Equals(state))
	{
		NorityStateChanging();
		m_state.renderState = state;
	}
}

void RenderingContext::SetAlphaBlendEnabled(bool enabled)
{
	if (m_state.renderState.alphaBlendEnabled != enabled)
	{
		NorityStateChanging();
		m_state.renderState.alphaBlendEnabled = enabled;
	}
}
void RenderingContext::SetBlendOp(BlendOp op)
{
	if (m_state.renderState.blendOp != op)
	{
		NorityStateChanging();
		m_state.renderState.blendOp = op;
	}
}
void RenderingContext::SetSourceBlend(BlendFactor blend)
{
	if (m_state.renderState.sourceBlend != blend)
	{
		NorityStateChanging();
		m_state.renderState.sourceBlend = blend;
	}
}
void RenderingContext::SetDestinationBlend(BlendFactor blend)
{
	if (m_state.renderState.destinationBlend != blend)
	{
		NorityStateChanging();
		m_state.renderState.destinationBlend = blend;
	}
}
void RenderingContext::SetBlendMode(BlendMode mode)
{
	switch (mode)
	{
		// もっといろいろ http://d.hatena.ne.jp/Ko-Ta/20070618/p1
	case BlendMode::Normal:
		SetAlphaBlendEnabled(false);
		SetBlendOp(BlendOp::Add);
		SetSourceBlend(BlendFactor::One);
		SetDestinationBlend(BlendFactor::Zero);
		break;
	case BlendMode::Alpha:
		SetAlphaBlendEnabled(true);
		SetBlendOp(BlendOp::Add);
		SetSourceBlend(BlendFactor::SourceAlpha);
		SetDestinationBlend(BlendFactor::InverseSourceAlpha);
		break;
	case BlendMode::Add:
		SetAlphaBlendEnabled(true);
		SetBlendOp(BlendOp::Add);
		SetSourceBlend(BlendFactor::SourceAlpha);
		SetDestinationBlend(BlendFactor::One);
		break;
	case BlendMode::AddAlphaDisabled:
		SetAlphaBlendEnabled(true);
		SetBlendOp(BlendOp::Add);
		SetSourceBlend(BlendFactor::One);
		SetDestinationBlend(BlendFactor::One);
		break;
	case BlendMode::Subtract:
		SetAlphaBlendEnabled(true);
		SetBlendOp(BlendOp::ReverseSubtract);
		SetSourceBlend(BlendFactor::SourceAlpha);
		SetDestinationBlend(BlendFactor::One);
		break;
	case BlendMode::SubtractAlphaDisabled:
		SetAlphaBlendEnabled(true);
		SetBlendOp(BlendOp::ReverseSubtract);
		SetSourceBlend(BlendFactor::One);
		SetDestinationBlend(BlendFactor::One);
		break;
	case BlendMode::MultiplyAlphaDisabled:
		SetAlphaBlendEnabled(true);
		SetBlendOp(BlendOp::Add);
		// AlphaDisable (Alpha を別指定できない今の仕様では Alpha を考慮できない)
		SetSourceBlend(BlendFactor::Zero);
		SetDestinationBlend(BlendFactor::SourceColor);
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
void RenderingContext::SetCullingMode(CullingMode mode)
{
	if (m_state.renderState.Culling != mode)
	{
		NorityStateChanging();
		m_state.renderState.Culling = mode;
	}
}
void RenderingContext::SetFillMode(FillMode mode)
{
	if (m_state.renderState.Fill != mode)
	{
		NorityStateChanging();
		m_state.renderState.Fill = mode;
	}
}
void RenderingContext::SetAlphaTestEnabled(bool enabled)
{
	if (m_state.renderState.AlphaTest != enabled)
	{
		NorityStateChanging();
		m_state.renderState.AlphaTest = enabled;
	}
}

//------------------------------------------------------------------------------
const RenderState& RenderingContext::GetRenderState() const
{
	return m_state.renderState;
}

//------------------------------------------------------------------------------
void RenderingContext::SetDepthStencilState(const DepthStencilState& state)
{
	if (!m_state.depthStencilState.Equals(state))
	{
		NorityStateChanging();
		m_state.depthStencilState = state;
	}
}

void RenderingContext::SetDepthTestEnabled(bool enabled)
{
	if (m_state.depthStencilState.DepthTestEnabled != enabled)
	{
		NorityStateChanging();
		m_state.depthStencilState.DepthTestEnabled = enabled;
	}
}
void RenderingContext::SetDepthWriteEnabled(bool enabled)
{
	if (m_state.depthStencilState.DepthWriteEnabled != enabled)
	{
		NorityStateChanging();
		m_state.depthStencilState.DepthWriteEnabled = enabled;
	}
}
void RenderingContext::SetDepthTestFunc(CompareFunc func)
{
	if (m_state.depthStencilState.DepthTestFunc != func)
	{
		NorityStateChanging();
		m_state.depthStencilState.DepthTestFunc = func;
	}
}
void RenderingContext::SetStencilEnabled(bool enabled)
{
	if (m_state.depthStencilState.StencilEnabled != enabled)
	{
		NorityStateChanging();
		m_state.depthStencilState.StencilEnabled = enabled;
	}
}
void RenderingContext::SetStencilFunc(CompareFunc func)
{
	if (m_state.depthStencilState.StencilFunc != func)
	{
		NorityStateChanging();
		m_state.depthStencilState.StencilFunc = func;
	}
}
void RenderingContext::SetStencilReferenceValue(uint8_t value)
{
	if (m_state.depthStencilState.StencilReferenceValue != value)
	{
		NorityStateChanging();
		m_state.depthStencilState.StencilReferenceValue = value;
	}
}
void RenderingContext::SetStencilFailOp(StencilOp op)
{
	if (m_state.depthStencilState.StencilFailOp != op)
	{
		NorityStateChanging();
		m_state.depthStencilState.StencilFailOp = op;
	}
}
void RenderingContext::SetStencilDepthFailOp(StencilOp op)
{
	if (m_state.depthStencilState.StencilDepthFailOp != op)
	{
		NorityStateChanging();
		m_state.depthStencilState.StencilDepthFailOp = op;
	}
}
void RenderingContext::SetStencilPassOp(StencilOp op)
{
	if (m_state.depthStencilState.StencilPassOp != op)
	{
		NorityStateChanging();
		m_state.depthStencilState.StencilPassOp = op;
	}
}
const DepthStencilState& RenderingContext::GetDepthStencilState() const
{
	return m_state.depthStencilState;
}

void RenderingContext::ResetStates()
{
	SetAlphaBlendEnabled(RenderState::Default.alphaBlendEnabled);
	SetBlendOp(RenderState::Default.blendOp);
	SetSourceBlend(RenderState::Default.sourceBlend);
	SetDestinationBlend(RenderState::Default.destinationBlend);
	SetCullingMode(RenderState::Default.Culling);
	SetFillMode(RenderState::Default.Fill);
	SetAlphaTestEnabled(RenderState::Default.alphaBlendEnabled);

	SetDepthTestEnabled(DepthStencilState::Default.DepthTestEnabled);
	SetDepthWriteEnabled(DepthStencilState::Default.DepthWriteEnabled);
	SetDepthTestFunc(DepthStencilState::Default.DepthTestFunc);
	SetStencilEnabled(DepthStencilState::Default.StencilEnabled);
	SetStencilFunc(DepthStencilState::Default.StencilFunc);
	SetStencilReferenceValue(DepthStencilState::Default.StencilReferenceValue);
	SetStencilFailOp(DepthStencilState::Default.StencilFailOp);
	SetStencilDepthFailOp(DepthStencilState::Default.StencilDepthFailOp);
	SetStencilPassOp(DepthStencilState::Default.StencilPassOp);
}

//------------------------------------------------------------------------------
void RenderingContext::SetRenderTarget(int index, Texture* texture)
{
	if (texture != m_state.GetRenderTarget(index))
	{
		NorityStateChanging();
		m_state.SetRenderTarget(index, texture);
	}
}

//------------------------------------------------------------------------------
Texture* RenderingContext::GetRenderTarget(int index) const
{
	return m_state.GetRenderTarget(index);
}

//------------------------------------------------------------------------------
void RenderingContext::SetDepthBuffer(Texture* depthBuffer)
{
	if (m_state.depthBuffer != depthBuffer)
	{
		NorityStateChanging();
		m_state.depthBuffer = depthBuffer;
	}
}

//------------------------------------------------------------------------------
Texture* RenderingContext::GetDepthBuffer() const
{
	return m_state.depthBuffer;
}

//------------------------------------------------------------------------------
void RenderingContext::SetViewport(const Rect& rect)
{
	if (m_state.viewport != rect)
	{
		NorityStateChanging();
		m_state.viewport = rect;
	}
}

//------------------------------------------------------------------------------
const Rect& RenderingContext::GetViewport() const
{
	return m_state.viewport;
}

//------------------------------------------------------------------------------
void RenderingContext::SetShaderPass(ShaderPass* pass)
{
	if (pass != m_state.GetShaderPass() || (pass != nullptr && pass->GetOwnerShader()->IsModifiedVariables()))
	{
		NorityStateChanging();
		m_state.SetShaderPass(pass);
	}
}

//------------------------------------------------------------------------------
ShaderPass* RenderingContext::GetShaderPass() const
{
	return m_state.GetShaderPass();
}

//------------------------------------------------------------------------------
void RenderingContext::PushState()
{
	m_stateStack.push(m_state);
}

//------------------------------------------------------------------------------
void RenderingContext::PopState()
{
	if (!m_state.Equals(m_stateStack.top()))
	{
		NorityStateChanging();
		m_state = m_stateStack.top();
	}
	m_stateStack.pop();
}














//------------------------------------------------------------------------------
void RenderingContext::Clear(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
	NorityStartDrawing(GetBaseRenderer());
	GetBaseRenderer()->Clear(flags, color, z, stencil);
}

//------------------------------------------------------------------------------
void RenderingContext::DrawPrimitive(VertexDeclaration* vertexDeclaration, VertexBuffer* vertexBuffer, PrimitiveType primitive, int startVertex, int primitiveCount)
{
	NorityStartDrawing(GetBaseRenderer());
	GetBaseRenderer()->DrawPrimitive(vertexDeclaration, vertexBuffer, primitive, startVertex, primitiveCount);
}

//------------------------------------------------------------------------------
void RenderingContext::DrawPrimitiveIndexed(VertexDeclaration* vertexDeclaration, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveType primitive, int startIndex, int primitiveCount)
{
	NorityStartDrawing(GetBaseRenderer());
	GetBaseRenderer()->DrawPrimitiveIndexed(vertexDeclaration, vertexBuffer, indexBuffer, primitive, startIndex, primitiveCount);
}

//------------------------------------------------------------------------------
void RenderingContext::DrawLinePrimitive(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor)
{
	NorityStartDrawing(m_primitiveRenderer);
	m_primitiveRenderer->DrawLine(from, fromColor, to, toColor);
}

//------------------------------------------------------------------------------
void RenderingContext::DrawSquarePrimitive(
	const Vector3& position1, const Vector2& uv1, const Color& color1,
	const Vector3& position2, const Vector2& uv2, const Color& color2,
	const Vector3& position3, const Vector2& uv3, const Color& color3,
	const Vector3& position4, const Vector2& uv4, const Color& color4)
{
	NorityStartDrawing(m_primitiveRenderer);
	m_primitiveRenderer->DrawSquare(
		position1, uv1, color1,
		position2, uv2, color2,
		position3, uv3, color3,
		position4, uv4, color4);
}



//------------------------------------------------------------------------------
void RenderingContext::Flush()
{
	detail::ContextInterface::FlushImplemented();
}

//------------------------------------------------------------------------------
void RenderingContext::Blt(Texture* source, RenderTarget* dest)
{
	BltInternal(source, dest, Matrix::Identity, nullptr);
}

//------------------------------------------------------------------------------
void RenderingContext::Blt(Texture* source, RenderTarget* dest, const Matrix& transform)
{
	BltInternal(source, dest, transform, nullptr);
}

//------------------------------------------------------------------------------
void RenderingContext::Blt(Texture* source, RenderTarget* dest, Shader* shader)
{
	BltInternal(source, dest, Matrix::Identity, shader);
}

//------------------------------------------------------------------------------
void RenderingContext::BltInternal(Texture* source, RenderTarget* dest, const Matrix& transform, Shader* shader)
{
	// ここで null にしておかないとPrimitiveRendererが古いシェーダを適用してしまう
	SetShaderPass(nullptr);

	RenderStateBlock2 stateBlock(this);
	SetBlendMode(BlendMode::Alpha);
	SetAlphaTestEnabled(false);
	SetDepthTestEnabled(false);
	SetDepthWriteEnabled(false);

	NorityStartDrawing(m_primitiveRenderer);
	m_primitiveRenderer->Blt(source, dest, transform, shader);
}

//------------------------------------------------------------------------------
void RenderingContext::DrawMesh(StaticMeshModel* mesh, int startIndex, int triangleCount)
{
	NorityStartDrawing(m_meshRendererProxy);
	m_meshRendererProxy->DrawMesh(mesh, startIndex, triangleCount);
}

//------------------------------------------------------------------------------
void RenderingContext::DrawMesh(StaticMeshModel* mesh, int startIndex, int triangleCount, Material* material)
{
	material->ApplyToShaderVariables();

	// TODO: とりあえず 0 番テクニックの全パスで描画する
	// Scene のほうで使っている Script も考慮したカスタマイズをしたい場合、
	// RenderingContext を派生させるのがいいと思う。
	Shader* shader = material->GetShader();
	ShaderTechnique* tech = shader->GetTechniques()[0];
	for (auto* pass : tech->GetPasses())
	{
		SetShaderPass(pass);
		DrawMesh(mesh, startIndex, triangleCount);
	}
}

//------------------------------------------------------------------------------
void RenderingContext::OnStateFlush()
{
	ContextInterface::OnStateFlush();
	SetBasicContextState(m_state);

	auto* pass = m_state.GetShaderPass();
	if (pass != nullptr) GetBaseRenderer()->SetShaderPass(pass);

	const SizeI& size = m_state.GetRenderTarget(0)->GetSize();
	m_primitiveRenderer->SetViewPixelSize(size);
	m_primitiveRenderer->SetUseInternalShader(GetShaderPass() == nullptr);
}

//------------------------------------------------------------------------------
void RenderingContext::OnPrimitiveFlush()
{
	ContextInterface::OnPrimitiveFlush();
}

//------------------------------------------------------------------------------
void RenderingContext::OnShaderVariableModified(ShaderVariable* var)
{
	if (m_state.GetShaderPass() != nullptr && m_state.GetShaderPass()->GetOwnerShader() == var->GetOwnerShader())
	{
		NorityStateChanging();
	}
}

LN_NAMESPACE_END
