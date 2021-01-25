
#include "Internal.hpp"
#include "DX12DeviceContext.hpp"
#include "DX12CommandList.hpp"

namespace ln {
namespace detail {
	
//==============================================================================
// DX12GraphicsContext

DX12GraphicsContext::DX12GraphicsContext()
	: m_device(nullptr)
    , m_waitingFenceValue(0)
{
}

bool DX12GraphicsContext::init(DX12Device* device)
{
	LN_CHECK(device);
    if (!ICommandList::init(device)) return false;
	m_device = device;

    ID3D12Device* dxDevice = m_device->device();

    if (FAILED(dxDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_dxCommandAllocator)))) {
        LN_ERROR("CreateCommandAllocator failed.");
        return false;
    }

    if (FAILED(dxDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_dxCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_dxCommandList)))) {
        LN_ERROR("CreateCommandList failed.");
        return false;
    }
    // CommandList は初期状態が "記録中" になっている。この状態では CommandAllocator::Reset() がエラーを返す。
    // 記録開始時は状態を "記録終了" (非記録状態) にしておく方がコードがシンプルになるので、そうしておく。
    // https://docs.microsoft.com/en-us/windows/win32/api/d3d12/nf-d3d12-id3d12graphicscommandlist-close
    m_dxCommandList->Close();

    if (FAILED(dxDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)))) {
        LN_ERROR("CreateFence failed.");
        return false;
    }

    m_event = ::CreateEvent(0, 0, 0, 0);
    if (!m_event) {
        LN_ERROR("CreateEvent failed.");
        return false;
    }

	return true;
}

void DX12GraphicsContext::dispose()
{
    if (m_event) {
        ::CloseHandle(m_event);
        m_event = nullptr;
    }
    m_fence.Reset();
    m_dxCommandList.Reset();
    m_dxCommandAllocator.Reset();
    ICommandList::dispose();
}

void DX12GraphicsContext::onSaveExternalRenderState()
{
}

void DX12GraphicsContext::onRestoreExternalRenderState()
{
}

void DX12GraphicsContext::onBeginCommandRecoding()
{
    // https://docs.microsoft.com/ja-jp/windows/win32/direct3d12/recording-command-lists-and-bundles
    // 基本的な使い方 (1フレーム内で複数のコマンドリストをマルチスレッドで構築するようなケースではない) の場合、
    // ID3D12CommandAllocator と ID3D12GraphicsCommandList はワンセットと考えてよい。
    // 逆にひとつの Allocator から複数の CommandList を作ったとき、Allocator だけ Reset() してしまうと、
    // 生きている CommandList が使っているメモリが壊れてしまう。
    if (FAILED(m_dxCommandAllocator->Reset())) {
        LN_ERROR("Reset failed.");
        return;
    }
    if (FAILED(m_dxCommandList->Reset(m_dxCommandAllocator.Get(), nullptr))) {
        LN_ERROR("Reset failed.");
        return;
    }
}

void DX12GraphicsContext::onEndCommandRecoding()
{
    if (FAILED(m_dxCommandList->Close())) {
        LN_ERROR("Close failed.");
        return;
    }
}

void DX12GraphicsContext::onBeginRenderPass(IRenderPass* renderPass_)
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::onEndRenderPass(IRenderPass* renderPass)
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::onSubmitStatus(const GraphicsContextState& state, uint32_t stateDirtyFlags, GraphicsContextSubmitSource submitSource, IPipeline* pipeline)
{
    LN_NOTIMPLEMENTED();
}

void* DX12GraphicsContext::onMapResource(IGraphicsRHIBuffer* resource, uint32_t offset, uint32_t size)
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

void DX12GraphicsContext::onUnmapResource(IGraphicsRHIBuffer* resource)
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::onSetSubData(IGraphicsRHIBuffer* resource, size_t offset, const void* data, size_t length)
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::onSetSubData2D(ITexture* resource, int x, int y, int width, int height, const void* data, size_t dataSize)
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::onSetSubData3D(ITexture* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize)
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount)
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::onDrawPrimitiveIndexed(PrimitiveTopology primitive, int startIndex, int primitiveCount, int instanceCount, int vertexOffset)
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::onDrawExtension(INativeGraphicsExtension* extension)
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::submit(UINT64 fenceValue)
{
    ID3D12CommandQueue* dxCommandQueue = m_device->dxCommandQueue();

    // 実行開始
    ID3D12CommandList* commandLists[1] = { m_dxCommandList.Get() };
    dxCommandQueue->ExecuteCommandLists(1, commandLists);

    // TODO: Sample ではここで Present していた。そのほうがよい？

    // Queue に FenceValue 設定コマンドを push する。
    // この時点までの CommandList の実行が終わったあと、あるいは既に終わっている場合、
    // m_fence に fence が値を設定されるようにする。
    if (FAILED(dxCommandQueue->Signal(m_fence.Get(), fenceValue))) {
        LN_ERROR("Signal failed.");
        return;
    }

    // 指定された commandList が、自分の実行を待つときはこの fenceValue に達するまで待ってほしい。
    m_waitingFenceValue = fenceValue;

    m_state = State::Executing;
}

void DX12GraphicsContext::wait()
{
    if (m_state != State::Executing) return;

    // NOTE: https://stackoverflow.com/questions/58539783/how-to-synchronize-cpu-and-gpu-using-fence-in-directx-direct3d-12

    ID3D12CommandQueue* dxCommandQueue = m_device->dxCommandQueue();

    const UINT64 fence = m_waitingFenceValue;


    if (m_fence->GetCompletedValue() < m_waitingFenceValue) {
        if (FAILED(m_fence->SetEventOnCompletion(m_waitingFenceValue, m_event))) {
            LN_ERROR("SetEventOnCompletion failed.");
            return;
        }
        WaitForSingleObject(m_event, INFINITE);
    }

    LN_NOTIMPLEMENTED();
    m_state = State::Finished;
}

} // namespace detail
} // namespace ln
