
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/DepthBuffer.hpp>
#include <LuminoEngine/Graphics/SwapChain.hpp>
#include <LuminoEngine/Graphics/RenderPass.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Shader/ShaderDescriptor.hpp>
#include "GraphicsManager.hpp"
#include "GraphicsDeviceContext.hpp"
#include "OpenGL/OpenGLDeviceContext.hpp"
#include "../Engine/LinearAllocator.hpp"
#include "SingleFrameAllocator.hpp"

namespace ln {

//==============================================================================
// GraphicsCommandList

namespace detail {

GraphicsCommandList::GraphicsCommandList()
{
}

void GraphicsCommandList::init(GraphicsManager* manager)
{
    m_rhiResource = manager->deviceContext()->createCommandList();
    m_allocator = makeRef<LinearAllocator>(manager->linearAllocatorPageManager());
	m_singleFrameUniformBufferAllocator = makeRef<detail::SingleFrameUniformBufferAllocator>(manager->singleFrameUniformBufferAllocatorPageManager());
}

void GraphicsCommandList::dispose()
{
    if (m_rhiResource) {
        m_rhiResource = nullptr;
    }
}

void GraphicsCommandList::reset()
{
    m_allocator->cleanup();
	//m_singleFrameUniformBufferAllocator->cleanup();
}

detail::ConstantBufferView GraphicsCommandList::allocateUniformBuffer(size_t size)
{
	return m_singleFrameUniformBufferAllocator->allocate(size);
}

ShaderDescriptor* GraphicsCommandList::acquireShaderDescriptor(Shader* shader)
{
	return shader->acquireDescriptor();
}

} // namespace detail

//==============================================================================
// SwapChain

SwapChain::SwapChain()
    : m_manager(nullptr)
	, m_rhiObject(nullptr)
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
    Object::init();
	detail::GraphicsResourceInternal::initializeHelper_GraphicsResource(this, &m_manager);

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

	detail::GraphicsResourceInternal::finalizeHelper_GraphicsResource(this, &m_manager);
	Object::onDispose(explicitDisposing);
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
	currentCommandList()->m_singleFrameUniformBufferAllocator->cleanup();
	m_graphicsContext->resetState();

	m_rhiObject->acquireNextImage(&m_imageIndex);
	return m_graphicsContext;
}

RenderPass* SwapChain::currentRenderPass() const
{
	return m_renderPasses[m_imageIndex];
}

void SwapChain::endFrame()
{
	currentCommandList()->m_singleFrameUniformBufferAllocator->unmap();
	detail::GraphicsContextInternal::flushCommandRecoding(m_graphicsContext, currentBackbuffer());
	detail::GraphicsContextInternal::endCommandRecoding(m_graphicsContext);
	detail::GraphicsResourceInternal::manager(this)->renderingQueue()->submit(m_graphicsContext);
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
        auto commandList = makeRef<detail::GraphicsCommandList>();
        commandList->init(detail::GraphicsResourceInternal::manager(this));
		m_commandLists[i] = commandList;
	}
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
