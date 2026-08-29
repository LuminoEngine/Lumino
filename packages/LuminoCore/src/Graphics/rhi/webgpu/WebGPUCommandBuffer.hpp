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
    void setVertexBuffer(uint32_t slot, Buffer* buffer, uint64_t offset) override;
    void setIndexBuffer(Buffer* buffer, IndexFormat format, uint64_t offset) override;
    void setBindGroup(uint32_t index, BindGroup* group) override;
    void setBindGroup(uint32_t index, BindGroup* group,
                      const uint32_t* dynamicOffsets, uint32_t dynamicOffsetCount) override;
    void setViewport(float x, float y, float w, float h, float minDepth, float maxDepth) override;
    void setScissorRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h) override;
    void setStencilReference(uint32_t reference) override;
    void draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) override;
    void drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t baseVertex, uint32_t firstInstance) override;
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
    // RenderPass ラッパー。1 つを使い回し、パスごとに一時的なエンコーダを結び直す。
    Ref<WebGPURenderPass> m_currentRenderPass;
};

} // namespace ln::rhi::webgpu
