
#include "Internal.hpp"
#include <LuminoPlatform/PlatformWindow.hpp>
#include <LuminoPlatform/PlatformSupport.hpp>
#include <LuminoShaderCompiler/ShaderHelper.hpp>
#include <LuminoGraphics/GraphicsExtension.hpp>
#include "../RHIProfiler.hpp"
#include "VulkanDeviceContext.hpp"
#include "VulkanBuffers.hpp"
#include "VulkanTextures.hpp"
#include "VulkanDescriptorPool.hpp"
#include "VulkanSingleFrameAllocator.hpp"
#include "VulkanCommandList.hpp"
#include "VulkanShaderPass.hpp"

namespace ln {
namespace detail {

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

//==============================================================================
// VulkanDevice

VulkanDevice::VulkanDevice()
    : m_instance(VK_NULL_HANDLE)
    , m_debugMessenger(VK_NULL_HANDLE)
    , m_activePhysicalDeviceInfoIndex(-1)
    //, m_graphicsContext(nullptr)
    , m_enableValidationLayers(false) {
}

bool VulkanDevice::init(const Settings& settings, bool* outIsDriverSupported) {
    if (LN_REQUIRE(outIsDriverSupported)) return false;
    *outIsDriverSupported = true;
    m_enableValidationLayers = settings.debugMode;

    LN_LOG_DEBUG("VulkanDevice initialization started.");

    if (!VulkanHelper::initVulkanFunctions()) {
        LN_LOG_WARNING("Valid vulkan library not found.");
        *outIsDriverSupported = false;
        return false;
    }
    if (!createInstance()) {
        *outIsDriverSupported = false;
        return false;
    }
    if (!setupDebugMessenger()) {
        return false;
    }
    if (!pickPhysicalDevice()) {
        return false;
    }
    if (!createLogicalDevice()) {
        return false;
    }
    if (!createCommandPool()) {
        return false;
    }

    //if (!m_renderPassCache.init(this)) {
    //	return false;
    //}
    //if (!m_pipelineCache.init(this)) {
    //	return false;
    //}

    const size_t PageSize = 0x200000; // 2MB
    //m_uniformBufferSingleFrameAllocator = makeRef<VulkanSingleFrameAllocatorPageManager>(this, PageSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    m_transferBufferSingleFrameAllocator = makeRef<VulkanSingleFrameAllocatorPageManager>(this, PageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    m_nativeInterface = std::make_unique<VulkanNativeGraphicsInterface>(this);

    //{
    //    vkGetPhysicalDeviceFormatProperties()
    //}

    LN_LOG_DEBUG("VulkanDevice initialization completed.");

    return true;
}

void VulkanDevice::dispose() {
    IGraphicsDevice::dispose();

    //m_uniformBufferSingleFrameAllocator = nullptr;
    m_transferBufferSingleFrameAllocator = nullptr;

    if (m_commandPool) {
        vkDestroyCommandPool(m_device, m_commandPool, vulkanAllocator());
        m_commandPool = VK_NULL_HANDLE;
    }

    if (m_device) {
        vkDestroyDevice(m_device, vulkanAllocator());
        m_device = VK_NULL_HANDLE;
    }

    if (m_debugMessenger) {
        DestroyDebugUtilsMessengerEXT(vulkanInstance(), m_debugMessenger, vulkanAllocator());
    }

    if (m_instance) {
        vkDestroyInstance(m_instance, vulkanAllocator());
        m_instance = VK_NULL_HANDLE;
    }
}

INativeGraphicsInterface* VulkanDevice::getNativeInterface() const {
    return m_nativeInterface.get();
}

void VulkanDevice::onGetCaps(GraphicsDeviceCaps* outCaps) {
    outCaps->graphicsAPI = GraphicsAPI::Vulkan;
    outCaps->requestedShaderTriple.target = "spv";
    outCaps->requestedShaderTriple.version = 110;
    outCaps->requestedShaderTriple.option = "";
    outCaps->uniformBufferOffsetAlignment = m_physicalDeviceInfos[m_activePhysicalDeviceInfoIndex].deviceProperty.limits.minUniformBufferOffsetAlignment;
}

Ref<ISwapChain> VulkanDevice::onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize) {
    auto ptr = makeRef<VulkanSwapChain>();
    if (!ptr->init(this, window, backbufferSize)) {
        return nullptr;
    }
    return ptr;
}

Ref<ICommandList> VulkanDevice::onCreateCommandList() {
    auto ptr = makeRef<VulkanGraphicsContext>();
    if (!ptr->init(this)) {
        return nullptr;
    }
    return ptr;
}

Ref<IRenderPass> VulkanDevice::onCreateRenderPass(const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil) {
    auto ptr = makeRef<VulkanRenderPass2>();
    if (!ptr->init(this, buffers, clearFlags, clearColor, clearDepth, clearStencil)) {
        return nullptr;
    }
    return ptr;
}

Ref<IPipeline> VulkanDevice::onCreatePipeline(const DevicePipelineStateDesc& state) {
    auto ptr = makeRef<VulkanPipeline2>();
    if (!ptr->init(this, state)) {
        return nullptr;
    }
    return ptr;
}

Ref<IVertexDeclaration> VulkanDevice::onCreateVertexDeclaration(const VertexElement* elements, int elementsCount) {
    auto ptr = makeRef<VulkanVertexDeclaration>();
    if (!ptr->init(elements, elementsCount)) {
        return nullptr;
    }
    return ptr;
}

Ref<RHIResource> VulkanDevice::onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) {
    auto ptr = makeRef<VulkanVertexBuffer>();
    if (!ptr->init(this, usage, bufferSize, initialData)) {
        return nullptr;
    }
    return ptr;
}

Ref<RHIResource> VulkanDevice::onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData) {
    auto ptr = makeRef<VulkanIndexBuffer>();
    if (!ptr->init(this, usage, format, indexCount, initialData)) {
        return nullptr;
    }
    return ptr;
}

Ref<RHIResource> VulkanDevice::onCreateTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData) {
    auto ptr = makeRef<VulkanTexture2D>();
    if (!ptr->init(this, usage, width, height, requestFormat, mipmap, initialData)) {
        return nullptr;
    }
    return ptr;
}

Ref<RHIResource> VulkanDevice::onCreateTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData) {
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<RHIResource> VulkanDevice::onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, bool msaa) {
    auto ptr = makeRef<VulkanRenderTarget>();
    if (!ptr->init(this, width, height, requestFormat, mipmap, msaa)) {
        return nullptr;
    }
    return ptr;
}

Ref<RHIResource> VulkanDevice::onCreateDepthBuffer(uint32_t width, uint32_t height) {
    auto ptr = makeRef<VulkanDepthBuffer>();
    if (!ptr->init(this, width, height)) {
        return nullptr;
    }
    return ptr;
}

Ref<ISamplerState> VulkanDevice::onCreateSamplerState(const SamplerStateData& desc) {
    auto ptr = makeRef<VulkanSamplerState>();
    if (!ptr->init(this, desc)) {
        return nullptr;
    }
    return ptr;
}

Ref<IShaderPass> VulkanDevice::onCreateShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag) {
    auto ptr = makeRef<VulkanShaderPass>();
    if (!ptr->init(this, createInfo, diag)) {
        return nullptr;
    }
    return ptr;
}

Ref<RHIResource> VulkanDevice::onCreateUniformBuffer(uint32_t size) {
    auto ptr = makeRef<VulkanUniformBuffer>();
    if (!ptr->init(this, size)) {
        return nullptr;
    }
    return ptr;
}

Ref<IDescriptorPool> VulkanDevice::onCreateDescriptorPool(IShaderPass* shaderPass) {
    auto ptr = makeRef<VulkanDescriptorPool2>();
    if (!ptr->init(this, static_cast<VulkanShaderPass*>(shaderPass))) {
        return nullptr;
    }
    return ptr;
}

// TODO: もし複数 swapchain へのレンダリングを1つの CommandBuffer でやる場合、flush 時には描画するすべての swapchain の image 準備を待たなければならない。
// CommandBuffer 単位で、setRenderTarget された SwapChain の RenderTarget をすべて覚えておく仕組みが必要だろう。
void VulkanDevice::onSubmitCommandBuffer(ICommandList* context, RHIResource* affectRendreTarget) {
    auto vulkanContext = static_cast<VulkanGraphicsContext*>(context);
    auto* t = static_cast<VulkanRenderTarget*>(affectRendreTarget);
    vulkanContext->recodingCommandBuffer()->submit(
        t->swapchainImageAvailableSemaphore(),
        t->renderFinishedSemaphore());

    // 一度でも submit したら、 null をセットして、次回の submit では何も待たないようにしておく。
    //      [GeForce GTX 1060] 既にシグナル状態のセマフォを vkQueueSubmit で待つように指定すると、vkQueueSubmit が VK_ERROR_DEVICE_LOST を返す。
    //      通常は vkAcquireNextImageKHR と vkQueueSubmit が交互に実行されるので問題ないが、
    //      RenderTarget の Read で一度 CommnadBuffer を vkQueueSubmit し、続けて CommnadBuffer の記録を再開 → vkQueueSubmit したときに問題になる。
    //      vkQueueSubmit で待ちたいのは vkAcquireNextImageKHR で準備開始された RenderTarget が本当に準備終わったかどうかなので、一度待てば十分。
    t->setSwapchainImageAvailableSemaphoreRef(nullptr);
}

ICommandQueue* VulkanDevice::getGraphicsCommandQueue() {
    LN_NOTIMPLEMENTED();
    return nullptr;
}

ICommandQueue* VulkanDevice::getComputeCommandQueue() {
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Result VulkanDevice::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, uint32_t* outType) {
    for (uint32_t i = 0; i < m_deviceMemoryProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (m_deviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            *outType = i;
            return ok();
        }
    }

    LN_LOG_ERROR("failed to find suitable memory type!");
    return err();
}

