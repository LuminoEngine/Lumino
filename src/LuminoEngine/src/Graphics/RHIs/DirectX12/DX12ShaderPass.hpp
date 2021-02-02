
#pragma once
#include "DX12Helper.hpp"

namespace ln {
namespace detail {

struct DX12ShaderPassLayoutInfo
{
    struct Descriptors
    {
        std::vector<int> bufferDescriptors;
        std::vector<size_t> bufferSizes;
        std::vector<int> textureDescriptors;
        std::vector<int> samplerDescriptors;
    };
    Descriptors vsDescriptors;
    Descriptors psDescriptors;

    // [VertexShader Stage] ConstantBuffer
    int8_t vs_CBV_RootParamIndex;
    int8_t vs_CBV_Count;
    // [VertexShader Stage] Texture
    int8_t vs_SRV_RootParamIndex;
    int8_t vs_SRV_Count;
    // [VertexShader Stage] Sampler
    int8_t vs_Sampler_RootParamIndex;
    int8_t vs_Sampler_Count;

    // [PixelShader Stage] ConstantBuffer
    int8_t ps_CBV_RootParamIndex;
    int8_t ps_CBV_Count;
    // [PixelShader Stage] Texture
    int8_t ps_SRV_RootParamIndex;
    int8_t ps_SRV_Count;
    // [PixelShader Stage] Sampler
    int8_t ps_Sampler_RootParamIndex;
    int8_t ps_Sampler_Count;
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
