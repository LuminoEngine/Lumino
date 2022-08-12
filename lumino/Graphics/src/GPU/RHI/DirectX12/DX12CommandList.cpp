
#include "Internal.hpp"
#include "DX12DeviceContext.hpp"
#include "DX12Buffers.hpp"
#include "DX12ShaderPass.hpp"
#include "DX12RenderPass.hpp"
#include "DX12DescriptorPool.hpp"
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


    m_descriptorHeapAllocator_RTV = makeRef<DX12DescriptorHeapAllocator>();
    if (!m_descriptorHeapAllocator_RTV->init(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 128)) {
        return false;
    }

    m_descriptorHeapAllocator_DSV = makeRef<DX12DescriptorHeapAllocator>();
    if (!m_descriptorHeapAllocator_DSV->init(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 128)) {
        return false;
    }

    m_state = State::Initial;

    m_uploadBufferAllocator = makeRef<DX12SingleFrameAllocator>(m_device->uploadBufferAllocatorManager());

	return true;
}

void DX12GraphicsContext::dispose()
{
    if (m_descriptorHeapAllocator_RTV) {
        m_descriptorHeapAllocator_RTV->dispose();
        m_descriptorHeapAllocator_RTV = nullptr;
    }
    if (m_descriptorHeapAllocator_DSV) {
        m_descriptorHeapAllocator_DSV->dispose();
        m_descriptorHeapAllocator_DSV = nullptr;
    }
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
    //printf("-----\n");
    m_uploadBufferAllocator->cleanup();

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

void DX12GraphicsContext::onBeginRenderPass(IRenderPass* baseRenderPass)
{
    DX12RenderPass* renderPass = static_cast<DX12RenderPass*>(baseRenderPass);
    ID3D12Device* dxDevice = m_device->device();

    int32_t renderTargetCount = renderPass->getAvailableRenderTargetCount();
    DX12DepthBuffer* depthBuffer = renderPass->depthBuffer();

    // CreateRenderTargetView, ResourceBarrior
    {
        m_currentRTVCount = renderTargetCount;
        if (renderTargetCount > 0) {
            DX12DescriptorHandles rtvHandles;
            if (!m_descriptorHeapAllocator_RTV->allocate(renderTargetCount, &rtvHandles)) {
                return;
            }

            for (int32_t i = 0; i < renderTargetCount; i++) {
                DX12RenderTarget* renderTarget = renderPass->renderTarget(i);
                DX12Image* image;
                D3D12_RENDER_TARGET_VIEW_DESC desc = {};
                desc.Format = renderTarget->dxFormat();
                if (renderPass->isMultisample() && renderTarget->isMultisample()) {
                    desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMS;
                    image = renderTarget->multisampleBuffer();
                }
                else {
                    desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
                    image = renderTarget->image();
                }

                D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = rtvHandles.cpuHandles[i];
                dxDevice->CreateRenderTargetView(image->dxResource(), nullptr, cpuHandle);
                m_currentRTVHandles[i] = cpuHandle;

                image->resourceBarrior(m_dxCommandList.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET);
            }
        }

        if (depthBuffer) {
            DX12DescriptorHandles dsvHandles;
            if (!m_descriptorHeapAllocator_DSV->allocate(1, &dsvHandles)) {
                return;
            }

            DX12Image* image;
            D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
            desc.Format = depthBuffer->dxFormat();
            if (renderPass->isMultisample() && depthBuffer->isMultisample()) {
                desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;
                image = depthBuffer->multisampleBuffer();
            }
            else {
                desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
                image = depthBuffer->image();
            }

            D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = dsvHandles.cpuHandles[0];
            dxDevice->CreateDepthStencilView(image->dxResource(), &desc, cpuHandle);
            m_currentDSVHandle = cpuHandle;

            image->resourceBarrior(m_dxCommandList.Get(), D3D12_RESOURCE_STATE_DEPTH_WRITE);
        }
        else {
            m_currentDSVHandle.ptr = 0;
        }
    }

    m_dxCommandList->OMSetRenderTargets(
        renderTargetCount,
        (renderTargetCount > 0) ? m_currentRTVHandles.data() : nullptr,
        FALSE,
        (depthBuffer) ? &m_currentDSVHandle : nullptr);

    // Clear
    {
        const ClearFlags clearFlags = renderPass->clearFlags();
        const Color& color = renderPass->clearColor();
        onClearBuffers(clearFlags, color, renderPass->clearDepth(), renderPass->clearStencil());
    }
}

void DX12GraphicsContext::onEndRenderPass(IRenderPass* baseRenderPass)
{
    DX12RenderPass* renderPass = static_cast<DX12RenderPass*>(baseRenderPass);

    for (int i = 0; i < m_currentRTVCount; i++) {
        DX12RenderTarget* renderTarget = renderPass->renderTarget(i);

        // Resolve MSAA
        if (renderTarget->isMultisample()) {
            DX12Image* src = renderTarget->multisampleBuffer();
            DX12Image* dst = renderTarget->image();
            src->resourceBarrior(m_dxCommandList.Get(), D3D12_RESOURCE_STATE_RESOLVE_SOURCE);
            dst->resourceBarrior(m_dxCommandList.Get(), D3D12_RESOURCE_STATE_RESOLVE_DEST);
            m_dxCommandList->ResolveSubresource(dst->dxResource(), 0, src->dxResource(), 0, dst->dxFormat());
        }

        renderTarget->image()->resourceBarrior(m_dxCommandList.Get(), D3D12_RESOURCE_STATE_GENERIC_READ);
    }

    DX12DepthBuffer* depthBuffer = renderPass->depthBuffer();
    if (depthBuffer) {

        // Resolve MSAA
#if 0
        if (depthBuffer->isMultisample()) {
            DX12Image* src = depthBuffer->multisampleBuffer();
            DX12Image* dst = depthBuffer->image();
            src->resourceBarrior(m_dxCommandList.Get(), D3D12_RESOURCE_STATE_RESOLVE_SOURCE);
            dst->resourceBarrior(m_dxCommandList.Get(), D3D12_RESOURCE_STATE_RESOLVE_DEST);
            m_dxCommandList->ResolveSubresource(dst->dxResource(), 0, src->dxResource(), 0, dst->dxFormat());
        }
#endif

        depthBuffer->image()->resourceBarrior(m_dxCommandList.Get(), D3D12_RESOURCE_STATE_DEPTH_READ);
    }
}

void DX12GraphicsContext::onSubmitStatus(const GraphicsContextState& state, uint32_t stateDirtyFlags, GraphicsContextSubmitSource submitSource, IPipeline* basePipeline)
{
    // Viewport & Scissor
    if (stateDirtyFlags & GraphicsContextStateDirtyFlags_RegionRects) {
        D3D12_VIEWPORT viewport;
        viewport.TopLeftX = static_cast<float>(state.regionRects.viewportRect.x);
        viewport.TopLeftY = static_cast<float>(state.regionRects.viewportRect.y);
        viewport.Width = static_cast<float>(state.regionRects.viewportRect.width);
        viewport.Height = static_cast<float>(state.regionRects.viewportRect.height);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        m_dxCommandList->RSSetViewports(1, &viewport);

        D3D12_RECT scissor;
        scissor.left = state.regionRects.scissorRect.getLeft();
        scissor.top = state.regionRects.scissorRect.getTop();
        scissor.right = state.regionRects.scissorRect.getRight();
        scissor.bottom = state.regionRects.scissorRect.getBottom();
        m_dxCommandList->RSSetScissorRects(1, &scissor);
    }


    if (submitSource == GraphicsContextSubmitSource_Draw) {
        DX12ShaderPass* shaderPass = static_cast<DX12ShaderPass*>(state.shaderPass);
        
        m_dxCommandList->SetGraphicsRootSignature(shaderPass->rootSignature());

        // Pipeline
        if (basePipeline) {
            DX12Pipeline* pipeline = static_cast<DX12Pipeline*>(basePipeline);
            m_dxCommandList->SetPipelineState(pipeline->dxPipelineState());
            m_dxCommandList->OMSetStencilRef(state.pipelineState.depthStencilState.stencilReferenceValue);
        }

        // VertexBuffer
        {
            DX12VertexDeclaration* vertexLayout = static_cast<DX12VertexDeclaration*>(state.pipelineState.vertexDeclaration);
            std::array<D3D12_VERTEX_BUFFER_VIEW, MaxVertexStreams> vertexBufferViews;
            int vbCount = 0;
            for (int i = 0; i < state.primitive.vertexBuffers.size(); i++) {
                DX12VertexBuffer* vertexBuffer = static_cast<DX12VertexBuffer*>(state.primitive.vertexBuffers[i]);
                if (vertexBuffer) {
                    vertexBufferViews[i].BufferLocation = vertexBuffer->dxResource()->GetGPUVirtualAddress();
                    vertexBufferViews[i].StrideInBytes = vertexLayout->stride(i);
                    vertexBufferViews[i].SizeInBytes = static_cast<UINT>(vertexBuffer->memorySize());
                    vbCount++;
                }
            }
            m_dxCommandList->IASetVertexBuffers(0, vbCount, vertexBufferViews.data());
        }

        // IndexBuffer
        if (state.primitive.indexBuffer) {
            DX12IndexBuffer* indexBuffer = static_cast<DX12IndexBuffer*>(state.primitive.indexBuffer);
            D3D12_INDEX_BUFFER_VIEW indexView;
            indexView.BufferLocation = indexBuffer->dxResource()->GetGPUVirtualAddress();
            indexView.SizeInBytes = static_cast<UINT>(indexBuffer->memorySize());
            indexView.Format = indexBuffer->indexFormat();
            m_dxCommandList->IASetIndexBuffer(&indexView);
        }

        // Descriptor
        if (state.descriptor) {
            DX12Descriptor* descriptor = static_cast<DX12Descriptor*>(state.descriptor);
            descriptor->bind(this);
        }
    }
}

void DX12GraphicsContext::onSetSubData(RHIResource* baseResource, size_t offset, const void* data, size_t length)
{
    // UPLOAD Buffer を使った動的なリソース更新は D3D12HDR.cpp が参考になる。

    DX12Buffer* buffer;
    D3D12_RESOURCE_STATES afterStatus;
    GraphicsResourceUsage usage;
    switch (baseResource->resourceType())
    {
    case RHIResourceType::VertexBuffer:
        usage = static_cast<DX12VertexBuffer*>(baseResource)->m_usage;
        if (LN_REQUIRE(usage == GraphicsResourceUsage::Static)) return;
        buffer = static_cast<DX12VertexBuffer*>(baseResource)->buffer().get();
        afterStatus = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
        break;
    case RHIResourceType::IndexBuffer:
        usage = static_cast<DX12IndexBuffer*>(baseResource)->m_usage;
        if (LN_REQUIRE(usage == GraphicsResourceUsage::Static)) return;
        buffer = static_cast<DX12IndexBuffer*>(baseResource)->buffer().get();
        afterStatus = D3D12_RESOURCE_STATE_INDEX_BUFFER;
        break;
    default:
        LN_NOTIMPLEMENTED();
        break;
    }


    //D3D12_RANGE readRange = { 0, 0 };
    ////D3D12_RANGE range;
    ////range.Begin = view.offset;
    ////range.End = view.offset + length;
    //void* mapped;
    //if (FAILED(uploadBuffer->Map(0, &readRange, &mapped))) {
    //    LN_ERROR("Map failed.");
    //    return;
    //}
    //memcpy(static_cast<uint8_t*>(mapped) + view.offset, data, length);
    //D3D12_RANGE writtenRange = { view.offset, view.offset + length };
    //uploadBuffer->Unmap(0, &writtenRange);




    DX12SingleFrameBufferView view = m_uploadBufferAllocator->allocate(length, DX12Helper::Alignment);
    ID3D12Resource* uploadBuffer = view.buffer->dxResource();
    //std::cout << "onSetSubData " << view.offset << std::endl;

    //D3D12_RANGE range;
    //range.Begin = view.offset;
    //range.End = view.offset + length;
    D3D12_RANGE readRange = { 0, 0 };
    void* mapped;
    if (FAILED(uploadBuffer->Map(0, &readRange, &mapped))) {
        LN_ERROR("Map failed.");
        return;
    }
    //memcpy(mapped, data, length);
    memcpy(static_cast<uint8_t*>(mapped) + view.offset, data, length);
    D3D12_RANGE writtenRange = { view.offset, view.offset + length };
    uploadBuffer->Unmap(0, &writtenRange);
    //uploadBuffer->Unmap(0, nullptr);

    buffer->resourceBarrior(m_dxCommandList.Get(), D3D12_RESOURCE_STATE_COPY_DEST);
    m_dxCommandList->CopyBufferRegion(buffer->dxResource(), offset, uploadBuffer, view.offset, length);
    buffer->resourceBarrior(m_dxCommandList.Get(), afterStatus);
}

void DX12GraphicsContext::onSetSubData2D(RHIResource* resource, int x, int y, int width, int height, const void* data, size_t dataSize)
{
    ID3D12Device* dxDevice = m_device->device();
    DX12Texture2D* texture = static_cast<DX12Texture2D*>(resource);

    D3D12_RESOURCE_DESC srcDesc;
    srcDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    srcDesc.Alignment = 0;
    srcDesc.Width = width;
    srcDesc.Height = height;
    srcDesc.DepthOrArraySize = 1;
    srcDesc.MipLevels = 1;
    srcDesc.Format = texture->dxFormat();
    srcDesc.SampleDesc.Count = 1;
    srcDesc.SampleDesc.Quality = 0;
    srcDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    srcDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
    UINT numRows;
    UINT64 rowSizeInBytes;
    UINT64 totalBytes;
    dxDevice->GetCopyableFootprints(&srcDesc, 0, 1, 0, &footprint, &numRows, &rowSizeInBytes, &totalBytes);

    DX12SingleFrameBufferView view = m_uploadBufferAllocator->allocate(totalBytes, D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT);
    ID3D12Resource* uploadBuffer = view.buffer->dxResource();

    D3D12_RANGE readRange = { 0, 0 };
    void* mapped;
    if (FAILED(uploadBuffer->Map(0, &readRange, &mapped))) {
        LN_ERROR("Map failed.");
        return;
    }

    int32_t pixelSize = DX12Helper::getFormatSize(srcDesc.Format);
    RHIBitmap bmp1, bmp2;
    bmp1.initWrap(data, pixelSize, width, height);
    bmp2.initWritableWrap(static_cast<uint8_t*>(mapped) + view.offset, pixelSize, footprint.Footprint.RowPitch / pixelSize, footprint.Footprint.Height);
    bmp2.blit(&bmp1);

    D3D12_RANGE writtenRange = { view.offset, view.offset + totalBytes };
    uploadBuffer->Unmap(0, &writtenRange);

    D3D12_TEXTURE_COPY_LOCATION dst;
    dst.pResource = texture->dxResource();
    dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    dst.SubresourceIndex = 0;

    D3D12_TEXTURE_COPY_LOCATION src;
    src.pResource = uploadBuffer;
    src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    src.PlacedFootprint = footprint;
    src.PlacedFootprint.Offset = view.offset;

    texture->image()->resourceBarrior(m_dxCommandList.Get(), D3D12_RESOURCE_STATE_COPY_DEST);
    m_dxCommandList->CopyTextureRegion(&dst, x, y, 0, &src, nullptr);
    texture->image()->resourceBarrior(m_dxCommandList.Get(), D3D12_RESOURCE_STATE_GENERIC_READ);
}

void DX12GraphicsContext::onSetSubData3D(RHIResource* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize)
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::onDispatch(const GraphicsContextState& state, IPipeline* basePipeline, int groupCountX, int groupCountY, int groupCountZ)
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::onClearBuffers(ClearFlags clearFlags, const Color& color, float z, uint8_t stencil)
{
    if (clearFlags & ClearFlags::Color) {
        if (m_currentRTVCount > 0) {
            float c[4] = { color.r, color.g, color.b, color.a };
            for (int i = 0; i < m_currentRTVCount; i++) {
                m_dxCommandList->ClearRenderTargetView(m_currentRTVHandles[i], c, 0, nullptr);
            }
        }
    }

    if ((clearFlags & ClearFlags::Depth) || (clearFlags & ClearFlags::Stencil)) {
        if (m_currentDSVHandle.ptr) {
            D3D12_CLEAR_FLAGS flags =
                ((clearFlags & ClearFlags::Depth) ? D3D12_CLEAR_FLAG_DEPTH : (D3D12_CLEAR_FLAGS)0) |
                ((clearFlags & ClearFlags::Stencil) ? D3D12_CLEAR_FLAG_STENCIL : (D3D12_CLEAR_FLAGS)0);
            m_dxCommandList->ClearDepthStencilView(m_currentDSVHandle, flags, z, stencil, 0, nullptr);
        }
    }
}

void DX12GraphicsContext::onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount, int instanceCount) {
    D3D_PRIMITIVE_TOPOLOGY topology;
    UINT vertexCount;
    if (!DX12Helper::getDrawPrimitiveData(primitive, primitiveCount, &topology, &vertexCount)) {
        return;
    }

    m_dxCommandList->IASetPrimitiveTopology(topology);
    m_dxCommandList->DrawInstanced(vertexCount, std::max(instanceCount, 1), startVertex, 0);
}

void DX12GraphicsContext::onDrawPrimitiveIndexed(PrimitiveTopology primitive, int startIndex, int primitiveCount, int instanceCount, int vertexOffset)
{
    D3D_PRIMITIVE_TOPOLOGY topology;
    UINT vertexCount;
    if (!DX12Helper::getDrawPrimitiveData(primitive, primitiveCount, &topology, &vertexCount)) {
        return;
    }

    m_dxCommandList->IASetPrimitiveTopology(topology);
    m_dxCommandList->DrawIndexedInstanced(vertexCount, (instanceCount == 0) ? 1 : instanceCount, startIndex, vertexOffset, 0);
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

    m_state = State::Finished;
}

} // namespace detail
} // namespace ln
