#pragma once
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <webgpu/webgpu.h>

namespace ln::rhi::webgpu {

class WebGPUDevice;

//------------------------------------------------------------------------------
class WebGPURenderPass final : public RenderPass {
public:
    WebGPURenderPass();

    void init(WGPURenderPassEncoder encoder, const RenderPassLayoutDesc& layoutDesc);

    const RenderPassLayoutDesc& layoutDesc() const override { return m_layoutDesc; }
    void setPipeline(RenderPipeline* pipeline) override;
    void setVertexBuffer(u32 slot, Buffer* buffer, u64 offset) override;
    void setIndexBuffer(Buffer* buffer, IndexFormat format, u64 offset) override;
    void setBindGroup(u32 index, BindGroup* group) override;
    void setBindGroup(u32 index, BindGroup* group,
                      const u32* dynamicOffsets, u32 dynamicOffsetCount) override;
    void setViewport(f32 x, f32 y, f32 w, f32 h, f32 minDepth, f32 maxDepth) override;
    void setScissorRect(u32 x, u32 y, u32 w, u32 h) override;
    void setStencilReference(u32 reference) override;
    void draw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance) override;
    void drawIndexed(u32 indexCount, u32 instanceCount, u32 firstIndex, i32 baseVertex, u32 firstInstance) override;
    void end() override;

protected:
    void finalize() override;

private:
    WGPURenderPassEncoder m_encoder = nullptr;
    RenderPassLayoutDesc m_layoutDesc;
};

//------------------------------------------------------------------------------
class WebGPUCommandBuffer final : public CommandBuffer {
public:
    WebGPUCommandBuffer();

    VoidResult init(WebGPUDevice* device);
    VoidResult begin();
    RenderPass* beginRenderPass(const RenderPassDesc& desc) override;
    void submit() override;

protected:
    void finalize() override;

private:
    WebGPUDevice* m_device = nullptr;
    WGPUCommandEncoder m_encoder = nullptr;
    Ref<WebGPURenderPass> m_currentRenderPass;
};

} // namespace ln::rhi::webgpu
