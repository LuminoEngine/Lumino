#include "GLHelper.hpp"
#include "GLDepthBuffer.hpp"

namespace ln {
namespace detail {

//=============================================================================
// GLDepthBuffer

GLDepthBuffer::GLDepthBuffer()
    : m_id(0) {
}

Result GLDepthBuffer::init(uint32_t width, uint32_t height) {
    if (LN_REQUIRE(!m_id)) return err();
    LN_TRY(RHIResource::initAsDepthBuffer(width, height, false));

    GL_CHECK(glGenRenderbuffers(1, &m_id));
    GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, m_id));
    GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
    GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, 0));

    return ok();
}

void GLDepthBuffer::dispose() {
    if (m_id != 0) {
        GL_CHECK(glDeleteRenderbuffers(1, &m_id));
        m_id = 0;
    }
    RHIResource::dispose();
}

} // namespace detail
} // namespace ln
