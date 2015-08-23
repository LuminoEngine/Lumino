
#include "../../../Internal.h"
#include <Lumino/Graphics/Utils.h>
#include "DX9Module.h"
#include "DX9GraphicsDevice.h"
#include "DX9Texture.h"

namespace Lumino
{
namespace Graphics
{
namespace Device
{

//=============================================================================
// DX9TextureBase
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DX9TextureBase::DX9TextureBase(DX9GraphicsDevice* device)
	: m_graphicsDevice(device)
{
	//LN_REFOBJ_SET(m_graphicsDevice);
	//m_graphicsDevice->AddDeviceObject(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DX9TextureBase::~DX9TextureBase()
{
	//m_graphicsDevice->RemoveDeviceObject(this);
	//LN_SAFE_RELEASE(m_graphicsDevice);
}


//=============================================================================
// DX9Texture
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DX9Texture::DX9Texture(DX9GraphicsDevice* device, const Size& size, TextureFormat format, uint32_t levels)
	: DX9TextureBase(device)
	, m_dxTexture(NULL)
	, m_dxSurface(NULL)
{
	/*
		format で指定されたフォーマットが本当に使えるかは必ず事前チェックしなければならない。
		D3DXCheckTextureRequirements() がその処理。

		というのも、本ライブラリの基本フォーマットは RGBA だが、DX9 は BGRA(D3DFMT_A8R8G8B8) である。
		RGBA フォーマットは DX9 では定数は定義されているものの、実際には使えないことがある。
		…というかほとんど使えない？「AMD Radeon(TM) HD8490」ではダメだった。D3DXCheckTextureRequirements() が BGRA の定数を返す。

		このため、事前チェックが必要。さらに、本当に使われたフォーマットからもう一度
		本ライブラリ用のフォーマットを求めなおしている。
	*/
	
	m_size = size;

	// 実際に作成されるべきテクスチャの情報を取得する
	UINT w = m_size.Width;
	UINT h = m_size.Height;
	UINT miplevels = levels;
	D3DFORMAT dx_fmt = DX9Module::TranslateLNFormatToDxFormat(format);
	LN_COMCALL(DX9Module::D3DXCheckTextureRequirements(
		m_graphicsDevice->GetIDirect3DDevice9(),
		&w, &h,				// GPU が 2 のべき乗サイズしか扱えなければ拡張されたサイズが入る
		&miplevels,			// 0 の場合は最大数が格納される
		0,
		&dx_fmt,
		D3DPOOL_MANAGED));

	m_realSize.Set(w, h);

	// テクスチャ作成
	// 3つめの引数 ( ミップマップ ) は、使わない場合は 1 にしておく( 0 にすると可能な限り全部作られる )
	LN_COMCALL(m_graphicsDevice->GetIDirect3DDevice9()->CreateTexture(
		w, h, miplevels, 0,
		dx_fmt, D3DPOOL_MANAGED, &m_dxTexture, NULL));

	// テクスチャのサーフェイスを取得しておく
	LN_COMCALL(m_dxTexture->GetSurfaceLevel(0, &m_dxSurface));

	// サーフェイスフォーマットの取得
	D3DSURFACE_DESC desc;
	m_dxSurface->GetDesc(&desc);
	m_format = DX9Module::TranslateFormatDxToLN(desc.Format);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DX9Texture::DX9Texture(DX9GraphicsDevice* device, const void* data, uint32_t size, const Color& colorKey, uint32_t levels, TextureFormat format)
	: DX9TextureBase(device)
	, m_dxTexture(NULL)
	, m_dxSurface(NULL)
{
	// テクスチャ情報
	D3DXIMAGE_INFO imageInfo;
	LN_COMCALL(DX9Module::D3DXGetImageInfoFromFileInMemory(data, size, &imageInfo));

	// テクスチャのフォーマットを決める
	D3DFORMAT dxFormat = (format == TextureFormat_Unknown) ? imageInfo.Format : DX9Module::TranslateLNFormatToDxFormat(format);

	D3DCOLOR ck = D3DCOLOR_ARGB(colorKey.A, colorKey.R, colorKey.G, colorKey.B);
	LN_COMCALL(DX9Module::D3DXCreateTextureFromFileInMemoryEx(
		m_graphicsDevice->GetIDirect3DDevice9(),
		data, size,
		//512, 512, // ノート OK
		//D3DX_DEFAULT_NONPOW2, // ノート NG  デスクトップ OK
		//D3DX_DEFAULT_NONPOW2,
		//D3DX_DEFAULT, // ノート OK  デスクトップ NG
		//D3DX_DEFAULT,
		//0, 0,       // ノート OK  デスクトップ NG
		//300, 300,       // デスクトップ OK
		imageInfo.Width,
		imageInfo.Height,
		levels,
		0,
		dxFormat,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		ck,
		NULL,
		NULL,
		&m_dxTexture));

	// ここの時点で mDxTexture の参照カウントは「 1 」

	// ファイルのイメージの幅と高さを記憶
	m_size.Set(imageInfo.Width, imageInfo.Height);

	UINT miplevels = levels;

	// 実際のテクスチャの大きさを取得
	LN_COMCALL(DX9Module::D3DXCheckTextureRequirements(
		m_graphicsDevice->GetIDirect3DDevice9(),
		&imageInfo.Width, &imageInfo.Height,
		&miplevels, 0, NULL, D3DPOOL_MANAGED));
	m_realSize.Set(imageInfo.Width, imageInfo.Height);

	// テクスチャのサーフェイスを取得する
	// ( ここでテクスチャのインターフェイスの参照カウントもひとつ増えてるみたい )
	LN_COMCALL(m_dxTexture->GetSurfaceLevel(0, &m_dxSurface));

	// ここの時点で m_dxSurface の参照カウントは「 3 」
	// ここの時点で m_dxTexture の参照カウントは「 2 」

	// サーフェイスフォーマットの取得
	D3DSURFACE_DESC desc;
	m_dxSurface->GetDesc(&desc);
	m_format = DX9Module::TranslateFormatDxToLN(desc.Format);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DX9Texture::~DX9Texture()
{
	LN_SAFE_RELEASE(m_dxSurface);
	LN_SAFE_RELEASE(m_dxTexture);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9Texture::SetSubData(const Point& point, const void* data, size_t dataBytes, const Size& dataBitmapSize)
{
	RECT lockRect = { point.X, point.Y, point.X + dataBitmapSize.Width, point.Y + dataBitmapSize.Height };
	D3DLOCKED_RECT lockedRect;
	LN_COMCALL(m_dxTexture->LockRect(0, &lockedRect, &lockRect, D3DLOCK_DISCARD));

	const byte_t* d = (const byte_t*)data;
	byte_t* w = (byte_t*)lockedRect.pBits;
	for (int row = 0; row < dataBitmapSize.Height; row++)
	{
		const byte_t* line = &d[(4 * dataBitmapSize.Width) * row];	// TODO format
		byte_t* wline = &w[lockedRect.Pitch * row];	// TODO format
		memcpy(wline, line, (4 * dataBitmapSize.Width));
	}

	//memcpy(lockedRect.pBits, data, lockedRect.Pitch * dataBitmapSize.Height);

	//try
	//{
	//	// 参照モードでロック領域を Bitmap 化する (メモリコピーを行わない)
	//	ByteBuffer refData(lockedRect.pBits, lockedRect.Pitch * m_realSize.Height, true);
	//	Bitmap lockedBmp(&refData, m_realSize, Graphics::PixelFormat_BYTE_B8G8R8A8);	// GDI 互換フォーマット

	//	// 全体を転送する
	//	Rect rc(0, 0, m_realSize);
	//	lockedBmp.BitBlt(rc, bitmap, rc, false);
	//}
	//catch (...)
	//{
	//	m_dxTexture->UnlockRect(0);
	//	throw;
	//}

	m_dxTexture->UnlockRect(0);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Bitmap* DX9Texture::Lock()
{
	// Lock
	D3DLOCKED_RECT lockedRect;
	LN_COMCALL(m_dxTexture->LockRect(0, &lockedRect, NULL, 0));

	// Lock したバッファを参照する Bitmap を作成して返す
	PixelFormat pixelFormat = Utils::TranslatePixelFormat(m_format);
	size_t size = Bitmap::GetPixelFormatByteCount(pixelFormat, m_realSize);
	//m_lockedBuffer =  ByteBuffer(lockedRect.pBits, size, true);
	m_lockedBitmap.Attach(LN_NEW Bitmap(lockedRect.pBits, m_realSize, pixelFormat));

	// TODO: ↑文字列書き込み等でわりと頻繁にロックされる場合がある。できれば new は 1 度にしたいが…

	return m_lockedBitmap;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9Texture::Unlock()
{
	m_lockedBitmap.SafeRelease();
	//m_lockedBuffer.Release();
	m_dxTexture->UnlockRect(0);
}

//=============================================================================
// DX9RenderTargetTexture
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DX9RenderTargetTexture::DX9RenderTargetTexture(DX9GraphicsDevice* device, const Size& size, TextureFormat format, int mipLevels)
	: DX9TextureBase(device)
	, m_dxTexture(NULL)
	, m_dxSurface(NULL)
	, m_format(format)
	, m_size(size)
	, m_realSize(size)
	, m_mipLevels(mipLevels)
{
	OnResetDevice();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DX9RenderTargetTexture::~DX9RenderTargetTexture()
{
	OnLostDevice();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9RenderTargetTexture::OnLostDevice()
{
	LN_SAFE_RELEASE(m_dxSurface);
	LN_SAFE_RELEASE(m_dxTexture);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9RenderTargetTexture::OnResetDevice()
{
	IDirect3DDevice9* dxDevice = m_graphicsDevice->GetIDirect3DDevice9();

	D3DFORMAT dx_fmt = DX9Module::TranslateLNFormatToDxFormat(m_format);
	//switch (dx_fmt)
	//{
	//case D3DFMT_A8B8G8R8: dx_fmt = D3DFMT_A8R8G8B8; break;
	//case D3DFMT_X8B8G8R8: dx_fmt = D3DFMT_X8R8G8B8; break;
	//}

	// サイズ格納
	UINT w = static_cast<UINT>(m_size.Width);
	UINT h = static_cast<UINT>(m_size.Height);
	UINT levels = m_mipLevels;
	/*
	// 実際に作成されるべきテクスチャの情報を取得する
	LN_DXCALL_R(
	GraphicsUtil::D3DXCheckTextureRequirements(
	dx_device,
	&w, &h,
	&levels,        // 0 の場合は最大数が格納される
	D3DUSAGE_RENDERTARGET,
	&dx_fmt,
	D3DPOOL_MANAGED ),
	"Texture::initialize" );


	mSize.dump();

	// 実際の値格納
	mRealSize.x = static_cast< lnFloat >( w );
	mRealSize.y = static_cast< lnFloat >( h );
	mMipLevel = levels;
	mFormat = GraphicsUtil::convertFormatDxToLN( dx_fmt );

	mRealSize.dump();
	*/

	LN_COMCALL(
		dxDevice->CreateTexture(
		w, h,
		levels,
		D3DUSAGE_RENDERTARGET,	// レンダリングターゲットとして作成
		dx_fmt,
		D3DPOOL_DEFAULT,		// レンダリングターゲットは D3DPOOL_DEFAULT
		&m_dxTexture,
		NULL));
	//"w=%d h=%d levels=%d dx_fmt=%d\n\nミップマップレベルが 1 以外でテクスチャサイズが 2 の累乗ではない場合、このエラーが発生することがあります。",
	//w, h, levels, dx_fmt );

	// サイズチェック
	D3DSURFACE_DESC desc;
	m_dxTexture->GetLevelDesc(0, &desc);
	//_p( desc.Width );
	//_p( desc.Height );
	m_realSize.Set(desc.Width, desc.Height);

	// レンダリングターゲットのサーフェイスを取得する
	LN_COMCALL(m_dxTexture->GetSurfaceLevel(0, &m_dxSurface));

	// ちなみに D3DFMT_R8G8B8 はレンダリングターゲットには使えない

	// サーフェイスフォーマットの取得
	m_format = DX9Module::TranslateFormatDxToLN(desc.Format);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Bitmap* DX9RenderTargetTexture::Lock()
{
	LN_THROW(0, NotImplementedException);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9RenderTargetTexture::Unlock()
{
	LN_THROW(0, NotImplementedException);
}


//=============================================================================
// DX9DepthBuffer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DX9DepthBuffer::DX9DepthBuffer(DX9GraphicsDevice* device, const Size& size, TextureFormat format)
	: DX9TextureBase(device)
	, m_dxSurface(NULL)
	, m_format(format)
	, m_size(size)
	, m_realSize(size)
{
	OnResetDevice();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DX9DepthBuffer::~DX9DepthBuffer()
{
	OnLostDevice();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9DepthBuffer::OnLostDevice()
{
	LN_SAFE_RELEASE(m_dxSurface);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9DepthBuffer::OnResetDevice()
{
	IDirect3DDevice9* dxDevice = m_graphicsDevice->GetIDirect3DDevice9();

	LN_COMCALL(
		dxDevice->CreateDepthStencilSurface(
		static_cast<UINT>(m_size.Width),
		static_cast<UINT>(m_size.Height),
		DX9Module::TranslateLNFormatToDxFormat(m_format),
		D3DMULTISAMPLE_NONE,	// 高度な補間方法の設定
		0,						// 画像の品質レベルの設定
		TRUE,					// 深度バッファの制御方法の指定
		&m_dxSurface,
		NULL));

	// サーフェイスフォーマットの取得
	D3DSURFACE_DESC desc;
	m_dxSurface->GetDesc(&desc);
	m_format = DX9Module::TranslateFormatDxToLN(desc.Format);
}


//=============================================================================
// DX9BackBufferTexture
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DX9BackBufferTexture::DX9BackBufferTexture(DX9GraphicsDevice* device)
	: DX9TextureBase(device)
	, m_dxSurface(NULL)
	, m_format(TextureFormat_Unknown)
	, m_realSize()
	, m_lockedSystemSurface(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DX9BackBufferTexture::~DX9BackBufferTexture()
{
	LN_SAFE_RELEASE(m_dxSurface);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9BackBufferTexture::Reset(IDirect3DSurface9* backBufferSurface)
{
	LN_SAFE_ADDREF(backBufferSurface);
	LN_SAFE_RELEASE(m_dxSurface);
	m_dxSurface = backBufferSurface;

	if (m_dxSurface != NULL)
	{
		// サイズとフォーマットを覚えておく
		D3DSURFACE_DESC desc;
		m_dxSurface->GetDesc(&desc);
		m_realSize.Set(desc.Width, desc.Height);
		m_format = DX9Module::TranslateFormatDxToLN(desc.Format);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Bitmap* DX9BackBufferTexture::Lock()
{
	IDirect3DDevice9* dxDevice = m_graphicsDevice->GetIDirect3DDevice9();

	D3DSURFACE_DESC desc;
	LN_COMCALL(m_dxSurface->GetDesc(&desc));

	// レンダーテクスチャと同じフォーマットのサーフェイスをシステムメモリに確保
	LN_COMCALL(dxDevice->CreateOffscreenPlainSurface(
		desc.Width, desc.Height, desc.Format,
		D3DPOOL_SYSTEMMEM, &m_lockedSystemSurface, NULL));

	// レンダータゲットのサーフェイスをシステムメモリに転送
	LN_COMCALL(dxDevice->GetRenderTargetData(m_dxSurface, m_lockedSystemSurface));

	// Lock
	D3DLOCKED_RECT	lockedRect;
	LN_COMCALL(m_lockedSystemSurface->LockRect(&lockedRect, NULL, D3DLOCK_READONLY));

	// Lock したバッファを参照する Bitmap を作成して返す
	PixelFormat pixelFormat = Utils::TranslatePixelFormat(m_format);
	size_t size = Bitmap::GetPixelFormatByteCount(pixelFormat, m_realSize);
	m_lockedBuffer = ByteBuffer(lockedRect.pBits, size, true);
	m_lockedBitmap.Attach(LN_NEW Bitmap(m_lockedBuffer, m_realSize, pixelFormat));

	return m_lockedBitmap;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DX9BackBufferTexture::Unlock()
{
	if (m_lockedSystemSurface != NULL)
	{
		m_lockedSystemSurface->UnlockRect();
		LN_SAFE_RELEASE(m_lockedSystemSurface);
	}
	m_lockedBitmap.SafeRelease();
	m_lockedBuffer.Release();
}

} // namespace Device
} // namespace Graphics
} // namespace Lumino
