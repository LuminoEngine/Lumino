
#include "../../Internal.h"
#include <Lumino/Graphics/Utils.h>
#include "DX9Module.h"
#include "DX9GraphicsDevice.h"
#include "DX9Texture.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

//==============================================================================
// DX9TextureBase
//==============================================================================

//------------------------------------------------------------------------------
DX9TextureBase::DX9TextureBase(DX9GraphicsDevice* device)
	: m_graphicsDevice(device)
{
	//LN_REFOBJ_SET(m_graphicsDevice);
	//m_graphicsDevice->AddDeviceObject(this);
}

//------------------------------------------------------------------------------
DX9TextureBase::~DX9TextureBase()
{
	//m_graphicsDevice->RemoveDeviceObject(this);
	//LN_SAFE_RELEASE(m_graphicsDevice);
}

//------------------------------------------------------------------------------
void DX9TextureBase::getData(const RectI& rect, void* outData)
{
	LN_UNREACHABLE();
}

//==============================================================================
// DX9Texture
//==============================================================================

//------------------------------------------------------------------------------
DX9Texture::DX9Texture(DX9GraphicsDevice* device, const SizeI& size, TextureFormat format, bool mipmap)
	: DX9TextureBase(device)
	, m_dxTexture(nullptr)
	, m_dxSurface(nullptr)
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
	IDirect3DDevice9* d3d9Device = m_graphicsDevice->getIDirect3DDevice9();
	m_size = size;

	// 実際に作成されるべきテクスチャの情報を取得する
	UINT w = m_size.width;
	UINT h = m_size.height;
	UINT miplevels = (mipmap) ? 0 : 1;
	D3DFORMAT dx_fmt = DX9Module::TranslateLNFormatToDxFormat(format);
	DWORD dxUsage = /*D3DUSAGE_DYNAMIC | */((mipmap) ? D3DUSAGE_AUTOGENMIPMAP : 0);

	LN_COMCALL(DX9Module::D3DXCheckTextureRequirements(
		d3d9Device,
		&w, &h,				// GPU が 2 のべき乗サイズしか扱えなければ拡張されたサイズが入る
		&miplevels,			// 0 の場合は最大数が格納される
		dxUsage,
		&dx_fmt,
		D3DPOOL_DEFAULT));

	m_realSize.set(w, h);

	// テクスチャ作成
	// 3つめの引数 ( ミップマップ ) は、使わない場合は 1 にしておく( 0 にすると可能な限り全部作られる )
	LN_COMCALL(d3d9Device->CreateTexture(
		w, h, miplevels,
		dxUsage,
		dx_fmt, D3DPOOL_DEFAULT, &m_dxTexture, NULL));


	m_format = format;
	//if (format == TextureFormat::R32G32B32A32_Float)
	//{
	//	auto* buf = lock();
	//	Vector4 v(255,0, 255,255);
	//	Vector4* d = (Vector4*)buf->getBitmapBuffer()->getData();
	//	for (int i = 0; i < size.width * size.height; i++)
	//	{
	//		d[i] = v;
	//	}


	//	unlock();
	//}
	//// テクスチャのサーフェイスを取得しておく
	//LN_COMCALL(m_dxTexture->GetSurfaceLevel(0, &m_dxSurface));

	//// サーフェイスフォーマットの取得
	//D3DSURFACE_DESC desc;
	//m_dxSurface->GetDesc(&desc);
	//m_format = DX9Module::TranslateFormatDxToLN(desc.Format);

}

