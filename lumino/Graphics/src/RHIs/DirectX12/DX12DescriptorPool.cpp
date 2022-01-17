
#include "Internal.hpp"
#include "DX12DeviceContext.hpp"
#include "DX12Textures.hpp"
#include "DX12ShaderPass.hpp"
#include "DX12CommandList.hpp"
#include "DX12DescriptorPool.hpp"

namespace ln {
namespace detail {
    
//==============================================================================
// DX12Descriptor

DX12Descriptor::DX12Descriptor(DX12DescriptorPool* pool)
    : m_pool(pool)
{
}

void DX12Descriptor::onUpdateData(const ShaderDescriptorTableUpdateInfo& data)
{
    ID3D12Device* dxDevice = m_pool->device()->device();

    const DX12ShaderPassLayoutInfo& layout = m_pool->shaderPass()->layoutInfo();

    // VertexShader
    {
        // Constant buffer (`b` register)
        for (int i = 0; i < layout.vs_CBV_Count(); i++) {
            //const ShaderDescriptorBufferView& view = data.uniforms[layout.vsDescriptors.bufferDescriptors[i].itemIndex];
            const ShaderDescriptorTableUpdateItem& item = data.uniforms[layout.vsDescriptors.bufferDescriptors[i].itemIndex];
            if (DX12UniformBuffer* buffer = static_cast<DX12UniformBuffer*>(item.object)) {
                D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
                desc.BufferLocation = buffer->dxResource()->GetGPUVirtualAddress() + item.offset;
                desc.SizeInBytes = DX12Helper::alignUp(layout.vsDescriptors.bufferDescriptors[i].size);
                dxDevice->CreateConstantBufferView(&desc, m_descriptorHandles2[DescriptorParamIndex_VS_CBV].cpuHandles[i]);
            }
            else {
                LN_ERROR();
            }
        }

        // Texture (`t` register)
        for (int i = 0; i < layout.vs_SRV_Count(); i++) {
            //const ShaderDescriptorCombinedSampler& view = data.textures[layout.vsDescriptors.textureDescriptors[i].itemIndex];
            const ShaderDescriptorTableUpdateItem& item = data.resources[layout.vsDescriptors.textureDescriptors[i].itemIndex];
            if (DX12Texture* texture = static_cast<DX12Texture*>(item.object)) {
                D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
                desc.Format = texture->dxFormat();
                desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
                desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                desc.Texture2D.MostDetailedMip = 0;
                desc.Texture2D.MipLevels = texture->mipLevels();
                desc.Texture2D.PlaneSlice = 0;
                desc.Texture2D.ResourceMinLODClamp = 0.0f;
                dxDevice->CreateShaderResourceView(texture->dxResource(), &desc, m_descriptorHandles2[DescriptorParamIndex_VS_SRV].cpuHandles[i]);
            }
            else {
                LN_ERROR();
            }
        }

        // SamplerState (`s` register)
        for (int i = 0; i < layout.vs_Sampler_Count(); i++) {
            //const ShaderDescriptorCombinedSampler& view = data.samplers[layout.vsDescriptors.samplerDescriptors[i].itemIndex];
            const ShaderDescriptorTableUpdateItem& item = data.samplers[layout.vsDescriptors.samplerDescriptors[i].itemIndex];
            if (DX12SamplerState* stamplerState = static_cast<DX12SamplerState*>(item.stamplerState)) {
                dxDevice->CreateSampler(&stamplerState->samplerDesc(), m_descriptorHandles2[DescriptorParamIndex_VS_Sampler].cpuHandles[i]);
            }
            else {
                // シェーダが s を要求しているのに C++ 側から設定が無い場合、
                // texture が持っている SamplerState を使う。
                // どうも CombindSampler (sampler2D) があると自動的に s レジスタが要求されるようだ。
                // TODO: これで本当に CombinedSampler ができるのか調べる
                //const ShaderDescriptorCombinedSampler& view2 = data.textures[i];
                const ShaderDescriptorTableUpdateItem& item2 = data.resources[i];
                if (DX12SamplerState* stamplerState = static_cast<DX12SamplerState*>(item2.stamplerState)) {
                    dxDevice->CreateSampler(&stamplerState->samplerDesc(), m_descriptorHandles2[DescriptorParamIndex_VS_Sampler].cpuHandles[i]);
                }
                else {
                    LN_ERROR();
                }
            }
        }
    }



    // PixelShader
    {
        // Constant buffer (`b` register)
        for (int i = 0; i < layout.ps_CBV_Count(); i++) {
            //const ShaderDescriptorBufferView& view = data.uniforms[layout.psDescriptors.bufferDescriptors[i].itemIndex];
            const ShaderDescriptorTableUpdateItem& item = data.uniforms[layout.psDescriptors.bufferDescriptors[i].itemIndex];
            if (DX12UniformBuffer* buffer = static_cast<DX12UniformBuffer*>(item.object)) {
                D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
                desc.BufferLocation = buffer->dxResource()->GetGPUVirtualAddress() + item.offset;
                desc.SizeInBytes = DX12Helper::alignUp(layout.psDescriptors.bufferDescriptors[i].size);
                dxDevice->CreateConstantBufferView(&desc, m_descriptorHandles2[DescriptorParamIndex_PS_CBV].cpuHandles[i]);
            }
            else {
                LN_ERROR();
            }
        }

        // Texture (`t` register)
        for (int i = 0; i < layout.ps_SRV_Count(); i++) {
            //const ShaderDescriptorCombinedSampler& view = data.textures[layout.psDescriptors.textureDescriptors[i].itemIndex];
            const ShaderDescriptorTableUpdateItem& item = data.resources[layout.psDescriptors.textureDescriptors[i].itemIndex];
            if (DX12Texture* texture = static_cast<DX12Texture*>(item.object)) {
                D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
                desc.Format = texture->dxFormat();
                desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
                desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
                desc.Texture2D.MostDetailedMip = 0;
                desc.Texture2D.MipLevels = texture->mipLevels();
                desc.Texture2D.PlaneSlice = 0;
                desc.Texture2D.ResourceMinLODClamp = 0.0f;
                dxDevice->CreateShaderResourceView(texture->dxResource(), &desc, m_descriptorHandles2[DescriptorParamIndex_PS_SRV].cpuHandles[i]);
            }
            else {
                LN_ERROR();
            }
        }

        // SamplerState (`s` register)
        for (int i = 0; i < layout.ps_Sampler_Count(); i++) {
            //const ShaderDescriptorCombinedSampler& view = data.samplers[layout.psDescriptors.samplerDescriptors[i].itemIndex];
            const ShaderDescriptorTableUpdateItem& item = data.samplers[layout.psDescriptors.samplerDescriptors[i].itemIndex];
            if (DX12SamplerState* stamplerState = static_cast<DX12SamplerState*>(item.stamplerState)) {
                dxDevice->CreateSampler(&stamplerState->samplerDesc(), m_descriptorHandles2[DescriptorParamIndex_PS_Sampler].cpuHandles[i]);
            }
            else {
                // シェーダが s を要求しているのに C++ 側から設定が無い場合、
                // texture が持っている SamplerState を使う。
                // どうも CombindSampler (sampler2D) があると自動的に s レジスタが要求されるようだ。
                // TODO: これで本当に CombinedSampler ができるのか調べる
                //const ShaderDescriptorCombinedSampler& view2 = data.textures[i];
                const ShaderDescriptorTableUpdateItem& item2 = data.resources[i];
                if (DX12SamplerState* stamplerState = static_cast<DX12SamplerState*>(item2.stamplerState)) {
                    dxDevice->CreateSampler(&stamplerState->samplerDesc(), m_descriptorHandles2[DescriptorParamIndex_PS_Sampler].cpuHandles[i]);
                }
                else {
                    LN_ERROR();
                }
            }
        }
    }
}

bool DX12Descriptor::allocateInternal()
{
    const DX12ShaderPassLayoutInfo& layout = m_pool->shaderPass()->layoutInfo();

    m_heapCount = 0;
    m_heaps = {};

    if (layout.vs_CBV_Count() > 0 || layout.vs_SRV_Count() > 0 ||
        layout.ps_CBV_Count() > 0 || layout.ps_SRV_Count() > 0) {
        if (layout.vs_CBV_Count() > 0) {
            if (!m_pool->descriptorHeapAllocator_CBV_SRV_UAV()->allocate(layout.vs_CBV_Count(), &m_descriptorHandles2[DescriptorParamIndex_VS_CBV])) {
                return false;
            }
        }
        if (layout.vs_SRV_Count() > 0) {
            if (!m_pool->descriptorHeapAllocator_CBV_SRV_UAV()->allocate(layout.vs_SRV_Count(), &m_descriptorHandles2[DescriptorParamIndex_VS_SRV])) {
                return false;
            }
        }
        if (layout.ps_CBV_Count() > 0) {
            if (!m_pool->descriptorHeapAllocator_CBV_SRV_UAV()->allocate(layout.ps_CBV_Count(), &m_descriptorHandles2[DescriptorParamIndex_PS_CBV])) {
                return false;
            }
        }
        if (layout.ps_SRV_Count() > 0) {
            if (!m_pool->descriptorHeapAllocator_CBV_SRV_UAV()->allocate(layout.ps_SRV_Count(), &m_descriptorHandles2[DescriptorParamIndex_PS_SRV])) {
                return false;
            }
        }

        if (layout.vs_CBV_Count() > 0 && layout.vs_SRV_Count() > 0) {
            // Allocator は (layout.cbvCount + layout.srvCount) の倍数で作ってあるので、必ず同じ Heap から Allocate されるはず
            assert(m_descriptorHandles2[DescriptorParamIndex_VS_CBV].descriptorHeap == m_descriptorHandles2[DescriptorParamIndex_VS_SRV].descriptorHeap);
        }
        
        // 'b' 't' の2つの register は同一の TYPE (D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) を使用する必要がある。
        // 同じ TYPE を持つ複数の Heap は SetDescriptorHeaps() で設定することができないため、
        // 'b' 't' は同じ Heap を使う必要がある。
        // http://masafumi.cocolog-nifty.com/masafumis_diary/2016/01/id3d12graphicsc.html
        if (layout.vs_CBV_Count() > 0) {
            m_heaps[m_heapCount] = m_descriptorHandles2[DescriptorParamIndex_VS_CBV].descriptorHeap;
            m_heapCount++;
        }
        else if (layout.vs_SRV_Count() > 0) {
            m_heaps[m_heapCount] = m_descriptorHandles2[DescriptorParamIndex_VS_SRV].descriptorHeap;
            m_heapCount++;
        }
        else if (layout.ps_CBV_Count() > 0) {
            m_heaps[m_heapCount] = m_descriptorHandles2[DescriptorParamIndex_PS_CBV].descriptorHeap;
            m_heapCount++;
        }
        else if (layout.ps_SRV_Count() > 0) {
            m_heaps[m_heapCount] = m_descriptorHandles2[DescriptorParamIndex_PS_SRV].descriptorHeap;
            m_heapCount++;
        }
        else {
            // 'b' 't' 両方未使用
        }

        //if (handles_CBV.descriptorHeap == handles_SRV.descriptorHeap) {
        //    m_heaps[m_heapCount] = handles_CBV.descriptorHeap;
        //    m_heapCount++;
        //}
        //else {
        //    m_heaps[m_heapCount] = handles_CBV.descriptorHeap;
        //    m_heapCount++;
        //    m_heaps[m_heapCount] = handles_SRV.descriptorHeap;
        //    m_heapCount++;
        //}
    }
    if (layout.vs_Sampler_Count() > 0) {
        if (!m_pool->descriptorHeapAllocator_SAMPLER()->allocate(layout.vs_Sampler_Count(), &m_descriptorHandles2[DescriptorParamIndex_VS_Sampler])) {
            return false;
        }
        m_heaps[m_heapCount] = m_descriptorHandles2[DescriptorParamIndex_VS_Sampler].descriptorHeap;
        m_heapCount++;
    }
    if (layout.ps_Sampler_Count() > 0) {
        if (!m_pool->descriptorHeapAllocator_SAMPLER()->allocate(layout.ps_Sampler_Count(), &m_descriptorHandles2[DescriptorParamIndex_PS_Sampler])) {
            return false;
        }

        if (m_heaps[m_heapCount] != m_descriptorHandles2[DescriptorParamIndex_PS_Sampler].descriptorHeap) {
            m_heaps[m_heapCount] = m_descriptorHandles2[DescriptorParamIndex_PS_Sampler].descriptorHeap;
            m_heapCount++;
        }
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
#if 1
    if (layout.vs_CBV_RootParamIndex >= 0) {
        dxCommandList->SetGraphicsRootDescriptorTable(
            layout.vs_CBV_RootParamIndex,
            m_descriptorHandles2[DescriptorParamIndex_VS_CBV].gpuHandles[0]);   // 'b' register
    }
    if (layout.vs_SRV_RootParamIndex >= 0) {
        dxCommandList->SetGraphicsRootDescriptorTable(
            layout.vs_SRV_RootParamIndex,
            m_descriptorHandles2[DescriptorParamIndex_VS_SRV].gpuHandles[0]);         // 't' register
    }
    if (layout.vs_Sampler_RootParamIndex >= 0) {
        dxCommandList->SetGraphicsRootDescriptorTable(
            layout.vs_Sampler_RootParamIndex,
            m_descriptorHandles2[DescriptorParamIndex_VS_Sampler].gpuHandles[0]);    // 's' register
    }
    if (layout.ps_CBV_RootParamIndex >= 0) {
        dxCommandList->SetGraphicsRootDescriptorTable(
            layout.ps_CBV_RootParamIndex,
            m_descriptorHandles2[DescriptorParamIndex_PS_CBV].gpuHandles[0]);   // 'b' register
    }
    if (layout.ps_SRV_RootParamIndex >= 0) {
        dxCommandList->SetGraphicsRootDescriptorTable(
            layout.ps_SRV_RootParamIndex,
            m_descriptorHandles2[DescriptorParamIndex_PS_SRV].gpuHandles[0]);         // 't' register
    }
    if (layout.ps_Sampler_RootParamIndex >= 0) {
        dxCommandList->SetGraphicsRootDescriptorTable(
            layout.ps_Sampler_RootParamIndex,
            m_descriptorHandles2[DescriptorParamIndex_PS_Sampler].gpuHandles[0]);    // 's' register
    }
#else
    if (layout.cbvRootParamIndex >= 0) {
        dxCommandList->SetGraphicsRootDescriptorTable(
            layout.cbvRootParamIndex,
            m_descriptorHandles[DescriptorType_UniformBuffer].gpuHandles[0]);   // 'b' register
    }
    if (layout.srvRootParamIndex >= 0) {
        dxCommandList->SetGraphicsRootDescriptorTable(
            layout.srvRootParamIndex,
            m_descriptorHandles[DescriptorType_Texture].gpuHandles[0]);         // 't' register
    }
    if (layout.samperRootParamIndex >= 0) {
        dxCommandList->SetGraphicsRootDescriptorTable(
            layout.samperRootParamIndex,
            m_descriptorHandles[DescriptorType_SamplerState].gpuHandles[0]);    // 's' register
    }
#endif
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

    if (layout.vs_CBV_Count() > 0 || layout.vs_SRV_Count() > 0 ||
        layout.ps_CBV_Count() > 0 || layout.ps_SRV_Count() > 0) {
        m_descriptorHeapAllocator_CBV_SRV_UAV = makeRef<DX12DescriptorHeapAllocator>();
        if (!m_descriptorHeapAllocator_CBV_SRV_UAV->init(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, (layout.vs_CBV_Count() + layout.vs_SRV_Count() + layout.ps_CBV_Count() + layout.ps_SRV_Count()) * AllocatableCountPerPage)) {
            return false;
        }
    }

    if (layout.vs_Sampler_Count() > 0 || layout.ps_Sampler_Count() > 0) {
        m_descriptorHeapAllocator_SAMPLER = makeRef<DX12DescriptorHeapAllocator>();
        if (!m_descriptorHeapAllocator_SAMPLER->init(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, (layout.vs_Sampler_Count() + layout.ps_Sampler_Count()) * AllocatableCountPerPage)) {
            return false;
        }
    }

    return true;
}

void DX12DescriptorPool::dispose()
{
    if (m_descriptorHeapAllocator_CBV_SRV_UAV) {
        m_descriptorHeapAllocator_CBV_SRV_UAV->dispose();
        m_descriptorHeapAllocator_CBV_SRV_UAV = nullptr;
    }
    if (m_descriptorHeapAllocator_SAMPLER) {
        m_descriptorHeapAllocator_SAMPLER->dispose();
        m_descriptorHeapAllocator_SAMPLER = nullptr;
    }
    IDescriptorPool::dispose();
}

void DX12DescriptorPool::reset()
{
    if (m_descriptorHeapAllocator_CBV_SRV_UAV) {
        m_descriptorHeapAllocator_CBV_SRV_UAV->reset();
    }
    if (m_descriptorHeapAllocator_SAMPLER) {
        m_descriptorHeapAllocator_SAMPLER->reset();
    }

    for (size_t i = 0; i < m_descriptors.size(); i++) {
        m_descriptors[i]->reset();
    }

    m_usedDescriptorCount = 0;
}

Result DX12DescriptorPool::allocate(IDescriptor** outDescriptor) {
    // grow
    if (m_usedDescriptorCount >= m_descriptors.size()) {
        for (int i = 0; i < AllocatableCountPerPage; i++) {
            auto descriptor = makeRef<DX12Descriptor>(this);
            m_descriptors.push_back(std::move(descriptor));
        }
    }

    DX12Descriptor* descriptor = m_descriptors[m_usedDescriptorCount];
    if (!descriptor->allocateInternal()) {
        return err();
    }
    m_usedDescriptorCount++;

    *outDescriptor = descriptor;
    return ok();
}

} // namespace detail
} // namespace ln
