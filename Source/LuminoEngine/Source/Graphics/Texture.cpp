
#pragma once
#include "../Internal.h"
#include <Lumino/IO/FileManager.h>
#include "../../include/Lumino/Graphics/Texture.h"
#include "GraphicsManager.h"
#include <Lumino/Graphics/Utils.h>
#include <Lumino/Graphics/Text/GlyphRun.h>
#include "CoreGraphicsRenderFeature.h"
#include "Internal.h"
#include "RenderingCommand.h"
#include "RenderingThread.h"
#include "Text/BitmapTextRenderer.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// Texture
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Texture, GraphicsResourceObject);

//------------------------------------------------------------------------------
Texture::Texture()
	: m_deviceObj(NULL)
	, m_size()
	, m_format(TextureFormat::Unknown)
	, m_primarySurface(NULL)
{
}

//------------------------------------------------------------------------------
Texture::~Texture()
{
}

//------------------------------------------------------------------------------
void Texture::Dispose()
{
	LN_SAFE_RELEASE(m_deviceObj);
	GraphicsResourceObject::Dispose();
}

//------------------------------------------------------------------------------
const SizeI& Texture::getSize() const
{
	return m_size;
}

//------------------------------------------------------------------------------
int Texture::getWidth() const
{
	return m_size.width;
}

//------------------------------------------------------------------------------
int Texture::getHeight() const
{
	return m_size.height;
}

//------------------------------------------------------------------------------
//const SizeI& Texture::getRealSize() const
//{
//	return m_deviceObj->getRealSize();
//}

//------------------------------------------------------------------------------
TextureFormat Texture::getFormat() const
{
	return m_format;
}

//------------------------------------------------------------------------------
void Texture::applyModifies()
{
}


//==============================================================================
// Texture2D
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Texture2D, Texture);

//static GraphicsManager* getManager()
//{
//	assert(GraphicsManager::Instance != NULL);
//	return GraphicsManager::Instance;
//}
//static Driver::IGraphicsDevice* getDevice()
//{
//	assert(GraphicsManager::Instance != NULL);
//	return GraphicsManager::Instance->getGraphicsDevice();
//}

//------------------------------------------------------------------------------
Texture2DPtr Texture2D::create(int width, int height, TextureFormat format, bool mipmap)
{
	return create(SizeI(width, height), format, mipmap);
}

//------------------------------------------------------------------------------
Texture2DPtr Texture2D::create(const SizeI& size, TextureFormat format, bool mipmap)
{
	RefPtr<Texture2D> tex(LN_NEW Texture2D(), false);
	tex->initialize(size, format, mipmap, ResourceUsage::Dynamic);
	return tex;
}

//------------------------------------------------------------------------------
Texture2DPtr Texture2D::create(const StringRef& filePath, TextureFormat format, bool mipmap)
{
	RefPtr<Texture2D> tex(LN_NEW Texture2D(), false);
	tex->initialize(filePath, format, mipmap);
	return tex;
}

//------------------------------------------------------------------------------
Texture2DPtr Texture2D::create(Stream* stream, TextureFormat format, bool mipmap)
{
	RefPtr<Texture2D> tex(LN_NEW Texture2D(), false);
	tex->initialize(stream, format, mipmap);
	return tex;
	/*
	if (getManager()->isPlatformTextureLoading())
	{
		RefPtr<Texture2D> tex(LN_NEW Texture2D(), false);
		tex->createImpl(getManager(), stream, format, mipLevels);
		tex.SafeAddRef();
		return tex;
	}

	// ビットマップを作る
	RefPtr<Bitmap> bitmap(LN_NEW Bitmap(stream), false);
	RefPtr<Texture2D> tex(LN_NEW Texture2D(), false);
	tex->createImpl(getManager(), bitmap->GetSize(), format, mipLevels, bitmap);
	tex.SafeAddRef();
	return tex;
	*/
}

