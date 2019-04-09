
#include "Internal.hpp"
//#include <LuminoEngine/Graphics/GraphicsContext.hpp>
//#include <LuminoEngine/Graphics/VertexLayout.hpp>
//#include <LuminoEngine/Graphics/VertexBuffer.hpp>
//#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/DepthBuffer.hpp>
#include <LuminoEngine/Graphics/SwapChain.hpp>
//#include <LuminoEngine/Shader/Shader.hpp>
#include "GraphicsManager.hpp"
#include "GraphicsDeviceContext.hpp"
#include "OpenGLDeviceContext.hpp"
//#include "../Engine/RenderingCommandList.hpp"
//
namespace ln {

//==============================================================================
// SwapChain

SwapChain::SwapChain()
	: m_rhiObject(nullptr)
	, m_colorBuffer(nullptr)
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
	m_colorBuffer = newObject<RenderTargetTexture>(this/*m_rhiObject->getRenderTarget(m_imageIndex)*/);
    m_colorBuffer->resetSwapchainFrameIfNeeded();
	m_depthBuffer = newObject<DepthBuffer>(backbufferSize.width, backbufferSize.height);
}

void SwapChain::onDispose(bool explicitDisposing)
{
	m_rhiObject.reset();
	m_depthBuffer.reset();
	m_colorBuffer.reset();
	Object::onDispose(explicitDisposing);
}

RenderTargetTexture* SwapChain::colorBuffer() const
{
	return m_colorBuffer;
}

DepthBuffer* SwapChain::depthBuffer() const
{
	return m_depthBuffer;
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
    m_rhiObject->acquireNextImage(&m_imageIndex);
    //m_colorBuffer->resetSwapchainFrame(m_rhiObject->getRenderTarget(m_imageIndex));
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

