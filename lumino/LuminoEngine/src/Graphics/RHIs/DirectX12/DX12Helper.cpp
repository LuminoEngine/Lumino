
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

D3D12_BLEND DX12Helper::LNBlendFactorToDX12Blend(BlendFactor value)
{
    static const std::pair<BlendFactor, D3D12_BLEND> table[] = {
        { BlendFactor::Zero, D3D12_BLEND_ZERO },
        { BlendFactor::One, D3D12_BLEND_ONE },
        { BlendFactor::SourceColor, D3D12_BLEND_SRC_COLOR },
        { BlendFactor::InverseSourceColor, D3D12_BLEND_INV_SRC_COLOR },
        { BlendFactor::SourceAlpha, D3D12_BLEND_SRC_ALPHA },
        { BlendFactor::InverseSourceAlpha, D3D12_BLEND_INV_SRC_ALPHA },
        { BlendFactor::DestinationColor, D3D12_BLEND_DEST_COLOR },
        { BlendFactor::InverseDestinationColor, D3D12_BLEND_INV_DEST_COLOR },
        { BlendFactor::DestinationAlpha, D3D12_BLEND_DEST_ALPHA },
        { BlendFactor::InverseDestinationAlpha, D3D12_BLEND_INV_DEST_ALPHA },
    };
    assert(table[(int)value].first == value);
    return table[(int)value].second;
}

D3D12_BLEND_OP DX12Helper::LNBlendOpToDX12Blend(BlendOp value)
{
    static const std::pair<BlendOp, D3D12_BLEND_OP> table[] = {
        { BlendOp::Add, D3D12_BLEND_OP_ADD },
        { BlendOp::Subtract, D3D12_BLEND_OP_SUBTRACT },
        { BlendOp::ReverseSubtract, D3D12_BLEND_OP_REV_SUBTRACT },
        { BlendOp::Min, D3D12_BLEND_OP_MIN },
        { BlendOp::Max, D3D12_BLEND_OP_MAX },
    };
    assert(table[(int)value].first == value);
    return table[(int)value].second;
}

D3D12_FILL_MODE DX12Helper::LNFillModeToDX12FillMode(FillMode value)
{
    static const std::pair<FillMode, D3D12_FILL_MODE> table[] = {
        { FillMode::Solid, D3D12_FILL_MODE_SOLID },
        { FillMode::Wireframe, D3D12_FILL_MODE_WIREFRAME },
    };
    assert(table[(int)value].first == value);
    return table[(int)value].second;
}

D3D12_CULL_MODE DX12Helper::LNCullModeToDX12CullMode(CullMode value)
{
    static const std::pair<CullMode, D3D12_CULL_MODE> table[] = {
        { CullMode::None, D3D12_CULL_MODE_NONE },
        { CullMode::Front, D3D12_CULL_MODE_FRONT },
        { CullMode::Back, D3D12_CULL_MODE_BACK },
    };
    assert(table[(int)value].first == value);
    return table[(int)value].second;
}

D3D12_COMPARISON_FUNC DX12Helper::LNComparisonFuncToDX12ComparisonFunc(ComparisonFunc value)
{
    static const std::pair<ComparisonFunc, D3D12_COMPARISON_FUNC> table[] = {
        { ComparisonFunc::Never, D3D12_COMPARISON_FUNC_NEVER },
        { ComparisonFunc::Less, D3D12_COMPARISON_FUNC_LESS },
        { ComparisonFunc::LessEqual, D3D12_COMPARISON_FUNC_LESS_EQUAL },
        { ComparisonFunc::Greater, D3D12_COMPARISON_FUNC_GREATER },
        { ComparisonFunc::GreaterEqual, D3D12_COMPARISON_FUNC_GREATER_EQUAL },
        { ComparisonFunc::Equal, D3D12_COMPARISON_FUNC_EQUAL },
        { ComparisonFunc::NotEqual, D3D12_COMPARISON_FUNC_NOT_EQUAL },
        { ComparisonFunc::Always, D3D12_COMPARISON_FUNC_ALWAYS },
    };
    assert(table[(int)value].first == value);
    return table[(int)value].second;
}