//------------------------------------------------------------------------------
DX9Texture::DX9Texture(DX9GraphicsDevice* device, const void* data, uint32_t size, const Color32& colorKey, bool mipmap, TextureFormat format)
	: DX9TextureBase(device)
	, m_dxTexture(NULL)
	, m_dxSurface(NULL)
{
	// テクスチャ情報
	D3DXIMAGE_INFO imageInfo;
	LN_COMCALL(DX9Module::D3DXGetImageInfoFromFileInMemory(data, size, &imageInfo));

	// テクスチャのフォーマットを決める
	D3DFORMAT dxFormat = D3DFMT_A8R8G8B8;// (format == TextureFormat::Unknown) ? imageInfo.Format :　DX9Module::TranslateLNFormatToDxFormat(format);

	UINT miplevels = (mipmap) ? 0 : 1;
	DWORD dxUsage = D3DUSAGE_DYNAMIC | ((mipmap) ? D3DUSAGE_AUTOGENMIPMAP : 0);

	D3DCOLOR ck = D3DCOLOR_ARGB(colorKey.a, colorKey.r, colorKey.g, colorKey.b);
	LN_COMCALL(DX9Module::D3DXCreateTextureFromFileInMemoryEx(
		m_graphicsDevice->getIDirect3DDevice9(),
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
		miplevels,
		dxUsage,
		dxFormat,
		D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		ck,
		NULL,
		NULL,
		&m_dxTexture));

	// ここの時点で mDxTexture の参照カウントは「 1 」

	// ファイルのイメージの幅と高さを記憶
	m_size.set(imageInfo.Width, imageInfo.Height);

	// 実際のテクスチャの大きさを取得
	D3DFORMAT afm;
	LN_COMCALL(DX9Module::D3DXCheckTextureRequirements(
		m_graphicsDevice->getIDirect3DDevice9(),
		&imageInfo.Width, &imageInfo.Height,
		&miplevels, dxUsage, &afm, D3DPOOL_DEFAULT));
	m_realSize.set(imageInfo.Width, imageInfo.Height);

	// テクスチャのサーフェイスを取得する
	// ( ここでテクスチャのインターフェイスの参照カウントもひとつ増えてるみたい )
	LN_COMCALL(m_dxTexture->GetSurfaceLevel(0, &m_dxSurface));

	// ここの時点で m_dxSurface の参照カウントは「 3 」
	// ここの時点で m_dxTexture の参照カウントは「 2 」

	// 実際に作成されたフォーマットを覚えておく
	D3DSURFACE_DESC desc;
	m_dxSurface->GetDesc(&desc);
	m_format = DX9Module::TranslateFormatDxToLN(desc.Format);
}

//------------------------------------------------------------------------------
DX9Texture::~DX9Texture()
{
	LN_COM_SAFE_RELEASE(m_dxSurface);
	LN_COM_SAFE_RELEASE(m_dxTexture);
}

//------------------------------------------------------------------------------
void DX9Texture::setSubData(const PointI& point, const void* data, size_t dataBytes, const SizeI& dataBitmapSize)
{
	RECT lockRect = { point.x, point.y, point.x + dataBitmapSize.width, point.y + dataBitmapSize.height };
	if (lockRect.left < 0) lockRect.left = 0;
	if (lockRect.top < 0) lockRect.top = 0;
	if (lockRect.right > m_size.width) lockRect.right = m_size.width;
	if (lockRect.bottom > m_size.height) lockRect.right = m_size.height;
	if (lockRect.right <= lockRect.left) return;
	if (lockRect.bottom <= lockRect.top) return;

	int lineWidth = lockRect.right - lockRect.left;
	int lineHeight = lockRect.bottom - lockRect.top;

	D3DLOCKED_RECT lockedRect;
	LN_COMCALL(m_dxTexture->LockRect(0, &lockedRect, &lockRect, D3DLOCK_DISCARD));

	size_t pixelByteSize = lockedRect.Pitch / m_size.width;		// ピクセルバイト数
	size_t srcRowBytes = pixelByteSize * dataBitmapSize.width;	// 横一列のバイト数

#if 1
	byte_t* dst = (byte_t*)lockedRect.pBits;
	const byte_t* src = (const byte_t*)data;
	for (int row = 0; row < lineHeight; ++row)
	{
		//byte_t* dstline = dst + (lockedRect.Pitch * row);		// TODO format
		byte_t* dstline = dst + (lockedRect.Pitch * (lineHeight - row - 1));		// TODO format
		const byte_t* srcline = src + (srcRowBytes * row);	// TODO format
		memcpy(dstline, srcline, srcRowBytes);
	}
#endif
#if 0
	byte_t* dst = (byte_t*)lockedRect.pBits;
	dst += lockedRect.Pitch * point.y;
	size_t ofs = pixelSize * point.x;
	const byte_t* src = (const byte_t*)data;
	for (int row = 0; row < lineHeight; ++row)
	{
		byte_t* dstline = dst + (ofs + lockedRect.Pitch * row);		// TODO format
		const byte_t* srcline = src + (srcRowBytes * row);	// TODO format
		memcpy(dstline, srcline, srcRowBytes);
	}
#endif
#if 0
	byte_t* dst = (byte_t*)lockedRect.pBits;
	const byte_t* src = (const byte_t*)data;
	for (int row = 0; row < lineHeight; ++row)
	{
		byte_t* dstline = dst + (srcRowBytes * row);		// TODO format
		const byte_t* srcline = src + (srcRowBytes * row);	// TODO format
		memcpy(dstline, srcline, srcRowBytes);
	}
#endif
#if 0
	const byte_t* d = (const byte_t*)data;
	byte_t* w = (byte_t*)lockedRect.pBits;
	w += lockedRect.Pitch * point.y;
	size_t ofs = pixelSize * point.x;
	for (int row = 0; row < lineHeight; ++row)
	{
		byte_t* dstline = &w[lockedRect.Pitch * row + ofs];	// TODO format
		const byte_t* srcline = &d[srcRowBytes * row];	// TODO format
		memcpy(dstline, srcline, lockedRect.Pitch);
	}
#endif

	m_dxTexture->UnlockRect(0);
}

