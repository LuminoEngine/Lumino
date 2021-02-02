
#pragma once
#include "DX12Helper.hpp"

namespace ln {
namespace detail {

struct DX12ShaderPassLayoutInfo
{
    struct Descriptor
    {
        int32_t itemIndex;
        int32_t registerIndex;  // b1, t1, s1 の "1" の部分
        int32_t size;
    };

    struct Descriptors
    {
        std::vector<Descriptor> bufferDescriptors;
        std::vector<Descriptor> textureDescriptors;
        std::vector<Descriptor> samplerDescriptors;
    };
    Descriptors vsDescriptors;
    Descriptors psDescriptors;

    // [VertexShader Stage] ConstantBuffer
    int8_t vs_CBV_RootParamIndex;
    int8_t vs_CBV_Count() const { return vsDescriptors.bufferDescriptors.size(); }
    // [VertexShader Stage] Texture
    int8_t vs_SRV_RootParamIndex;
    int8_t vs_SRV_Count() const { return vsDescriptors.textureDescriptors.size(); }
    // [VertexShader Stage] Sampler
    int8_t vs_Sampler_RootParamIndex;
    int8_t vs_Sampler_Count() const { return vsDescriptors.samplerDescriptors.size(); }

    // [PixelShader Stage] ConstantBuffer
    int8_t ps_CBV_RootParamIndex;
    int8_t ps_CBV_Count() const { return psDescriptors.bufferDescriptors.size(); }
    // [PixelShader Stage] Texture
    int8_t ps_SRV_RootParamIndex;
    int8_t ps_SRV_Count() const { return psDescriptors.textureDescriptors.size(); }
    // [PixelShader Stage] Sampler
    int8_t ps_Sampler_RootParamIndex;
    int8_t ps_Sampler_Count() const { return psDescriptors.samplerDescriptors.size(); }
};

class DX12ShaderPass
    : public IShaderPass
{
public:
    DX12ShaderPass();
    Result init(DX12Device* deviceContext, const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag);
    void dispose();
    const DX12ShaderPassLayoutInfo& layoutInfo() const { return m_layoutInfo; }
    ID3D12RootSignature* rootSignature() const { return m_rootSignature.Get(); }
    //const std::vector<uint8_t>& vsCode() const { return m_vsCode; }
    //const std::vector<uint8_t>& psCode() const { return m_psCode; }

    D3D12_SHADER_BYTECODE dxVSByteCode() const;
    D3D12_SHADER_BYTECODE dxPSByteCode() const;

private:
    DX12Device* m_deviceContext;
    DX12ShaderPassLayoutInfo m_layoutInfo;
    ComPtr<ID3D12RootSignature> m_rootSignature;
    //ComPtr<ID3D12DescriptorHeap> m_descriptorHeap;
    //std::vector<uint8_t> m_vsCode;
    //std::vector<uint8_t> m_psCode;

    ComPtr<ID3DBlob> m_vsCode;
    ComPtr<ID3DBlob> m_psCode;
};

} // namespace detail
} // namespace ln
