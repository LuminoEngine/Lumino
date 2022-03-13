#pragma once
#include "Common.hpp"
#include "../GraphicsDeviceContext.hpp"

namespace ln {
namespace detail {
class OpenGLDevice;

class GLTextureBase : public RHIResource {
public:
    GLTextureBase();
    virtual ~GLTextureBase() = default;
    Result init(uint32_t width, uint32_t height, uint32_t depth = 1);
    const RHISize3D& size() const { return m_size; }

    virtual GLuint id() const = 0;
    virtual bool mipmap() const = 0;
    virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) = 0;
    virtual void setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) = 0;

private:
    RHISize3D m_size;
};

class GLTexture2D : public GLTextureBase {
public:
    GLTexture2D();
    virtual ~GLTexture2D();
    Result init(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData);
    virtual void dispose() override;

    //virtual DeviceTextureType type() const override { return DeviceTextureType::Texture2D; }
    virtual RHIRef<RHIBitmap> readData() override;
    //virtual SizeI realSize() override;
    //virtual TextureFormat getTextureFormat() const override;
    //virtual GraphicsResourceUsage usage() const override { return m_usage; }
    virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) override;
    virtual void setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override;

    virtual GLuint id() const override { return m_id; }
    virtual bool mipmap() const override { return m_mipmap; }

private:
    GLuint m_id;
    GraphicsResourceUsage m_usage;
    TextureFormat m_textureFormat;
    GLenum m_pixelFormat;
    GLenum m_elementType;
    bool m_mipmap;
};

class GLTexture3D : public GLTextureBase {
public:
    GLTexture3D();
    virtual ~GLTexture3D();
    Result init(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData);
    virtual void dispose() override;

    //virtual DeviceTextureType type() const override { return DeviceTextureType::Texture3D; }
    virtual RHIRef<RHIBitmap> readData() override;
    //virtual SizeI realSize() override;
    //virtual TextureFormat getTextureFormat() const override;
    //virtual GraphicsResourceUsage usage() const override { return m_usage; }
    virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) override;
    virtual void setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override;

    virtual GLuint id() const override { return m_id; }
    virtual bool mipmap() const override { return false; }

private:
    GLuint m_id;
    GraphicsResourceUsage m_usage;
    TextureFormat m_textureFormat;
    GLenum m_pixelFormat;
    GLenum m_elementType;
};

class GLRenderTargetTexture : public GLTextureBase {
public:
    GLRenderTargetTexture();
    virtual ~GLRenderTargetTexture();
    Result init(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap);
    Result init(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight);
    virtual void dispose() override;

    //virtual DeviceTextureType type() const override { return DeviceTextureType::RenderTarget; }
    virtual RHIRef<RHIBitmap> readData() override;
    //virtual SizeI realSize() override;
    //virtual TextureFormat getTextureFormat() const override;
    //virtual GraphicsResourceUsage usage() const override { return GraphicsResourceUsage::Static; }
    virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) override;
    virtual void setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override;

    virtual GLuint id() const override { return m_id; }
    virtual bool mipmap() const override { return false; }

private:
    TextureFormat m_textureFormat;
    GLuint m_id;
    GLenum m_pixelFormat;
    GLenum m_elementType;

    //public:
    //	// override IDeviceObject
    //	virtual void onLostDevice();
    //	virtual void onResetDevice();
    //
    //	// override ITexture
    //	virtual TextureType getTextureType() const { return TextureType_RenderTarget; }
    //	virtual TextureFormat getTextureFormat() const { return m_format; }
    //	virtual const SizeI& getSize() const { return m_size; }
    //	virtual const SizeI& getRealSize() const { return m_realSize; }
    //	virtual void setSamplerState(const SamplerState& state) { LN_UNREACHABLE(); }
    //	virtual void setSubData(const PointI& point, const void* data, size_t dataBytes, const SizeI& dataBitmapSize) { LN_UNREACHABLE(); }
    //	virtual void setSubData3D(const BoxI& box, const void* data, size_t dataBytes);
    //	virtual void getData(const RectI& rect, void* outData) override;
    //	virtual RawBitmap* lock();
    //	virtual void unlock();
    //
    //	// override GLTextureBase
    //	virtual GLuint getGLTexture() { return m_glTexture; }
    //
    //private:
    //	GLuint				m_glTexture;
    //	TextureFormat		m_format;
    //	SizeI				m_size;
    //	SizeI				m_realSize;
    //	int					m_mipLevels;
    //	GLenum				m_pixelFormat;
    //	GLenum				m_elementType;
    //	RawBitmap*	m_lockingBitmap;
};

class GLDepthBuffer : public IDepthBuffer {
public:
    GLDepthBuffer();
    void init(uint32_t width, uint32_t height);
    virtual void dispose() override;

    GLuint id() const { return m_id; }
    const SizeI& size() const { return m_size; }

private:
    GLuint m_id;
    SizeI m_size;
};

class GLSamplerState : public ISamplerState {
public:
    GLSamplerState();
    virtual ~GLSamplerState();
    void init(OpenGLDevice* device, const SamplerStateData& desc);
    virtual void dispose() override;

    GLuint resolveId(bool mipmap) const { return (mipmap) ? m_idMip : m_id; }

private:
    GLuint m_id;
    GLuint m_idMip;
    SamplerStateData m_desc;
};

} // namespace detail
} // namespace ln
