
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
	enum HeapIndex
	{
		HeapIndex_CBV_SRV_UAV = 0,
		HeapIndex_Sampler = 1,
		HeapIndex_Count = 2,
	};

	DX12DescriptorPool* m_pool;
	std::array<DX12DescriptorHandles, DescriptorType_Count> m_descriptorHandles;
	std::array<ID3D12DescriptorHeap*, HeapIndex_Count> m_heaps;
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
	const Ref<DX12DescriptorHeapAllocator>& descriptorHeapAllocator_CBV_SRV_UAV() const { return m_descriptorHeapAllocator_CBV_SRV_UAV; }
	const Ref<DX12DescriptorHeapAllocator>& descriptorHeapAllocator_SAMPLER() const { return m_descriptorHeapAllocator_SAMPLER; }


private:
	static const int AllocatableCountPerPage = 256;

	//void grow(int32_t count);

	DX12Device* m_device;
	DX12ShaderPass* m_shaderPass;

	// SetDescriptorHeaps() は、同一の Type である Heap を複数登録できない。必ずこの2つにする必要がある。
	Ref<DX12DescriptorHeapAllocator> m_descriptorHeapAllocator_CBV_SRV_UAV;	// `b`, `t` register
	Ref<DX12DescriptorHeapAllocator> m_descriptorHeapAllocator_SAMPLER;		// `s` register

	std::vector<Ref<DX12Descriptor>> m_descriptors;
	int32_t m_usedDescriptorCount;
};

} // namespace detail
} // namespace ln
