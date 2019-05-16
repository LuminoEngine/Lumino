
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
    , m_backbuffer(nullptr)
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
    m_rhiObject->acquireNextImage(&m_imageIndex);
    m_backbuffer = makeObject<RenderTargetTexture>(this);
    detail::TextureInternal::resetSwapchainFrameIfNeeded(m_backbuffer, false);
}

void SwapChain::onDispose(bool explicitDisposing)
{
    m_rhiObject = nullptr;
    m_backbuffer = nullptr;
	GraphicsResource::onDispose(explicitDisposing);
}

void SwapChain::onChangeDevice(detail::IGraphicsDevice* device)
{
}

RenderTargetTexture* SwapChain::backbuffer() const
{
    return m_backbuffer;
}

void SwapChain::resizeBackbuffer(int width, int height)
{
    if (LN_ENSURE(m_rhiObject->resizeBackbuffer(width, height))) return;
    detail::TextureInternal::resetSwapchainFrameIfNeeded(m_backbuffer, true);
}

void SwapChain::present()
{
    GraphicsContext* context = detail::GraphicsResourceInternal::manager(this)->graphicsContext();
    detail::IGraphicsContext* nativeContext = detail::GraphicsResourceInternal::manager(this)->deviceContext()->getGraphicsContext();

    detail::GraphicsContextInternal::flushCommandRecoding(context, backbuffer());

    // TODO: threading
	detail::ISwapChain* rhi = detail::GraphicsResourceInternal::resolveRHIObject<detail::ISwapChain>(nullptr, this, nullptr);

	LN_ENQUEUE_RENDER_COMMAND_2(
		SwapChain_present, context,
		detail::IGraphicsContext*, nativeContext,
		detail::ISwapChain*, rhi,
		{
			nativeContext->present(rhi);
		});
	
	detail::GraphicsResourceInternal::manager(this)->submitCommandList(detail::GraphicsContextInternal::getRenderingCommandList(context));

    // この後 readData などでバックバッファのイメージをキャプチャしたりするので、
    // ここでは次に使うべきバッファの番号だけを取り出しておく。
    // バックバッファをラップしている RenderTarget が次に resolve されたときに、
    // 実際にこの番号を使って、ラップするべきバッファを取り出す。
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
