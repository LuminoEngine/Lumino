
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Graphics/VertexLayout.hpp>
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
	: m_manager(nullptr)
	, m_context(nullptr)
	, m_staging()
	, m_lastCommit()
	, m_modifiedFlags(ModifiedFlags_All)
	, m_recordingBegan(false)
{
}

GraphicsContext::~GraphicsContext()
{
}

void GraphicsContext::init(detail::IGraphicsContext* context)
{
    Object::init();
	m_manager = detail::EngineDomain::graphicsManager();
    m_context = context;
    m_lastCommit.reset();
	resetState();
}

void GraphicsContext::onDispose(bool explicitDisposing)
{
	if (m_context) {
        endCommandRecodingIfNeeded();
        m_context = nullptr;
	}
    m_lastCommit.reset();
    m_staging.reset();
    Object::onDispose(explicitDisposing);
}

void GraphicsContext::resetState()
{
	m_staging.reset();
	m_modifiedFlags = ModifiedFlags_All;
}

void GraphicsContext::setBlendState(const BlendStateDesc& value)
{
	if (!BlendStateDesc::equals(m_staging.blendState, value)) {
		m_staging.blendState = value;
		m_modifiedFlags |= ModifiedFlags_BlendState;
	}
}

void GraphicsContext::setRasterizerState(const RasterizerStateDesc& value)
{
	if (!RasterizerStateDesc::equals(m_staging.rasterizerState, value)) {
		m_staging.rasterizerState = value;
		m_modifiedFlags |= ModifiedFlags_RasterizerState;
	}
}

void GraphicsContext::setDepthStencilState(const DepthStencilStateDesc& value)
{
	if (!DepthStencilStateDesc::equals(m_staging.depthStencilState, value)) {
		m_staging.depthStencilState = value;
		m_modifiedFlags |= ModifiedFlags_DepthStencilState;
	}
}

void GraphicsContext::setRenderTarget(int index, RenderTargetTexture* value)
{
	if (m_staging.renderTargets[index] != value)
	{
		m_staging.renderTargets[index] = value;
		if (index == 0 && value) {
			auto rect = Rect(0, 0, value->width(), value->height());
			setViewportRect(rect);
			setScissorRect(rect);
		}
		m_modifiedFlags |= ModifiedFlags_Framebuffer;
	}
}

RenderTargetTexture* GraphicsContext::renderTarget(int index) const
{
    return m_staging.renderTargets[index];
}

void GraphicsContext::setDepthBuffer(DepthBuffer* value)
{
	if (m_staging.depthBuffer != value) {
		m_staging.depthBuffer = value;
		m_modifiedFlags |= ModifiedFlags_Framebuffer;
	}
}

DepthBuffer* GraphicsContext::depthBuffer() const
{
    return m_staging.depthBuffer;
}

void GraphicsContext::setViewportRect(const Rect& value)
{
	if (m_staging.viewportRect != value) {
		m_staging.viewportRect = value;
		m_modifiedFlags |= ModifiedFlags_RegionRects;
	}
}

void GraphicsContext::setScissorRect(const Rect& value)
{
	if (m_staging.scissorRect != value) {
		m_staging.scissorRect = value;
		m_modifiedFlags |= ModifiedFlags_RegionRects;
	}
}

void GraphicsContext::setVertexLayout(VertexLayout* value)
{
	if (m_staging.VertexLayout != value) {
		m_staging.VertexLayout = value;
		m_modifiedFlags |= ModifiedFlags_PipelinePrimitiveState;
	}
}

VertexLayout* GraphicsContext::vertexLayout() const
{
	return m_staging.VertexLayout;
}

void GraphicsContext::setPrimitiveTopology(PrimitiveTopology value)
{
	if (m_staging.topology != value) {
		m_staging.topology = value;
		m_modifiedFlags |= ModifiedFlags_PipelinePrimitiveState;
	}
}

void GraphicsContext::setVertexBuffer(int streamIndex, VertexBuffer* value)
{
    if (m_staging.vertexBuffers[streamIndex] != value) {
        m_staging.vertexBuffers[streamIndex] = value;
        m_modifiedFlags |= ModifiedFlags_PrimitiveBuffers;
    }
}

VertexBuffer* GraphicsContext::vertexBuffer(int streamIndex) const
{
	if (LN_REQUIRE_RANGE(streamIndex, 0, MaxVertexStreams)) return nullptr;
	return m_staging.vertexBuffers[streamIndex];
}

void GraphicsContext::setIndexBuffer(IndexBuffer* value)
{
    if (m_staging.indexBuffer != value) {
        m_staging.indexBuffer = value;
        m_modifiedFlags |= ModifiedFlags_PrimitiveBuffers;
    }
}

