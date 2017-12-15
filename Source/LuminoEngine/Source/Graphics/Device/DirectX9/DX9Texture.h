
#pragma once
#include <Lumino/Graphics/SamplerState.h>
#include <Lumino/Graphics/Bitmap.h>
#include "../GraphicsDriverInterface.h"
#include "DX9Module.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{
class DX9GraphicsDevice;

class DX9TextureBase
	: public ITexture
{
public:
	DX9TextureBase(DX9GraphicsDevice* device);
	virtual ~DX9TextureBase();

	const SamplerState& getSamplerState() const { return m_samplerState; }

public:
	// ITexture interface
	virtual void setSamplerState(const SamplerState& state) { m_samplerState = state; }
	virtual void getData(const RectI& rect, void* outData) override;

	virtual IDirect3DBaseTexture9* getIDirect3DBaseTexture9() = 0;
	virtual IDirect3DSurface9* getIDirect3DSurface9() = 0;

protected:
	DX9GraphicsDevice*	m_graphicsDevice;
	SamplerState		m_samplerState;
};

/// 普通のテクスチャ
/*
	基本的なフォーマットは D3DFMT_A8R8G8B8。
	D3DX の画像ファイル読み込み系 API から作ると、強制的にこのフォーマットになる。
	D3DFMT_A8B8G8R8 を指定しても成功するが、実際には D3DFMT_A8R8G8B8 となる。
	実際にどんなフォーマットになったのかを知るため、必ず D3DXCheckTextureRequirements 等で事後確認しなければならない。

	D3DFMT はリトルエンディアンとしてみたときのバイト列を示している。
	D3DFMT_A8R8G8B8 の実際のメモリ上の並びは BB GG RR AA となる。
	例えば、
	- Red(255, 0, 0, 255) は 00 00 ff ff
	- Blue(0, 0, 255, 128) は ff 00 00 80

	行の並びは Down flow。
	LockRect した結果の [0] はビットマップの左上となる。
*/
class DX9Texture
	: public DX9TextureBase
{
public:
	/// テクスチャ作成 (level : ミップマップレベル ( 0 で全て作成 ))
	DX9Texture(DX9GraphicsDevice* device, const SizeI& size, TextureFormat format, bool mipmap);

	/// テクスチャ作成 (メモリに展開された画像データから)
	DX9Texture(DX9GraphicsDevice* device, const void* data, uint32_t size, const Color32& colorKey, bool mipmap, TextureFormat format);

	virtual ~DX9Texture();

public:
	// IDeviceObject interface
	virtual void onLostDevice() {}
	virtual void onResetDevice() {}

	// ITexture interface
	virtual TextureType getTextureType() const { return TextureType_Normal; }
	virtual TextureFormat getTextureFormat() const { return m_format; }
	virtual const SizeI& getSize() const { return m_size; }
	virtual const SizeI& getRealSize() const { return m_realSize; }
	virtual void setSubData(const PointI& point, const void* data, size_t dataBytes, const SizeI& dataBitmapSize);
	virtual void setSubData3D(const Box32& box, const void* data, size_t dataBytes);
	virtual void getData(const RectI& rect, void* outData) override;
	virtual RawBitmap* lock();
	virtual void unlock();

	// DX9TextureBase interface
	virtual IDirect3DBaseTexture9* getIDirect3DBaseTexture9() { return m_dxTexture; }
	virtual IDirect3DSurface9* getIDirect3DSurface9() { return m_dxSurface; }

private:
	IDirect3DTexture9*		m_dxTexture;	///< Direct3DTexture 本体
	IDirect3DSurface9*		m_dxSurface;	///< テクスチャのサーフェイス
	TextureFormat			m_format;
	SizeI					m_size;
	SizeI					m_realSize;
	Ref<RawBitmap>			m_lockedBitmap;
};

class DX9Texture3D
	: public DX9TextureBase
{
public:
	DX9Texture3D(DX9GraphicsDevice* device);
	virtual ~DX9Texture3D();
	void initialize(int width, int height, int depth, TextureFormat format, uint32_t levels);

	// IDeviceObject interface
	virtual void onLostDevice() {}
	virtual void onResetDevice() {}

	// ITexture interface
	virtual TextureType getTextureType() const { return TextureType_Normal; }
	virtual TextureFormat getTextureFormat() const { return m_format; }
	virtual const SizeI& getSize() const { return m_size; }
	virtual const SizeI& getRealSize() const { return m_realSize; }
	virtual void setSubData(const PointI& point, const void* data, size_t dataBytes, const SizeI& dataBitmapSize);
	virtual void setSubData3D(const Box32& box, const void* data, size_t dataBytes);
	virtual RawBitmap* lock();
	virtual void unlock();

	// DX9TextureBase interface
	virtual IDirect3DBaseTexture9* getIDirect3DBaseTexture9() { return m_dxTexture; }
	virtual IDirect3DSurface9* getIDirect3DSurface9() { return nullptr; }

private:
	IDirect3DVolumeTexture9*	m_dxTexture;
	TextureFormat				m_format;
	SizeI						m_size;
	int							m_depth;
	SizeI						m_realSize;
	Ref<RawBitmap>				m_lockedBitmap;
};

