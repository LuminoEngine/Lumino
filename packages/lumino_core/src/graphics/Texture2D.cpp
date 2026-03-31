#include <lumino_core/graphics/Texture2D.hpp>

namespace ln {

Texture2D::Texture2D(uint32_t width, uint32_t height, uint32_t format)
    : m_width(width)
    , m_height(height)
    , m_format(format) {
}

Texture2D::Texture2D(Ref<rhi::Texture> rhiTexture, uint32_t width, uint32_t height)
    : m_width(width)
    , m_height(height)
    , m_format(0)
    , m_rhiTexture(std::move(rhiTexture)) {
}

} // namespace ln
