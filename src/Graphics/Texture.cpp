
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
const SizeI& Texture::GetSize() const
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
const SizeI& Texture::GetRealSize() const
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
				m_deviceObj, PointI(0, 0), m_primarySurface->GetBitmapBuffer()->GetConstData(), m_primarySurface->GetBitmapBuffer()->GetSize(), m_deviceObj->GetSize());
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
Texture2DPtr Texture2D::Create(int width, int height, TextureFormat format, bool mipmap)
{
	return Create(SizeI(width, height), format, mipmap);
}

//------------------------------------------------------------------------------
Texture2DPtr Texture2D::Create(const SizeI& size, TextureFormat format, bool mipmap)
{
	RefPtr<Texture2D> tex(LN_NEW Texture2D(), false);
	tex->Initialize(detail::GraphicsManager::GetInstance(), size, format, mipmap, ResourceUsage::Dynamic);
	return tex;
}

//------------------------------------------------------------------------------
Texture2DPtr Texture2D::Create(const StringRef& filePath, TextureFormat format, bool mipmap)
{
	RefPtr<Texture2D> tex(LN_NEW Texture2D(), false);
	tex->Initialize(detail::GraphicsManager::GetInstance(), filePath, format, mipmap);
	return tex;
}

//------------------------------------------------------------------------------
Texture2DPtr Texture2D::Create(Stream* stream, TextureFormat format, bool mipmap)
{
	RefPtr<Texture2D> tex(LN_NEW Texture2D(), false);
	tex->Initialize(detail::GraphicsManager::GetInstance(), stream, format, mipmap);
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
Texture2DPtr Texture2D::Create(const void* data, size_t size, TextureFormat format, bool mipmap)
{
	MemoryStream stream(data, size);
	return Create(&stream, format, mipmap);
}

//------------------------------------------------------------------------------
Texture2DPtr Texture2D::GetWhiteTexture()
{
	return detail::GraphicsManager::GetInstance()->GetDummyWhiteTexture();
}

//------------------------------------------------------------------------------
Texture2D::Texture2D()
	: m_mipmap(false)
	, m_isPlatformLoaded(false)
	, m_usage(ResourceUsage::Static)
	, m_primarySurface2()
	, m_locked(false)
	, m_initializing(false)
{
}

//------------------------------------------------------------------------------
void Texture2D::Initialize(detail::GraphicsManager* manager, const SizeI& size, TextureFormat format, bool mipmap, ResourceUsage usage)
{
	GraphicsResourceObject::Initialize(manager);

	m_size = size;
	m_mipmap = mipmap;
	m_format = format;
	m_usage = usage;

	// ロック用のビットマップを作る
	m_primarySurface = LN_NEW Bitmap(size, Utils::TranslatePixelFormat(format));

	// テクスチャを作る
	m_deviceObj = manager->GetGraphicsDevice()->CreateTexture(size, m_mipmap, format, nullptr);

	m_initializing = true;
}

//------------------------------------------------------------------------------
void Texture2D::Initialize(detail::GraphicsManager* manager, const StringRef& filePath, TextureFormat format, bool mipmap)
{
	RefPtr<Stream> stream(manager->GetFileManager()->CreateFileStream(filePath), false);
	Initialize(manager, stream, format, mipmap);
}

//------------------------------------------------------------------------------
// プラットフォーム依存用
//------------------------------------------------------------------------------
void Texture2D::Initialize(detail::GraphicsManager* manager, Stream* stream, TextureFormat format, bool mipmap)
{
	GraphicsResourceObject::Initialize(manager);
	m_mipmap = mipmap;
	m_format = format;

	if (m_manager->IsPlatformTextureLoading())
	{
		m_deviceObj = manager->GetGraphicsDevice()->CreateTexturePlatformLoading(stream, m_mipmap, format);
		if (m_deviceObj != NULL)
		{
			m_primarySurface = LN_NEW Bitmap(m_deviceObj->GetSize(), Utils::TranslatePixelFormat(format));
			m_size = m_deviceObj->GetSize();
		}
	}

	// プラットフォーム依存のロードが失敗したら普通の処理
	if (m_deviceObj == NULL)
	{
		m_primarySurface = LN_NEW Bitmap(stream, true);
		m_deviceObj = manager->GetGraphicsDevice()->CreateTexture(m_primarySurface->GetSize(), m_mipmap, format, m_primarySurface->GetBitmapBuffer()->GetConstData());
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
//	m_manager->AddResourceObject(this);
//}


//------------------------------------------------------------------------------
Texture2D::~Texture2D()
{
	LN_SAFE_RELEASE(m_primarySurface);
}

//------------------------------------------------------------------------------
void Texture2D::TryLock()
{
	if (!m_locked)
	{
		if (m_primarySurface2 == nullptr)
		{
			// フォーマットは DeviceObject 側のフォーマットに合わせる。
			// もし合わせていないと、転送時に同じサイズで DeviceObject 側のフォーマットと同じ Bitmap を
			// いちいち作らなければならなくなる。
			// 基本的に Texture への直接転送は重い体でいるので、ユーザーがあまり Clear や Blit を多用しないような想定でいる。
			m_primarySurface2 = RefPtr<Bitmap>::MakeRef(m_size, Utils::TranslatePixelFormat(m_deviceObj->GetTextureFormat()));
		}
		m_locked = true;
	}
}

//------------------------------------------------------------------------------
void Texture2D::ApplyModifies()
{
	if (m_locked)
	{
		ByteBuffer* bmpData = m_primarySurface2->GetBitmapBuffer();
		SizeI bmpSize = m_primarySurface2->GetSize();

		// 上下反転した一時ビットマップを作ってそれを転送する
		RenderingCommandList* cmdList = m_manager->GetPrimaryRenderingCommandList();
		RenderBulkData bmpRawData;
		void* bmpRawDataBuf = bmpRawData.Alloc(cmdList, bmpData->GetSize());
		Bitmap bmpTmp(bmpRawDataBuf, bmpSize, m_primarySurface2->GetPixelFormat(), true);
		bmpTmp.BitBlt(0, 0, m_primarySurface2, Rect(0, 0, bmpSize), Color32::White, false);


		if (m_initializing)
		{
			// まだ1度もコマンドリストに入れられていなければ直接転送できる
			//m_deviceObj->SetSubData(PointI::Zero, bmpData->GetConstData(), bmpData->GetSize(), bmpSize);
			m_deviceObj->SetSubData(PointI::Zero, bmpRawData.GetData(), bmpRawData.GetSize(), bmpSize);
		}
		else
		{
			//RenderBulkData bmpRawData(bmpData->GetConstData(), bmpData->GetSize());
			

			
			Driver::ITexture* deviceTexture = m_deviceObj;
			LN_ENQUEUE_RENDER_COMMAND_3(
				Texture3D_ApplyModifies, m_manager,
				RenderBulkData, bmpRawData,
				SizeI, bmpSize,
				RefPtr<Driver::ITexture>, deviceTexture,
				{
					deviceTexture->SetSubData(PointI::Zero, bmpRawData.GetData(), bmpRawData.GetSize(), bmpSize);
				});
		}
		m_initializing = false;
		m_locked = false;

		// static ならメモリを無駄に使わないように解放する
		if (m_usage == ResourceUsage::Static)
		{
			m_primarySurface2.SafeRelease();
		}
	}
}

//------------------------------------------------------------------------------
void Texture2D::Clear(const Color32& color)
{
	TryLock();
	m_primarySurface2->Clear(color);
}

//------------------------------------------------------------------------------
void Texture2D::SetPixel(int x, int y, const Color& color)
{
	TryLock();
	Color32 c = color.To32BitColor();
	m_primarySurface2->SetPixel(x, y, c);
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
	PointI point(x, y);
	SizeI srcSize = srcBitmap->GetSize();
	//// TODO: この Blt は基本的にフォントビットマップの転送に使うので、このままだと非常に効率が悪い。が、とりあえずまずは動くものを。
	//ScopedTextureLock lock1(this);
	//lock1.GetBitmap()->BitBlt(x, y, srcBitmap, srcRect, Color::White, true);

	//// ビットマップデータをコマンドリストの一時メモリへ保存する
	//RenderingCommandList* cmdList = m_manager->GetPrimaryRenderingCommandList();
	////size_t data = cmdList->AllocExtData(srcBitmap->GetSerializeSize(srcRect), nullptr);
	////srcBitmap->Serialize(cmdList->GetExtData(data), srcRect);

	//PointI point(x, y);

	//RenderBulkData data;
	//data.Alloc(cmdList, srcBitmap->GetSerializeSize(srcRect));
	//srcBitmap->Serialize(data.GetData(), srcRect);

	//m_deviceObj->SetSubData(point, data.GetData(), )

	RenderingCommandList* cmdList = m_manager->GetPrimaryRenderingCommandList();


	//RenderBulkData bmpPropData;
	//bmpPropData.Alloc(cmdList, srcBitmap->GetPropertySerializeSize());
	//srcBitmap->SerializeProperty(bmpPropData.GetData());

	RenderBulkData bmpRawData;
	void* bmpRawDataBuf = bmpRawData.Alloc(cmdList, Bitmap::GetPixelFormatByteCount(m_primarySurface->GetPixelFormat(), srcBitmap->GetSize(), 1));

	Bitmap bmpTmp(bmpRawDataBuf, srcBitmap->GetSize(), m_primarySurface->GetPixelFormat());
	bmpTmp.BitBlt(0, 0, srcBitmap, Rect(0, 0, srcBitmap->GetSize()), Color32::White, false);

	Driver::ITexture* deviceTexture = m_deviceObj;

	LN_ENQUEUE_RENDER_COMMAND_4(
		Texture2D_Blt_Bitmap, m_manager,
		PointI, point,
		SizeI, srcSize,
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
	TryLock();
	auto* r = m_manager->GetBitmapTextRenderer();
	auto* gr = r->GetTempGlyphRun();
	gr->SetFont(font->ResolveRawFont());
	gr->SetText(text);
	r->SetRenderArea(rect);
	r->SetTextAlignment(alignment);
	r->DrawGlyphRun(m_primarySurface2, gr, fillColor, strokeColor, strokeThickness);
}
#pragma pop_macro("DrawText")

//------------------------------------------------------------------------------
void Texture2D::SetSubData(const PointI& offset, Bitmap* bitmap)
{
	LN_CHECK_ARG(bitmap != nullptr);

	// TODO: 現状、ピクセルフォーマットが一致していることが前提
	if (bitmap->GetPixelFormat() != Utils::TranslatePixelFormat(m_deviceObj->GetTextureFormat())) {
		LN_THROW(0, NotImplementedException);
	}

	LN_CALL_TEXTURE_COMMAND(SetSubData, SetSubDataTextureCommand,
		offset, bitmap->GetBitmapBuffer()->GetConstData(), bitmap->GetBitmapBuffer()->GetSize(), bitmap->GetSize());
}

//------------------------------------------------------------------------------
void Texture2D::SetSubData(const PointI& offset, const void* data)
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
		m_deviceObj = device->CreateTexture(m_primarySurface->GetSize(), m_mipmap, m_format, m_primarySurface->GetBitmapBuffer()->GetConstData());
		//m_deviceObj->SetSubData(PointI(0, 0), m_primarySurface->GetBitmapBuffer()->GetConstData(), m_primarySurface->GetBitmapBuffer()->GetSize(), m_primarySurface->GetSize());
		// TODO: Create でinitialデータも渡してしまう。
	}
}


//==============================================================================
// Texture3D
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Texture3D, Texture);

//------------------------------------------------------------------------------
Texture3DPtr Texture3D::Create(int width, int height, int depth, TextureFormat format, int mipLevels, ResourceUsage usage)
{
	auto ptr = Texture3DPtr::MakeRef();
	ptr->Initialize(detail::GraphicsManager::GetInstance(), width, height, depth, format, mipLevels, usage);
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
void Texture3D::Initialize(detail::GraphicsManager* manager, int width, int height, int depth, TextureFormat format, int mipLevels, ResourceUsage usage)
{
	GraphicsResourceObject::Initialize(manager);
	m_size.width = width;
	m_size.height = height;
	m_depth = depth;
	m_format = format;
	m_mipLevels = mipLevels;
	m_usage = usage;
	m_deviceObj = manager->GetGraphicsDevice()->CreateTexture3D(m_size.width, m_size.height, m_depth, m_mipLevels, m_format, m_usage, nullptr);
	m_initializing = true;

	if (m_usage == ResourceUsage::Dynamic)
	{
		m_primarySurface = RefPtr<Bitmap>::MakeRef(m_size.width, m_size.height, m_depth, Utils::TranslatePixelFormat(m_format));
	}
}

//------------------------------------------------------------------------------
int Texture3D::GetDepth() const
{
	return m_depth;
}

//------------------------------------------------------------------------------
void Texture3D::SetPixel32(int x, int y, int z, const Color32& color)
{
	TryLock();
	m_primarySurface->SetPixel(x, y, z, color);
}

//------------------------------------------------------------------------------
void Texture3D::TryLock()
{
	if (!m_locked)
	{
		if (m_usage == ResourceUsage::Static)
		{
			m_primarySurface = RefPtr<Bitmap>::MakeRef(m_size.width, m_size.height, m_depth, Utils::TranslatePixelFormat(m_format));
		}
		m_locked = true;
	}
}

//------------------------------------------------------------------------------
void Texture3D::ApplyModifies()
{
	if (m_locked)
	{
		ByteBuffer* bmpData = m_primarySurface->GetBitmapBuffer();
		if (m_initializing)
		{
			// まだ1度もコマンドリストに入れられていなければ直接転送できる
			m_deviceObj->SetSubData3D(Box32::Zero, bmpData->GetConstData(), bmpData->GetSize());
		}
		else
		{
			RenderBulkData bmpRawData(bmpData->GetConstData(), bmpData->GetSize());
			Driver::ITexture* deviceTexture = m_deviceObj;
			LN_ENQUEUE_RENDER_COMMAND_2(
				Texture3D_ApplyModifies, m_manager,
				RenderBulkData, bmpRawData,
				RefPtr<Driver::ITexture>, deviceTexture,
				{
					deviceTexture->SetSubData3D(Box32::Zero, bmpRawData.GetData(), bmpRawData.GetSize());
				});
		}
		m_initializing = false;
		m_locked = false;

		if (m_usage == ResourceUsage::Static)
		{
			m_primarySurface.SafeRelease();
		}
	}
}

//------------------------------------------------------------------------------
void Texture3D::OnChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == nullptr)
	{
		LN_SAFE_RELEASE(m_deviceObj);
	}
	else
	{
		// TODO: 内容の復元
		m_deviceObj = device->CreateTexture3D(m_size.width, m_size.height, m_depth, m_mipLevels, m_format, m_usage, nullptr);
	}
}

