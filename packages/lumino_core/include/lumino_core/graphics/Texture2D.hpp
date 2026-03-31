#pragma once

#include <lumino_core/Object.hpp>
#include <lumino_base/RefObject.hpp>
#include <lumino_core/graphics/rhi/Rhi.hpp>
#include <cstdint>

namespace ln {

/**
 * C-API で公開する 2D テクスチャオブジェクト。
 * RHI テクスチャをラップし、ハンドル管理の基盤として機能する。
 */
class Texture2D : public Object {
public:
    Texture2D(uint32_t width, uint32_t height, uint32_t format);

    /** RHI テクスチャから構築（TextureLoader の結果をラップ）。 */
    Texture2D(Ref<rhi::Texture> rhiTexture, uint32_t width, uint32_t height);

    uint32_t width() const { return m_width; }
    uint32_t height() const { return m_height; }
    uint32_t format() const { return m_format; }

    /** 内部の RHI テクスチャ（未設定時は nullptr）。 */
    rhi::Texture* rhiTexture() const { return m_rhiTexture.get(); }

private:
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_format;
    Ref<rhi::Texture> m_rhiTexture;
};

} // namespace ln
