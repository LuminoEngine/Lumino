
#include "Internal.hpp"
#include "DX12DeviceContext.hpp"
#include "DX12DescriptorPool.hpp"

namespace ln {
namespace detail {
	
//==============================================================================
// DX12DescriptorHeapAllocatorPage

DX12DescriptorHeapAllocatorPage::DX12DescriptorHeapAllocatorPage()
	: m_descriptorHeap()
	, m_numDescriptors(0)
	, m_usedOffset(0)
	, m_handleSize(0)
{
}

bool DX12DescriptorHeapAllocatorPage::init(DX12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors)
{
	m_numDescriptors = numDescriptors;
	ID3D12Device* dxDevice = device->device();

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = m_numDescriptors;
	desc.Type = type;
	desc.Flags = (type <= D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER)
		? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
		: D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask = device->visibleNodeMask();	// TODO: これでいい？

	if (FAILED(dxDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_descriptorHeap)))) {
		LN_ERROR("CreateDescriptorHeap failed.");
		return false;
	}

	m_handleSize = dxDevice->GetDescriptorHandleIncrementSize(type);

	return true;
}

void DX12DescriptorHeapAllocatorPage::dispose()
{
	m_descriptorHeap.Reset();
}

void DX12DescriptorHeapAllocatorPage::reset()
{
	m_usedOffset = 0;
}

bool DX12DescriptorHeapAllocatorPage::allocate(int32_t count, DX12DescriptorHandles* outHandles)
{
	if (m_numDescriptors <= m_usedOffset + count) {
		return false;
	}

	auto cpuHandle = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	auto gpuHandle = m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();

	for (int i = 0; i < count; i++) {
		outHandles->cpuHandles[i].ptr += m_handleSize * (i + m_usedOffset);
		outHandles->gpuHandles[i].ptr += m_handleSize * (i + m_usedOffset);
	}

	m_usedOffset += count;

	return true;
}

//==============================================================================
// DX12DescriptorHeapAllocator

DX12DescriptorHeapAllocator::DX12DescriptorHeapAllocator()
{
}

bool DX12DescriptorHeapAllocator::init(DX12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptorsPerPage)
{
	m_device = device;
	m_type = type;
	m_numDescriptorsPerPage = numDescriptorsPerPage;

	auto page = makeRef<DX12DescriptorHeapAllocatorPage>();
	if (!page->init(m_device, m_type, m_numDescriptorsPerPage)) {
		return false;
	}

	m_pages.push_back(page);
	m_activePageIndex = 0;
	return true;
}

void DX12DescriptorHeapAllocator::dispose()
{
}

void DX12DescriptorHeapAllocator::reset()
{
	for (auto& page : m_pages) {
		page->reset();
	}
	m_activePageIndex = 0;
}

bool DX12DescriptorHeapAllocator::allocate(int32_t count, DX12DescriptorHandles* outHandles)
{
	if (m_pages[m_activePageIndex]->allocate(count, outHandles)) {
		outHandles->descriptorHeap = m_pages[m_activePageIndex]->descriptorHeap();
		return true;
	}
	else {
		// Active Page が一杯だったので新しい Page へ遷移する

		m_activePageIndex++;
		if (m_activePageIndex < static_cast<int32_t>(m_pages.size()) &&
			m_pages[m_activePageIndex]->allocate(count, outHandles)) {
			outHandles->descriptorHeap = m_pages[m_activePageIndex]->descriptorHeap();
			return true;
		}

		// 新しい Page を作って allocate 再試行
		auto page = makeRef<DX12DescriptorHeapAllocatorPage>();
		if (!page->init(m_device, m_type, m_numDescriptorsPerPage)) {
			return false;
		}
		m_pages.push_back(page);
		if (m_pages[m_activePageIndex]->allocate(count, outHandles)) {
			outHandles->descriptorHeap = m_pages[m_activePageIndex]->descriptorHeap();
			return true;
		}
	}

	LN_UNREACHABLE();
	return false;
}

//==============================================================================
// DX12Descriptor

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
	: m_device(nullptr)
	, m_shaderPass(nullptr)
{
}

bool DX12DescriptorPool::init(DX12Device* device, DX12ShaderPass* shaderPass)
{
	m_device = device;
	m_shaderPass = shaderPass;

	m_descriptorHeapAllocator_CBV_SRV_UAV = makeRef<DX12DescriptorHeapAllocator>();
	if (!m_descriptorHeapAllocator_CBV_SRV_UAV->init(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, NumDescriptorsPerPage)) {
		return false;
	}

	m_descriptorHeapAllocator_SAMPLER = makeRef<DX12DescriptorHeapAllocator>();
	if (!m_descriptorHeapAllocator_SAMPLER->init(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, NumDescriptorsPerPage)) {
		return false;
	}

	return true;
}

void DX12DescriptorPool::dispose()
{
	if (m_descriptorHeapAllocator_CBV_SRV_UAV) {
		m_descriptorHeapAllocator_CBV_SRV_UAV->dispose();
		m_descriptorHeapAllocator_CBV_SRV_UAV = nullptr;
	}
	if (m_descriptorHeapAllocator_SAMPLER) {
		m_descriptorHeapAllocator_SAMPLER->dispose();
		m_descriptorHeapAllocator_SAMPLER = nullptr;
	}
}

void DX12DescriptorPool::reset()
{
	m_descriptorHeapAllocator_CBV_SRV_UAV->reset();
	m_descriptorHeapAllocator_SAMPLER->reset();
}

IDescriptor* DX12DescriptorPool::allocate()
{
	/*
SetGraphicsRootSignature
SetPipelineState

SetDescriptorHeaps
SetGraphicsRootDescriptorTable


CreateConstantBufferView




1. CBO を作る
2. CreateConstantBufferView で DescriptorHeap 内に View を作る。
https://sites.google.com/site/monshonosuana/directxno-hanashi-1/directx-145

- Vulkan のように、XXXXView というデータ構造があるわけではない。
- Heap は ヒープメモリのことではないのでイメージの違いに注意。
  本来の意味の「集合」でイメージする。DescriptorHeap は View の集合と考えてよさそう。
- CBO は Vulkan と同じくコマンドリストごとに必要。実行中は書き換え禁止。


- DescriptorTableは1つで1つのHeap


RootSignature は複数の DescriptorTable を持つ
- ここでいう DescriptorTable と、cmdList->SetGraphicsRootDescriptorTable() は別物。
- DescriptorTable というオブジェクトがあるわけではなく、D3D12_ROOT_DESCRIPTOR_TABLE 構造体で表される。


ROOT_PARAMETER を複数作る必要はある？
D:\Tech\Graphics\DirectX-Graphics-Samples\Libraries\D3D12RaytracingFallback\src\fallbacklayerunittests.cpp



*/
	LN_NOTIMPLEMENTED();
	return 0;
}

} // namespace detail
} // namespace ln