//------------------------------------------------------------------------------
Texture2DPtr Texture2D::create(const void* data, size_t size, TextureFormat format, bool mipmap)
{
	MemoryStream stream(data, size);
	return create(&stream, format, mipmap);
}

//------------------------------------------------------------------------------
Texture2DPtr Texture2D::getBlackTexture()
{
	return detail::GraphicsManager::getInstance()->getDummyBlackTexture();
}

//------------------------------------------------------------------------------
Texture2DPtr Texture2D::getWhiteTexture()
{
	return detail::GraphicsManager::getInstance()->getDummyWhiteTexture();
}

//------------------------------------------------------------------------------
Texture2D::Texture2D()
	: m_mipmap(false)
	, m_isPlatformLoaded(false)
	, m_usage(ResourceUsage::Static)
	, m_usageReadFast(false)
	, m_primarySurface2()
	, m_locked(false)
	, m_initializing(false)
{
}

//------------------------------------------------------------------------------
void Texture2D::initialize(const SizeI& size, TextureFormat format, bool mipmap, ResourceUsage usage)
{
	GraphicsResourceObject::initialize();

	m_size = size;
	m_mipmap = mipmap;
	m_format = format;
	m_usage = usage;

	// ロック用のビットマップを作る
	m_primarySurface = LN_NEW Bitmap(size, Utils::translatePixelFormat(format));

	// テクスチャを作る
	m_deviceObj = m_manager->getGraphicsDevice()->createTexture(size, m_mipmap, format, nullptr);

	m_initializing = true;
}

//------------------------------------------------------------------------------
void Texture2D::initialize(int width, int height, TextureFormat format, bool mipmap)
{
	initialize(SizeI(width, height), format, mipmap, ResourceUsage::Dynamic);
}

//------------------------------------------------------------------------------
void Texture2D::initialize(const StringRef& filePath, TextureFormat format, bool mipmap)
{
	RefPtr<Stream> stream(detail::GraphicsManager::getInstance()->getFileManager()->createFileStream(filePath), false);
	initialize(stream, format, mipmap);
}

//------------------------------------------------------------------------------
// プラットフォーム依存用
//------------------------------------------------------------------------------
void Texture2D::initialize(Stream* stream, TextureFormat format, bool mipmap)
{
	GraphicsResourceObject::initialize();
	m_mipmap = mipmap;
	m_format = format;

	if (m_manager->isPlatformTextureLoading())
	{
		m_deviceObj = m_manager->getGraphicsDevice()->createTexturePlatformLoading(stream, m_mipmap, format);
		if (m_deviceObj != NULL)
		{
			SizeI size = m_deviceObj->getSize();
			Bitmap deviceSurface(size, Utils::translatePixelFormat(m_deviceObj->getTextureFormat()), true);
			m_deviceObj->getData(RectI(0, 0, size), deviceSurface.getBitmapBuffer()->getData());

			m_primarySurface2 = RefPtr<Bitmap>::makeRef(m_deviceObj->getSize(), Utils::translatePixelFormat(format));
			m_primarySurface2->bitBlt(RectI(0, 0, size), &deviceSurface, RectI(0, 0, size), Color32::White, false);

			m_size = size;
		}
	}

	// プラットフォーム依存のロードが失敗したら普通の処理
	if (m_deviceObj == NULL)
	{
		m_locked = true;
		m_primarySurface2 = RefPtr<Bitmap>::makeRef(stream);
		m_size = m_primarySurface2->getSize();
	}

#if 0
	m_isPlatformLoaded = true;

	m_deviceObj = GetDevice()->CreateTexturePlatformLoading(stream, mipLevels, format);
	if (m_deviceObj != NULL)
	{
		m_primarySurface = LN_NEW Bitmap(m_deviceObj->GetSize(), Utils::TranslatePixelFormat(format));
		// TODO: m_primarySurface へ内容をフィードバックする
		//Driver::ITexture::ScopedLock lock(m_deviceObj);
		//m_primarySurface->copyRawData(lock.GetBitmap(), m_primarySurface->GetByteCount());
		m_size = m_deviceObj->GetSize();
	}
	// TODO: 失敗したら普通の処理
#endif
	m_initializing = true;
}

