#pragma once
#include <semaphore>
#include "Common.hpp"

namespace ln {
namespace detail {

class WebGPUCommandList : public ICommandList {
public:
    WebGPUCommandList();
    Result<> init(WebGPUDevice* rhiDevice);
    void onDestroy() override;

	WGPUCommandEncoder commandEncoder() const { return m_commandEncoder; }

	void onSubmitted();
	void onSubmittedWorkDone();

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
    void onDispatch(const GraphicsContextState& state, IPipeline* basePipeline, int groupCountX, int groupCountY, int groupCountZ) override;
    void onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) override;
    void onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount, int instanceCount) override;
    void onDrawPrimitiveIndexed(PrimitiveTopology primitive, int startIndex, int primitiveCount, int instanceCount, int vertexOffset) override;
    void onDrawExtension(INativeGraphicsExtension* extension) override;
    void wait() override;

private:
    WebGPUDevice* m_rhiDevice;
    WGPUCommandEncoder m_commandEncoder;
    WGPURenderPassEncoder m_renderPassEncoder;
    ConditionEvent m_working;
};

} // namespace detail
} // namespace ln
