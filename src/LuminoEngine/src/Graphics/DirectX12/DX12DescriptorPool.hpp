
#pragma once
#include <LuminoCore/Base/LinearAllocator.hpp>
#include "DX12Helper.hpp"

namespace ln {
namespace detail {
class DX12Device;
class DX12ShaderPass;
class DX12DescriptorPool;
class DX12GraphicsContext;

//struct DX12DescriptorHeapView
//{
//    //VulkanBuffer* buffer;
//    //uint32_t offset;
//};
//
//class DX12DescriptorHeapAllocatorPage
//	: public AbstractLinearAllocatorPage
//{
//public:
//
//private:
//};
//
//class DX12DescriptorHeapAllocatorPageManager
//	: public LinearAllocatorPageManager
//{
//public:
//
//private:
//};
//
//class VulkanSingleFrameAllocator
//	: public AbstractLinearAllocator
//{
//public:
//	VulkanSingleFrameAllocator(DX12DescriptorHeapAllocatorPageManager* manager);
//	DX12DescriptorHeapView allocate(size_t size, size_t alignment = 64);
//
//protected:
//};
//

static const int MaxDescriptorHandles = 16;

// VulkanDescriptor で使っている VkDescriptorSet 相当
struct DX12DescriptorHandles
{
	ID3D12DescriptorHeap* descriptorHeap = nullptr;
	std::array<D3D12_CPU_DESCRIPTOR_HANDLE, MaxDescriptorHandles> cpuHandles = {};
	std::array<D3D12_GPU_DESCRIPTOR_HANDLE, MaxDescriptorHandles> gpuHandles = {};
};

class DX12DescriptorHeapAllocatorPage
	: public RefObject
{
public:
	DX12DescriptorHeapAllocatorPage();
	bool init(DX12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors);
	void dispose();
	void reset();
	bool allocate(int32_t count, DX12DescriptorHandles* outHandles);
	ID3D12DescriptorHeap* descriptorHeap() const { return m_descriptorHeap.Get(); }

private:
	ComPtr<ID3D12DescriptorHeap> m_descriptorHeap;
	int32_t m_numDescriptors;
	int32_t m_usedOffset;
	UINT m_handleSize;
};

class DX12DescriptorHeapAllocator
	: public RefObject
{
public:
	DX12DescriptorHeapAllocator();
	bool init(DX12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptorsPerPage);
	void dispose();
	void reset();
	bool allocate(int32_t count, DX12DescriptorHandles* outHandles);

private:
	DX12Device* m_device;
	D3D12_DESCRIPTOR_HEAP_TYPE m_type;
	UINT m_numDescriptorsPerPage;
	std::vector<Ref<DX12DescriptorHeapAllocatorPage>> m_pages;
	int m_activePageIndex;
};


class DX12Descriptor
	: public IDescriptor
{
public:
	DX12Descriptor(DX12DescriptorPool* pool);
	void setData(const ShaderDescriptorTableUpdateInfo& data) override;
	std::array<DX12DescriptorHandles, DescriptorType_Count>& descriptorHandles() { return m_descriptorHandles; }
	bool allocateInternal();
	void bind(DX12GraphicsContext* commandList);

private:
	DX12DescriptorPool* m_pool;
	std::array<DX12DescriptorHandles, DescriptorType_Count> m_descriptorHandles;
	std::array<ID3D12DescriptorHeap*, DescriptorType_Count> m_heaps;
	int32_t m_heapCount;
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
	DX12Device* device() const { return m_device; }
	DX12ShaderPass* shaderPass() const { return m_shaderPass; }
	const Ref<DX12DescriptorHeapAllocator>& descriptorHeapAllocator_CBV() const { return m_descriptorHeapAllocator_CBV; }
	const Ref<DX12DescriptorHeapAllocator>& descriptorHeapAllocator_SRV() const { return m_descriptorHeapAllocator_SRV; }
	const Ref<DX12DescriptorHeapAllocator>& descriptorHeapAllocator_SAMPLER() const { return m_descriptorHeapAllocator_SAMPLER; }


private:
	static const int AllocatableCountPerPage = 256;

	//void grow(int32_t count);

	DX12Device* m_device;
	DX12ShaderPass* m_shaderPass;

	// NOTE: CBV(UniformBuffer) と SRV(Texture) の D3D12_DESCRIPTOR_HEAP_TYPE は同じものを使うのだが、
	// Layout との対応関係をわかりやすくするため別の Allocator に分けてある。

	Ref<DX12DescriptorHeapAllocator> m_descriptorHeapAllocator_CBV;
	Ref<DX12DescriptorHeapAllocator> m_descriptorHeapAllocator_SRV;
	Ref<DX12DescriptorHeapAllocator> m_descriptorHeapAllocator_SAMPLER;

	std::vector<Ref<DX12Descriptor>> m_descriptors;
	int32_t m_usedDescriptorCount;
};

} // namespace detail
} // namespace ln
