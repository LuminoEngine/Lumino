
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

	const SamplerState& GetSamplerState() const { return m_samplerState; }

public:
	// ITexture interface
	virtual void SetSamplerState(const SamplerState& state) { m_samplerState = state; }

	virtual IDirect3DBaseTexture9* GetIDirect3DBaseTexture9() = 0;
	virtual IDirect3DSurface9* GetIDirect3DSurface9() = 0;

protected:
	DX9GraphicsDevice*	m_graphicsDevice;
	SamplerState		m_samplerState;
};

/// 普通のテクスチャ
class DX9Texture
	: public DX9TextureBase
{
public:
	/// テクスチャ作成 (level : ミップマップレベル ( 0 で全て作成 ))
	DX9Texture(DX9GraphicsDevice* device, const Size& size, TextureFormat format, uint32_t levels);

	/// テクスチャ作成 (メモリに展開された画像データから)
	DX9Texture(DX9GraphicsDevice* device, const void* data, uint32_t size, const Color32& colorKey, uint32_t levels, TextureFormat format);

	virtual ~DX9Texture();

public:
	// IDeviceObject interface
	virtual void OnLostDevice() {}
	virtual void OnResetDevice() {}

	// ITexture interface
	virtual TextureType GetTextureType() const { return TextureType_Normal; }
	virtual TextureFormat GetTextureFormat() const { return m_format; }
	virtual const Size& GetSize() const { return m_size; }
	virtual const Size& GetRealSize() const { return m_realSize; }
	virtual void SetSubData(const Point& point, const void* data, size_t dataBytes, const Size& dataBitmapSize);
	virtual Bitmap* Lock();
	virtual void Unlock();

	// DX9TextureBase interface
	virtual IDirect3DBaseTexture9* GetIDirect3DBaseTexture9() { return m_dxTexture; }
	virtual IDirect3DSurface9* GetIDirect3DSurface9() { return m_dxSurface; }

private:
	IDirect3DTexture9*		m_dxTexture;	///< Direct3DTexture 本体
	IDirect3DSurface9*		m_dxSurface;	///< テクスチャのサーフェイス
	TextureFormat			m_format;
	Size					m_size;
	Size					m_realSize;
	RefPtr<Bitmap>			m_lockedBitmap;
};

class DX9Texture3D
	: public DX9TextureBase
{
public:
	DX9Texture3D(DX9GraphicsDevice* device);
	virtual ~DX9Texture3D();
	void Initialize(int width, int height, int depth, TextureFormat format, uint32_t levels);

	// IDeviceObject interface
	virtual void OnLostDevice() {}
	virtual void OnResetDevice() {}

	// ITexture interface
	virtual TextureType GetTextureType() const { return TextureType_Normal; }
	virtual TextureFormat GetTextureFormat() const { return m_format; }
	virtual const Size& GetSize() const { return m_size; }
	virtual const Size& GetRealSize() const { return m_realSize; }
	virtual void SetSubData(const Point& point, const void* data, size_t dataBytes, const Size& dataBitmapSize);
	virtual Bitmap* Lock();
	virtual void Unlock();

	// DX9TextureBase interface
	virtual IDirect3DBaseTexture9* GetIDirect3DBaseTexture9() { return m_dxTexture; }
	virtual IDirect3DSurface9* GetIDirect3DSurface9() { return nullptr; }

private:
	IDirect3DVolumeTexture9*	m_dxTexture;
	TextureFormat				m_format;
	Size						m_size;
	int							m_depth;
	Size						m_realSize;
	RefPtr<Bitmap>				m_lockedBitmap;
};

