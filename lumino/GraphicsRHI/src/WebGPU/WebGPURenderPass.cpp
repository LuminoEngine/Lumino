#include <LuminoGraphicsRHI/WebGPU/WebGPURenderTarget.hpp>
#include <LuminoGraphicsRHI/WebGPU/WebGPURenderPass.hpp>

namespace ln {
namespace detail {

//==============================================================================
// WebGPURenderPass

WebGPURenderPass::WebGPURenderPass()
    : m_rhiDevice(nullptr)
    , m_renderPassDesc({})
    , m_colorAttachments({}) {
}

Result<> WebGPURenderPass::init(WebGPUDevice* rhiDevice, const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil) {
    m_rhiDevice = rhiDevice;
	
    for (int i = 0; i < MaxMultiRenderTargets; i++) {
        m_renderTargets[i] = buffers.renderTargets[i];

        WGPURenderPassColorAttachment* attachment = &m_colorAttachments[i];
        attachment->view = nullptr;          // set at resolve();
        attachment->resolveTarget = nullptr; // MSAA で使うらしい？
        attachment->loadOp = WGPULoadOp_Clear;
        attachment->storeOp = WGPUStoreOp_Store;
        attachment->clearValue = WGPUColor{ clearColor.r, clearColor.g, clearColor.b, clearColor.a };
    }

	m_depthBuffer = buffers.depthBuffer;

    m_renderPassDesc.nextInChain = nullptr;
    m_renderPassDesc.label = nullptr;
    m_renderPassDesc.colorAttachmentCount = m_colorAttachments.size();
    m_renderPassDesc.colorAttachments = m_colorAttachments.data();
    m_renderPassDesc.depthStencilAttachment = nullptr;
    m_renderPassDesc.occlusionQuerySet = nullptr;
    m_renderPassDesc.timestampWriteCount = 0;
    m_renderPassDesc.timestampWrites = nullptr;

    return ok();
}

void WebGPURenderPass::onDestroy() {
}

const WGPURenderPassDescriptor* WebGPURenderPass::resolve() {
    for (int i = 0; i < MaxMultiRenderTargets; i++) {
        WGPURenderPassColorAttachment* attachment = &m_colorAttachments[i];
        if (m_renderTargets[i]) {
            attachment->view = static_cast<WebGPURenderTarget*>(m_renderTargets[i])->wgpuTextureView();
        }
        else {
            attachment->view = nullptr;
        }
    }

    //if (m_depthTarget) {
    //    m_depthAttachment.view = static_cast<WebGPUTexture*>(buffers.depthTarget)->m_textureView;
    //}
    //else {
    //    m_depthAttachment.view = nullptr;
    //}

    return &m_renderPassDesc;
}

} // namespace detail
} // namespace ln
