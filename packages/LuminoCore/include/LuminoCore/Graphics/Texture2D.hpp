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
class Texture : public Object {
public:
    Texture(uint32_t width, uint32_t height, rhi::TextureFormat format);

    /** RHI テクスチャから構築（TextureLoader の結果をラップ）。 */
    Texture(Ref<rhi::Texture> rhiTexture, uint32_t width, uint32_t height);

    /**
     * バックバッファをラップしてクライアントに提供するダミーの Texture オブジェクトを作成します。
     * この Texture は実際のテクスチャリソースを持たず、GraphicsContext::beginFrame() で
     * 毎フレーム更新されるバックバッファビューを提供するためのものです。
     */
    static Ref<Texture> createBackbufferWrapper();

    /**
     * 深度ステンシルテクスチャを作成します。
     */
    static Result<Ref<Texture>> createDepthStencil(
        rhi::Device* device,
        uint32_t width,
        uint32_t height);

    /**
     * 指定フォーマットのレンダーターゲットテクスチャ (カラーのみ) を作成します。
     * Sampled|RenderTarget のカラーテクスチャを生成します。
     * バックバッファと互換にする場合は GraphicsContext::colorFormat() を渡してください
     * (スワップチェーンのフォーマットは環境依存で、BGRA8 とは限りません)。
     * 深度バッファは createDepthStencil() で別途作成してください。
     */
    static Result<Ref<Texture>> createRenderTarget(
        rhi::Device* device, uint32_t width, uint32_t height, rhi::TextureFormat format);

    uint32_t width() const { return m_width; }
    uint32_t height() const { return m_height; }
    rhi::TextureFormat format() const { return m_format; }

    /** 内部の RHI テクスチャ（未設定時は nullptr）。 */
    rhi::Texture* rhiTexture() const { return m_rhiTexture.get(); }

    /** カラーテクスチャの TextureView（RT の場合のみ有効）。 */
    rhi::TextureView* rhiTextureView() const { return m_rhiTextureView.get(); }

    /** レンダーターゲットとして作成されたかどうか。 */
    bool isRenderTarget() const { return m_isRenderTarget; }

    void wrapBackbuffer(
        rhi::TextureView* rhiTextureView,
        uint32_t width,
        uint32_t height,
        rhi::TextureFormat format);

private:
    uint32_t m_width;
    uint32_t m_height;
    rhi::TextureFormat m_format;
    bool m_isRenderTarget = false;
    Ref<rhi::Texture> m_rhiTexture;
    Ref<rhi::TextureView> m_rhiTextureView;
};

} // namespace ln
