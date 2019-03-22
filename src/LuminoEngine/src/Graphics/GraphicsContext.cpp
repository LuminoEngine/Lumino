
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Graphics/VertexDeclaration.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/DepthBuffer.hpp>
#include <LuminoEngine/Graphics/SwapChain.hpp>
#include <LuminoEngine/Shader/Shader.hpp>
#include "GraphicsManager.hpp"
#include "GraphicsDeviceContext.hpp"
#include "../Engine/RenderingCommandList.hpp"

namespace ln {

//==============================================================================
// GraphicsContext

GraphicsContext::GraphicsContext()
	: m_device(nullptr)
    , m_modifiedFlags(ModifiedFlags_All)
    , m_recordingBegan(false)
{
}

GraphicsContext::~GraphicsContext()
{
}

void GraphicsContext::init(detail::IGraphicsDeviceContext* device)
{
    Object::init();
	m_manager = detail::EngineDomain::graphicsManager();
	m_device = device;
    m_lastCommit.reset();
	resetState();
}

void GraphicsContext::onDispose(bool explicitDisposing)
{
	if (m_device) {
        endCommandRecodingIfNeeded();
		m_device = nullptr;
	}
    m_lastCommit.reset();
    m_staging.reset();
    Object::onDispose(explicitDisposing);
}

void GraphicsContext::resetState()
{
	m_staging.reset();
}

void GraphicsContext::setBlendState(const BlendStateDesc& value)
{
	m_staging.blendState = value;
}

void GraphicsContext::setRasterizerState(const RasterizerStateDesc& value)
{
	m_staging.rasterizerState = value;
}

void GraphicsContext::setDepthStencilState(const DepthStencilStateDesc& value)
{
	m_staging.depthStencilState = value;
}

void GraphicsContext::setColorBuffer(int index, RenderTargetTexture* value)
{
	m_staging.renderTargets[index] = value;
	if (index == 0 && value) {
		auto rect = Rect(0, 0, value->width(), value->height());
		setViewportRect(rect);
		setScissorRect(rect);
	}
}

RenderTargetTexture* GraphicsContext::colorBuffer(int index) const
{
    return m_staging.renderTargets[index];
}

void GraphicsContext::setDepthBuffer(DepthBuffer* value)
{
	m_staging.depthBuffer = value;
}

DepthBuffer* GraphicsContext::depthBuffer() const
{
    return m_staging.depthBuffer;
}

void GraphicsContext::setViewportRect(const Rect& value)
{
	m_staging.viewportRect = value;
}

void GraphicsContext::setScissorRect(const Rect& value)	// 使用するのは主に UI なので、ピクセル単位で指定
{
	m_staging.scissorRect = value;
}

void GraphicsContext::setVertexDeclaration(VertexDeclaration* value)
{
	m_staging.vertexDeclaration = value;
}

void GraphicsContext::setVertexBuffer(int streamIndex, VertexBuffer* value)
{
    if (m_staging.vertexBuffers[streamIndex] != value) {
        m_staging.vertexBuffers[streamIndex] = value;
        m_modifiedFlags |= ModifiedFlags_VertexBuffers;
    }
}

void GraphicsContext::setIndexBuffer(IndexBuffer* value)
{
    if (m_staging.indexBuffer != value) {
        m_staging.indexBuffer = value;
        m_modifiedFlags |= ModifiedFlags_IndexBuffer;
    }
}

void GraphicsContext::setShaderPass(ShaderPass* pass)
{
    if (m_staging.shaderPass != pass)
    {
        if (pass)
        {
            m_staging.shader = pass->shader();
            m_staging.shaderPass = pass;
        }
        else
        {
            m_staging.shader = nullptr;
            m_staging.shaderPass = nullptr;
        }
        m_modifiedFlags |= ModifiedFlags_ShaderPass;
    }
}

void GraphicsContext::setPrimitiveTopology(PrimitiveTopology value)
{
	if (m_staging.topology != value) {
		m_staging.topology = value;
	}
}

void GraphicsContext::clear(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
    beginCommandRecodingIfNeeded();
	commitState();
	// TODO: threading
	m_device->clearBuffers(flags, color, z, stencil);
}

void GraphicsContext::drawPrimitive(int startVertex, int primitiveCount)
{
    beginCommandRecodingIfNeeded();
	commitState();
	LN_ENQUEUE_RENDER_COMMAND_3(
		GraphicsContext_setIndexBuffer, m_manager,
		detail::IGraphicsDeviceContext*, m_device,
		int, startVertex,
		int, primitiveCount,
		{
			m_device->drawPrimitive(startVertex, primitiveCount);
		});
}

void GraphicsContext::drawPrimitiveIndexed(int startIndex, int primitiveCount)
{
    beginCommandRecodingIfNeeded();
	commitState();
	LN_ENQUEUE_RENDER_COMMAND_3(
		GraphicsContext_setIndexBuffer, m_manager,
		detail::IGraphicsDeviceContext*, m_device,
		int, startIndex,
		int, primitiveCount,
		{
			m_device->drawPrimitiveIndexed(startIndex, primitiveCount);
		});
}

void GraphicsContext::present(SwapChain* swapChain)
{
	if (LN_REQUIRE(swapChain)) return;

    endCommandRecodingIfNeeded();

	// TODO: threading
	m_device->present(swapChain->resolveRHIObject());
    m_manager->primaryRenderingCommandList()->clear();
    swapChain->onPostPresent();
}

void GraphicsContext::flush()
{
    endCommandRecodingIfNeeded();
}

void GraphicsContext::beginCommandRecodingIfNeeded()
{
    if (!m_recordingBegan) {
        m_device->begin();
        m_recordingBegan = true;
    }
}

void GraphicsContext::endCommandRecodingIfNeeded()
{
    if (m_recordingBegan) {
        m_device->end();
        m_recordingBegan = false;
    }
}

detail::IGraphicsDeviceContext* GraphicsContext::commitState()
{
	// ポインタとしては変わっていなくても、resolve は毎回呼び出す。
	// こうしておかないと、
	// 頂点バッファset > 描画 > 頂点バッファ更新 > 描画
	// といったように、同じオブジェクトを set したまま内容を更新した場合に反映されなくなる。

	// TODO: ぜんぶまとめて送信できる方法も用意していい気がする。

	{
		auto& blendState = m_staging.blendState;
		auto& rasterizerState = m_staging.rasterizerState;
		auto& depthStencilState = m_staging.depthStencilState;
		LN_ENQUEUE_RENDER_COMMAND_4(
			GraphicsContext_setPipelineState, m_manager,
			detail::IGraphicsDeviceContext*, m_device,
			BlendStateDesc, blendState,
			RasterizerStateDesc, rasterizerState,
			DepthStencilStateDesc, depthStencilState,
			{
				m_device->setBlendState(blendState);
				m_device->setRasterizerState(rasterizerState);
				m_device->setDepthStencilState(depthStencilState);
			});
	}

	{
		for (int i = 0; i < m_staging.renderTargets.size(); i++)
		{
			auto& value = m_staging.renderTargets[i];
			detail::ITexture* rhiObject = (value) ? value->resolveRHIObject() : nullptr;
			LN_ENQUEUE_RENDER_COMMAND_3(
				GraphicsContext_setDepthBuffer, m_manager,
				detail::IGraphicsDeviceContext*, m_device,
				int, i,
				detail::ITexture*, rhiObject,
				{
					m_device->setColorBuffer(i, rhiObject);
				});
		}
	}

	{
		auto& value = m_staging.depthBuffer;
		detail::IDepthBuffer* rhiObject = (value) ? value->resolveRHIObject() : nullptr;
		LN_ENQUEUE_RENDER_COMMAND_2(
			GraphicsContext_setDepthBuffer, m_manager,
			detail::IGraphicsDeviceContext*, m_device,
			detail::IDepthBuffer*, rhiObject,
			{
				m_device->setDepthBuffer(rhiObject);
			});
	}

	{
		RectI viewportRect = RectI::fromFloatRect(m_staging.viewportRect);
		RectI scissorRect = RectI::fromFloatRect(m_staging.scissorRect);
		LN_ENQUEUE_RENDER_COMMAND_3(
			GraphicsContext_setDepthBuffer, m_manager,
			detail::IGraphicsDeviceContext*, m_device,
			RectI, viewportRect,
			RectI, scissorRect,
			{
				m_device->setViewportRect(viewportRect);
				m_device->setScissorRect(scissorRect);
			});
	}

	{
		auto& value = m_staging.vertexDeclaration;
		detail::IVertexDeclaration* rhiObject = (value) ? value->resolveRHIObject() : nullptr;
		LN_ENQUEUE_RENDER_COMMAND_2(
			GraphicsContext_setVertexDeclaration, m_manager,
			detail::IGraphicsDeviceContext*, m_device,
			detail::IVertexDeclaration*, rhiObject,
			{
				m_device->setVertexDeclaration(rhiObject);
			});
	}

    //if ((m_modifiedFlags & ModifiedFlags_VertexBuffers) != 0)
	{
		for (int i = 0; i < m_staging.vertexBuffers.size(); i++)
		{
			auto& value = m_staging.vertexBuffers[i];
			detail::IVertexBuffer* rhiObject = detail::GraphicsResourceHelper::resolveRHIObject<detail::IVertexBuffer>(value);
			LN_ENQUEUE_RENDER_COMMAND_3(
				GraphicsContext_setVertexBuffer, m_manager,
				detail::IGraphicsDeviceContext*, m_device,
				int, i,
				detail::IVertexBuffer*, rhiObject,
				{
					m_device->setVertexBuffer(i, rhiObject);
				});
		}

        m_lastCommit.vertexBuffers = m_staging.vertexBuffers;
	}

    //if ((m_modifiedFlags & ModifiedFlags_IndexBuffer) != 0)
	{
		auto& value = m_staging.indexBuffer;
		detail::IIndexBuffer* rhiObject = (value) ? value->resolveRHIObject() : nullptr;
		LN_ENQUEUE_RENDER_COMMAND_2(
			GraphicsContext_setIndexBuffer, m_manager,
			detail::IGraphicsDeviceContext*, m_device,
			detail::IIndexBuffer*, rhiObject,
			{
				m_device->setIndexBuffer(rhiObject);
			});

        m_lastCommit.indexBuffer = m_staging.indexBuffer;
	}

    //if ((m_modifiedFlags & ModifiedFlags_ShaderPass) != 0)
	{
		auto& value = m_staging.shaderPass;
		detail::IShaderPass* rhiObject = (value) ? value->resolveRHIObject() : nullptr;
		LN_ENQUEUE_RENDER_COMMAND_2(
			GraphicsContext_setShaderPass, m_manager,
			detail::IGraphicsDeviceContext*, m_device,
			detail::IShaderPass*, rhiObject,
			{
				m_device->setShaderPass(rhiObject);
			});

        m_lastCommit.shader = m_staging.shader;
        m_lastCommit.shaderPass = m_staging.shaderPass;

        if (value) {
            //value->commitContantBuffers();
        }
	}

	{
		auto& topology = m_staging.topology;
		LN_ENQUEUE_RENDER_COMMAND_2(
			GraphicsContext_setShaderPass, m_manager,
			detail::IGraphicsDeviceContext*, m_device,
			PrimitiveTopology, topology,
			{
				m_device->setPrimitiveTopology(topology);
			});
	}

    m_modifiedFlags = ModifiedFlags_None;

	return m_manager->deviceContext();
}

void GraphicsContext::State::reset()
{
	blendState = BlendStateDesc();
	rasterizerState = RasterizerStateDesc();
	depthStencilState = DepthStencilStateDesc();
	renderTargets = {};
	depthBuffer = nullptr;
	vertexDeclaration = nullptr;
	vertexBuffers = {};
	indexBuffer = nullptr;
    shader = nullptr;
	shaderPass = nullptr;
	topology = PrimitiveTopology::TriangleList;
}

} // namespace ln

