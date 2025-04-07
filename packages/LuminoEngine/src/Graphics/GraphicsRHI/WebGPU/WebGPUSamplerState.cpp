#include <LuminoEngine/Graphics/GraphicsRHI/RHIHelper.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU//WebGPUHelper.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDevice.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUSamplerState.hpp>

namespace ln {
namespace detail {

WGPUFilterMode toWGPUFilterMode(TextureFilterMode value) {
    switch (value) {
        case TextureFilterMode::Point:
            return WGPUFilterMode_Nearest;
        case TextureFilterMode::Linear:
            return WGPUFilterMode_Linear;
        default:
            LN_UNREACHABLE();
            return WGPUFilterMode_Nearest;
    }
}

WGPUAddressMode toWGPUAddressMode(TextureAddressMode value) {
    switch (value) {
        case TextureAddressMode::Repeat:
            return WGPUAddressMode_Repeat;
        case TextureAddressMode::Clamp:
            return WGPUAddressMode_ClampToEdge;
        default:
            LN_UNREACHABLE();
            return WGPUAddressMode_ClampToEdge;
    }
}


WebGPUSamplerState::WebGPUSamplerState()
    : m_device(nullptr)
    , m_nativeSampler(nullptr) {
}

WebGPUSamplerState::~WebGPUSamplerState() {
}

MaybeResult WebGPUSamplerState::init(WebGPUDevice* device, const SamplerStateData& desc) {
    m_device = device;

    WGPUFilterMode filter = toWGPUFilterMode(desc.filter);
    WGPUAddressMode address = toWGPUAddressMode(desc.address);

    WGPUSamplerDescriptor samplerDesc = WGPU_SAMPLER_DESCRIPTOR_INIT;
    samplerDesc.addressModeU = address;
    samplerDesc.addressModeV = address;
    samplerDesc.addressModeW = address;
    samplerDesc.minFilter = filter;
    samplerDesc.magFilter = filter;
    samplerDesc.mipmapFilter = WGPUMipmapFilterMode_Linear;
    samplerDesc.lodMinClamp = 0.0f;
    samplerDesc.lodMaxClamp = 1.0f;
    samplerDesc.compare = WGPUCompareFunction_Undefined;
    samplerDesc.maxAnisotropy = desc.anisotropy ? 8 : 1; // TODO: てきとう
    m_nativeSampler = wgpuDeviceCreateSampler(m_device->wgpuDevice(), &samplerDesc);
    if (!m_nativeSampler) {
        return LN_MAKE_ERROR("wgpuDeviceCreateSampler Failed.");
    }

    return LN_MAKE_SUCCESS();
}

void WebGPUSamplerState::onDestroy() {
    if (m_nativeSampler) {
        wgpuSamplerRelease(m_nativeSampler);
        m_nativeSampler = nullptr;
    }
    m_device = nullptr;
}

} // namespace detail
} // namespace ln
