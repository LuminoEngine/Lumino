
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "RenderingCommandList.hpp"
#include "GraphicsDeviceContext.hpp"
#include <Lumino/Graphics/Bitmap.hpp>
#include <Lumino/Graphics/Texture.hpp>

namespace ln {

//==============================================================================
// Texture

Texture::Texture()
	: m_size(0, 0)
	, m_format(TextureFormat::Unknown)
{
}

Texture::~Texture()
{
}

//==============================================================================
// Texture2D

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
	m_mipmap = mipmap;
	m_initialUpdate = true;
	m_modified = true;
	setSize(SizeI(width, height));
	setFormat(format);
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

void Texture2D::dispose()
{
	m_rhiObject.reset();
	Texture::dispose();
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
				m_rhiObject = manager()->deviceContext()->createTexture2D(width(), height(), format(), m_mipmap, m_bitmap->data());
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

//void Texture2D::resizeInternal(int width, int height)
//{
//	setSize(SizeI(width, height));
//}

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

	SizeI size = rhiObject->realSize();
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