Result VulkanDevice::createInstance() {
    if (m_enableValidationLayers) {
        m_availableValidationLayers = VulkanHelper::checkValidationLayerSupport();
        if (m_availableValidationLayers.empty()) {
            LN_LOG_ERROR("validation layers requested, but not available!");
            return err();
        }
    }

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Lumino Application";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Lumino Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    // API バージョンは 1.1.0。負値の Viewpoer height を利用するため。
    // https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VkViewport
    appInfo.apiVersion = VK_MAKE_VERSION(1, 1, 0);

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    //auto extensions = getRequiredExtensions();
    std::vector<const char*> extensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef LN_OS_WIN32
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif LN_OS_LINUX
        VK_KHR_XCB_SURFACE_EXTENSION_NAME,
#elif LN_OS_ANDROID
        VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,
#endif
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
    };
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (m_enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(m_availableValidationLayers.size());
        createInfo.ppEnabledLayerNames = m_availableValidationLayers.data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    LN_VK_CHECK(vkCreateInstance(&createInfo, vulkanAllocator(), &m_instance));

    return ok();
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {

    if (StringHelper::indexOf(pCallbackData->pMessage, -1, "prior to any Draw Cmds.", -1) >= 0 ||
        StringHelper::indexOf(pCallbackData->pMessage, -1, "Vertex shader consumes input at location", -1) >= 0) {
    }
    else {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    }

    return VK_FALSE;
}

Result VulkanDevice::setupDebugMessenger() {
    if (m_enableValidationLayers) {
        VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;

        LN_VK_CHECK(CreateDebugUtilsMessengerEXT(vulkanInstance(), &createInfo, vulkanAllocator(), &m_debugMessenger));
    }

    return ok();
}

Result VulkanDevice::pickPhysicalDevice() {
    // Get physical devices
    {
        uint32_t count = 0;
        auto ret = vkEnumeratePhysicalDevices(m_instance, &count, nullptr);
        if (ret != VK_SUCCESS || count < 1) {
            LN_LOG_ERROR("Failed vkEnumeratePhysicalDevices");
            return err();
        }

        m_physicalDeviceInfos.resize(count);

        std::vector<VkPhysicalDevice> gpuDevices(count);
        if (vkEnumeratePhysicalDevices(m_instance, &count, gpuDevices.data()) != VK_SUCCESS) {
            LN_LOG_ERROR("Failed vkEnumeratePhysicalDevices");
            return err();
        }

        for (auto i = 0u; i < count; ++i) {
            m_physicalDeviceInfos[i].device = gpuDevices[i];
            vkGetPhysicalDeviceMemoryProperties(gpuDevices[i], &m_physicalDeviceInfos[i].memoryProperty);
            vkGetPhysicalDeviceProperties(gpuDevices[i], &m_physicalDeviceInfos[i].deviceProperty);

#if 0 // test
			if (m_physicalDeviceInfos[i].deviceProperty.apiVersion >= VK_MAKE_VERSION(1, 1, 0))
			{
				/*
					 *	vkGetPhysicalDeviceProperties2() を使う前には、
					 *	事前にプロパティ構造体のチェーンを準備しておく必要がある
					 */
				VkPhysicalDeviceProperties2 dev_props2 = {};

				//struct StructChainInfo {
				//	VkStructureType sType;
				//	uint32_t        mem_size;
				//};

				//struct StructChainInfo chain_info[] = {
				//	{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BLEND_OPERATION_ADVANCED_PROPERTIES_EXT,
				//	   sizeof(VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT) },
				//	{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_POINT_CLIPPING_PROPERTIES_KHR,
				//	   sizeof(VkPhysicalDevicePointClippingPropertiesKHR) },
				//	{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PUSH_DESCRIPTOR_PROPERTIES_KHR,
				//	   sizeof(VkPhysicalDevicePushDescriptorPropertiesKHR) },
				//	{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DISCARD_RECTANGLE_PROPERTIES_EXT,
				//	   sizeof(VkPhysicalDeviceDiscardRectanglePropertiesEXT) },
				//	{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES_KHR,
				//	   sizeof(VkPhysicalDeviceMultiviewPropertiesKHR) } };

				//uint32_t chain_info_len = LN_ARRAY_SIZE_OF(chain_info);

				dev_props2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR;
				dev_props2.pNext = nullptr;
				//BuildStructChain((struct StructHeader *)&dev_props2, chain_info, chain_info_len);

				VkPhysicalDeviceMaintenance3Properties m3props;
				m3props.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES;
				m3props.pNext = nullptr;
				dev_props2.pNext = &m3props;

				vkGetPhysicalDeviceProperties2(gpuDevices[i],			/* [in ] Vulkan物理デバイスのハンドル */
					&dev_props2);	/* [out] Vulkan物理デバイス情報が格納される */

				//VkPhysicalDeviceBlendOperationAdvancedPropertiesEXT *pBlendOp = dev_props2.pNext;
			}
#endif
        }
    }

    LN_LOG_INFO("Detected device:");
    for (const auto& i : m_physicalDeviceInfos) {
        LN_LOG_INFO("  deviceName: {}", i.deviceProperty.deviceName);
    }

    // Select device
    const auto itr = std::max_element(m_physicalDeviceInfos.begin(), m_physicalDeviceInfos.end(), [](const PhysicalDeviceInfo& a, const PhysicalDeviceInfo& b) {
        return a.deviceProperty.limits.maxImageDimension2D < b.deviceProperty.limits.maxImageDimension2D;
    });
    int index = itr - m_physicalDeviceInfos.begin();
    m_activePhysicalDeviceInfoIndex = index;

    const PhysicalDeviceInfo& info = m_physicalDeviceInfos[index];
    m_physicalDevice = info.device;

    // x86, GeForce RTX 2080 SUPER において、SwapChain 作成後に vkGetPhysicalDevice** が軒並みハングする問題の対策。
    // 2020/12/22 時点の問題で、今後修正が入るかもしれない。
    {
        m_imageFormatProperties = {
            { VK_FORMAT_D32_SFLOAT_S8_UINT, VkFormatProperties{} },
            { VK_FORMAT_D24_UNORM_S8_UINT, VkFormatProperties{} },
            { VK_FORMAT_D16_UNORM_S8_UINT, VkFormatProperties{} },
        };
        for (ImageFormatProperty& i : m_imageFormatProperties) {
            vkGetPhysicalDeviceFormatProperties(m_physicalDevice, i.format, &i.props);
        }

        vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_deviceMemoryProperties);
    }

    {
        VkSampleCountFlags counts = info.deviceProperty.limits.framebufferColorSampleCounts & info.deviceProperty.limits.framebufferDepthSampleCounts;
        if (counts & VK_SAMPLE_COUNT_64_BIT) {
            m_msaaSamples = VK_SAMPLE_COUNT_64_BIT;
        }
        else if (counts & VK_SAMPLE_COUNT_32_BIT) {
            m_msaaSamples = VK_SAMPLE_COUNT_32_BIT;
        }
        else if (counts & VK_SAMPLE_COUNT_16_BIT) {
            m_msaaSamples = VK_SAMPLE_COUNT_16_BIT;
        }
        else if (counts & VK_SAMPLE_COUNT_8_BIT) {
            m_msaaSamples = VK_SAMPLE_COUNT_8_BIT;
        }
        else if (counts & VK_SAMPLE_COUNT_4_BIT) {
            m_msaaSamples = VK_SAMPLE_COUNT_4_BIT;
        }
        else if (counts & VK_SAMPLE_COUNT_2_BIT) {
            m_msaaSamples = VK_SAMPLE_COUNT_2_BIT;
        }
        else {
            m_msaaSamples = VK_SAMPLE_COUNT_1_BIT;
        }
    }

    // Dump Caps
    {
        LN_LOG_INFO("DeviceProperties:");
        LN_LOG_INFO("  VK_VERSION_MAJOR: {}", VK_VERSION_MAJOR(info.deviceProperty.apiVersion));
        LN_LOG_INFO("  VK_VERSION_MAJOR: {}", VK_VERSION_MINOR(info.deviceProperty.apiVersion));
        LN_LOG_INFO("  VK_VERSION_MAJOR: {}", VK_VERSION_PATCH(info.deviceProperty.apiVersion));
        LN_LOG_INFO("  deviceName: {}", info.deviceProperty.deviceName);
        LN_LOG_INFO("  limits.maxImageDimension1D: {}", info.deviceProperty.limits.maxImageDimension1D);
        LN_LOG_INFO("  limits.maxImageDimension2D: {}", info.deviceProperty.limits.maxImageDimension2D);
        LN_LOG_INFO("  limits.maxImageDimension3D: {}", info.deviceProperty.limits.maxImageDimension3D);
        LN_LOG_INFO("  limits.maxImageDimensionCube: {}", info.deviceProperty.limits.maxImageDimensionCube);
        LN_LOG_INFO("  limits.maxImageArrayLayers: {}", info.deviceProperty.limits.maxImageArrayLayers);
        LN_LOG_INFO("  limits.maxTexelBufferElements: {}", info.deviceProperty.limits.maxTexelBufferElements);
        LN_LOG_INFO("  limits.maxUniformBufferRange: {}", info.deviceProperty.limits.maxUniformBufferRange);
        LN_LOG_INFO("  limits.maxStorageBufferRange: {}", info.deviceProperty.limits.maxStorageBufferRange);
        LN_LOG_INFO("  limits.maxPushConstantsSize: {}", info.deviceProperty.limits.maxPushConstantsSize);
        LN_LOG_INFO("  limits.maxMemoryAllocationCount: {}", info.deviceProperty.limits.maxMemoryAllocationCount);
        LN_LOG_INFO("  limits.maxSamplerAllocationCount: {}", info.deviceProperty.limits.maxSamplerAllocationCount);
        LN_LOG_INFO("  limits.bufferImageGranularity: {}", info.deviceProperty.limits.bufferImageGranularity);
        LN_LOG_INFO("  limits.sparseAddressSpaceSize: {}", info.deviceProperty.limits.sparseAddressSpaceSize);
        LN_LOG_INFO("  limits.maxBoundDescriptorSets: {}", info.deviceProperty.limits.maxBoundDescriptorSets);
        LN_LOG_INFO("  limits.maxPerStageDescriptorSamplers: {}", info.deviceProperty.limits.maxPerStageDescriptorSamplers);
        LN_LOG_INFO("  limits.maxPerStageDescriptorUniformBuffers: {}", info.deviceProperty.limits.maxPerStageDescriptorUniformBuffers);
        LN_LOG_INFO("  limits.maxPerStageDescriptorStorageBuffers: {}", info.deviceProperty.limits.maxPerStageDescriptorStorageBuffers);
        LN_LOG_INFO("  limits.maxPerStageDescriptorSampledImages: {}", info.deviceProperty.limits.maxPerStageDescriptorSampledImages);
        LN_LOG_INFO("  limits.maxPerStageDescriptorStorageImages: {}", info.deviceProperty.limits.maxPerStageDescriptorStorageImages);
        LN_LOG_INFO("  limits.maxPerStageDescriptorInputAttachments: {}", info.deviceProperty.limits.maxPerStageDescriptorInputAttachments);
        LN_LOG_INFO("  limits.maxPerStageResources: {}", info.deviceProperty.limits.maxPerStageResources);
        LN_LOG_INFO("  limits.maxDescriptorSetSamplers: {}", info.deviceProperty.limits.maxDescriptorSetSamplers);
        LN_LOG_INFO("  limits.maxDescriptorSetUniformBuffers: {}", info.deviceProperty.limits.maxDescriptorSetUniformBuffers);
        LN_LOG_INFO("  limits.maxDescriptorSetUniformBuffersDynamic: {}", info.deviceProperty.limits.maxDescriptorSetUniformBuffersDynamic);
        LN_LOG_INFO("  limits.maxDescriptorSetStorageBuffers: {}", info.deviceProperty.limits.maxDescriptorSetStorageBuffers);
        LN_LOG_INFO("  limits.maxDescriptorSetStorageBuffersDynamic: {}", info.deviceProperty.limits.maxDescriptorSetStorageBuffersDynamic);
        LN_LOG_INFO("  limits.maxDescriptorSetSampledImages: {}", info.deviceProperty.limits.maxDescriptorSetSampledImages);
        LN_LOG_INFO("  limits.maxDescriptorSetStorageImages: {}", info.deviceProperty.limits.maxDescriptorSetStorageImages);
        LN_LOG_INFO("  limits.maxDescriptorSetInputAttachments: {}", info.deviceProperty.limits.maxDescriptorSetInputAttachments);
        LN_LOG_INFO("  limits.maxVertexInputAttributes: {}", info.deviceProperty.limits.maxVertexInputAttributes);
        LN_LOG_INFO("  limits.maxVertexInputBindings: {}", info.deviceProperty.limits.maxVertexInputBindings);
        LN_LOG_INFO("  limits.maxVertexInputAttributeOffset: {}", info.deviceProperty.limits.maxVertexInputAttributeOffset);
        LN_LOG_INFO("  limits.maxVertexInputBindingStride: {}", info.deviceProperty.limits.maxVertexInputBindingStride);
        LN_LOG_INFO("  limits.maxVertexOutputComponents: {}", info.deviceProperty.limits.maxVertexOutputComponents);
        LN_LOG_INFO("  limits.maxTessellationGenerationLevel: {}", info.deviceProperty.limits.maxTessellationGenerationLevel);
        LN_LOG_INFO("  limits.maxTessellationPatchSize: {}", info.deviceProperty.limits.maxTessellationPatchSize);
        LN_LOG_INFO("  limits.maxTessellationControlPerVertexInputComponents: {}", info.deviceProperty.limits.maxTessellationControlPerVertexInputComponents);
        LN_LOG_INFO("  limits.maxTessellationControlPerVertexOutputComponents: {}", info.deviceProperty.limits.maxTessellationControlPerVertexOutputComponents);
        LN_LOG_INFO("  limits.maxTessellationControlPerPatchOutputComponents: {}", info.deviceProperty.limits.maxTessellationControlPerPatchOutputComponents);
        LN_LOG_INFO("  limits.maxTessellationControlTotalOutputComponents: {}", info.deviceProperty.limits.maxTessellationControlTotalOutputComponents);
        LN_LOG_INFO("  limits.maxTessellationEvaluationInputComponents: {}", info.deviceProperty.limits.maxTessellationEvaluationInputComponents);
        LN_LOG_INFO("  limits.maxTessellationEvaluationOutputComponents: {}", info.deviceProperty.limits.maxTessellationEvaluationOutputComponents);
        LN_LOG_INFO("  limits.maxGeometryShaderInvocations: {}", info.deviceProperty.limits.maxGeometryShaderInvocations);
        LN_LOG_INFO("  limits.maxGeometryInputComponents: {}", info.deviceProperty.limits.maxGeometryInputComponents);
        LN_LOG_INFO("  limits.maxGeometryOutputComponents: {}", info.deviceProperty.limits.maxGeometryOutputComponents);
        LN_LOG_INFO("  limits.maxGeometryOutputVertices: {}", info.deviceProperty.limits.maxGeometryOutputVertices);
        LN_LOG_INFO("  limits.maxGeometryTotalOutputComponents: {}", info.deviceProperty.limits.maxGeometryTotalOutputComponents);
        LN_LOG_INFO("  limits.maxFragmentInputComponents: {}", info.deviceProperty.limits.maxFragmentInputComponents);
        LN_LOG_INFO("  limits.maxFragmentOutputAttachments: {}", info.deviceProperty.limits.maxFragmentOutputAttachments);
        LN_LOG_INFO("  limits.maxFragmentDualSrcAttachments: {}", info.deviceProperty.limits.maxFragmentDualSrcAttachments);
        LN_LOG_INFO("  limits.maxFragmentCombinedOutputResources: {}", info.deviceProperty.limits.maxFragmentCombinedOutputResources);
        LN_LOG_INFO("  limits.maxComputeSharedMemorySize: {}", info.deviceProperty.limits.maxComputeSharedMemorySize);
        LN_LOG_INFO("  limits.maxComputeWorkGroupCount[0]: {}", info.deviceProperty.limits.maxComputeWorkGroupCount[0]);
        LN_LOG_INFO("  limits.maxComputeWorkGroupCount[1]: {}", info.deviceProperty.limits.maxComputeWorkGroupCount[1]);
        LN_LOG_INFO("  limits.maxComputeWorkGroupCount[2]: {}", info.deviceProperty.limits.maxComputeWorkGroupCount[2]);
        LN_LOG_INFO("  limits.maxComputeWorkGroupInvocations: {}", info.deviceProperty.limits.maxComputeWorkGroupInvocations);
        LN_LOG_INFO("  limits.maxComputeWorkGroupSize[0]: {}", info.deviceProperty.limits.maxComputeWorkGroupSize[0]);
        LN_LOG_INFO("  limits.maxComputeWorkGroupSize[1]: {}", info.deviceProperty.limits.maxComputeWorkGroupSize[1]);
        LN_LOG_INFO("  limits.maxComputeWorkGroupSize[2]: {}", info.deviceProperty.limits.maxComputeWorkGroupSize[2]);
        LN_LOG_INFO("  limits.subPixelPrecisionBits: {}", info.deviceProperty.limits.subPixelPrecisionBits);
        LN_LOG_INFO("  limits.subTexelPrecisionBits: {}", info.deviceProperty.limits.subTexelPrecisionBits);
        LN_LOG_INFO("  limits.mipmapPrecisionBits: {}", info.deviceProperty.limits.mipmapPrecisionBits);
        LN_LOG_INFO("  limits.maxDrawIndexedIndexValue: {}", info.deviceProperty.limits.maxDrawIndexedIndexValue);
        LN_LOG_INFO("  limits.maxDrawIndirectCount: {}", info.deviceProperty.limits.maxDrawIndirectCount);
        LN_LOG_INFO("  limits.maxSamplerLodBias: {}", info.deviceProperty.limits.maxSamplerLodBias);
        LN_LOG_INFO("  limits.maxSamplerAnisotropy: {}", info.deviceProperty.limits.maxSamplerAnisotropy);
        LN_LOG_INFO("  limits.maxViewports: {}", info.deviceProperty.limits.maxViewports);
        LN_LOG_INFO("  limits.maxViewportDimensions[0]: {}", info.deviceProperty.limits.maxViewportDimensions[0]);
        LN_LOG_INFO("  limits.maxViewportDimensions[1]: {}", info.deviceProperty.limits.maxViewportDimensions[1]);
        LN_LOG_INFO("  limits.viewportBoundsRange[0]: {}", info.deviceProperty.limits.viewportBoundsRange[0]);
        LN_LOG_INFO("  limits.viewportBoundsRange[1]: {}", info.deviceProperty.limits.viewportBoundsRange[1]);
        LN_LOG_INFO("  limits.viewportSubPixelBits: {}", info.deviceProperty.limits.viewportSubPixelBits);
        LN_LOG_INFO("  limits.minMemoryMapAlignment: {}", info.deviceProperty.limits.minMemoryMapAlignment);
        LN_LOG_INFO("  limits.minTexelBufferOffsetAlignment: {}", info.deviceProperty.limits.minTexelBufferOffsetAlignment);
        LN_LOG_INFO("  limits.minUniformBufferOffsetAlignment: {}", info.deviceProperty.limits.minUniformBufferOffsetAlignment);
        LN_LOG_INFO("  limits.minStorageBufferOffsetAlignment: {}", info.deviceProperty.limits.minStorageBufferOffsetAlignment);
        LN_LOG_INFO("  limits.minTexelOffset: {}", info.deviceProperty.limits.minTexelOffset);
        LN_LOG_INFO("  limits.minTexelOffset: {}", info.deviceProperty.limits.minTexelOffset);
        LN_LOG_INFO("  limits.maxTexelOffset: {}", info.deviceProperty.limits.maxTexelOffset);
        LN_LOG_INFO("  limits.minTexelGatherOffset: {}", info.deviceProperty.limits.minTexelGatherOffset);
        LN_LOG_INFO("  limits.maxTexelGatherOffset: {}", info.deviceProperty.limits.maxTexelGatherOffset);
        LN_LOG_INFO("  limits.minInterpolationOffset: {}", info.deviceProperty.limits.minInterpolationOffset);
        LN_LOG_INFO("  limits.maxInterpolationOffset: {}", info.deviceProperty.limits.maxInterpolationOffset);
        LN_LOG_INFO("  limits.subPixelInterpolationOffsetBits: {}", info.deviceProperty.limits.subPixelInterpolationOffsetBits);
        LN_LOG_INFO("  limits.maxFramebufferWidth: {}", info.deviceProperty.limits.maxFramebufferWidth);
        LN_LOG_INFO("  limits.maxFramebufferHeight: {}", info.deviceProperty.limits.maxFramebufferHeight);
        LN_LOG_INFO("  limits.maxFramebufferLayers: {}", info.deviceProperty.limits.maxFramebufferLayers);
        LN_LOG_INFO("  limits.framebufferColorSampleCounts: {}", info.deviceProperty.limits.framebufferColorSampleCounts);
        LN_LOG_INFO("  limits.framebufferDepthSampleCounts: {}", info.deviceProperty.limits.framebufferDepthSampleCounts);
        LN_LOG_INFO("  limits.framebufferStencilSampleCounts: {}", info.deviceProperty.limits.framebufferStencilSampleCounts);
        LN_LOG_INFO("  limits.framebufferNoAttachmentsSampleCounts: {}", info.deviceProperty.limits.framebufferNoAttachmentsSampleCounts);
        LN_LOG_INFO("  limits.maxColorAttachments: {}", info.deviceProperty.limits.maxColorAttachments);
        LN_LOG_INFO("  limits.sampledImageColorSampleCounts: {}", info.deviceProperty.limits.sampledImageColorSampleCounts);
        LN_LOG_INFO("  limits.sampledImageIntegerSampleCounts: {}", info.deviceProperty.limits.sampledImageIntegerSampleCounts);
        LN_LOG_INFO("  limits.sampledImageDepthSampleCounts: {}", info.deviceProperty.limits.sampledImageDepthSampleCounts);
        LN_LOG_INFO("  limits.sampledImageStencilSampleCounts: {}", info.deviceProperty.limits.sampledImageStencilSampleCounts);
        LN_LOG_INFO("  limits.storageImageSampleCounts: {}", info.deviceProperty.limits.storageImageSampleCounts);
        LN_LOG_INFO("  limits.maxSampleMaskWords: {}", info.deviceProperty.limits.maxSampleMaskWords);
        LN_LOG_INFO("  limits.timestampComputeAndGraphics: {}", info.deviceProperty.limits.timestampComputeAndGraphics);
        LN_LOG_INFO("  limits.timestampPeriod: {}", info.deviceProperty.limits.timestampPeriod);
        LN_LOG_INFO("  limits.maxClipDistances: {}", info.deviceProperty.limits.maxClipDistances);
        LN_LOG_INFO("  limits.maxCullDistances: {}", info.deviceProperty.limits.maxCullDistances);
        LN_LOG_INFO("  limits.maxCombinedClipAndCullDistances: {}", info.deviceProperty.limits.maxCombinedClipAndCullDistances);
        LN_LOG_INFO("  limits.discreteQueuePriorities: {}", info.deviceProperty.limits.discreteQueuePriorities);
        LN_LOG_INFO("  limits.pointSizeRange[0]: {}", info.deviceProperty.limits.pointSizeRange[0]);
        LN_LOG_INFO("  limits.pointSizeRange[1]: {}", info.deviceProperty.limits.pointSizeRange[1]);
        LN_LOG_INFO("  limits.lineWidthRange[0]: {}", info.deviceProperty.limits.lineWidthRange[0]);
        LN_LOG_INFO("  limits.lineWidthRange[1]: {}", info.deviceProperty.limits.lineWidthRange[1]);
        LN_LOG_INFO("  limits.pointSizeGranularity: {}", info.deviceProperty.limits.pointSizeGranularity);
        LN_LOG_INFO("  limits.lineWidthGranularity: {}", info.deviceProperty.limits.lineWidthGranularity);
        LN_LOG_INFO("  limits.strictLines: {}", info.deviceProperty.limits.strictLines);
        LN_LOG_INFO("  limits.standardSampleLocations: {}", info.deviceProperty.limits.standardSampleLocations);
        LN_LOG_INFO("  limits.optimalBufferCopyOffsetAlignment: {}", info.deviceProperty.limits.optimalBufferCopyOffsetAlignment);
        LN_LOG_INFO("  limits.optimalBufferCopyRowPitchAlignment: {}", info.deviceProperty.limits.optimalBufferCopyRowPitchAlignment);
        LN_LOG_INFO("  limits.nonCoherentAtomSize: {}", info.deviceProperty.limits.nonCoherentAtomSize);
    }

    return ok();
}

