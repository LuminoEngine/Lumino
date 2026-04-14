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
    uint32_t sampleCount = 1;

    bool operator==(const WebGPURenderPassLayoutKey& other) const {
        return depthStencilFormat == other.depthStencilFormat
            && sampleCount == other.sampleCount
            && colorFormats == other.colorFormats;
    }
};

struct WebGPURenderPassLayoutKeyHash {
    size_t operator()(const WebGPURenderPassLayoutKey& k) const {
        size_t h = std::hash<uint32_t>{}(k.sampleCount);
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
    Ref<WebGPURenderPass> m_currentRenderPass;
    // Cache of WebGPURenderPass wrappers keyed by attachment layout. The
    // wrapper identity must remain stable across frames so that downstream
    // PipelineCache (which hashes by rhi::RenderPass*) can hit consistently.
    std::unordered_map<WebGPURenderPassLayoutKey, Ref<WebGPURenderPass>, WebGPURenderPassLayoutKeyHash> m_renderPassCache;
};

} // namespace ln::rhi::webgpu