//Texture2D::Texture(GraphicsManager* manager, Driver::ITexture* deviceObj, Bitmap* primarySurface)
//	: m_manager(manager)
//	, m_deviceObj(deviceObj)
//	, m_primarySurface(primarySurface)
//	//, m_primarySurfaceModified(false)
//	, m_isDefaultBackBuffer(false)
//{
//	LN_SAFE_ADDREF(m_primarySurface);
//	m_manager->addResourceObject(this);
//}


//------------------------------------------------------------------------------
Texture2D::~Texture2D()
{
	LN_SAFE_RELEASE(m_primarySurface);
}

//------------------------------------------------------------------------------
void Texture2D::tryLock()
{
	if (!m_locked)
	{
		if (m_primarySurface2 == nullptr)
		{
			// フォーマットは DeviceObject 側のフォーマットに合わせる。
			// もし合わせていないと、転送時に同じサイズで DeviceObject 側のフォーマットと同じ Bitmap を
			// いちいち作らなければならなくなる。
			// 基本的に Texture への直接転送は重い体でいるので、ユーザーがあまり Clear や blit を多用しないような想定でいる。
			m_primarySurface2 = RefPtr<Bitmap>::makeRef(m_size, Utils::translatePixelFormat(m_deviceObj->getTextureFormat()));
		}
		m_locked = true;
	}
}

//------------------------------------------------------------------------------
void Texture2D::applyModifies()
{
	if (m_deviceObj == nullptr)
	{
		m_deviceObj = m_manager->getGraphicsDevice()->createTexture(m_primarySurface2->getSize(), m_mipmap, m_format, nullptr);
	}

	if (m_locked)
	{
		ByteBuffer* bmpData = m_primarySurface2->getBitmapBuffer();
		SizeI bmpSize = m_primarySurface2->getSize();

		// 上下反転した一時ビットマップを作ってそれを転送する
		auto* cmdList = m_manager->getPrimaryRenderingCommandList();
		detail::RenderBulkData bmpRawData;
		void* bmpRawDataBuf = bmpRawData.alloc(cmdList, bmpData->getSize());
		Bitmap bmpTmp(bmpRawDataBuf, bmpSize, m_primarySurface2->getPixelFormat(), true);
		bmpTmp.bitBlt(0, 0, m_primarySurface2, RectI(0, 0, bmpSize), Color32::White, false);


		if (m_initializing)
		{
			// まだ1度もコマンドリストに入れられていなければ直接転送できる
			//m_deviceObj->setSubData(PointI::Zero, bmpData->GetConstData(), bmpData->GetSize(), bmpSize);
			m_deviceObj->setSubData(PointI::Zero, bmpRawData.getData(), bmpRawData.getSize(), bmpSize);
		}
		else
		{
			//RenderBulkData bmpRawData(bmpData->GetConstData(), bmpData->GetSize());
			

			
			Driver::ITexture* deviceTexture = m_deviceObj;
			LN_ENQUEUE_RENDER_COMMAND_3(
				Texture3D_ApplyModifies, m_manager,
				detail::RenderBulkData, bmpRawData,
				SizeI, bmpSize,
				RefPtr<Driver::ITexture>, deviceTexture,
				{
					deviceTexture->setSubData(PointI::Zero, bmpRawData.getData(), bmpRawData.getSize(), bmpSize);
				});
		}
		m_initializing = false;
		m_locked = false;

		// static ならメモリを無駄に使わないように解放する
		if (m_usage == ResourceUsage::Static && m_usageReadFast == false)
		{
			m_primarySurface2.safeRelease();
		}
	}
}

//------------------------------------------------------------------------------
void Texture2D::clear(const Color32& color)
{
	tryLock();
	m_primarySurface2->clear(color);
}

