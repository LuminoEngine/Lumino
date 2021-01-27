
#include "Internal.hpp"
#include "DX12DeviceContext.hpp"
#include "DX12ShaderPass.hpp"
#include "DX12CommandList.hpp"
#include "DX12DescriptorPool.hpp"

namespace ln {
namespace detail {
    
//==============================================================================
// DX12DescriptorHeapAllocatorPage

DX12DescriptorHeapAllocatorPage::DX12DescriptorHeapAllocatorPage()
    : m_descriptorHeap()
    , m_numDescriptors(0)
    , m_usedOffset(0)
    , m_handleSize(0)
{
}

bool DX12DescriptorHeapAllocatorPage::init(DX12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors)
{
    m_numDescriptors = numDescriptors;
    ID3D12Device* dxDevice = device->device();

    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = m_numDescriptors;
    desc.Type = type;
    desc.Flags = (type <= D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER)
        ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
        : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    desc.NodeMask = device->visibleNodeMask();    // TODO: これでいい？

    if (FAILED(dxDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_descriptorHeap)))) {
        LN_ERROR("CreateDescriptorHeap failed.");
        return false;
    }

    m_handleSize = dxDevice->GetDescriptorHandleIncrementSize(type);

    return true;
}

void DX12DescriptorHeapAllocatorPage::dispose()
{
    m_descriptorHeap.Reset();
}

void DX12DescriptorHeapAllocatorPage::reset()
{
    m_usedOffset = 0;
}

bool DX12DescriptorHeapAllocatorPage::allocate(int32_t count, DX12DescriptorHandles* outHandles)
{
    if (m_numDescriptors <= m_usedOffset + count) {
        return false;
    }

    auto cpuHandle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
    auto gpuHandle = m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();

    for (int i = 0; i < count; i++) {
        outHandles->cpuHandles[i].ptr = cpuHandle.ptr + m_handleSize * (i + m_usedOffset);
        outHandles->gpuHandles[i].ptr = gpuHandle.ptr + m_handleSize * (i + m_usedOffset);
    }

    m_usedOffset += count;

    return true;
}

//==============================================================================
// DX12DescriptorHeapAllocator

DX12DescriptorHeapAllocator::DX12DescriptorHeapAllocator()
{
}

bool DX12DescriptorHeapAllocator::init(DX12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptorsPerPage)
{
    m_device = device;
    m_type = type;
    m_numDescriptorsPerPage = numDescriptorsPerPage;

    auto page = makeRef<DX12DescriptorHeapAllocatorPage>();
    if (!page->init(m_device, m_type, m_numDescriptorsPerPage)) {
        return false;
    }

    m_pages.push_back(page);
    m_activePageIndex = 0;
    return true;
}

void DX12DescriptorHeapAllocator::dispose()
{
}

void DX12DescriptorHeapAllocator::reset()
{
    for (auto& page : m_pages) {
        page->reset();
    }
    m_activePageIndex = 0;
}

bool DX12DescriptorHeapAllocator::allocate(int32_t count, DX12DescriptorHandles* outHandles)
{
    if (m_pages[m_activePageIndex]->allocate(count, outHandles)) {
        outHandles->descriptorHeap = m_pages[m_activePageIndex]->descriptorHeap();
        return true;
    }
    else {
        // Active Page が一杯だったので新しい Page へ遷移する

        m_activePageIndex++;
        if (m_activePageIndex < static_cast<int32_t>(m_pages.size()) &&
            m_pages[m_activePageIndex]->allocate(count, outHandles)) {
            outHandles->descriptorHeap = m_pages[m_activePageIndex]->descriptorHeap();
            return true;
        }

        // 新しい Page を作って allocate 再試行
        auto page = makeRef<DX12DescriptorHeapAllocatorPage>();
        if (!page->init(m_device, m_type, m_numDescriptorsPerPage)) {
            return false;
        }
        m_pages.push_back(page);
        if (m_pages[m_activePageIndex]->allocate(count, outHandles)) {
            outHandles->descriptorHeap = m_pages[m_activePageIndex]->descriptorHeap();
            return true;
        }
    }

    LN_UNREACHABLE();
    return false;
}

//==============================================================================
// DX12Descriptor

DX12Descriptor::DX12Descriptor(DX12DescriptorPool* pool)
    : m_pool(pool)
{
}

void DX12Descriptor::setData(const ShaderDescriptorTableUpdateInfo& data)
{
    ID3D12Device* dxDevice = m_pool->device()->device();

    const DX12ShaderPassLayoutInfo& layout = m_pool->shaderPass()->layoutInfo();

    // Constant buffer (`b` register)
    for (int i = 0; i < layout.cbvCount; i++) {
        const ShaderDescriptorBufferView& view = data.uniforms[i];
        if (DX12UniformBuffer* buffer = static_cast<DX12UniformBuffer*>(view.buffer)) {
            D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
            desc.BufferLocation = buffer->dxResource()->GetGPUVirtualAddress() + view.offset;
            desc.SizeInBytes = buffer->size() - view.offset;
            dxDevice->CreateConstantBufferView(&desc, m_descriptorHandles[DescriptorType_UniformBuffer].cpuHandles[i]);
        }
        else {
            LN_ERROR();
        }
    }

    // Texture (`t` register)
    for (int i = 0; i < layout.srvCount; i++) {
        LN_NOTIMPLEMENTED();
    }

    // SamplerState (`s` register)
    for (int i = 0; i < layout.srvCount; i++) {
        LN_NOTIMPLEMENTED();
    }
}

