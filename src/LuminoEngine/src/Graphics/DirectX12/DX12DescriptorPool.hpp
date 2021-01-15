
#pragma once
#include "DX12Helper.hpp"

namespace ln {
namespace detail {
class DX12Device;

// ShaderPass によって管理され、通常は CommandList と同じ数だけインスタンスが作られる。
class DX12DescriptorPool
	: public RefObject
{
public:
	DX12DescriptorPool();
	bool init(DX12Device* device);

private:
};

} // namespace detail
} // namespace ln
