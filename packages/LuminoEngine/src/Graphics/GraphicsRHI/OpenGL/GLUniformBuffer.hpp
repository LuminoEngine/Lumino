#pragma once
#include "GLHelper.hpp"

namespace ln {
namespace detail {

class GLUniformBuffer : public RHIResource {
public:
    GLUniformBuffer();
    Result<> init(size_t size);
    void onDestroy() override;
    void* map() override;
    void unmap() override;

    GLuint ubo() const { return m_ubo; }
    void flush();

private:
    GLuint m_ubo;
    size_t m_size;
    uint8_t* m_data;
    bool m_mapped;
};

} // namespace detail
} // namespace ln
