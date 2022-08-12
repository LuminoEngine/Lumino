
#pragma once
#include "VulkanHelper.hpp"

namespace ln {
namespace detail {
//class VulkanGraphicsContext;
//class VulkanSwapChain;
//class VulkanTexture;
//class VulkanRenderTarget;
//class VulkanDepthBuffer;
//class VulkanShaderDescriptorTable;
//class VulkanSingleFrameAllocatorPageManager;
//class VulkanNativeGraphicsInterface;

class VulkanGraphicsContext
	: public ICommandList
{
public:
	VulkanGraphicsContext();
    bool init(VulkanDevice* owner);
	void dispose();

    const Ref<VulkanCommandBuffer>& recodingCommandBuffer() const { return m_commandBuffer; }
    //void setRecodingCommandBuffer(const Ref<VulkanCommandBuffer>& value) { m_recodingCommandBuffer = value; }

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
    Ref<VulkanCommandBuffer> m_commandBuffer;
};

} // namespace detail
} // namespace ln
