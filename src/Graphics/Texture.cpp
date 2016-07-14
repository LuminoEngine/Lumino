
#pragma once
#include "../Internal.h"
#include <Lumino/IO/FileManager.h>
#include "../../include/Lumino/Graphics/Texture.h"
#include "GraphicsManager.h"
#include <Lumino/Graphics/Utils.h>
#include <Lumino/Graphics/Text/GlyphRun.h>
#include "RendererImpl.h"
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
	LN_SAFE_RELEASE(m_deviceObj);
}

//------------------------------------------------------------------------------
const Size& Texture::GetSize() const
{
	return m_deviceObj->GetSize();
}

//------------------------------------------------------------------------------
int Texture::GetWidth() const
{
	return m_deviceObj->GetSize().width;
}

//------------------------------------------------------------------------------
int Texture::GetHeight() const
{
	return m_deviceObj->GetSize().height;
}

//------------------------------------------------------------------------------
const Size& Texture::GetRealSize() const
{
	return m_deviceObj->GetRealSize();
}

//------------------------------------------------------------------------------
TextureFormat Texture::GetFormat() const
{
	return m_deviceObj->GetTextureFormat();
}


//------------------------------------------------------------------------------
Bitmap* Texture::Lock()
{
	// Deferred
	if (m_manager->GetRenderingType() == GraphicsRenderingType::Threaded)
	{
		if (m_deviceObj->GetTextureType() == Driver::TextureType_Normal)
		{
			return m_primarySurface;
		}
		else if (m_deviceObj->GetTextureType() == Driver::TextureType_RenderTarget)
		{
			RenderingCommandList* cmdList = m_manager->GetRenderer()->m_primaryCommandList;
			cmdList->AddCommand<ReadLockTextureCommand>(this);

			// ここでたまっているコマンドをすべて実行する。
			// ReadLockTexture コマンドが実行されると、m_lockedBitmap に
			// ロックしたビットマップがセットされる。
			m_manager->GetRenderingThread()->PushRenderingCommand(cmdList);
			cmdList->WaitForIdle();

			return m_primarySurface;
		}
		return NULL;
	}
	// Immediate
	else
	{
		return m_deviceObj->Lock();
	}
}

//------------------------------------------------------------------------------
void Texture::Unlock()
{
	// Deferred
	if (m_manager->GetRenderingType() == GraphicsRenderingType::Threaded)
	{
		RenderingCommandList* cmdList = m_manager->GetRenderer()->m_primaryCommandList;
		if (m_deviceObj->GetTextureType() == Driver::TextureType_Normal)
		{
			// 変更済みフラグを ON にしておく。
			// あとで本当に使うタイミング (=シェーダパスのApply時) になったら FlushPrimarySurface() が呼ばれる
			//m_primarySurfaceModified = true;

			// TODO: 遅延転送
			//cmdList->SetTextureSubData(m_deviceObj, m_primarySurface);
			//cmdList->AddCommand<SetTextureSubDataCommand>(m_deviceObj, m_primarySurface);
			//SetTextureSubDataCommand::AddCommand(cmdList, m_deviceObj, m_primarySurface);
			cmdList->AddCommand<SetSubDataTextureCommand>(
				m_deviceObj, Point(0, 0), m_primarySurface->GetBitmapBuffer()->GetConstData(), m_primarySurface->GetBitmapBuffer()->GetSize(), m_deviceObj->GetSize());
		}
		else if (m_deviceObj->GetTextureType() == Driver::TextureType_RenderTarget)
		{
			cmdList->AddCommand<ReadUnlockTextureCommand>(this);
			//ReadUnlockTextureCommand::AddCommand(cmdList, this);
			//cmdList->ReadUnlockTexture(this);
			m_manager->GetRenderingThread()->PushRenderingCommand(cmdList);
			cmdList->WaitForIdle();
		}
	}
	// Immediate
	else
	{
		m_deviceObj->Unlock();
	}
}

//==============================================================================
// Texture2D
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Texture2D, Texture);

