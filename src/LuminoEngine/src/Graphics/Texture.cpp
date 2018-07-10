
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "RenderingCommandList.hpp"
#include "GraphicsDeviceContext.hpp"
#include <Lumino/Graphics/Bitmap.hpp>
#include <Lumino/Graphics/Texture.hpp>

namespace ln {

//==============================================================================
// RenderTargetTexture

RenderTargetTexture::RenderTargetTexture()
	: m_rhiObject(nullptr)
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

void RenderTargetTexture::initialize(detail::ITexture* ref)
{
	GraphicsResource::initialize();
	m_rhiObject = ref;
}

void RenderTargetTexture::dispose()
{
	m_rhiObject = nullptr;

	GraphicsResource::dispose();
}

Ref<Bitmap2D> RenderTargetTexture::readData()
{
	detail::ITexture* rhiObject = resolveRHIObject();

	SizeI size;
	rhiObject->getSize(&size);
	auto bitmap = newObject<Bitmap2D>(size.width, size.height, GraphicsHelper::translateToPixelFormat(rhiObject->getTextureFormat()));

	if (manager()->renderingType() == GraphicsRenderingType::Threaded)
	{
		LN_NOTIMPLEMENTED();
	}
	else
	{
		rhiObject->readData(bitmap->data());
	}

	bitmap->flipVerticalFlow();
	return bitmap;
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
