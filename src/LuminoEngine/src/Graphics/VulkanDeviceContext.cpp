
#include "Internal.hpp"
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/Platform/PlatformSupport.hpp>
#include "VulkanDeviceContext.hpp"

namespace ln {
namespace detail {

#define LN_VK_CHECK(f) \
{ \
    VkResult r = (f); \
	if (r != VK_SUCCESS) { \
        LN_LOG_ERROR << #f << "; VkResult:" << r << "(" << VulkanDeviceContext::getVkResultName(r) << ")"; \
		return false; \
	} \
}

PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallback = nullptr;
PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallback = nullptr;
PFN_vkDebugReportMessageEXT vkDebugReportMessage = nullptr;
VkDebugReportCallbackEXT vkDebugReportCallback = 0;

#if defined(VK_EXT_debug_marker)
PFN_vkDebugMarkerSetObjectTagEXT vkDebugMarkerSetObjectTag = nullptr;
PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectName = nullptr;
PFN_vkCmdDebugMarkerBeginEXT vkCmdDebugMarkerBegin = nullptr;
PFN_vkCmdDebugMarkerEndEXT vkCmdDebugMarkerEnd = nullptr;
PFN_vkCmdDebugMarkerInsertEXT vkCmdDebugMarkerInsert = nullptr;
#endif

#if defined(VK_KHR_push_descriptor)
PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSet = nullptr;
#endif

#if defined(VK_EXT_hdr_metadata)
PFN_vkSetHdrMetadataEXT vkSetHdrMetadata = nullptr;
#endif

VKAPI_ATTR
void* VKAPI_CALL AllocCallback(
    void* pUserData,
    size_t size,
    size_t alignment,
    VkSystemAllocationScope scope)
{
    VulkanAllocator* allocator = reinterpret_cast<VulkanAllocator*>(pUserData);
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
    VulkanAllocator* allocator = reinterpret_cast<VulkanAllocator*>(pUserData);
    return allocator->realloc(pOriginal, size, alignment, scope);
}

VKAPI_ATTR
void VKAPI_CALL FreeCallback(void* pUserData, void* pMemory)
{
    VulkanAllocator* allocator = reinterpret_cast<VulkanAllocator*>(pUserData);
    return allocator->free(pMemory);
}

VKAPI_ATTR
VkBool32 VKAPI_CALL DebugReportCallback(
    VkFlags msgFlags,
    VkDebugReportObjectTypeEXT objType,
    uint64_t srcObject,
    size_t location,
    int32_t msgCode,
    const char* pLayerPrefix,
    const char* pMsg,
    void* pUserData)
{
    if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
        LN_LOG_ERROR << "[" << pLayerPrefix << "] Code " << msgCode << " : " << pMsg;
    } else if (msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
        LN_LOG_WARNING << "[" << pLayerPrefix << "] Code " << msgCode << " : " << pMsg;
    } else if (msgFlags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
        LN_LOG_INFO << "[" << pLayerPrefix << "] Code " << msgCode << " : " << pMsg;
    } else if (msgFlags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
        LN_LOG_DEBUG << "[" << pLayerPrefix << "] Code " << msgCode << " : " << pMsg;
    } else if (msgFlags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
        LN_LOG_DEBUG << "Performance Warning [" << pLayerPrefix << "] Code " << msgCode << " : " << pMsg;
    }

