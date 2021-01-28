
#pragma once
#include "DX12Helper.hpp"

namespace ln {
namespace detail {

struct DX12ShaderPassLayoutInfo
{
    int8_t cbvRootParamIndex;
    int8_t cbvCount;            // UniformBuffer
    std::vector<int32_t> cvbSizes;
    int8_t srvRootParamIndex;
    int8_t srvCount;            // Texture
    int8_t samperRootParamIndex;
    int8_t samplerCount;        // Sampler
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
    const std::vector<uint8_t>& vsCode() const { return m_vsCode; }
    const std::vector<uint8_t>& psCode() const { return m_psCode; }

    D3D12_SHADER_BYTECODE dxVSByteCode() const;
    D3D12_SHADER_BYTECODE dxPSByteCode() const;

private:
    DX12Device* m_deviceContext;
    DX12ShaderPassLayoutInfo m_layoutInfo;
    ComPtr<ID3D12RootSignature> m_rootSignature;
    ComPtr<ID3D12DescriptorHeap> m_descriptorHeap;
    std::vector<uint8_t> m_vsCode;
    std::vector<uint8_t> m_psCode;
};

} // namespace detail
} // namespace ln
