
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/DepthBuffer.hpp>
#include <LuminoEngine/Graphics/SwapChain.hpp>
#include <LuminoEngine/Graphics/RenderPass.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include "GraphicsManager.hpp"
#include "GraphicsDeviceContext.hpp"
#include "OpenGLDeviceContext.hpp"

namespace ln {

//==============================================================================
// CommandList
namespace detail {
CommandList::CommandList()
{
}

void CommandList::init(IGraphicsDevice* device)
{
    m_rhiResource = device->createCommandList();
}

void CommandList::dispose()
{
    if (m_rhiResource) {
        m_rhiResource = nullptr;
    }
}

} // namespace detail

//==============================================================================
// SwapChain

SwapChain::SwapChain()
    : m_rhiObject(nullptr)
    , m_backbuffers()
    , m_imageIndex(0)
{
}

SwapChain::~SwapChain()
{
}

void SwapChain::init(detail::PlatformWindow* window, const SizeI& backbufferSize)
{
    // TODO: onChangeDevice でバックバッファをアタッチ
    GraphicsResource::init();
    m_rhiObject = detail::GraphicsResourceInternal::manager(this)->deviceContext()->createSwapChain(window, backbufferSize);
	m_graphicsContext = makeObject<GraphicsContext>(detail::EngineDomain::graphicsManager()->renderingType());
	resetRHIBackbuffers();
}

void SwapChain::onDispose(bool explicitDisposing)
{
    m_rhiObject = nullptr;
    for (auto& x : m_commandLists) x->dispose();
    m_commandLists.clear();
    m_depthBuffers.clear();
	m_backbuffers.clear();
	GraphicsResource::onDispose(explicitDisposing);
}

void SwapChain::onChangeDevice(detail::IGraphicsDevice* device)
{
}

RenderTargetTexture* SwapChain::currentBackbuffer() const
{
    return m_backbuffers[m_imageIndex];
}

void SwapChain::resizeBackbuffer(int width, int height)
{
    if (LN_ENSURE(m_rhiObject->resizeBackbuffer(width, height))) return;
	resetRHIBackbuffers();
}

GraphicsContext* SwapChain::beginFrame2()
{
	detail::GraphicsContextInternal::resetCommandList(m_graphicsContext, currentCommandList());
	detail::GraphicsContextInternal::beginCommandRecoding(m_graphicsContext);
	m_graphicsContext->resetState();
	return m_graphicsContext;
}

RenderPass* SwapChain::currentRenderPass() const
{
	return m_renderPasses[m_imageIndex];
}

void SwapChain::endFrame()
{
	detail::GraphicsContextInternal::flushCommandRecoding(m_graphicsContext, currentBackbuffer());
	detail::GraphicsContextInternal::endCommandRecoding(m_graphicsContext);
	detail::GraphicsResourceInternal::manager(this)->renderingQueue()->submit(m_graphicsContext);
	//auto nativeContext = detail::GraphicsContextInternal::commitState(m_graphicsContext);
	detail::GraphicsContextInternal::resetCommandList(m_graphicsContext, nullptr);

    present(m_graphicsContext);
}

void SwapChain::resetRHIBackbuffers()
{
	uint32_t count = m_rhiObject->getBackbufferCount();
	m_backbuffers.resize(count);
	m_depthBuffers.resize(count);
	m_renderPasses.resize(count);
	m_commandLists.resize(count);
	for (uint32_t i = 0; i < count; i++) {
		// backbuffer
		auto buffer = makeObject<RenderTargetTexture>(this);
		detail::TextureInternal::resetRHIObject(buffer, m_rhiObject->getRenderTarget(i));
		m_backbuffers[i] = buffer;

		// DepthBuffer
		auto depthBuffer = makeObject<DepthBuffer>(buffer->width(), buffer->height());
		m_depthBuffers[i] = depthBuffer;

		// RenderPass
		auto renderPass = makeObject<RenderPass>(buffer, depthBuffer);
		renderPass->setClearValues(ClearFlags::All, Color::Transparency, 1.0f, 0x00);
		m_renderPasses[i] = renderPass;
			
		// CommandList
        auto commandList = makeRef<detail::CommandList>();
        commandList->init(detail::GraphicsResourceInternal::manager(this)->deviceContext());
		m_commandLists[i] = commandList;
	}

	m_rhiObject->acquireNextImage(&m_imageIndex);
}

void SwapChain::present(GraphicsContext* context)
{
	auto device = detail::GraphicsResourceInternal::manager(this)->deviceContext();


    // TODO: threading
	detail::ISwapChain* rhi = detail::GraphicsResourceInternal::resolveRHIObject<detail::ISwapChain>(nullptr, this, nullptr);

	
	LN_ENQUEUE_RENDER_COMMAND_2(
		SwapChain_present, context,
		detail::IGraphicsDevice*, device,
		detail::ISwapChain*, rhi,
		{
			rhi->present();
		});
	
    m_rhiObject->acquireNextImage(&m_imageIndex);
}

detail::ISwapChain* SwapChain::resolveRHIObject(GraphicsContext* context, bool* outModified) const
{
	*outModified = false;
    return m_rhiObject;
}

//==============================================================================
// GraphicsContext
namespace detail {

void SwapChainInternal::setBackendBufferSize(SwapChain* swapChain, int width, int height)
{
    LN_DCHECK(swapChain);
    if (GLSwapChain* glswap = dynamic_cast<GLSwapChain*>(detail::GraphicsResourceInternal::resolveRHIObject<detail::ISwapChain>(nullptr, swapChain, nullptr))) {
        glswap->setBackendBufferSize(width, height);
    }
}

void SwapChainInternal::setOpenGLBackendFBO(SwapChain* swapChain, uint32_t id)
{
    LN_DCHECK(swapChain);
    if (GLSwapChain* glswap = dynamic_cast<GLSwapChain*>(detail::GraphicsResourceInternal::resolveRHIObject<detail::ISwapChain>(nullptr, swapChain, nullptr))) {
        glswap->setDefaultFBO(id);
    }
}

} // namespace detail
} // namespace ln
