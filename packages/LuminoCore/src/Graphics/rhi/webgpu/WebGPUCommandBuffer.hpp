#pragma once
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <webgpu/webgpu.h>
#include <functional>
#include <unordered_map>
#include <vector>

namespace ln::rhi::webgpu {

class WebGPUDevice;

//------------------------------------------------------------------------------
// Key for caching WebGPURenderPass objects. Only pipeline-compatibility fields
// belong here (formats + sample count); load/store ops and clear values don't
// affect pipeline compatibility.
struct WebGPURenderPassLayoutKey {
    std::vector<TextureFormat> colorFormats;
    TextureFormat depthStencilFormat = TextureFormat::Undefined;
    u32 sampleCount = 1;

    bool operator==(const WebGPURenderPassLayoutKey& other) const {
        return depthStencilFormat == other.depthStencilFormat
            && sampleCount == other.sampleCount
            && colorFormats == other.colorFormats;
    }
};

struct WebGPURenderPassLayoutKeyHash {
    size_t operator()(const WebGPURenderPassLayoutKey& k) const {
        size_t h = std::hash<u32>{}(k.sampleCount);
        h ^= std::hash<int>{}(static_cast<int>(k.depthStencilFormat))
             + 0x9e3779b9 + (h << 6) + (h >> 2);
        for (auto f : k.colorFormats) {
            h ^= std::hash<int>{}(static_cast<int>(f))
                 + 0x9e3779b9 + (h << 6) + (h >> 2);
        }
        return h;
    }
};

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
    // Cache of WebGPURenderPass wrappers keyed by attachment layout. The
    // wrapper identity must remain stable across frames so that downstream
    // PipelineCache (which hashes by rhi::RenderPass*) can hit consistently.
    std::unordered_map<WebGPURenderPassLayoutKey, Ref<WebGPURenderPass>, WebGPURenderPassLayoutKeyHash> m_renderPassCache;
};

} // namespace ln::rhi::webgpu
