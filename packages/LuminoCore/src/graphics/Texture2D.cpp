#include <LuminoCore/graphics/Texture2D.hpp>

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

Result<Ref<Texture2D>> Texture2D::createRenderTarget(rhi::Device* device, uint32_t width, uint32_t height) {
    // Color texture (BGRA8Unorm — matches swapchain for pipeline compatibility)
    rhi::TextureDesc colorDesc;
    colorDesc.width = width;
    colorDesc.height = height;
    colorDesc.format = rhi::TextureFormat::BGRA8Unorm;
    colorDesc.usage = rhi::TextureUsage::Sampled | rhi::TextureUsage::RenderTarget;
    auto colorResult = device->createTexture(colorDesc);
    if (!colorResult) return tl::make_unexpected(colorResult.error());

    auto colorViewResult = device->createTextureView(colorResult->get());
    if (!colorViewResult) return tl::make_unexpected(colorViewResult.error());

    // Depth texture (Depth24Stencil8 — matches swapchain depth)
    rhi::TextureDesc depthDesc;
    depthDesc.width = width;
    depthDesc.height = height;
    depthDesc.format = rhi::TextureFormat::Depth24Stencil8;
    depthDesc.usage = rhi::TextureUsage::DepthStencil;
    auto depthResult = device->createTexture(depthDesc);
    if (!depthResult) return tl::make_unexpected(depthResult.error());

    auto depthViewResult = device->createTextureView(depthResult->get());
    if (!depthViewResult) return tl::make_unexpected(depthViewResult.error());

    auto tex = Ref<Texture2D>::adopt(new Texture2D(width, height, static_cast<uint32_t>(rhi::TextureFormat::BGRA8Unorm)));
    tex->m_isRenderTarget = true;
    tex->m_rhiTexture = std::move(*colorResult);
    tex->m_rhiTextureView = std::move(*colorViewResult);
    tex->m_depthTexture = std::move(*depthResult);
    tex->m_depthView = std::move(*depthViewResult);
    return tex;
}

} // namespace ln