//static GraphicsManager* GetManager()
//{
//	assert(GraphicsManager::Instance != NULL);
//	return GraphicsManager::Instance;
//}
//static Driver::IGraphicsDevice* GetDevice()
//{
//	assert(GraphicsManager::Instance != NULL);
//	return GraphicsManager::Instance->GetGraphicsDevice();
//}

//------------------------------------------------------------------------------
Texture2DPtr Texture2D::Create(int width, int height, TextureFormat format, int mipLevels)
{
	return Create(Size(width, height), format, mipLevels);
}

//------------------------------------------------------------------------------
Texture2DPtr Texture2D::Create(const Size& size, TextureFormat format, int mipLevels)
{
	RefPtr<Texture2D> tex(LN_NEW Texture2D(), false);
	tex->Initialize(GraphicsManager::GetInstance(), size, format, mipLevels);
	return tex;
}

//------------------------------------------------------------------------------
Texture2DPtr Texture2D::Create(const StringRef& filePath, TextureFormat format, int mipLevels)
{
	RefPtr<Texture2D> tex(LN_NEW Texture2D(), false);
	tex->Initialize(GraphicsManager::GetInstance(), filePath, format, mipLevels);
	return tex;
}

//------------------------------------------------------------------------------
Texture2DPtr Texture2D::Create(Stream* stream, TextureFormat format, int mipLevels)
{
	RefPtr<Texture2D> tex(LN_NEW Texture2D(), false);
	tex->Initialize(GraphicsManager::GetInstance(), stream, format, mipLevels);
	return tex;
	/*
	if (GetManager()->IsPlatformTextureLoading())
	{
		RefPtr<Texture2D> tex(LN_NEW Texture2D(), false);
		tex->CreateImpl(GetManager(), stream, format, mipLevels);
		tex.SafeAddRef();
		return tex;
	}

	// ビットマップを作る
	RefPtr<Bitmap> bitmap(LN_NEW Bitmap(stream), false);
	RefPtr<Texture2D> tex(LN_NEW Texture2D(), false);
	tex->CreateImpl(GetManager(), bitmap->GetSize(), format, mipLevels, bitmap);
	tex.SafeAddRef();
	return tex;
	*/
}

//------------------------------------------------------------------------------
Texture2DPtr Texture2D::Create(const void* data, size_t size, TextureFormat format, int mipLevels)
{
	MemoryStream stream(data, size);
	return Create(&stream, format, mipLevels);
}

//------------------------------------------------------------------------------
Texture2D::Texture2D()
	: m_isPlatformLoaded(false)
{
}

//------------------------------------------------------------------------------
void Texture2D::Initialize(GraphicsManager* manager, const Size& size, TextureFormat format, int mipLevels)
{
	GraphicsResourceObject::Initialize(manager);

	m_size = size;
	m_mipLevels = mipLevels;
	m_format = format;

	// ロック用のビットマップを作る
	m_primarySurface = LN_NEW Bitmap(size, Utils::TranslatePixelFormat(format));

	// テクスチャを作る
	m_deviceObj = GraphicsManager::GetInstance()->GetGraphicsDevice()->CreateTexture(size, mipLevels, format, nullptr);
}

//------------------------------------------------------------------------------
void Texture2D::Initialize(GraphicsManager* manager, const StringRef& filePath, TextureFormat format, int mipLevels)
{
	RefPtr<Stream> stream(manager->GetFileManager()->CreateFileStream(filePath), false);
	Initialize(manager, stream, format, mipLevels);
}

