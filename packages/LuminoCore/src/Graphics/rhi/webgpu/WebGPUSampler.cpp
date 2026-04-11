#include "WebGPUSampler.hpp"
#include "WebGPUDevice.hpp"
#include "WebGPUHelpers.hpp"

namespace ln::rhi::webgpu {

WebGPUSampler::WebGPUSampler() = default;

VoidResult WebGPUSampler::init(WebGPUDevice* device, const SamplerDesc& desc) {
    m_device = device;

    WGPUSamplerDescriptor samplerDesc = WGPU_SAMPLER_DESCRIPTOR_INIT;
    samplerDesc.label = {"LuminoSampler", WGPU_STRLEN};
    samplerDesc.addressModeU = toWGPUAddressMode(desc.addressU);
    samplerDesc.addressModeV = toWGPUAddressMode(desc.addressV);
    samplerDesc.addressModeW = toWGPUAddressMode(desc.addressW);
    samplerDesc.magFilter = toWGPUFilterMode(desc.magFilter);
    samplerDesc.minFilter = toWGPUFilterMode(desc.minFilter);
    samplerDesc.mipmapFilter = toWGPUMipmapFilterMode(desc.minFilter);
    samplerDesc.lodMinClamp = 0.0f;
    samplerDesc.lodMaxClamp = 1.0f;
    samplerDesc.compare = WGPUCompareFunction_Undefined;
    samplerDesc.maxAnisotropy = static_cast<uint16_t>(desc.maxAnisotropy > 1 ? desc.maxAnisotropy : 1);

    m_sampler = wgpuDeviceCreateSampler(m_device->wgpuDevice(), &samplerDesc);
    if (!m_sampler) {
        return LN_MAKE_ERROR("wgpuDeviceCreateSampler failed.");
    }
    return LN_MAKE_SUCCESS();
}

void WebGPUSampler::finalize() {
    if (m_sampler) {
        wgpuSamplerRelease(m_sampler);
        m_sampler = nullptr;
    }
    Sampler::finalize();
}

} // namespace ln::rhi::webgpu
