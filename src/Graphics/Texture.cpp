
#pragma once

#include "../Internal.h"
#include "../../include/Lumino/Graphics/Texture.h"
#include "../../include/Lumino/Graphics/GraphicsManager.h"
#include <Lumino/Graphics/Utils.h>
#include "RendererImpl.h"
#include "Internal.h"
#include "RenderingCommand.h"
#include "RenderingThread.h"
#include "GraphicsHelper.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
	
//=============================================================================
// Texture
//=============================================================================

static GraphicsManager* GetManager()
{
	assert(GraphicsManager::Instance != NULL);
	return GraphicsManager::Instance;
}
static Driver::IGraphicsDevice* GetDevice()
{
	assert(GraphicsManager::Instance != NULL);
	return GraphicsManager::Instance->GetGraphicsDevice();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* Texture::Create(const Size& size, TextureFormat format, int mipLevels)
{
	// ロック用のビットマップを作る
	RefPtr<Bitmap> bitmap(LN_NEW Bitmap(size, Utils::TranslatePixelFormat(format)), false);
	RefPtr<Texture> tex(LN_NEW Texture(), false);
	tex->CreateImpl(GetManager(), size, format, mipLevels, bitmap);
	tex.SafeAddRef();
	return tex;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* Texture::Create(const TCHAR* filePath, TextureFormat format, int mipLevels)
{
	RefPtr<Stream> stream(GetManager()->GetFileManager()->CreateFileStream(filePath), false);
	return Create(stream, format, mipLevels);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* Texture::Create(Stream* stream, TextureFormat format, int mipLevels)
{
	if (GetManager()->IsPlatformTextureLoading())
	{
		RefPtr<Texture> tex(LN_NEW Texture(), false);
		tex->CreateImpl(GetManager(), stream, format, mipLevels);
		tex.SafeAddRef();
		return tex;
	}

	// ビットマップを作る
	RefPtr<Bitmap> bitmap(LN_NEW Bitmap(stream), false);
	RefPtr<Texture> tex(LN_NEW Texture(), false);
	tex->CreateImpl(GetManager(), bitmap->GetSize(), format, mipLevels, bitmap);
	tex.SafeAddRef();
	return tex;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* Texture::Create(const void* data, size_t size, TextureFormat format, int mipLevels)
{
	MemoryStream stream;
	stream.Create(data, size);
	return Create(&stream, format, mipLevels);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture::Texture()
	: m_deviceObj(NULL)
	, m_primarySurface(NULL)
	, m_isPlatformLoaded(false)
	, m_isDefaultBackBuffer(false)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Texture::CreateImpl(GraphicsManager* manager, const Size& size, TextureFormat format, int mipLevels, Bitmap* primarySurface)
{
	GraphicsResourceObject::Initialize(manager);

	m_size = size;
	m_mipLevels = mipLevels;
	m_format = format;

	// テクスチャを作る
	m_deviceObj = GetDevice()->CreateTexture(primarySurface->GetSize(), mipLevels, format, primarySurface->GetBitmapBuffer()->GetConstData());

	// ビットマップを転送する
	//Driver::IGraphicsDevice::ScopedLockContext lock(GetDevice());
	//obj->SetSubData(Point(0, 0), primarySurface->GetBitmapBuffer()->GetConstData(), primarySurface->GetBitmapBuffer()->GetSize(), primarySurface->GetSize());
}

//-----------------------------------------------------------------------------
// プラットフォーム依存用
//-----------------------------------------------------------------------------
void Texture::CreateImpl(GraphicsManager* manager, Stream* stream, TextureFormat format, int mipLevels)
{
	GraphicsResourceObject::Initialize(manager);

	m_mipLevels = mipLevels;
	m_format = format;
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
}

//Texture::Texture(GraphicsManager* manager, Driver::ITexture* deviceObj, Bitmap* primarySurface)
//	: m_manager(manager)
//	, m_deviceObj(deviceObj)
//	, m_primarySurface(primarySurface)
//	//, m_primarySurfaceModified(false)
//	, m_isDefaultBackBuffer(false)
//{
//	LN_SAFE_ADDREF(m_primarySurface);
//	m_manager->AddResourceObject(this);
//}

void Texture::CreateImpl(GraphicsManager* manager, bool isDefaultBackBuffer)
{
	GraphicsResourceObject::Initialize(manager);
	m_deviceObj = NULL;
	m_primarySurface = NULL;
	m_isDefaultBackBuffer = isDefaultBackBuffer;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture::~Texture()
{
	LN_SAFE_RELEASE(m_primarySurface);
	LN_SAFE_RELEASE(m_deviceObj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Size& Texture::GetSize() const
{
	return m_deviceObj->GetSize();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Size& Texture::GetRealSize() const
{
	return m_deviceObj->GetRealSize();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextureFormat Texture::GetFormat() const
{
	return m_deviceObj->GetTextureFormat();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Texture::SetSubData(const Point& offset, Bitmap* bitmap)
{
	LN_CHECK_ARGS_RETURN(bitmap != NULL);

	// TODO: 現状、ピクセルフォーマットが一致していることが前提
	if (bitmap->GetPixelFormat() != Utils::TranslatePixelFormat(m_deviceObj->GetTextureFormat())) {
		LN_THROW(0, NotImplementedException);
	}

	LN_CALL_TEXTURE_COMMAND(SetSubData, SetSubDataTextureCommand,
		offset, bitmap->GetBitmapBuffer()->GetConstData(), bitmap->GetBitmapBuffer()->GetSize(), bitmap->GetSize());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void Texture::SetSubData(const Point& offset, const void* data)
//{
//LN_CHECK_ARGS_RETURN(data != NULL);
//	// TODO: m_primarySurface にもセット
//	m_manager->GetRenderer()->m_primaryCommandList->AddCommand<SetSubDataTextureCommand>(
//		m_deviceObj, offset, data, m_primarySurface->GetBitmapBuffer()->GetSize(), m_deviceObj->GetSize());
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Bitmap* Texture::Lock()
{
	// Deferred
	if (m_manager->GetRenderingType() == RenderingType::Deferred)
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Texture::Unlock()
{
	// Deferred
	if (m_manager->GetRenderingType() == RenderingType::Deferred)
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Texture::OnChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == NULL)
	{
		// Immediate のときは Lock で取得する必要がある。Deferred のときは m_primarySurface が持っている。
		if (m_manager->GetRenderingType() == RenderingType::Immediate)
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Texture::AttachDefaultBackBuffer(Driver::ITexture* deviceObj)
{
	assert(m_isDefaultBackBuffer);
	assert(m_deviceObj == NULL);
	LN_REFOBJ_SET(m_deviceObj, deviceObj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Texture::DetachDefaultBackBuffer()
{
	assert(m_isDefaultBackBuffer);
	assert(m_deviceObj != NULL);
	LN_SAFE_RELEASE(m_deviceObj);
}


//=============================================================================
// RenderTarget
//=============================================================================



	/**
		@brief		レンダリングターゲットを作成します。
		@param[in]	manager		: 作成に使用する GraphicsManager
		@param[in]	size		: レンダリングターゲットのサイズ (ピクセル単位)
		@param[in]	mipLevels	: ミップマップレベル (0 を指定すると、1x1 までのすべてのミップマップテクスチャを作成する)
		@param[in]	format		: テクスチャのピクセルフォーマット
		@details	この関数はデフォルト以外の GraphicsManager を指定して作成する場合に使用します。
	*/
	//static Texture* CreateRenderTarget(GraphicsManager* manager, const Size& size, int mipLevels = 1, TextureFormat format = TextureFormat_R8G8B8A8);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderTarget::RenderTarget()
	: Texture()
	, m_size()
	, m_mipLevels(0)
	, m_format(TextureFormat_Unknown)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderTarget::CreateImpl(GraphicsManager* manager, const Size& size, int mipLevels, TextureFormat format)
{
	GraphicsResourceObject::Initialize(manager);

	m_size = size;
	m_mipLevels = mipLevels;
	m_format = format;
	m_deviceObj = m_manager->GetGraphicsDevice()->CreateRenderTarget(m_size.Width, m_size.Height, m_mipLevels, m_format);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderTarget::~RenderTarget()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderTarget::OnChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == NULL) {
		LN_SAFE_RELEASE(m_deviceObj);
	}
	else {
		m_deviceObj = m_manager->GetGraphicsDevice()->CreateRenderTarget(m_size.Width, m_size.Height, m_mipLevels, m_format);
	}
}

//=============================================================================
// DepthBuffer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* DepthBuffer::Create(const Size& size, TextureFormat format)
{
	RefPtr<DepthBuffer> tex(LN_NEW DepthBuffer(), false);
	tex->CreateImpl(GraphicsManager::Instance, size, format);
	tex.SafeAddRef();
	return tex;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DepthBuffer::DepthBuffer()
	: m_size()
	, m_format(TextureFormat_Unknown)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DepthBuffer::CreateImpl(GraphicsManager* manager, const Size& size, TextureFormat format)
{
	GraphicsResourceObject::Initialize(manager);

	m_size = size;
	m_format = format;
	m_deviceObj = m_manager->GetGraphicsDevice()->CreateDepthBuffer(m_size.Width, m_size.Height, m_format);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DepthBuffer::~DepthBuffer()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DepthBuffer::OnChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == NULL) {
		LN_SAFE_RELEASE(m_deviceObj);
	}
	else {
		m_deviceObj = m_manager->GetGraphicsDevice()->CreateDepthBuffer(m_size.Width, m_size.Height, m_format);
	}
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