//==============================================================================
// RenderTarget
//==============================================================================

//------------------------------------------------------------------------------
RenderTargetPtr RenderTarget::Create(const SizeI& size, TextureFormat format, int mipLevels)
{
	RefPtr<RenderTarget> tex(LN_NEW RenderTarget(), false);
	tex->CreateImpl(detail::GraphicsManager::GetInstance(), size, mipLevels, format);
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
void RenderTarget::CreateImpl(detail::GraphicsManager* manager, const SizeI& size, int mipLevels, TextureFormat format)
{
	GraphicsResourceObject::Initialize(manager);

	m_size = size;
	m_mipLevels = mipLevels;
	m_format = format;
	m_deviceObj = m_manager->GetGraphicsDevice()->CreateRenderTarget(m_size.width, m_size.height, m_mipLevels, m_format);
}

//------------------------------------------------------------------------------
void RenderTarget::CreateCore(detail::GraphicsManager* manager, bool isDefaultBackBuffer)
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
Texture* DepthBuffer::Create(const SizeI& size, TextureFormat format)
{
	RefPtr<DepthBuffer> tex(LN_NEW DepthBuffer(), false);
	tex->CreateImpl(detail::GraphicsManager::GetInstance(), size, format);
	tex.SafeAddRef();
	return tex;
}

//------------------------------------------------------------------------------
DepthBuffer::DepthBuffer()
{
}

//------------------------------------------------------------------------------
void DepthBuffer::CreateImpl(detail::GraphicsManager* manager, const SizeI& size, TextureFormat format)
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
void DepthBuffer::Resize(const SizeI& newSize)
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
