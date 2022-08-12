#include "GLVertexBuffer.hpp"

namespace ln {
namespace detail {

//==============================================================================
// GLVertexBuffer

GLVertexBuffer::GLVertexBuffer()
    : m_objectId(0)
    , m_usage(GraphicsResourceUsage::Static)
    , m_size(0) {
}

GLVertexBuffer::~GLVertexBuffer() {
    LN_CHECK(m_objectId == 0);
}

Result GLVertexBuffer::init(GraphicsResourceUsage usage, uint64_t bufferSize, const void* initialData) {
    LN_TRY(RHIResource::initAsVertexBuffer(usage, bufferSize));

    m_usage = usage;
    m_size = bufferSize;

    GL_CHECK(glGenBuffers(1, &m_objectId));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_objectId));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, bufferSize, initialData, (m_usage == GraphicsResourceUsage::Static) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
    return ok();
}

void GLVertexBuffer::dispose() {
    if (m_objectId) {
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GL_CHECK(glDeleteBuffers(1, &m_objectId));
        m_objectId = 0;
    }
    RHIResource::dispose();
}

void GLVertexBuffer::setSubData(size_t offset, const void* data, size_t length) {
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_objectId));
    GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, offset, length, data));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void* GLVertexBuffer::map(uint64_t offset, uint64_t size) {
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_objectId));
    void* buffer;

    // https://developer.apple.com/jp/documentation/3DDrawing/Conceptual/OpenGLES_ProgrammingGuide/TechniquesforWorkingwithVertexData/TechniquesforWorkingwithVertexData.html

    GL_CHECK(buffer = glMapBufferRange(GL_ARRAY_BUFFER, offset, size, GL_MAP_WRITE_BIT));

    return buffer;
}

void GLVertexBuffer::unmap() {
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_objectId));
    GL_CHECK(glUnmapBuffer(GL_ARRAY_BUFFER));
}

} // namespace detail
} // namespace ln
