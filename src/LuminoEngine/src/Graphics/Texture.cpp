﻿
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "GraphicsDeviceContext.hpp"
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Graphics/Bitmap.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Graphics/SwapChain.hpp>
#include <LuminoEngine/Font/Font.hpp>
#include <LuminoEngine/Asset/Assets.hpp>
#include "../Asset/AssetManager.hpp"
#include "../Font/TextLayoutEngine.hpp"
#include "../Font/FontManager.hpp"
#include "RenderTargetTextureCache.hpp"

namespace ln {

//==============================================================================
// Texture

LN_OBJECT_IMPLEMENT(Texture, AssetObject) {}

Texture::Texture()
    : m_manager(nullptr)
    , m_width(0)
    , m_height(0)
    , m_format(TextureFormat::Unknown)
    , m_mipmap(false)
    , m_samplerState(nullptr)
{
}

Texture::~Texture()
{
}

void Texture::init()
{
    AssetObject::init();
    detail::GraphicsResourceInternal::initializeHelper_GraphicsResource(this, &m_manager);
}

void Texture::onDispose(bool explicitDisposing)
{
    detail::GraphicsResourceInternal::finalizeHelper_GraphicsResource(this, &m_manager);
    AssetObject::onDispose(explicitDisposing);
}

void Texture::setDesc(int width, int height, TextureFormat format)
{
    m_width = width;
    m_height = height;
    m_format = format;
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

LN_OBJECT_IMPLEMENT(Texture2D, Texture) {}

Ref<Texture2D> Texture2D::create(int width, int height)
{
    return makeObject<Texture2D>(width, height);
}

Ref<Texture2D> Texture2D::create(int width, int height, TextureFormat format)
{
    return makeObject<Texture2D>(width, height, format);
}

//Ref<Texture2D> Texture2D::create(const StringRef& filePath, TextureFormat format)
//{
//	return makeObject<Texture2D>(filePath, format);
//}

Ref<Texture2D> Texture2D::load(const StringRef& filePath)
{
    return detail::EngineDomain::graphicsManager()->loadTexture2D(filePath);
}

Ref<Texture2DPromise> Texture2D::loadAsync(const StringRef& filePath)
{
    // TODO: 排他処理が必要
    LN_NOTIMPLEMENTED();

    return Texture2DPromise::run([filePath](Texture2DPromise* p) {
        p->resolve(load(filePath));
    });
}

Ref<Texture2D> Texture2D::loadEmoji(StringRef code)
{
	// TODO: String.toUTF32() 作ってもいいかも
	String str = code;
	std::vector<byte_t> bytes = TextEncoding::utf32Encoding()->encode(str);
	if (LN_REQUIRE(bytes.size() == 4)) return nullptr;

	return loadEmoji(*reinterpret_cast<const uint32_t*>(bytes.data()));
}

// Note: NotoColorEmoji は bitmap サイズ1種類固定で格納されているため、スケーリング不可能。
// Bitmap として取得する場合は常に同じサイズとなる。
Ref<Texture2D> Texture2D::loadEmoji(uint32_t codePoint)
{
	Font* font = detail::EngineDomain::fontManager()->emojiFont();
	if (font) {
		detail::FontCore* core = detail::FontHelper::resolveFontCore(font, 1.0f);

		// TODO: NotoColorEmoji に限っては bitmap をキャッシュしていい気がする。(すべてが同一サイズの bitmap である)
		detail::BitmapGlyphInfo info;
		info.loadColor = true;
		core->lookupGlyphBitmap(codePoint, &info);

		auto texture = makeObject<Texture2D>(info.glyphBitmap->clone(), GraphicsHelper::translateToTextureFormat(info.glyphBitmap->format()));
		texture->setSamplerState(detail::EngineDomain::graphicsManager()->linearSamplerState());
		return texture;
	}
	else {
		LN_WARNING(u"Emoji font not loaded.\n");
		return nullptr;
	}
}

Texture2D* Texture2D::blackTexture()
{
	return detail::EngineDomain::graphicsManager()->blackTexture();
}

Texture2D* Texture2D::whiteTexture()
{
	return detail::EngineDomain::graphicsManager()->whiteTexture();
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

bool Texture2D::init()
{
    Texture::init();
    return true;
}

void Texture2D::init(int width, int height)
{
    init(width, height, TextureFormat::RGBA8);
}

void Texture2D::init(int width, int height, TextureFormat format)
{
    width = std::max(1, width);
    height = std::max(1, height);
    init();
    m_bitmap = makeObject<Bitmap2D>(width, height, GraphicsHelper::translateToPixelFormat(format));
    m_initialUpdate = true;
    m_modified = true;
    detail::TextureInternal::setDesc(this, width, height, format);
}

//bool Texture2D::init(const Path& assetPath)
//{
//    // TODO: 2回initやめたほうがいい
//    if (!init()) return false;
//    if (!initLoad(assetPath)) return false;
//    return true;
//
//    //if (!init()) return false;
//
//    //if (detail::AssetPath::isAssetFilePath(assetPath))
//    //    setAssetPath(assetPath);
//    //else
//    //    m_sourceFilePath = detail::AssetPath::resolveAssetPath(assetPath, GraphicsHelper::CandidateExts_Texture2D);
//    //
//    //reload();
//
//    //return true;
//}
//
void Texture2D::init(Stream* stream, TextureFormat format)
{
    auto bitmap = makeObject<Bitmap2D>();
    bitmap->load(stream);
    init(bitmap, format);
}

void Texture2D::init(Bitmap2D* bitmap, TextureFormat format)
{
    m_bitmap = bitmap;
    init();

    detail::TextureInternal::setDesc(this, m_bitmap->width(), m_bitmap->height(), format);
    // TODO: check and convert format

    m_initialUpdate = true;
    m_modified = true;
}

void Texture2D::onDispose(bool explicitDisposing)
{
    m_rhiObject.reset();
    Texture::onDispose(explicitDisposing);
}

void Texture2D::setMipmapEnabled(bool value)
{
    if (mipmap() != value) {
        detail::TextureInternal::setMipmapEnabled(this, value);
        m_modified = true;
    }
}

Bitmap2D* Texture2D::map(MapMode mode)
{
    if (m_initialUpdate && m_usage == GraphicsResourceUsage::Static && m_pool == GraphicsResourcePool::None) {
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
    if (device) {
        if (m_pool == GraphicsResourcePool::Managed) {
            // data is transferred by the next resolveRHIObject()
            m_modified = true;
        }
    } else {
        m_rhiObject.reset();
    }
}

detail::ITexture* Texture2D::resolveRHIObject(GraphicsContext* context, bool* outModified)
{
    *outModified = m_modified;

    if (m_modified) {
        if (m_rhiLockedBuffer) {
            LN_NOTIMPLEMENTED();
        } else {
            detail::IGraphicsDevice* deviceContext = detail::GraphicsResourceInternal::manager(this)->deviceContext();

            ByteBuffer* bmpBuffer = m_bitmap->rawBuffer();
            SizeI bmpSize(m_bitmap->width(), m_bitmap->height());
            detail::RenderBulkData bmpRawData = detail::GraphicsContextInternal::getRenderingCommandList(context)->allocateBulkData(bmpBuffer->size());
            detail::BitmapHelper::blitRawSimple(
                bmpRawData.writableData(), bmpBuffer->data(), m_bitmap->width(), m_bitmap->height(), GraphicsHelper::getPixelSize(m_bitmap->format()), deviceContext->caps().imageLayoytVFlip);

            if (!m_rhiObject || m_usage != m_rhiObject->usage() || mipmap() != m_rhiObject->mipmap()) {
                m_rhiObject = deviceContext->createTexture2D(m_usage, width(), height(), format(), mipmap(), bmpRawData.data());
            } else {
                context->interruptCurrentRenderPassFromResolveRHI();
                detail::ITexture* rhiObject = m_rhiObject;
				auto commandList = detail::GraphicsContextInternal::getCommandListForTransfer(context);
                LN_ENQUEUE_RENDER_COMMAND_4(
                    Texture2D_setSubData, context,
                    detail::ICommandList*, commandList,
                    detail::RenderBulkData, bmpRawData,
                    SizeI, bmpSize,
                    Ref<detail::ITexture>, rhiObject,
                    {
						commandList->setSubData2D(rhiObject, 0, 0, bmpSize.width, bmpSize.height, bmpRawData.data(), bmpRawData.size());
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

void Texture2D::serialize(Serializer2& ar)
{
    Texture::serialize(ar);


    //// TODO: Object::assetPath 使いたい
    //String path;// =// m_assetSourcePath;
    //if (ar.isSaving() && !m_sourceFilePath.isNull()) {
    //    // save to relative path.
    //    // TODO: 毎回 parseAssetPath するのはアレなので、ar.basePath() の型を AssetPath にしたいところ。
    //    path = detail::AssetPath::makeRelativePath(ar.basePath(), m_sourceFilePath);
    //}

    //ar & makeNVP(u"file", path);

    //if (ar.isLoading()) {
    //    if (!path.isEmpty()) {
    //        // convert relative path to full path.
    //        m_sourceFilePath = detail::AssetPath::resolveAssetPath(
    //            detail::AssetPath::combineAssetPath(ar.basePath(), path),
    //            GraphicsHelper::CandidateExts_Texture2D);
    //    }
    //}
}

const std::vector<const Char*>& Texture2D::resourceExtensions() const
{
    static const std::vector<const Char*> exts = { u".png", u".jpg", u".tga", u".bmp", u".gif" };
    return exts;
}

void Texture2D::onLoadResourceFile(Stream* stream, const detail::AssetPath& assetPath)
{
    auto bitmap = makeObject<Bitmap2D>();
    bitmap->load(stream);

    m_bitmap = bitmap;

    // TODO: format
    detail::TextureInternal::setDesc(this, m_bitmap->width(), m_bitmap->height(), TextureFormat::RGBA8);
    // TODO: check and convert format

    m_initialUpdate = true;
    m_modified = true;
}

//void Texture2D::onLoadResourceFile()
//{
//    assert(!m_sourceFilePath.isNull());
//
//    auto assetManager = detail::GraphicsResourceInternal::manager(this)->assetManager();
//    auto stream = assetManager->openStreamFromAssetPath(m_sourceFilePath);
//    if (stream) {
//        auto bitmap = makeObject<Bitmap2D>();
//        bitmap->load(stream);
//
//        m_bitmap = bitmap;
//
//        // TODO: format
//        detail::TextureInternal::setDesc(this, m_bitmap->width(), m_bitmap->height(), TextureFormat::RGBA8);
//        // TODO: check and convert format
//
//        m_initialUpdate = true;
//        m_modified = true;
//    }
//    else {
//        LN_WARNING(u"Asset not found: " + m_sourceFilePath.toString());    // TODO: operator
//    }
//}

//==============================================================================
// RenderTargetTexture

Ref<RenderTargetTexture> RenderTargetTexture::create(int width, int height)
{
    return makeObject<RenderTargetTexture>(width, height);
}

Ref<RenderTargetTexture> RenderTargetTexture::create(int width, int height, TextureFormat format)
{
    return makeObject<RenderTargetTexture>(width, height, format);
}

Ref<RenderTargetTexture> RenderTargetTexture::getTemporary(int width, int height, TextureFormat format, bool mipmap)
{
    return detail::EngineDomain::graphicsManager()->frameBufferCache()->requestRenderTargetTexture2(SizeI(width, height), format, mipmap);
}

void RenderTargetTexture::releaseTemporary(RenderTargetTexture* renderTarget)
{
    detail::EngineDomain::graphicsManager()->frameBufferCache()->release(renderTarget);
}

Ref<RenderTargetTexture> RenderTargetTexture::realloc(RenderTargetTexture* renderTarget, int width, int height, TextureFormat format, bool mipmap, SamplerState* samplerState)
{
    if (!renderTarget ||
        renderTarget->width() != width ||
        renderTarget->height() != height ||
        renderTarget->format() != format ||
        renderTarget->mipmap() != mipmap) { // NOTE: SamplerState はチェック不要。というかどこかで変えたのを戻し忘れると毎回 RenderTarget 作り直しになってしまう
        auto ptr = makeObject<RenderTargetTexture>(width, height, format, mipmap);
        ptr->setSamplerState(samplerState);
        return ptr;
    }
    else {
        return renderTarget;
    }
}

RenderTargetTexture::RenderTargetTexture()
    : m_rhiObject(nullptr)
    , m_ownerSwapchain(nullptr)
    , m_nativeObject(0)
    , m_modified(true)
    , m_hasNativeObject(false)
{
}

RenderTargetTexture::~RenderTargetTexture()
{
}

void RenderTargetTexture::init(int width, int height)
{
    init(width, height, TextureFormat::RGBA8, false);
}

void RenderTargetTexture::init(int width, int height, TextureFormat format)
{
    init(width, height, format, true);
}

void RenderTargetTexture::init(int width, int height, TextureFormat format, bool mipmap)
{
    width = std::max(1, width);
    height = std::max(1, height);
    Texture::init();
    detail::TextureInternal::setDesc(this, width, height, format);
    detail::TextureInternal::setMipmapEnabled(this, mipmap);
}

void RenderTargetTexture::init(SwapChain* owner)
{
    Texture::init();
    m_ownerSwapchain = owner;
}

void RenderTargetTexture::init(intptr_t nativeObject, TextureFormat format)
{
    Texture::init();
    detail::TextureInternal::setDesc(this, 1, 1, format);
    resetNativeObject(nativeObject);
}

void RenderTargetTexture::resetNativeObject(intptr_t nativeObject)
{
    m_nativeObject = nativeObject;
    m_hasNativeObject = true;
}

void RenderTargetTexture::resetSize(int width, int height)
{
    width = std::max(1, width);
    height = std::max(1, height);
    detail::TextureInternal::setDesc(this, width, height, format());
}

void RenderTargetTexture::resetRHIObject(detail::ITexture* rhiObject)
{
	LN_CHECK(rhiObject);
	m_rhiObject = rhiObject;
	auto size = m_rhiObject->realSize();
	detail::TextureInternal::setDesc(this, size.width, size.height, m_rhiObject->getTextureFormat());
	m_modified = false;
}

void RenderTargetTexture::onDispose(bool explicitDisposing)
{
    m_rhiObject.reset();
    Texture::onDispose(explicitDisposing);
}

Ref<Bitmap2D> RenderTargetTexture::readData(GraphicsContext* context)
{
    bool modified = false;
    detail::ITexture* rhiObject = resolveRHIObject(nullptr, &modified);

    SizeI size = rhiObject->realSize();
    auto bitmap = makeObject<Bitmap2D>(size.width, size.height, GraphicsHelper::translateToPixelFormat(rhiObject->getTextureFormat()));

  //  if (detail::GraphicsContextInternal::getRenderingType(context) == RenderingType::Threaded) {
  //      LN_NOTIMPLEMENTED();
  //  } else {
  ////      detail::GraphicsContextInternal::flushCommandRecoding(context, this);
		////detail::GraphicsResourceInternal::manager(this)->renderingQueue()->submit(context);
        rhiObject->readData(bitmap->data());
  //  }

    detail::IGraphicsDevice* deviceContext = detail::GraphicsResourceInternal::manager(this)->deviceContext();
    if (deviceContext->caps().imageLayoytVFlip) {
        bitmap->flipVerticalFlow();
    }
    //
    return bitmap;
}

detail::ITexture* RenderTargetTexture::resolveRHIObject(GraphicsContext* context, bool* outModified)
{
    *outModified = m_modified;

    if (m_modified) {
        if (m_hasNativeObject) {
            m_rhiObject = detail::GraphicsResourceInternal::manager(this)->deviceContext()->createWrappedRenderTarget(m_nativeObject, width(), height());
        }
        else {
            m_rhiObject = detail::GraphicsResourceInternal::manager(this)->deviceContext()->createRenderTarget(width(), height(), format(), mipmap(), false);
        }
    }

    m_modified = false;
    return m_rhiObject;
}

void RenderTargetTexture::onChangeDevice(detail::IGraphicsDevice* device)
{
    LN_NOTIMPLEMENTED();
}

namespace detail {

//==============================================================================
// TextureInternal

void TextureInternal::setMappedData(Texture2D* texture, const void* data)
{
    Bitmap2D* surface = texture->map(MapMode::Write);
    memcpy(surface->rawBuffer()->data(), data, surface->rawBuffer()->size());
}

void TextureInternal::resetOpenGLTextureIdFromCurrentFramebuffer(RenderTargetTexture* renderTarget)
{
	int id;
	if (IGraphicsDevice::getOpenGLCurrentFramebufferTextureId(&id)) {
		renderTarget->resetNativeObject(id);
	}
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
    Texture::init();
    m_depth = depth;
    m_bitmap = makeObject<Bitmap3D>(width, height, depth, GraphicsHelper::translateToPixelFormat(format));
    m_usage = usage;
    m_initialUpdate = true;
    m_modified = true;
    detail::TextureInternal::setDesc(this, width, height, format);
    detail::TextureInternal::setMipmapEnabled(this, mipmap);
}

Bitmap3D* Texture3D::map(MapMode mode)
{
    if (m_initialUpdate && m_pool == GraphicsResourcePool::None) {
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
    if (device) {
        if (m_pool == GraphicsResourcePool::Managed) {
            // data is transferred by the next resolveRHIObject()
            m_modified = true;
        }
    } else {
        m_rhiObject.reset();
    }
}

detail::ITexture* Texture3D::resolveRHIObject(GraphicsContext* context, bool* outModified)
{
    *outModified = m_modified;

    if (m_modified) {
        if (m_rhiLockedBuffer) {
            LN_NOTIMPLEMENTED();
        } else {
            detail::IGraphicsDevice* deviceContext = detail::GraphicsResourceInternal::manager(this)->deviceContext();

            ByteBuffer* bmpBuffer = m_bitmap->rawBuffer();
            BoxSizeI bmpSize = {m_bitmap->width(), m_bitmap->height(), m_bitmap->depth()};
            detail::RenderBulkData bmpRawData = detail::GraphicsContextInternal::getRenderingCommandList(context)->allocateBulkData(bmpBuffer->size());
            detail::BitmapHelper::blitRawSimple3D(
                bmpRawData.writableData(), bmpBuffer->data(), m_bitmap->width(), m_bitmap->height(), m_bitmap->depth(), GraphicsHelper::getPixelSize(m_bitmap->format()), deviceContext->caps().imageLayoytVFlip);

            if (!m_rhiObject || m_usage != m_rhiObject->usage()) {
                m_rhiObject = deviceContext->createTexture3D(m_usage, width(), height(), depth(), format(), mipmap(), m_bitmap->data());
            } else {
                detail::ITexture* rhiObject = m_rhiObject;
				auto commandList = detail::GraphicsContextInternal::getCommandListForTransfer(context);
                LN_ENQUEUE_RENDER_COMMAND_4(
                    Texture3D_setSubData, context,
                    detail::ICommandList*, commandList,
                    detail::RenderBulkData, bmpRawData,
                    BoxSizeI, bmpSize,
                    Ref<detail::ITexture>, rhiObject, {
						commandList->setSubData3D(rhiObject, 0, 0, 0, bmpSize.width, bmpSize.height, bmpSize.depth, bmpRawData.data(), bmpRawData.size());
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