//------------------------------------------------------------------------------
void DX9Texture::setSubData3D(const Box32& box, const void* data, size_t dataBytes)
{
	LN_UNREACHABLE();
}

//------------------------------------------------------------------------------
void DX9Texture::getData(const RectI& rect, void* outData)
{
	if (LN_REQUIRE(outData != nullptr)) return;

	if (rect != RectI(0, 0, m_realSize))
	{
		// TODO: 今はサイズ全体が一致している場合のみ
		LN_NOTIMPLEMENTED();
	}

	D3DLOCKED_RECT lockedRect;
	LN_COMCALL(m_dxTexture->LockRect(0, &lockedRect, NULL, D3DLOCK_READONLY));

	size_t pixelByteSize = lockedRect.Pitch / m_size.width;		// ピクセルバイト数
	size_t srcRowBytes = pixelByteSize * m_realSize.width;		// 横一列のバイト数

	byte_t* dst = (byte_t*)outData;
	const byte_t* src = (const byte_t*)lockedRect.pBits;
	for (int row = 0; row < rect.height; ++row)
	{
		//byte_t* dstline = dst + (lockedRect.Pitch * row);
		byte_t* dstline = dst + (lockedRect.Pitch * (rect.height - row - 1));
		const byte_t* srcline = src + (srcRowBytes * row);
		memcpy(dstline, srcline, srcRowBytes);
	}

	LN_COMCALL(m_dxTexture->UnlockRect(0));
}

