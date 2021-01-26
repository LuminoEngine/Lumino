
#include "Internal.hpp"
#include "DX12DescriptorPool.hpp"

namespace ln {
namespace detail {

DX12Descriptor::DX12Descriptor(DX12DescriptorPool* pool)
	: m_pool(pool)
{
}

void DX12Descriptor::setData(const ShaderDescriptorTableUpdateInfo& data)
{
	LN_NOTIMPLEMENTED();
}

//==============================================================================
// DX12DescriptorPool

DX12DescriptorPool::DX12DescriptorPool()
{
}

bool DX12DescriptorPool::init(DX12Device* device, DX12ShaderPass* shaderPass)
{
	m_device = device;
	m_shaderPass = shaderPass;

	return true;
}

void DX12DescriptorPool::dispose()
{
}

void DX12DescriptorPool::reset()
{
}

IDescriptor* DX12DescriptorPool::allocate()
{
	LN_NOTIMPLEMENTED();
	return 0;
}

} // namespace detail
} // namespace ln
