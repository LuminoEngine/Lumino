#pragma once
#include "GLHelper.hpp"

namespace ln {
namespace detail {
class OpenGLContext;
class GLGraphicsContext;

class GLVertexBuffer : public RHIResource {
public:
    GLVertexBuffer();
    virtual ~GLVertexBuffer();
    void init(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData);
    virtual void dispose() override;

    GLuint getGLVertexBuffer() const { return m_glVertexBuffer; }

    GLuint vertexBufferId() const { return m_glVertexBuffer; }

    //virtual size_t getBytesSize() override { return m_size; }
    //virtual GraphicsResourceUsage usage() const override { return m_usage; }

    void setSubData(size_t offset, const void* data, size_t length);
    void* map(uint32_t offset, uint32_t size);
    virtual void* map() override { return map(0, m_size); }
    virtual void unmap() override;

private:
    GLuint m_glVertexBuffer;
    GraphicsResourceUsage m_usage;
    //GraphicsResourceUsage m_format;
    size_t m_size;
};

class GLIndexBuffer : public RHIResource {
public:
    GLIndexBuffer();
    virtual ~GLIndexBuffer();
    void init(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData);
    virtual void dispose() override;

    GLuint indexBufferId() const { return m_indexBufferId; }
    IndexBufferFormat format() const { return m_format; }

    //virtual size_t getBytesSize() override { return m_size; }
    //virtual GraphicsResourceUsage usage() const override { return m_usage; }

    void setSubData(size_t offset, const void* data, size_t length);
    void* map(uint32_t offset, uint32_t size);
    virtual void* map() override { return map(0, m_size); }
    virtual void unmap() override;

private:
    GLuint m_indexBufferId;
    IndexBufferFormat m_format;
    GraphicsResourceUsage m_usage;
    size_t m_size;
};

} // namespace detail
} // namespace ln