//------------------------------------------------------------------------------
// プラットフォーム依存用
//------------------------------------------------------------------------------
void Texture2D::Initialize(GraphicsManager* manager, Stream* stream, TextureFormat format, int mipLevels)
{
	GraphicsResourceObject::Initialize(manager);
	m_mipLevels = mipLevels;
	m_format = format;

	if (m_manager->IsPlatformTextureLoading())
	{
		m_deviceObj = GraphicsManager::GetInstance()->GetGraphicsDevice()->CreateTexturePlatformLoading(stream, mipLevels, format);
		if (m_deviceObj != NULL)
		{
			m_primarySurface = LN_NEW Bitmap(m_deviceObj->GetSize(), Utils::TranslatePixelFormat(format));
			m_size = m_deviceObj->GetSize();
		}
	}

	// プラットフォーム依存のロードが失敗したら普通の処理
	if (m_deviceObj == NULL)
	{
		m_primarySurface = LN_NEW Bitmap(stream);
		m_deviceObj = GraphicsManager::GetInstance()->GetGraphicsDevice()->CreateTexture(m_primarySurface->GetSize(), mipLevels, format, m_primarySurface->GetBitmapBuffer()->GetConstData());
	}

#if 0
	m_isPlatformLoaded = true;

	m_deviceObj = GetDevice()->CreateTexturePlatformLoading(stream, mipLevels, format);
	if (m_deviceObj != NULL)
	{
		m_primarySurface = LN_NEW Bitmap(m_deviceObj->GetSize(), Utils::TranslatePixelFormat(format));
		// TODO: m_primarySurface へ内容をフィードバックする
		//Driver::ITexture::ScopedLock lock(m_deviceObj);
		//m_primarySurface->CopyRawData(lock.GetBitmap(), m_primarySurface->GetByteCount());
		m_size = m_deviceObj->GetSize();
	}
	// TODO: 失敗したら普通の処理
#endif
}

//Texture2D::Texture(GraphicsManager* manager, Driver::ITexture* deviceObj, Bitmap* primarySurface)
//	: m_manager(manager)
//	, m_deviceObj(deviceObj)
//	, m_primarySurface(primarySurface)
//	//, m_primarySurfaceModified(false)
//	, m_isDefaultBackBuffer(false)
//{
//	LN_SAFE_ADDREF(m_primarySurface);
//	m_manager->AddResourceObject(this);
//}


//------------------------------------------------------------------------------
Texture2D::~Texture2D()
{
	LN_SAFE_RELEASE(m_primarySurface);
}

//------------------------------------------------------------------------------
void Texture2D::Clear(const Color32& color)
{
	ScopedTextureLock lock(this);
	lock.GetBitmap()->Clear(color);
}

//------------------------------------------------------------------------------
void Texture2D::Blt(int x, int y, Texture* srcTexture, const Rect& srcRect)
{
	ScopedTextureLock lock1(this);
	ScopedTextureLock lock2(srcTexture);
	lock1.GetBitmap()->BitBlt(x, y, lock2.GetBitmap(), srcRect, Color32::White, true);
}

//------------------------------------------------------------------------------
void Texture2D::Blt(int x, int y, Bitmap* srcBitmap/*, const Rect& srcRect*/)
{
	Point point(x, y);
	Size srcSize = srcBitmap->GetSize();
	//// TODO: この Blt は基本的にフォントビットマップの転送に使うので、このままだと非常に効率が悪い。が、とりあえずまずは動くものを。
	//ScopedTextureLock lock1(this);
	//lock1.GetBitmap()->BitBlt(x, y, srcBitmap, srcRect, Color::White, true);

	//// ビットマップデータをコマンドリストの一時メモリへ保存する
	//RenderingCommandList* cmdList = m_manager->GetPrimaryRenderingCommandList();
	////size_t data = cmdList->AllocExtData(srcBitmap->GetSerializeSize(srcRect), nullptr);
	////srcBitmap->Serialize(cmdList->GetExtData(data), srcRect);

	//Point point(x, y);

	//RenderBulkData data;
	//data.Alloc(cmdList, srcBitmap->GetSerializeSize(srcRect));
	//srcBitmap->Serialize(data.GetData(), srcRect);

	//m_deviceObj->SetSubData(point, data.GetData(), )

	RenderingCommandList* cmdList = m_manager->GetPrimaryRenderingCommandList();


	//RenderBulkData bmpPropData;
	//bmpPropData.Alloc(cmdList, srcBitmap->GetPropertySerializeSize());
	//srcBitmap->SerializeProperty(bmpPropData.GetData());

	RenderBulkData bmpRawData;
	bmpRawData.Alloc(cmdList, Bitmap::GetPixelFormatByteCount(m_primarySurface->GetPixelFormat(), srcBitmap->GetSize()));

	Bitmap bmpTmp(bmpRawData.GetData(), srcBitmap->GetSize(), m_primarySurface->GetPixelFormat());
	bmpTmp.BitBlt(0, 0, srcBitmap, Rect(0, 0, srcBitmap->GetSize()), Color32::White, false);

	Driver::ITexture* deviceTexture = m_deviceObj;

	LN_ENQUEUE_RENDER_COMMAND_4(
		Texture2D_Blt_Bitmap, m_manager,
		Point, point,
		Size, srcSize,
		//RenderBulkData, bmpPropData,
		RenderBulkData, bmpRawData,
		RefPtr<Driver::ITexture>, deviceTexture,
		{
			deviceTexture->SetSubData(point, bmpRawData.GetData(), bmpRawData.GetSize(), srcSize);
		});

}

