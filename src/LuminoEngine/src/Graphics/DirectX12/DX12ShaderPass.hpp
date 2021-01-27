
#pragma once
#include "DX12Helper.hpp"

namespace ln {
namespace detail {

struct DX12ShaderPassLayoutInfo
{
    int8_t cbvRootParamIndex;
    int8_t cbvCount;            // UniformBuffer
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

private:
    DX12Device* m_deviceContext;
    ComPtr<ID3D12RootSignature> m_rootSignature;
    ComPtr<ID3D12DescriptorHeap> m_descriptorHeap;
    DX12ShaderPassLayoutInfo m_layoutInfo;
};

} // namespace detail
} // namespace ln
