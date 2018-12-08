
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "../Engine/RenderingCommandList.hpp"
#include "GraphicsDeviceContext.hpp"
#include <LuminoEngine/Graphics/Bitmap.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Font/Font.hpp>
#include "../Font/TextLayoutEngine.hpp"

namespace ln {

//==============================================================================
// Texture

Texture::Texture()
	: m_size(0, 0)
	, m_format(TextureFormat::Unknown)
	, m_samplerState(nullptr)
	, m_mipmap(false)
{
}

Texture::~Texture()
{
}

void Texture::initialize()
{
	GraphicsResource::initialize();
}

SamplerState* Texture::samplerState() const
{
	return m_samplerState;
}

void Texture::setSamplerState(SamplerState* value)
{
	m_samplerState = value;
}

//==============================================================================
// Texture2D

Ref<Texture2D> Texture2D::create(int width, int height, TextureFormat format, bool mipmap, GraphicsResourceUsage usage)
{
    return newObject<Texture2D>(width, height, format, mipmap, usage);
}

Texture2D::Texture2D()
	: m_rhiObject(nullptr)
	, m_usage(GraphicsResourceUsage::Static)
	, m_pool(GraphicsResourcePool::Managed)
	, m_bitmap(nullptr)
	, m_rhiLockedBuffer(nullptr)
	, m_initialUpdate(false)
	, m_modified(false)
{
}

Texture2D::~Texture2D()
{
}

void Texture2D::initialize(int width, int height, TextureFormat format, bool mipmap, GraphicsResourceUsage usage)
{
	Texture::initialize();
	m_bitmap = newObject<Bitmap2D>(width, height, GraphicsHelper::translateToPixelFormat(format));
	m_usage = usage;
	m_initialUpdate = true;
	m_modified = true;
	setSize(SizeI(width, height));
	setFormat(format);
	setMipmap(mipmap);
}

void Texture2D::initialize(const StringRef& filePath, TextureFormat format, bool mipmap, GraphicsResourceUsage usage)
{
    Texture::initialize();
	m_bitmap = newObject<Bitmap2D>();
	m_bitmap->load(filePath);

	// TODO: check and convert format

	m_usage = usage;
	m_initialUpdate = true;
	m_modified = true;
	setSize(SizeI(m_bitmap->width(), m_bitmap->height()));
	setFormat(format);
	setMipmap(mipmap);
}

void Texture2D::dispose()
{
    m_rhiObject.reset();
    Texture::dispose();
}

Bitmap2D* Texture2D::map(MapMode mode)
{
	if (m_initialUpdate && m_pool == GraphicsResourcePool::None)
	{
		LN_NOTIMPLEMENTED();
	}

	m_modified = true;
	return m_bitmap;
}

void Texture2D::setResourcePool(GraphicsResourcePool pool)
{
	m_pool = pool;
}

void Texture2D::clear(const Color& color)
{
    Bitmap2D* bitmap = map(MapMode::Write);
    bitmap->clear(color.to32BitColor());
}

void Texture2D::drawText(const StringRef& text, const Rect& rect, Font* font, const Color& color)
{
    Bitmap2D* bitmap = map(MapMode::Write);
    detail::BitmapTextRenderer renderer;
    renderer.render(bitmap, text, rect, font, color, detail::TextLayoutOptions::None);
}

void Texture2D::onChangeDevice(detail::IGraphicsDeviceContext* device)
{
	if (device)
	{
		if (m_pool == GraphicsResourcePool::Managed)
		{
			// data is transferred by the next resolveRHIObject()
			m_modified = true;
		}
	}
	else
	{
		m_rhiObject.reset();
	}
}

detail::ITexture* Texture2D::resolveRHIObject()
{
	if (m_modified)
	{
		if (m_rhiLockedBuffer)
		{
			LN_NOTIMPLEMENTED();
		}
		else
		{
			ByteBuffer* bmpBuffer = m_bitmap->rawBuffer();
			SizeI bmpSize(m_bitmap->width(), m_bitmap->height());
			detail::RenderBulkData bmpRawData = manager()->primaryRenderingCommandList()->allocateBulkData(bmpBuffer->size());
			detail::BitmapHelper::blitRawSimple(
				bmpRawData.writableData(), bmpBuffer->data(), m_bitmap->width(), m_bitmap->height(), Bitmap2D::getPixelFormatByteSize(m_bitmap->format()), false);

			if (!m_rhiObject)
			{
				m_rhiObject = manager()->deviceContext()->createTexture2D(width(), height(), format(), mipmap(), m_bitmap->data());
			}
			else
			{
				detail::ITexture* rhiObject = m_rhiObject;
				LN_ENQUEUE_RENDER_COMMAND_3(
					Texture2D_setSubData, manager(),
					detail::RenderBulkData, bmpRawData,
					SizeI, bmpSize,
					Ref<detail::ITexture>, rhiObject,
					{
						rhiObject->setSubData(0, 0, bmpSize.width, bmpSize.height, bmpRawData.data(), bmpRawData.size());
					});
			}
		}
	}

	if (LN_ENSURE(m_rhiObject)) return nullptr;

	if (m_pool == GraphicsResourcePool::None) {
		m_bitmap.reset();
	}

	m_initialUpdate = false;
	m_modified = false;
	return m_rhiObject;
}

//==============================================================================
// Texture3D

Texture3D::Texture3D()
	: m_rhiObject(nullptr)
	, m_usage(GraphicsResourceUsage::Static)
	, m_pool(GraphicsResourcePool::Managed)
	, m_depth(0)
	, m_bitmap(nullptr)
	, m_rhiLockedBuffer(nullptr)
	, m_initialUpdate(false)
	, m_modified(false)
{
}

Texture3D::~Texture3D()
{
}

void Texture3D::initialize(int width, int height, int depth, TextureFormat format, bool mipmap, GraphicsResourceUsage usage)
{
	Texture::initialize();
	m_depth = depth;
	m_bitmap = newObject<Bitmap3D>(width, height, depth, GraphicsHelper::translateToPixelFormat(format));
	m_usage = usage;
	m_initialUpdate = true;
	m_modified = true;
	setSize(SizeI(width, height));
	setFormat(format);
	setMipmap(mipmap);
}

Bitmap3D* Texture3D::map(MapMode mode)
{
	if (m_initialUpdate && m_pool == GraphicsResourcePool::None)
	{
		LN_NOTIMPLEMENTED();
	}

	m_modified = true;
	return m_bitmap;
}

void Texture3D::setResourcePool(GraphicsResourcePool pool)
{
	m_pool = pool;
}

void Texture3D::dispose()
{
	m_rhiObject.reset();
	Texture::dispose();
}

void Texture3D::onChangeDevice(detail::IGraphicsDeviceContext* device)
{
	if (device)
	{
		if (m_pool == GraphicsResourcePool::Managed)
		{
			// data is transferred by the next resolveRHIObject()
			m_modified = true;
		}
	}
	else
	{
		m_rhiObject.reset();
	}
}

detail::ITexture* Texture3D::resolveRHIObject()
{
	if (m_modified)
	{
		if (m_rhiLockedBuffer)
		{
			LN_NOTIMPLEMENTED();
		}
		else
		{
			ByteBuffer* bmpBuffer = m_bitmap->rawBuffer();
			BoxSizeI bmpSize = { m_bitmap->width(), m_bitmap->height(), m_bitmap->depth() };
			detail::RenderBulkData bmpRawData = manager()->primaryRenderingCommandList()->allocateBulkData(bmpBuffer->size());
			detail::BitmapHelper::blitRawSimple3D(
				bmpRawData.writableData(), bmpBuffer->data(), m_bitmap->width(), m_bitmap->height(), m_bitmap->depth(), Bitmap2D::getPixelFormatByteSize(m_bitmap->format()), false);

			if (!m_rhiObject)
			{
				m_rhiObject = manager()->deviceContext()->createTexture3D(width(), height(), depth(), format(), mipmap(), m_bitmap->data());
			}
			else
			{
				detail::ITexture* rhiObject = m_rhiObject;
				LN_ENQUEUE_RENDER_COMMAND_3(
					Texture3D_setSubData, manager(),
					detail::RenderBulkData, bmpRawData,
					BoxSizeI, bmpSize,
					Ref<detail::ITexture>, rhiObject,
					{
						rhiObject->setSubData3D(0, 0, 0, bmpSize.width, bmpSize.height, bmpSize.depth, bmpRawData.data(), bmpRawData.size());
					});
			}
		}
	}

	if (LN_ENSURE(m_rhiObject)) return nullptr;

	if (m_pool == GraphicsResourcePool::None) {
		m_bitmap.reset();
	}

	m_initialUpdate = false;
	m_modified = false;
	return m_rhiObject;
}


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
	Texture::initialize();
	//m_size.width = width;
	//m_size.height = height;
	//m_requestFormat = requestFormat;
	//m_mipmap = mipmap;
	m_rhiObject = manager()->deviceContext()->createRenderTarget(width, height, requestFormat, mipmap);
	setSize(m_rhiObject->realSize());
	setMipmap(mipmap);
    setFormat(m_rhiObject->getTextureFormat());
}

void RenderTargetTexture::initialize(detail::ITexture* ref)
{
	Texture::initialize();
	m_rhiObject = ref;
	setSize(m_rhiObject->realSize());
    setFormat(m_rhiObject->getTextureFormat());
}

void RenderTargetTexture::dispose()
{
	m_rhiObject.reset();
	Texture::dispose();
}

Ref<Bitmap2D> RenderTargetTexture::readData()
{
	detail::ITexture* rhiObject = resolveRHIObject();

	SizeI size = rhiObject->realSize();
	auto bitmap = newObject<Bitmap2D>(size.width, size.height, GraphicsHelper::translateToPixelFormat(rhiObject->getTextureFormat()));

	if (manager()->renderingType() == RenderingType::Threaded)
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

namespace detail {


//==============================================================================
// TextureHelper

void TextureHelper::setMappedData(Texture2D* texture, const void* data)
{
	Bitmap2D* surface = texture->map(MapMode::Write);
	memcpy(surface->rawBuffer()->data(), data, surface->rawBuffer()->size());
}

} // namespace detail
} // namespace ln