Result VulkanDevice::createLogicalDevice() {
    //QueueFamilyIndices indices = findQueueFamilies(vulkanPhysicalDevice());

    auto graphicsFamilyIndex = UINT32_MAX;
    auto computeFamilyIndex = UINT32_MAX;
    auto transferFamilyIndex = UINT32_MAX;
    auto graphicsQueueIndex = UINT32_MAX;
    auto computeQueueIndex = UINT32_MAX;
    auto transferQueueindex = UINT32_MAX;
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::vector<float> queuePriorities;
    {
        uint32_t propCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &propCount, nullptr);
        m_queueFamilyProps.resize(propCount);
        vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &propCount, m_queueFamilyProps.data());
        queueCreateInfos.resize(propCount);

        int queueIndex = 0;
        int totalQueueCount = 0;
        float queuePriority = 1.0f;
        for (int i = 0; i < propCount; ++i) {
            queueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfos[i].pNext = nullptr;
            queueCreateInfos[i].flags = 0;
            queueCreateInfos[i].queueCount = m_queueFamilyProps[i].queueCount;
            queueCreateInfos[i].queueFamilyIndex = i;

            totalQueueCount += m_queueFamilyProps[i].queueCount;

            // Graphics queue
            //if (m_queueFamilyProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            if ((m_queueFamilyProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && (m_queueFamilyProps[i].queueFlags & VK_QUEUE_COMPUTE_BIT)) {
                if (graphicsFamilyIndex == UINT32_MAX) {
                    graphicsFamilyIndex = i;
                    graphicsQueueIndex = queueIndex;
                    queueIndex++;
                }
            }

            // Compute queue
            if ((m_queueFamilyProps[i].queueFlags & VK_QUEUE_COMPUTE_BIT) && ((m_queueFamilyProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != VK_QUEUE_GRAPHICS_BIT)) {
                if (computeFamilyIndex == UINT32_MAX) {
                    computeFamilyIndex = i;
                    computeQueueIndex = queueIndex;
                    queueIndex++;
                }
            }

            // Transfer queue
            if ((m_queueFamilyProps[i].queueFlags & VK_QUEUE_TRANSFER_BIT) && ((m_queueFamilyProps[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != VK_QUEUE_GRAPHICS_BIT)) {
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
                if (m_queueFamilyProps[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
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
                if (m_queueFamilyProps[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
                    if (transferFamilyIndex == UINT32_MAX) {
                        transferFamilyIndex = i;
                        transferQueueindex = queueIndex;
                        queueIndex++;
                    }
                }
            }
        }

        uint32_t offset = 0u;
        queuePriorities.resize(totalQueueCount);
        for (uint32_t i = 0u; i < propCount; ++i) {
            queueCreateInfos[i].pQueuePriorities = &queuePriorities[offset];
            offset += queueCreateInfos[i].queueCount;
        }
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.samplerAnisotropy = VK_TRUE;
    deviceFeatures.fillModeNonSolid = VK_TRUE; // ワイヤーフレームの描画をできるようにする (VK_POLYGON_MODE_LINE を使えるようにする)

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (m_enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(m_availableValidationLayers.size());
        createInfo.ppEnabledLayerNames = m_availableValidationLayers.data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(vulkanPhysicalDevice(), &createInfo, nullptr, &m_device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    m_graphicsQueueFamilyIndex = graphicsFamilyIndex;
    vkGetDeviceQueue(m_device, graphicsFamilyIndex, graphicsQueueIndex, &m_graphicsQueue);

    return ok();
}

Result VulkanDevice::createCommandPool() {
    //QueueFamilyIndices queueFamilyIndices = findQueueFamilies(vulkanPhysicalDevice());

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = m_graphicsQueueFamilyIndex;

    // 1つのコマンドバッファを繰り返し使えるようにマークする。
    // 内部的には、
    // - フラグONの場合 : CommandBuffer ごとにひとつの Allocator を持つ。→ 少し無駄にメモリを消費するが、CommandBuffer ごとにリセットできる。
    // - フラグOFFの場合 : Pool にひとつの Allocator を持つ。→ CommandBuffer ごとにメモリをリセットしたりできない。1度構築したら変化しない static な CommandBuffer を使いたいときに使用する。
    // https://www.reddit.com/r/vulkan/comments/5zwfot/whats_your_command_buffer_allocation_strategy/
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    LN_VK_CHECK(vkCreateCommandPool(m_device, &poolInfo, vulkanAllocator(), &m_commandPool));

    return ok();
}

//QueueFamilyIndices VulkanDevice::findQueueFamilies(VkPhysicalDevice device)
//{
//	QueueFamilyIndices indices;
//
//	uint32_t queueFamilyCount = 0;
//	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
//
//	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
//	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
//
//	int i = 0;
//	for (const auto& queueFamily : queueFamilies) {
//		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
//			indices.graphicsFamily = i;
//		}
//
//		if (indices.isComplete()) {
//			break;
//		}
//
//		i++;
//	}
//
//	return indices;
//}

bool VulkanDevice::findPresentQueueFamily(VkSurfaceKHR surface, uint32_t* outIndex) {
    for (uint32_t i = 0; i < m_queueFamilyProps.size(); i++) {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, surface, &presentSupport);
        if (m_queueFamilyProps[i].queueCount > 0 && presentSupport) {
            *outIndex = i;
            return true;
        }
    }
    return false;
}

VkFormat VulkanDevice::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
    for (VkFormat format : candidates) {
        const auto itr = std::find_if(
            m_imageFormatProperties.begin(), m_imageFormatProperties.end(), [format](const ImageFormatProperty& x) { return x.format == format; });
        if (itr != m_imageFormatProperties.end()) {
            const VkFormatProperties& props = itr->props;

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                return format;
            }
            else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                return format;
            }
        }
    }

    LN_UNREACHABLE();
    return VK_FORMAT_UNDEFINED;
}

VkFormat VulkanDevice::findDepthFormat() {
    // Stencil 要素を含むフォーマットでないと、そもそもステンシルテスト自体が行われない (ので、Never を指定しても描画される)
    // VK_FORMAT_D24_UNORM_S8_UINT が一般的そうだけど、Radeon Vega 8 Mobile では使えなかった。
    return findSupportedFormat(
        { /*VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, */ VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D16_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

VkCommandBuffer VulkanDevice::beginSingleTimeCommands() {
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = vulkanCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(m_device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    // https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#commandbuffers-lifecycle

    return commandBuffer;
}

Result VulkanDevice::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    LN_VK_CHECK(vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE));
    LN_VK_CHECK(vkQueueWaitIdle(m_graphicsQueue));

    vkFreeCommandBuffers(m_device, vulkanCommandPool(), 1, &commandBuffer);

    return ok();
}

void VulkanDevice::copyBufferImmediately(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferCopy copyRegion = {};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(commandBuffer);
}

void VulkanDevice::copyBufferToImageImmediately(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

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
        width,
        height,
        1
    };

    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    endSingleTimeCommands(commandBuffer);
}

Result VulkanDevice::transitionImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkFormat format, uint32_t mipLevel, VkImageLayout oldLayout, VkImageLayout newLayout) {
    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;

    if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

        if (VulkanHelper::hasStencilComponent(format)) {
            barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
    }
    else {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = mipLevel;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else {
        LN_LOG_ERROR("unsupported layout transition!");
        return err();
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage,
        destinationStage,
        0,
        0,
        nullptr,
        0,
        nullptr,
        1,
        &barrier);

    return ok();
}

Result VulkanDevice::transitionImageLayoutImmediately(VkImage image, VkFormat format, uint32_t mipLevel, VkImageLayout oldLayout, VkImageLayout newLayout) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();
    Result result = transitionImageLayout(commandBuffer, image, format, mipLevel, oldLayout, newLayout);
    endSingleTimeCommands(commandBuffer);
    return result;
}

//==============================================================================
// VulkanSwapChain

VulkanSwapChain::VulkanSwapChain()
    : m_swapchain(VK_NULL_HANDLE) {
}

Result VulkanSwapChain::init(VulkanDevice* deviceContext, PlatformWindow* window, const SizeI& backbufferSize) {
    LN_DCHECK(deviceContext);
    m_deviceContext = deviceContext;
    VkDevice device = m_deviceContext->vulkanDevice();

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

        LN_VK_CHECK(vkCreateWin32SurfaceKHR(m_deviceContext->vulkanInstance(), &info, m_deviceContext->vulkanAllocator(), &m_surface));
    }
#else
#error Invalid target.
#endif

    if (!createNativeSwapchain(backbufferSize)) {
        return err();
    }

    m_imageAvailableSemaphores.resize(maxFrameCount());

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    for (size_t i = 0; i < maxFrameCount(); i++) {
        LN_VK_CHECK(vkCreateSemaphore(device, &semaphoreInfo, m_deviceContext->vulkanAllocator(), &m_imageAvailableSemaphores[i]));
    }
    m_currentFrame = 0;

    return ok();
}

void VulkanSwapChain::dispose() {
    VkDevice device = m_deviceContext->vulkanDevice();

    for (auto& x : m_imageAvailableSemaphores) {
        vkDestroySemaphore(device, x, m_deviceContext->vulkanAllocator());
    }
    m_imageAvailableSemaphores.clear();

    cleanupNativeSwapchain();

    if (m_surface) {
        vkDestroySurfaceKHR(m_deviceContext->vulkanInstance(), m_surface, m_deviceContext->vulkanAllocator());
        m_surface = VK_NULL_HANDLE;
    }

    ISwapChain::dispose();
}

uint32_t VulkanSwapChain::getBackbufferCount() {
    return m_swapChainImageViews.size();
}

bool VulkanSwapChain::createNativeSwapchain(const SizeI& backbufferSize) {
    if (LN_REQUIRE(!m_swapchain)) return false;

    VkDevice device = m_deviceContext->vulkanDevice();
    // この Swapchain と対応する Surface と互換性がある QueueFamily を選択する
    uint32_t presentQueueFamily = 0;
    if (m_deviceContext->findPresentQueueFamily(m_surface, &presentQueueFamily)) {
        vkGetDeviceQueue(device, presentQueueFamily, 0, &m_presentQueue);
        // TODO: ↑ここのqueueIndex は 0 ではなく、使ってないインデックスにした方が効率良いかも。今だとほとんどの場合、graphicsQueue と同じになる。
    }
    else {
        LN_ERROR("PresentQueueFamily not found.");
        return false;
    }

    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_deviceContext->vulkanPhysicalDevice(), m_surface);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, backbufferSize.width, backbufferSize.height);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.pNext = nullptr;
    createInfo.surface = m_surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT; // readData できるようにするため、VK_IMAGE_USAGE_TRANSFER_SRC_BIT も指定しておく

    ///QueueFamilyIndices indices = m_deviceContext->findQueueFamilies(m_deviceContext->vulkanPhysicalDevice());
    uint32_t queueFamilyIndices[] = { m_deviceContext->graphicsQueueFamilyIndex(), presentQueueFamily };

    if (m_deviceContext->graphicsQueueFamilyIndex() != presentQueueFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupport.preTransform;
    createInfo.compositeAlpha = swapChainSupport.compositeAlpha;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(device, &createInfo, m_deviceContext->vulkanAllocator(), &m_swapchain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    std::vector<VkImage> swapChainImages;
    {
        vkGetSwapchainImagesKHR(device, m_swapchain, &imageCount, nullptr);
        swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(device, m_swapchain, &imageCount, swapChainImages.data());

        m_swapchainImageFormat = surfaceFormat.format;
        m_swapchainExtent = extent;
    }

    // 初期状態ではバックバッファのレイアウトは VK_IMAGE_LAYOUT_UNDEFINED となっている。
    // Vulkan-Tutorial では、初回の VkAttachmentDescription::initialLayout
    for (uint32_t i = 0; i < swapChainImages.size(); i++) {
        if (!m_deviceContext->transitionImageLayoutImmediately(swapChainImages[i], m_swapchainImageFormat, 1, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)) {
            return false;
        }
    }

    {
        m_swapChainImageViews.resize(swapChainImages.size());

        for (uint32_t i = 0; i < swapChainImages.size(); i++) {
            if (!VulkanHelper::createImageView(m_deviceContext, swapChainImages[i], m_swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1, &m_swapChainImageViews[i])) {
                return false;
            }
        }
    }

    m_swapchainRenderTargets.resize(swapChainImages.size());
    for (uint32_t i = 0; i < swapChainImages.size(); i++) {
        auto target = makeRef<VulkanRenderTarget>();
        target->initFromSwapchainImage(m_deviceContext, m_swapchainExtent.width, m_swapchainExtent.height, m_swapchainImageFormat, swapChainImages[i], m_swapChainImageViews[i]);
        target->m_device = m_deviceContext;
        target->m_objectId = m_deviceContext->m_objectNextId++;
        m_deviceContext->profiler()->addRenderTarget(target);
        m_swapchainRenderTargets[i] = target;
    }

    return true;
}

void VulkanSwapChain::cleanupNativeSwapchain() {
    VkDevice device = m_deviceContext->vulkanDevice();

    for (auto& iv : m_swapChainImageViews) {
        vkDestroyImageView(device, iv, m_deviceContext->vulkanAllocator());
    }
    m_swapChainImageViews.clear();

    for (auto& x : m_swapchainRenderTargets) {
        x->dispose();
    }
    m_swapchainRenderTargets.clear();

    if (m_swapchain) {
        vkDestroySwapchainKHR(device, m_swapchain, m_deviceContext->vulkanAllocator());
        m_swapchain = VK_NULL_HANDLE;
    }
}

void VulkanSwapChain::acquireNextImage(int* outIndex) {
    VkResult result = vkAcquireNextImageKHR(
        m_deviceContext->vulkanDevice(), vulkanSwapchain(), std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &m_imageIndex);

    // 次に present や readData など、この RenderTarget への書き込みコマンドを実行するとき、これを待たなければならない
    m_swapchainRenderTargets[m_imageIndex]->setSwapchainImageAvailableSemaphoreRef(&m_imageAvailableSemaphores[m_currentFrame]);

    *outIndex = m_imageIndex;
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        //recreateSwapChain();
        throw std::runtime_error("failed to acquire swap chain image!");
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }
}

