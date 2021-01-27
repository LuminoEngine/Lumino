
#include "Internal.hpp"
#include "DX12DeviceContext.hpp"
#include "DX12Helper.hpp"

namespace ln {
namespace detail {

//=============================================================================
// DX12Helper

static const std::pair<TextureFormat, DXGI_FORMAT> s_textureFormatConversionTable[] =
{
	{ TextureFormat::Unknown, DXGI_FORMAT_UNKNOWN },
	{ TextureFormat::RGBA8, DXGI_FORMAT_R8G8B8A8_UNORM },
	{ TextureFormat::RGB8, DXGI_FORMAT_UNKNOWN },	// TODO: remove
	{ TextureFormat::RGBA16F, DXGI_FORMAT_R16G16B16A16_FLOAT },
	{ TextureFormat::RGBA32F, DXGI_FORMAT_R32G32B32A32_FLOAT },
	{ TextureFormat::R16F, DXGI_FORMAT_R16_FLOAT },
	{ TextureFormat::R32F, DXGI_FORMAT_R32_FLOAT },
	{ TextureFormat::R32S, DXGI_FORMAT_R32_SINT },
};

DXGI_FORMAT DX12Helper::LNTextureFormatToDXFormat(TextureFormat value)
{
	assert(s_textureFormatConversionTable[(int)value].first == value);
	return s_textureFormatConversionTable[(int)value].second;
}

TextureFormat DX12Helper::DXFormatToLNTextureFormat(DXGI_FORMAT value)
{
	for (auto& i : s_textureFormatConversionTable) {
		if (i.second == value) {
			return i.first;
		}
	}
	return TextureFormat::Unknown;
}

DXGI_FORMAT DX12Helper::LNVertexElementTypeToDXFormat(VertexElementType value)
{
	struct VertexElementTypeConversionItem
	{
		VertexElementType lnValue;
		DXGI_FORMAT dxValue;
	};
	static const VertexElementTypeConversionItem s_vertexElementTypeConversionTable[] =
	{
		{ VertexElementType::Unknown, DXGI_FORMAT_UNKNOWN },
		{ VertexElementType::Float1, DXGI_FORMAT_R32_FLOAT },
		{ VertexElementType::Float2, DXGI_FORMAT_R32G32_FLOAT },
		{ VertexElementType::Float3, DXGI_FORMAT_R32G32B32_FLOAT },
		{ VertexElementType::Float4, DXGI_FORMAT_R32G32B32A32_FLOAT },
		{ VertexElementType::Ubyte4, DXGI_FORMAT_R8G8B8A8_UINT },
		{ VertexElementType::Color4, DXGI_FORMAT_R8G8B8A8_UNORM },
		{ VertexElementType::Short2, DXGI_FORMAT_R16G16_SINT },
		{ VertexElementType::Short4, DXGI_FORMAT_R16G16B16A16_SINT },
	};
	assert(s_vertexElementTypeConversionTable[(int)value].lnValue == value);
	return s_vertexElementTypeConversionTable[(int)value].dxValue;
}

const char* DX12Helper::LNVertexElementUsageToSemanticName(VertexElementUsage value)
{
	struct VertexElementUsageConversionItem
	{
		VertexElementUsage lnValue;
		std::string dxValue;
	};
	static const VertexElementUsageConversionItem s_vertexElementUsageConversionTable[] =
	{
		{ VertexElementUsage::Unknown, "" },
		{ VertexElementUsage::Position, "POSITION" },
		{ VertexElementUsage::Normal, "NORMAL" },
		{ VertexElementUsage::Color, "COLOR" },
		{ VertexElementUsage::TexCoord, "TEXCOORD" },
		{ VertexElementUsage::PointSize, "" },
		{ VertexElementUsage::Tangent, "TANGENT" },
		{ VertexElementUsage::Binormal, "BINORMAL" },
		{ VertexElementUsage::BlendIndices, "BLENDINDICES" },
		{ VertexElementUsage::BlendWeight, "BLENDWEIGHT" },
	};
	assert(s_vertexElementUsageConversionTable[(int)value].lnValue == value);
	return s_vertexElementUsageConversionTable[(int)value].dxValue.c_str();
}

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
    desc.NodeMask = device->visibleNodeMask();    // TODO: これでいい？

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
        outHandles->cpuHandles[i].ptr = cpuHandle.ptr + m_handleSize * (i + m_usedOffset);
        outHandles->gpuHandles[i].ptr = gpuHandle.ptr + m_handleSize * (i + m_usedOffset);
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

} // namespace detail
} // namespace ln
