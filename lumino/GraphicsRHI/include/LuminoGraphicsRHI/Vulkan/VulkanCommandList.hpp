
#pragma once
#include "VulkanHelper.hpp"

namespace ln {
namespace detail {

// ひとつのコマンドバッファ。通常、記録中バッファと実行中バッファなどに分かれるため、インスタンスは複数作られる。
// VkCommandBuffer のほか、動的な各種バッファの変更などで必要となるメモリプールの管理も行う。
class VulkanCommandBuffer : public URefObject {
public:
    VulkanCommandBuffer();
    ~VulkanCommandBuffer();
    Result<> init(VulkanDevice* deviceContext);
    void dispose();

    VkCommandBuffer vulkanCommandBuffer() const { return m_commandBuffer; }

    Result<> submit(VkSemaphore waitSemaphore, VkSemaphore signalSemaphore);

    const Ref<VulkanSingleFrameAllocator>& transferBufferSingleFrameAllocator() const { return m_transferBufferSingleFrameAllocator; }

    // データを destination へ送信するためのコマンドを push する。
    // 元データは戻り値のメモリ領域に書き込むこと。
    VulkanSingleFrameBufferInfo cmdCopyBuffer(size_t size, VulkanBuffer* destination);
    VulkanSingleFrameBufferInfo cmdCopyBufferToImage(size_t size, int width, int height, VulkanImage* destination);

public:
    VulkanDevice* m_deviceContext;
    VkCommandBuffer m_commandBuffer;
    VkFence m_inFlightFence;
    Ref<VulkanSingleFrameAllocator> m_transferBufferSingleFrameAllocator;
};

class VulkanGraphicsContext : public ICommandList
{
public:
	VulkanGraphicsContext();
    bool init(VulkanDevice* owner);
	void onDestroy() override;

    VulkanCommandBuffer* recodingCommandBuffer() const { return m_commandBuffer; }

    void wait() override;

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
    URef<VulkanCommandBuffer> m_commandBuffer;
};

} // namespace detail
} // namespace ln
