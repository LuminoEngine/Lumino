
#include "Internal.hpp"
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/Platform/PlatformSupport.hpp>
#include "VulkanHelper.hpp"
#include "VulkanDeviceContext.hpp"

namespace ln {
namespace detail {

//=============================================================================
// VulkanHelper

const std::vector<const char*> VulkanHelper::validationLayers =
{
    "VK_LAYER_LUNARG_standard_validation"
};

struct FormatConversionItem
{
	VkFormat vulkanFormat;
	uint32_t bitPerPixel;
	TextureFormat lnFormat;
	bool isCompress;
};

struct BlendFactorConversionItem
{
	BlendFactor lnValue;
	VkBlendFactor vkValueColor;
	VkBlendFactor vkValueAlpha;
};

struct BlendOpConversionItem
{
	BlendOp lnValue;
	VkBlendOp vkValue;
};

struct ComparisonFuncConversionItem
{
	ComparisonFunc lnValue;
	VkCompareOp vkValue;
};

struct FillModeConversionItem
{
	FillMode lnValue;
	VkPolygonMode vkValue;
};

struct CullModeConversionItem
{
	CullMode lnValue;
	VkCullModeFlagBits vkValue;
};

struct StencilOpConversionItem
{
	StencilOp lnValue;
	VkStencilOp vkValue;
};

struct FilterModeConversionItem
{
	TextureFilterMode lnValue;
	VkFilter vkValue;
};

struct AddressModeConversionItem
{
	TextureAddressMode lnValue;
	VkSamplerAddressMode vkValue;
};

struct VertexElementTypeConversionItem
{
	VertexElementType lnValue;
	VkFormat vkValue;
};

static FormatConversionItem s_formatConversionTable[] =
{
	{VK_FORMAT_UNDEFINED, 0, TextureFormat::Unknown, false},
	{VK_FORMAT_R8G8B8A8_UNORM, 32, TextureFormat::RGBA32, false},
	{VK_FORMAT_UNDEFINED, 0, TextureFormat::RGB24, false}, // TODO: remove
	{VK_FORMAT_R16G16B16A16_SFLOAT, 64, TextureFormat::R16G16B16A16Float, false},
	{VK_FORMAT_R32G32B32A32_SFLOAT, 128, TextureFormat::R32G32B32A32Float, false},
	{VK_FORMAT_R16_SFLOAT, 16, TextureFormat::R16Float, false},
	{VK_FORMAT_R32_SFLOAT, 32, TextureFormat::R32Float, false},
	{VK_FORMAT_R32_UINT, 32, TextureFormat::R32UInt, false},
};

static const BlendFactorConversionItem s_blendFactorConversionTable[] =
{
	{BlendFactor::Zero, VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO},
	{BlendFactor::One, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE},
	{BlendFactor::SourceColor, VK_BLEND_FACTOR_SRC_COLOR, VK_BLEND_FACTOR_SRC_ALPHA},
	{BlendFactor::InverseSourceColor, VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA},
	{BlendFactor::SourceAlpha, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_SRC_ALPHA},
	{BlendFactor::InverseSourceAlpha, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA},
	{BlendFactor::DestinationColor, VK_BLEND_FACTOR_DST_COLOR, VK_BLEND_FACTOR_DST_ALPHA},
	{BlendFactor::InverseDestinationColor, VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR, VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA},
	{BlendFactor::DestinationAlpha, VK_BLEND_FACTOR_DST_ALPHA, VK_BLEND_FACTOR_DST_ALPHA},
	{BlendFactor::InverseDestinationAlpha, VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA},
};

static const BlendOpConversionItem s_blendOpConversionTable[] =
{
	{BlendOp::Add, VK_BLEND_OP_ADD},
	{BlendOp::Subtract, VK_BLEND_OP_SUBTRACT},
	{BlendOp::ReverseSubtract, VK_BLEND_OP_REVERSE_SUBTRACT},
	{BlendOp::Min, VK_BLEND_OP_MIN},
	{BlendOp::Max, VK_BLEND_OP_MAX},
};

static const ComparisonFuncConversionItem s_comparisoFuncConversionTable[] =
{
	{ComparisonFunc::Never, VK_COMPARE_OP_NEVER},
	{ComparisonFunc::Less, VK_COMPARE_OP_LESS},
	{ComparisonFunc::LessEqual, VK_COMPARE_OP_LESS_OR_EQUAL},
	{ComparisonFunc::Greater, VK_COMPARE_OP_GREATER},
	{ComparisonFunc::GreaterEqual, VK_COMPARE_OP_GREATER_OR_EQUAL},
	{ComparisonFunc::Equal, VK_COMPARE_OP_EQUAL},
	{ComparisonFunc::NotEqual, VK_COMPARE_OP_NOT_EQUAL},
	{ComparisonFunc::Always, VK_COMPARE_OP_ALWAYS},
};

static const FillModeConversionItem s_fillModeConversionTable[] =
{
	{FillMode::Solid, VK_POLYGON_MODE_FILL},
	{FillMode::Wireframe, VK_POLYGON_MODE_LINE},
};

static const CullModeConversionItem s_cullModeConversionTable[] =
{
	{CullMode::None, VK_CULL_MODE_NONE},
	{CullMode::Front, VK_CULL_MODE_FRONT_BIT},
	{CullMode::Back, VK_CULL_MODE_BACK_BIT},
};

static const StencilOpConversionItem s_stencilOpConversionTable[] =
{
	{StencilOp::Keep, VK_STENCIL_OP_KEEP},
	{StencilOp::Replace, VK_STENCIL_OP_REPLACE},
};

static const FilterModeConversionItem s_filterModeConversionTable[] =
{
	{TextureFilterMode::Point, VK_FILTER_NEAREST},
	{TextureFilterMode::Linear, VK_FILTER_LINEAR},
};

static const AddressModeConversionItem s_addressModeConversionTable[] =
{
	{TextureAddressMode::Repeat, VK_SAMPLER_ADDRESS_MODE_REPEAT},
	{TextureAddressMode::Clamp, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE},
};

static const VertexElementTypeConversionItem s_vertexElementTypeConversionTable[] =
{
	{VertexElementType::Unknown, VK_FORMAT_UNDEFINED},

	{VertexElementType::Float1, VK_FORMAT_R32_SFLOAT},
	{VertexElementType::Float2, VK_FORMAT_R32G32_SFLOAT},
	{VertexElementType::Float3, VK_FORMAT_R32G32B32_SFLOAT},
	{VertexElementType::Float4, VK_FORMAT_R32G32B32A32_SFLOAT},

	{VertexElementType::Ubyte4, VK_FORMAT_R8G8B8A8_UINT},
	{VertexElementType::Color4, VK_FORMAT_R8G8B8A8_UNORM}, // UNORM : https://msdn.microsoft.com/ja-jp/library/ee415736%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396

	{VertexElementType::Short2, VK_FORMAT_R16G16_SINT},
	{VertexElementType::Short4, VK_FORMAT_R16G16B16A16_SINT},
};

VkFormat VulkanHelper::LNFormatToVkFormat(TextureFormat format)
{
	assert(s_formatConversionTable[(int)format].lnFormat == format);
	return s_formatConversionTable[(int)format].vulkanFormat;
}

TextureFormat VulkanHelper::VkFormatToLNFormat(VkFormat format)
{
	for (auto& i : s_formatConversionTable) {
		if (i.vulkanFormat == format) {
			return i.lnFormat;
		}
	}
	return TextureFormat::Unknown;
}

VkBlendFactor VulkanHelper::LNBlendFactorToVkBlendFactor_Color(BlendFactor value)
{
	assert(s_blendFactorConversionTable[(int)value].lnValue == value);
	return s_blendFactorConversionTable[(int)value].vkValueColor;
}

VkBlendFactor VulkanHelper::LNBlendFactorToVkBlendFactor_Alpha(BlendFactor value)
{
	assert(s_blendFactorConversionTable[(int)value].lnValue == value);
	return s_blendFactorConversionTable[(int)value].vkValueAlpha;
}

VkBlendOp VulkanHelper::LNBlendOpToVkBlendOp(BlendOp value)
{
	assert(s_blendOpConversionTable[(int)value].lnValue == value);
	return s_blendOpConversionTable[(int)value].vkValue;
}

VkCompareOp VulkanHelper::LNComparisonFuncToVkCompareOp(ComparisonFunc value)
{
	assert(s_comparisoFuncConversionTable[(int)value].lnValue == value);
	return s_comparisoFuncConversionTable[(int)value].vkValue;
}

VkPolygonMode VulkanHelper::LNFillModeToVkPolygonMode(FillMode value)
{
	assert(s_fillModeConversionTable[(int)value].lnValue == value);
	return s_fillModeConversionTable[(int)value].vkValue;
}

VkCullModeFlagBits VulkanHelper::LNCullModeToVkCullMode(CullMode value)
{
	assert(s_cullModeConversionTable[(int)value].lnValue == value);
	return s_cullModeConversionTable[(int)value].vkValue;
}

VkStencilOp VulkanHelper::LNStencilOpToVkStencilOp(StencilOp value)
{
	assert(s_stencilOpConversionTable[(int)value].lnValue == value);
	return s_stencilOpConversionTable[(int)value].vkValue;
}

VkFilter VulkanHelper::LNTextureFilterModeToVkFilter(TextureFilterMode value)
{
	assert(s_filterModeConversionTable[(int)value].lnValue == value);
	return s_filterModeConversionTable[(int)value].vkValue;
}

VkSamplerAddressMode VulkanHelper::LNTextureAddressModeModeToVkSamplerAddressMode(TextureAddressMode value)
{
	assert(s_addressModeConversionTable[(int)value].lnValue == value);
	return s_addressModeConversionTable[(int)value].vkValue;
}

VkFormat VulkanHelper::LNVertexElementTypeToVkFormat(VertexElementType value)
{
	assert(s_vertexElementTypeConversionTable[(int)value].lnValue == value);
	return s_vertexElementTypeConversionTable[(int)value].vkValue;
}

const char* VulkanHelper::getVkResultName(VkResult result)
{
#define VK_RESULT_VALUE(v) case v: return #v
    switch (result)
    {
        VK_RESULT_VALUE(VK_SUCCESS);
        VK_RESULT_VALUE(VK_NOT_READY);
        VK_RESULT_VALUE(VK_TIMEOUT);
        VK_RESULT_VALUE(VK_EVENT_SET);
        VK_RESULT_VALUE(VK_EVENT_RESET);
        VK_RESULT_VALUE(VK_INCOMPLETE);	// and VK_RESULT_END_RANGE
        VK_RESULT_VALUE(VK_ERROR_OUT_OF_HOST_MEMORY);
        VK_RESULT_VALUE(VK_ERROR_OUT_OF_DEVICE_MEMORY);
        VK_RESULT_VALUE(VK_ERROR_INITIALIZATION_FAILED);
        VK_RESULT_VALUE(VK_ERROR_DEVICE_LOST);
        VK_RESULT_VALUE(VK_ERROR_MEMORY_MAP_FAILED);
        VK_RESULT_VALUE(VK_ERROR_LAYER_NOT_PRESENT);
        VK_RESULT_VALUE(VK_ERROR_EXTENSION_NOT_PRESENT);
        VK_RESULT_VALUE(VK_ERROR_FEATURE_NOT_PRESENT);
        VK_RESULT_VALUE(VK_ERROR_INCOMPATIBLE_DRIVER);
        VK_RESULT_VALUE(VK_ERROR_TOO_MANY_OBJECTS);
        VK_RESULT_VALUE(VK_ERROR_FORMAT_NOT_SUPPORTED);
        VK_RESULT_VALUE(VK_ERROR_FRAGMENTED_POOL);			// and VK_RESULT_BEGIN_RANGE
        VK_RESULT_VALUE(VK_ERROR_OUT_OF_POOL_MEMORY);		// and VK_ERROR_OUT_OF_POOL_MEMORY_KHR
        VK_RESULT_VALUE(VK_ERROR_INVALID_EXTERNAL_HANDLE);	// and VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR
        VK_RESULT_VALUE(VK_ERROR_SURFACE_LOST_KHR);
        VK_RESULT_VALUE(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR);
        VK_RESULT_VALUE(VK_SUBOPTIMAL_KHR);
        VK_RESULT_VALUE(VK_ERROR_OUT_OF_DATE_KHR);
        VK_RESULT_VALUE(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR);
        VK_RESULT_VALUE(VK_ERROR_VALIDATION_FAILED_EXT);
        VK_RESULT_VALUE(VK_ERROR_INVALID_SHADER_NV);
        //VK_RESULT_VALUE(VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT);
        VK_RESULT_VALUE(VK_ERROR_FRAGMENTATION_EXT);
        VK_RESULT_VALUE(VK_ERROR_NOT_PERMITTED_EXT);
        //VK_RESULT_VALUE(VK_ERROR_INVALID_DEVICE_ADDRESS_EXT);
        VK_RESULT_VALUE(VK_RESULT_RANGE_SIZE);
        VK_RESULT_VALUE(VK_RESULT_MAX_ENUM);
    }
#undef VK_RESULT_VALUE
    return "<Unkonwn VkResult>";
}

bool VulkanHelper::checkValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

//=============================================================================
// AbstractVulkanAllocator

VKAPI_ATTR
void* VKAPI_CALL AllocCallback(
	void* pUserData,
	size_t size,
	size_t alignment,
	VkSystemAllocationScope scope)
{
	AbstractVulkanAllocator* allocator = reinterpret_cast<AbstractVulkanAllocator*>(pUserData);
	return allocator->alloc(size, alignment, scope);
}

VKAPI_ATTR
void* VKAPI_CALL ReallocCallback(
	void* pUserData,
	void* pOriginal,
	size_t size,
	size_t alignment,
	VkSystemAllocationScope scope)
{
	AbstractVulkanAllocator* allocator = reinterpret_cast<AbstractVulkanAllocator*>(pUserData);
	return allocator->realloc(pOriginal, size, alignment, scope);
}

VKAPI_ATTR
void VKAPI_CALL FreeCallback(void* pUserData, void* pMemory)
{
	AbstractVulkanAllocator* allocator = reinterpret_cast<AbstractVulkanAllocator*>(pUserData);
	return allocator->free(pMemory);
}

AbstractVulkanAllocator::AbstractVulkanAllocator()
	: m_allocatorCallbacks()
{
}

Result AbstractVulkanAllocator::init()
{
	m_allocatorCallbacks.pfnAllocation = AllocCallback;
	m_allocatorCallbacks.pfnFree = FreeCallback;
	m_allocatorCallbacks.pfnReallocation = ReallocCallback;
	m_allocatorCallbacks.pfnInternalAllocation = nullptr;
	m_allocatorCallbacks.pfnInternalFree = nullptr;
	m_allocatorCallbacks.pUserData = this;
	return true;
}

//=============================================================================
// VulkanAllocator

VulkanAllocator::VulkanAllocator()
	: m_counter(0)
	, m_allocationSize{}
{
}

Result VulkanAllocator::init()
{
	return AbstractVulkanAllocator::init();
}

void* VulkanAllocator::alloc(size_t size, size_t alignment, VkSystemAllocationScope scope) noexcept
{
	m_counter++;
	m_allocationSize[scope] -= size;
#ifdef LN_OS_WIN32
	return _aligned_malloc(size, alignment);
#else
	return aligned_alloc(alignment, size);
#endif
}

void* VulkanAllocator::realloc(void* ptr, size_t size, size_t alignment, VkSystemAllocationScope scope) noexcept
{
	m_counter++;
#ifdef LN_OS_WIN32
	return _aligned_realloc(ptr, size, alignment);
#else
	A3D_UNUSED(alignment);
	return realloc(ptr, size);
#endif
}

void VulkanAllocator::free(void* ptr) noexcept
{
	m_counter--;
#ifdef LN_OS_WIN32
	_aligned_free(ptr);
#else
	free(ptr);
#endif
}

//=============================================================================
// VulkanLinearAllocator

VulkanLinearAllocator::VulkanLinearAllocator()
	: m_linearAllocator(nullptr)
{
}

Result VulkanLinearAllocator::init()
{
	return AbstractVulkanAllocator::init();
}

void* VulkanLinearAllocator::alloc(size_t size, size_t alignment, VkSystemAllocationScope scope) noexcept
{
	// TODO: alignment
	void* ptr = m_linearAllocator->allocate(size);
	assert(((size_t)ptr) % alignment == 0);
	return ptr;
}

void* VulkanLinearAllocator::realloc(void* ptr, size_t size, size_t alignment, VkSystemAllocationScope scope) noexcept
{
	// TODO: NotImplemented
	assert(0);
	return nullptr;
}

void VulkanLinearAllocator::free(void* ptr) noexcept
{
	// フレーム終了時にすべてクリアされるため不要
}

//=============================================================================
// VulkanBuffer

VulkanBuffer::VulkanBuffer()
    : m_deviceContext(nullptr)
    , m_buffer(VK_NULL_HANDLE)
    , m_bufferMemory(VK_NULL_HANDLE)
    , m_size(0)
{
}

Result VulkanBuffer::init(VulkanDeviceContext* deviceContext, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
	if (LN_REQUIRE(deviceContext)) return false;
	m_deviceContext = deviceContext;

	if (!resetBuffer(size, usage)) {
		return false;
	}
	if (!resetMemoryBuffer(properties, m_deviceContext->vulkanAllocator())) {
		return false;
	}

    return true;
}

Result VulkanBuffer::init(VulkanDeviceContext* deviceContext)
{
	if (LN_REQUIRE(deviceContext)) return false;
	m_deviceContext = deviceContext;
    return true;
}

Result VulkanBuffer::resetBuffer(VkDeviceSize size, VkBufferUsageFlags usage)
{
	if (m_buffer) {
		vkDestroyBuffer(m_deviceContext->vulkanDevice(), m_buffer, m_deviceContext->vulkanAllocator());
		m_buffer = VK_NULL_HANDLE;
	}

	m_size = size;

	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	LN_VK_CHECK(vkCreateBuffer(m_deviceContext->vulkanDevice(), &bufferInfo, m_deviceContext->vulkanAllocator(), &m_buffer));

	return true;
}

Result VulkanBuffer::resetMemoryBuffer(VkMemoryPropertyFlags properties, const VkAllocationCallbacks* allocator)
{
	if (m_bufferMemory) {
		vkFreeMemory(m_deviceContext->vulkanDevice(), m_bufferMemory, allocator);
		m_bufferMemory = VK_NULL_HANDLE;
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(m_deviceContext->vulkanDevice(), m_buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	m_deviceContext->findMemoryType(memRequirements.memoryTypeBits, properties, &allocInfo.memoryTypeIndex);

	LN_VK_CHECK(vkAllocateMemory(m_deviceContext->vulkanDevice(), &allocInfo, allocator, &m_bufferMemory));

	LN_VK_CHECK(vkBindBufferMemory(m_deviceContext->vulkanDevice(), m_buffer, m_bufferMemory, 0));

	return true;
}

void VulkanBuffer::dispose()
{
    if (m_buffer) {
        vkDestroyBuffer(m_deviceContext->vulkanDevice(), m_buffer, m_deviceContext->vulkanAllocator());
        m_buffer = VK_NULL_HANDLE;
    }

    if (m_bufferMemory) {
        vkFreeMemory(m_deviceContext->vulkanDevice(), m_bufferMemory, m_deviceContext->vulkanAllocator());
        m_bufferMemory = VK_NULL_HANDLE;
    }
}

void* VulkanBuffer::map()
{
    void* mapped;
    if (vkMapMemory(m_deviceContext->vulkanDevice(), m_bufferMemory, 0, m_size, 0, &mapped) != VK_SUCCESS) {
        LN_LOG_ERROR << "Failed vkMapMemory";
        return nullptr;
    }
    return mapped;
}

void VulkanBuffer::unmap()
{
    vkUnmapMemory(m_deviceContext->vulkanDevice(), m_bufferMemory);
}

void VulkanBuffer::setData(size_t offset, const void* data, VkDeviceSize size)
{
    if (LN_REQUIRE(data)) return;
    if (LN_REQUIRE((offset + size) <= m_size)) return;

    if (auto mapped = reinterpret_cast<unsigned char*>(map())) {
        memcpy(mapped + offset, data, size);
        unmap();
    }
}

//=============================================================================
// VulkanImage

VulkanImage::VulkanImage()
{
}

Result VulkanImage::init(VulkanDeviceContext* deviceContext, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageAspectFlags aspectFlags)
{
	LN_DCHECK(deviceContext);
	m_deviceContext = deviceContext;
    m_externalManagement = false;
    //m_width = width;
    //m_height = height;
    m_format = format;

	VkDevice device = m_deviceContext->vulkanDevice();

	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	LN_VK_CHECK(vkCreateImage(device, &imageInfo, m_deviceContext->vulkanAllocator(), &m_image));

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device, m_image, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;

	m_deviceContext->findMemoryType(memRequirements.memoryTypeBits, properties, &allocInfo.memoryTypeIndex);

	LN_VK_CHECK(vkAllocateMemory(device, &allocInfo, m_deviceContext->vulkanAllocator(), &m_imageMemory));

    {
        vkBindImageMemory(device, m_image, m_imageMemory, 0);

        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = m_image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        LN_VK_CHECK(vkCreateImageView(device, &viewInfo, m_deviceContext->vulkanAllocator(), &m_imageView));
    }

	return true;
}

Result VulkanImage::init(VulkanDeviceContext* deviceContext/*, uint32_t width, uint32_t height*/, VkFormat format, VkImage image, VkImageView imageView)
{
    LN_DCHECK(deviceContext);
    m_externalManagement = true;
    //m_width = width;
    //m_height = height;
    m_deviceContext = deviceContext;
    m_format = format;
    m_image = image;
    m_imageView = imageView;
    return true;
}

void VulkanImage::dispose()
{
    if (!m_externalManagement) {
        if (m_imageView) {
            vkDestroyImageView(m_deviceContext->vulkanDevice(), m_imageView, m_deviceContext->vulkanAllocator());
            m_imageView = VK_NULL_HANDLE;
        }
        if (m_image) {
            vkDestroyImage(m_deviceContext->vulkanDevice(), m_image, m_deviceContext->vulkanAllocator());
            m_image = VK_NULL_HANDLE;
        }
        if (m_imageMemory) {
            vkFreeMemory(m_deviceContext->vulkanDevice(), m_imageMemory, m_deviceContext->vulkanAllocator());
            m_imageMemory = VK_NULL_HANDLE;
        }
    }
}

//=============================================================================
// VulkanCommandBuffer

VulkanCommandBuffer::VulkanCommandBuffer()
{
}

Result VulkanCommandBuffer::init(VulkanDeviceContext* deviceContext)
{
	if (LN_REQUIRE(deviceContext)) return false;
    m_deviceContext = deviceContext;

	if (!m_vulkanAllocator.init()) {
		return false;
	}

	// ひとまず 16MB (100万頂点くらいでの見積)
	// 1ページは、更新したいバッファ全体が乗るサイズになっていればよい。
	// もしあふれる場合は一度 LinearAllocator の LargePage 扱いにして、
	// 次のフレームに移る前にページサイズを大きくして LinearAllocator を作り直す。
	// ただ、普通動的なバッファ更新でこんなに大きなサイズを使うことはないような気もする。
	// なお、静的なバッファの場合は init 時に malloc でメモリをとるようにしているので LinearAllocator は関係ない。
	resetAllocator(LinearAllocatorPageManager::DefaultPageSize);

	m_stagingBufferPoolUsed = 0;
	glowStagingBufferPool();

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_deviceContext->vulkanCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    LN_VK_CHECK(vkAllocateCommandBuffers(m_deviceContext->vulkanDevice(), &allocInfo, &m_commandBuffer));

	return true;
}

void VulkanCommandBuffer::dispose()
{
    if (m_commandBuffer) {
        vkFreeCommandBuffers(m_deviceContext->vulkanDevice(), m_deviceContext->vulkanCommandPool(), 1, &m_commandBuffer);
        m_commandBuffer = VK_NULL_HANDLE;
    }
}

Result VulkanCommandBuffer::beginRecording()
{
    m_linearAllocator->cleanup();

    // 前回の描画で使ったリソースを開放する。
    // end で解放しないのは、まだその後の実際のコマンド実行で使うリソースであるから。
    cleanInFlightResources();

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

    LN_VK_CHECK(vkBeginCommandBuffer(vulkanCommandBuffer(), &beginInfo));

    return true;
}

Result VulkanCommandBuffer::endRecording()
{
    LN_VK_CHECK(vkEndCommandBuffer(vulkanCommandBuffer()));

    for (auto& pass : m_usingShaderPasses) {
        pass->recodingPool = nullptr;
    }

    return true;
}

Result VulkanCommandBuffer::allocateDescriptorSets(VulkanShaderPass* shaderPass, std::array<VkDescriptorSet, DescriptorType_Count>* outSets)
{
    LN_DCHECK(shaderPass);
    
    if (!shaderPass->recodingPool) {
        // null の場合は begin からここまでではじめて CommandBuffer で使われた、ということで新しく作る
        auto pool = shaderPass->getDescriptorSetsPool();
        m_usingDescriptorSetsPools.push_back(pool);
        shaderPass->recodingPool = pool;
        m_usingShaderPasses.push_back(shaderPass);
    }

    return shaderPass->recodingPool->allocateDescriptorSets(this, outSets);
}

VulkanBuffer* VulkanCommandBuffer::cmdCopyBuffer(size_t size, VulkanBuffer* destination)
{
	if (m_stagingBufferPoolUsed >= m_stagingBufferPool.size()) {
		glowStagingBufferPool();
	}

	VulkanBuffer* buffer = &m_stagingBufferPool[m_stagingBufferPoolUsed];
	m_stagingBufferPoolUsed++;

	// できるだけ毎回オブジェクトを再構築するのは避けたいので、サイズが小さい時だけにしてみる
	if (buffer->size() < size) {
		buffer->resetBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
	}

	// LinearAllocator からメモリ確保
	buffer->resetMemoryBuffer(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_vulkanAllocator.vulkanAllocator());

	// コマンドバッファに乗せる
	VkBufferCopy copyRegion = {};
	copyRegion.size = size;
	vkCmdCopyBuffer(m_commandBuffer, buffer->vulkanBuffer(), destination->vulkanBuffer(), 1, &copyRegion);

	// 戻り先で書いてもらう
	return buffer;
}

void VulkanCommandBuffer::cleanInFlightResources()
{
    for (auto& pool : m_usingDescriptorSetsPools) {
        pool->owner()->releaseDescriptorSetsPool(pool);
    }
    m_usingShaderPasses.clear();
    m_usingDescriptorSetsPools.clear();

    m_stagingBufferPoolUsed = 0;
}

void VulkanCommandBuffer::resetAllocator(size_t pageSize)
{
	m_linearAllocatorManager = makeRef<LinearAllocatorPageManager>(pageSize);
	m_linearAllocator = makeRef<LinearAllocator>(m_linearAllocatorManager);
	m_vulkanAllocator.setLinearAllocator(m_linearAllocator);
}

Result VulkanCommandBuffer::glowStagingBufferPool()
{
	size_t oldSize = 0;
	size_t newSize = m_stagingBufferPool.empty() ? 64 : m_stagingBufferPool.size() * 2;

    m_stagingBufferPool.resize(newSize);
	for (size_t i = oldSize; i < newSize; i++) {
		if (!m_stagingBufferPool[i].init(m_deviceContext)) {
			return false;
		}
	}

	return true;
}

//=============================================================================
// VulkanDescriptorSetsPool

VulkanDescriptorSetsPool::VulkanDescriptorSetsPool()
{

}

Result VulkanDescriptorSetsPool::init(VulkanDeviceContext* deviceContext, VulkanShaderPass* owner)
{
    LN_DCHECK(deviceContext);
    LN_DCHECK(owner);
    m_deviceContext = deviceContext;
    m_owner = owner;

    uint32_t count = 32;    // TODO:

    std::array<VkDescriptorPoolSize, DescriptorType_Count> poolSizes;
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = count;
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;//VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = count;
    poolSizes[2].type = VK_DESCRIPTOR_TYPE_SAMPLER;//VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
    poolSizes[2].descriptorCount = count;

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.maxSets = count;// static_cast<uint32_t>(poolSizes.size());//static_cast<uint32_t>(swapChainImages.size());
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();

    LN_VK_CHECK(vkCreateDescriptorPool(m_deviceContext->vulkanDevice(), &poolInfo, m_deviceContext->vulkanAllocator(), &m_descriptorPool));

	return true;
}

void VulkanDescriptorSetsPool::dispose()
{
    if (m_descriptorPool) {
        vkDestroyDescriptorPool(m_deviceContext->vulkanDevice(), m_descriptorPool, m_deviceContext->vulkanAllocator());
        m_descriptorPool = VK_NULL_HANDLE;
    }
}

Result VulkanDescriptorSetsPool::allocateDescriptorSets(VulkanCommandBuffer* commandBuffer, std::array<VkDescriptorSet, DescriptorType_Count>* sets)
{
    VkDescriptorSetAllocateInfo allocInfo;
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.descriptorSetCount = m_owner->descriptorSetLayouts().size();
    allocInfo.pSetLayouts = m_owner->descriptorSetLayouts().data();

    // TODO: 使い切ったときの Page 追加
    LN_VK_CHECK(vkAllocateDescriptorSets(m_deviceContext->vulkanDevice(), &allocInfo, sets->data()));

    const std::vector<VkWriteDescriptorSet>& writeInfos = m_owner->submitDescriptorWriteInfo(commandBuffer, *sets);

    vkUpdateDescriptorSets(m_deviceContext->vulkanDevice(), static_cast<uint32_t>(writeInfos.size()), writeInfos.data(), 0, nullptr);

    return true;
}

void VulkanDescriptorSetsPool::reset()
{
    vkResetDescriptorPool(m_deviceContext->vulkanDevice(), m_descriptorPool, 0);
}

//uint32_t VulkanDescriptorSetCache::computeHash(const DescriptorLayout& layoutInfo)
//{
//	MixHash hash;
//	for (int i = 0; i < DescriptorType_Count; i++) {
//		auto& items = layoutInfo.getLayoutItems((DescriptorType)i);
//		for (auto& item : items) {
//			hash.add(item.stageFlags);
//			hash.add(item.binding);
//			hash.add(item.size);
//		}
//	}
//	return hash.value();
//}

//=============================================================================
// VulkanRenderPassCache

VulkanRenderPassCache::VulkanRenderPassCache()
{
}

Result VulkanRenderPassCache::init(VulkanDeviceContext* deviceContext)
{
    LN_DCHECK(deviceContext);
    m_deviceContext = deviceContext;
    VkDevice device = m_deviceContext->vulkanDevice();


    return true;
}

void VulkanRenderPassCache::dispose()
{
    clear();
}

VkRenderPass VulkanRenderPassCache::findOrCreate(const DeviceFramebufferState& state)
{
    uint64_t hash = computeHash(state);
    VkRenderPass renderPass = VK_NULL_HANDLE;
    if (find(hash, &renderPass)) {
        return renderPass;
    }
    else {

        // MaxRenderTargets + 1枚の depthbuffer
        VkAttachmentDescription attachmentDescs[MaxMultiRenderTargets/* + 1*/] = {};
        VkAttachmentReference attachmentRefs[MaxMultiRenderTargets/* + 1*/] = {};
        VkAttachmentReference* depthAttachmentRef = nullptr;
        int attachmentCount = 0;
        int colorAttachmentCount = 0;

        for (int i = 0; i < MaxMultiRenderTargets; i++) {
            if (state.renderTargets[i]) {
                attachmentDescs[i].flags = 0;
                attachmentDescs[i].format = static_cast<VulkanTexture*>(state.renderTargets[i])->image()->vulkanFormat();//VulkanHelper::LNFormatToVkFormat(state.renderTargets[i]->getTextureFormat());
                attachmentDescs[i].samples = VK_SAMPLE_COUNT_1_BIT;
                //attachmentDescs[i].loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                attachmentDescs[i].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;// サンプルでは画面全体 clear する前提なので、前回値を保持する必要はない。そのため CLEAR。というか、CLEAR 指定しないと clear しても背景真っ黒になった。
                attachmentDescs[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
                attachmentDescs[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;    // TODO: stencil。今は未対応
                attachmentDescs[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;//VK_ATTACHMENT_STORE_OP_STORE;    // TODO: stencil。今は未対応
                if (1) {// TODO:
                    // swapchain の場合
                    attachmentDescs[i].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;     // レンダリング前のレイアウト定義。UNDEFINED はレイアウトは何でもよいが、内容の保証はない。サンプルでは全体 clear するので問題ない。
                    attachmentDescs[i].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
                }
                else {
                    attachmentDescs[i].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                    attachmentDescs[i].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                }

                attachmentRefs[i].attachment = attachmentCount;
                attachmentRefs[i].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                attachmentCount++;
                colorAttachmentCount++;
            }
            else {
                break;
            }
        }

        if (state.depthBuffer) {
            int i = colorAttachmentCount;

            attachmentDescs[i].flags = 0;
            attachmentDescs[i].format = m_deviceContext->findDepthFormat();//VK_FORMAT_D32_SFLOAT_S8_UINT; 
            attachmentDescs[i].samples = VK_SAMPLE_COUNT_1_BIT;
            attachmentDescs[i].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;// VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            attachmentDescs[i].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;// VK_ATTACHMENT_STORE_OP_STORE;
            attachmentDescs[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            attachmentDescs[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;// VK_ATTACHMENT_STORE_OP_STORE;
            attachmentDescs[i].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;// VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            attachmentDescs[i].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            attachmentRefs[i].attachment = attachmentCount;
            attachmentRefs[i].layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            depthAttachmentRef = &attachmentRefs[i];
            attachmentCount++;
        }

        VkSubpassDescription subpass;
        subpass.flags = 0;
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.inputAttachmentCount = 0;
        subpass.pInputAttachments = nullptr;
        subpass.colorAttachmentCount = colorAttachmentCount;
        subpass.pColorAttachments = attachmentRefs;
        subpass.pResolveAttachments = nullptr;
        subpass.pDepthStencilAttachment = depthAttachmentRef;
        subpass.preserveAttachmentCount = 0;
        subpass.pPreserveAttachments = nullptr;

        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.pNext = nullptr;
        renderPassInfo.flags = 0;
        renderPassInfo.attachmentCount = attachmentCount;
        renderPassInfo.pAttachments = attachmentDescs;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        LN_VK_CHECK(vkCreateRenderPass(m_deviceContext->vulkanDevice(), &renderPassInfo, m_deviceContext->vulkanAllocator(), &renderPass));

        add(hash, renderPass);
        return renderPass;

#if 0
        std::array<VkAttachmentDescription, 2> colorAttachment = {};
        colorAttachment[0].format = swapChainImageFormat;
        colorAttachment[0].samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;   // サンプルでは画面全体 clear する前提なので、前回値を保持する必要はない。そのため CLEAR。というか、CLEAR 指定しないと clear しても背景真っ黒になった。
        //colorAttachment[0].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;   // 
        colorAttachment[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;    // TODO: stencil。今は未対応
        colorAttachment[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;  // TODO: stencil。今は未対応
        colorAttachment[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;      // レンダリング前のレイアウト定義。UNDEFINED はレイアウトは何でもよいが、内容の保証はない。サンプルでは全体 clear するので問題ない。
        colorAttachment[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;  // レンダリング後に自動遷移するレイアウト。スワップチェインはこれ。
        //colorAttachment[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;  // レンダリング後に自動遷移するレイアウト。普通のレンダーターゲットはこれ。

        colorAttachment[1].format = swapChainImageFormat;
        colorAttachment[1].samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;   // サンプルでは画面全体 clear する前提なので、前回値を保持する必要はない。そのため CLEAR。というか、CLEAR 指定しないと clear しても背景真っ黒になった。
        colorAttachment[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;    // TODO: stencil。今は未対応
        colorAttachment[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;  // TODO: stencil。今は未対応
        colorAttachment[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;      // レンダリング前のレイアウト定義。UNDEFINED はレイアウトは何でもよいが、内容の保証はない。サンプルでは全体 clear するので問題ない。
        colorAttachment[1].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;  // レンダリング後に自動遷移するレイアウト。普通のレンダーターゲットはこれ。


        VkAttachmentDescription depthAttachment = {};
        depthAttachment.format = m_deviceContext->findDepthFormat();
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        // https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Render_passes
        // TODO: ポストエフェクト処理の最適化として考えてみてもいいかもしれない。
        std::array<VkAttachmentReference, 2> colorAttachmentRef = {};
        VkAttachmentReference depthAttachmentRef = {};
        VkSubpassDescription subpass = {};
        {
            colorAttachmentRef[0].attachment = 0;
            colorAttachmentRef[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            colorAttachmentRef[1].attachment = 2;
            colorAttachmentRef[1].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            depthAttachmentRef.attachment = 1;
            depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpass.colorAttachmentCount = 1;//;colorAttachmentRef.size();
            subpass.pColorAttachments = colorAttachmentRef.data();
            subpass.pDepthStencilAttachment = &depthAttachmentRef;
        }

        // 今は subpass 1 個なのであまり関係はないが、前後の subpass に対してどんなアクションが完了するまで待つべきかという指定を行う。
        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        std::array<VkAttachmentDescription, 3> attachments = { colorAttachment[0], depthAttachment/*, colorAttachment[1]*/ };
        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        LN_VK_CHECK(vkCreateRenderPass(device, &renderPassInfo, m_deviceContext->vulkanAllocator(), &m_renderPass));
#endif
    }
}

void VulkanRenderPassCache::onInvalidate(VkRenderPass value)
{
    if (value) {
        vkDestroyRenderPass(m_deviceContext->vulkanDevice(), value, m_deviceContext->vulkanAllocator());
    }
}

uint64_t VulkanRenderPassCache::computeHash(const DeviceFramebufferState& state)
{
    MixHash hash;
    hash.add(state.renderTargets.size());
    for (size_t i = 0; i < state.renderTargets.size(); i++) {
        if (state.renderTargets[i]) {
            hash.add(static_cast<VulkanTexture*>(state.renderTargets[i])->image()->vulkanFormat());
        }
    }
    if (state.depthBuffer) {
        hash.add(state.depthBuffer->format());
    }
    return hash.value();
}

//=============================================================================
// VulkanFrameBuffer

VulkanFrameBuffer::VulkanFrameBuffer()
{
}

Result VulkanFrameBuffer::init(VulkanDeviceContext* deviceContext, const DeviceFramebufferState& state)
{
    m_deviceContext = deviceContext;
    //m_renderTargetCount = state.renderTargets.size();
    for (size_t i = 0; i < state.renderTargets.size(); i++) {
        m_renderTargets[i] = state.renderTargets[i];
    }
    m_depthBuffer = state.depthBuffer;

    m_renderPass = deviceContext->renderPassCache()->findOrCreate(state);
    if (m_renderPass == VK_NULL_HANDLE) {
        return false;
    }
    else
    {
        VkImageView attachments[MaxMultiRenderTargets + 1] = {};
        int attachmentsCount = 0;
        for (size_t i = 0; i < m_renderTargets.size(); i++) {
            if (m_renderTargets[i]) {
                attachments[attachmentsCount] = static_cast<VulkanTexture*>(m_renderTargets[i])->image()->vulkanImageView();
                attachmentsCount++;
            }
        }
        if (m_depthBuffer) {
            attachments[attachmentsCount] = static_cast<VulkanDepthBuffer*>(m_depthBuffer)->image()->vulkanImageView();
            attachmentsCount++;
        }

        SizeI size = m_renderTargets[0]->realSize();

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.pNext = nullptr;
        framebufferInfo.flags = 0;
        framebufferInfo.renderPass = m_renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = size.width;
        framebufferInfo.height = size.height;
        framebufferInfo.layers = 1;

        LN_VK_CHECK(vkCreateFramebuffer(m_deviceContext->vulkanDevice(), &framebufferInfo, m_deviceContext->vulkanAllocator(), &m_framebuffer));

        return true;
    }
}

void VulkanFrameBuffer::dispose()
{
    if (m_framebuffer) {
        vkDestroyFramebuffer(m_deviceContext->vulkanDevice(), m_framebuffer, m_deviceContext->vulkanAllocator());
        m_framebuffer = 0;
    }
}

bool VulkanFrameBuffer::containsRenderTarget(ITexture* renderTarget) const
{
    for (auto& i : m_renderTargets) {
        if (i == renderTarget) {
            return true;
        }
    }
    return false;
}

bool VulkanFrameBuffer::containsDepthBuffer(IDepthBuffer* depthBuffer) const
{
    return m_depthBuffer == depthBuffer;
}


//=============================================================================
// VulkanFramebufferCache

VulkanFramebufferCache::VulkanFramebufferCache()
{
}

Result VulkanFramebufferCache::init(VulkanDeviceContext* deviceContext)
{
    LN_DCHECK(deviceContext);
    m_deviceContext = deviceContext;
    return true;
}

void VulkanFramebufferCache::dispose()
{
    clear();
}

VulkanFrameBuffer* VulkanFramebufferCache::findOrCreate(const DeviceFramebufferState& state)
{
    uint64_t hash = computeHash(state);
    Ref<VulkanFrameBuffer> framebuffer;
    if (find(hash, &framebuffer)) {
        return framebuffer;
    }
    else {
        framebuffer = makeRef<VulkanFrameBuffer>();
        if (!framebuffer->init(m_deviceContext, state)) {
            return nullptr;
        }
        add(hash, framebuffer);
        return framebuffer;
    }
}

} // namespace detail
} // namespace ln
