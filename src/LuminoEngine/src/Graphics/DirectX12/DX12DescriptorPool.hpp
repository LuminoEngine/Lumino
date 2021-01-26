
#pragma once
#include "DX12Helper.hpp"

namespace ln {
namespace detail {
class DX12Device;
class DX12ShaderPass;
class DX12DescriptorPool;

class DX12Descriptor
	: public IDescriptor
{
public:
	DX12Descriptor(DX12DescriptorPool* pool);
	void setData(const ShaderDescriptorTableUpdateInfo& data) override;

private:
	DX12DescriptorPool* m_pool;
};


// ShaderPass によって管理され、通常は CommandList と同じ数だけインスタンスが作られる。
class DX12DescriptorPool
	: public IDescriptorPool
{
public:
	DX12DescriptorPool();
	bool init(DX12Device* device, DX12ShaderPass* shaderPass);
	void dispose() override;
	void reset() override;
	IDescriptor* allocate() override;

private:
	DX12Device* m_device;
	DX12ShaderPass* m_shaderPass;
};

} // namespace detail
} // namespace ln
