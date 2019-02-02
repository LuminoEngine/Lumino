
#include "Internal.hpp"
//#include <LuminoEngine/Graphics/GraphicsContext.hpp>
//#include <LuminoEngine/Graphics/VertexDeclaration.hpp>
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
	m_colorBuffer = newObject<RenderTargetTexture>(m_rhiObject->getColorBuffer());
	m_depthBuffer = newObject<DepthBuffer>(m_colorBuffer->width(), m_colorBuffer->height());
}

void SwapChain::dispose()
{
	m_rhiObject.reset();
	m_depthBuffer.reset();
	m_colorBuffer.reset();
	Object::dispose();
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
    m_colorBuffer->resetSwapchainFrame(m_rhiObject->getColorBuffer());
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