//------------------------------------------------------------------------------
#pragma push_macro("DrawText")
#undef DrawText
void Texture2D::DrawText(const StringRef& text, const Rect& rect, Font* font, const Color32& fillColor, const Color32& strokeColor, int strokeThickness, TextAlignment alignment) { LN_AFX_FUNCNAME(DrawText)(text, rect, font, fillColor, strokeColor, strokeThickness, alignment); }
void Texture2D::LN_AFX_FUNCNAME(DrawText)(const StringRef& text, const Rect& rect, Font* font, const Color32& fillColor, const Color32& strokeColor, int strokeThickness, TextAlignment alignment)
{
	ScopedTextureLock lock(this);
	auto* r = m_manager->GetBitmapTextRenderer();
	auto* gr = r->GetTempGlyphRun();
	gr->SetFont(font);
	gr->SetText(text);
	//gr->SetTextAlignment(alignment);
	r->SetRenderArea(rect);
	r->SetTextAlignment(alignment);
	r->DrawGlyphRun(lock.GetBitmap(), gr, fillColor, strokeColor, strokeThickness);
}
#pragma pop_macro("DrawText")

//------------------------------------------------------------------------------
void Texture2D::SetSubData(const Point& offset, Bitmap* bitmap)
{
	LN_CHECK_ARG(bitmap != NULL);

	// TODO: 現状、ピクセルフォーマットが一致していることが前提
	if (bitmap->GetPixelFormat() != Utils::TranslatePixelFormat(m_deviceObj->GetTextureFormat())) {
		LN_THROW(0, NotImplementedException);
	}

	LN_CALL_TEXTURE_COMMAND(SetSubData, SetSubDataTextureCommand,
		offset, bitmap->GetBitmapBuffer()->GetConstData(), bitmap->GetBitmapBuffer()->GetSize(), bitmap->GetSize());
}

//------------------------------------------------------------------------------
void Texture2D::SetSubData(const Point& offset, const void* data)
{
	LN_CHECK_ARG(data != NULL);
	// TODO: m_primarySurface にもセット
	LN_CALL_TEXTURE_COMMAND(SetSubData, SetSubDataTextureCommand,
		offset, data, m_primarySurface->GetBitmapBuffer()->GetSize(), m_deviceObj->GetSize());
}

//------------------------------------------------------------------------------
void Texture2D::OnChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == NULL)
	{
		// Immediate のときは Lock で取得する必要がある。Deferred のときは m_primarySurface が持っている。
		if (m_manager->GetRenderingType() == GraphicsRenderingType::Immediate)
		{
			Driver::ITexture::ScopedLock lock(m_deviceObj);
			m_primarySurface->CopyRawData(lock.GetBitmap()->GetBitmapBuffer()->GetConstData(), m_primarySurface->GetByteCount());
			//m_primarySurface->Save(_T("test.png"));
		}
		LN_SAFE_RELEASE(m_deviceObj);
	}
	else
	{
		// この時点では描画モードにかかわらず m_primarySurface がバックアップデータを保持しているのでそれから復元する。
		m_deviceObj = device->CreateTexture(m_primarySurface->GetSize(), m_mipLevels, m_format, m_primarySurface->GetBitmapBuffer()->GetConstData());
		//m_deviceObj->SetSubData(Point(0, 0), m_primarySurface->GetBitmapBuffer()->GetConstData(), m_primarySurface->GetBitmapBuffer()->GetSize(), m_primarySurface->GetSize());
		// TODO: Create でinitialデータも渡してしまう。
	}
}


