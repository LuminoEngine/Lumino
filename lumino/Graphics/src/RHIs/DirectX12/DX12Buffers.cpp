
#include "Internal.hpp"
#include "DX12DeviceContext.hpp"
#include "DX12Buffers.hpp"

namespace ln {
namespace detail {
	
//==============================================================================
// DX12Buffer

/*
D3D12_HEAP_TYPE_UPLOAD について
----------

- [D3D12_HEAP_TYPE](https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ne-d3d12-d3d12_heap_type)

- [D3D12HelloTriangle.cpp の注意書き](https://github.com/microsoft/DirectX-Graphics-Samples/blob/master/Samples/Desktop/D3D12HelloWorld/src/HelloTriangle/D3D12HelloTriangle.cpp#L210-L213)

- [注意書きに対する質問](https://github.com/microsoft/DirectX-Graphics-Samples/issues/147)

## CPU-write-once, GPU-read-once とは

1つの大きなバッファに対して memcpy による書き込みや DrawXXXX() による読み取りが複数回発生してもよいが、
CPU 側は write する範囲、GPU 側は読み取る範囲が重なってはならない、ということ。（と解釈した）

結論としては今の DX12SingleFrameAllocator (DirectX-Graphics-Samples の LinearAllocator) の考え方で問題ない。

write 側が重なるのは論外としても、read 側が重なるとダメなのは、その時点で CPU までデータを取りに行くから、とのこと。

## ダメなケース

これが響いてくるのは、CPU 側は1度の write でメッシュを作り、コマンドリストでそのメッシュを繰り返し描画する、という場合。

Sprite は特にそうだが Lumino の場合は Box や Sphere 等簡易的なプリミティブの描画・デバッグ描画など、
これらはシーンレンダリングの最初に頂点バッファを動的に構築してから描画を行う。
この頂点バッファは通常の描画の他、シャドウマップの生成など複数の描画パスで使われる。

頂点バッファの構築と描画が 1:1 であれば DEFAULT なバッファにデータを送信するのと差は無いのだが、
複数回描画が発生するとその分だけ GPU 側はデータをプルしようとする。


## よさそうな方針

前述の質問には「CPU-write-once, GPU-read-once にならない場合、常に UPLOAD から DEFAULT へのコピーを実装するのはやりすぎか？」
というのがあるが、回答は「やりすぎではない。ただしあなたのアプリの目的に応じてプロファイリングしてね」とのこと。

実際のところ描画のたびに本当に GPU からのプルが走るのかは、GPU やドライバの実装によるものと考えられる。
少なくとも GPU のキャッシュが無効の場合は上記のように動くのが一般的。

ちなみに DirectX12 には Microsoft によるソフトウェアドライバの実装もあるので、実装は本当に様々。
なので MSDN にはあまり詳しく書かれていないのだろうか？

とりあえず Lumino だと、次のようなのがよさそう。

- 3Dシーンの描画では UPLOAD から DEFAULT へのコピー
- GUI (2D) の描画では UPLOAD だけでもかまわない

でも区別するの大変なので、当面は UPLOAD から DEFAULT へのコピーを基本にしてみる。

## ちなみに

UPLOAD は使い勝手いいので全部 UPLOAD にしてしまえ、は論外。実行速度を考えないサンプルならいいけど…。

3Dモデルでこれをやると悲惨な描画速度になる。


*/
    
DX12Buffer::DX12Buffer()
	: m_dxResource()
	, m_size(0)
	, m_resourceState(D3D12_RESOURCE_STATE_COMMON)
    , m_mapped(false)
{
}

bool DX12Buffer::init(DX12Device* device, size_t size, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES initialState)
{
    m_size = size;
    m_resourceState = initialState;

    D3D12_HEAP_PROPERTIES props;
    props.Type = heapType;
    props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    props.CreationNodeMask = device->creationNodeMask();
    props.VisibleNodeMask = device->visibleNodeMask();

    D3D12_RESOURCE_DESC desc;
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Alignment = 0;
    desc.Width = m_size;
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    HRESULT hr = device->device()->CreateCommittedResource(
        &props,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        m_resourceState,
        nullptr,
        IID_PPV_ARGS(&m_dxResource));
    if (FAILED(hr)) {
        LN_ERROR("CreateCommittedResource failed.");
        return false;
    }

	return true;
}

void DX12Buffer::dispose()
{
    m_dxResource.Reset();
}

void* DX12Buffer::map()
{
    if (LN_REQUIRE(!m_mapped)) return nullptr;

    // Range(0,0) は、データの読み取りを行わないことを明示する。
    // null は読み取りの可能性を示すもので、最適化がされないことがある。
    // https://docs.microsoft.com/en-us/windows/win32/api/d3d12/nf-d3d12-id3d12resource-map
    D3D12_RANGE readRange = { 0, 0 };
    UINT8* data;
    if (FAILED(m_dxResource->Map(0, &readRange, reinterpret_cast<void**>(&data)))) {
        LN_ERROR("Map failed.");
        return nullptr;
    }
    m_mapped = true;
    return data;
}

void DX12Buffer::unmap()
{
    if (LN_REQUIRE(m_mapped)) return;
    m_dxResource->Unmap(0, nullptr);
}

void DX12Buffer::resourceBarrior(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES newState)
{
    if (m_resourceState == newState) {
        return;
    }

    D3D12_RESOURCE_BARRIER barrier;
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = m_dxResource.Get();
    barrier.Transition.StateBefore = m_resourceState;
    barrier.Transition.StateAfter = newState;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    commandList->ResourceBarrier(1, &barrier);

    m_resourceState = newState;
}

//==============================================================================
// DX12VertexBuffer

DX12VertexBuffer::DX12VertexBuffer()
    : m_device(nullptr)
    , m_usage(GraphicsResourceUsage::Static)
    , m_buffer()
{
}

bool DX12VertexBuffer::init(DX12Device* device, GraphicsResourceUsage usage, size_t size, const void* initialData)
{
    if (!RHIResource::initAsVertexBuffer(usage, size)) return false;
    m_device = device;
    m_usage = usage;

    // NOTE: ↓ DirectX-Graphics-Samples, D3D12HelloTriangle.cpp より
    //   Note: using upload heaps to transfer static data like vert buffers is not 
    //   recommended. Every time the GPU needs it, the upload heap will be marshalled 
    //   over. Please read up on Default Heap usage. An upload heap is used here for 
    //   code simplicity and because there are very few verts to actually transfer.
    // Dynamic な時は D3D12_HEAP_TYPE_UPLOAD を使う、でよさそう。

    // 組み合わせはこれが必須
    D3D12_HEAP_TYPE heapType;
    D3D12_RESOURCE_STATES resourceState;
    if (m_usage == GraphicsResourceUsage::Dynamic) {
        heapType = D3D12_HEAP_TYPE_UPLOAD;
        resourceState = D3D12_RESOURCE_STATE_GENERIC_READ;
    }
    else {
        heapType = D3D12_HEAP_TYPE_DEFAULT;
        resourceState = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
    }

    m_buffer = makeRHIRef<DX12Buffer>();
    if (!m_buffer->init(m_device, size, heapType, resourceState)) {
        return false;
    }




    //m_usage = usage;
    //m_size = size;

    //ID3D12Device* dx12Device = m_device->device();

    //D3D12_HEAP_PROPERTIES props;
    //props.Type = heapType;
    //props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    //props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    //props.CreationNodeMask = 1;
    //props.VisibleNodeMask = 1;

    //D3D12_RESOURCE_DESC desc;
    //desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    //desc.Alignment = 0;
    //desc.Width = m_size;
    //desc.Height = 1;
    //desc.DepthOrArraySize = 1;
    //desc.MipLevels = 1;
    //desc.Format = DXGI_FORMAT_UNKNOWN;
    //desc.SampleDesc.Count = 1;
    //desc.SampleDesc.Quality = 0;
    //desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    //desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    //if (FAILED(dx12Device->CreateCommittedResource(
    //    &props,
    //    D3D12_HEAP_FLAG_NONE,
    //    &desc,
    //    resourceState,
    //    nullptr,
    //    IID_PPV_ARGS(&m_vertexBuffer)))) {
    //    LN_ERROR("CreateCommittedResource failed.");
    //    return false;
    //}

    if (initialData) {
        if (m_usage == GraphicsResourceUsage::Dynamic) {
            void* data = m_buffer->map();
            memcpy(data, initialData, size);
            m_buffer->unmap();
        }
        else {
            // D3D12_HEAP_TYPE_DEFAULT を指定して作成した Resource は Map/Unmap できない。
            // 転送用の一時バッファ (uploadBuffer) を作って、それ経由でデータを転送する。

            DX12Buffer uploadBuffer;
            if (!uploadBuffer.init(m_device, size, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ)) {
                return false;
            }
            void* data = uploadBuffer.map();
            memcpy(data, initialData, size);
            uploadBuffer.unmap();

            ID3D12GraphicsCommandList* commandList = m_device->beginSingleTimeCommandList();
            m_buffer->resourceBarrior(commandList, D3D12_RESOURCE_STATE_COPY_DEST);
            commandList->CopyResource(m_buffer->dxResource(), uploadBuffer.dxResource());
            m_buffer->resourceBarrior(commandList, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
            m_device->endSingleTimeCommandList(commandList);
        }
    }

    return true;
}

void DX12VertexBuffer::dispose()
{
    if (m_buffer) {
        m_buffer->dispose();
        m_buffer = nullptr;
    }
    RHIResource::dispose();
}

//==============================================================================
// DX12IndexBuffer

DX12IndexBuffer::DX12IndexBuffer()
    : m_usage(GraphicsResourceUsage::Static)
    , m_indexFormat(DXGI_FORMAT_R16_UINT)
{
}

Result DX12IndexBuffer::init(DX12Device* device, GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData)
{
    if (!RHIResource::initAsIndexBuffer(usage, format, indexCount)) return err();
    m_device = device;
    m_usage = usage;

    // Buffer Size
    if (format == IndexBufferFormat::UInt16) {
        m_indexFormat = DXGI_FORMAT_R16_UINT;
    }
    else {
        m_indexFormat = DXGI_FORMAT_R32_UINT;
    }

    // Types
    D3D12_HEAP_TYPE heapType;
    D3D12_RESOURCE_STATES resourceState;
    if (m_usage == GraphicsResourceUsage::Dynamic) {
        heapType = D3D12_HEAP_TYPE_UPLOAD;
        resourceState = D3D12_RESOURCE_STATE_GENERIC_READ;
    }
    else {
        heapType = D3D12_HEAP_TYPE_DEFAULT;
        resourceState = D3D12_RESOURCE_STATE_INDEX_BUFFER;
    }

    uint64_t size = memorySize();
    m_buffer = makeRHIRef<DX12Buffer>();
    if (!m_buffer->init(m_device, size, heapType, resourceState)) {
        return err();
    }

    if (initialData) {
        if (m_usage == GraphicsResourceUsage::Dynamic) {
            void* data = m_buffer->map();
            memcpy(data, initialData, size);
            m_buffer->unmap();
        }
        else {
            // D3D12_HEAP_TYPE_DEFAULT を指定して作成した Resource は Map/Unmap できない。
            // 転送用の一時バッファ (uploadBuffer) を作って、それ経由でデータを転送する。

            DX12Buffer uploadBuffer;
            if (!uploadBuffer.init(m_device, size, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ)) {
                return err();
            }
            void* data = uploadBuffer.map();
            memcpy(data, initialData, size);
            uploadBuffer.unmap();

            ID3D12GraphicsCommandList* commandList = m_device->beginSingleTimeCommandList();
            m_buffer->resourceBarrior(commandList, D3D12_RESOURCE_STATE_COPY_DEST);
            commandList->CopyResource(m_buffer->dxResource(), uploadBuffer.dxResource());
            m_buffer->resourceBarrior(commandList, D3D12_RESOURCE_STATE_INDEX_BUFFER);
            m_device->endSingleTimeCommandList(commandList);
        }
    }

    return ok();
}

void DX12IndexBuffer::dispose()
{
    if (m_buffer) {
        m_buffer->dispose();
        m_buffer = nullptr;
    }
    RHIResource::dispose();
}

//==============================================================================
// DX12UniformBuffer

DX12UniformBuffer::DX12UniformBuffer()
    : m_deviceContext(nullptr)
    , m_size(0)
    , m_constantBuffer()
    , m_mappedBuffer(nullptr)
{
}

bool DX12UniformBuffer::init(DX12Device* deviceContext, uint32_t size)
{
    LN_DCHECK(deviceContext);
    m_deviceContext = deviceContext;
    m_size = size;

    ID3D12Device* device = m_deviceContext->device();

    // https://github.com/microsoft/DirectX-Graphics-Samples/blob/master/Samples/UWP/D3D12HelloWorld/src/HelloConstBuffers/d3dx12.h#L411
    D3D12_HEAP_PROPERTIES prop = {};
    prop.Type = D3D12_HEAP_TYPE_UPLOAD; // フレームごとに全体を書き換えるような運用をするため UPLOAD
    prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prop.CreationNodeMask = 1;
    prop.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC desc = {};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Alignment = 0;
    desc.Width = m_size;
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    if (FAILED(device->CreateCommittedResource(
        &prop,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_constantBuffer)))) {
        LN_ERROR("CreateCommittedResource failed.");
        return false;
    }

    // Map したままで OK
    if (FAILED(m_constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedBuffer)))) {
        LN_ERROR("Map failed.");
        return false;
    }

    return true;
}

void DX12UniformBuffer::dispose()
{
    if (m_constantBuffer) {
        m_constantBuffer->Unmap(0, nullptr);
        m_mappedBuffer = nullptr;
        m_constantBuffer.Reset();
    }
    RHIResource::dispose();
}

void* DX12UniformBuffer::map()
{
    return m_mappedBuffer;
}

void DX12UniformBuffer::unmap()
{
}

} // namespace detail
} // namespace ln
