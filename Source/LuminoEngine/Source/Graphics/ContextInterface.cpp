
#include "Internal.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/VertexBuffer.h>
#include <Lumino/Graphics/IndexBuffer.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/SwapChain.h>
#include <Lumino/Graphics/ContextInterface.h>
#include <Lumino/Graphics/Brush.h>
#include <Lumino/Graphics/Utils.h>
#include "RendererImpl.h"
#include "GraphicsManager.h"
#include "Device/GraphicsDriverInterface.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail
{

//==============================================================================
// ContextState
//==============================================================================


//------------------------------------------------------------------------------
ContextState::ContextState()
	: m_fillBrush(nullptr)
{
}

//------------------------------------------------------------------------------
ContextState::~ContextState()
{
	for (Texture* t : m_renderTargets) {
		LN_SAFE_RELEASE(t);
	}
	LN_SAFE_RELEASE(depthBuffer);
	//LN_SAFE_RELEASE(vertexBuffer);
	//LN_SAFE_RELEASE(indexBuffer);
	LN_SAFE_RELEASE(m_ownerShader);
	LN_SAFE_RELEASE(m_fillBrush);
}

//------------------------------------------------------------------------------
void ContextState::setRenderTarget(int index, Texture* texture)
{
	if (index == 0 && texture == nullptr)
	{
		// index0 は null であってはならない
		LN_THROW(0, ArgumentException);
	}

	if (m_renderTargets[index] != texture)
	{
		LN_REFOBJ_SET(m_renderTargets[index], texture);

		if (index == 0)
		{
			auto& size = texture->getSize();
			viewport.set(0, 0, size.width, size.height);
		}

		modifiedFlags |= ContextStateFlags::CommonState;
	}
}

//------------------------------------------------------------------------------
Texture* ContextState::getRenderTarget(int index) const
{
	return m_renderTargets[index];
}

//------------------------------------------------------------------------------
void ContextState::setShaderPass(ShaderPass* pass)
{
	if (m_shaderPass != pass || (pass != nullptr && pass->getOwnerShader()->isModifiedVariables()))
	{
		if (pass != nullptr)
		{
			LN_REFOBJ_SET(m_ownerShader, pass->getOwnerShader());
			m_shaderPass = pass;
		}
		else
		{
			LN_SAFE_RELEASE(m_ownerShader);
			m_shaderPass = nullptr;
		}
		modifiedFlags |= ContextStateFlags::ShaderPass;
	}
}

//------------------------------------------------------------------------------
void ContextState::setFillBrush(Brush* brush)
{
	LN_REFOBJ_SET(m_fillBrush, brush);
	modifiedFlags |= ContextStateFlags::CommonState;
}

//------------------------------------------------------------------------------
void ContextState::copy(const ContextState& obj)
{

	renderState = obj.renderState;
	depthStencilState = obj.depthStencilState;
	LN_REFOBJ_SET(depthBuffer, obj.depthBuffer);

	viewport = obj.viewport;

	worldTransform = obj.worldTransform;
	viewTransform = obj.viewTransform;
	projectionTransform = obj.projectionTransform;
	modifiedFlags = obj.modifiedFlags;

	for (size_t i = 0; i < m_renderTargets.size(); ++i)
	{
		LN_REFOBJ_SET(m_renderTargets[i], obj.m_renderTargets[i]);
	}

	LN_REFOBJ_SET(m_ownerShader, obj.m_ownerShader);
	LN_REFOBJ_SET(m_shaderPass, obj.m_shaderPass);
	LN_REFOBJ_SET(m_fillBrush, obj.m_fillBrush);
}





//==============================================================================
// BasicContextState
//==============================================================================


//------------------------------------------------------------------------------
void BasicContextState::setRenderTarget(int index, Texture* texture)
{
	if (index == 0 && texture == nullptr)
	{
		// index0 は null であってはならない
		LN_THROW(0, ArgumentException);
	}

	if (m_renderTargets[index] != texture)
	{
		m_renderTargets[index] = texture;

		//if (index == 0)
		//{
		//	auto& size = texture->GetSize();
		//	viewport.Set(0, 0, size.width, size.height);
		//}
	}
}

//------------------------------------------------------------------------------
Texture* BasicContextState::getRenderTarget(int index) const
{
	return m_renderTargets[index];
}

//------------------------------------------------------------------------------
void BasicContextState::setShaderPass(ShaderPass* pass)
{
	if (m_shaderPass != pass || (pass != nullptr && pass->getOwnerShader()->isModifiedVariables()))
	{
		if (pass != nullptr)
		{
			m_ownerShader = pass->getOwnerShader();
			m_shaderPass = pass;
		}
		else
		{
			m_ownerShader = nullptr;
			m_shaderPass = nullptr;
		}
	}
}

//------------------------------------------------------------------------------
bool BasicContextState::equals(const BasicContextState& s) const
{
	if (renderState != s.renderState) return false;
	if (depthStencilState != s.depthStencilState) return false;
	if (depthBuffer != s.depthBuffer) return false;
	for (int i = 0; i < Graphics::MaxMultiRenderTargets; ++i)
	{
		if (!Utils::equalsTexture(m_renderTargets[i], s.m_renderTargets[i])) return false;
	}
	if (m_ownerShader != s.m_ownerShader) return false;
	if (m_shaderPass != s.m_shaderPass) return false;
	return true;
}

//------------------------------------------------------------------------------
void BasicContextState::copy(const BasicContextState& s)
{
	renderState = s.renderState;
	depthStencilState = s.depthStencilState;
	depthBuffer = s.depthBuffer;
	//viewport = s.viewport;
	m_renderTargets = s.m_renderTargets;
	m_ownerShader = s.m_ownerShader;
	m_shaderPass = s.m_shaderPass;
}

//==============================================================================
// ContextInterface
//==============================================================================

//------------------------------------------------------------------------------
ContextInterface::ContextInterface()
	: m_manager(nullptr)
	, m_baseRenderer(nullptr)
	, m_activeRendererPloxy(nullptr)
	, m_stateChanged(false)
{
}

//------------------------------------------------------------------------------
ContextInterface::~ContextInterface()
{
}

//------------------------------------------------------------------------------
void ContextInterface::initialize(GraphicsManager* manager)
{
	assert(manager != nullptr);
	m_manager = manager;
	m_baseRenderer = m_manager->getRenderer();
	m_stateChanged = true;
}

//------------------------------------------------------------------------------
void ContextInterface::norityStateChanging()
{
	m_manager->switchActiveContext(this);
	m_stateChanged = true;
}

//------------------------------------------------------------------------------
void ContextInterface::norityStartDrawing(detail::IRenderFeature* rendererPloxy)
{
	m_manager->switchActiveContext(this);

	bool forceStateFlush = (m_activeRendererPloxy != rendererPloxy);

	// アクティブな Renderer の切り替え
	switchActiveRendererPloxy(rendererPloxy);

	if (m_stateChanged || forceStateFlush)
	{
		if (onCheckStateChanged() || forceStateFlush)
		{
			onPrimitiveFlush();
			onStateFlush(m_activeRendererPloxy);
		}
		m_stateChanged = false;
	}
}

//------------------------------------------------------------------------------
void ContextInterface::flushImplemented()
{
	m_manager->switchActiveContext(this);

	if (m_activeRendererPloxy != nullptr)
	{
		if (m_stateChanged)
		{
			if (onCheckStateChanged())
			{
				onStateFlush(m_activeRendererPloxy);
			}
			m_stateChanged = false;
		}
		onPrimitiveFlush();
	}
}

//------------------------------------------------------------------------------
void ContextInterface::onActivated()
{
	m_stateChanged = true;
}

//------------------------------------------------------------------------------
void ContextInterface::onDeactivated()
{
	flushImplemented();
	switchActiveRendererPloxy(nullptr);
}

//------------------------------------------------------------------------------
void ContextInterface::onStateFlush(detail::IRenderFeature* activeRenderer)
{
}

//------------------------------------------------------------------------------
void ContextInterface::onPrimitiveFlush()
{
	if (m_activeRendererPloxy != nullptr)
	{
		m_activeRendererPloxy->flush();
	}
}

//------------------------------------------------------------------------------
void ContextInterface::onShaderVariableModified(ShaderVariable* var)
{
}

//------------------------------------------------------------------------------
void ContextInterface::switchActiveRendererPloxy(detail::IRenderFeature* rendererPloxy)
{
	if (rendererPloxy != m_activeRendererPloxy)
	{
		if (m_activeRendererPloxy != nullptr)
		{
			m_activeRendererPloxy->onDeactivated();	// 古いものを Deactivate
		}

		m_activeRendererPloxy = rendererPloxy;

		if (m_activeRendererPloxy != nullptr)
		{
			m_activeRendererPloxy->onActivated();	// 新しいものを activate
		}
	}
}

//------------------------------------------------------------------------------
// サブクラスの onStateFlush() で呼び出す
void ContextInterface::setBasicContextState(const BasicContextState& state)
{
	m_baseRenderer->setRenderState(state.renderState);
	m_baseRenderer->setDepthStencilState(state.depthStencilState);
	for (int i = 0; i < Graphics::MaxMultiRenderTargets; ++i)
		m_baseRenderer->setRenderTarget(i, state.getRenderTarget(i));
	m_baseRenderer->setDepthBuffer(state.depthBuffer);
//	m_baseRenderer->setViewport(state.viewport);
}

//------------------------------------------------------------------------------
void ContextInterface::makeBlendMode(BlendMode mode, RenderState* state)
{
	switch (mode)
	{
		// もっといろいろ http://d.hatena.ne.jp/Ko-Ta/20070618/p1
		case BlendMode::Normal:
			state->alphaBlendEnabled = false;
			state->blendOp = BlendOp::Add;
			state->sourceBlend = BlendFactor::One;
			state->destinationBlend = BlendFactor::Zero;
			break;
		case BlendMode::Alpha:
			state->alphaBlendEnabled = true;
			state->blendOp = BlendOp::Add;
			state->sourceBlend = BlendFactor::SourceAlpha;
			state->destinationBlend = BlendFactor::InverseSourceAlpha;
			break;
		case BlendMode::Add:
			state->alphaBlendEnabled = true;
			state->blendOp = BlendOp::Add;
			state->sourceBlend = BlendFactor::SourceAlpha;
			state->destinationBlend = BlendFactor::One;
			break;
		case BlendMode::Subtract:
			state->alphaBlendEnabled = true;
			state->blendOp = BlendOp::ReverseSubtract;
			state->sourceBlend = BlendFactor::SourceAlpha;
			state->destinationBlend = BlendFactor::One;
			break;
		case BlendMode::Multiply:
			state->alphaBlendEnabled = true;
			state->blendOp = BlendOp::Add;
			state->sourceBlend = BlendFactor::Zero;	// AlphaDisable (Alpha を別指定できない今の仕様では Alpha を考慮できない)
			state->destinationBlend = BlendFactor::SourceColor;
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
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
