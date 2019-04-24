
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "../Engine/RenderingCommandList.hpp"
#include "GraphicsDeviceContext.hpp"
#include <LuminoEngine/Graphics/Bitmap.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Graphics/SwapChain.hpp>
#include <LuminoEngine/Font/Font.hpp>
#include "../Font/TextLayoutEngine.hpp"
#include "RenderTargetTextureCache.hpp"

namespace ln {

//==============================================================================
// Texture

Texture::Texture()
	: m_samplerState(nullptr)
{
    m_desc.width = 0;
    m_desc.height = 0;
    m_desc.format = TextureFormat::Unknown;
    m_desc.mipmap = false;
}

Texture::~Texture()
{
}

void Texture::init(/*const detail::TextureDesc& desc*/)
{
	GraphicsResource::init();
    //m_desc = desc;
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

Ref<Texture2D> Texture2D::create(int width, int height)
{
	return newObject<Texture2D>(width, height);
}

Ref<Texture2D> Texture2D::create(int width, int height, TextureFormat format)
{
    return newObject<Texture2D>(width, height, format);
}

Texture2D::Texture2D()
	: m_rhiObject(nullptr)
	, m_usage(GraphicsResourceUsage::Static)
	, m_pool(GraphicsResourcePool::Managed)
	, m_bitmap(nullptr)
	, m_rhiLockedBuffer(nullptr)
	, m_initialUpdate(false)
	, m_modified(true)
{
}

Texture2D::~Texture2D()
{
}

void Texture2D::init(int width, int height)
{
	init(width, height, TextureFormat::RGBA8);
}

void Texture2D::init(int width, int height, TextureFormat format)
{
	Texture::init(/*desc*/);
	m_bitmap = newObject<Bitmap2D>(width, height, GraphicsHelper::translateToPixelFormat(format));
	m_initialUpdate = true;
	m_modified = true;
    detail::TextureDesc desc = { width, height, format, false };
    detail::TextureHelper::setDesc(this, desc);
}

void Texture2D::init(const StringRef& filePath, TextureFormat format)
{
	auto bitmap = newObject<Bitmap2D>();
    bitmap->load(filePath);
    init(bitmap, format);
}

void Texture2D::init(Stream* stream, TextureFormat format)
{
    auto bitmap = newObject<Bitmap2D>();
    bitmap->load(stream);
    init(bitmap, format);
}

void Texture2D::init(Bitmap2D* bitmap, TextureFormat format)
{
    m_bitmap = bitmap;
    Texture::init(/*desc*/);

    detail::TextureDesc desc = { m_bitmap->width(), m_bitmap->height(), format, false };
    detail::TextureHelper::setDesc(this, desc);
    // TODO: check and convert format

    //m_usage = usage;
    m_initialUpdate = true;
    m_modified = true;
    //setSize(SizeI(m_bitmap->width(), m_bitmap->height()));
    //setFormat(format);
    //setMipmap(mipmap);
}

void Texture2D::onDispose(bool explicitDisposing)
{
    m_rhiObject.reset();
    Texture::onDispose(explicitDisposing);
}

void Texture2D::setMipmapEnabled(bool value)
{
	if (mipmap() != value) {
		detail::TextureHelper::setMipmapEnabled(this, value);
		m_modified = true;
	}
}

Bitmap2D* Texture2D::map(MapMode mode)
{
	if (m_initialUpdate && m_usage == GraphicsResourceUsage::Static && m_pool == GraphicsResourcePool::None)
	{
		LN_NOTIMPLEMENTED();
	}

	m_modified = true;
	return m_bitmap;
}

void Texture2D::setResourceUsage(GraphicsResourceUsage usage)
{
	if (m_usage != usage) {
		m_usage = usage;
		m_modified = true;
	}
}

void Texture2D::setResourcePool(GraphicsResourcePool pool)
{
	m_pool = pool;
}

void Texture2D::clear(const Color& color)
{
    Bitmap2D* bitmap = map(MapMode::Write);
    bitmap->clear(ColorI::fromLinearColor(color));
}

void Texture2D::setPixel(int x, int y, const Color& color)
{
    Bitmap2D* bitmap = map(MapMode::Write);
    bitmap->setPixel32(x, y, ColorI::fromLinearColor(color));
}

void Texture2D::blit(int x, int y, Texture2D* srcTexture, int sx, int sy, int sw, int sh)
{
	RectI srcRect(sx, sy, sw, sh);
    if (LN_REQUIRE(srcTexture)) return;
    Bitmap2D* dst = map(MapMode::Write);
    Bitmap2D* src = srcTexture->map(MapMode::Read);
    dst->blit(RectI(x, y, srcRect.getSize()), src, srcRect, ColorI::White, BitmapBlitOptions::AlphaBlend);
}

void Texture2D::drawText(const StringRef& text, const Rect& rect, Font* font, const Color& color, TextAlignment alignment)
{
    Bitmap2D* bitmap = map(MapMode::Write);
    detail::BitmapTextRenderer renderer;
    renderer.render(bitmap, text, rect, font, color, alignment);
}

void Texture2D::onChangeDevice(detail::IGraphicsDevice* device)
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

detail::ITexture* Texture2D::resolveRHIObject(bool* outModified)
{
	*outModified = m_modified;

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
			detail::RenderBulkData bmpRawData = detail::GraphicsResourceInternal::manager(this)->primaryRenderingCommandList()->allocateBulkData(bmpBuffer->size());
			detail::BitmapHelper::blitRawSimple(
				bmpRawData.writableData(), bmpBuffer->data(), m_bitmap->width(), m_bitmap->height(), Bitmap2D::getPixelFormatByteSize(m_bitmap->format()), false);

            detail::IGraphicsDevice* deviceContext = detail::GraphicsResourceInternal::manager(this)->deviceContext();
			if (!m_rhiObject || m_usage != m_rhiObject->usage() || mipmap() != m_rhiObject->mipmap())
			{
				m_rhiObject = deviceContext->createTexture2D(m_usage, width(), height(), format(), mipmap(), m_bitmap->data());
			}
			else
			{
				detail::ITexture* rhiObject = m_rhiObject;
				LN_ENQUEUE_RENDER_COMMAND_4(
					Texture2D_setSubData, detail::GraphicsResourceInternal::manager(this),
                    detail::IGraphicsDevice*, deviceContext,
					detail::RenderBulkData, bmpRawData,
					SizeI, bmpSize,
					Ref<detail::ITexture>, rhiObject,
					{
                        deviceContext->getGraphicsContext()->setSubData2D(rhiObject, 0, 0, bmpSize.width, bmpSize.height, bmpRawData.data(), bmpRawData.size());
					});
			}
		}
	}