//------------------------------------------------------------------------------
RawBitmap* DX9Texture::lock()
{
#if 1

	D3DSURFACE_DESC desc;
	//LN_COMCALL(m_dxSurface->GetDesc(&desc));
	LN_COMCALL(m_dxTexture->GetLevelDesc(0, &desc));

	IDirect3DDevice9* d3d9Device = m_graphicsDevice->getIDirect3DDevice9();
	LN_COMCALL(d3d9Device->CreateTexture(
		desc.Width, desc.Height, 1,
		0,
		desc.Format,
		D3DPOOL_SYSTEMMEM,
		&staging, NULL));



	DWORD flags = D3DLOCK_DISCARD;//D3DLOCK_READONLY;

					// lock
	RECT lockRect = { 0, 0, m_realSize.width, m_realSize.height };
	D3DLOCKED_RECT lockedRect;
	LN_COMCALL(staging->LockRect(0, &lockedRect, &lockRect, flags));	// TODO: 読むのか書くのか、ロックの種類を指定したい

																			// lock したバッファを参照する RawBitmap を作成して返す
	PixelFormat pixelFormat = Utils::translatePixelFormat(m_format);
	size_t size = RawBitmap::getPixelFormatByteCount(pixelFormat, m_realSize, 1);
	//m_lockedBuffer =  ByteBuffer(lockedRect.pBits, size, true);
	m_lockedBitmap.attach(LN_NEW RawBitmap(lockedRect.pBits, m_realSize, pixelFormat));

	// TODO: ↑文字列書き込み等でわりと頻繁にロックされる場合がある。できれば new は 1 度にしたいが…

	//memset(lockedRect.pBits, 0xff, size);

	if (m_format == TextureFormat::R32G32B32A32_Float)
	{
		//auto* buf = lock();
		Vector4 v(255, 0, 255, 255);
		Vector4* d = (Vector4*)m_lockedBitmap->getBitmapBuffer()->getData();
		for (int i = 0; i < desc.Width * desc.Height; i++)
		{
			d[i] = v;
		}


		//unlock();
	}
	return m_lockedBitmap;
#else
	DWORD flags = 0;//D3DLOCK_DISCARD;//D3DLOCK_READONLY;

	// lock
	RECT lockRect = { 0, 0, m_realSize.width, m_realSize.height };
	D3DLOCKED_RECT lockedRect;
	LN_COMCALL(m_dxTexture->LockRect(0, &lockedRect, &lockRect, flags));	// TODO: 読むのか書くのか、ロックの種類を指定したい

	// lock したバッファを参照する RawBitmap を作成して返す
	PixelFormat pixelFormat = Utils::translatePixelFormat(m_format);
	size_t size = RawBitmap::getPixelFormatByteCount(pixelFormat, m_realSize, 1);
	//m_lockedBuffer =  ByteBuffer(lockedRect.pBits, size, true);
	m_lockedBitmap.attach(LN_NEW RawBitmap(lockedRect.pBits, m_realSize, pixelFormat));

	// TODO: ↑文字列書き込み等でわりと頻繁にロックされる場合がある。できれば new は 1 度にしたいが…

	return m_lockedBitmap;
#endif
}
//#pragma comment(lib, "d3dx9d.lib")
//------------------------------------------------------------------------------
void DX9Texture::unlock()
{
#if 1
	IDirect3DDevice9* d3d9Device = m_graphicsDevice->getIDirect3DDevice9();
	LN_COMCALL(DX9Module::D3DXFilterTexture(staging, NULL, staging->GetLevelCount() - 1, 0));
	LN_COMCALL(staging->UnlockRect(0));
	LN_COMCALL(d3d9Device->UpdateTexture(staging, m_dxTexture));

	staging->Release();
#else
	//m_lockedBuffer.Release();
	LN_COMCALL(m_dxTexture->UnlockRect(0));
#endif
	m_lockedBitmap.safeRelease();
}

//==============================================================================
// DX9Texture3D
//==============================================================================
//------------------------------------------------------------------------------
DX9Texture3D::DX9Texture3D(DX9GraphicsDevice* device)
	: DX9TextureBase(device)
	, m_dxTexture(nullptr)
	, m_format(TextureFormat::Unknown)
	, m_size(0, 0)
	, m_depth(0)
	, m_realSize(0, 0)
	, m_lockedBitmap()
{
}

//------------------------------------------------------------------------------
DX9Texture3D::~DX9Texture3D()
{
	LN_COM_SAFE_RELEASE(m_dxTexture);
}

//------------------------------------------------------------------------------
void DX9Texture3D::initialize(int width, int height, int depth, TextureFormat format, uint32_t levels)
{
	IDirect3DDevice9* d3d9Device = m_graphicsDevice->getIDirect3DDevice9();

	m_size.set(width, height);

	// 実際に作成されるべきテクスチャの情報を取得する
	UINT w = m_size.width;
	UINT h = m_size.height;
	UINT d = depth;
	UINT miplevels = levels;
	D3DFORMAT dxFormat = DX9Module::TranslateLNFormatToDxFormat(format);
	LN_COMCALL(DX9Module::D3DXCheckVolumeTextureRequirements(
		d3d9Device,
		&w, &h, &d,
		&miplevels,
		D3DUSAGE_DYNAMIC,
		&dxFormat,
		D3DPOOL_DEFAULT));
	m_realSize.set(w, h);
	m_depth = d;

	// テクスチャ作成
	LN_COMCALL(d3d9Device->CreateVolumeTexture(
		w, h, d,
		miplevels,
		D3DUSAGE_DYNAMIC,
		dxFormat,
		D3DPOOL_DEFAULT,
		&m_dxTexture,
		NULL));

	// 実際に作成されたテクスチャのフォーマットを覚えておく
	m_format = DX9Module::TranslateFormatDxToLN(dxFormat);
}