//------------------------------------------------------------------------------
void Texture2D::setPixel(int x, int y, const Color& color)
{
	tryLock();
	Color32 c = color.to32BitColor();
	m_primarySurface2->setPixel(x, y, c);
}

//------------------------------------------------------------------------------
void Texture2D::blit(int x, int y, Texture2D* srcTexture, const RectI& srcRect)
{
	if (LN_CHECK_ARG(srcTexture != nullptr)) return;
	if (srcTexture->m_primarySurface2 == nullptr)
	{
		// TODO: ITexture から読み取るべきかもしれない
		LN_NOTIMPLEMENTED();
	}

	// 頻繁な読み取り操作を行うものとしてマークする
	srcTexture->m_usageReadFast = true;

	tryLock();
	m_primarySurface2->bitBlt(RectI(x, y, srcRect.getSize()), srcTexture->m_primarySurface2, srcRect, Color32::White, false);
}

//------------------------------------------------------------------------------
void Texture2D::blt(int x, int y, Bitmap* srcBitmap/*, const RectI& srcRect*/)
{
	tryLock();
	SizeI srcSize = srcBitmap->getSize();
	m_primarySurface2->bitBlt(RectI(x, y, srcSize), srcBitmap, RectI(0, 0, srcSize), Color32::White, false);
}

//------------------------------------------------------------------------------
#pragma push_macro("DrawText")
#undef drawText
void Texture2D::drawText(const StringRef& text, const RectI& rect, Font* font, const Color32& fillColor, const Color32& strokeColor, int strokeThickness, TextAlignment alignment) { LN_AFX_FUNCNAME(drawText)(text, rect, font, fillColor, strokeColor, strokeThickness, alignment); }
void Texture2D::LN_AFX_FUNCNAME(drawText)(const StringRef& text, const RectI& rect, Font* font, const Color32& fillColor, const Color32& strokeColor, int strokeThickness, TextAlignment alignment)
{
	tryLock();
	auto* r = m_manager->getBitmapTextRenderer();
	auto* gr = r->getTempGlyphRun();
	gr->setFont(font->resolveRawFont());
	gr->setText(text);
	r->setRenderArea(rect);
	r->setTextAlignment(alignment);
	r->drawGlyphRun(m_primarySurface2, gr, fillColor, strokeColor, strokeThickness);
}
#pragma pop_macro("DrawText")

//------------------------------------------------------------------------------
void Texture2D::setSubData(const PointI& offset, Bitmap* bitmap)
{
	if (LN_CHECK_ARG(bitmap != nullptr)) return;

	// TODO: 現状、ピクセルフォーマットが一致していることが前提
	if (bitmap->getPixelFormat() != Utils::translatePixelFormat(m_deviceObj->getTextureFormat())) {
		LN_THROW(0, NotImplementedException);
	}

	LN_CALL_TEXTURE_COMMAND(setSubData, SetSubDataTextureCommand,
		offset, bitmap->getBitmapBuffer()->getConstData(), bitmap->getBitmapBuffer()->getSize(), bitmap->getSize());
}

//------------------------------------------------------------------------------
void Texture2D::setData(const void* data)
{
	LN_NOTIMPLEMENTED();
	//// TODO: m_primarySurface にもセット
	//LN_CALL_TEXTURE_COMMAND(setSubData, SetSubDataTextureCommand,
	//	offset, data, m_primarySurface->getBitmapBuffer()->GetSize(), m_deviceObj->GetSize());
}

