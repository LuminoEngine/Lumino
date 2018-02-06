
#pragma once 
#include <Lumino/Graphics/SamplerState.h>
#include <Lumino/Graphics/Bitmap.h>
#include "../GraphicsDriverInterface.h"
#include "GLCommon.h"

LN_NAMESPACE_BEGIN
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
	virtual GLuint getGLTexture() = 0;

public:
	static void getGLTextureFormat(TextureFormat format, GLenum* internalFormat, GLenum* pixelFormat, GLenum* elementType);
	static void setGLSamplerState(const SamplerState& state);
};

/// 普通のテクスチャ
class GLTexture
	: public GLTextureBase
{
public:
	GLTexture(const SizeI& size, TextureFormat format, bool mipmap);
	//GLTexture(const RawBitmap* bitmap, TextureFormat format, uint32_t mipLevels);
	virtual ~GLTexture();

public:
	// override IDeviceObject
	virtual void onLostDevice() override;
	virtual void onResetDevice() override;

	// override ITexture
	virtual TextureType getTextureType() const override { return TextureType_Normal; }
	virtual TextureFormat getTextureFormat() const override { return m_format; }
	virtual const SizeI& getSize() const override { return m_size; }
	virtual const SizeI& getRealSize() const override { return m_realSize; }
	virtual void setSamplerState(const SamplerState& state) override;
	virtual void setSubData(const PointI& point, const void* data, size_t dataBytes, const SizeI& dataBitmapSize) override;
	virtual void setSubData3D(const Box32& box, const void* data, size_t dataBytes) override;
	virtual void getData(const RectI& rect, void* outData) override;
	virtual RawBitmap* lock();
	virtual void unlock();

	// override GLTextureBase
	virtual GLuint getGLTexture() override { return m_glTexture; }

private:
	GLuint				m_glTexture;
	TextureFormat		m_format;
	SizeI				m_size;
	SizeI				m_realSize;
	bool				m_mipmap;
	GLenum				m_pixelFormat;
	GLenum				m_elementType;
	SamplerState		m_samplerState;
	Ref<RawBitmap>	m_lockedTexture;		///< lock ～ unlock で作られる
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
	virtual void onLostDevice();
	virtual void onResetDevice();

	// override ITexture
	virtual TextureType getTextureType() const { return TextureType_RenderTarget; }
	virtual TextureFormat getTextureFormat() const { return m_format; }
	virtual const SizeI& getSize() const { return m_size; }
	virtual const SizeI& getRealSize() const { return m_realSize; }
	virtual void setSamplerState(const SamplerState& state) { LN_UNREACHABLE(); }
	virtual void setSubData(const PointI& point, const void* data, size_t dataBytes, const SizeI& dataBitmapSize) { LN_UNREACHABLE(); }
	virtual void setSubData3D(const Box32& box, const void* data, size_t dataBytes);
	virtual void getData(const RectI& rect, void* outData) override;
	virtual RawBitmap* lock();
	virtual void unlock();

	// override GLTextureBase
	virtual GLuint getGLTexture() { return m_glTexture; }

private:
	GLuint				m_glTexture;
	TextureFormat		m_format;
	SizeI				m_size;
	SizeI				m_realSize;
	int					m_mipLevels;
	GLenum				m_pixelFormat;
	GLenum				m_elementType;
	RawBitmap*	m_lockingBitmap;
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
	virtual void onLostDevice();
	virtual void onResetDevice();

	// override ITexture
	virtual TextureType getTextureType() const { return TextureType_DepthBuffer; }
	virtual TextureFormat getTextureFormat() const { return m_format; }
	virtual const SizeI& getSize() const { return m_size; }
	virtual const SizeI& getRealSize() const { return m_realSize; }
	virtual void setSamplerState(const SamplerState& state) { LN_UNREACHABLE(); }
	virtual void setSubData(const PointI& point, const void* data, size_t dataBytes, const SizeI& dataBitmapSize) { LN_UNREACHABLE(); }
	virtual void setSubData3D(const Box32& box, const void* data, size_t dataBytes);
	virtual void getData(const RectI& rect, void* outData) override;
	virtual RawBitmap* lock() { LN_UNREACHABLE(); return NULL; }
	virtual void unlock() { LN_UNREACHABLE(); }

	// override GLTextureBase
	virtual GLuint getGLTexture() { return m_glBuffer; }

private:
	GLuint			m_glBuffer;
	TextureFormat	m_format;
	SizeI			m_size;
	SizeI			m_realSize;
};

} // namespace Driver
LN_NAMESPACE_END