//------------------------------------------------------------------------------
void DX9Texture3D::setSubData(const PointI& point, const void* data, size_t dataBytes, const SizeI& dataBitmapSize)
{
	LN_UNREACHABLE();
}

//------------------------------------------------------------------------------
void DX9Texture3D::setSubData3D(const Box32& box, const void* data, size_t dataBytes)
{
	if (LN_REQUIRE(data != nullptr)) return;
	if (dataBytes == 0) return;

	D3DBOX d3dBox;
	D3DBOX* pD3DBox = NULL;
	if (box.width > 0 && box.height > 0 && box.depth > 0)
	{
		d3dBox.Left = box.x;
		d3dBox.Top = box.y;
		d3dBox.Front = box.z;
		d3dBox.Right = box.x + box.width;
		d3dBox.Bottom = box.y + box.height;
		d3dBox.Back = box.z + box.depth;
		pD3DBox = &d3dBox;
	}

	// TODO: level
	D3DLOCKED_BOX lockedVolume;
	LN_COMCALL(m_dxTexture->LockBox(0, &lockedVolume, pD3DBox, D3DLOCK_DISCARD));

	memcpy(lockedVolume.pBits, data, dataBytes);

	LN_COMCALL(m_dxTexture->UnlockBox(0));
}

//------------------------------------------------------------------------------
RawBitmap* DX9Texture3D::lock()
{
	LN_NOTIMPLEMENTED();
	return nullptr;
}

//------------------------------------------------------------------------------
void DX9Texture3D::unlock()
{
	LN_NOTIMPLEMENTED();
}

//==============================================================================
// DX9RenderTargetTexture
//==============================================================================

//------------------------------------------------------------------------------
DX9RenderTargetTexture::DX9RenderTargetTexture(DX9GraphicsDevice* device, const SizeI& size, TextureFormat format, int mipLevels)
	: DX9TextureBase(device)
	, m_dxTexture(nullptr)
	, m_dxSurface(nullptr)
	, m_format(format)
	, m_size(size)
	, m_realSize(size)
	, m_mipLevels(mipLevels)
	, m_lockedSystemSurface(nullptr)
	, m_lockedBuffer()
	, m_lockedBitmap(nullptr)
{
	onResetDevice();
}

//------------------------------------------------------------------------------
DX9RenderTargetTexture::~DX9RenderTargetTexture()
{
	onLostDevice();
}

//------------------------------------------------------------------------------
void DX9RenderTargetTexture::onLostDevice()
{
	LN_COM_SAFE_RELEASE(m_dxSurface);
	LN_COM_SAFE_RELEASE(m_dxTexture);
}

//------------------------------------------------------------------------------
void DX9RenderTargetTexture::onResetDevice()
{
	IDirect3DDevice9* dxDevice = m_graphicsDevice->getIDirect3DDevice9();

	// レンダーターゲットは GDI 互換フォーマットでなければならない (Radeon HD8490)
	if (m_format == TextureFormat::R8G8B8A8) {
		m_format = TextureFormat::B8G8R8A8;
	}
	else if (m_format == TextureFormat::R8G8B8X8) {
		m_format = TextureFormat::B8G8R8X8;
	}

	D3DFORMAT dx_fmt = DX9Module::TranslateLNFormatToDxFormat(m_format);
	//switch (dx_fmt)
	//{
	//case D3DFMT_A8B8G8R8: dx_fmt = D3DFMT_A8R8G8B8; break;
	//case D3DFMT_X8B8G8R8: dx_fmt = D3DFMT_X8R8G8B8; break;
	//}

	// サイズ格納
	UINT w = static_cast<UINT>(m_size.width);
	UINT h = static_cast<UINT>(m_size.height);
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
	m_realSize.set(desc.Width, desc.Height);

	// レンダリングターゲットのサーフェイスを取得する
	LN_COMCALL(m_dxTexture->GetSurfaceLevel(0, &m_dxSurface));

	// ちなみに D3DFMT_R8G8B8 はレンダリングターゲットには使えない

	// サーフェイスフォーマットの取得
	m_format = DX9Module::TranslateFormatDxToLN(desc.Format);

	//lock();
}