//------------------------------------------------------------------------------
void Texture2D::onChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == nullptr)
	{
		tryLock();
		m_deviceObj->getData(RectI(0, 0, m_size), m_primarySurface2->getBitmapBuffer()->getData());
		LN_SAFE_RELEASE(m_deviceObj);
	}
	else
	{
		m_deviceObj = device->createTexture(m_primarySurface2->getSize(), m_mipmap, m_format, m_primarySurface2->getBitmapBuffer()->getConstData());
	}


	//if (device == NULL)
	//{
	//	// Immediate のときは lock で取得する必要がある。Deferred のときは m_primarySurface が持っている。
	//	if (m_manager->getRenderingType() == GraphicsRenderingType::Immediate)
	//	{
	//		Driver::ITexture::ScopedLock lock(m_deviceObj);
	//		m_primarySurface->copyRawData(lock.GetBitmap()->getBitmapBuffer()->GetConstData(), m_primarySurface->GetByteCount());
	//		//m_primarySurface->Save(_T("test.png"));
	//	}
	//	LN_SAFE_RELEASE(m_deviceObj);
	//}
	//else
	//{
	//	// この時点では描画モードにかかわらず m_primarySurface がバックアップデータを保持しているのでそれから復元する。
	//	m_deviceObj = device->createTexture(m_primarySurface->GetSize(), m_mipmap, m_format, m_primarySurface->getBitmapBuffer()->GetConstData());
	//	//m_deviceObj->setSubData(PointI(0, 0), m_primarySurface->getBitmapBuffer()->GetConstData(), m_primarySurface->getBitmapBuffer()->GetSize(), m_primarySurface->GetSize());
	//	// TODO: Create でinitialデータも渡してしまう。
	//}
}


//==============================================================================
// Texture3D
//==============================================================================
namespace tr {
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Texture3D, Texture);

//------------------------------------------------------------------------------
Texture3DPtr Texture3D::create(int width, int height, int depth, TextureFormat format, int mipLevels, ResourceUsage usage)
{
	auto ptr = Texture3DPtr::makeRef();
	ptr->initialize(ln::detail::GraphicsManager::getInstance(), width, height, depth, format, mipLevels, usage);
	return ptr;
}

//------------------------------------------------------------------------------
Texture3D::Texture3D()
	: m_depth(0)
	, m_mipLevels(0)
	, m_locked(false)
	, m_initializing(false)
{
}

//------------------------------------------------------------------------------
Texture3D::~Texture3D()
{
}

//------------------------------------------------------------------------------
void Texture3D::initialize(ln::detail::GraphicsManager* manager, int width, int height, int depth, TextureFormat format, int mipLevels, ResourceUsage usage)
{
	GraphicsResourceObject::initialize();
	m_size.width = width;
	m_size.height = height;
	m_depth = depth;
	m_format = format;
	m_mipLevels = mipLevels;
	m_usage = usage;
	m_deviceObj = m_manager->getGraphicsDevice()->createTexture3D(m_size.width, m_size.height, m_depth, m_mipLevels, m_format, m_usage, nullptr);
	m_initializing = true;

	if (m_usage == ResourceUsage::Dynamic)
	{
		m_primarySurface = RefPtr<Bitmap>::makeRef(m_size.width, m_size.height, m_depth, Utils::translatePixelFormat(m_format));
	}
}

//------------------------------------------------------------------------------
int Texture3D::getDepth() const
{
	return m_depth;
}

//------------------------------------------------------------------------------
void Texture3D::setPixel32(int x, int y, int z, const Color32& color)
{
	tryLock();
	m_primarySurface->setPixel(x, y, z, color);
}

//------------------------------------------------------------------------------
void Texture3D::tryLock()
{
	if (!m_locked)
	{
		if (m_usage == ResourceUsage::Static)
		{
			m_primarySurface = RefPtr<Bitmap>::makeRef(m_size.width, m_size.height, m_depth, Utils::translatePixelFormat(m_format));
		}
		m_locked = true;
	}
}

