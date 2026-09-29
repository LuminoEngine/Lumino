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

VoidResult Texture::writePixels(
    rhi::Device* device,
    uint32_t x,
    uint32_t y,
    uint32_t width,
    uint32_t height,
    const void* data,
    uint64_t size) {
    // LoadFromMemory 等は幅・高さ 0 のまま包んでいるため、検証には RHI テクスチャ側の値を使う。
    if (!m_rhiTexture || m_isRenderTarget) {
        return LN_MAKE_ERROR_WITH_CODE(ErrorCode::InvalidArgument,
            "Texture is not writable. Only textures created from pixels or images can be written.");
    }
    const uint32_t texWidth = m_rhiTexture->width();
    const uint32_t texHeight = m_rhiTexture->height();
    if (width == 0 || height == 0 || x > texWidth || y > texHeight ||
        width > texWidth - x || height > texHeight - y) {
        return LN_MAKE_ERROR_WITH_CODE(ErrorCode::InvalidArgument,
            "Write region (%u, %u, %u, %u) is out of texture bounds (%u x %u).",
            x, y, width, height, texWidth, texHeight);
    }
    const uint64_t expectedSize =
        static_cast<uint64_t>(width) * height * rhi::bytesPerPixel(m_rhiTexture->format());
    if (!data || size != expectedSize) {
        return LN_MAKE_ERROR_WITH_CODE(ErrorCode::InvalidArgument,
            "Pixel data size mismatch. (expected: %llu, actual: %llu)",
            static_cast<unsigned long long>(expectedSize), static_cast<unsigned long long>(size));
    }
    return device->writeTexture(m_rhiTexture.get(), x, y, width, height, data, size);
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