bool DX12Descriptor::allocateInternal()
{
    const DX12ShaderPassLayoutInfo& layout = m_pool->shaderPass()->layoutInfo();

    m_heapCount = 0;
    if (layout.cbvCount > 0) {
        if (!m_pool->descriptorHeapAllocator_CBV()->allocate(layout.cbvCount, &m_descriptorHandles[DescriptorType_UniformBuffer])) {
            return false;
        }
        m_heaps[m_heapCount] = m_descriptorHandles[DescriptorType_UniformBuffer].descriptorHeap;
        m_heapCount++;
    }
    if (layout.srvCount > 0) {
        if (!m_pool->descriptorHeapAllocator_SRV()->allocate(layout.srvCount, &m_descriptorHandles[DescriptorType_Texture])) {
            return false;
        }
        m_heaps[m_heapCount] = m_descriptorHandles[DescriptorType_Texture].descriptorHeap;
        m_heapCount++;
    }
    if (layout.samplerCount > 0) {
        if (!m_pool->descriptorHeapAllocator_SAMPLER()->allocate(layout.samplerCount, &m_descriptorHandles[DescriptorType_SamplerState])) {
            return false;
        }
        m_heaps[m_heapCount] = m_descriptorHandles[DescriptorType_SamplerState].descriptorHeap;
        m_heapCount++;
    }

    return true;
}

void DX12Descriptor::bind(DX12GraphicsContext* commandList)
{
    const DX12ShaderPassLayoutInfo& layout = m_pool->shaderPass()->layoutInfo();

    ID3D12GraphicsCommandList* dxCommandList = commandList->dxCommandList();

    dxCommandList->SetDescriptorHeaps(m_heapCount, m_heaps.data());

    // SetGraphicsRootDescriptorTable() の第1引数は D3D12_ROOT_PARAMETER のインデックス と対応する。
    // 第2引数は ID3D12DescriptorHeap のバッファ内にある Descriptor 配列の先頭アドレス。
    // b0, b1... や t0, t1.. など複数の register(Descriptor) を使っている場合、
    // ID3D12DescriptorHeap::GetGPUDescriptorHandleForHeapStart() を stride として並んでいなければならない。
    // サンプルの D3D12HDR.cpp あたりが参考になる。
    dxCommandList->SetGraphicsRootDescriptorTable(
        layout.cbvRootParamIndex,
        m_descriptorHandles[DescriptorType_UniformBuffer].gpuHandles[0]);   // 'b' register
    dxCommandList->SetGraphicsRootDescriptorTable(
        layout.srvRootParamIndex,
        m_descriptorHandles[DescriptorType_Texture].gpuHandles[0]);         // 't' register
    dxCommandList->SetGraphicsRootDescriptorTable(
        layout.samperRootParamIndex,
        m_descriptorHandles[DescriptorType_SamplerState].gpuHandles[0]);    // 's' register
}

//==============================================================================
// DX12DescriptorPool

DX12DescriptorPool::DX12DescriptorPool()
    : m_device(nullptr)
    , m_shaderPass(nullptr)
    , m_usedDescriptorCount(0)
{
}

bool DX12DescriptorPool::init(DX12Device* device, DX12ShaderPass* shaderPass)
{
    m_device = device;
    m_shaderPass = shaderPass;

    const auto& layout = shaderPass->layoutInfo();

    if (layout.cbvCount > 0) {
        m_descriptorHeapAllocator_CBV = makeRef<DX12DescriptorHeapAllocator>();
        if (!m_descriptorHeapAllocator_CBV->init(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, layout.cbvCount * AllocatableCountPerPage)) {
            return false;
        }
    }

    if (layout.srvCount > 0) {
        m_descriptorHeapAllocator_SRV = makeRef<DX12DescriptorHeapAllocator>();
        if (!m_descriptorHeapAllocator_SRV->init(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, layout.srvCount * AllocatableCountPerPage)) {
            return false;
        }
    }

    if (layout.samplerCount > 0) {
        m_descriptorHeapAllocator_SAMPLER = makeRef<DX12DescriptorHeapAllocator>();
        if (!m_descriptorHeapAllocator_SAMPLER->init(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, layout.samplerCount * AllocatableCountPerPage)) {
            return false;
        }
    }

    return true;
}

void DX12DescriptorPool::dispose()
{
    if (m_descriptorHeapAllocator_CBV) {
        m_descriptorHeapAllocator_CBV->dispose();
        m_descriptorHeapAllocator_CBV = nullptr;
    }
    if (m_descriptorHeapAllocator_SRV) {
        m_descriptorHeapAllocator_SRV->dispose();
        m_descriptorHeapAllocator_SRV = nullptr;
    }
    if (m_descriptorHeapAllocator_SAMPLER) {
        m_descriptorHeapAllocator_SAMPLER->dispose();
        m_descriptorHeapAllocator_SAMPLER = nullptr;
    }
}

void DX12DescriptorPool::reset()
{
    m_descriptorHeapAllocator_CBV->reset();
    m_descriptorHeapAllocator_SRV->reset();
    m_descriptorHeapAllocator_SAMPLER->reset();
    m_usedDescriptorCount = 0;
}

IDescriptor* DX12DescriptorPool::allocate()
{
    // grow
    if (m_usedDescriptorCount >= m_descriptors.size()) {
        for (int i = 0; i < AllocatableCountPerPage; i++) {
            auto descriptor = makeRef<DX12Descriptor>(this);
            m_descriptors.push_back(std::move(descriptor));
        }
    }

    DX12Descriptor* descriptor = m_descriptors[m_usedDescriptorCount];
    if (!descriptor->allocateInternal()) {
        return nullptr;
    }
    m_usedDescriptorCount++;

    return descriptor;
}

} // namespace detail
} // namespace ln
