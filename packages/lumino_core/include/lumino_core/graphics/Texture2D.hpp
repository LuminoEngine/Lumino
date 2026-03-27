#pragma once

#include <lumino_core/Object.hpp>
#include <cstdint>

namespace ln {

namespace rhi {
enum class TextureFormat;
}

/**
 * C-API で公開する 2D テクスチャオブジェクト。
 * 現時点では RHI テクスチャとの接続は行わず、パラメータ保持とハンドル管理の基盤として機能する。
 */
class Texture2D : public Object {
public:
    Texture2D(uint32_t width, uint32_t height, uint32_t format);

    uint32_t width() const { return m_width; }
    uint32_t height() const { return m_height; }
    uint32_t format() const { return m_format; }

private:
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_format;
};

} // namespace ln
