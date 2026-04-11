#include <iostream>
#include "WebGPUCommandBuffer.hpp"
#include "WebGPUDevice.hpp"
#include "WebGPUTextureView.hpp"
#include "WebGPUHelpers.hpp"

namespace ln::rhi::webgpu {

//------------------------------------------------------------------------------
// WebGPURenderPass
//------------------------------------------------------------------------------

WebGPURenderPass::WebGPURenderPass() = default;

void WebGPURenderPass::init(WGPURenderPassEncoder encoder, const RenderPassLayoutDesc& layoutDesc) {
    m_encoder = encoder;
    m_layoutDesc = layoutDesc;
}

void WebGPURenderPass::setPipeline(RenderPipeline*) {
    // Not needed for ClearScreen
}

void WebGPURenderPass::setVertexBuffer(u32, Buffer*, u64) {
    // Not needed for ClearScreen
}

void WebGPURenderPass::setIndexBuffer(Buffer*, IndexFormat, u64) {
    // Not needed for ClearScreen
}

void WebGPURenderPass::setBindGroup(u32, BindGroup*) {
    // Not needed for ClearScreen
}

void WebGPURenderPass::setBindGroup(u32, BindGroup*, const u32*, u32) {
    // Not needed for ClearScreen
}

void WebGPURenderPass::setViewport(f32, f32, f32, f32, f32, f32) {
    // Not needed for ClearScreen
}

void WebGPURenderPass::setScissorRect(u32, u32, u32, u32) {
    // Not needed for ClearScreen
}

void WebGPURenderPass::setStencilReference(u32) {
    // Not needed for ClearScreen
}

void WebGPURenderPass::draw(u32, u32, u32, u32) {
    // Not needed for ClearScreen
}

void WebGPURenderPass::drawIndexed(u32, u32, u32, i32, u32) {
    // Not needed for ClearScreen
}

void WebGPURenderPass::end() {
    if (m_encoder) {
        wgpuRenderPassEncoderEnd(m_encoder);
        wgpuRenderPassEncoderRelease(m_encoder);
        m_encoder = nullptr;
    }
}

void WebGPURenderPass::finalize() {
    if (m_encoder) {
        wgpuRenderPassEncoderRelease(m_encoder);
        m_encoder = nullptr;
    }
    RenderPass::finalize();
}

//------------------------------------------------------------------------------
// WebGPUCommandBuffer
//------------------------------------------------------------------------------

WebGPUCommandBuffer::WebGPUCommandBuffer() = default;

VoidResult WebGPUCommandBuffer::init(WebGPUDevice* device) {
    m_device = device;
    return LN_MAKE_SUCCESS();
}

VoidResult WebGPUCommandBuffer::begin() {
    WGPUCommandEncoderDescriptor desc = WGPU_COMMAND_ENCODER_DESCRIPTOR_INIT;
    m_encoder = wgpuDeviceCreateCommandEncoder(m_device->wgpuDevice(), &desc);
    if (!m_encoder) {
        return LN_MAKE_ERROR("wgpuDeviceCreateCommandEncoder failed.");
    }
    return LN_MAKE_SUCCESS();
}

RenderPass* WebGPUCommandBuffer::beginRenderPass(const RenderPassDesc& desc) {
    // Build WGPURenderPassColorAttachment array
    std::vector<WGPURenderPassColorAttachment> colorAttachments(desc.colorAttachments.size());
    RenderPassLayoutDesc layoutDesc;

    for (size_t i = 0; i < desc.colorAttachments.size(); ++i) {
        auto& src = desc.colorAttachments[i];
        auto* view = static_cast<WebGPUTextureView*>(src.view);

        colorAttachments[i] = WGPU_RENDER_PASS_COLOR_ATTACHMENT_INIT;
        colorAttachments[i].view = view->handle();
        colorAttachments[i].loadOp = toWGPULoadOp(src.loadOp);
        colorAttachments[i].storeOp = toWGPUStoreOp(src.storeOp);
        colorAttachments[i].clearValue = {
            static_cast<double>(src.clearColor.r),
            static_cast<double>(src.clearColor.g),
            static_cast<double>(src.clearColor.b),
            static_cast<double>(src.clearColor.a)
        };

        // Track color format for pipeline compatibility lookup.
        layoutDesc.colorFormats.push_back(fromWGPUTextureFormat(view->format()));
    }

    // Build depth/stencil attachment if present
    WGPURenderPassDepthStencilAttachment depthStencilAttachment = WGPU_RENDER_PASS_DEPTH_STENCIL_ATTACHMENT_INIT;
    bool hasDepthStencil = false;
    if (desc.depthStencilAttachment && desc.depthStencilAttachment->view) {
        hasDepthStencil = true;
        auto* dsView = static_cast<WebGPUTextureView*>(desc.depthStencilAttachment->view);
        depthStencilAttachment.view = dsView->handle();
        layoutDesc.depthStencilFormat = fromWGPUTextureFormat(dsView->format());
        depthStencilAttachment.depthLoadOp = toWGPULoadOp(desc.depthStencilAttachment->depthLoadOp);
        depthStencilAttachment.depthStoreOp = toWGPUStoreOp(desc.depthStencilAttachment->depthStoreOp);
        depthStencilAttachment.depthClearValue = desc.depthStencilAttachment->clearDepth;
        depthStencilAttachment.depthReadOnly = WGPU_FALSE;
        depthStencilAttachment.stencilLoadOp = toWGPULoadOp(desc.depthStencilAttachment->stencilLoadOp);
        depthStencilAttachment.stencilStoreOp = toWGPUStoreOp(desc.depthStencilAttachment->stencilStoreOp);
        depthStencilAttachment.stencilClearValue = desc.depthStencilAttachment->clearStencil;
        depthStencilAttachment.stencilReadOnly = WGPU_FALSE;
    }

    WGPURenderPassDescriptor rpDesc = WGPU_RENDER_PASS_DESCRIPTOR_INIT;
    rpDesc.colorAttachmentCount = colorAttachments.size();
    rpDesc.colorAttachments = colorAttachments.data();
    rpDesc.depthStencilAttachment = hasDepthStencil ? &depthStencilAttachment : nullptr;

    WGPURenderPassEncoder rpEncoder = wgpuCommandEncoderBeginRenderPass(m_encoder, &rpDesc);
    if (!rpEncoder) {
        std::cerr << "[WebGPU] wgpuCommandEncoderBeginRenderPass failed.\n";
        return nullptr;
    }

    m_currentRenderPass = Ref<WebGPURenderPass>::adopt(new WebGPURenderPass());
    m_currentRenderPass->init(rpEncoder, layoutDesc);
    return m_currentRenderPass.get();
}

void WebGPUCommandBuffer::submit() {
    if (!m_encoder) return;

    WGPUCommandBufferDescriptor desc = WGPU_COMMAND_BUFFER_DESCRIPTOR_INIT;
    WGPUCommandBuffer cmdBuf = wgpuCommandEncoderFinish(m_encoder, &desc);
    if (cmdBuf) {
        wgpuQueueSubmit(m_device->wgpuQueue(), 1, &cmdBuf);
        wgpuCommandBufferRelease(cmdBuf);
    }

    wgpuCommandEncoderRelease(m_encoder);
    m_encoder = nullptr;
    m_currentRenderPass = nullptr;
}

void WebGPUCommandBuffer::finalize() {
    m_currentRenderPass = nullptr;
    if (m_encoder) {
        wgpuCommandEncoderRelease(m_encoder);
        m_encoder = nullptr;
    }
    CommandBuffer::finalize();
}

} // namespace ln::rhi::webgpu