	if (LN_ENSURE(m_rhiObject)) return nullptr;

    if (m_usage == GraphicsResourceUsage::Static && m_pool == GraphicsResourcePool::None) {
		m_bitmap.reset();
	}

	m_initialUpdate = false;
	m_modified = false;
	return m_rhiObject;
}

//==============================================================================
// RenderTargetTexture

Ref<RenderTargetTexture> RenderTargetTexture::getTemporary(int width, int height, TextureFormat format, bool mipmap)
{
	return detail::EngineDomain::graphicsManager()->frameBufferCache()->requestRenderTargetTexture2(SizeI(width, height), format, mipmap);
}

void RenderTargetTexture::releaseTemporary(RenderTargetTexture* renderTarget)
{
	detail::EngineDomain::graphicsManager()->frameBufferCache()->release(renderTarget);
}

RenderTargetTexture::RenderTargetTexture()
	: m_rhiObject(nullptr)
    , m_ownerSwapchain(nullptr)
    , m_swapchainImageIndex(-1)
{
}

RenderTargetTexture::~RenderTargetTexture()
{
}

void RenderTargetTexture::init(int width, int height, TextureFormat requestFormat, bool mipmap)
{
	Texture::init(/*detail::TextureDesc()*/);
    m_rhiObject = detail::GraphicsResourceInternal::manager(this)->deviceContext()->createRenderTarget(width, height, requestFormat, mipmap);
    detail::TextureDesc desc = { width, height, m_rhiObject->getTextureFormat(), mipmap };
    //m_desc = desc;
    detail::TextureHelper::setDesc(this, desc);
	//m_size.width = width;
	//m_size.height = height;
	//m_requestFormat = requestFormat;
	//m_mipmap = mipmap;
	//m_rhiObject = detail::GraphicsResourceInternal::manager(this)->deviceContext()->createRenderTarget(width, height, requestFormat, mipmap);
	//setSize(m_rhiObject->realSize());
	//setMipmap(mipmap);
 //   setFormat(m_rhiObject->getTextureFormat());
}

void RenderTargetTexture::init(SwapChain* owner/*, detail::ITexture* ref*/)
{
    //detail::TextureDesc desc = { 0, 0, TextureFormat::Unknown, false };
	Texture::init(/*desc*/);
    m_ownerSwapchain = owner;
    resetSwapchainFrameIfNeeded();
}

void RenderTargetTexture::onDispose(bool explicitDisposing)
{
	m_rhiObject.reset();
	Texture::onDispose(explicitDisposing);
}

Ref<Bitmap2D> RenderTargetTexture::readData()
{
    //manager()->graphicsContext()->flush();

    //manager()->deviceContext()->end();

	bool modified = false;
	detail::ITexture* rhiObject = resolveRHIObject(&modified);

	SizeI size = rhiObject->realSize();
	auto bitmap = newObject<Bitmap2D>(size.width, size.height, GraphicsHelper::translateToPixelFormat(rhiObject->getTextureFormat()));

    //manager()->deviceContext()->begin();

	if (detail::GraphicsResourceInternal::manager(this)->renderingType() == RenderingType::Threaded)
	{
		LN_NOTIMPLEMENTED();
	}
	else
	{
		detail::GraphicsContextInternal::flushCommandRecoding(detail::GraphicsResourceInternal::manager(this)->graphicsContext(), this);
		rhiObject->readData(bitmap->data());
	}

	bitmap->flipVerticalFlow();
	return bitmap;
}

