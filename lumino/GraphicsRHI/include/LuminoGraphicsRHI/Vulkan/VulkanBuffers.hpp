
#pragma once
#include "VulkanHelper.hpp"

namespace ln {
namespace detail {

// 頂点バッファ、インデックスバッファ、レンダーターゲット転送のための一時バッファなど、様々な目的で使用する汎用バッファ。
class VulkanBuffer
{
public:
    VulkanBuffer();
    Result<> init(VulkanDevice* deviceContext, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, const VkAllocationCallbacks* allocator);
    void dispose();
    VulkanDevice* deviceContext() const { return m_deviceContext; }
    VkBuffer nativeBuffer() const { return m_nativeBuffer; }
    VkDeviceMemory nativeBufferMemory() const { return m_nativeBufferMemory; }
    VkDeviceSize size() const { return m_size; }
    void* map();
    void unmap();
    void setData(size_t offset, const void* data, VkDeviceSize size);

private:
    VulkanDevice* m_deviceContext;
    VkBuffer m_nativeBuffer;
    VkDeviceMemory m_nativeBufferMemory;
    VkDeviceSize m_size;
	const VkAllocationCallbacks* m_allocator;	// CommandBuffer が使用する一時メモリは Device とは Allocator が異なる
};

class VulkanVertexBuffer
    : public RHIResource
{
public:
    VulkanVertexBuffer();
    Result<> init(VulkanDevice* deviceContext, GraphicsResourceUsage usage, size_t bufferSize, const void* initialData);
    void onDestroy() override;
    //size_t getBytesSize() override;
    //GraphicsResourceUsage usage() const override;

    VulkanBuffer* buffer() { return &m_buffer; }
    VkBuffer vulkanBuffer() const { return m_buffer.nativeBuffer(); }
    VkDeviceMemory vulkanDeviceMemory() const { return m_buffer.nativeBufferMemory(); }

    VulkanSingleFrameBufferInfo m_mappedResource = { nullptr, 0 };

private:
    VulkanDevice* m_deviceContext;
    VulkanBuffer m_buffer;
    //GraphicsResourceUsage m_usage;
};

class VulkanIndexBuffer
    : public RHIResource
{
public:
    VulkanIndexBuffer();
    Result<> init(VulkanDevice* deviceContext, GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData);
    void onDestroy() override;
    //size_t getBytesSize() override;
    //GraphicsResourceUsage usage() const override;

    VulkanBuffer* buffer() { return &m_buffer; }
    VkBuffer vulkanBuffer() const { return m_buffer.nativeBuffer(); }
    VkDeviceMemory vulkanDeviceMemory() const { return m_buffer.nativeBufferMemory(); }
    VkIndexType indexType() const { return m_indexType; }

    VulkanSingleFrameBufferInfo m_mappedResource = { nullptr, 0 };

protected:
    VulkanDevice* m_deviceContext;
    VulkanBuffer m_buffer;
    GraphicsResourceUsage m_usage;
    VkIndexType m_indexType;
};

class VulkanUniformBuffer
    : public RHIResource
{
public:
    VulkanUniformBuffer();
    Result<> init(VulkanDevice* deviceContext, uint32_t size);
    void onDestroy() override;
    void* map() override;
    void unmap() override;
    VulkanBuffer* buffer() { return &m_buffer; }

protected:
    VulkanBuffer m_buffer;
};

} // namespace detail
} // namespace ln
