#pragma once
#include "Common.hpp"
#include <LuminoGraphicsRHI/GraphicsDeviceContext.hpp>

namespace ln {
namespace detail {
class OpenGLDevice;

class GLTextureBase : public RHIResource {
public:
    GLTextureBase();
    virtual ~GLTextureBase() = default;

    virtual GLuint id() const = 0;
    virtual bool mipmap() const = 0;
    virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) = 0;
    virtual void setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) = 0;

private:
};

class GLTexture2D : public GLTextureBase {
public:
    GLTexture2D();
    virtual ~GLTexture2D();
    Result<> init(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData);
    void onDestroy() override;

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
    Result<> init(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData);
    virtual void onDestroy() override;

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
    Result<> init(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap);
    Result<> init(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight);
    void onDestroy() override;

    virtual RHIRef<RHIBitmap> readData() override;
    virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) override;
    virtual void setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override;

    virtual GLuint id() const override { return m_id; }
    virtual bool mipmap() const override { return false; }

    bool isBackbuffer() const { return m_id == 0; }

private:
    GLuint m_id;
    GLenum m_pixelFormat;
    GLenum m_elementType;
};

class GLSamplerState : public ISamplerState {
public:
    GLSamplerState();
    virtual ~GLSamplerState();
    void init(OpenGLDevice* device, const SamplerStateData& desc);
    virtual void onDestroy() override;

    GLuint resolveId(bool mipmap) const { return (mipmap) ? m_idMip : m_id; }

private:
    GLuint m_id;
    GLuint m_idMip;
    SamplerStateData m_desc;
};

} // namespace detail
} // namespace ln