//------------------------------------------------------------------------------
void Texture3D::applyModifies()
{
	if (m_locked)
	{
		ByteBuffer* bmpData = m_primarySurface->getBitmapBuffer();
		if (m_initializing)
		{
			// まだ1度もコマンドリストに入れられていなければ直接転送できる
			m_deviceObj->setSubData3D(Box32::Zero, bmpData->getConstData(), bmpData->getSize());
		}
		else
		{
			ln::detail::RenderBulkData bmpRawData(bmpData->getConstData(), bmpData->getSize());
			Driver::ITexture* deviceTexture = m_deviceObj;
			LN_ENQUEUE_RENDER_COMMAND_2(
				Texture3D_ApplyModifies, m_manager,
				ln::detail::RenderBulkData, bmpRawData,
				RefPtr<Driver::ITexture>, deviceTexture,
				{
					deviceTexture->setSubData3D(Box32::Zero, bmpRawData.getData(), bmpRawData.getSize());
				});
		}
		m_initializing = false;
		m_locked = false;

		if (m_usage == ResourceUsage::Static)
		{
			m_primarySurface.safeRelease();
		}
	}
}

//------------------------------------------------------------------------------
void Texture3D::onChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == nullptr)
	{
		LN_SAFE_RELEASE(m_deviceObj);
	}
	else
	{
		// TODO: 内容の復元
		m_deviceObj = device->createTexture3D(m_size.width, m_size.height, m_depth, m_mipLevels, m_format, m_usage, nullptr);
	}
}

} // namespace tr

//==============================================================================
// RenderTargetTexture
//==============================================================================

//------------------------------------------------------------------------------
RenderTargetTexturePtr RenderTargetTexture::create(const SizeI& size, TextureFormat format, int mipLevels)
{
	RefPtr<RenderTargetTexture> tex(LN_NEW RenderTargetTexture(), false);
	tex->createImpl(detail::GraphicsManager::getInstance(), size, mipLevels, format);
	return tex;
}

//------------------------------------------------------------------------------
RenderTargetTexture::RenderTargetTexture()
	: Texture()
	, m_mipLevels(0)
	, m_isDefaultBackBuffer(false)
	, m_usedCacheOnFrame(false)
{
}

//------------------------------------------------------------------------------
void RenderTargetTexture::initialize(const SizeI& size, int mipLevels, TextureFormat format)
{
	createImpl(detail::EngineDomain::getGraphicsManager(), size, mipLevels, format);
}

//------------------------------------------------------------------------------
void RenderTargetTexture::createImpl(detail::GraphicsManager* manager, const SizeI& size, int mipLevels, TextureFormat format)
{
	GraphicsResourceObject::initialize();
	m_size = size;
	m_mipLevels = mipLevels;
	m_format = format;
	m_deviceObj = m_manager->getGraphicsDevice()->createRenderTarget(m_size.width, m_size.height, m_mipLevels, m_format);
}

//------------------------------------------------------------------------------
void RenderTargetTexture::createCore(detail::GraphicsManager* manager, bool isDefaultBackBuffer)
{
	GraphicsResourceObject::initialize();
	m_deviceObj = NULL;
	m_isDefaultBackBuffer = isDefaultBackBuffer;
}

//------------------------------------------------------------------------------
RenderTargetTexture::~RenderTargetTexture()
{
}

//------------------------------------------------------------------------------
void RenderTargetTexture::attachDefaultBackBuffer(Driver::ITexture* deviceObj)
{
	assert(m_isDefaultBackBuffer);
	LN_REFOBJ_SET(m_deviceObj, deviceObj);
	m_size = m_deviceObj->getSize();
}

//------------------------------------------------------------------------------
void RenderTargetTexture::detachDefaultBackBuffer()
{
	assert(m_isDefaultBackBuffer);
	assert(m_deviceObj != NULL);
	LN_SAFE_RELEASE(m_deviceObj);
}

//------------------------------------------------------------------------------
void RenderTargetTexture::onChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == NULL) {
		LN_SAFE_RELEASE(m_deviceObj);
	}
	else {
		m_deviceObj = m_manager->getGraphicsDevice()->createRenderTarget(m_size.width, m_size.height, m_mipLevels, m_format);
	}
}

