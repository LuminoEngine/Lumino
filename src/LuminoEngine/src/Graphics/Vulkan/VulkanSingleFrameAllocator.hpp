
#pragma once
#include "VulkanHelper.hpp"

namespace ln {
namespace detail {

#if 0
struct VulkanSingleFrameBufferInfo
{
	VulkanBuffer* buffer;
	uint32_t offset;
	uint32_t size;
};

// CommandList と 1:1 で存在し、1度の CommandList 実行中のメモリ転送・ディスクリプタで必要となるメモリの確保を行う。
class VulkanSingleFrameAllocator
{
public:
	VulkanSingleFrameAllocator();
	bool init(VulkanDevice* device);
	void reset();
	VulkanSingleFrameBufferInfo allocateUniformBuffer(uint32_t size);
	VulkanSingleFrameBufferInfo allocateTransferBuffer(uint32_t size);

private:
	struct BufferInfo
	{
		std::unique_ptr<VulkanBuffer> buffer;
		uint32_t usedBytes;
	};

	VulkanDevice* m_device;
	uint32_t m_pageSize;
	std::vector<BufferInfo> m_uniformBuffers;
	std::vector<BufferInfo> m_transferBuffers;
	int32_t m_uniformBufferIndex;
	int32_t m_transferBufferIndex;
};
#endif

} // namespace detail
} // namespace ln
