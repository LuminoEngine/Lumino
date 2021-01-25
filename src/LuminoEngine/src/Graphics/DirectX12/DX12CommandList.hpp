
#pragma once
#include "DX12Helper.hpp"

namespace ln {
namespace detail {
	
class DX12GraphicsContext
	: public ICommandList
{
public:
    enum State
    {
        Recording,
        Executing,
        Finished,
    };

	DX12GraphicsContext();
	bool init(DX12Device* device);
	void dispose();
    void submit(UINT64 fenceValue);

protected:
    void onSaveExternalRenderState() override;
    void onRestoreExternalRenderState() override;
    void onBeginCommandRecoding() override;
    void onEndCommandRecoding() override;
    void onBeginRenderPass(IRenderPass* renderPass) override;
    void onEndRenderPass(IRenderPass* renderPass) override;
    void onSubmitStatus(const GraphicsContextState& state, uint32_t stateDirtyFlags, GraphicsContextSubmitSource submitSource, IPipeline* pipeline) override;
    void* onMapResource(IGraphicsRHIBuffer* resource, uint32_t offset, uint32_t size) override;
    void onUnmapResource(IGraphicsRHIBuffer* resource) override;
    void onSetSubData(IGraphicsRHIBuffer* resource, size_t offset, const void* data, size_t length) override;
    void onSetSubData2D(ITexture* resource, int x, int y, int width, int height, const void* data, size_t dataSize) override;
    void onSetSubData3D(ITexture* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override;
    void onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) override;
    void onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount) override;
    void onDrawPrimitiveIndexed(PrimitiveTopology primitive, int startIndex, int primitiveCount, int instanceCount, int vertexOffset) override;
    void onDrawExtension(INativeGraphicsExtension* extension) override;
    void wait() override;


private:
	DX12Device* m_device;
    ComPtr<ID3D12CommandAllocator> m_dxCommandAllocator;
    ComPtr<ID3D12GraphicsCommandList> m_dxCommandList;
    ComPtr<ID3D12Fence> m_fence;
    HANDLE m_event;
    State m_state;
    UINT64 m_waitingFenceValue;

};

} // namespace detail
} // namespace ln