/// レンダーターゲットテクスチャ
class DX9RenderTargetTexture
	: public DX9TextureBase
{
public:
	DX9RenderTargetTexture(DX9GraphicsDevice* device, const SizeI& size, TextureFormat format, int mipLevels);
	virtual ~DX9RenderTargetTexture();

public:
	// IDeviceObject interface
	virtual void onLostDevice();
	virtual void onResetDevice();

	// ITexture interface
	virtual TextureType getTextureType() const { return TextureType_RenderTarget; }
	virtual TextureFormat getTextureFormat() const { return m_format; }
	virtual const SizeI& getSize() const { return m_size; }
	virtual const SizeI& getRealSize() const { return m_realSize; }
	virtual void setSubData(const PointI& point, const void* data, size_t dataBytes, const SizeI& dataBitmapSize) { LN_UNREACHABLE(); }
	virtual void setSubData3D(const Box32& box, const void* data, size_t dataBytes);
	virtual void readData(void* outData) override;
	virtual RawBitmap* lock();
	virtual void unlock();

	// DX9TextureBase interface
	virtual IDirect3DBaseTexture9* getIDirect3DBaseTexture9() { return m_dxTexture; }
	virtual IDirect3DSurface9* getIDirect3DSurface9() { return m_dxSurface; }

	static void lockRenderTarget(IDirect3DDevice9* dxDevice, IDirect3DSurface9* dxSurface, TextureFormat format, const SizeI& realSize, IDirect3DSurface9** outLockedSystemSurface, ByteBuffer* outLockedBuffer, RawBitmap** outLockedBitmap);
	static void unlockRenderTarget(IDirect3DSurface9** lockedSystemSurface, ByteBuffer* lockedBuffer, RawBitmap** lockedBitmap);

private:
	IDirect3DTexture9*		m_dxTexture;
	IDirect3DSurface9*		m_dxSurface;
	TextureFormat			m_format;
	SizeI					m_size;
	SizeI					m_realSize;
	int						m_mipLevels;

	IDirect3DSurface9*		m_lockedSystemSurface;
	ByteBuffer				m_lockedBuffer;
	RawBitmap*					m_lockedBitmap;
};

/// 深度バッファ
class DX9DepthBuffer
	: public DX9TextureBase
{
public:
	DX9DepthBuffer(DX9GraphicsDevice* device, const SizeI& size, TextureFormat format);
	virtual ~DX9DepthBuffer();

public:
	// IDeviceObject interface
	virtual void onLostDevice();
	virtual void onResetDevice();

	// ITexture interface
	virtual TextureType getTextureType() const { return TextureType_DepthBuffer; }
	virtual TextureFormat getTextureFormat() const { return m_format; }
	virtual const SizeI& getSize() const { return m_size; }
	virtual const SizeI& getRealSize() const { return m_realSize; }
	virtual void setSamplerState(const SamplerState& state) { LN_UNREACHABLE(); }
	virtual void setSubData(const PointI& point, const void* data, size_t dataBytes, const SizeI& dataBitmapSize) { LN_UNREACHABLE(); }
	virtual void setSubData3D(const Box32& box, const void* data, size_t dataBytes);
	virtual RawBitmap* lock() { LN_UNREACHABLE(); return nullptr; }
	virtual void unlock() { LN_UNREACHABLE(); }

	// DX9TextureBase interface
	virtual IDirect3DBaseTexture9* getIDirect3DBaseTexture9() { return NULL; }
	virtual IDirect3DSurface9* getIDirect3DSurface9() { return m_dxSurface; }

private:
	IDirect3DSurface9*	m_dxSurface;
	TextureFormat		m_format;
	SizeI				m_size;
	SizeI				m_realSize;
	int					m_mipLevels;
};

/// バックバッファ
///		このクラスは DirectX 側で自動的に作成されたサーフェイスをラップする。
///		これは Renderer::setRenderTarget に、他のレンダーターゲットと同じようにセットして使えるようにするため。
class DX9BackBufferTexture
	: public DX9TextureBase
{
public:
	DX9BackBufferTexture(DX9GraphicsDevice* device);
	virtual ~DX9BackBufferTexture();

public:
	void reset(IDirect3DSurface9* backBufferSurface);

public:
	// IDeviceObject interface
	virtual void onLostDevice() {}
	virtual void onResetDevice() {}

	// ITexture interface
	virtual TextureType getTextureType() const { return TextureType_RenderTarget; }
	virtual TextureFormat getTextureFormat() const { return m_format; }
	virtual const SizeI& getSize() const { return m_realSize; }
	virtual const SizeI& getRealSize() const { return m_realSize; }
	virtual void setSamplerState(const SamplerState& state) { LN_UNREACHABLE(); }
	virtual void setSubData(const PointI& point, const void* data, size_t dataBytes, const SizeI& dataBitmapSize) { LN_UNREACHABLE(); }
	virtual void setSubData3D(const Box32& box, const void* data, size_t dataBytes);
	virtual RawBitmap* lock();
	virtual void unlock();

	// DX9TextureBase interface
	virtual IDirect3DBaseTexture9* getIDirect3DBaseTexture9() { return NULL; }
	virtual IDirect3DSurface9* getIDirect3DSurface9() { return m_dxSurface; }

private:
	IDirect3DSurface9*	m_dxSurface;
	TextureFormat		m_format;
	SizeI				m_realSize;

	IDirect3DSurface9*		m_lockedSystemSurface;
	ByteBuffer				m_lockedBuffer;
	RawBitmap*					m_lockedBitmap;
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
