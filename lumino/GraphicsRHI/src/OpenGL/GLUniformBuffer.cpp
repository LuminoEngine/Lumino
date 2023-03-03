#include "GLUniformBuffer.hpp"

namespace ln {
namespace detail {

//==============================================================================
// GLUniformBuffer

GLUniformBuffer::GLUniformBuffer()
    : m_ubo(0)
    , m_size(0)
    , m_data(nullptr)
    , m_mapped(false) {
}

Result<> GLUniformBuffer::init(size_t size) {
    LN_TRY(RHIResource::initAsUniformBuffer(GraphicsResourceUsage::Dynamic, size));
    m_size = size;

    // 大きなバッファをずっと持つ可能性があるが、UBO は通常、毎フレームデータの書き込みが行われるため、
    // map() のたびに大きなバッファを new するとオーバーヘッドが大きくなる。
    m_data = LN_NEW uint8_t[m_size];

    GL_CHECK(glGenBuffers(1, &m_ubo));
    //GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, info.ubo));
    //GL_CHECK(glBufferData(GL_UNIFORM_BUFFER, info.blockSize, nullptr, GL_DYNAMIC_DRAW));
    //GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, 0));

    map();
    unmap();

    return ok();
}

void GLUniformBuffer::onDestroy() {
    if (m_ubo) {
        GL_CHECK(glDeleteBuffers(1, &m_ubo));
        m_ubo = 0;
    }
    LN_SAFE_DELETE_ARRAY(m_data);
    RHIResource::onDestroy();
}

// モバイル環境で glMapBuffer が使えないことがあるため、glBufferData() で対応する。
void* GLUniformBuffer::map() {
    if (LN_REQUIRE(!m_mapped)) return nullptr;
    m_mapped = true;
    return m_data;
}

void GLUniformBuffer::unmap() {
    if (LN_REQUIRE(m_mapped)) return;
    GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, m_ubo));
    GL_CHECK(glBufferData(GL_UNIFORM_BUFFER, m_size, m_data, GL_STREAM_DRAW));
    GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, 0));
    m_mapped = false;
}

void GLUniformBuffer::flush() {
    if (m_mapped) {
        unmap();
        map();
    }
}

} // namespace detail
} // namespace ln

