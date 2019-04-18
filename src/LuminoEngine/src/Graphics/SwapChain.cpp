
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SwapChain.hpp>
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
	// TODO: GraphicsResource にして、onChangeDevice でバックバッファをアタッチ
	Object::init();
	m_rhiObject = detail::EngineDomain::graphicsManager()->deviceContext()->createSwapChain(window, backbufferSize);
    m_rhiObject->acquireNextImage(&m_imageIndex);
	m_backbuffer = newObject<RenderTargetTexture>(this/*m_rhiObject->getRenderTarget(m_imageIndex)*/);
	m_backbuffer->resetSwapchainFrameIfNeeded();
}

void SwapChain::onDispose(bool explicitDisposing)
{
	m_rhiObject = nullptr;
	m_backbuffer = nullptr;
	Object::onDispose(explicitDisposing);
}

RenderTargetTexture* SwapChain::backbuffer() const
{
	return m_backbuffer;
}

void SwapChain::wait()
{
	// TODO
}

detail::ISwapChain* SwapChain::resolveRHIObject() const
{
	return m_rhiObject;
}

void SwapChain::onPostPresent()
{
	// この後 readData などでバックバッファのイメージをキャプチャしたりするので、
	// ここでは次に使うべきバッファの番号だけを取り出しておく。
	// バックバッファをラップしている RenderTarget が次に resolve されたときに、
	// 実際にこの番号を使って、ラップするべきバッファを取り出す。
    m_rhiObject->acquireNextImage(&m_imageIndex);
}

//==============================================================================
// GraphicsContext
namespace detail {

void SwapChainHelper::setBackendBufferSize(SwapChain* swapChain, int width, int height)
{
    LN_DCHECK(swapChain);
    if (GLSwapChain* glswap = dynamic_cast<GLSwapChain*>(swapChain->resolveRHIObject())) {
        glswap->setBackendBufferSize(width, height);
    }
}

void SwapChainHelper::setOpenGLBackendFBO(SwapChain* swapChain, uint32_t id)
{
    LN_DCHECK(swapChain);
    if (GLSwapChain* glswap = dynamic_cast<GLSwapChain*>(swapChain->resolveRHIObject())) {
        glswap->setDefaultFBO(id);
    }
}

} // namespace detail
} // namespace ln