RHIResource* VulkanSwapChain::getRenderTarget(int imageIndex) const {
    return m_swapchainRenderTargets[imageIndex];
}

Result VulkanSwapChain::resizeBackbuffer(uint32_t width, uint32_t height) {
    vkDeviceWaitIdle(m_deviceContext->vulkanDevice());

    cleanupNativeSwapchain();

    if (!createNativeSwapchain(SizeI(width, height))) {
        return err();
    }

    return ok();
}

void VulkanSwapChain::present() {
    VkSemaphore renderFinishedSemaphore = m_swapchainRenderTargets[m_imageIndex]->renderFinishedSemaphore();
    //m_deviceContext->recodingCommandBuffer()->submit(
    //    m_swapchainRenderTargets[m_imageIndex]->imageAvailableSemaphore(),
    //    renderFinishedSemaphore);

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderFinishedSemaphore; // このセマフォの通知を待ってから実際に present する

    VkSwapchainKHR swapChains[] = { vulkanSwapchain() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &m_imageIndex;

    VkResult result = vkQueuePresentKHR(m_presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        //framebufferResized = false;
        throw std::runtime_error("failed to present swap chain image!");
        // TODO: recreate
    }
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    //// Swap command buffer
    //{
    //    auto t = m_deviceContext->graphicsContext()->recodingCommandBuffer();
    //    m_deviceContext->graphicsContext()->setRecodingCommandBuffer(m_inFlightCommandBuffers[m_currentFrame]);
    //    m_inFlightCommandBuffers[m_currentFrame] = t;
    //}

    m_currentFrame = (m_currentFrame + 1) % maxFrameCount();
}

VkSurfaceFormatKHR VulkanSwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
        return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
    }

    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR VulkanSwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {
    VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
        else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
            bestMode = availablePresentMode;
        }
    }

    return bestMode;
}