/// レンダーターゲットテクスチャ
class DX9RenderTargetTexture
	: public DX9TextureBase
{
public:
	DX9RenderTargetTexture(DX9GraphicsDevice* device, const Size& size, TextureFormat format, int mipLevels);
	virtual ~DX9RenderTargetTexture();

public:
	// IDeviceObject interface
	virtual void OnLostDevice();
	virtual void OnResetDevice();

	// ITexture interface
	virtual TextureType GetTextureType() const { return TextureType_RenderTarget; }
	virtual TextureFormat GetTextureFormat() const { return m_format; }
	virtual const Size& GetSize() const { return m_size; }
	virtual const Size& GetRealSize() const { return m_realSize; }
	virtual void SetSubData(const Point& point, const void* data, size_t dataBytes, const Size& dataBitmapSize) { LN_THROW(0, InvalidOperationException); }
	virtual Bitmap* Lock();
	virtual void Unlock();

	// DX9TextureBase interface
	virtual IDirect3DBaseTexture9* GetIDirect3DBaseTexture9() { return m_dxTexture; }
	virtual IDirect3DSurface9* GetIDirect3DSurface9() { return m_dxSurface; }

	static void LockRenderTarget(IDirect3DDevice9* dxDevice, IDirect3DSurface9* dxSurface, TextureFormat format, const Size& realSize, IDirect3DSurface9** outLockedSystemSurface, ByteBuffer* outLockedBuffer, Bitmap** outLockedBitmap);
	static void UnlockRenderTarget(IDirect3DSurface9** lockedSystemSurface, ByteBuffer* lockedBuffer, Bitmap** lockedBitmap);

private:
	IDirect3DTexture9*		m_dxTexture;
	IDirect3DSurface9*		m_dxSurface;
	TextureFormat			m_format;
	Size					m_size;
	Size					m_realSize;
	int						m_mipLevels;

	IDirect3DSurface9*		m_lockedSystemSurface;
	ByteBuffer				m_lockedBuffer;
	Bitmap*					m_lockedBitmap;
};

/// 深度バッファ
class DX9DepthBuffer
	: public DX9TextureBase
{
public:
	DX9DepthBuffer(DX9GraphicsDevice* device, const Size& size, TextureFormat format);
	virtual ~DX9DepthBuffer();

public:
	// IDeviceObject interface
	virtual void OnLostDevice();
	virtual void OnResetDevice();

	// ITexture interface
	virtual TextureType GetTextureType() const { return TextureType_DepthBuffer; }
	virtual TextureFormat GetTextureFormat() const { return m_format; }
	virtual const Size& GetSize() const { return m_size; }
	virtual const Size& GetRealSize() const { return m_realSize; }
	virtual void SetSamplerState(const SamplerState& state) { LN_THROW(0, InvalidOperationException); }
	virtual void SetSubData(const Point& point, const void* data, size_t dataBytes, const Size& dataBitmapSize) { LN_THROW(0, InvalidOperationException); }
	virtual Bitmap* Lock() { LN_THROW(0, InvalidOperationException); }
	virtual void Unlock() { LN_THROW(0, InvalidOperationException); }

	// DX9TextureBase interface
	virtual IDirect3DBaseTexture9* GetIDirect3DBaseTexture9() { return NULL; }
	virtual IDirect3DSurface9* GetIDirect3DSurface9() { return m_dxSurface; }

private:
	IDirect3DSurface9*	m_dxSurface;
	TextureFormat		m_format;
	Size				m_size;
	Size				m_realSize;
	int					m_mipLevels;
};

/// バックバッファ
///		このクラスは DirectX 側で自動的に作成されたサーフェイスをラップする。
///		これは Renderer::SetRenderTarget に、他のレンダーターゲットと同じようにセットして使えるようにするため。
class DX9BackBufferTexture
	: public DX9TextureBase
{
public:
	DX9BackBufferTexture(DX9GraphicsDevice* device);
	virtual ~DX9BackBufferTexture();

public:
	void Reset(IDirect3DSurface9* backBufferSurface);

public:
	// IDeviceObject interface
	virtual void OnLostDevice() {}
	virtual void OnResetDevice() {}

	// ITexture interface
	virtual TextureType GetTextureType() const { return TextureType_RenderTarget; }
	virtual TextureFormat GetTextureFormat() const { return m_format; }
	virtual const Size& GetSize() const { return m_realSize; }
	virtual const Size& GetRealSize() const { return m_realSize; }
	virtual void SetSamplerState(const SamplerState& state) { LN_THROW(0, InvalidOperationException); }
	virtual void SetSubData(const Point& point, const void* data, size_t dataBytes, const Size& dataBitmapSize) { LN_THROW(0, InvalidOperationException); }
	virtual Bitmap* Lock();
	virtual void Unlock();

	// DX9TextureBase interface
	virtual IDirect3DBaseTexture9* GetIDirect3DBaseTexture9() { return NULL; }
	virtual IDirect3DSurface9* GetIDirect3DSurface9() { return m_dxSurface; }

private:
	IDirect3DSurface9*	m_dxSurface;
	TextureFormat		m_format;
	Size				m_realSize;

	IDirect3DSurface9*		m_lockedSystemSurface;
	ByteBuffer				m_lockedBuffer;
	Bitmap*					m_lockedBitmap;
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
