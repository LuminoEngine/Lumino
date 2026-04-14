#include <LuminoBase/Logger.hpp>
#include "WebGPUCommandBuffer.hpp"
#include "WebGPUDevice.hpp"
#include "WebGPUTextureView.hpp"
#include "WebGPUBuffer.hpp"
#include "WebGPUPipelineLayout.hpp"
#include "WebGPURenderPipeline.hpp"
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

void WebGPURenderPass::setPipeline(RenderPipeline* pipeline) {
    auto* wp = static_cast<WebGPURenderPipeline*>(pipeline);
    wgpuRenderPassEncoderSetPipeline(m_encoder, wp->handle());
}

void WebGPURenderPass::setVertexBuffer(uint32_t slot, Buffer* buffer, uint64_t offset) {
    auto* wb = static_cast<WebGPUBuffer*>(buffer);
    wgpuRenderPassEncoderSetVertexBuffer(m_encoder, slot, wb->handle(), offset, WGPU_WHOLE_SIZE);
}

void WebGPURenderPass::setIndexBuffer(Buffer* buffer, IndexFormat format, uint64_t offset) {
    auto* wb = static_cast<WebGPUBuffer*>(buffer);
    wgpuRenderPassEncoderSetIndexBuffer(m_encoder, wb->handle(),
                                        toWGPUIndexFormat(format),
                                        offset, WGPU_WHOLE_SIZE);
}

void WebGPURenderPass::setBindGroup(uint32_t index, BindGroup* group) {
    auto* wg = static_cast<WebGPUBindGroup*>(group);
    wgpuRenderPassEncoderSetBindGroup(m_encoder, index, wg->handle(), 0, nullptr);
}

void WebGPURenderPass::setBindGroup(uint32_t index, BindGroup* group,
                                    const uint32_t* dynamicOffsets, uint32_t dynamicOffsetCount) {
    auto* wg = static_cast<WebGPUBindGroup*>(group);
    wgpuRenderPassEncoderSetBindGroup(m_encoder, index, wg->handle(),
                                      dynamicOffsetCount, dynamicOffsets);
}

void WebGPURenderPass::setViewport(float x, float y, float w, float h, float minDepth, float maxDepth) {
    wgpuRenderPassEncoderSetViewport(m_encoder, x, y, w, h, minDepth, maxDepth);
}

void WebGPURenderPass::setScissorRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    wgpuRenderPassEncoderSetScissorRect(m_encoder, x, y, w, h);
}

void WebGPURenderPass::setStencilReference(uint32_t reference) {
    wgpuRenderPassEncoderSetStencilReference(m_encoder, reference);
}

void WebGPURenderPass::draw(uint32_t vertexCount, uint32_t instanceCount,
                            uint32_t firstVertex, uint32_t firstInstance) {
    wgpuRenderPassEncoderDraw(m_encoder, vertexCount, instanceCount, firstVertex, firstInstance);
}

void WebGPURenderPass::drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex,
                                   int32_t baseVertex, uint32_t firstInstance) {
    wgpuRenderPassEncoderDrawIndexed(m_encoder, indexCount, instanceCount,
                                     firstIndex, baseVertex, firstInstance);
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
        LN_LOG_ERROR("[WebGPU] wgpuCommandEncoderBeginRenderPass failed.");
        return nullptr;
    }

    // Look up (or allocate) a WebGPURenderPass wrapper for this attachment
    // layout. Reusing the same wrapper instance across frames keeps the
    // rhi::RenderPass* pointer - swhich is part of PipelineCacheKey - stable,
    // so the upstream PipelineCache hits instead of recompiling every draw.
    WebGPURenderPassLayoutKey key;
    key.colorFormats = layoutDesc.colorFormats;
    key.depthStencilFormat = layoutDesc.depthStencilFormat;
    key.sampleCount = layoutDesc.sampleCount;

    auto it = m_renderPassCache.find(key);
    if (it == m_renderPassCache.end()) {
        auto rp = Ref<WebGPURenderPass>::adopt(new WebGPURenderPass());
        it = m_renderPassCache.emplace(std::move(key), std::move(rp)).first;
    }
    // Re-bind the ephemeral encoder to the cached wrapper each frame.
    it->second->init(rpEncoder, layoutDesc);
    m_currentRenderPass = it->second;
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
    m_renderPassCache.clear();
    if (m_encoder) {
        wgpuCommandEncoderRelease(m_encoder);
        m_encoder = nullptr;
    }
    CommandBuffer::finalize();
}

} // namespace ln::rhi::webgpu