VkExtent2D VulkanSwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t requiredWidth, uint32_t requiredHeight) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }
    else {
        VkExtent2D actualExtent = {
            requiredWidth,
            requiredHeight
        };

        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

SwapChainSupportDetails VulkanSwapChain::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    if (details.capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
        details.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }
    else {
        details.preTransform = details.capabilities.currentTransform;
    }

    // Find a supported composite alpha mode
    details.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    VkCompositeAlphaFlagBitsKHR compositeAlphaFlags[4] = {
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
    };
    for (uint32_t i = 0; i < sizeof(compositeAlphaFlags) / sizeof(compositeAlphaFlags[0]); i++) {
        if (details.capabilities.supportedCompositeAlpha & compositeAlphaFlags[i]) {
            details.compositeAlpha = compositeAlphaFlags[i];
            break;
        }
    }

    return details;
}

//==============================================================================
// VulkanRenderPass2

VulkanRenderPass2::VulkanRenderPass2()
    : m_device(nullptr)
    , m_nativeRenderPass(VK_NULL_HANDLE)
    , m_framebuffer(nullptr)
    , m_clearFlags(ClearFlags::None)
    , m_clearColor()
    , m_clearDepth(1.0f)
    , m_clearStencil(0x00)
    , m_containsMultisampleTarget(false) {
}

