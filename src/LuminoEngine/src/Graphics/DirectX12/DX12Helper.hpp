
#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include "../GraphicsDeviceContext.hpp"

using Microsoft::WRL::ComPtr;

namespace ln {
namespace detail {
class DX12Device;

class DX12Helper
{
public:
	static DXGI_FORMAT LNTextureFormatToDXFormat(TextureFormat value);
	static TextureFormat DXFormatToLNTextureFormat(DXGI_FORMAT value);
	static DXGI_FORMAT LNVertexElementTypeToDXFormat(VertexElementType value);
	static const char* LNVertexElementUsageToSemanticName(VertexElementUsage value);
};


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

} // namespace detail
} // namespace ln