IndexBuffer* GraphicsContext::indexBuffer() const
{
	return m_staging.indexBuffer;
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

ShaderPass* GraphicsContext::shaderPass() const
{
	return m_staging.shaderPass;
}

void GraphicsContext::clear(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
    beginCommandRecodingIfNeeded();
	commitState();
	// TODO: threading
    m_context->clearBuffers(flags, color, z, stencil);
}

void GraphicsContext::drawPrimitive(int startVertex, int primitiveCount)
{
    beginCommandRecodingIfNeeded();
	commitState();
	LN_ENQUEUE_RENDER_COMMAND_3(
		GraphicsContext_setIndexBuffer, m_manager,
		detail::IGraphicsContext*, m_context,
		int, startVertex,
		int, primitiveCount,
		{
			m_context->drawPrimitive(startVertex, primitiveCount);
		});
}

void GraphicsContext::drawPrimitiveIndexed(int startIndex, int primitiveCount)
{
    beginCommandRecodingIfNeeded();
	commitState();
	LN_ENQUEUE_RENDER_COMMAND_3(
		GraphicsContext_setIndexBuffer, m_manager,
		detail::IGraphicsContext*, m_context,
		int, startIndex,
		int, primitiveCount,
		{
			m_context->drawPrimitiveIndexed(startIndex, primitiveCount);
		});
}

//void GraphicsContext::present(SwapChain* swapChain)
//{
//	if (LN_REQUIRE(swapChain)) return;
//
//    flushCommandRecoding(swapChain->backbuffer());
//
//	// TODO: threading
//	m_context->present(swapChain->resolveRHIObject());
//    m_manager->primaryRenderingCommandList()->clear();
//    swapChain->onPostPresent();
//}

void GraphicsContext::beginCommandRecodingIfNeeded()
{
    if (!m_recordingBegan) {
        LN_ENQUEUE_RENDER_COMMAND_1(
            GraphicsContext_beginCommandRecodingIfNeeded, m_manager,
            detail::IGraphicsContext*, m_context,
            {
                m_context->begin();
            });

        m_recordingBegan = true;
    }
}

void GraphicsContext::endCommandRecodingIfNeeded()
{
    if (m_recordingBegan) {
        LN_ENQUEUE_RENDER_COMMAND_1(
            GraphicsContext_beginCommandRecodingIfNeeded, m_manager,
            detail::IGraphicsContext*, m_context,
            {
                m_context->end();
            });
        m_recordingBegan = false;
    }
}

void GraphicsContext::flushCommandRecoding(RenderTargetTexture* affectRendreTarget)
{
    // Vulkan: CommandBuffer が空の状態で VkSubmitQueue するとエラーするので、一度もコマンドを作っていない場合は flush が呼ばれても何もしないようにする
    if (m_recordingBegan)
    {
        endCommandRecodingIfNeeded();

        detail::ITexture* rhiObject = detail::GraphicsResourceInternal::resolveRHIObject<detail::ITexture>(affectRendreTarget, nullptr);
        LN_ENQUEUE_RENDER_COMMAND_2(
            GraphicsContext_beginCommandRecodingIfNeeded, m_manager,
            detail::IGraphicsContext*, m_context,
            detail::ITexture*, rhiObject,
            {
                m_context->flushCommandBuffer(rhiObject);
            });
    }
}

// IGraphicsDevice の clear, draw 系の機能を呼び出したい場合はこの戻り値を使うこと。
// GraphicsContext は変更中のステートをキャッシュするが、それを確実に IGraphicsDevice へ送信した状態にする。
detail::IGraphicsContext* GraphicsContext::commitState()
{
	// ポインタとしては変わっていなくても、resolve は毎回呼び出す。
	// こうしておかないと、
	// 頂点バッファset > 描画 > 頂点バッファ更新 > 描画
	// といったように、同じオブジェクトを set したまま内容を更新した場合に反映されなくなる。

	// TODO: ぜんぶまとめて送信できる方法も用意していい気がする。

	//{
	//	auto& blendState = m_staging.blendState;
	//	auto& rasterizerState = m_staging.rasterizerState;
	//	auto& depthStencilState = m_staging.depthStencilState;
	//	LN_ENQUEUE_RENDER_COMMAND_4(
	//		GraphicsContext_setPipelineState, m_manager,
	//		detail::IGraphicsContext*, m_context,
	//		BlendStateDesc, blendState,
	//		RasterizerStateDesc, rasterizerState,
	//		DepthStencilStateDesc, depthStencilState,
	//		{
	//			m_context->setBlendState(blendState);
	//			m_context->setRasterizerState(rasterizerState);
	//			m_context->setDepthStencilState(depthStencilState);
	//		});
	//}

	// BlendState
	if ((m_modifiedFlags & ModifiedFlags_BlendState) != 0)
	{
		auto& blendState = m_staging.blendState;
		LN_ENQUEUE_RENDER_COMMAND_2(
			GraphicsContext_setPipelineState, m_manager,
			detail::IGraphicsContext*, m_context,
			BlendStateDesc, blendState,
			{
				m_context->setBlendState(blendState);
			});

		m_lastCommit.blendState = m_staging.blendState;
	}

	// RasterizerState
	if ((m_modifiedFlags & ModifiedFlags_RasterizerState) != 0)
	{
		auto& rasterizerState = m_staging.rasterizerState;
		LN_ENQUEUE_RENDER_COMMAND_2(
			GraphicsContext_setPipelineState, m_manager,
			detail::IGraphicsContext*, m_context,
			RasterizerStateDesc, rasterizerState,
			{
				m_context->setRasterizerState(rasterizerState);
			});

		m_lastCommit.rasterizerState = m_staging.rasterizerState;
	}

	// DepthStencilState
	if ((m_modifiedFlags & ModifiedFlags_DepthStencilState) != 0)
	{
		auto& depthStencilState = m_staging.depthStencilState;
		LN_ENQUEUE_RENDER_COMMAND_2(
			GraphicsContext_setPipelineState, m_manager,
			detail::IGraphicsContext*, m_context,
			DepthStencilStateDesc, depthStencilState,
			{
				m_context->setDepthStencilState(depthStencilState);
			});

		m_lastCommit.depthStencilState = m_staging.depthStencilState;
	}


	//{
	//	for (int i = 0; i < m_staging.renderTargets.size(); i++)
	//	{
	//		auto& value = m_staging.renderTargets[i];
 //           if (value) {
 //               value->resetSwapchainFrameIfNeeded();
 //           }
	//		detail::ITexture* rhiObject = detail::GraphicsResourceInternal::resolveRHIObject<detail::ITexture>(value, nullptr);
	//		LN_ENQUEUE_RENDER_COMMAND_3(
	//			GraphicsContext_setDepthBuffer, m_manager,
	//			detail::IGraphicsContext*, m_context,
	//			int, i,
	//			detail::ITexture*, rhiObject,
	//			{
	//				m_context->setColorBuffer(i, rhiObject);
	//			});
	//	}
	//}

	//{
	//	auto& value = m_staging.depthBuffer;
	//	bool modified = false;
	//	detail::IDepthBuffer* rhiObject = detail::GraphicsResourceInternal::resolveRHIObject<detail::IDepthBuffer>(value, &modified);
	//	LN_ENQUEUE_RENDER_COMMAND_2(
	//		GraphicsContext_setDepthBuffer, m_manager,
	//		detail::IGraphicsContext*, m_context,
	//		detail::IDepthBuffer*, rhiObject,
	//		{
	//			m_context->setDepthBuffer(rhiObject);
	//		});
	//}



	// RenderTarget, DepthBuffer
	{
		bool anyModified = false;
		bool modified = false;

		using RenderTargetArray = std::array<detail::ITexture*, detail::MaxMultiRenderTargets>;
		RenderTargetArray renderTargets;
		for (int i = 0; i < detail::MaxMultiRenderTargets; i++) {
			auto& value = m_staging.renderTargets[i];
			if (value) {
				value->resetSwapchainFrameIfNeeded();
			}
			renderTargets[i] = detail::GraphicsResourceInternal::resolveRHIObject<detail::ITexture>(value, &modified);
			anyModified |= modified;
		}

		detail::IDepthBuffer* depthBuffer = detail::GraphicsResourceInternal::resolveRHIObject<detail::IDepthBuffer>(m_staging.depthBuffer, &modified);
		anyModified |= modified;

		if ((m_modifiedFlags & ModifiedFlags_Framebuffer) != 0 || anyModified)
		{
			LN_ENQUEUE_RENDER_COMMAND_3(
				GraphicsContext_setDepthBuffer, m_manager,
				detail::IGraphicsContext*, m_context,
				RenderTargetArray, renderTargets,
				detail::IDepthBuffer*, depthBuffer,
				{
					for (int i = 0; i < detail::MaxMultiRenderTargets; i++) {
						m_context->setColorBuffer(i, renderTargets[i]);
					}
					m_context->setDepthBuffer(depthBuffer);
				});
		}
	}

	// Viewport, Scissor
	if ((m_modifiedFlags & ModifiedFlags_RegionRects) != 0)
	{
		RectI viewportRect = RectI::fromFloatRect(m_staging.viewportRect);
		RectI scissorRect = RectI::fromFloatRect(m_staging.scissorRect);
		LN_ENQUEUE_RENDER_COMMAND_3(
			GraphicsContext_setDepthBuffer, m_manager,
			detail::IGraphicsContext*, m_context,
			RectI, viewportRect,
			RectI, scissorRect,
			{
				m_context->setViewportRect(viewportRect);
				m_context->setScissorRect(scissorRect);
			});

		m_lastCommit.viewportRect = m_staging.viewportRect;
		m_lastCommit.scissorRect = m_staging.scissorRect;
	}

	// VertexLayout, Topology
	{
		bool modified = false;
		detail::IVertexDeclaration* vertexDeclaration = detail::GraphicsResourceInternal::resolveRHIObject<detail::IVertexDeclaration>(m_staging.VertexLayout, &modified);
		PrimitiveTopology topology = m_staging.topology;

		if ((m_modifiedFlags & ModifiedFlags_PipelinePrimitiveState) != 0 || modified)
		{
			LN_ENQUEUE_RENDER_COMMAND_3(
				GraphicsContext_setPrimitiveBuffers, m_manager,
				detail::IGraphicsContext*, m_context,
				PrimitiveTopology, topology,
				detail::IVertexDeclaration*, vertexDeclaration,
				{
					m_context->setPrimitiveTopology(topology);
					m_context->setVertexDeclaration(vertexDeclaration);
				});

			m_lastCommit.VertexLayout = m_staging.VertexLayout;
			m_lastCommit.topology = m_staging.topology;
		}
	}
   
	// VertexBuffer, IndexBuffer
	{
		bool anyModified = false;
		bool modified = false;

		//PrimitiveTopology topology = m_staging.topology;

		//detail::IVertexDeclaration* vertexDeclaration = detail::GraphicsResourceInternal::resolveRHIObject<detail::IVertexDeclaration>(m_staging.VertexLayout, &modified);
		//anyModified |= modified;

		using VertexBufferArray = std::array<detail::IVertexBuffer*, detail::MaxVertexStreams>;
		VertexBufferArray vertexBuffers;
		for (int i = 0; i < m_staging.vertexBuffers.size(); i++)
		{
			auto& value = m_staging.vertexBuffers[i];
			vertexBuffers[i] = detail::GraphicsResourceInternal::resolveRHIObject<detail::IVertexBuffer>(value, &modified);
			anyModified |= modified;
		}

		detail::IIndexBuffer* indexBuffer = detail::GraphicsResourceInternal::resolveRHIObject<detail::IIndexBuffer>(m_staging.indexBuffer, &modified);
		anyModified |= modified;

		if ((m_modifiedFlags & ModifiedFlags_PrimitiveBuffers) != 0 || anyModified)
		{
			LN_ENQUEUE_RENDER_COMMAND_3(
				GraphicsContext_setPrimitiveBuffers, m_manager,
				detail::IGraphicsContext*, m_context,
				//PrimitiveTopology, topology,
				//detail::IVertexDeclaration*, vertexDeclaration,
				VertexBufferArray, vertexBuffers,
				detail::IIndexBuffer*, indexBuffer,
				{
					//m_context->setPrimitiveTopology(topology);
					//m_context->setVertexDeclaration(vertexDeclaration);
					for (int i = 0; i < detail::MaxVertexStreams; i++) {
						m_context->setVertexBuffer(i, vertexBuffers[i]);
					}
					m_context->setIndexBuffer(indexBuffer);
				});

			m_lastCommit.vertexBuffers = m_staging.vertexBuffers;
			m_lastCommit.indexBuffer = m_staging.indexBuffer;
		}
	}

    //if ((m_modifiedFlags & ModifiedFlags_ShaderPass) != 0)
	{
		auto& value = m_staging.shaderPass;
		detail::IShaderPass* rhiObject = (value) ? value->resolveRHIObject() : nullptr;

		if ((m_modifiedFlags & ModifiedFlags_ShaderPass) != 0)
		{
			LN_ENQUEUE_RENDER_COMMAND_2(
				GraphicsContext_setShaderPass, m_manager,
				detail::IGraphicsContext*, m_context,
				detail::IShaderPass*, rhiObject,
				{
					m_context->setShaderPass(rhiObject);
				});

			m_lastCommit.shader = m_staging.shader;
			m_lastCommit.shaderPass = m_staging.shaderPass;

			if (value) {
				//value->commitContantBuffers();
			}
		}

	}

    m_modifiedFlags = ModifiedFlags_None;

	return m_context;
}

void GraphicsContext::State::reset()
{
	blendState = BlendStateDesc();
	rasterizerState = RasterizerStateDesc();
	depthStencilState = DepthStencilStateDesc();
	renderTargets = {};
	depthBuffer = nullptr;
	viewportRect = Rect();
	scissorRect = Rect();
	VertexLayout = nullptr;
	vertexBuffers = {};
	indexBuffer = nullptr;
    shader = nullptr;
	shaderPass = nullptr;
	topology = PrimitiveTopology::TriangleList;
}

} // namespace ln