Result VulkanRenderPass2::init(VulkanDevice* device, const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil) {
    LN_CHECK(device);
    m_device = device;
    m_clearFlags = clearFlags;
    m_clearColor = clearColor;
    m_clearDepth = clearDepth;
    m_clearStencil = clearStencil;

    auto colorLoadOp = testFlag(clearFlags, ClearFlags::Color) ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
    auto depthLoadOp = testFlag(clearFlags, ClearFlags::Depth) ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
    auto stencilLoadOp = testFlag(clearFlags, ClearFlags::Stencil) ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;

    // Note: LoadOp はパフォーマンを考えなければ常に VK_ATTACHMENT_LOAD_OP_LOAD でもかまわないように見えるが、
    // RenderTarget 生成直後は必ず VK_ATTACHMENT_LOAD_OP_CLEAR を指定しないと GPU が落ちることがある。
    // 一方 StoreOp は RenderPass end 後に、RenderTarget の内容が不要であるかを指定することで GPU がメモリを再利用できるようにするための仕組みだが、
    // パフォーマンを考えなければ常に VK_ATTACHMENT_STORE_OP_STORE のままで構わない。

    // attachments は普段は次のようなレイアウトになる
    // [0] RenderTarget0
    // [1] RenderTarget1
    // [2] RenderTarget2
    // [3] RenderTarget3
    // [4] DepthBuffer
    //
    // RenderTarget がひとつしか使われていない場合は次のようになる
    // [0] RenderTarget0
    // [1] DepthBuffer
    //
    // MSAA 有効時は、DepthBuffer の前に Resolve 用のターゲットが入る
    // [0] MultisampleColorTarget(RenderTarget0)
    // [1] ResolveColorTarget(RenderTarget0)
    // [2] DepthBuffer

    // MaxRenderTargets + Resolve-MaxRenderTargets(for MSAA) + depthbuffer
    std::array<VkAttachmentDescription, MaxMultiRenderTargets * 2 + 1> attachmentDescs;
    std::array<VkAttachmentReference, MaxMultiRenderTargets + 1> attachmentRefs;
    VkAttachmentReference* depthAttachmentRef = nullptr;
    std::array<VkAttachmentReference, MaxMultiRenderTargets> resolveRefs;
    int attachmentCount = 0;
    int colorAttachmentCount = 0;
    ;
    int resolveCount = 0;
    for (int i = 0; i < MaxMultiRenderTargets; i++) {
        if (buffers.renderTargets[i]) {
            VulkanRenderTarget* renderTarget = static_cast<VulkanRenderTarget*>(buffers.renderTargets[i]);

            attachmentDescs[i].flags = 0;
            if (renderTarget->isMultisample()) {
                attachmentDescs[i].format = renderTarget->multisampleColorBuffer()->vulkanFormat();
                attachmentDescs[i].samples = renderTarget->msaaSamples();
            }
            else {
                attachmentDescs[i].format = renderTarget->image()->vulkanFormat();
                attachmentDescs[i].samples = VK_SAMPLE_COUNT_1_BIT;
            }
            //attachmentDescs[i].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;//VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            attachmentDescs[i].loadOp = colorLoadOp; // (loadOpClear) ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;// サンプルでは画面全体 clear する前提なので、前回値を保持する必要はない。そのため CLEAR。というか、CLEAR 指定しないと clear しても背景真っ黒になった。
            attachmentDescs[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachmentDescs[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;   // VK_ATTACHMENT_LOAD_OP_LOAD;// VK_ATTACHMENT_LOAD_OP_DONT_CARE;    // TODO: stencil。今は未対応
            attachmentDescs[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; //VK_ATTACHMENT_STORE_OP_STORE; //VK_ATTACHMENT_STORE_OP_DONT_CARE;//    // TODO: stencil。今は未対応
            if (renderTarget->isSwapchainBackbuffer()) {
                if (renderTarget->isMultisample()) {
                    LN_NOTIMPLEMENTED();
                    return err();
                }

                // swapchain の場合
                // ※Barrier に乗せて遷移させることは許可されていない。ここで何とかする必要がある。
                // https://stackoverflow.com/questions/37524032/how-to-deal-with-the-layouts-of-presentable-images
                // validation layer: Submitted command buffer expects image 0x50 (subresource: aspectMask 0x1 array layer 0, mip level 0) to be in layout VK_IMAGE_LAYOUT_PRESENT_SRC_KHR--instead, image 0x50's current layout is VK_IMAGE_LAYOUT_UNDEFINED.
                attachmentDescs[i].initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // VK_IMAGE_LAYOUT_UNDEFINED;     // レンダリング前のレイアウト定義。UNDEFINED はレイアウトは何でもよいが、内容の保証はない。サンプルでは全体 clear するので問題ない。
                attachmentDescs[i].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            }
            else {
                attachmentDescs[i].initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; // DONT_CARE と併用する場合は UNDEFINED にしておくとよい

                // パス終了後はシェーダ入力(テクスチャ)として使用できるように VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL に遷移する。
                // https://qiita.com/Pctg-x8/items/a1a39678e9ca95c59d19
                attachmentDescs[i].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            }

            attachmentRefs[i].attachment = attachmentCount;

            // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL まはた GENERAL でなければならない。(https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-VkRenderPassCreateInfo-pAttachments-00836)
            // = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ではダメ。
            // 要するに、描画中のレンダーターゲットは書き込み可能なレイアウトにしておきなさい、ということ。
            attachmentRefs[i].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            attachmentCount++;
            colorAttachmentCount++;
        }
        else {
            break;
        }
    }

    for (int i = 0; i < MaxMultiRenderTargets; i++) {
        if (buffers.renderTargets[i]) {
            VulkanRenderTarget* renderTarget = static_cast<VulkanRenderTarget*>(buffers.renderTargets[i]);
            if (renderTarget->isMultisample()) {
                m_containsMultisampleTarget = true;
                resolveRefs[i].attachment = colorAttachmentCount + i;
                resolveRefs[i].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                if (renderTarget->isSwapchainBackbuffer()) {
                    LN_NOTIMPLEMENTED();
                    return err();
                }

                VkAttachmentDescription& colorAttachmentResolve = attachmentDescs[attachmentCount];
                colorAttachmentResolve.format = renderTarget->image()->vulkanFormat();
                colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
                colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
                colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

                attachmentCount++;
            }
            resolveCount++;
        }
    }

    if (buffers.depthBuffer) {
        VulkanDepthBuffer* depthBuffer = static_cast<VulkanDepthBuffer*>(buffers.depthBuffer);
        int i = attachmentCount;

        attachmentDescs[i].flags = 0;
        attachmentDescs[i].format = depthBuffer->nativeFormat(); //m_device->findDepthFormat();//VK_FORMAT_D32_SFLOAT_S8_UINT;
        attachmentDescs[i].samples = VK_SAMPLE_COUNT_1_BIT;
        attachmentDescs[i].loadOp = depthLoadOp; // (loadOpClear) ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;// VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        //attachmentDescs[i].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;// VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentDescs[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;                           //VK_ATTACHMENT_STORE_OP_DONT_CARE;//
        attachmentDescs[i].stencilLoadOp = stencilLoadOp;                                    // VK_ATTACHMENT_LOAD_OP_LOAD;// VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentDescs[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;                    // VK_ATTACHMENT_STORE_OP_DONT_CARE;// VK_ATTACHMENT_STORE_OP_STORE;
        attachmentDescs[i].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; // VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
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
    subpass.pColorAttachments = attachmentRefs.data();
    subpass.pResolveAttachments = (m_containsMultisampleTarget) ? resolveRefs.data() : nullptr; // 指定する場合、要素数は colorAttachmentCount でなければならない (https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkSubpassDescription.html)
    subpass.pDepthStencilAttachment = depthAttachmentRef;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = nullptr;

    std::array<VkSubpassDependency, 2> dependencies;

    // この RenderPass より前の RenderPass で内容が作られることを示す
    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
    dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    // この RenderPass の後の RenderPass で内容が参照されることを示す
    dependencies[1].srcSubpass = 0;
    dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.pNext = nullptr;
    renderPassInfo.flags = 0;
    renderPassInfo.attachmentCount = attachmentCount;
    renderPassInfo.pAttachments = attachmentDescs.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    //renderPassInfo.dependencyCount = 1;
    //renderPassInfo.pDependencies = &dependency;
    renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
    renderPassInfo.pDependencies = dependencies.data();

    LN_VK_CHECK(vkCreateRenderPass(m_device->vulkanDevice(), &renderPassInfo, m_device->vulkanAllocator(), &m_nativeRenderPass));

    m_framebuffer = makeRef<VulkanFramebuffer2>();
    if (!m_framebuffer->init(m_device, this, buffers)) {
        return err();
    }

#ifdef LN_DEBUG
    memcpy(m_attachmentDescs, attachmentDescs.data(), sizeof(m_attachmentDescs));
    memcpy(m_attachmentRefs, attachmentRefs.data(), sizeof(m_attachmentRefs));
#endif

    return ok();
}

void VulkanRenderPass2::dispose() {
    if (m_framebuffer) {
        m_framebuffer->dispose();
        m_framebuffer = nullptr;
    }

    if (m_nativeRenderPass) {
        vkDestroyRenderPass(m_device->vulkanDevice(), m_nativeRenderPass, m_device->vulkanAllocator());
        m_nativeRenderPass = VK_NULL_HANDLE;
    }

    m_device = nullptr;

    IRenderPass::dispose();
}

//==============================================================================
// VulkanFramebuffer2

VulkanFramebuffer2::VulkanFramebuffer2()
    : m_device(nullptr)
    , m_ownerRenderPass(nullptr)
    , m_framebuffer(VK_NULL_HANDLE)
    , m_renderTargets{}
    , m_depthBuffer(nullptr) {
}

Result VulkanFramebuffer2::init(VulkanDevice* device, VulkanRenderPass2* ownerRenderPass, const DeviceFramebufferState& state) {
    LN_CHECK(device);
    LN_CHECK(ownerRenderPass);
    m_device = device;
    m_ownerRenderPass = ownerRenderPass;

    for (size_t i = 0; i < state.renderTargets.size(); i++) {
        m_renderTargets[i] = static_cast<VulkanRenderTarget*>(state.renderTargets[i]);
    }
    m_depthBuffer = static_cast<VulkanDepthBuffer*>(state.depthBuffer);

    // MaxRenderTargets + Resolve-MaxRenderTargets(for MSAA) + depthbuffer
    std::array<VkImageView, MaxMultiRenderTargets * 2 + 1> attachments;
    int attachmentsCount = 0;
    for (size_t i = 0; i < m_renderTargets.size(); i++) {
        if (m_renderTargets[i]) {
            if (m_renderTargets[i]->isMultisample()) {
                attachments[attachmentsCount] = m_renderTargets[i]->multisampleColorBuffer()->vulkanImageView();
            }
            else {
                attachments[attachmentsCount] = m_renderTargets[i]->image()->vulkanImageView();
            }
            attachmentsCount++;
        }
    }
    for (size_t i = 0; i < m_renderTargets.size(); i++) {
        if (m_renderTargets[i]) {
            if (m_renderTargets[i]->isMultisample()) {
                attachments[attachmentsCount] = m_renderTargets[i]->image()->vulkanImageView();
                attachmentsCount++;
            }
        }
    }
    if (m_depthBuffer) {
        attachments[attachmentsCount] = m_depthBuffer->image()->vulkanImageView();
        attachmentsCount++;
    }

    _sizeBase = m_renderTargets[0];
    _sizeBaseImg = _sizeBase->image();
    _baseImg = _sizeBaseImg->vulkanImage();
    _baseId = _sizeBase->objectId();
    const auto imageSize = m_renderTargets[0]->extentSize();
    VkFramebufferCreateInfo framebufferInfo = {};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.pNext = nullptr;
    framebufferInfo.flags = 0;
    framebufferInfo.renderPass = m_ownerRenderPass->nativeRenderPass();
    framebufferInfo.attachmentCount = attachmentsCount;
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = imageSize.width;
    framebufferInfo.height = imageSize.height;
    framebufferInfo.layers = 1;

    LN_VK_CHECK(vkCreateFramebuffer(m_device->vulkanDevice(), &framebufferInfo, m_device->vulkanAllocator(), &m_framebuffer));
    ff = framebufferInfo;
    return ok();
}

void VulkanFramebuffer2::dispose() {
    if (m_framebuffer) {
        vkDestroyFramebuffer(m_device->vulkanDevice(), m_framebuffer, m_device->vulkanAllocator());
        m_framebuffer = VK_NULL_HANDLE;
    }

    m_renderTargets = {};
    m_depthBuffer = nullptr;
    m_ownerRenderPass = nullptr;
    m_device = nullptr;
}

//==============================================================================
// VulkanPipeline2

VulkanPipeline2::VulkanPipeline2()
    : m_device(nullptr)
    , m_ownerRenderPass(nullptr)
    , m_pipeline(VK_NULL_HANDLE) {
}

Result VulkanPipeline2::init(VulkanDevice* deviceContext, const DevicePipelineStateDesc& state) {
    LN_DCHECK(deviceContext);
    m_device = deviceContext;

    if (state.renderPass) {
        return createGraphicsPipeline(state);
    }
    else {
        return createComputePipeline(state);
    }
}

void VulkanPipeline2::dispose() {
    if (m_pipeline) {
        vkDestroyPipeline(m_device->vulkanDevice(), m_pipeline, m_device->vulkanAllocator());
        m_pipeline = VK_NULL_HANDLE;
    }

    m_ownerRenderPass = nullptr;
    m_device = nullptr;
    IPipeline::dispose();
}

Result VulkanPipeline2::createGraphicsPipeline(const DevicePipelineStateDesc& state) {
    LN_DCHECK(state.renderPass);
    m_ownerRenderPass = static_cast<VulkanRenderPass2*>(state.renderPass);

    auto* vertexDeclaration = static_cast<VulkanVertexDeclaration*>(state.vertexDeclaration);
    auto* shaderPass = static_cast<VulkanShaderPass*>(state.shaderPass);
    //m_relatedFramebuffer = m_deviceContext->framebufferCache()->findOrCreate(state.framebufferState);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = shaderPass->vulkanVertShaderModule();
    vertShaderStageInfo.pName = shaderPass->vertEntryPointName().c_str();

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = shaderPass->vulkanFragShaderModule();
    fragShaderStageInfo.pName = shaderPass->fragEntryPointName().c_str();

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    //if (shaderPass->isComputeShader()) {
    //    VkPipelineShaderStageCreateInfo csShaderStageInfo = {};
    //    csShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    //    csShaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    //    csShaderStageInfo.module = shaderPass->vulkanCompShaderModule();
    //    csShaderStageInfo.pName = shaderPass->compEntryPointName().c_str();
    //}

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
    {
        const auto& attrs = shaderPass->attributes();
        for (size_t i = 0; i < attrs.size(); i++) {
            const auto& attr = attrs[i];
            if (const auto* s = vertexDeclaration->findAttributeDescriptionSource(attr.usage, attr.index)) {
                VkVertexInputAttributeDescription desc;
                desc.location = i;
                desc.binding = s->binding;
                desc.format = s->format;
                desc.offset = s->offset;
                attributeDescriptions.push_back(desc);
            }
            else {
                // Shader が必要としている情報が頂点バッファ側に足りていない。警告が出る。
            }
        }
    }

    const auto& bindingDescription = vertexDeclaration->vertexBindingDescriptions();
    vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescription.size());
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = bindingDescription.data();
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VulkanHelper::LNPrimitiveTopologyToVkPrimitiveTopology(state.topology);
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport = {};
    //viewport.x = 0.0f;
    //viewport.y = 0.0f;
    //viewport.width = state.regionRects.viewportRect.width; //(float)swapChainExtent.width;
    //viewport.height = state.regionRects.viewportRect.height;//(float)swapChainExtent.height;
    //viewport.minDepth = 0.0f;
    //viewport.maxDepth = 1.0f;

    VkRect2D scissor = {};
    //scissor.offset = { 0, 0 };
    //scissor.extent.width = state.regionRects.scissorRect.width;;
    //scissor.extent.height = state.regionRects.scissorRect.height;;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizerInfo = {};
    {
        const RasterizerStateDesc& desc = state.rasterizerState;

        rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizerInfo.depthClampEnable = VK_FALSE;
        rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
        rasterizerInfo.polygonMode = VulkanHelper::LNFillModeToVkPolygonMode(desc.fillMode);
        rasterizerInfo.cullMode = VulkanHelper::LNCullModeToVkCullMode(desc.cullMode);
#ifdef LN_COORD_RH
        rasterizerInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
#else
        rasterizerInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
#endif
        rasterizerInfo.depthBiasEnable = VK_FALSE;
        rasterizerInfo.depthBiasConstantFactor = 0.0f;
        rasterizerInfo.depthBiasClamp = 0.0f;
        rasterizerInfo.depthBiasSlopeFactor = 0.0f;
        rasterizerInfo.lineWidth = 1.0f;
    }
    //VkPipelineRasterizationStateCreateInfo rasterizer = {};
    //rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    //rasterizer.depthClampEnable = VK_FALSE;
    //rasterizer.rasterizerDiscardEnable = VK_FALSE;
    //rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    //rasterizer.lineWidth = 1.0f;
    //rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    //rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE; // Viewport height を反転しているので、時計回りを正面 //VK_FRONT_FACE_COUNTER_CLOCKWISE;
    //rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = m_ownerRenderPass->containsMultisampleTarget() ? m_device->msaaSamples() : VK_SAMPLE_COUNT_1_BIT;

    //VkPipelineDepthStencilStateCreateInfo depthStencil = {};
    //depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    //depthStencil.depthTestEnable = VK_TRUE;
    //depthStencil.depthWriteEnable = VK_TRUE;
    //depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    //depthStencil.depthBoundsTestEnable = VK_FALSE;
    //depthStencil.stencilTestEnable = VK_FALSE;
    VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo = {};
    {
        const DepthStencilStateDesc& desc = state.depthStencilState;

        depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencilStateInfo.pNext = nullptr;
        depthStencilStateInfo.flags = 0;
        depthStencilStateInfo.depthTestEnable = (desc.depthTestFunc == ComparisonFunc::Always ? VK_FALSE : VK_TRUE);
        depthStencilStateInfo.depthWriteEnable = (desc.depthWriteEnabled ? VK_TRUE : VK_FALSE);
        depthStencilStateInfo.depthCompareOp = VulkanHelper::LNComparisonFuncToVkCompareOp(desc.depthTestFunc);
        depthStencilStateInfo.depthBoundsTestEnable = VK_FALSE;
        depthStencilStateInfo.stencilTestEnable = (desc.stencilEnabled ? VK_TRUE : VK_FALSE);

        depthStencilStateInfo.front.failOp = VulkanHelper::LNStencilOpToVkStencilOp(desc.frontFace.stencilFailOp);
        depthStencilStateInfo.front.passOp = VulkanHelper::LNStencilOpToVkStencilOp(desc.frontFace.stencilPassOp);
        depthStencilStateInfo.front.depthFailOp = VulkanHelper::LNStencilOpToVkStencilOp(desc.frontFace.stencilDepthFailOp);
        depthStencilStateInfo.front.compareOp = VulkanHelper::LNComparisonFuncToVkCompareOp(desc.frontFace.stencilFunc);
        depthStencilStateInfo.front.compareMask = 0xff;
        depthStencilStateInfo.front.writeMask = 0xff;
        depthStencilStateInfo.front.reference = desc.stencilReferenceValue;

        depthStencilStateInfo.back.failOp = VulkanHelper::LNStencilOpToVkStencilOp(desc.backFace.stencilFailOp);
        depthStencilStateInfo.back.passOp = VulkanHelper::LNStencilOpToVkStencilOp(desc.backFace.stencilPassOp);
        depthStencilStateInfo.back.depthFailOp = VulkanHelper::LNStencilOpToVkStencilOp(desc.backFace.stencilDepthFailOp);
        depthStencilStateInfo.back.compareOp = VulkanHelper::LNComparisonFuncToVkCompareOp(desc.backFace.stencilFunc);
        depthStencilStateInfo.back.compareMask = 0xff;
        depthStencilStateInfo.back.writeMask = 0xff;
        depthStencilStateInfo.back.reference = desc.stencilReferenceValue;

        depthStencilStateInfo.minDepthBounds = 0.0f;
        depthStencilStateInfo.maxDepthBounds = 0.0f;
    }

    auto& framebuffer = m_ownerRenderPass->framebuffer();
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    VkPipelineColorBlendAttachmentState colorBlendAttachments[BlendStateDesc::MaxRenderTargets] = {};
    {
        const BlendStateDesc& desc = state.blendState;
        int attachmentsCount = 0;
        for (int i = 0; i < BlendStateDesc::MaxRenderTargets; i++) {
            if (framebuffer->renderTargets()[i]) {
                const RenderTargetBlendDesc* atdesc = (desc.independentBlendEnable) ? &desc.renderTargets[i] : &desc.renderTargets[0];

                colorBlendAttachments[i].blendEnable = (atdesc->blendEnable) ? VK_TRUE : VK_FALSE;

                colorBlendAttachments[i].srcColorBlendFactor = VulkanHelper::LNBlendFactorToVkBlendFactor_Color(atdesc->sourceBlend);
                colorBlendAttachments[i].dstColorBlendFactor = VulkanHelper::LNBlendFactorToVkBlendFactor_Color(atdesc->destinationBlend);
                colorBlendAttachments[i].colorBlendOp = VulkanHelper::LNBlendOpToVkBlendOp(atdesc->blendOp);

                colorBlendAttachments[i].srcAlphaBlendFactor = VulkanHelper::LNBlendFactorToVkBlendFactor_Alpha(atdesc->sourceBlendAlpha);
                colorBlendAttachments[i].dstAlphaBlendFactor = VulkanHelper::LNBlendFactorToVkBlendFactor_Alpha(atdesc->destinationBlendAlpha);
                colorBlendAttachments[i].alphaBlendOp = VulkanHelper::LNBlendOpToVkBlendOp(atdesc->blendOpAlpha);

                colorBlendAttachments[i].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

                attachmentsCount++;
            }
        }

        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = attachmentsCount;
        colorBlending.pAttachments = colorBlendAttachments;
        colorBlending.blendConstants[0] = 1.0f;
        colorBlending.blendConstants[1] = 1.0f;
        colorBlending.blendConstants[2] = 1.0f;
        colorBlending.blendConstants[3] = 1.0f;
    }
    //VkPipelineColorBlendStateCreateInfo colorBlending = {};
    //colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    //colorBlending.logicOpEnable = VK_FALSE;
    //colorBlending.logicOp = VK_LOGIC_OP_COPY;
    //colorBlending.attachmentCount = 1;
    //colorBlending.pAttachments = &colorBlendAttachment;
    //colorBlending.blendConstants[0] = 0.0f;
    //colorBlending.blendConstants[1] = 0.0f;
    //colorBlending.blendConstants[2] = 0.0f;
    //colorBlending.blendConstants[3] = 0.0f;

    const VkDynamicState dynamicStates[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        //VK_DYNAMIC_STATE_BLEND_CONSTANTS,
        //VK_DYNAMIC_STATE_STENCIL_REFERENCE,
    };
    VkPipelineDynamicStateCreateInfo dynamicState;
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pNext = nullptr;
    dynamicState.flags = 0;
    dynamicState.dynamicStateCount = LN_ARRAY_SIZE_OF(dynamicStates);
    dynamicState.pDynamicStates = dynamicStates;

    //VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    //pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    //pipelineLayoutInfo.setLayoutCount = m_shaderPass->descriptorSetLayouts().size();
    //pipelineLayoutInfo.pSetLayouts = m_shaderPass->descriptorSetLayouts().data();

    //if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
    //    throw std::runtime_error("failed to create pipeline layout!");
    //}

    //renderPass = framebuffer->vulkanRenderPass();//

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stageCount = 2;
    pipelineCreateInfo.pStages = shaderStages;
    pipelineCreateInfo.pVertexInputState = &vertexInputInfo;
    pipelineCreateInfo.pInputAssemblyState = &inputAssembly;
    pipelineCreateInfo.pViewportState = &viewportState;
    pipelineCreateInfo.pRasterizationState = &rasterizerInfo;
    pipelineCreateInfo.pMultisampleState = &multisampling;
    pipelineCreateInfo.pDepthStencilState = &depthStencilStateInfo;
    pipelineCreateInfo.pColorBlendState = &colorBlending;
    pipelineCreateInfo.layout = shaderPass->vulkanPipelineLayout(); // 省略不可 https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkGraphicsPipelineCreateInfo.html
    pipelineCreateInfo.pDynamicState = &dynamicState;
    pipelineCreateInfo.renderPass = m_ownerRenderPass->nativeRenderPass();
    pipelineCreateInfo.subpass = 0;
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

    LN_VK_CHECK(vkCreateGraphicsPipelines(m_device->vulkanDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, m_device->vulkanAllocator(), &m_pipeline));

    return ok();
}

Result VulkanPipeline2::createComputePipeline(const DevicePipelineStateDesc& state) {
    auto* shaderPass = static_cast<VulkanShaderPass*>(state.shaderPass);

    VkPipelineShaderStageCreateInfo shaderStage = {};
    shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStage.flags = 0;
    shaderStage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    shaderStage.module = shaderPass->vulkanCompShaderModule();
    shaderStage.pName = shaderPass->compEntryPointName().c_str();
    shaderStage.pSpecializationInfo = nullptr;

    // Create pipeline
    VkComputePipelineCreateInfo pipelineCreateInfo; // = vks::initializers::computePipelineCreateInfo(compute.pipelineLayout, 0);
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.pNext = nullptr;
    ;
    pipelineCreateInfo.flags = 0;
    pipelineCreateInfo.stage = shaderStage;
    pipelineCreateInfo.layout = shaderPass->vulkanPipelineLayout();
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineCreateInfo.basePipelineIndex = 0;
    //computePipelineCreateInfo.stage = loadShader(getShadersPath() + "computecloth/cloth.comp.spv", VK_SHADER_STAGE_COMPUTE_BIT);

    //VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    //pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    //pipelineInfo.stageCount = 2;
    //pipelineInfo.pStages = shaderStages;
    //pipelineInfo.pVertexInputState = &vertexInputInfo;
    //pipelineInfo.pInputAssemblyState = &inputAssembly;
    //pipelineInfo.pViewportState = &viewportState;
    //pipelineInfo.pRasterizationState = &rasterizerInfo;
    //pipelineInfo.pMultisampleState = &multisampling;
    //pipelineInfo.pDepthStencilState = &depthStencilStateInfo;
    //pipelineInfo.pColorBlendState = &colorBlending;
    //pipelineInfo.layout = shaderPass->vulkanPipelineLayout();	// 省略不可 https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkGraphicsPipelineCreateInfo.html
    //pipelineInfo.pDynamicState = &dynamicState;
    //pipelineInfo.renderPass = m_ownerRenderPass->nativeRenderPass();
    //pipelineInfo.subpass = 0;
    //pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    LN_VK_CHECK(vkCreateComputePipelines(m_device->vulkanDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &m_pipeline));

    return ok();
}

//==============================================================================
// VulkanVertexDeclaration

VulkanVertexDeclaration::VulkanVertexDeclaration() {
}

// https://gist.github.com/SaschaWillems/428d15ed4b5d71ead462bc63adffa93a
Result VulkanVertexDeclaration::init(const VertexElement* elements, int elementsCount) {
    LN_DCHECK(elements);

    // 事前に binding がどれだけあるのか調べる
    m_maxStreamCount = 0;
    for (int i = 0; i < elementsCount; i++) {
        m_maxStreamCount = std::max(m_maxStreamCount, elements[i].StreamIndex);
        m_elements.push_back(elements[i]);
    }
    m_maxStreamCount++;
    m_bindings.resize(m_maxStreamCount);

    for (int i = 0; i < m_maxStreamCount; i++) {
        m_bindings[i].binding = i; // VkVertexInputAttributeDescription が参照する binding 番号。ひとまず連番

        for (int j = 0; j < elementsCount; j++) {
            if (elements[j].StreamIndex == i) {
                m_bindings[i].inputRate = (elements[j].rate == VertexInputRate::Vertex) ? VK_VERTEX_INPUT_RATE_VERTEX : VK_VERTEX_INPUT_RATE_INSTANCE;
            }
        }

        //m_bindings[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    }

    // 実際に値を計算する
    for (int i = 0; i < elementsCount; i++) {
        auto& element = m_elements[i];

        AttributeDescriptionSource attr;
        attr.usage = element.Usage;
        attr.usageIndex = element.UsageIndex;
        attr.binding = element.StreamIndex;
        attr.format = VulkanHelper::LNVertexElementTypeToVkFormat(elements[i].Type);
        attr.offset = m_bindings[attr.binding].stride;
        m_attributeSources.push_back(attr);

        m_bindings[element.StreamIndex].stride += GraphicsHelper::getVertexElementTypeSize(elements[i].Type);
    }

    return ok();
}

void VulkanVertexDeclaration::dispose() {
    IVertexDeclaration::dispose();
}

const VulkanVertexDeclaration::AttributeDescriptionSource* VulkanVertexDeclaration::findAttributeDescriptionSource(AttributeUsage usage, int usageIndex) const {
    // TODO: これ線形探索じゃなくて、map 作った方がいいかも。
    // usage の種類は固定だし、usageIndex も最大 16 あれば十分だし、byte 型 8x16 くらいの Matrix で足りる。
    auto u = IGraphicsHelper::AttributeUsageToElementUsage(usage);
    for (auto& e : m_attributeSources) {
        if (e.usage == u && e.usageIndex == usageIndex) {
            return &e;
        }
    }
    return nullptr;
}

//==============================================================================
// VulkanSamplerState

VulkanSamplerState::VulkanSamplerState()
    : m_deviceContext(nullptr)
    , m_sampler(VK_NULL_HANDLE) {
}

Result VulkanSamplerState::init(VulkanDevice* deviceContext, const SamplerStateData& desc) {
    LN_DCHECK(deviceContext);
    m_deviceContext = deviceContext;

    VkFilter filter = VulkanHelper::LNTextureFilterModeToVkFilter(desc.filter);
    VkSamplerAddressMode address = VulkanHelper::LNTextureAddressModeModeToVkSamplerAddressMode(desc.address);

    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = filter;
    samplerInfo.minFilter = filter;
    samplerInfo.addressModeU = address;
    samplerInfo.addressModeV = address;
    samplerInfo.addressModeW = address;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = 0;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;

    samplerInfo.maxLod = 8;

    if (desc.anisotropy) {
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = 16;
    }

    LN_VK_CHECK(vkCreateSampler(m_deviceContext->vulkanDevice(), &samplerInfo, m_deviceContext->vulkanAllocator(), &m_sampler));

    return ok();
}

void VulkanSamplerState::dispose() {
    if (m_sampler) {
        vkDestroySampler(m_deviceContext->vulkanDevice(), m_sampler, m_deviceContext->vulkanAllocator());
        m_sampler = VK_NULL_HANDLE;
    }

    m_deviceContext = nullptr;
    ISamplerState::dispose();
}

//==============================================================================
// VulkanNativeGraphicsInterface

VulkanNativeGraphicsInterface::VulkanNativeGraphicsInterface(VulkanDevice* device)
    : m_device(device)
    , m_context(nullptr) {
}

void VulkanNativeGraphicsInterface::setContext(VulkanGraphicsContext* context) {
    m_context = context;
}

GraphicsAPI VulkanNativeGraphicsInterface::getGraphicsAPI() const {
    return GraphicsAPI::Vulkan;
}

VkInstance VulkanNativeGraphicsInterface::getInstance() const {
    return m_device->vulkanInstance();
}

VkPhysicalDevice VulkanNativeGraphicsInterface::getPhysicalDevice() const {
    return m_device->vulkanPhysicalDevice();
}

VkDevice VulkanNativeGraphicsInterface::getDevice() const {
    return m_device->vulkanDevice();
}

VkQueue VulkanNativeGraphicsInterface::getGraphicsQueue() const {
    return m_device->m_graphicsQueue;
}

uint32_t VulkanNativeGraphicsInterface::getGraphicsQueueFamilyIndex() const {
    return m_device->m_graphicsQueueFamilyIndex;
}

VkCommandBuffer VulkanNativeGraphicsInterface::getRecordingCommandBuffer() const {
    return m_context->recodingCommandBuffer()->vulkanCommandBuffer();
}

} // namespace detail
} // namespace ln