////------------------------------------------------------------------------------
//Bitmap* RenderTargetTexture::readSurface()
//{
//	if (m_manager->getRenderingType() == GraphicsRenderingType::Threaded)
//	{
//		Bitmap* surface = nullptr;
//		Bitmap** surfacePtr = &surface;
//		Mutex localMutex;
//		Mutex* localMutexPtr = &localMutex;
//
//		LN_ENQUEUE_RENDER_COMMAND_3(
//			readSurface, m_manager,
//			Driver::ITexture*, m_deviceObj,
//			Bitmap**, surfacePtr,
//			Mutex*, localMutexPtr,
//			{
//				MutexScopedLock lock(*localMutexPtr);
//				*surfacePtr = m_deviceObj->lock();
//			});
//
//		MutexScopedLock lock(*localMutexPtr);
//		return *surfacePtr;
//	}
//	else
//	{
//		return m_deviceObj->lock();
//	}
//}

//------------------------------------------------------------------------------
Bitmap* RenderTargetTexture::lock()
{
	if (m_manager->getRenderingType() == GraphicsRenderingType::Threaded)
	{
		auto* cmdList = m_manager->getRenderer()->m_primaryCommandList;
		cmdList->addCommand<ReadLockTextureCommand>(this);

		// ここでたまっているコマンドをすべて実行する。
		// ReadLockTexture コマンドが実行されると、m_lockedBitmap に
		// ロックしたビットマップがセットされる。
		m_manager->getRenderingThread()->pushRenderingCommand(cmdList);
		cmdList->waitForIdle();
		cmdList->clearCommands();

		return m_primarySurface;
	}
	else
	{
		return m_deviceObj->lock();
	}
}

//------------------------------------------------------------------------------
void RenderTargetTexture::unlock()
{
	if (m_manager->getRenderingType() == GraphicsRenderingType::Threaded)
	{
		auto* cmdList = m_manager->getRenderer()->m_primaryCommandList;
		cmdList->addCommand<ReadUnlockTextureCommand>(this);
		//ReadUnlockTextureCommand::addCommand(cmdList, this);
		//cmdList->ReadUnlockTexture(this);
		m_manager->getRenderingThread()->pushRenderingCommand(cmdList);
		cmdList->waitForIdle();
		cmdList->clearCommands();
	}
	else
	{
		m_deviceObj->unlock();
	}
}


//==============================================================================
// DepthBuffer
//==============================================================================

//------------------------------------------------------------------------------
DepthBufferPtr DepthBuffer::create(const SizeI& size, TextureFormat format)
{
	auto ptr = DepthBufferPtr::makeRef();
	ptr->createImpl(detail::GraphicsManager::getInstance(), size, format);
	return ptr;
}

//------------------------------------------------------------------------------
DepthBuffer::DepthBuffer()
	: m_deviceObj(nullptr)
	, m_size()
	, m_format(TextureFormat::Unknown)
{
}

//------------------------------------------------------------------------------
DepthBuffer::~DepthBuffer()
{
}

//------------------------------------------------------------------------------
void DepthBuffer::createImpl(detail::GraphicsManager* manager, const SizeI& size, TextureFormat format)
{
	GraphicsResourceObject::initialize();
	m_size = size;
	m_format = format;
	refreshDeviceResource();
}

//------------------------------------------------------------------------------
void DepthBuffer::Dispose()
{
	LN_SAFE_RELEASE(m_deviceObj);
	GraphicsResourceObject::Dispose();
}

//------------------------------------------------------------------------------
void DepthBuffer::resize(const SizeI& newSize)
{
	refreshDeviceResource();
}

//------------------------------------------------------------------------------
void DepthBuffer::onChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == nullptr)
	{
		LN_SAFE_RELEASE(m_deviceObj);
	}
	else
	{
		refreshDeviceResource();
	}
}

//------------------------------------------------------------------------------
void DepthBuffer::refreshDeviceResource()
{
	LN_SAFE_RELEASE(m_deviceObj);
	m_deviceObj = m_manager->getGraphicsDevice()->createDepthBuffer(m_size.width, m_size.height, m_format);
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
