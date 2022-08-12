#include "GLIndexBuffer.hpp"

namespace ln {
namespace detail {

//==============================================================================
// GLIndexBuffer

GLIndexBuffer::GLIndexBuffer()
    : m_objectId(0)
    , m_format(IndexBufferFormat::UInt16)
    , m_size(0) {
}

GLIndexBuffer::~GLIndexBuffer() {
    LN_CHECK(m_objectId == 0);
}

Result GLIndexBuffer::init(GraphicsResourceUsage usage, IndexBufferFormat format, uint32_t indexCount, const void* initialData) {
    LN_TRY(RHIResource::initAsIndexBuffer(usage, format, indexCount));

    m_format = format;
    size_t stride = (m_format == IndexBufferFormat::UInt16) ? 2 : 4;
    m_size = stride * indexCount;

    GL_CHECK(glGenBuffers(1, &m_objectId));
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_objectId));
    GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, initialData, (usage == GraphicsResourceUsage::Static) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    return ok();
}

void GLIndexBuffer::dispose() {
    if (m_objectId) {
        GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        GL_CHECK(glDeleteBuffers(1, &m_objectId));
        m_objectId = 0;
    }
    RHIResource::dispose();
}

void GLIndexBuffer::setSubData(size_t offset, const void* data, size_t length) {
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_objectId));
    GL_CHECK(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, length, data));
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void* GLIndexBuffer::map(uint64_t offset, uint64_t size) {
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_objectId));
    void* buffer;
    GL_CHECK(buffer = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, offset, m_size, GL_MAP_WRITE_BIT));
    return buffer;
}

void GLIndexBuffer::unmap() {
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_objectId));
    GL_CHECK(glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER));
}

} // namespace detail
} // namespace ln