D3D12_STENCIL_OP DX12Helper::LNStencilOpToDX12StencilOp(StencilOp value)
{
    static const std::pair<StencilOp, D3D12_STENCIL_OP> table[] = {
        { StencilOp::Keep, D3D12_STENCIL_OP_KEEP },
        { StencilOp::Replace, D3D12_STENCIL_OP_REPLACE },
    };
    assert(table[(int)value].first == value);
    return table[(int)value].second;
}

D3D_PRIMITIVE_TOPOLOGY DX12Helper::LNPrimitiveTopologyToDX12PrimitiveTopology(PrimitiveTopology value)
{
    static const std::pair<PrimitiveTopology, D3D_PRIMITIVE_TOPOLOGY> table[] = {
        { PrimitiveTopology::TriangleList, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST },
        { PrimitiveTopology::TriangleStrip, D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP },
        { PrimitiveTopology::TriangleFan, D3D_PRIMITIVE_TOPOLOGY_UNDEFINED },
        { PrimitiveTopology::LineList, D3D_PRIMITIVE_TOPOLOGY_LINELIST },
        { PrimitiveTopology::LineStrip, D3D_PRIMITIVE_TOPOLOGY_LINESTRIP },
        { PrimitiveTopology::PointList, D3D_PRIMITIVE_TOPOLOGY_POINTLIST },
    };
    assert(table[(int)value].first == value);
    return table[(int)value].second;
}

int32_t DX12Helper::getFormatSize(DXGI_FORMAT value)
{
    switch (value)
    {
    case DXGI_FORMAT_R8G8B8A8_UNORM:
        return 4;
    case DXGI_FORMAT_R16G16B16A16_FLOAT:
        return 8;
    case DXGI_FORMAT_R32G32B32A32_FLOAT:
        return 16;
    case DXGI_FORMAT_R16_FLOAT:
        return 2;
    case DXGI_FORMAT_R32_FLOAT:
        return 4;
    case DXGI_FORMAT_R32_SINT:
        return 4;
    default:
        LN_NOTIMPLEMENTED();
        break;
    }
    return 0;
}

bool DX12Helper::getDrawPrimitiveData(PrimitiveTopology primitive, int primitiveCount, D3D_PRIMITIVE_TOPOLOGY* outTopology, UINT* outVertexCount)
{
    switch (primitive)
    {
    case ln::PrimitiveTopology::TriangleList:
        *outTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        *outVertexCount = primitiveCount * 3;
        return true;
    case ln::PrimitiveTopology::TriangleStrip:
        *outTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
        *outVertexCount = 2 + primitiveCount;
        return true;
    case ln::PrimitiveTopology::LineList:
        *outTopology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
        *outVertexCount = primitiveCount * 2;
        return true;
    case ln::PrimitiveTopology::LineStrip:
        *outTopology = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
        *outVertexCount = 1 + primitiveCount;
        return true;
    case ln::PrimitiveTopology::PointList:
        *outTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
        *outVertexCount = primitiveCount;
        return true;
    default:
        LN_UNREACHABLE();
        return false;
    }
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

//==============================================================================
// DX12CommandListCore

DX12CommandListCore::DX12CommandListCore()
{
}

bool DX12CommandListCore::init(DX12Device* device)
{
    ID3D12Device* dxDevice = device->device();

    if (FAILED(dxDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_dxCommandAllocator)))) {
        LN_ERROR("CreateCommandAllocator failed.");
        return false;
    }

    if (FAILED(dxDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_dxCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_dxCommandList)))) {
        LN_ERROR("CreateCommandList failed.");
        return false;
    }

    m_dxCommandList->Close();

    return true;
}

void DX12CommandListCore::dispose()
{
    m_dxCommandList.Reset();
    m_dxCommandAllocator.Reset();
}

bool DX12CommandListCore::reset()
{
    if (FAILED(m_dxCommandAllocator->Reset())) {
        LN_ERROR("Reset failed.");
        return false;
    }

    if (FAILED(m_dxCommandList->Reset(m_dxCommandAllocator.Get(), nullptr))) {
        LN_ERROR("Reset failed.");
        return false;
    }

    return true;
}

bool DX12CommandListCore::close()
{
    if (FAILED(m_dxCommandList->Close())) {
        LN_ERROR("Close failed.");
        return false;
    }

    return true;
}

} // namespace detail
} // namespace ln
