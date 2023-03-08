
#pragma once
#include "VulkanHelper.hpp"

namespace ln {
namespace detail {

class VulkanGraphicsContext : public ICommandList {
public:
    VulkanGraphicsContext();
    Result<> init(VulkanDevice* owner);
    void onDestroy() override;

    void wait() override;

    VkCommandBuffer vulkanCommandBuffer() const { return m_commandBuffer; }
    Result<> submit(VkSemaphore waitSemaphore, VkSemaphore signalSemaphore);

    // データを destination へ送信するためのコマンドを push する。
    // 元データは戻り値のメモリ領域に書き込むこと。
    VulkanSingleFrameBufferInfo cmdCopyBuffer(size_t size, VulkanBuffer* destination);
    VulkanSingleFrameBufferInfo cmdCopyBufferToImage(size_t size, int width, int height, VulkanImage* destination);

protected:
    void onSaveExternalRenderState() override;
    void onRestoreExternalRenderState() override;
    void onBeginCommandRecoding() override;
    void onEndCommandRecoding() override;
    void onBeginRenderPass(IRenderPass* renderPass) override;
    void onEndRenderPass(IRenderPass* renderPass) override;
    void onSubmitStatus(const GraphicsContextState& state, uint32_t stateDirtyFlags, GraphicsContextSubmitSource submitSource, IPipeline* pipeline) override;
    void onSetSubData(RHIResource* resource, size_t offset, const void* data, size_t length) override;
    void onSetSubData2D(RHIResource* resource, int x, int y, int width, int height, const void* data, size_t dataSize) override;
    void onSetSubData3D(RHIResource* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override;
    void onDispatch(const GraphicsContextState& state, IPipeline* pipeline, int groupCountX, int groupCountY, int groupCountZ) override;
    void onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) override;
    void onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount, int instanceCount) override;
    void onDrawPrimitiveIndexed(PrimitiveTopology primitive, int startIndex, int primitiveCount, int instanceCount, int vertexOffset) override;
    void onDrawExtension(INativeGraphicsExtension* extension) override;

private:
    VulkanDevice* m_device;
    VkCommandBuffer m_commandBuffer;
    VkFence m_inFlightFence;
    Ref<VulkanSingleFrameAllocator> m_transferBufferSingleFrameAllocator;
};

} // namespace detail
} // namespace ln
