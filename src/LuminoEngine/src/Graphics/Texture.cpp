
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "RenderingCommandList.hpp"
#include "GraphicsDeviceContext.hpp"
#include <Lumino/Graphics/Texture.hpp>

namespace ln {

//==============================================================================
// RenderTargetTexture

RenderTargetTexture::RenderTargetTexture()
{
}

RenderTargetTexture::~RenderTargetTexture()
{
}

void RenderTargetTexture::initialize(int width, int height, TextureFormat requestFormat, bool mipmap)
{
	GraphicsResource::initialize();
	//m_size.width = width;
	//m_size.height = height;
	//m_requestFormat = requestFormat;
	//m_mipmap = mipmap;
	m_rhiObject = manager()->deviceContext()->createRenderTarget(width, height, requestFormat, mipmap);
}

detail::ITexture* RenderTargetTexture::resolveRHIObject()
{
	return m_rhiObject;
}

void RenderTargetTexture::onChangeDevice(detail::IGraphicsDeviceContext* device)
{
	LN_NOTIMPLEMENTED();
}

} // namespace ln
