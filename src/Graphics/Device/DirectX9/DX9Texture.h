
#pragma once
#include <Lumino/Graphics/SamplerState.h>
#include "../GraphicsDriverInterface.h"

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

public:
	virtual IDirect3DTexture9* GetIDirect3DTexture9() = 0;
	virtual IDirect3DSurface9* GetIDirect3DSurface9() = 0;

protected:
	DX9GraphicsDevice*	m_graphicsDevice;
};

/// 普通のテクスチャ
class DX9Texture
	: public DX9TextureBase
{
public:
	/// テクスチャ作成 (level : ミップマップレベル ( 0 で全て作成 ))
	DX9Texture(DX9GraphicsDevice* device, const Size& size, TextureFormat format, uint32_t levels);

	/// テクスチャ作成 (メモリに展開された画像データから)
	DX9Texture(DX9GraphicsDevice* device, const void* data, uint32_t size, const Color& colorKey, uint32_t levels, TextureFormat format);

	virtual ~DX9Texture();

public:
	// override IDeviceObject
	virtual void OnLostDevice() {}
	virtual void OnResetDevice() {}

	// override ITexture
	virtual TextureType GetTextureType() const { return TextureType_Normal; }
	virtual TextureFormat GetTextureFormat() const { return m_format; }
	virtual const Size& GetSize() const { return m_size; }
	virtual const Size& GetRealSize() const { return m_realSize; }
	virtual void SetSamplerState(const SamplerState& state) { m_samplerState = state; }
	virtual void SetSubData(const Point& point, const void* data, size_t dataBytes, const Size& dataBitmapSize);
	virtual Bitmap* Lock();
	virtual void Unlock();

	// override GLTextureBase
	virtual IDirect3DTexture9* GetIDirect3DTexture9() { return m_dxTexture; }
	virtual IDirect3DSurface9* GetIDirect3DSurface9() { return m_dxSurface; }

private:
	IDirect3DTexture9*		m_dxTexture;	///< Direct3DTexture 本体
	IDirect3DSurface9*		m_dxSurface;	///< テクスチャのサーフェイス
	TextureFormat			m_format;
	Size					m_size;
	Size					m_realSize;
	SamplerState			m_samplerState;
	//ByteBuffer				m_lockedBuffer;
	RefPtr<Bitmap>	m_lockedBitmap;
};

/// レンダーターゲットテクスチャ
class DX9RenderTargetTexture
	: public DX9TextureBase
{
public:
	DX9RenderTargetTexture(DX9GraphicsDevice* device, const Size& size, TextureFormat format, int mipLevels);
	virtual ~DX9RenderTargetTexture();

public:
	// override IDeviceObject
	virtual void OnLostDevice();
	virtual void OnResetDevice();

	// override ITexture
	virtual TextureType GetTextureType() const { return TextureType_RenderTarget; }
	virtual TextureFormat GetTextureFormat() const { return m_format; }
	virtual const Size& GetSize() const { return m_size; }
	virtual const Size& GetRealSize() const { return m_realSize; }
	virtual void SetSamplerState(const SamplerState& state) { LN_THROW(0, InvalidOperationException); }
	virtual void SetSubData(const Point& point, const void* data, size_t dataBytes, const Size& dataBitmapSize) { LN_THROW(0, InvalidOperationException); }
	virtual Bitmap* Lock();
	virtual void Unlock();

	// override GLTextureBase
	virtual IDirect3DTexture9* GetIDirect3DTexture9() { return m_dxTexture; }
	virtual IDirect3DSurface9* GetIDirect3DSurface9() { return m_dxSurface; }

private:
	IDirect3DTexture9*	m_dxTexture;
	IDirect3DSurface9*	m_dxSurface;
	TextureFormat		m_format;
	Size				m_size;
	Size				m_realSize;
	int					m_mipLevels;
};

/// 深度バッファ
class DX9DepthBuffer
	: public DX9TextureBase
{
public:
	DX9DepthBuffer(DX9GraphicsDevice* device, const Size& size, TextureFormat format);
	virtual ~DX9DepthBuffer();

public:
	// override IDeviceObject
	virtual void OnLostDevice();
	virtual void OnResetDevice();

	// override ITexture
	virtual TextureType GetTextureType() const { return TextureType_DepthBuffer; }
	virtual TextureFormat GetTextureFormat() const { return m_format; }
	virtual const Size& GetSize() const { return m_size; }
	virtual const Size& GetRealSize() const { return m_realSize; }
	virtual void SetSamplerState(const SamplerState& state) { LN_THROW(0, InvalidOperationException); }
	virtual void SetSubData(const Point& point, const void* data, size_t dataBytes, const Size& dataBitmapSize) { LN_THROW(0, InvalidOperationException); }
	virtual Bitmap* Lock() { LN_THROW(0, InvalidOperationException); }
	virtual void Unlock() { LN_THROW(0, InvalidOperationException); }

	// override GLTextureBase
	virtual IDirect3DTexture9* GetIDirect3DTexture9() { return NULL; }
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
	// override IDeviceObject
	virtual void OnLostDevice() {}
	virtual void OnResetDevice() {}

	// override ITexture
	virtual TextureType GetTextureType() const { return TextureType_RenderTarget; }
	virtual TextureFormat GetTextureFormat() const { return m_format; }
	virtual const Size& GetSize() const { return m_realSize; }
	virtual const Size& GetRealSize() const { return m_realSize; }
	virtual void SetSamplerState(const SamplerState& state) { LN_THROW(0, InvalidOperationException); }
	virtual void SetSubData(const Point& point, const void* data, size_t dataBytes, const Size& dataBitmapSize) { LN_THROW(0, InvalidOperationException); }
	virtual Bitmap* Lock();
	virtual void Unlock();

	// override GLTextureBase
	virtual IDirect3DTexture9* GetIDirect3DTexture9() { return NULL; }
	virtual IDirect3DSurface9* GetIDirect3DSurface9() { return m_dxSurface; }

private:
	IDirect3DSurface9*	m_dxSurface;
	TextureFormat		m_format;
	Size				m_realSize;

	IDirect3DSurface9*		m_lockedSystemSurface;
	ByteBuffer				m_lockedBuffer;
	RefPtr<Bitmap>	m_lockedBitmap;
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
