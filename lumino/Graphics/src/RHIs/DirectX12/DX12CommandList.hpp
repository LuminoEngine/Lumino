
#pragma once
#include "DX12Helper.hpp"
#include "DX12SingleFrameAllocator.hpp"

namespace ln {
namespace detail {
	
class DX12GraphicsContext
	: public ICommandList
{
public:
    enum State
    {
        Initial,
        Recording,
        Executing,
        Finished,
    };

	DX12GraphicsContext();
	bool init(DX12Device* device);
	void dispose();
    void submit(UINT64 fenceValue);
    ID3D12GraphicsCommandList* dxCommandList() const { return m_dxCommandList.Get(); }

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

    // DirectX の Sample では、実行中に RenderTarget が破棄されることが無いため
    // グローバルに固定長な1つの DescriptorHeap を作ってそれを使っている。
    // RenderTarget の破棄を考慮する場合はそれ用のメモリ管理を作る必要があるのだがそれはそれで大変。
    // なので、CommandList 構築中に RTV, DSV が必要になるたびに View を作り、
    // 終わったら (というより次回開始時) 全てリセットするような、Shader 用の Allocator を再利用してみる。
    Ref<DX12DescriptorHeapAllocator> m_descriptorHeapAllocator_RTV;
    Ref<DX12DescriptorHeapAllocator> m_descriptorHeapAllocator_DSV;
    int32_t m_currentRTVCount;
    std::array<D3D12_CPU_DESCRIPTOR_HANDLE, MaxMultiRenderTargets> m_currentRTVHandles;
    D3D12_CPU_DESCRIPTOR_HANDLE m_currentDSVHandle;

    Ref<DX12SingleFrameAllocator> m_uploadBufferAllocator;
};

} // namespace detail
} // namespace ln
