
#include "Internal.hpp"
#include "VulkanSingleFrameAllocator.hpp"

namespace ln {
namespace detail {

#if 0
VulkanSingleFrameAllocator::VulkanSingleFrameAllocator()
	: m_device(nullptr)
	, m_pageSize(0x200000)	// 2MB
	, m_uniformBuffers()
	, m_transferBuffers()
	, m_uniformBufferIndex(0)
	, m_transferBufferIndex(0)
{
}

bool VulkanSingleFrameAllocator::init(VulkanDevice* device)
{
	return true;
}

void VulkanSingleFrameAllocator::reset()
{
	for (auto& info : m_uniformBuffers) {
		info.usedBytes = 0;
	}
	for (auto& info : m_transferBuffers) {
		info.usedBytes = 0;
	}
	m_uniformBufferIndex = 0;
	m_transferBufferIndex = 0;
}

VulkanSingleFrameBufferInfo VulkanSingleFrameAllocator::allocateUniformBuffer(uint32_t size)
{
	if (LN_REQUIRE(size <= m_pageSize)) return;


	// Grow?
	if (m_uniformBuffers.empty() || m_uniformBuffers[m_uniformBufferIndex].usedBytes + size > m_pageSize) {
		if (m_uniformBufferIndex < m_uniformBuffers.size() - 1) {
			m_uniformBufferIndex++;
		}
		else {
			BufferInfo info;
			info.usedBytes = 0;
			info.buffer = std::make_unique<VulkanBuffer>();
			if (!info.buffer->init(m_device, m_pageSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, nullptr)) {
				return {};
			}
			m_uniformBuffers.push_back(std::move(info));
			m_uniformBufferIndex = m_uniformBuffers.size() - 1;
		}
	}





	//VulkanCommandBuffer::allocateBuffer
	//VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
	//VK_BUFFER_USAGE_TRANSFER_SRC_BIT
}

VulkanSingleFrameBufferInfo VulkanSingleFrameAllocator::allocateTransferBuffer(uint32_t size)
{
	//VulkanCommandBuffer::allocateBuffer
	//VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
	//VK_BUFFER_USAGE_TRANSFER_SRC_BIT
}
#endif

} // namespace detail
} // namespace ln