//------------------------------------------------------------------------------
void DX9RenderTargetTexture::setSubData3D(const Box32& box, const void* data, size_t dataBytes)
{
	LN_UNREACHABLE();
}

//------------------------------------------------------------------------------
RawBitmap* DX9RenderTargetTexture::lock()
{
	IDirect3DDevice9* dxDevice = m_graphicsDevice->getIDirect3DDevice9();
	//m_graphicsDevice->getDefaultSwapChain()->present(nullptr);

	
	//LN_COMCALL(dxDevice->Clear(0, NULL, flag, dxc, 1, 0));



	DX9RenderTargetTexture::lockRenderTarget(dxDevice, m_dxSurface, m_format, m_realSize, &m_lockedSystemSurface, &m_lockedBuffer, &m_lockedBitmap);
	return m_lockedBitmap;
}

//------------------------------------------------------------------------------
void DX9RenderTargetTexture::unlock()
{
	DX9RenderTargetTexture::unlockRenderTarget(&m_lockedSystemSurface, &m_lockedBuffer, &m_lockedBitmap);
}

//------------------------------------------------------------------------------
void DX9RenderTargetTexture::lockRenderTarget(IDirect3DDevice9* dxDevice, IDirect3DSurface9* dxSurface, TextureFormat format, const SizeI& realSize, IDirect3DSurface9** outLockedSystemSurface, ByteBuffer* outLockedBuffer, RawBitmap** outLockedBitmap)
{
	D3DSURFACE_DESC desc;
	LN_COMCALL(dxSurface->GetDesc(&desc));

	// レンダーテクスチャと同じフォーマットのサーフェイスをシステムメモリに確保
	LN_COMCALL(dxDevice->CreateOffscreenPlainSurface(
		desc.Width, desc.Height, desc.Format,
		D3DPOOL_SYSTEMMEM, outLockedSystemSurface, NULL));

	// レンダータゲットのサーフェイスをシステムメモリに転送
	LN_COMCALL(dxDevice->GetRenderTargetData(dxSurface, *outLockedSystemSurface));

	// lock
	D3DLOCKED_RECT	lockedRect;
	LN_COMCALL((*outLockedSystemSurface)->LockRect(&lockedRect, NULL, D3DLOCK_READONLY));

	// lock したバッファを参照する RawBitmap を作成して返す。
	// 取得できるビットマップデータの [0] は (0, 0)
	PixelFormat pixelFormat = Utils::translatePixelFormat(format);
	size_t size = RawBitmap::getPixelFormatByteCount(pixelFormat, realSize, 1);
	*outLockedBuffer = ByteBuffer(lockedRect.pBits, size, true);
	*outLockedBitmap = LN_NEW RawBitmap(*outLockedBuffer, realSize, pixelFormat, false);
}

//------------------------------------------------------------------------------
void DX9RenderTargetTexture::unlockRenderTarget(IDirect3DSurface9** lockedSystemSurface, ByteBuffer* lockedBuffer, RawBitmap** lockedBitmap)
{
	if ((*lockedSystemSurface) != NULL)
	{
		(*lockedSystemSurface)->UnlockRect();
		LN_COM_SAFE_RELEASE((*lockedSystemSurface));
	}
	LN_SAFE_RELEASE((*lockedBitmap));
	lockedBuffer->free();
}

//==============================================================================
// DX9DepthBuffer
//==============================================================================

//------------------------------------------------------------------------------
DX9DepthBuffer::DX9DepthBuffer(DX9GraphicsDevice* device, const SizeI& size, TextureFormat format)
	: DX9TextureBase(device)
	, m_dxSurface(NULL)
	, m_format(format)
	, m_size(size)
	, m_realSize(size)
{
	onResetDevice();
}

//------------------------------------------------------------------------------
void DX9DepthBuffer::setSubData3D(const Box32& box, const void* data, size_t dataBytes)
{
	LN_UNREACHABLE();
}

//------------------------------------------------------------------------------
DX9DepthBuffer::~DX9DepthBuffer()
{
	onLostDevice();
}

//------------------------------------------------------------------------------
void DX9DepthBuffer::onLostDevice()
{
	LN_COM_SAFE_RELEASE(m_dxSurface);
}