//==============================================================================
// RenderTarget
//==============================================================================

//------------------------------------------------------------------------------
RenderTargetPtr RenderTarget::Create(const Size& size, TextureFormat format, int mipLevels)
{
	RefPtr<RenderTarget> tex(LN_NEW RenderTarget(), false);
	tex->CreateImpl(GraphicsManager::GetInstance(), size, mipLevels, format);
	return tex;
}

//------------------------------------------------------------------------------
RenderTarget::RenderTarget()
	: Texture()
	, m_mipLevels(0)
	, m_isDefaultBackBuffer(false)
	, m_usedCacheOnFrame(false)
{
}

//------------------------------------------------------------------------------
void RenderTarget::CreateImpl(GraphicsManager* manager, const Size& size, int mipLevels, TextureFormat format)
{
	GraphicsResourceObject::Initialize(manager);

	m_size = size;
	m_mipLevels = mipLevels;
	m_format = format;
	m_deviceObj = m_manager->GetGraphicsDevice()->CreateRenderTarget(m_size.width, m_size.height, m_mipLevels, m_format);
}

//------------------------------------------------------------------------------
void RenderTarget::CreateCore(GraphicsManager* manager, bool isDefaultBackBuffer)
{
	GraphicsResourceObject::Initialize(manager);
	m_deviceObj = NULL;
	//m_primarySurface = NULL;
	m_isDefaultBackBuffer = isDefaultBackBuffer;
}

//------------------------------------------------------------------------------
RenderTarget::~RenderTarget()
{
}

//------------------------------------------------------------------------------
void RenderTarget::AttachDefaultBackBuffer(Driver::ITexture* deviceObj)
{
	assert(m_isDefaultBackBuffer);
	assert(m_deviceObj == NULL);
	LN_REFOBJ_SET(m_deviceObj, deviceObj);
}

//------------------------------------------------------------------------------
void RenderTarget::DetachDefaultBackBuffer()
{
	assert(m_isDefaultBackBuffer);
	assert(m_deviceObj != NULL);
	LN_SAFE_RELEASE(m_deviceObj);
}

//------------------------------------------------------------------------------
void RenderTarget::OnChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == NULL) {
		LN_SAFE_RELEASE(m_deviceObj);
	}
	else {
		m_deviceObj = m_manager->GetGraphicsDevice()->CreateRenderTarget(m_size.width, m_size.height, m_mipLevels, m_format);
	}
}

//==============================================================================
// DepthBuffer
//==============================================================================

//------------------------------------------------------------------------------
Texture* DepthBuffer::Create(const Size& size, TextureFormat format)
{
	RefPtr<DepthBuffer> tex(LN_NEW DepthBuffer(), false);
	tex->CreateImpl(GraphicsManager::GetInstance(), size, format);
	tex.SafeAddRef();
	return tex;
}

//------------------------------------------------------------------------------
DepthBuffer::DepthBuffer()
{
}

//------------------------------------------------------------------------------
void DepthBuffer::CreateImpl(GraphicsManager* manager, const Size& size, TextureFormat format)
{
	GraphicsResourceObject::Initialize(manager);
	m_size = size;
	m_format = format;
	RefreshDeviceResource();
}

//------------------------------------------------------------------------------
DepthBuffer::~DepthBuffer()
{
}

//------------------------------------------------------------------------------
void DepthBuffer::Resize(const Size& newSize)
{
	m_size = newSize;
	RefreshDeviceResource();
}

//------------------------------------------------------------------------------
void DepthBuffer::OnChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == NULL) {
		LN_SAFE_RELEASE(m_deviceObj);
	}
	else {
		RefreshDeviceResource();
	}
}

//------------------------------------------------------------------------------
void DepthBuffer::RefreshDeviceResource()
{
	LN_SAFE_RELEASE(m_deviceObj);
	m_deviceObj = m_manager->GetGraphicsDevice()->CreateDepthBuffer(m_size.width, m_size.height, m_format);
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
