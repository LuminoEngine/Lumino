#pragma once
#include "GLHelper.hpp"

namespace ln {
namespace detail {

class GLVertexBuffer : public RHIResource {
public:
    GLVertexBuffer();
    virtual ~GLVertexBuffer();
    Result init(GraphicsResourceUsage usage, uint64_t bufferSize, const void* initialData);
    void dispose() override;

    GLuint objectId() const { return m_objectId; }

    void setSubData(size_t offset, const void* data, size_t length);
    void* map(uint32_t offset, uint32_t size);
    void* map() override { return map(0, m_size); }
    void unmap() override;

private:
    GLuint m_objectId;
    GraphicsResourceUsage m_usage;
    size_t m_size;
};

} // namespace detail
} // namespace ln
