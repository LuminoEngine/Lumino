#pragma once
#include "Common.hpp"
#include "WebGPUTextureBase.hpp"

namespace ln {
namespace detail {

class WebGPUSamplerState : public ISamplerState {
public:
    WebGPUSamplerState();
    ~WebGPUSamplerState() override;
    MaybeResult_deprecated init(WebGPUDevice* device, const SamplerStateData& desc);
    void onDestroy() override;

    WGPUSampler nativeSampler() const { return m_nativeSampler; }

private:
    WebGPUDevice* m_device;
    WGPUSampler m_nativeSampler;
};

} // namespace detail
} // namespace ln
