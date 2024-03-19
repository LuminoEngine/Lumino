#pragma once
#include <LuminoEngine/GraphicsRHI/Vulkan/VulkanHelper.hpp>

namespace ln {
namespace detail {
class VulkanDevice;

class VulkanSingleFrameAllocatorPage
	: public AbstractLinearAllocatorPage
{
public:
	virtual ~VulkanSingleFrameAllocatorPage();
	bool init(VulkanDevice* device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
	VulkanBuffer* buffer() const { return m_buffer.get(); }

private:
	std::unique_ptr<VulkanBuffer> m_buffer;
};

class VulkanSingleFrameAllocatorPageManager
	: public LinearAllocatorPageManager
{
public:
	VulkanSingleFrameAllocatorPageManager(VulkanDevice* device, size_t pageSize, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

protected:
	Ref<AbstractLinearAllocatorPage> onCreateNewPage(size_t size) override;

private:
	VulkanDevice* m_device;
	VkBufferUsageFlags m_usage;
	VkMemoryPropertyFlags m_properties;
};

class VulkanSingleFrameAllocator
	: public AbstractLinearAllocator
{
public:
	VulkanSingleFrameAllocator(VulkanSingleFrameAllocatorPageManager* manager);
	VulkanSingleFrameBufferInfo allocate(size_t size, size_t alignment = 64);

protected:
};

#if 0

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
