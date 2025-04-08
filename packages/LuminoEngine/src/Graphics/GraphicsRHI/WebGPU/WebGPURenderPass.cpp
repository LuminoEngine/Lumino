#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPURenderTarget.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDepthBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPURenderPass.hpp>

namespace ln {
namespace detail {

//==============================================================================
// WebGPURenderPass

WebGPURenderPass::WebGPURenderPass()
    : m_rhiDevice(nullptr)
    , m_renderPassDesc(WGPU_RENDER_PASS_DESCRIPTOR_INIT)
    , m_colorAttachments({}) {
}

Result_deprecated<> WebGPURenderPass::init(
    WebGPUDevice* rhiDevice,
    const DeviceFramebufferState& buffers,
    ClearFlags clearFlags,
    const Color& clearColor,
    float clearDepth,
    uint8_t clearStencil) {
    m_rhiDevice = rhiDevice;

    for (int i = 0; i < MaxMultiRenderTargets; i++) {
        m_renderTargets[i] = buffers.renderTargets[i];

        WGPURenderPassColorAttachment* attachment = &m_colorAttachments[i];
        attachment->view = nullptr;          // set at resolve();
        attachment->resolveTarget = nullptr; // MSAA で使うらしい？
        attachment->loadOp = WGPULoadOp_Clear;
        attachment->storeOp = WGPUStoreOp_Store;
        attachment->clearValue = WGPUColor{ clearColor.r, clearColor.g, clearColor.b, clearColor.a };
#ifndef WEBGPU_BACKEND_WGPU
        attachment->depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
#endif // NOT WEBGPU_BACKEND_WGPU
    }

    m_depthBuffer = buffers.depthBuffer;

    m_renderPassDesc.nextInChain = nullptr;
    m_renderPassDesc.label = WGPU_STRING_VIEW_INIT;
    m_renderPassDesc.colorAttachmentCount = m_colorAttachments.size();
    m_renderPassDesc.colorAttachments = m_colorAttachments.data();
    m_renderPassDesc.depthStencilAttachment = nullptr;
    m_renderPassDesc.occlusionQuerySet = nullptr;
    m_renderPassDesc.timestampWrites = nullptr;

    m_depthStencilAttachment = WGPU_RENDER_PASS_DEPTH_STENCIL_ATTACHMENT_INIT;
    if (m_depthBuffer) {
        auto* wgpuDepthBuffer = static_cast<WebGPUDepthBuffer*>(m_depthBuffer);
        m_depthStencilAttachment.view = wgpuDepthBuffer->nativeTextureView();
        if (clearFlags & ClearFlags::Depth || clearFlags & ClearFlags::Stencil) {
            m_depthStencilAttachment.depthLoadOp = WGPULoadOp_Clear;
            m_depthStencilAttachment.stencilLoadOp = WGPULoadOp_Clear;
        }
        else {
            m_depthStencilAttachment.depthLoadOp = WGPULoadOp_Load;
            m_depthStencilAttachment.stencilLoadOp = WGPULoadOp_Load;
        }
        m_depthStencilAttachment.depthStoreOp = WGPUStoreOp_Store;
        m_depthStencilAttachment.stencilStoreOp = WGPUStoreOp_Store;
        m_depthStencilAttachment.depthClearValue = clearDepth;
        m_depthStencilAttachment.stencilClearValue = clearStencil;
        m_renderPassDesc.depthStencilAttachment = &m_depthStencilAttachment;
    }

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
