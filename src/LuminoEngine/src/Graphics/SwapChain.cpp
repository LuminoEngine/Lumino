
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
    m_rhiObject = manager()->deviceContext()->createSwapChain(window, backbufferSize);
    m_rhiObject->acquireNextImage(&m_imageIndex);
    m_backbuffer = newObject<RenderTargetTexture>(this);
    m_backbuffer->resetSwapchainFrameIfNeeded();
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
    m_backbuffer->resetSwapchainFrameIfNeeded(true);
}

void SwapChain::present()
{
    GraphicsContext* context = manager()->graphicsContext();
    detail::IGraphicsContext* nativeContext = manager()->deviceContext()->getGraphicsContext();

    detail::GraphicsContextInternal::flushCommandRecoding(context, backbuffer());

    // TODO: threading
    nativeContext->present(resolveRHIObject());
    manager()->primaryRenderingCommandList()->clear();

    // この後 readData などでバックバッファのイメージをキャプチャしたりするので、
    // ここでは次に使うべきバッファの番号だけを取り出しておく。
    // バックバッファをラップしている RenderTarget が次に resolve されたときに、
    // 実際にこの番号を使って、ラップするべきバッファを取り出す。
    m_rhiObject->acquireNextImage(&m_imageIndex);
}

detail::ISwapChain* SwapChain::resolveRHIObject() const
{
    return m_rhiObject;
}

//==============================================================================
// GraphicsContext
namespace detail {

void SwapChainInternal::setBackendBufferSize(SwapChain* swapChain, int width, int height)
{
    LN_DCHECK(swapChain);
    if (GLSwapChain* glswap = dynamic_cast<GLSwapChain*>(swapChain->resolveRHIObject())) {
        glswap->setBackendBufferSize(width, height);
    }
}

void SwapChainInternal::setOpenGLBackendFBO(SwapChain* swapChain, uint32_t id)
{
    LN_DCHECK(swapChain);
    if (GLSwapChain* glswap = dynamic_cast<GLSwapChain*>(swapChain->resolveRHIObject())) {
        glswap->setDefaultFBO(id);
    }
}

} // namespace detail
} // namespace ln
