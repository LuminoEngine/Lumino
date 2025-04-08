#pragma once
#include "Common.hpp"
#include "WebGPUTextureBase.hpp"

namespace ln {
namespace detail {

class WebGPUTexture2D : public WebGPUTextureBase {
public:
    WebGPUTexture2D();
    MaybeResult_deprecated init(
        WebGPUDevice* device, GraphicsResourceUsage usage, uint32_t width, uint32_t height,
        TextureFormat requestFormat, bool mipmap, const void* initialData);
    void onDestroy() override;

    WGPUTexture nativeTexture() const override { return m_nativeTexture; }
    WGPUTextureView nativeTextureView() const override { return m_nativeTextureView; }

private:
    WebGPUDevice* m_device;
    WGPUTexture m_nativeTexture;
    WGPUTextureView m_nativeTextureView;
};

} // namespace detail
} // namespace ln
