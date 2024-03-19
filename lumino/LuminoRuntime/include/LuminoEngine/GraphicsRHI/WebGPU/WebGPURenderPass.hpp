#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class WebGPURenderPass : public IRenderPass {
public:
    WebGPURenderPass();
    Result<> init(WebGPUDevice* rhiDevice, const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil);
    void onDestroy() override;

    ClearFlags clearFlags() const { return m_clearFlags; }
    const Color& clearColor() const { return m_clearColor; }
    float clearDepth() const { return m_clearDepth; }
    uint8_t clearStencil() const { return m_clearStencil; }

	const WGPURenderPassDescriptor* resolve();

private:
    WebGPUDevice* m_rhiDevice;
    WGPURenderPassDescriptor m_renderPassDesc;
    std::array<WGPURenderPassColorAttachment, MaxMultiRenderTargets> m_colorAttachments;
	
    ClearFlags m_clearFlags;
    Color m_clearColor;
    float m_clearDepth;
    uint8_t m_clearStencil;
};

} // namespace detail
} // namespace ln