    return VK_TRUE;
}

struct FormatConversionItem
{
    VkFormat vulkanFormat;
    uint32_t bitPerPixel;
    TextureFormat lnFormat;
    bool isCompress;
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

static VkFormat LNFormatToVkFormat(TextureFormat format)
{
    assert(s_formatConversionTable[(int)format].lnFormat == format);
    return s_formatConversionTable[(int)format].vulkanFormat;
}

static TextureFormat VkFormatToLNFormat(VkFormat format)
{
    for (auto& i : s_formatConversionTable) {
        if (i.vulkanFormat == format) {
            return i.lnFormat;
        }
    }
    return TextureFormat::Unknown;
}

struct BlendFactorConversionItem
{
    BlendFactor lnValue;
    VkBlendFactor vkValueColor;
    VkBlendFactor vkValueAlpha;
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

static VkBlendFactor LNBlendFactorToVkBlendFactor_Color(BlendFactor value)
{
    assert(s_blendFactorConversionTable[(int)value].lnValue == value);
    return s_blendFactorConversionTable[(int)value].vkValueColor;
}

static VkBlendFactor LNBlendFactorToVkBlendFactor_Alpha(BlendFactor value)
{
    assert(s_blendFactorConversionTable[(int)value].lnValue == value);
    return s_blendFactorConversionTable[(int)value].vkValueAlpha;
}

struct BlendOpConversionItem
{
    BlendOp lnValue;
    VkBlendOp vkValue;
};

static const BlendOpConversionItem s_blendOpConversionTable[] =
{
    {BlendOp::Add, VK_BLEND_OP_ADD},
    {BlendOp::Subtract, VK_BLEND_OP_SUBTRACT},
    {BlendOp::ReverseSubtract, VK_BLEND_OP_REVERSE_SUBTRACT},
    {BlendOp::Min, VK_BLEND_OP_MIN},
    {BlendOp::Max, VK_BLEND_OP_MAX},
};

static VkBlendOp LNBlendOpToVkBlendOp(BlendOp value)
{
    assert(s_blendOpConversionTable[(int)value].lnValue == value);
    return s_blendOpConversionTable[(int)value].vkValue;
}

struct ComparisonFuncConversionItem
{
    ComparisonFunc lnValue;
    VkCompareOp vkValue;
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

static VkCompareOp LNComparisonFuncToVkCompareOp(ComparisonFunc value)
{
    assert(s_comparisoFuncConversionTable[(int)value].lnValue == value);
    return s_comparisoFuncConversionTable[(int)value].vkValue;
}

struct FillModeConversionItem
{
    FillMode lnValue;
    VkPolygonMode vkValue;
};

static const FillModeConversionItem s_fillModeConversionTable[] =
{
    {FillMode::Solid, VK_POLYGON_MODE_FILL},
    {FillMode::Wireframe, VK_POLYGON_MODE_LINE},
};

static VkPolygonMode LNFillModeToVkPolygonMode(FillMode value)
{
    assert(s_fillModeConversionTable[(int)value].lnValue == value);
    return s_fillModeConversionTable[(int)value].vkValue;
}

struct CullModeConversionItem
{
    CullMode lnValue;
    VkCullModeFlagBits vkValue;
};

static const CullModeConversionItem s_cullModeConversionTable[] =
{
    {CullMode::None, VK_CULL_MODE_NONE},
    {CullMode::Front, VK_CULL_MODE_FRONT_BIT},
    {CullMode::Back, VK_CULL_MODE_BACK_BIT},
};

static VkCullModeFlagBits LNCullModeToVkCullMode(CullMode value)
{
    assert(s_cullModeConversionTable[(int)value].lnValue == value);
    return s_cullModeConversionTable[(int)value].vkValue;
}

struct StencilOpConversionItem
{
    StencilOp lnValue;
    VkStencilOp vkValue;
};

static const StencilOpConversionItem s_stencilOpConversionTable[] =
{
    {StencilOp::Keep, VK_STENCIL_OP_KEEP},
    {StencilOp::Replace, VK_STENCIL_OP_REPLACE},
};

static VkStencilOp LNStencilOpToVkStencilOp(StencilOp value)
{
    assert(s_stencilOpConversionTable[(int)value].lnValue == value);
    return s_stencilOpConversionTable[(int)value].vkValue;
}

struct VertexElementTypeConversionItem
{
    VertexElementType lnValue;
    VkFormat vkValue;
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

static VkFormat LNVertexElementTypeToVkFormat(VertexElementType value)
{
    assert(s_vertexElementTypeConversionTable[(int)value].lnValue == value);
    return s_vertexElementTypeConversionTable[(int)value].vkValue;
}

//=============================================================================
// VulkanAllocator

VulkanAllocator::VulkanAllocator()
    : m_counter(0)
{
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
// VulkanPipelineCache

uint64_t VulkanPipelineCache::computeHash(const DevicePipelineState& state, const DeviceFramebufferState& framebuffer)
{
	MixHash hash;
	hash.add(state.blendState);
	hash.add(state.rasterizerState);
	hash.add(state.depthStencilState);
	hash.add(state.vertexDeclaration);
	hash.add(state.shaderPass);
	for (auto& t : framebuffer.renderTargets) {
        if (t) {
            hash.add(t->getTextureFormat());
        }
	}
    if (framebuffer.depthBuffer) {
        hash.add(framebuffer.depthBuffer->format());
    }
	return hash.value();
}


//=============================================================================
// VulkanRenderPassCache

uint64_t VulkanRenderPassCache::computeHash(const DeviceFramebufferState& state)
{
	MixHash hash;
	for (size_t i = 0; i < state.renderTargets.size(); i++) {
        if (state.renderTargets[i]) {
            hash.add(state.renderTargets[i]->getTextureFormat());
        }
	}
    if (state.depthBuffer) {
        hash.add(state.depthBuffer->format());
    }
	return hash.value();
}

//=============================================================================
// VulkanRenderPassCache

bool VulkanFrameBuffer::init(VulkanDeviceContext* deviceContext, const DeviceFramebufferState& state)
{
	m_deviceContext = deviceContext;
	m_renderTargetCount = state.renderTargets.size();
	for (size_t i = 0; i < m_renderTargetCount; i++) {
		m_renderTargets[i] = state.renderTargets[i];
	}
	m_depthBuffer = state.depthBuffer;



	VulkanTextureBase::TextureDesc desc = static_cast<VulkanTextureBase*>(m_renderTargets[0])->desc();


	if (!deviceContext->getVkRenderPass(state, &m_renderPass)) {
		return false;
	}
	else
	{
		VkImageView attachments[MaxMultiRenderTargets] = {};
		for (size_t i = 0; i < m_renderTargets.size(); i++) {
            if (m_renderTargets[i]) {
                attachments[i] = static_cast<VulkanTextureBase*>(m_renderTargets[i])->vulkanImageView();
            }
            else {
                attachments[i] = 0;
            }
		}

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.pNext = nullptr;
		framebufferInfo.flags = 0;
		framebufferInfo.renderPass = m_renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = desc.Width;
		framebufferInfo.height = desc.Height;
		framebufferInfo.layers = 1;

		LN_VK_CHECK(vkCreateFramebuffer(m_deviceContext->vulkanDevice(), &framebufferInfo, m_deviceContext->vulkanAllocator(), &m_framebuffer));
	}

	return true;
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
	for (size_t i = 0; i < m_renderTargetCount; i++) {
		if (renderTarget == m_renderTargets[i]) {
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
// VulkanDeviceContext

VulkanDeviceContext::VulkanDeviceContext()
    : m_instance(nullptr)
    , m_allocatorCallbacks()
    , m_allocator()
{
}

VulkanDeviceContext::~VulkanDeviceContext()
{
}

bool VulkanDeviceContext::init(const Settings& settings)
{
    //#ifdef LN_OS_WIN32
    //    Win32PlatformInterface::getWin32WindowHandle(settings.mainWindow);
    //#endif

    const char* instanceExtension[] = {
        VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef LN_OS_WIN32
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif LN_OS_LINUX
        VK_KHR_XCB_SURFACE_EXTENSION_NAME,
#elif LN_OS_ANDROID
        VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,
#endif
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
    };
    size_t instanceExtensionCount = 2;

    // https://vulkan.lunarg.com/doc/view/1.0.61.0/windows/layers.html
    const char* validationLayerNames[] = {
        "VK_LAYER_LUNARG_standard_validation",
        //"VK_LAYER_LUNARG_api_dump",
        //"VK_LAYER_LUNARG_monitor",
        //"VK_LAYER_LUNARG_core_validation",
        //"VK_LAYER_LUNARG_object_tracker",
    };
    size_t validationLayerCount = 0;

    if (settings.debugEnabled) {
        instanceExtensionCount++;
        validationLayerCount = 1;//sizeof(validationLayerNames);
    }

    std::vector<std::string> extensions;
    CheckInstanceExtension(
        nullptr,
        instanceExtensionCount,
        instanceExtension,
        &extensions);
    std::vector<const char*> extensionPtrs;
    for (auto& str : extensions)
        extensionPtrs.push_back(str.c_str());

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.pApplicationName = "Lumino";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Lumino";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo instanceInfo = {};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pNext = nullptr;
    instanceInfo.flags = 0;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledLayerCount = validationLayerCount;
    instanceInfo.ppEnabledLayerNames = (validationLayerCount == 0) ? nullptr : validationLayerNames;
    instanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instanceInfo.ppEnabledExtensionNames = extensionPtrs.data();

    m_allocatorCallbacks.pfnAllocation = AllocCallback;
    m_allocatorCallbacks.pfnFree = FreeCallback;
    m_allocatorCallbacks.pfnReallocation = ReallocCallback;
    m_allocatorCallbacks.pfnInternalAllocation = nullptr;
    m_allocatorCallbacks.pfnInternalFree = nullptr;
    m_allocatorCallbacks.pUserData = &m_allocator;

	LN_VK_CHECK(vkCreateInstance(&instanceInfo, &m_allocatorCallbacks, &m_instance));

    if (settings.debugEnabled) {
        vkCreateDebugReportCallback = GetVkInstanceProc<PFN_vkCreateDebugReportCallbackEXT>("vkCreateDebugReportCallbackEXT");
        vkDestroyDebugReportCallback = GetVkInstanceProc<PFN_vkDestroyDebugReportCallbackEXT>("vkDestroyDebugReportCallbackEXT");
        vkDebugReportMessage = GetVkInstanceProc<PFN_vkDebugReportMessageEXT>("vkDebugReportMessageEXT");

        if (vkCreateDebugReportCallback != nullptr &&
            vkDestroyDebugReportCallback != nullptr &&
            vkDebugReportMessage != nullptr) {
            VkFlags flags = VK_DEBUG_REPORT_ERROR_BIT_EXT;
            flags |= VK_DEBUG_REPORT_WARNING_BIT_EXT;
            flags |= VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
            flags |= VK_DEBUG_REPORT_INFORMATION_BIT_EXT;
            flags |= VK_DEBUG_REPORT_DEBUG_BIT_EXT;

            VkDebugReportCallbackCreateInfoEXT info = {};
            info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
            info.pNext = nullptr;
            info.pfnCallback = DebugReportCallback;
            info.pUserData = nullptr;
            info.flags = flags;

			LN_VK_CHECK(vkCreateDebugReportCallback(m_instance, &info, nullptr, &vkDebugReportCallback));
        }
    }

    // Get physical devices
    {
        uint32_t count = 0;
        auto ret = vkEnumeratePhysicalDevices(m_instance, &count, nullptr);
        if (ret != VK_SUCCESS || count < 1) {
            LN_LOG_ERROR << "Failed vkEnumeratePhysicalDevices";
            return false;
        }

        m_physicalDeviceCount = count;
        m_physicalDeviceInfos.resize(m_physicalDeviceCount);

        std::vector<VkPhysicalDevice> gpuDevices(m_physicalDeviceCount);
        if (vkEnumeratePhysicalDevices(m_instance, &count, gpuDevices.data()) != VK_SUCCESS) {
            LN_LOG_ERROR << "Failed vkEnumeratePhysicalDevices";
            return false;
        }

        for (auto i = 0u; i < count; ++i) {
            m_physicalDeviceInfos[i].device = gpuDevices[i];
            vkGetPhysicalDeviceMemoryProperties(gpuDevices[i], &m_physicalDeviceInfos[i].memoryProperty);
            vkGetPhysicalDeviceProperties(gpuDevices[i], &m_physicalDeviceInfos[i].deviceProperty);
        }
    }

    // Select device
    m_physicalDevice = m_physicalDeviceInfos[0].device;

    // Create Device and Queue
    {
        auto graphicsFamilyIndex = UINT32_MAX;
        auto computeFamilyIndex = UINT32_MAX;
        auto transferFamilyIndex = UINT32_MAX;

        auto graphicsQueueIndex = UINT32_MAX;
        auto computeQueueIndex = UINT32_MAX;
        auto transferQueueindex = UINT32_MAX;

        {
            uint32_t propCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &propCount, nullptr);
            std::vector<VkQueueFamilyProperties> queueFamilyPros(propCount);
            vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &propCount, queueFamilyPros.data());

            std::vector<VkDeviceQueueCreateInfo> queueInfos(propCount);
            int queueIndex = 0;
            int totalQueueCount = 0;
            for (int i = 0; i < propCount; ++i) {
                queueInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueInfos[i].pNext = nullptr;
                queueInfos[i].flags = 0;
                queueInfos[i].queueCount = queueFamilyPros[i].queueCount;
                queueInfos[i].queueFamilyIndex = i;

                totalQueueCount += queueFamilyPros[i].queueCount;

                // Graphics queue
                if (queueFamilyPros[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    if (graphicsFamilyIndex == UINT32_MAX) {
                        graphicsFamilyIndex = i;
                        graphicsQueueIndex = queueIndex;
                        queueIndex++;
                    }
                }

                // Compute queue
                if ((queueFamilyPros[i].queueFlags & VK_QUEUE_COMPUTE_BIT) && ((queueFamilyPros[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != VK_QUEUE_GRAPHICS_BIT)) {
                    if (computeFamilyIndex == UINT32_MAX) {
                        computeFamilyIndex = i;
                        computeQueueIndex = queueIndex;
                        queueIndex++;
                    }
                }

                // Transfer queue
                if ((queueFamilyPros[i].queueFlags & VK_QUEUE_TRANSFER_BIT) && ((queueFamilyPros[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != VK_QUEUE_GRAPHICS_BIT)) {
                    if (transferFamilyIndex == UINT32_MAX) {
                        transferFamilyIndex = i;
                        transferQueueindex = queueIndex;
                        queueIndex++;
                    }
                }
            }

            // 1つも見つからなければ仕方ないので共用のものを探す.
            if (computeFamilyIndex == UINT32_MAX) {
                for (auto i = 0u; i < propCount; ++i) {
                    if (queueFamilyPros[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
                        if (computeFamilyIndex == UINT32_MAX) {
                            computeFamilyIndex = i;
                            computeQueueIndex = queueIndex;
                            queueIndex++;
                        }
                    }
                }
            }

            // 1つも見つからなければ仕方ないので共用のものを探す.
            if (transferFamilyIndex == UINT32_MAX) {
                for (auto i = 0u; i < propCount; ++i) {
                    if (queueFamilyPros[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
                        if (transferFamilyIndex == UINT32_MAX) {
                            transferFamilyIndex = i;
                            transferQueueindex = queueIndex;
                            queueIndex++;
                        }
                    }
                }
            }

            std::vector<float> queuePriorities(totalQueueCount);
            uint32_t offset = 0u;
            for (uint32_t i = 0u; i < propCount; ++i) {
                queueInfos[i].pQueuePriorities = &queuePriorities[offset];
                offset += queueInfos[i].queueCount;
            }

            std::vector<std::string> deviceExtensions;
            if (settings.debugEnabled) {
                deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
            }
            else {
                GetDeviceExtension(
                    nullptr,
                    m_physicalDevice,
                    &deviceExtensions);

                m_ext_EXT_KHR_PUSH_DESCRIPTOR = false;
                m_ext_EXT_KHR_DESCRIPTOR_UPDATE_TEMPLATE = false;
                m_ext_EXT_NVX_DEVICE_GENERATE_COMMAND = false;
                for (size_t i = 0; i < deviceExtensions.size(); ++i) {
#if defined(VK_KHR_push_descriptor)
                    if (deviceExtensions[i] == VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME) {
                        m_ext_EXT_KHR_PUSH_DESCRIPTOR = true;
                    }
                    if (deviceExtensions[i] == VK_KHR_DESCRIPTOR_UPDATE_TEMPLATE_EXTENSION_NAME) {
                        m_ext_EXT_KHR_DESCRIPTOR_UPDATE_TEMPLATE = true;
                    }
#endif
#if defined(VK_NVX_device_generated_commands)
                    if (deviceExtensions[i] == VK_NVX_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME) {
                        m_ext_EXT_NVX_DEVICE_GENERATE_COMMAND = true;
                    }
#endif
#if defined(VK_AMD_draw_indirect_count)
                    if (deviceExtensions[i] == VK_AMD_DRAW_INDIRECT_COUNT_EXTENSION_NAME) {
                        m_ext_EXT_AMD_DRAW_INDIRECT_COUNT = true;
                    }
#endif
#if defined(VK_EXT_debug_marker)
                    if (deviceExtensions[i] == VK_EXT_DEBUG_MARKER_EXTENSION_NAME) {
                        m_ext_EXT_DEBUG_MARKER = true;
                    }
#endif
#if defined(VK_EXT_hdr_metadata)
                    if (deviceExtensions[i] == VK_EXT_HDR_METADATA_EXTENSION_NAME) {
                        m_ext_EXT_HDR_METADATA = true;
                    }
#endif
                }
            }

            std::vector<const char*> deviceExtensionPtrs;
            for (auto& str : deviceExtensions)
                deviceExtensionPtrs.push_back(str.c_str());

            VkDeviceCreateInfo deviceInfo = {};
            deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            deviceInfo.pNext = nullptr;
            deviceInfo.queueCreateInfoCount = propCount;
            deviceInfo.pQueueCreateInfos = queueInfos.data();
            deviceInfo.enabledLayerCount = 0;         // deprecated https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkDeviceCreateInfo.html
            deviceInfo.ppEnabledLayerNames = nullptr; // deprecated https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkDeviceCreateInfo.html
            deviceInfo.enabledExtensionCount = (uint32_t)deviceExtensionPtrs.size();
            deviceInfo.ppEnabledExtensionNames = deviceExtensionPtrs.data();
            deviceInfo.pEnabledFeatures = nullptr;

			LN_VK_CHECK(vkCreateDevice(m_physicalDevice, &deviceInfo, nullptr, &m_device));
        }

#if defined(VK_EXT_debug_marker)
        if (m_ext_EXT_DEBUG_MARKER) {
            vkDebugMarkerSetObjectTag = GetVkDeviceProc<PFN_vkDebugMarkerSetObjectTagEXT>("vkDebugMarkerSetObjectTagEXT");
            vkDebugMarkerSetObjectName = GetVkDeviceProc<PFN_vkDebugMarkerSetObjectNameEXT>("vkDebugMarkerSetObjectNameEXT");
            vkCmdDebugMarkerBegin = GetVkDeviceProc<PFN_vkCmdDebugMarkerBeginEXT>("vkCmdDebugMarkerBeginEXT");
            vkCmdDebugMarkerEnd = GetVkDeviceProc<PFN_vkCmdDebugMarkerEndEXT>("vkCmdDebugMarkerEndEXT");
            vkCmdDebugMarkerInsert = GetVkDeviceProc<PFN_vkCmdDebugMarkerInsertEXT>("vkCmdDebugMarkerInsert");
        }
#endif
#if defined(VK_KHR_push_descriptor)
        if (m_ext_EXT_KHR_PUSH_DESCRIPTOR) {
            vkCmdPushDescriptorSet = GetVkDeviceProc<PFN_vkCmdPushDescriptorSetKHR>("vkCmdPushDescriptorSetKHR");
        }
#endif
#if defined(VK_EXT_hdr_metadata)
        if (m_ext_EXT_HDR_METADATA) {
            vkSetHdrMetadata = GetVkDeviceProc<PFN_vkSetHdrMetadataEXT>("vkSetHdrMetadataEXT");
        }
#endif
        m_graphicsQueue = makeRef<VulkanQueue>();
        if (!m_graphicsQueue->init(this, graphicsFamilyIndex, graphicsQueueIndex, settings.maxGraphicsQueueSubmitCount)) {
            return false;
        }

        m_computeQueue = makeRef<VulkanQueue>();
        if (!m_computeQueue->init(this, computeFamilyIndex, computeQueueIndex, settings.maxComputeQueueSubmitCount)) {
            return false;
        }

        m_transferQueue = makeRef<VulkanQueue>();
        if (!m_transferQueue->init(this, transferFamilyIndex, transferQueueindex, settings.maxTransferQueueSubmitCount)) {
            return false;
        }
    }

    // Default Shader
    {
		static const unsigned char vertData[] =
		{
#include "Resource/default.vert.spv.inl"
		};
		static const size_t vertSize = LN_ARRAY_SIZE_OF(vertData);

		static const unsigned char fragData[] =
		{
#include "Resource/default.frag.spv.inl"
		};
		static const size_t fragSize = LN_ARRAY_SIZE_OF(fragData);

		m_defaultShaderPass = makeRef<VulkanShaderPass>();
		if (!m_defaultShaderPass->init(this, vertData, vertSize, fragData, fragSize, nullptr, nullptr)) {
			return false;
		}
    }

    // Get device infomation
    {
        auto& limits = m_physicalDeviceInfos[0].deviceProperty.limits;
        m_caps.ConstantBufferMemoryAlignment = static_cast<uint32_t>(limits.minUniformBufferOffsetAlignment);
        m_caps.MaxTargetWidth = limits.maxFramebufferWidth;
        m_caps.MaxTargetHeight = limits.maxFramebufferHeight;
        m_caps.MaxTargetArraySize = limits.maxFramebufferLayers;
        m_caps.MaxColorSampleCount = static_cast<uint32_t>(limits.framebufferColorSampleCounts);
        m_caps.MaxDepthSampleCount = static_cast<uint32_t>(limits.framebufferDepthSampleCounts);
        m_caps.MaxStencilSampleCount = static_cast<uint32_t>(limits.framebufferStencilSampleCounts);

        if (limits.timestampComputeAndGraphics) {
            auto nanoToSec = 1000 * 1000 * 1000;
            m_timeStampFrequency = static_cast<uint64_t>(limits.timestampPeriod * nanoToSec);
        } else {
            m_timeStampFrequency = 1;
        }
    }

	if (!m_renderPassCache.init([this](VkRenderPass v) { vkDestroyRenderPass(m_device, v, &m_allocatorCallbacks); })) {
        return false;
    }

    if (!m_pipelineCache.init([this](Ref<VulkanPipeline> v) { v->dispose(); })) {
        return false;
    }

	if (!m_frameBufferCache.init([this](Ref<VulkanFrameBuffer> v) { v->dispose(); })) {
		return false;
	}

    m_activeCommandBuffer = makeRef<VulkanCommandList>();
    if (!m_activeCommandBuffer->init(this, VulkanCommandList::Type::COMMANDLIST_TYPE_DIRECT)) {
        return false;
    }

    if (!beginActiveCommandBuffer()) {
        return false;
    }

	m_requiredChangePipeline = true;
	m_requiredChangeRenderPass = true;
    return true;
}

void VulkanDeviceContext::dispose()
{
	endActiveCommandBuffer();

	m_frameBufferCache.clear();
    m_pipelineCache.clear();
    m_renderPassCache.clear();

	if (m_defaultShaderPass) {
		m_defaultShaderPass->dispose();
		m_defaultShaderPass = nullptr;
	}

    if (m_transferQueue) {
        m_transferQueue->dispose();
        m_transferQueue = nullptr;
    }
    if (m_computeQueue) {
        m_computeQueue->dispose();
        m_computeQueue = nullptr;
    }
    if (m_graphicsQueue) {
        m_graphicsQueue->dispose();
        m_graphicsQueue = nullptr;
    }

    if (m_device) {
        vkDeviceWaitIdle(m_device);
        vkDestroyDevice(m_device, nullptr);
        m_device = nullptr;
    }

    if (vkDebugReportCallback != 0) {
        vkDestroyDebugReportCallback(
            m_instance,
            vkDebugReportCallback,
            nullptr);
        vkDebugReportCallback = 0;
    }

    if (m_instance) {
        vkDestroyInstance(m_instance, &m_allocatorCallbacks);
        m_instance = nullptr;
    }

    IGraphicsDeviceContext::dispose();
}

uint32_t VulkanDeviceContext::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

bool VulkanDeviceContext::getVkRenderPass(const DeviceFramebufferState& state, VkRenderPass* outPass)
{
	uint64_t hash = VulkanRenderPassCache::computeHash(state);
	VkRenderPass renderPass = 0;
	if (renderPassCache().find(hash, &renderPass)) {
		// use renderPass
	}
	else
	{
		// MaxRenderTargets + 1枚の depthbuffer
		VkAttachmentDescription attachmentDescs[MaxMultiRenderTargets + 1] = {};
		VkAttachmentReference attachmentRefs[MaxMultiRenderTargets + 1] = {};
		VkAttachmentReference* depthAttachmentRef = nullptr;
		int attachmentCount = 0;
		int colorAttachmentCount = 0;

		for (int i = 0; i < MaxMultiRenderTargets; i++) {
			if (state.renderTargets[i]) {
				attachmentDescs[i].flags = 0;
				attachmentDescs[i].format = LNFormatToVkFormat(state.renderTargets[i]->getTextureFormat());
				attachmentDescs[i].samples = VK_SAMPLE_COUNT_1_BIT;
				attachmentDescs[i].loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachmentDescs[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachmentDescs[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachmentDescs[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachmentDescs[i].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				attachmentDescs[i].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

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
			attachmentDescs[i].format = VK_FORMAT_D32_SFLOAT_S8_UINT;
			attachmentDescs[i].samples = VK_SAMPLE_COUNT_1_BIT;
			attachmentDescs[i].loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachmentDescs[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachmentDescs[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachmentDescs[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachmentDescs[i].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
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

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.pNext = nullptr;
		renderPassInfo.flags = 0;
		renderPassInfo.attachmentCount = attachmentCount;
		renderPassInfo.pAttachments = attachmentDescs;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 0;
		renderPassInfo.pDependencies = nullptr;

		LN_VK_CHECK(vkCreateRenderPass(vulkanDevice(), &renderPassInfo, vulkanAllocator(), &renderPass));

		renderPassCache().add(hash, renderPass);
	}

	*outPass = renderPass;
	return true;
}

bool VulkanDeviceContext::beginActiveCommandBuffer()
{
	if (!m_activeCommandBuffer->begin()) {
		return false;
	}
    return true;
}

bool VulkanDeviceContext::endActiveCommandBuffer()
{
	if (!m_activeCommandBuffer->end()) {
		return false;
	}
	return true;
}

const char* VulkanDeviceContext::getVkResultName(VkResult result)
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

void VulkanDeviceContext::onGetCaps(GraphicsDeviceCaps* outCaps)
{
    outCaps->requestedShaderTriple.target = "spv";
    outCaps->requestedShaderTriple.version = 110;
    outCaps->requestedShaderTriple.option = "";
}

void VulkanDeviceContext::onEnterMainThread()
{
}

void VulkanDeviceContext::onLeaveMainThread()
{
}

void VulkanDeviceContext::onSaveExternalRenderState()
{
}

void VulkanDeviceContext::onRestoreExternalRenderState()
{
}

Ref<ISwapChain> VulkanDeviceContext::onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize)
{
    VulkanSwapChain::SwapChainDesc desc;
    desc.Width = backbufferSize.width;
    desc.Height = backbufferSize.height;
    desc.Format = TextureFormat::RGBA32;
    desc.vulkanFormat = LNFormatToVkFormat(desc.Format);
    desc.MipLevels = 1;
    desc.SampleCount = 1;
    desc.BufferCount = 2;
    desc.SyncInterval = 1;
    desc.EnableFullScreen = false;

    auto ptr = makeRef<VulkanSwapChain>();
    if (!ptr->init(this, window, desc)) {
        return nullptr;
    }

    return ptr;
}

Ref<IVertexDeclaration> VulkanDeviceContext::onCreateVertexDeclaration(const VertexElement* elements, int elementsCount)
{
    auto ptr = makeRef<VulkanVertexDeclaration>();
    if (!ptr->init(elements, elementsCount)) {
        return nullptr;
    }
    return ptr;
}

Ref<IVertexBuffer> VulkanDeviceContext::onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData)
{
    auto ptr = makeRef<VulkanVertexBuffer>();
    if (!ptr->init(this, usage, bufferSize, initialData)) {
        return nullptr;
    }
    return ptr;
}

Ref<IIndexBuffer> VulkanDeviceContext::onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData)
{
	auto ptr = makeRef<VulkanIndexBuffer>();
	if (!ptr->init(this, usage, format, indexCount, initialData)) {
		return nullptr;
	}
	return ptr;
}

Ref<ITexture> VulkanDeviceContext::onCreateTexture2D(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<ITexture> VulkanDeviceContext::onCreateTexture3D(uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<ITexture> VulkanDeviceContext::onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap)
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<IDepthBuffer> VulkanDeviceContext::onCreateDepthBuffer(uint32_t width, uint32_t height)
{
    auto ptr = makeRef<VulkanDepthBuffer>();
    ptr->init(this, width, height);
    return ptr;
}

Ref<ISamplerState> VulkanDeviceContext::onCreateSamplerState(const SamplerStateData& desc)
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<IShaderPass> VulkanDeviceContext::onCreateShaderPass(const byte_t* vsCode, int vsCodeLen, const byte_t* psCode, int psCodeLen, const ShaderVertexInputAttributeTable* attributeTable, const UnifiedShaderRefrectionInfo* refrection, ShaderCompilationDiag* diag)
{
    auto ptr = makeRef<VulkanShaderPass>();
    if (!ptr->init(this, vsCode, vsCodeLen, psCode, psCodeLen, attributeTable, refrection)) {
        return nullptr;
    }
    return ptr;
}

void VulkanDeviceContext::onUpdatePipelineState(const BlendStateDesc& blendState, const RasterizerStateDesc& rasterizerState, const DepthStencilStateDesc& depthStencilState)
{
	m_requiredChangePipeline = true;
}

void VulkanDeviceContext::onUpdateFrameBuffers(ITexture** renderTargets, int renderTargetsCount, IDepthBuffer* depthBuffer)
{
	//m_requiredChangePipeline = true;
}

void VulkanDeviceContext::onUpdateRegionRects(const RectI& viewportRect, const RectI& scissorRect, const SizeI& targetSize)
{
}

void VulkanDeviceContext::onUpdatePrimitiveData(IVertexDeclaration* decls, IVertexBuffer** vertexBuufers, int vertexBuffersCount, IIndexBuffer* indexBuffer)
{
	// TODO: decls の変更時のみでよい
	m_requiredChangePipeline = true;
}

void VulkanDeviceContext::onUpdateShaderPass(IShaderPass* newPass)
{
	m_requiredChangePipeline = true;
}

void VulkanDeviceContext::onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
	submitStatus();

    const State& state = committedState();

    SizeI size = state.framebufferState.renderTargets[0]->realSize();
	{
        VkClearRect rect[1];
        rect[0].rect.offset.x = 0;
        rect[0].rect.offset.y = 0;
        rect[0].rect.extent.width = size.width;
        rect[0].rect.extent.height = size.height;
        rect[0].baseArrayLayer = 0;
        rect[0].layerCount = 1;

        VkClearAttachment attachments[MaxMultiRenderTargets + 1] = {};

        uint32_t count = 0;
        uint32_t rtCount = 1;

        if (testFlag(flags, ClearFlags::Color))
        {
            float frgba[4] = { color.r, color.g, color.b, color.a, };

            for (uint32_t ii = 0; ii < rtCount; ++ii)
            {
                attachments[count].colorAttachment = count;
                attachments[count].aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                attachments[count].clearValue.color.float32[0] = color.r;
                attachments[count].clearValue.color.float32[1] = color.g;
                attachments[count].clearValue.color.float32[2] = color.b;
                attachments[count].clearValue.color.float32[3] = color.a;
                ++count;
            }
        }

        if (state.framebufferState.depthBuffer != nullptr &&
            testFlag(flags, (ClearFlags)(ClearFlags::Depth | ClearFlags::Stencil)))
        {
            attachments[count].colorAttachment = count;
            attachments[count].aspectMask = 0;
            attachments[count].aspectMask |= (testFlag(flags, ClearFlags::Depth)) ? VK_IMAGE_ASPECT_DEPTH_BIT : 0;
            attachments[count].aspectMask |= (testFlag(flags, ClearFlags::Stencil)) ? VK_IMAGE_ASPECT_STENCIL_BIT : 0;
            attachments[count].clearValue.depthStencil.stencil = stencil;
            attachments[count].clearValue.depthStencil.depth = z;
            ++count;
        }

        vkCmdClearAttachments(
            m_activeCommandBuffer->vulkanCommandBuffer()
            , count
            , attachments
            , LN_ARRAY_SIZE_OF(rect)
            , rect
        );

	}
}

void VulkanDeviceContext::onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount)
{
    submitStatus();
    vkCmdDraw(m_activeCommandBuffer->vulkanCommandBuffer(), primitiveCount * 3, primitiveCount, startVertex, 0);
}

void VulkanDeviceContext::onDrawPrimitiveIndexed(PrimitiveTopology primitive, int startIndex, int primitiveCount)
{
    submitStatus();
    vkCmdDrawIndexed(m_activeCommandBuffer->vulkanCommandBuffer(), primitiveCount * 3, 1, startIndex, 0, 0);
}

void VulkanDeviceContext::onPresent(ISwapChain* swapChain)
{
    if (!static_cast<VulkanSwapChain*>(swapChain)->present()) {
        LN_ERROR();
    }
}

bool VulkanDeviceContext::submitStatus()
{
	const State& state = committedState();

	if (m_requiredChangeRenderPass) {
		uint64_t hash = VulkanRenderPassCache::computeHash(state.framebufferState);
		Ref<VulkanFrameBuffer> framebuffer;
		if (frameBufferCache().find(hash, &framebuffer)) {
		}
		else {
			framebuffer = makeRef<VulkanFrameBuffer>();
			if (!framebuffer->init(this, state.framebufferState)) {
				return false;
			}
		}

		m_activeCommandBuffer->addFrameBufferCmd(framebuffer);
	}


	if (m_requiredChangePipeline) {
		uint64_t hash = VulkanPipelineCache::computeHash(state.pipelineState, state.framebufferState);
		Ref<VulkanPipeline> pipeline;
		if (pipelineCache().find(hash, &pipeline)) {
		}
		else {
			pipeline = makeRef<VulkanPipeline>();
			if (!pipeline->init(this, state)) {
				return false;
			}
		}

		m_activeCommandBuffer->addPipelineCmd(pipeline);
	}


    VkCommandBuffer commandBuffer = m_activeCommandBuffer->vulkanCommandBuffer();


    // TODO: modify チェック
    {
        VkViewport viewport;
        viewport.x = state.viewportRect.x;
        viewport.y = state.viewportRect.height + state.viewportRect.y;
        viewport.width = state.viewportRect.width;
        viewport.height = -state.viewportRect.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor;
        scissor.offset.x = state.scissorRect.x;
        scissor.offset.y = state.scissorRect.y;
        scissor.extent.width = state.scissorRect.width;
        scissor.extent.height = state.scissorRect.height;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    // TODO: modify チェック
    for (int i = 0; i < MaxVertexStreams; i++)
    {
        if (state.vertexBuffers[i])
        {
            VkBuffer vertexBuffers[] = { static_cast<VulkanVertexBuffer*>(state.vertexBuffers[i])->vulkanBuffer() };
            VkDeviceSize offsets[] = { 0 };
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        }
        

    }

    // TODO: modify チェック
    if (state.indexBuffer)
    {
        VkBuffer indexBuffer = static_cast<VulkanIndexBuffer*>(state.indexBuffer)->vulkanBuffer();
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);  // TODO: format
    }

	return true;
}

// 要求したインスタンスの拡張が本当に使えるか確認する
void VulkanDeviceContext::CheckInstanceExtension(
    const char* layer,
    size_t requestCount,
    const char** requestNames,
    std::vector<std::string>* result)
{
    uint32_t count;
    auto t = vkEnumerateInstanceExtensionProperties(layer, &count, nullptr);

    std::vector<VkExtensionProperties> exts;
    exts.resize(count);
    vkEnumerateInstanceExtensionProperties(layer, &count, exts.data());

    // dump
    for (size_t i = 0; i < exts.size(); ++i) {
        LN_LOG_INFO << exts[i].extensionName;
    }

    result->reserve(count);
    for (size_t i = 0; i < exts.size(); ++i) {
        bool hit = false;
        for (size_t j = 0; j < requestCount; ++j) {
            if (strcmp(exts[i].extensionName, requestNames[j]) == 0) {
                hit = true;
                break;
            }
        }

        if (!hit) {
            continue;
        }

        result->push_back(exts[i].extensionName);
    }
}

void VulkanDeviceContext::GetDeviceExtension(
    const char* layer,
    VkPhysicalDevice physicalDevice,
    std::vector<std::string>* result)
{
    uint32_t count;
    vkEnumerateDeviceExtensionProperties(physicalDevice, layer, &count, nullptr);

    std::vector<VkExtensionProperties> exts;
    exts.resize(count);
    vkEnumerateDeviceExtensionProperties(physicalDevice, layer, &count, exts.data());

    result->reserve(count);
    for (size_t i = 0; i < exts.size(); ++i) {
        result->push_back(exts[i].extensionName);
    }
}

//=============================================================================
// VulkanQueue

VulkanQueue::VulkanQueue()
    : m_deviceContext(nullptr)
    , m_familyIndex(0)
    , m_maxSubmitCount(0)
    , m_queue(nullptr)
{
}

bool VulkanQueue::init(VulkanDeviceContext* deviceContext, uint32_t familyIndex, uint32_t queueIndex, uint32_t maxSubmitCount)
{
    if (LN_REQUIRE(deviceContext)) return false;
    m_deviceContext = deviceContext;
    VkDevice vulkanDevice = m_deviceContext->vulkanDevice();


    vkGetDeviceQueue(vulkanDevice, familyIndex, queueIndex, &m_queue);

    m_maxSubmitCount = maxSubmitCount;
    m_familyIndex = familyIndex;

    m_submitList.resize(maxSubmitCount);
    for (int i = 0; i < maxSubmitCount; i++) {
        m_submitList[i] = 0;
    }

    //m_submitIndex = 0;
    //m_currentBufferIndex = 0;
    //m_previousBufferIndex = 0;

    return true;
}

void VulkanQueue::dispose()
{
    VkDevice vulkanDevice = m_deviceContext->vulkanDevice();

    // Wait for complation
    if (m_queue) {
        vkQueueWaitIdle(m_queue);
    }

    m_submitList.clear();
    m_queue = 0;
}

bool VulkanQueue::submit(VulkanCommandList* commandBuffer, VkSemaphore waitSemaphore, VkSemaphore signalSemaphore)
{
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = nullptr;

    // 実行を開始する前に待機するセマフォ
    VkSemaphore waitSemaphores[] = { waitSemaphore };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    // 実行するコマンド
    VkCommandBuffer commandBuffers[] = { commandBuffer->vulkanCommandBuffer() };
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = commandBuffers;

    // 実行を完了したときに通知されるセマフォ
    VkSemaphore signalSemaphores[] = { signalSemaphore };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    // コマンド実行通知。fence は、この実行の完了が通知される。
	LN_VK_CHECK(vkQueueSubmit(m_queue, 1, &submitInfo, commandBuffer->vulkanInFlightFence()));

    return true;
}

//=============================================================================
// VulkanCommandList

VulkanCommandList::VulkanCommandList()
	: m_inFlightFence(0)
{
}

VulkanCommandList::~VulkanCommandList()
{
}

bool VulkanCommandList::init(VulkanDeviceContext* deviceContext, Type type)
{
    m_deviceContext = deviceContext;

    // Create command pool
    {
        uint32_t queueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

        VulkanQueue* queue = nullptr;
        if (type == Type::COMMANDLIST_TYPE_DIRECT) {
            queue = m_deviceContext->graphicsQueue();
        } else if (type == Type::COMMANDLIST_TYPE_COMPUTE) {
            queue = m_deviceContext->computeQueue();
        } else if (type == Type::COMMANDLIST_TYPE_COPY) {
            queue = m_deviceContext->transferQueue();
        }

        if (queue) {
            queueFamilyIndex = queue->familyIndex();
        }

        VkCommandPoolCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        info.pNext = nullptr;
        info.queueFamilyIndex = queueFamilyIndex;
        info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		LN_VK_CHECK(vkCreateCommandPool(m_deviceContext->vulkanDevice(), &info, m_deviceContext->vulkanAllocator(), &m_commandPool));
    }

    if (type == Type::COMMANDLIST_TYPE_BUNDLE) {
        VkCommandBufferAllocateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        info.pNext = nullptr;
        info.commandPool = m_commandPool;
        info.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
        info.commandBufferCount = 1;

		LN_VK_CHECK(vkAllocateCommandBuffers(m_deviceContext->vulkanDevice(), &info, &m_commandBuffer));

    } else {
        VkCommandBufferAllocateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        info.pNext = nullptr;
        info.commandPool = m_commandPool;
        info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        info.commandBufferCount = 1;

		LN_VK_CHECK(vkAllocateCommandBuffers(m_deviceContext->vulkanDevice(), &info, &m_commandBuffer));

    }

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;	// Signal 状態 = コマンド実行完了状態にしておく
	LN_VK_CHECK(vkCreateFence(m_deviceContext->vulkanDevice(), &fenceInfo, m_deviceContext->vulkanAllocator(), &m_inFlightFence));

    return true;
}

void VulkanCommandList::dispose()
{
    if (m_commandBuffer) {
        vkFreeCommandBuffers(m_deviceContext->vulkanDevice(), m_commandPool, 1, &m_commandBuffer);
        m_commandBuffer = nullptr;
    }

    if (m_commandPool) {
        vkDestroyCommandPool(m_deviceContext->vulkanDevice(), m_commandPool, m_deviceContext->vulkanAllocator());
        m_commandPool = 0;
    }

	if (m_inFlightFence) {
		vkDestroyFence(m_deviceContext->vulkanDevice(), m_inFlightFence, m_deviceContext->vulkanAllocator());
		m_inFlightFence = 0;
	}
}

bool VulkanCommandList::begin()
{
    VkCommandBufferInheritanceInfo inheritanceInfo = {};
    inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
    inheritanceInfo.pNext = nullptr;
    inheritanceInfo.renderPass = 0;
    inheritanceInfo.subpass = 0;
    inheritanceInfo.framebuffer = 0;
    inheritanceInfo.occlusionQueryEnable = VK_FALSE;
    inheritanceInfo.queryFlags = 0;
    inheritanceInfo.pipelineStatistics = 0;

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pNext = nullptr;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = &inheritanceInfo;

	LN_VK_CHECK(vkBeginCommandBuffer(m_commandBuffer, &beginInfo));

    VkViewport dummyViewport = {};
    dummyViewport.width = 1;
    dummyViewport.height = 1;
    dummyViewport.minDepth = 0.0f;
    dummyViewport.maxDepth = 1.0f;
    vkCmdSetViewport(m_commandBuffer, 0, 1, &dummyViewport);

    VkRect2D dummyScissor = {};
    vkCmdSetScissor(m_commandBuffer, 0, 1, &dummyScissor);

    // TODO: これは固定でいい気がする
    float blendConstant[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    vkCmdSetBlendConstants(m_commandBuffer, blendConstant);

    vkCmdSetStencilReference(m_commandBuffer, VK_STENCIL_FRONT_AND_BACK, 0);

	m_currentFrameBuffer = nullptr;

	return true;
}

bool VulkanCommandList::end()
{
    if (m_currentFrameBuffer)
    {
        vkCmdEndRenderPass(m_commandBuffer);
		m_currentFrameBuffer = nullptr;
    }

	LN_VK_CHECK(vkEndCommandBuffer(m_commandBuffer));

	return true;
}

bool VulkanCommandList::flush()
{
    VulkanQueue* queue = m_deviceContext->graphicsQueue();
    VkQueue vulkanQueue = queue->vulkanQueue();

    VkPipelineStageFlags waitDstStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkSubmitInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    info.pNext = nullptr;
    info.waitSemaphoreCount = 0;
    info.pWaitSemaphores = nullptr;
    info.pWaitDstStageMask = &waitDstStageMask;
    info.commandBufferCount = 1;
    info.pCommandBuffers = &m_commandBuffer;
    info.signalSemaphoreCount = 0;
    info.pSignalSemaphores = nullptr;

    vkQueueSubmit(vulkanQueue, 1, &info, 0);
    vkQueueWaitIdle(vulkanQueue);

	return true;
}

void VulkanCommandList::addFrameBufferCmd(VulkanFrameBuffer* frameBuffer)
{
	if (frameBuffer != m_currentFrameBuffer)
	{
		if (m_currentFrameBuffer) {
			vkCmdEndRenderPass(m_commandBuffer);
		}

		m_currentFrameBuffer = frameBuffer;

		if (m_currentFrameBuffer) {
			SizeI extent = m_currentFrameBuffer->extent();

			VkRenderPassBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			beginInfo.pNext = nullptr;
			beginInfo.renderPass = m_currentFrameBuffer->vulkanRenderPass();
			beginInfo.framebuffer = m_currentFrameBuffer->vulkanFramebuffer();
			beginInfo.renderArea.offset.x = 0;
			beginInfo.renderArea.offset.y = 0;
			beginInfo.renderArea.extent.width = extent.width;
			beginInfo.renderArea.extent.height = extent.height;
			beginInfo.clearValueCount = 0;
			beginInfo.pClearValues = nullptr;
			vkCmdBeginRenderPass(m_commandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
		}
	}
}

void VulkanCommandList::addPipelineCmd(VulkanPipeline* pipeline)
{
    vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->vulkanPipeline());
}

//==============================================================================
// VulkanPipeline

VulkanPipeline::VulkanPipeline()
{
}

VulkanPipeline::~VulkanPipeline()
{
}

bool VulkanPipeline::init(VulkanDeviceContext* deviceContext, const IGraphicsDeviceContext::State& committed)
{
	m_deviceContext = deviceContext;

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	VkPipelineColorBlendAttachmentState colorBlendAttachments[BlendStateDesc::MaxRenderTargets] = {};
	{
		const BlendStateDesc& state = committed.pipelineState.blendState;
		int attachmentsCount = 0;
		for (int i = 0; i < BlendStateDesc::MaxRenderTargets; i++) {
			colorBlendAttachments[i].blendEnable = (state.renderTargets[i].blendEnable) ? VK_TRUE : VK_FALSE;

			colorBlendAttachments[i].srcColorBlendFactor = LNBlendFactorToVkBlendFactor_Color(state.renderTargets[i].sourceBlend);
			colorBlendAttachments[i].dstColorBlendFactor = LNBlendFactorToVkBlendFactor_Color(state.renderTargets[i].destinationBlend);
			colorBlendAttachments[i].colorBlendOp = LNBlendOpToVkBlendOp(state.renderTargets[i].blendOp);

			colorBlendAttachments[i].srcAlphaBlendFactor = LNBlendFactorToVkBlendFactor_Alpha(state.renderTargets[i].sourceBlendAlpha);
			colorBlendAttachments[i].dstAlphaBlendFactor = LNBlendFactorToVkBlendFactor_Alpha(state.renderTargets[i].destinationBlendAlpha);
			colorBlendAttachments[i].alphaBlendOp = LNBlendOpToVkBlendOp(state.renderTargets[i].blendOpAlpha);

			colorBlendAttachments[i].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

			attachmentsCount++;

			if (!state.independentBlendEnable) {
				break;
			}
		}

		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = attachmentsCount;
		colorBlending.pAttachments = colorBlendAttachments;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;
	}

	VkPipelineRasterizationStateCreateInfo rasterizerInfo = {};
	{
		const RasterizerStateDesc& state = committed.pipelineState.rasterizerState;

		rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizerInfo.depthClampEnable = VK_FALSE;
		rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
		rasterizerInfo.polygonMode = LNFillModeToVkPolygonMode(state.fillMode);
        rasterizerInfo.cullMode = LNCullModeToVkCullMode(state.cullMode);
		rasterizerInfo.frontFace = VK_FRONT_FACE_CLOCKWISE; // 右回り
		rasterizerInfo.depthBiasEnable = VK_FALSE;
		rasterizerInfo.depthBiasConstantFactor = 0.0f;
		rasterizerInfo.depthBiasClamp = 0.0f;
		rasterizerInfo.depthBiasSlopeFactor = 0.0f;
		rasterizerInfo.lineWidth = 1.0f;
	}

	VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo = {};
	{
		const DepthStencilStateDesc& state = committed.pipelineState.depthStencilState;

		depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilStateInfo.pNext = nullptr;
		depthStencilStateInfo.flags = 0;
		depthStencilStateInfo.depthTestEnable = (state.depthTestFunc == ComparisonFunc::Always ? VK_FALSE : VK_TRUE);
		depthStencilStateInfo.depthWriteEnable = (state.depthWriteEnabled ? VK_TRUE : VK_FALSE);
		depthStencilStateInfo.depthCompareOp = LNComparisonFuncToVkCompareOp(state.depthTestFunc);
		depthStencilStateInfo.depthBoundsTestEnable = VK_FALSE;
		depthStencilStateInfo.stencilTestEnable = (state.stencilEnabled ? VK_TRUE : VK_FALSE);

		depthStencilStateInfo.front.failOp = LNStencilOpToVkStencilOp(state.frontFace.stencilFailOp);
		depthStencilStateInfo.front.passOp = LNStencilOpToVkStencilOp(state.frontFace.stencilPassOp);
		depthStencilStateInfo.front.depthFailOp = LNStencilOpToVkStencilOp(state.frontFace.stencilDepthFailOp);
		depthStencilStateInfo.front.compareOp = LNComparisonFuncToVkCompareOp(state.frontFace.stencilFunc);
		depthStencilStateInfo.front.compareMask = UINT32_MAX;
		depthStencilStateInfo.front.writeMask = UINT32_MAX;
		depthStencilStateInfo.front.reference = state.stencilReferenceValue;

		depthStencilStateInfo.back.failOp = LNStencilOpToVkStencilOp(state.backFace.stencilFailOp);
		depthStencilStateInfo.back.passOp = LNStencilOpToVkStencilOp(state.backFace.stencilPassOp);
		depthStencilStateInfo.back.depthFailOp = LNStencilOpToVkStencilOp(state.backFace.stencilDepthFailOp);
		depthStencilStateInfo.back.compareOp = LNComparisonFuncToVkCompareOp(state.backFace.stencilFunc);
		depthStencilStateInfo.back.compareMask = UINT32_MAX;
		depthStencilStateInfo.back.writeMask = UINT32_MAX;
		depthStencilStateInfo.back.reference = state.stencilReferenceValue;

		depthStencilStateInfo.minDepthBounds = 0.0f;
		depthStencilStateInfo.maxDepthBounds = 1.0f;
	}

	VkPipelineMultisampleStateCreateInfo multisampleState;
	{
		multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleState.pNext = nullptr;
		multisampleState.flags = 0;
		multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampleState.sampleShadingEnable = VK_FALSE;
		multisampleState.minSampleShading = 0.0f;
		multisampleState.pSampleMask = nullptr;
		multisampleState.alphaToCoverageEnable = VK_FALSE;
		multisampleState.alphaToOneEnable = VK_FALSE;
	}

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	std::array<VkVertexInputAttributeDescription, 16> vertexAttributeDescriptions;
	{
		VulkanVertexDeclaration* decl = static_cast<VulkanVertexDeclaration*>(committed.pipelineState.vertexDeclaration);
		VulkanShaderPass* shaderPass = static_cast<VulkanShaderPass*>(committed.pipelineState.shaderPass);

		for (size_t i = 0; i < decl->vertexAttributeTemplate().size(); i++) {
			vertexAttributeDescriptions[i] = decl->vertexAttributeTemplate()[i];
			auto& element = decl->elements()[i];
			if (!shaderPass->findAttributeLocation(element.Usage, element.UsageIndex, &vertexAttributeDescriptions[i].location)) {
				LN_NOTIMPLEMENTED();
				return false;
			}
		}

		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.pNext = nullptr;
		vertexInputInfo.flags = 0;

		vertexInputInfo.vertexBindingDescriptionCount = decl->vertexBindingDescriptions().size();
		vertexInputInfo.pVertexBindingDescriptions = decl->vertexBindingDescriptions().data();

		vertexInputInfo.vertexAttributeDescriptionCount = decl->vertexAttributeTemplate().size();
		vertexInputInfo.pVertexAttributeDescriptions = vertexAttributeDescriptions.data();
	}

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; // TODO
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	// viewport と scissor については DynamicState として Command で設定するため、 ここではダミーの値を登録しておく。
	VkPipelineViewportStateCreateInfo viewportState;
	VkViewport viewport;
	VkRect2D scissor;
	{
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = 1.0f;
		viewport.height = 1.0f;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		scissor.offset.x = 0;
		scissor.offset.y = 0;
		scissor.extent.width = 1;
		scissor.extent.height = 1;

		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.pNext = nullptr;
		viewportState.flags = 0;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;
	}


	VulkanShaderPass* shaderPass = static_cast<VulkanShaderPass*>(committed.pipelineState.shaderPass);
	if (!shaderPass) {
		shaderPass = m_deviceContext->defaultShaderPass();
	}

    VkPipelineShaderStageCreateInfo shaderStages[2];
    {
        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].pNext = nullptr;
        shaderStages[0].flags = 0;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].module = shaderPass->vertShaderModule();
        shaderStages[0].pName = "vsMain";
        shaderStages[0].pSpecializationInfo = nullptr;
        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].pNext = nullptr;
        shaderStages[1].flags = 0;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].module = shaderPass->fragShaderModule();
        shaderStages[1].pName = "psMain";
        shaderStages[1].pSpecializationInfo = nullptr;
    }

    const VkDynamicState dynamicStates[] =
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_BLEND_CONSTANTS,
        VK_DYNAMIC_STATE_STENCIL_REFERENCE,
    };
    VkPipelineDynamicStateCreateInfo dynamicState;
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pNext = nullptr;
    dynamicState.flags = 0;
    dynamicState.dynamicStateCount = LN_ARRAY_SIZE_OF(dynamicStates);
    dynamicState.pDynamicStates = dynamicStates;

	VkRenderPass renderPass = 0;
	if (!m_deviceContext->getVkRenderPass(committed.framebufferState, &renderPass)) {
		return false;
	}

	std::array<VkDescriptorSetLayout, 1> descriptorSetLayouts = { shaderPass->descriptorSetLayout() };
    VkPipelineLayoutCreateInfo pipelineLayoutInfo;
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.pNext = nullptr;
	pipelineLayoutInfo.flags = 0;
    pipelineLayoutInfo.setLayoutCount = descriptorSetLayouts.size();
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;
	LN_VK_CHECK(vkCreatePipelineLayout(m_deviceContext->vulkanDevice(), &pipelineLayoutInfo, m_deviceContext->vulkanAllocator(), &m_pipelineLayout));

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = nullptr;
    pipelineInfo.flags = 0;
	pipelineInfo.stageCount = LN_ARRAY_SIZE_OF(shaderStages);
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pTessellationState = nullptr;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizerInfo;
	pipelineInfo.pMultisampleState = &multisampleState;
    pipelineInfo.pDepthStencilState = &depthStencilStateInfo;
	pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = m_pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = 0;

	LN_VK_CHECK(vkCreateGraphicsPipelines(m_deviceContext->vulkanDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, m_deviceContext->vulkanAllocator(), &m_pipeline));

	// command

	//VkViewport vp;
	//vp.x = rect.m_x;
	//vp.y = rect.m_y;
	//vp.width = rect.m_width;
	//vp.height = rect.m_height;
	//vp.minDepth = 0.0f;
	//vp.maxDepth = 1.0f;
	//vkCmdSetViewport(m_commandBuffer, 0, 1, &vp);

	//VkRect2D rc;
	//rc.offset.x = viewScissorRect.m_x;
	//rc.offset.y = viewScissorRect.m_y;
	//rc.extent.width = viewScissorRect.m_x + viewScissorRect.m_width;
	//rc.extent.height = viewScissorRect.m_y + viewScissorRect.m_height;
	//vkCmdSetScissor(m_commandBuffer, 0, 1, &rc);

	return true;
}

void VulkanPipeline::dispose()
{
    //if (vkCreatePipelineLayout(m_deviceContext->vulkanDevice(), &pipelineLayoutInfo, m_deviceContext->vulkanAllocator(), &m_pipelineLayout) != VK_SUCCESS) {

    if (m_pipelineLayout) {
        vkDestroyPipelineLayout(m_deviceContext->vulkanDevice(), m_pipelineLayout, m_deviceContext->vulkanAllocator());
        m_pipelineLayout = 0;
    }

    if (m_pipeline) {
        vkDestroyPipeline(m_deviceContext->vulkanDevice(), m_pipeline, m_deviceContext->vulkanAllocator());
        m_pipeline = 0;
    }
}

//=============================================================================
// VulkanBuffer

Result VulkanBuffer::init(VulkanDeviceContext* deviceContext, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
    m_deviceContext = deviceContext;
	m_size = size;

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	LN_VK_CHECK(vkCreateBuffer(m_deviceContext->vulkanDevice(), &bufferInfo, m_deviceContext->vulkanAllocator(), &m_buffer));

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(m_deviceContext->vulkanDevice(), m_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = m_deviceContext->findMemoryType(memRequirements.memoryTypeBits, properties);

	LN_VK_CHECK(vkAllocateMemory(m_deviceContext->vulkanDevice(), &allocInfo, m_deviceContext->vulkanAllocator(), &m_bufferMemory));

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


//=============================================================================
// VulkanSwapChain

VulkanSwapChain::VulkanSwapChain()
	: m_currentPresentationFrameIndex(0)
{
	for (uint32_t i = 0; i < MaxPresentationFrameIndex; i++) {
		m_renderFinishedSemaphores[i] = 0;
		m_imageAvailableSemaphores[i] = 0;
		m_imageAvailableFences[i] = 0;
	}
}

VulkanSwapChain::~VulkanSwapChain()
{
}

bool VulkanSwapChain::init(VulkanDeviceContext* deviceContext, PlatformWindow* window, const SwapChainDesc& desc)
{
    m_deviceContext = deviceContext;
    m_desc = desc;

#if defined(LN_OS_WIN32)
    {
        HWND hWnd = (HWND)PlatformSupport::getWin32WindowHandle(window);
        HINSTANCE hInstance = ::GetModuleHandle(NULL);

        VkWin32SurfaceCreateInfoKHR info = {};
        info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        info.pNext = nullptr;
        info.flags = 0;
        info.hinstance = hInstance;
        info.hwnd = hWnd;

		LN_VK_CHECK(vkCreateWin32SurfaceKHR(m_deviceContext->vulkanInstance(), &info, nullptr, &m_surface));
    }
#endif

    VulkanQueue* graphicsQueue = m_deviceContext->graphicsQueue();

    VkDevice vulkanDevice = m_deviceContext->vulkanDevice();
    VkPhysicalDevice vulkanPhysicalDevice = m_deviceContext->vulkanPhysicalDevice();

    // Check format
    m_imageFormat = VK_FORMAT_UNDEFINED;
    m_colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    {
        // 事前に取り出しておいた GraphicsQueue が、Present をサポートするかを確認する
        // Note: 丁寧にやるなら、もう一度すべての Queue を列挙して調べなおすのがよい。
        //       ただし、チュートリアルにもあるように、ほとんどのケースでは同じ Queue が選択される。
        //       https://vulkan-tutorial.com/Drawing_a_triangle/Presentation/Window_surface
        auto familyIndex = graphicsQueue->familyIndex();
        VkBool32 support = VK_FALSE;
		LN_VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(vulkanPhysicalDevice, familyIndex, m_surface, &support));
        if (support == VK_FALSE) {
            LN_LOG_ERROR << "Failed vkGetPhysicalDeviceSurfaceSupportKHR unsupported";
            return false;
        }

        uint32_t srfaceFormatCount;
		LN_VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(vulkanPhysicalDevice, m_surface, &srfaceFormatCount, nullptr));

        m_surfaceFormats.resize(srfaceFormatCount);
        if (vkGetPhysicalDeviceSurfaceFormatsKHR(vulkanPhysicalDevice, m_surface, &srfaceFormatCount, m_surfaceFormats.data()) != VK_SUCCESS) {
            LN_LOG_ERROR << "Failed vkGetPhysicalDeviceSurfaceFormatsKHR";
            return false;
        }

        //bool found = false;

        auto nativeFormat = LNFormatToVkFormat(m_desc.Format);
        auto nativeColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

        for (int i = 0; i < m_surfaceFormats.size(); i++) {
            if (nativeFormat == m_surfaceFormats[i].format &&
                nativeColorSpace == m_surfaceFormats[i].colorSpace) {
                m_imageFormat = m_surfaceFormats[i].format;
                m_colorSpace = m_surfaceFormats[i].colorSpace;
                //found = true;
                break;
            }
        }

        // VK_FORMAT_UNDEFINED のままでは vkCreateSwapchainKHR が強制終了する
        if (m_imageFormat == VK_FORMAT_UNDEFINED) {
            m_imageFormat = m_surfaceFormats[0].format;
            m_colorSpace = m_surfaceFormats[0].colorSpace;
            m_desc.vulkanFormat = m_imageFormat;  // TODO: 多重管理辞めたい
            LN_LOG_WARNING << "Requested format is not supported.";
        }

        //if (!found) {
        //    LN_LOG_ERROR << "Not found requested format";
        //    return false;
        //}
    }

    // Check buffer caps
    {
        VkSurfaceCapabilitiesKHR capabilities;
		LN_VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkanPhysicalDevice, m_surface, &capabilities));

        if (capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
            m_preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        } else {
            m_preTransform = capabilities.currentTransform;
        }

        if (capabilities.maxImageCount < m_desc.BufferCount) {
            LN_LOG_ERROR << "Invalid buffer count";
            return false;
        }

        if (capabilities.maxImageExtent.width < m_desc.Width) {
            m_desc.Width = capabilities.maxImageExtent.width;
        }
        if (capabilities.maxImageExtent.height < m_desc.Height) {
            m_desc.Height = capabilities.maxImageExtent.height;
        }

        if (1)
        {
            // SwapChain の Image を直接読み取るには VK_IMAGE_USAGE_TRANSFER_SRC_BIT が必要
            // https://stackoverflow.com/questions/38985094/how-to-copy-swap-chain-image-to-a-vkbuffer-in-vulkan
            if (capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) LN_LOG_INFO << "VK_IMAGE_USAGE_TRANSFER_SRC_BIT";
            if (capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) LN_LOG_INFO << "VK_IMAGE_USAGE_TRANSFER_DST_BIT";
            if (capabilities.supportedUsageFlags & VK_IMAGE_USAGE_SAMPLED_BIT) LN_LOG_INFO << "VK_IMAGE_USAGE_SAMPLED_BIT";
            if (capabilities.supportedUsageFlags & VK_IMAGE_USAGE_STORAGE_BIT) LN_LOG_INFO << "VK_IMAGE_USAGE_STORAGE_BIT";
            if (capabilities.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) LN_LOG_INFO << "VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT";
            if (capabilities.supportedUsageFlags & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) LN_LOG_INFO << "VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT";
            if (capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT) LN_LOG_INFO << "VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT";
            if (capabilities.supportedUsageFlags & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT) LN_LOG_INFO << "VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT";
        }
    }

    // Select present mode
    {
        m_presentMode = VK_PRESENT_MODE_FIFO_KHR;

        uint32_t presentModeCount;
		LN_VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(vulkanPhysicalDevice, m_surface, &presentModeCount, nullptr));

        std::vector<VkPresentModeKHR> presentModes(presentModeCount);
		LN_VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(vulkanPhysicalDevice, m_surface, &presentModeCount, presentModes.data()));

        bool found = false;
        for (uint32_t i = 0; i < presentModeCount; i++) {
            if (m_desc.SyncInterval == 0) {
                if (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR) {
                    m_presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
                    found = true;
                    break;
                }
            } else if (m_desc.SyncInterval == -1) {
                if (presentModes[i] == VK_PRESENT_MODE_FIFO_RELAXED_KHR) {
                    m_presentMode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
                    found = true;
                    break;
                }
            } else {
                if (presentModes[i] == VK_PRESENT_MODE_FIFO_KHR) {
                    m_presentMode = VK_PRESENT_MODE_FIFO_KHR;
                    found = true;
                    break;
                }
            }
        }

        if (!found) {
            LN_LOG_ERROR << "Not found present mode";
            return false;
        }
    }

    // Create swap chain
    {
        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.surface = m_surface;
        createInfo.minImageCount = m_desc.BufferCount;
        createInfo.imageFormat = m_imageFormat;
        createInfo.imageColorSpace = m_colorSpace;
        createInfo.imageExtent = {m_desc.Width, m_desc.Height};
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
        createInfo.preTransform = m_preTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = m_presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = 0;

		LN_VK_CHECK(vkCreateSwapchainKHR(vulkanDevice, &createInfo, nullptr, &m_swapChain));
    }

    // Get swap chain images
    std::vector<VkImage> images;
    std::vector<VkImageView> imageViews;
    {
        uint32_t chainCount;
		LN_VK_CHECK(vkGetSwapchainImagesKHR(vulkanDevice, m_swapChain, &chainCount, nullptr));
        if (chainCount != m_desc.BufferCount) {
            LN_LOG_ERROR << "Invalid chain count";
            return false;
        }

        images.resize(chainCount);
        imageViews.resize(chainCount);

		LN_VK_CHECK(vkGetSwapchainImagesKHR(vulkanDevice, m_swapChain, &chainCount, images.data()));

        // ここで取り出した Image は VkSwapchainKHR が破棄されると自動的にクリーンアップされるので、クリーンアップコードを追加する必要はない。
    }

    // イメージビューを生成.
    {
        VkImageSubresourceRange range = {};
        range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        range.baseMipLevel = 0;
        range.layerCount = 1;
        range.baseArrayLayer = 0;
        range.levelCount = m_desc.MipLevels;

        for (auto i = 0u; i < m_desc.BufferCount; ++i) {
            VkImageViewCreateInfo viewInfo = {};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.pNext = nullptr;
            viewInfo.flags = 0;
            viewInfo.image = images[i];
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = m_imageFormat;
            viewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
            viewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
            viewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
            viewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
            viewInfo.subresourceRange = range;

			LN_VK_CHECK(vkCreateImageView(vulkanDevice, &viewInfo, nullptr, &imageViews[i]));
        }
    }

    // バックバッファに Texture としてアクセスできるようにラップしたインスタンスを作っておく
    {
        m_colorBuffer = makeRef<VulkanSwapchainRenderTargetTexture>();
        m_colorBuffer->init(m_deviceContext);
        m_colorBuffer->reset(m_desc, images, imageViews);
    }

    // Change image layout
    // https://sites.google.com/site/monshonosuana/vulkan/vulkan_002
    {
        auto commandBuffer = makeRef<VulkanCommandList>();
        if (!commandBuffer->init(m_deviceContext, VulkanCommandList::Type::COMMANDLIST_TYPE_DIRECT)) {
            return false;
        }

        commandBuffer->begin();
        VkCommandBuffer cmdBuffer = commandBuffer->vulkanCommandBuffer();

        for (auto i = 0u; i < m_desc.BufferCount; ++i) {
            VkImageMemoryBarrier barrier = {};
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.pNext = nullptr;
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
            barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            barrier.srcQueueFamilyIndex = 0;
            barrier.dstQueueFamilyIndex = 0;
            barrier.image = m_colorBuffer->vulkanImage(i);

            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.baseMipLevel = 0;
            barrier.subresourceRange.layerCount = m_colorBuffer->desc().DepthOrArraySize;
            barrier.subresourceRange.levelCount = m_colorBuffer->desc().MipLevels;

            vkCmdPipelineBarrier(
                cmdBuffer,
                VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                0,
                0,
                nullptr,
                0,
                nullptr,
                1,
                &barrier);
        }

        commandBuffer->end();
        commandBuffer->flush();
        commandBuffer->dispose();
    }

	{
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreInfo.pNext = nullptr;
		semaphoreInfo.flags = 0;

		for (int i = 0; i < MaxPresentationFrameIndex; i++) {
			LN_VK_CHECK(vkCreateSemaphore(vulkanDevice, &semaphoreInfo, m_deviceContext->vulkanAllocator(), &m_renderFinishedSemaphores[i]));
			LN_VK_CHECK(vkCreateSemaphore(vulkanDevice, &semaphoreInfo, m_deviceContext->vulkanAllocator(), &m_imageAvailableSemaphores[i]));
		}


		for (auto i = 0; i < MaxPresentationFrameIndex; ++i) {
			VkFenceCreateInfo fenceInfo = {};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.pNext = nullptr;
			fenceInfo.flags = 0;
			LN_VK_CHECK(vkCreateFence(vulkanDevice, &fenceInfo, nullptr, &m_imageAvailableFences[i]));
			LN_VK_CHECK(vkResetFences(vulkanDevice, 1, &m_imageAvailableFences[i]));
		}
	}


    m_inactiveCommandBuffer = makeRef<VulkanCommandList>();
    if (!m_inactiveCommandBuffer->init(m_deviceContext, VulkanCommandList::Type::COMMANDLIST_TYPE_DIRECT)) {
        return false;
    }

    // フレーム開始時に一度 vkAcquireNextImageKHR を実行しているが、
    // これは公式チュートリアルや bgfx とは違うところ。asura-sdk と同じ。
    // これをしない場合は present の先頭で行うことになるが、そうすると present までは
    // バックバッファを示す Image がわからない。そのため、OpenGL のように
    // 0(nullptr) を「デフォルトのフレームバッファとする」というような対応をとる必要がある。
    // この方式にしてしまうと GraphicsContext と SwapChain が強く結びついてしまうので、
    // OpenGL 対応で複数 SwapChain サポートに苦労したときと同じことになってしまう。
    if (!acquireNextImage()) {
        return false;
    }

    return true;
}

void VulkanSwapChain::dispose()
{
    if (m_swapChain) {
        vkDestroySwapchainKHR(m_deviceContext->vulkanDevice(), m_swapChain, m_deviceContext->vulkanAllocator());
        m_swapChain = 0;
    }

    if (m_surface) {
        vkDestroySurfaceKHR(m_deviceContext->vulkanInstance(), m_surface, m_deviceContext->vulkanAllocator());
        m_surface = 0;
    }

	for (uint32_t i = 0; i < MaxPresentationFrameIndex; i++) {
		if (m_renderFinishedSemaphores[i]) {
			vkDestroySemaphore(m_deviceContext->vulkanDevice(), m_renderFinishedSemaphores[i], m_deviceContext->vulkanAllocator());
			m_renderFinishedSemaphores[i] = 0;
		}
		if (m_imageAvailableSemaphores[i]) {
			vkDestroySemaphore(m_deviceContext->vulkanDevice(), m_imageAvailableSemaphores[i], m_deviceContext->vulkanAllocator());
			m_imageAvailableSemaphores[i] = 0;
		}
		if (m_imageAvailableFences[i]) {
			vkDestroyFence(m_deviceContext->vulkanDevice(), m_imageAvailableFences[i], m_deviceContext->vulkanAllocator());
			m_imageAvailableFences[i] = 0;
		}
	}

    ISwapChain::dispose();
}

ITexture* VulkanSwapChain::getColorBuffer() const
{
    return m_colorBuffer;
}

bool VulkanSwapChain::present()
{
    if (!m_deviceContext->endActiveCommandBuffer()) {
        return false;
    }

	VkDevice vulkanDevice = m_deviceContext->vulkanDevice();
	VkQueue graphicsQueue = m_deviceContext->graphicsQueue()->vulkanQueue();

	// 前回この SwapChain で実行要求したコマンドの完了を待機する
	{
		VkFence inFlightFence = m_inactiveCommandBuffer->vulkanInFlightFence();
		vkWaitForFences(vulkanDevice, 1, &inFlightFence, VK_TRUE, std::numeric_limits<uint64_t>::max());
		vkResetFences(vulkanDevice, 1, &inFlightFence);
	}

    // Submit
    {
        if (!m_deviceContext->graphicsQueue()->submit(
            m_deviceContext->activeCommandBuffer(),
            m_imageAvailableSemaphores[m_currentPresentationFrameIndex],
            m_renderFinishedSemaphores[m_currentPresentationFrameIndex])) {
            return false;
        }
    }

	// Present
	{
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &m_renderFinishedSemaphores[m_currentPresentationFrameIndex];  // Present を発行する前に待機するセマフォ
		presentInfo.pNext = nullptr;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_swapChain;
		presentInfo.pImageIndices = &m_currentBufferIndex;
		presentInfo.pResults = nullptr;
		LN_VK_CHECK(vkQueuePresentKHR(graphicsQueue, &presentInfo));
	}

	// Swap
	{
		auto t = m_deviceContext->activeCommandBuffer();
		m_deviceContext->setActiveCommandBuffer(m_inactiveCommandBuffer);
		m_inactiveCommandBuffer = t;
	
		m_currentPresentationFrameIndex = (m_currentPresentationFrameIndex + 1) % MaxPresentationFrameIndex;
        
        if (!acquireNextImage()) {
            return false;
        }
    }


	if (!m_deviceContext->beginActiveCommandBuffer()) {
		return false;
	}

    return true;
}

bool VulkanSwapChain::acquireNextImage()
{
    VulkanQueue* graphicsQueue = m_deviceContext->graphicsQueue();
    VkDevice vulkanDevice = m_deviceContext->vulkanDevice();

    // Get current backbuffer index
    {
		// SwapChain の Image の準備ができたら通知する Semaphore と Fence
		VkSemaphore semaphore = m_imageAvailableSemaphores[m_currentPresentationFrameIndex];
		VkFence fence = m_imageAvailableFences[m_currentPresentationFrameIndex];

        // 毎フレーム行わないと、次の VkQueueSubmit で VK_ERROR_DEVICE_LOST する
		LN_VK_CHECK(vkAcquireNextImageKHR(vulkanDevice, m_swapChain, UINT64_MAX, semaphore, fence, &m_currentBufferIndex));

        m_colorBuffer->setBufferIndex(m_currentBufferIndex);

        // TODO: ここで待つのは効率悪い。
        // 本当はバックバッファを示す RenderTarget を CommandBuffer に Add するとき、
        // その直前で WaitForFence を挿入するようなことができればベスト。
		// なので、現状 vkQueueSubmit で m_imageAvailableSemaphores の完了を待っているが、それは意味がないことになる。
        vkWaitForFences(vulkanDevice, 1, &fence, VK_FALSE, UINT64_MAX);
        vkResetFences(vulkanDevice, 1, &fence);
    }

	return true;
}


//==============================================================================
// VulkanVertexDeclaration

VulkanVertexDeclaration::VulkanVertexDeclaration()
{
}

VulkanVertexDeclaration::~VulkanVertexDeclaration()
{
}

bool VulkanVertexDeclaration::init(const VertexElement* elements, int elementsCount)
{
	m_maxStreamCount = 0;
    for (int i = 0; i < elementsCount; i++) {
		m_maxStreamCount = std::max(m_maxStreamCount, elements[i].StreamIndex);
		m_elements.push_back(elements[i]);
    }
    m_maxStreamCount++;
    m_bindings.resize(m_maxStreamCount);

    //uint32_t loc = 0;
    for (int i = 0; i < elementsCount; i++) {
		auto& element = m_elements[i];

        VkVertexInputAttributeDescription attr;
		attr.location = 0;
        attr.binding = elements[i].StreamIndex;
        attr.format = LNVertexElementTypeToVkFormat(elements[i].Type);
        attr.offset = m_bindings[attr.binding].stride;
		m_attributeTemplate.push_back(attr);


        m_bindings[element.StreamIndex].stride += GraphicsHelper::getVertexElementTypeSize(elements[i].Type);

		//loc++;

		

        // TODO: Lumino のシェーダとしては、location と Semantics の対応を固定してもいいかもしれない。
        // たとえば、location=0 は POSITION0 とか。
    }

    return true;
}

void VulkanVertexDeclaration::dispose()
{
    IVertexDeclaration::dispose();
}

//==============================================================================
// VulkanVertexBuffer

VulkanVertexBuffer::VulkanVertexBuffer()
{
}

VulkanVertexBuffer::~VulkanVertexBuffer()
{
}

Result VulkanVertexBuffer::init(VulkanDeviceContext* deviceContext, GraphicsResourceUsage usage, size_t bufferSize, const void* initialData)
{
	m_usage = usage;

	VkBufferUsageFlags vkusage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	if (usage == GraphicsResourceUsage::Dynamic) {
		vkusage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	}

	if (!m_buffer.init(deviceContext, bufferSize, vkusage, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
		return false;
	}

    if (initialData)
    {
        void* data;
        vkMapMemory(deviceContext->vulkanDevice(), vulkanDeviceMemory(), 0, bufferSize, 0, &data);
        memcpy(data, initialData, bufferSize);
        vkUnmapMemory(deviceContext->vulkanDevice(), vulkanDeviceMemory());
    }

    return true;
}

void VulkanVertexBuffer::dispose()
{
	m_buffer.dispose();
    IVertexBuffer::dispose();
}

void VulkanVertexBuffer::setSubData(size_t offset, const void* data, size_t length)
{
    LN_NOTIMPLEMENTED();
}

void* VulkanVertexBuffer::map()
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

void VulkanVertexBuffer::unmap()
{
    LN_NOTIMPLEMENTED();
}

//==============================================================================
// VulkanIndexBuffer

VulkanIndexBuffer::VulkanIndexBuffer()
{
}

VulkanIndexBuffer::~VulkanIndexBuffer()
{
}

Result VulkanIndexBuffer::init(VulkanDeviceContext* deviceContext, GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData)
{
	size_t stride = (format == IndexBufferFormat::UInt16) ? 2 : 4;
	size_t bufferSize = stride * indexCount;
	m_usage = usage;

	VkBufferUsageFlags vkusage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	if (usage == GraphicsResourceUsage::Dynamic) {
		vkusage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	}

	if (!m_buffer.init(deviceContext, bufferSize, vkusage, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
		return false;
	}

	if (initialData)
	{
		void* data;
		vkMapMemory(deviceContext->vulkanDevice(), vulkanDeviceMemory(), 0, bufferSize, 0, &data);
		memcpy(data, initialData, bufferSize);
		vkUnmapMemory(deviceContext->vulkanDevice(), vulkanDeviceMemory());
	}

	return true;
}

void VulkanIndexBuffer::dispose()
{
	m_buffer.dispose();
    IIndexBuffer::dispose();
}

void VulkanIndexBuffer::setSubData(size_t offset, const void* data, size_t length)
{
    LN_NOTIMPLEMENTED();
}

void* VulkanIndexBuffer::map()
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

void VulkanIndexBuffer::unmap()
{
    LN_NOTIMPLEMENTED();
}

//=============================================================================
// VulkanTextureBase

VulkanTextureBase::VulkanTextureBase()
{
}

VulkanTextureBase::~VulkanTextureBase()
{
}

//=============================================================================
// VulkanTexture2D

VulkanTexture2D::VulkanTexture2D()
{
}

VulkanTexture2D::~VulkanTexture2D()
{
}

bool VulkanTexture2D::init(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
    LN_NOTIMPLEMENTED();
    return true;
}

void VulkanTexture2D::dispose()
{
    VulkanTextureBase::dispose();
}

DeviceTextureType VulkanTexture2D::type() const
{
    return DeviceTextureType::Texture2D;
}

void VulkanTexture2D::readData(void* outData)
{
    LN_UNREACHABLE();
}

SizeI VulkanTexture2D::realSize()
{
    LN_NOTIMPLEMENTED();
    return SizeI::Zero;
}

TextureFormat VulkanTexture2D::getTextureFormat() const
{
    LN_NOTIMPLEMENTED();
    return TextureFormat::Unknown;
}

void VulkanTexture2D::setSubData(int x, int y, int width, int height, const void* data, size_t dataSize)
{
    LN_NOTIMPLEMENTED();
}

void VulkanTexture2D::setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize)
{
    LN_UNREACHABLE();
}

//=============================================================================
// VulkanTexture3D

VulkanTexture3D::VulkanTexture3D()
{
}

VulkanTexture3D::~VulkanTexture3D()
{
}

void VulkanTexture3D::init(uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
    LN_NOTIMPLEMENTED();
}

void VulkanTexture3D::dispose()
{
    LN_NOTIMPLEMENTED();
    VulkanTextureBase::dispose();
}

DeviceTextureType VulkanTexture3D::type() const
{
    return DeviceTextureType::Texture3D;
}

void VulkanTexture3D::readData(void* outData)
{
    LN_UNREACHABLE();
}

SizeI VulkanTexture3D::realSize()
{
    LN_NOTIMPLEMENTED();
    return SizeI::Zero;
}

TextureFormat VulkanTexture3D::getTextureFormat() const
{
    LN_NOTIMPLEMENTED();
    return TextureFormat::Unknown;
}

void VulkanTexture3D::setSubData(int x, int y, int width, int height, const void* data, size_t dataSize)
{
    LN_UNREACHABLE();
}

void VulkanTexture3D::setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize)
{
    LN_NOTIMPLEMENTED();
}

//=============================================================================
// VulkanRenderTargetTexture

VulkanRenderTargetTexture::VulkanRenderTargetTexture()
{
}

VulkanRenderTargetTexture::~VulkanRenderTargetTexture()
{
}

void VulkanRenderTargetTexture::init(VulkanDeviceContext* context, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap)
{
	m_deviceContext = context;
    LN_NOTIMPLEMENTED();
}

void VulkanRenderTargetTexture::dispose()
{
	if (m_deviceContext) {
		m_deviceContext->frameBufferCache().invalidateRenderTarget(this);
	}

	if (!m_isExternal) {
		if (m_image) {
			vkDestroyImage(m_deviceContext->vulkanDevice(), m_image, nullptr);
			m_image = 0;
		}

		if (m_deviceMemory) {
			vkFreeMemory(m_deviceContext->vulkanDevice(), m_deviceMemory, nullptr);
			m_deviceMemory = 0;
		}
	}
	else {
		m_image = 0;
		m_deviceMemory = 0;
	}

    VulkanTextureBase::dispose();
}

DeviceTextureType VulkanRenderTargetTexture::type() const
{
    return DeviceTextureType::RenderTarget;
}

void VulkanRenderTargetTexture::readData(void* outData)
{
    LN_NOTIMPLEMENTED();
}

SizeI VulkanRenderTargetTexture::realSize()
{
    return SizeI(m_desc.Width, m_desc.Height);
}

TextureFormat VulkanRenderTargetTexture::getTextureFormat() const
{
    return m_format;
}

void VulkanRenderTargetTexture::setSubData(int x, int y, int width, int height, const void* data, size_t dataSize)
{
    LN_UNREACHABLE();
}

void VulkanRenderTargetTexture::setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize)
{
    LN_UNREACHABLE();
}

//void VulkanRenderTargetTexture::resestSwapchainFrame(VkImage image, VkImageView view)
//{
//    LN_CHECK(m_isExternal);
//    m_image = image;
//    m_imageView = view;
//}

//void VulkanRenderTargetTexture::transitionImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
//{
//    VkImageMemoryBarrier barrier = {};
//    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
//    barrier.oldLayout = oldLayout;
//    barrier.newLayout = newLayout;
//    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
//    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
//    barrier.image = image;
//    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//    barrier.subresourceRange.baseMipLevel = 0;
//    barrier.subresourceRange.levelCount = 1;
//    barrier.subresourceRange.baseArrayLayer = 0;
//    barrier.subresourceRange.layerCount = 1;
//
//    VkPipelineStageFlags sourceStage;
//    VkPipelineStageFlags destinationStage;
//
//    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
//        barrier.srcAccessMask = 0;
//        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
//
//        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
//        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
//    }
//    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
//        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;   // ホスト側によってメモリ書き込みされた (かもしれない)
//        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
//
//        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
//        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
//    } 
//    else if (oldLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
//        barrier.srcAccessMask = 0;
//        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
//
//        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
//        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
//    }
//    else {
//        LN_ERROR("unsupported layout transition!");
//        return;
//    }
//
//    vkCmdPipelineBarrier(
//        commandBuffer,
//        sourceStage, destinationStage,
//        0,
//        0, nullptr,
//        0, nullptr,
//        1, &barrier
//    );
//}


//=============================================================================
// VulkanSwapchainRenderTargetTexture

VulkanSwapchainRenderTargetTexture::VulkanSwapchainRenderTargetTexture()
{
}

VulkanSwapchainRenderTargetTexture::~VulkanSwapchainRenderTargetTexture()
{
}

bool VulkanSwapchainRenderTargetTexture::init(VulkanDeviceContext* deviceContext)
{
    m_deviceContext = deviceContext;
    m_bufferIndex = 0;
    return true;
}

void VulkanSwapchainRenderTargetTexture::dispose()
{
    m_images.clear();
    m_imageViews.clear();
    VulkanTextureBase::dispose();
}

void VulkanSwapchainRenderTargetTexture::reset(const VulkanSwapChain::SwapChainDesc& desc, std::vector<VkImage> images, std::vector<VkImageView> views)
{
    m_images = std::move(images);
    m_imageViews = std::move(views);
    //m_imageAspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
    //m_deviceMemory = 0;
    //m_Desc.Dimension = RESOURCE_DIMENSION_TEXTURE2D;
    m_desc.Width = desc.Width;
    m_desc.Height = desc.Height;
    m_desc.DepthOrArraySize = 1;
    //m_Desc.Format = desc.Format;
    m_desc.MipLevels = desc.MipLevels;
    //m_Desc.SampleCount = desc.SampleCount;
    //m_Desc.Layout = RESOURCE_LAYOUT_OPTIMAL;
    //m_Desc.InitState = RESOURCE_STATE_UNKNOWN;
    //m_Desc.HeapProperty.Type = HEAP_TYPE_DEFAULT;
    //m_Desc.HeapProperty.CpuPageProperty = CPU_PAGE_PROPERTY_NOT_AVAILABLE;


    m_format = desc.Format;//VkFormatToLNFormat(desc.Format);
    m_vulkanFormat = desc.vulkanFormat;

}

DeviceTextureType VulkanSwapchainRenderTargetTexture::type() const
{
    return DeviceTextureType::RenderTarget;
}

void VulkanSwapchainRenderTargetTexture::readData(void* outData)
{
    VkDevice device = m_deviceContext->vulkanDevice();

    bool supportsBlit = true;
    {
        // Check blit support for source and destination
        VkFormatProperties formatProps;

        // Check if the device supports blitting from optimal images (the swapchain images are in optimal format)
        vkGetPhysicalDeviceFormatProperties(m_deviceContext->vulkanPhysicalDevice(), m_vulkanFormat, &formatProps);
        if (!(formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT)) {
            std::cerr << "Device does not support blitting from optimal tiled images, using copy instead of blit!" << std::endl;
            supportsBlit = false;
        }

        // Check if the device supports blitting to linear images 
        vkGetPhysicalDeviceFormatProperties(m_deviceContext->vulkanPhysicalDevice(), VK_FORMAT_R8G8B8A8_UNORM, &formatProps);
        if (!(formatProps.linearTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT)) {
            std::cerr << "Device does not support blitting to linear tiled images, using copy instead of blit!" << std::endl;
            supportsBlit = false;
        }
    }

    VkDeviceSize size = m_desc.Width * m_desc.Height * 4; // TODO
    uint32_t width = m_desc.Width;
    uint32_t height = m_desc.Height;
    VkImage srcImage = m_images[prevBufferIndex()];

    auto dstImageMemory = makeRef<VulkanBuffer>();
    {

        // とりあえず buffer はいらないが、MemoryBuffer がほしい
        dstImageMemory->init(
            m_deviceContext, size,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }

    //VulkanCommandList* commandBuffer = m_deviceContext->activeCommandBuffer();
    //auto commandBuffer = makeRef<VulkanCommandList>();
    //commandBuffer->init(m_deviceContext, VulkanCommandList::Type::COMMANDLIST_TYPE_DIRECT);
    //VkCommandBuffer copyCmd = commandBuffer->vulkanCommandBuffer();
    //commandBuffer->begin();

    VkCommandBuffer copyCmd = 0;
    {
        VkCommandBufferAllocateInfo cmdBufAllocateInfo = {};
        cmdBufAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cmdBufAllocateInfo.commandPool = m_deviceContext->activeCommandBuffer()->vulkanCommandPool();
        cmdBufAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        cmdBufAllocateInfo.commandBufferCount = 1;

        vkAllocateCommandBuffers(device, &cmdBufAllocateInfo, &copyCmd);

        // If requested, also start recording for the new command buffer
        if (1)
        {
            VkCommandBufferBeginInfo cmdBufInfo = {};
            cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            vkBeginCommandBuffer(copyCmd, &cmdBufInfo);
        }

    }


    // transitionImageLayout
    {
        VkImageMemoryBarrier imageMemoryBarrier = {};
        imageMemoryBarrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        imageMemoryBarrier.image = srcImage;
        imageMemoryBarrier.subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

        vkCmdPipelineBarrier(
            copyCmd,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            0,
            0, nullptr,
            0, nullptr,
            1, &imageMemoryBarrier);
    }


    auto buffer = makeRef<VulkanBuffer>();
    buffer->init(
        m_deviceContext, size,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT,   // 転送先として作成
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    {
        VkBufferImageCopy region = {};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = {
            m_desc.Width,
            m_desc.Height,
            1
        };

        vkCmdCopyImageToBuffer(
            copyCmd,
            srcImage,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            buffer->vulkanBuffer(),
            1, &region);
    }


    // transitionImageLayout
    {
        VkImageMemoryBarrier imageMemoryBarrier = {};
        imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        imageMemoryBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        imageMemoryBarrier.image = srcImage;
        imageMemoryBarrier.subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

        vkCmdPipelineBarrier(
            copyCmd,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            0,
            0, nullptr,
            0, nullptr,
            1, &imageMemoryBarrier);
    }



    {
        vkEndCommandBuffer(copyCmd);
    }

    // Submit
    {
        auto queue = m_deviceContext->graphicsQueue()->vulkanQueue();

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &copyCmd;

        // Create fence to ensure that the command buffer has finished executing
        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = 0;
        VkFence fence = 0;;
        vkCreateFence(device, &fenceInfo, nullptr, &fence);


        // Submit to the queue
        auto r = vkQueueSubmit(queue, 1, &submitInfo, fence);
        // Wait for the fence to signal that command buffer has finished executing
        vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);

        vkDestroyFence(device, fence, nullptr);

        //m_deviceContext->graphicsQueue()->submit(
        //    commandBuffer,
        //    VK_NULL_HANDLE,
        //    VK_NULL_HANDLE);

        //// wait
        //vkQueueWaitIdle(m_deviceContext->graphicsQueue()->vulkanQueue());
    }


    {

        void* rawData;
        vkMapMemory(m_deviceContext->vulkanDevice(), buffer->vulkanBufferMemory(), 0, size, 0, &rawData);


        // TODO: まだ Bitmap クラス側が BGRA の save に対応していないのでここで変換してしまう。
        if (m_vulkanFormat == VK_FORMAT_B8G8R8A8_UNORM) {
            unsigned char* data = (unsigned char*)rawData;
            for (uint32_t y = 0; y < height; y++)
            {
                unsigned char *row = data;
                for (uint32_t x = 0; x < width; x++)
                {
                    std::swap(row[0], row[2]);
                    row += 4;
                }
                data += width * 4;//subResourceLayout.rowPitch;
            }
        }

        memcpy(outData, rawData, static_cast<size_t>(size));
        vkUnmapMemory(m_deviceContext->vulkanDevice(), buffer->vulkanBufferMemory());
    }

    //buffer->dispose();


    //transitionImageLayout(commandBuffer->vulkanCommandBuffer(), m_image, m_vulkanFormat, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

    //commandBuffer->dispose();



    dstImageMemory->dispose();
}

SizeI VulkanSwapchainRenderTargetTexture::realSize()
{
    return SizeI(m_desc.Width, m_desc.Height);
}

TextureFormat VulkanSwapchainRenderTargetTexture::getTextureFormat() const
{
    return m_format;
}

void VulkanSwapchainRenderTargetTexture::setSubData(int x, int y, int width, int height, const void* data, size_t dataSize)
{
    LN_UNREACHABLE();
}

void VulkanSwapchainRenderTargetTexture::setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize)
{
    LN_UNREACHABLE();
}

//=============================================================================
// VulkanDepthBuffer

VulkanDepthBuffer::VulkanDepthBuffer()
{
}

VulkanDepthBuffer::~VulkanDepthBuffer()
{
}

void VulkanDepthBuffer::init(VulkanDeviceContext* context, uint32_t width, uint32_t height)
{
	m_deviceContext = context;
}

void VulkanDepthBuffer::dispose()
{
	if (m_deviceContext) {
		m_deviceContext->frameBufferCache().invalidateDepthBuffer(this);
	}

    IDepthBuffer::dispose();
}

//=============================================================================
// VulkanSamplerState

VulkanSamplerState::VulkanSamplerState()
{
}

VulkanSamplerState::~VulkanSamplerState()
{
}

void VulkanSamplerState::init(const SamplerStateData& desc)
{
    LN_NOTIMPLEMENTED();
}

void VulkanSamplerState::dispose()
{
    LN_NOTIMPLEMENTED();
    ISamplerState::dispose();
}

//=============================================================================
// VulkanShaderPass

VulkanShaderPass::VulkanShaderPass()
	: m_deviceContext(nullptr)
	, m_vertShaderModule(0)
	, m_fragShaderModule(0)
{
}

VulkanShaderPass::~VulkanShaderPass()
{
}

bool VulkanShaderPass::init(VulkanDeviceContext* context, const void* spvVert, size_t spvVertLen, const void* spvFrag, size_t spvFragLen, const ShaderVertexInputAttributeTable* attributeTable, const UnifiedShaderRefrectionInfo* refrection)
{
	m_deviceContext = context;
	VkDevice device = m_deviceContext->vulkanDevice();

	{
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = spvVertLen;
		createInfo.pCode = reinterpret_cast<const uint32_t*>(spvVert);

		LN_VK_CHECK(vkCreateShaderModule(device, &createInfo, m_deviceContext->vulkanAllocator(), &m_vertShaderModule));
	}

	{
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = spvFragLen;
		createInfo.pCode = reinterpret_cast<const uint32_t*>(spvFrag);

		LN_VK_CHECK(vkCreateShaderModule(device, &createInfo, m_deviceContext->vulkanAllocator(), &m_fragShaderModule));
	}

	if (attributeTable) {
		m_inputAttributeTable = *attributeTable;
	}

	// createDescriptorSetLayout
	{

#if 1
		std::vector<VkDescriptorSetLayoutBinding> bindings;
		for (int i = 0; i < x; i++)
		{
			VkDescriptorSetLayoutBinding uboLayoutBinding;
			uboLayoutBinding.binding = i;			// TODO: GLSL で layout(binding = 0) とか書かれたやつのインデックス？
			uboLayoutBinding.descriptorCount = 1;	// バッファ自体の要素数。主に sampler で使う。https://vulkan.lunarg.com/doc/view/1.0.33.0/linux/vkspec.chunked/ch13s02.html
			uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; // TODO: or VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
			uboLayoutBinding.pImmutableSamplers = nullptr;
			uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		}

		VkDescriptorSetLayoutCreateInfo layoutInfo;
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.pNext = nullptr;
		layoutInfo.flags = 0;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		if (vkCreateDescriptorSetLayout(device, &layoutInfo, m_deviceContext->vulkanAllocator(), &m_descriptorSetLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor set layout!");
		}
#endif
		/*
		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.pImmutableSamplers = nullptr;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor set layout!");
		}
		*/
	}
    if (refrection)
    {
        for (auto& info : refrection->buffers)
        {
            auto buf = makeRef<VulkanShaderUniformBuffer>();
            if (!buf->init(info)) {
                return false;
            }
            m_uniformBuffers.push_back(buf);
        }
    }

	return true;
}

void VulkanShaderPass::dispose()
{
	VkDevice device = m_deviceContext->vulkanDevice();

	if (m_descriptorSetLayout) {
		vkDestroyDescriptorSetLayout(device, m_descriptorSetLayout, m_deviceContext->vulkanAllocator());
		m_descriptorSetLayout = 0;
	}

	if (m_vertShaderModule) {
		vkDestroyShaderModule(device, m_vertShaderModule, m_deviceContext->vulkanAllocator());
		m_vertShaderModule = 0;
	}

	if (m_fragShaderModule) {
		vkDestroyShaderModule(device, m_fragShaderModule, m_deviceContext->vulkanAllocator());
		m_fragShaderModule = 0;
	}

    IShaderPass::dispose();
}

bool VulkanShaderPass::findAttributeLocation(VertexElementUsage usage, uint32_t usageIndex, uint32_t* outLocation) const
{
	for (auto& i : m_inputAttributeTable) {
		if (i.usage == usage && i.index == usageIndex) {
			*outLocation = i.layoutLocation;
			return true;
		}
	}
	return false;
}

//int VulkanShaderPass::getUniformCount() const
//{
//    LN_NOTIMPLEMENTED();
//    return 0;
//}
//
//IShaderUniform* VulkanShaderPass::getUniform(int index) const
//{
//    // TODO: 必要？
//    LN_NOTIMPLEMENTED();
//    return nullptr;
//}
//
//void VulkanShaderPass::setUniformValue(int index, const void* data, size_t size)
//{
//    LN_NOTIMPLEMENTED();
//}

int VulkanShaderPass::getUniformBufferCount() const
{
    return 0;
}

IShaderUniformBuffer* VulkanShaderPass::getUniformBuffer(int index) const
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

IShaderSamplerBuffer* VulkanShaderPass::samplerBuffer() const
{
    return nullptr;
}

//=============================================================================
// VulkanShaderUniformBuffer

VulkanShaderUniformBuffer::VulkanShaderUniformBuffer()
{
}

VulkanShaderUniformBuffer::~VulkanShaderUniformBuffer()
{
}

Result VulkanShaderUniformBuffer::init(const ShaderUniformBufferInfo& info)
{
    m_name = info.name;

    for (auto& member : info.members)
    {
        auto uniform = makeRef<VulkanShaderUniform>();
        if (!uniform->init(member)) {
            return false;
        }
        m_uniforms.push_back(uniform);
    }

    return true;
}

void VulkanShaderUniformBuffer::dispose()
{
    IShaderUniformBuffer::dispose();
}

const std::string& VulkanShaderUniformBuffer::name() const
{
    LN_NOTIMPLEMENTED();
    return m_name;
}

int VulkanShaderUniformBuffer::getUniformCount() const
{
    LN_NOTIMPLEMENTED();
    return 0;
}

IShaderUniform* VulkanShaderUniformBuffer::getUniform(int index) const
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

size_t VulkanShaderUniformBuffer::bufferSize() const
{
    LN_NOTIMPLEMENTED();
    return 0;
}

void VulkanShaderUniformBuffer::setData(const void* data, size_t size)
{
    LN_NOTIMPLEMENTED();
}

//=============================================================================
// VulkanShaderUniform

VulkanShaderUniform::VulkanShaderUniform()
{
}

VulkanShaderUniform::~VulkanShaderUniform()
{
}

Result VulkanShaderUniform::init(const ShaderUniformInfo& info)
{
    LN_NOTIMPLEMENTED();
    return true;
}

void VulkanShaderUniform::dispose()
{
    IShaderUniform::dispose();
}

//=============================================================================
// VulkanLocalShaderSamplerBuffer

VulkanLocalShaderSamplerBuffer::VulkanLocalShaderSamplerBuffer()
{
}

VulkanLocalShaderSamplerBuffer::~VulkanLocalShaderSamplerBuffer()
{
}

void VulkanLocalShaderSamplerBuffer::init()
{
}

void VulkanLocalShaderSamplerBuffer::dispose()
{
}

int VulkanLocalShaderSamplerBuffer::registerCount() const
{
    LN_NOTIMPLEMENTED();
    return 0;
}

const std::string& VulkanLocalShaderSamplerBuffer::getTextureRegisterName(int registerIndex) const
{
    LN_NOTIMPLEMENTED();
    return std::string();
}

const std::string& VulkanLocalShaderSamplerBuffer::getSamplerRegisterName(int registerIndex) const
{
    LN_NOTIMPLEMENTED();
    return std::string();
}

void VulkanLocalShaderSamplerBuffer::setTexture(int registerIndex, ITexture* texture)
{
    LN_NOTIMPLEMENTED();
}

void VulkanLocalShaderSamplerBuffer::setSamplerState(int registerIndex, ISamplerState* state)
{
    LN_NOTIMPLEMENTED();
}

} // namespace detail
} // namespace ln
