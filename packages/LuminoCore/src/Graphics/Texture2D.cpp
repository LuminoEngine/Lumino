#include <LuminoCore/Graphics/Texture2D.hpp>

namespace ln {

Texture::Texture(uint32_t width, uint32_t height, rhi::TextureFormat format)
    : m_width(width)
    , m_height(height)
    , m_format(format) {
}

Texture::Texture(Ref<rhi::Texture> rhiTexture, uint32_t width, uint32_t height)
    : m_width(width)
    , m_height(height)
    , m_format(rhi::TextureFormat::Undefined)
    , m_rhiTexture(std::move(rhiTexture)) {
}

Ref<Texture> Texture::createBackbufferWrapper() {
    // バックバッファは実際のテクスチャリソースを持たないダミーオブジェクトとして作成。
    // GraphicsContext::beginFrame() で毎フレーム更新されるバックバッファビューを提供するためのもの。
    return Ref<Texture>::adopt(new Texture(0, 0, 0));
}

Result<Ref<Texture>> Texture::createDepthStencil(
    rhi::Device* device,
    uint32_t width,
    uint32_t height) {

    // 深度テクスチャ (Depth24Stencil8 - スワップチェーンの深度と一致させる)
    rhi::TextureDesc depthDesc;
    depthDesc.width = width;
    depthDesc.height = height;
    depthDesc.format = rhi::TextureFormat::Depth24Stencil8;
    depthDesc.usage = rhi::TextureUsage::DepthStencil;
    auto depthResult = device->createTexture(depthDesc);
    if (!depthResult) return LN_FORWARD_ERROR(depthResult);

    auto depthViewResult = device->createTextureView(depthResult->get());
    if (!depthViewResult) return LN_FORWARD_ERROR(depthViewResult);

    auto tex = Ref<Texture>::adopt(new Texture(
        width,
        height,
        rhi::TextureFormat::Depth24Stencil8));
    tex->m_isRenderTarget = true;
    tex->m_rhiTexture = std::move(*depthResult);
    tex->m_rhiTextureView = std::move(*depthViewResult);
    return tex;
}

Result<Ref<Texture>> Texture::createRenderTarget(
    rhi::Device* device, uint32_t width, uint32_t height, rhi::TextureFormat format) {
    rhi::TextureDesc colorDesc;
    colorDesc.width = width;
    colorDesc.height = height;
    colorDesc.format = format;
    colorDesc.usage = rhi::TextureUsage::Sampled | rhi::TextureUsage::RenderTarget;
    auto colorResult = device->createTexture(colorDesc);
    if (!colorResult) return LN_FORWARD_ERROR(colorResult);

    auto colorViewResult = device->createTextureView(colorResult->get());
    if (!colorViewResult) return LN_FORWARD_ERROR(colorViewResult);

    auto tex = Ref<Texture>::adopt(new Texture(width, height, format));
    tex->m_isRenderTarget = true;
    tex->m_rhiTexture = std::move(*colorResult);
    tex->m_rhiTextureView = std::move(*colorViewResult);
    return tex;
}

void Texture::wrapBackbuffer(
    rhi::TextureView* rhiTextureView,
    uint32_t width,
    uint32_t height,
    rhi::TextureFormat format) {
    m_width = width;
    m_height = height;
    m_format = format;
    m_rhiTextureView = Ref<rhi::TextureView>::retain(rhiTextureView);
}

} // namespace ln
