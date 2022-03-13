#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {
class OpenGLDevice;

class GLDepthBuffer : public RHIResource {
public:
    GLDepthBuffer();
    Result init(uint32_t width, uint32_t height);
    void dispose() override;
    GLuint id() const { return m_id; }

private:
    GLuint m_id;
};

} // namespace detail
} // namespace ln
