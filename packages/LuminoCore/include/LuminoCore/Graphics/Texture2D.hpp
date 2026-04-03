#pragma once

#include <LuminoCore/Object.hpp>
#include <LuminoBase/Result.hpp>
#include <LuminoBase/RefObject.hpp>
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
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

    /**
     * レンダーターゲットテクスチャを作成します。
     * カラーテクスチャ (BGRA8Unorm, Sampled|RenderTarget) と
     * 深度テクスチャ (Depth24Stencil8) を内部で生成します。
     */
    static Result<Ref<Texture2D>> createRenderTarget(rhi::Device* device, uint32_t width, uint32_t height);

    uint32_t width() const { return m_width; }
    uint32_t height() const { return m_height; }
    uint32_t format() const { return m_format; }

    /** 内部の RHI テクスチャ（未設定時は nullptr）。 */
    rhi::Texture* rhiTexture() const { return m_rhiTexture.get(); }

    /** カラーテクスチャの TextureView（RT の場合のみ有効）。 */
    rhi::TextureView* rhiTextureView() const { return m_rhiTextureView.get(); }

    /** 深度テクスチャの TextureView（RT の場合のみ有効）。 */
    rhi::TextureView* depthView() const { return m_depthView.get(); }

    /** レンダーターゲットとして作成されたかどうか。 */
    bool isRenderTarget() const { return m_isRenderTarget; }

private:
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_format;
    bool m_isRenderTarget = false;
    Ref<rhi::Texture> m_rhiTexture;
    Ref<rhi::TextureView> m_rhiTextureView;
    Ref<rhi::Texture> m_depthTexture;
    Ref<rhi::TextureView> m_depthView;
};

} // namespace ln
