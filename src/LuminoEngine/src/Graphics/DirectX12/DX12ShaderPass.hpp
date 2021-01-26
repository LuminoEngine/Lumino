
#pragma once
#include "DX12Helper.hpp"

namespace ln {
namespace detail {
	
class DX12ShaderPass
    : public IShaderPass
{
public:
    DX12ShaderPass();
    Result init(DX12Device* deviceContext, const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag);
    void dispose();

private:
    DX12Device* m_deviceContext;
    ComPtr<ID3D12RootSignature> m_rootSignature;
    ComPtr<ID3D12DescriptorHeap> m_descriptorHeap;
};

} // namespace detail
} // namespace ln