detail::ITexture* RenderTargetTexture::resolveRHIObject(bool* outModified)
{
	*outModified = false;
	return m_rhiObject;
}

void RenderTargetTexture::onChangeDevice(detail::IGraphicsDevice* device)
{
	LN_NOTIMPLEMENTED();
}

// ちょっとイメージしづらいかもしれないけど、これはレガシーAPI の Backbuffer の ReadData を正しくするための仕組み。
// Native Swapchain は内部的に複数の RenderTarget を持っていて。これは通常 present のたびに切り替える。
// で、そうすると present 直後に ReadData して最新の描画結果を得るには、その present 時点の ImageIndex の RenderTarget を取り出さなければならない。
// ImageIndex を Lumino の API として公開するのはちょっとユーザー負担が増えてしまうので、この層でカバーする。
void RenderTargetTexture::resetSwapchainFrameIfNeeded(bool force)
{
    if (m_ownerSwapchain) {
		int imageIndex = detail::SwapChainInternal::imageIndex(m_ownerSwapchain);
        if (m_swapchainImageIndex != imageIndex || force) {
            m_swapchainImageIndex = imageIndex;
            m_rhiObject = detail::GraphicsResourceInternal::resolveRHIObject<detail::ISwapChain>(m_ownerSwapchain, nullptr)->getRenderTarget(m_swapchainImageIndex);
            
            auto size = m_rhiObject->realSize();
            detail::TextureDesc desc = { size.width, size.height, m_rhiObject->getTextureFormat(), false };
            //m_desc.width = size.width;
            //m_desc.height = size.height;
            //m_desc.format = m_rhiObject->getTextureFormat();
            detail::TextureHelper::setDesc(this, desc);
            //setSize(m_rhiObject->realSize());
            //setFormat(m_rhiObject->getTextureFormat());
        }
    }
}

namespace detail {


//==============================================================================
// TextureHelper

void TextureHelper::setMappedData(Texture2D* texture, const void* data)
{
	Bitmap2D* surface = texture->map(MapMode::Write);
	memcpy(surface->rawBuffer()->data(), data, surface->rawBuffer()->size());
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

void Texture3D::init(int width, int height, int depth, TextureFormat format, bool mipmap, GraphicsResourceUsage usage)
{
    detail::TextureDesc desc = { width, height, format, mipmap };
	Texture::init(/*desc*/);
	m_depth = depth;
	m_bitmap = newObject<Bitmap3D>(width, height, depth, GraphicsHelper::translateToPixelFormat(format));
	m_usage = usage;
	m_initialUpdate = true;
	m_modified = true;
    detail::TextureHelper::setDesc(this, desc);
	//setSize(SizeI());
	//setFormat(format);
	//setMipmap(mipmap);
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

void Texture3D::onDispose(bool explicitDisposing)
{
	m_rhiObject.reset();
	Texture::onDispose(explicitDisposing);
}

void Texture3D::onChangeDevice(detail::IGraphicsDevice* device)
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

detail::ITexture* Texture3D::resolveRHIObject(bool* outModified)
{
	*outModified = m_modified;

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
			detail::RenderBulkData bmpRawData = detail::GraphicsResourceInternal::manager(this)->primaryRenderingCommandList()->allocateBulkData(bmpBuffer->size());
			detail::BitmapHelper::blitRawSimple3D(
				bmpRawData.writableData(), bmpBuffer->data(), m_bitmap->width(), m_bitmap->height(), m_bitmap->depth(), Bitmap2D::getPixelFormatByteSize(m_bitmap->format()), false);

			detail::IGraphicsDevice* deviceContext = detail::GraphicsResourceInternal::manager(this)->deviceContext();
			if (!m_rhiObject || m_usage != m_rhiObject->usage())
			{
				m_rhiObject = deviceContext->createTexture3D(m_usage, width(), height(), depth(), format(), mipmap(), m_bitmap->data());
			}
			else
			{
				detail::ITexture* rhiObject = m_rhiObject;
				LN_ENQUEUE_RENDER_COMMAND_4(
					Texture3D_setSubData, detail::GraphicsResourceInternal::manager(this),
					detail::IGraphicsDevice*, deviceContext,
					detail::RenderBulkData, bmpRawData,
					BoxSizeI, bmpSize,
					Ref<detail::ITexture>, rhiObject,
					{
						deviceContext->getGraphicsContext()->setSubData3D(rhiObject, 0, 0, 0, bmpSize.width, bmpSize.height, bmpSize.depth, bmpRawData.data(), bmpRawData.size());
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

} // namespace detail
} // namespace ln
