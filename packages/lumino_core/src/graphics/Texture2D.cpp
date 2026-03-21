#include <lumino_core/graphics/Texture2D.hpp>

namespace ln {

Texture2D::Texture2D(uint32_t width, uint32_t height, uint32_t format)
    : width_(width)
    , height_(height)
    , format_(format) {
}

} // namespace ln