//------------------------------------------------------------------------------
void DX9DepthBuffer::onResetDevice()
{
	IDirect3DDevice9* dxDevice = m_graphicsDevice->getIDirect3DDevice9();

	LN_COMCALL(
		dxDevice->CreateDepthStencilSurface(
		static_cast<UINT>(m_size.width),
		static_cast<UINT>(m_size.height),
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


//==============================================================================
// DX9BackBufferTexture
//==============================================================================
//------------------------------------------------------------------------------
DX9BackBufferTexture::DX9BackBufferTexture(DX9GraphicsDevice* device)
	: DX9TextureBase(device)
	, m_dxSurface(NULL)
	, m_format(TextureFormat::Unknown)
	, m_realSize()
	, m_lockedSystemSurface(NULL)
{
}

//------------------------------------------------------------------------------
DX9BackBufferTexture::~DX9BackBufferTexture()
{
	LN_COM_SAFE_RELEASE(m_dxSurface);
}

//------------------------------------------------------------------------------
void DX9BackBufferTexture::reset(IDirect3DSurface9* backBufferSurface)
{
	LN_COM_SAFE_ADDREF(backBufferSurface);
	LN_COM_SAFE_RELEASE(m_dxSurface);
	m_dxSurface = backBufferSurface;

	if (m_dxSurface != NULL)
	{
		// サイズとフォーマットを覚えておく
		D3DSURFACE_DESC desc;
		m_dxSurface->GetDesc(&desc);
		m_realSize.set(desc.Width, desc.Height);
		m_format = DX9Module::TranslateFormatDxToLN(desc.Format);
	}
}

//------------------------------------------------------------------------------
void DX9BackBufferTexture::setSubData3D(const Box32& box, const void* data, size_t dataBytes)
{
	LN_UNREACHABLE();
}

//------------------------------------------------------------------------------
RawBitmap* DX9BackBufferTexture::lock()
{
	IDirect3DDevice9* dxDevice = m_graphicsDevice->getIDirect3DDevice9();
	DX9RenderTargetTexture::lockRenderTarget(dxDevice, m_dxSurface, m_format, m_realSize, &m_lockedSystemSurface, &m_lockedBuffer, &m_lockedBitmap);

	//D3DSURFACE_DESC desc;
	//LN_COMCALL(m_dxSurface->GetDesc(&desc));

	//// レンダーテクスチャと同じフォーマットのサーフェイスをシステムメモリに確保
	//LN_COMCALL(dxDevice->CreateOffscreenPlainSurface(
	//	desc.Width, desc.Height, desc.Format,
	//	D3DPOOL_SYSTEMMEM, &m_lockedSystemSurface, NULL));

	//// レンダータゲットのサーフェイスをシステムメモリに転送
	//LN_COMCALL(dxDevice->GetRenderTargetData(m_dxSurface, m_lockedSystemSurface));

	//// lock
	//D3DLOCKED_RECT	lockedRect;
	//LN_COMCALL(m_lockedSystemSurface->LockRect(&lockedRect, NULL, D3DLOCK_READONLY));

	//// lock したバッファを参照する RawBitmap を作成して返す。
	//// 取得できるビットマップデータの [0] は (0, 0)
	//PixelFormat pixelFormat = Utils::translatePixelFormat(m_format);
	//size_t size = RawBitmap::getPixelFormatByteCount(pixelFormat, m_realSize);
	//m_lockedBuffer = ByteBuffer(lockedRect.pBits, size, true);
	//m_lockedBitmap.Attach(LN_NEW RawBitmap(m_lockedBuffer, m_realSize, pixelFormat, false));

	return m_lockedBitmap;
}

//------------------------------------------------------------------------------
void DX9BackBufferTexture::unlock()
{
	DX9RenderTargetTexture::unlockRenderTarget(&m_lockedSystemSurface, &m_lockedBuffer, &m_lockedBitmap);
	//if (m_lockedSystemSurface != NULL)
	//{
	//	m_lockedSystemSurface->UnlockRect();
	//	LN_SAFE_RELEASE(m_lockedSystemSurface);
	//}
	//m_lockedBitmap.SafeRelease();
	//m_lockedBuffer.Release();
}

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
