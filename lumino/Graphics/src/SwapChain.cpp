
#include "Internal.hpp"
#include <LuminoGraphics/Texture.hpp>
#include <LuminoGraphics/DepthBuffer.hpp>
#include <LuminoGraphics/SwapChain.hpp>
#include <LuminoGraphics/RenderPass.hpp>
#include <LuminoGraphics/GraphicsContext.hpp>
#include <LuminoGraphics/GraphicsCommandBuffer.hpp>
#include <LuminoGraphics/ShaderDescriptor.hpp>
#include "GraphicsManager.hpp"
#include "RenderTargetTextureCache.hpp"
#include "RHIs/GraphicsDeviceContext.hpp"
#include "../../LuminoEngine/src/Engine/LinearAllocator.hpp"
#include "SingleFrameAllocator.hpp"

namespace ln {

//==============================================================================
// SwapChain

SwapChain::SwapChain()
    : m_manager(nullptr)
	, m_rhiObject(nullptr)
    , m_backbuffers()
    , m_imageIndex(-1)
{
}

SwapChain::~SwapChain()
{
}

void SwapChain::init(PlatformWindow* window, const SizeI& backbufferSize)
{
    // TODO: onChangeDevice でバックバッファをアタッチ
    Object::init();
	detail::GraphicsResourceInternal::initializeHelper_GraphicsResource(this, &m_manager);

    m_rhiObject = detail::GraphicsResourceInternal::manager(this)->deviceContext()->createSwapChain(window, backbufferSize);
	m_graphicsContext = makeObject<GraphicsContext>(detail::GraphicsManager::instance()->renderingType());
	
	resetRHIBackbuffers();

	// CommandList
	uint32_t count = m_rhiObject->getBackbufferCount();
	m_commandLists.resize(count);
	for (uint32_t i = 0; i < count; i++) {
		auto commandList = makeRef<detail::GraphicsCommandList>();
		commandList->init(detail::GraphicsResourceInternal::manager(this));
		m_commandLists[i] = commandList;
	}
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

Size SwapChain::backbufferSize() const
{
	const RenderTargetTexture* backbuffers = m_backbuffers[0];
	return Size(backbuffers->width(), backbuffers->height());
}

RenderTargetTexture* SwapChain::currentBackbuffer() const
{
	if (LN_REQUIRE(m_imageIndex >= 0)) return nullptr;
    return m_backbuffers[m_imageIndex];
}

void SwapChain::resizeBackbuffer(int width, int height)
{
    if (LN_ENSURE(m_rhiObject->resizeBackbuffer(width, height))) return;
	resetRHIBackbuffers();
}

GraphicsContext* SwapChain::beginFrame2()
{
	m_rhiObject->acquireNextImage(&m_imageIndex);

	detail::GraphicsContextInternal::resetCommandList(m_graphicsContext, currentCommandList());
	detail::GraphicsContextInternal::beginCommandRecoding(m_graphicsContext);
	currentCommandList()->m_singleFrameUniformBufferAllocator->cleanup();
	m_graphicsContext->resetState();

	return m_graphicsContext;
}

RenderPass* SwapChain::currentRenderPass() const
{
	if (LN_REQUIRE(m_imageIndex >= 0)) return nullptr;
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
	}
	m_imageIndex = -1;
}

void SwapChain::present(GraphicsContext* context)
{
	detail::GraphicsManager* manager = detail::GraphicsResourceInternal::manager(this);
	auto device = manager->deviceContext();


    // TODO: threading
	detail::ISwapChain* rhi = detail::GraphicsResourceInternal::resolveRHIObject<detail::ISwapChain>(nullptr, this, nullptr);

	
	LN_ENQUEUE_RENDER_COMMAND_2(
		SwapChain_present, context,
		detail::IGraphicsDevice*, device,
		detail::ISwapChain*, rhi,
		{
			rhi->present();
		});

	manager->frameBufferCache()->gcObjects();
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
	LN_NOTIMPLEMENTED();
}

void SwapChainInternal::setOpenGLBackendFBO(SwapChain* swapChain, uint32_t id)
{
	LN_NOTIMPLEMENTED();
}

} // namespace detail
} // namespace ln
