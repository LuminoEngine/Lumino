
#pragma once 
#include <Lumino/Graphics/SamplerState.h>
#include <Lumino/Graphics/Bitmap.h>
#include "../GraphicsDriverInterface.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

/// OpenGL 用の ITexture の実装
class GLTextureBase
	: public ITexture
{
public:
	GLTextureBase() {}
	virtual ~GLTextureBase() {}

public:
	/// GLテクスチャの取得
	virtual GLuint GetGLTexture() = 0;

public:
	static void GetGLTextureFormat(TextureFormat format, GLenum* internalFormat, GLenum* pixelFormat, GLenum* elementType);
	static void SetGLSamplerState(const SamplerState& state);
};

/// 普通のテクスチャ
class GLTexture
	: public GLTextureBase
{
public:
	GLTexture(const SizeI& size, TextureFormat format, bool mipmap);
	//GLTexture(const Bitmap* bitmap, TextureFormat format, uint32_t mipLevels);
	virtual ~GLTexture();

public:
	// override IDeviceObject
	virtual void OnLostDevice();
	virtual void OnResetDevice();

	// override ITexture
	virtual TextureType GetTextureType() const { return TextureType_Normal; }
	virtual TextureFormat GetTextureFormat() const { return m_format; }
	virtual const SizeI& GetSize() const { return m_size; }
	virtual const SizeI& GetRealSize() const { return m_realSize; }
	virtual void SetSamplerState(const SamplerState& state);
	virtual void SetSubData(const PointI& point, const void* data, size_t dataBytes, const SizeI& dataBitmapSize);
	virtual void SetSubData3D(const Box32& box, const void* data, size_t dataBytes);
	virtual void GetData(const RectI& rect, void* outData) override;
	virtual Bitmap* Lock();
	virtual void Unlock();

	// override GLTextureBase
	virtual GLuint GetGLTexture() { return m_glTexture; }

private:
	GLuint				m_glTexture;
	TextureFormat		m_format;
	SizeI				m_size;
	SizeI				m_realSize;
	bool				m_mipmap;
	GLenum				m_pixelFormat;
	GLenum				m_elementType;
	SamplerState		m_samplerState;
	RefPtr<Bitmap>	m_lockedTexture;		///< Lock ～ Unlock で作られる
};

/// レンダーターゲットテクスチャ
class GLRenderTargetTexture
	: public GLTextureBase
{
public:
	GLRenderTargetTexture(const SizeI& size, TextureFormat format, int mipLevels);
	virtual ~GLRenderTargetTexture();

public:
	// override IDeviceObject
	virtual void OnLostDevice();
	virtual void OnResetDevice();

	// override ITexture
	virtual TextureType GetTextureType() const { return TextureType_RenderTarget; }
	virtual TextureFormat GetTextureFormat() const { return m_format; }
	virtual const SizeI& GetSize() const { return m_size; }
	virtual const SizeI& GetRealSize() const { return m_realSize; }
	virtual void SetSamplerState(const SamplerState& state) { LN_THROW(0, InvalidOperationException); }
	virtual void SetSubData(const PointI& point, const void* data, size_t dataBytes, const SizeI& dataBitmapSize) { LN_THROW(0, InvalidOperationException); }
	virtual void SetSubData3D(const Box32& box, const void* data, size_t dataBytes);
	virtual void GetData(const RectI& rect, void* outData) override;
	virtual Bitmap* Lock();
	virtual void Unlock();

	// override GLTextureBase
	virtual GLuint GetGLTexture() { return m_glTexture; }

private:
	GLuint				m_glTexture;
	TextureFormat		m_format;
	SizeI				m_size;
	SizeI				m_realSize;
	int					m_mipLevels;
	GLenum				m_pixelFormat;
	GLenum				m_elementType;
	Bitmap*	m_lockingBitmap;
};

/// 深度バッファ
class GLDepthBuffer
	: public GLTextureBase
{
public:
	GLDepthBuffer(const SizeI& size, TextureFormat format);
	virtual ~GLDepthBuffer();

public:
	// override IDeviceObject
	virtual void OnLostDevice();
	virtual void OnResetDevice();

	// override ITexture
	virtual TextureType GetTextureType() const { return TextureType_DepthBuffer; }
	virtual TextureFormat GetTextureFormat() const { return m_format; }
	virtual const SizeI& GetSize() const { return m_size; }
	virtual const SizeI& GetRealSize() const { return m_realSize; }
	virtual void SetSamplerState(const SamplerState& state) { LN_THROW(0, InvalidOperationException); }
	virtual void SetSubData(const PointI& point, const void* data, size_t dataBytes, const SizeI& dataBitmapSize) { LN_THROW(0, InvalidOperationException); }
	virtual void SetSubData3D(const Box32& box, const void* data, size_t dataBytes);
	virtual void GetData(const RectI& rect, void* outData) override;
	virtual Bitmap* Lock() { LN_THROW(0, InvalidOperationException); return NULL; }
	virtual void Unlock() { LN_THROW(0, InvalidOperationException); }

	// override GLTextureBase
	virtual GLuint GetGLTexture() { return m_glBuffer; }

private:
	GLuint			m_glBuffer;
	TextureFormat	m_format;
	SizeI			m_size;
	SizeI			m_realSize;
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
