#include <lumino_core/graphics/Texture2D.hpp>

namespace ln {

Texture2D::Texture2D(uint32_t width, uint32_t height, uint32_t format)
    : m_width(width)
    , m_height(height)
    , m_format(format) {
}

} // namespace ln
