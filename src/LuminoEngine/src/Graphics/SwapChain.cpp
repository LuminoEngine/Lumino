
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SwapChain.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include "GraphicsManager.hpp"
#include "GraphicsDeviceContext.hpp"
#include "OpenGLDeviceContext.hpp"

namespace ln {

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
	resetRHIBackbuffers();
}

void SwapChain::onDispose(bool explicitDisposing)
{
    m_rhiObject = nullptr;
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

void SwapChain::resetRHIBackbuffers()
{
	uint32_t count = m_rhiObject->getBackbufferCount();
	m_backbuffers.resize(count);
	m_commandLists.resize(count);
	for (uint32_t i = 0; i < count; i++) {
		// backbuffer
		auto buffer = makeObject<RenderTargetTexture>(this);
		detail::TextureInternal::resetRHIObject(buffer, m_rhiObject->getRenderTarget(i));
		m_backbuffers[i] = buffer;

		// commandList
		//auto commandList = detail::GraphicsResourceInternal::manager(this)->deviceContext()->createCommandList();
		//m_commandLists.push_back(commandList);
	}

	m_rhiObject->acquireNextImage(&m_imageIndex);
}

void SwapChain::present(GraphicsContext* context)
{
	auto device = detail::GraphicsResourceInternal::manager(this)->deviceContext();
	auto nativeContext = detail::GraphicsContextInternal::commitState(context);

    detail::GraphicsContextInternal::flushCommandRecoding(context, currentBackbuffer());

    // TODO: threading
	detail::ISwapChain* rhi = detail::GraphicsResourceInternal::resolveRHIObject<detail::ISwapChain>(nullptr, this, nullptr);

	
	LN_ENQUEUE_RENDER_COMMAND_2(
		SwapChain_present, context,
		detail::IGraphicsDevice*, device,
		detail::ISwapChain*, rhi,
		{
			rhi->present();
			device->collectGarbageObjects();
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
