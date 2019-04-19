
#include "Internal.hpp"
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/Platform/PlatformSupport.hpp>

//#define GLFW_INCLUDE_VULKAN
//#include <GLFW/glfw3.h>
//#include "../Platform/GLFWPlatformWindowManager.hpp"

#include "VulkanDeviceContext.hpp"

//#define STB_IMAGE_IMPLEMENTATION
//#include "../../../../build/ExternalSource/stb/stb_image.h"

//#include <iostream>
//#include <fstream>
//#include <stdexcept>
//#include <algorithm>
//#include <chrono>
//#include <vector>
//#include <cstring>
//#include <cstdlib>
//#include <array>
//#include <optional>
//#include <set>

namespace ln {
namespace detail {

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}





//==============================================================================
// VulkanDevice

VulkanDevice::VulkanDevice()
    : m_instance(VK_NULL_HANDLE)
    , m_debugMessenger(VK_NULL_HANDLE)
	, m_graphicsContext(nullptr)
{
}

bool VulkanDevice::init(const Settings& settings, bool* outIsDriverSupported)
{
	if (LN_REQUIRE(outIsDriverSupported)) return false;
	*outIsDriverSupported = true;

	if (!VulkanHelper::initVulkanFunctions()) {
		LN_LOG_WARNING << "Valid vulkan library not found.";
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

	if (!m_renderPassCache.init(this)) {
		return false;
	}
	if (!m_framebufferCache.init(this)) {
		return false;
	}
	if (!m_pipelineCache.init(this)) {
		return false;
	}

	m_graphicsContext = makeRef<VulkanGraphicsContext>();
	if (!m_graphicsContext->init(this)) {
		return false;
	}

	return true;
}

void VulkanDevice::dispose()
{
    // まずは CommandBuffer の dispose() を行う。
    // CommandBuffer は最後に Submit した時、コマンドリストが参照しているリソースを開放しないように、影響リソースの参照を持っている。
    // CommandBuffer は次回描画開始時や、解放のタイミング (今の時点) で、これらのリソース参照をはずす。
    // このとき、ShaderPass に対しては、VulkanDescriptorSetsPool を返却する。ShaderPass は dispose でこの VulkanDescriptorSetsPool を削除するので、
    // ShaderPass を dispose ずる前に CommandBuffer の dispose() を行う必要がある。
	if (m_graphicsContext) {
		m_graphicsContext->dispose();
		m_graphicsContext = nullptr;
	}

    IGraphicsDevice::dispose();


    m_pipelineCache.dispose();
    m_framebufferCache.dispose();
    m_renderPassCache.dispose();

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

IGraphicsContext* VulkanDevice::getGraphicsContext() const
{
	return m_graphicsContext;
}

void VulkanDevice::onGetCaps(GraphicsDeviceCaps * outCaps)
{
    outCaps->requestedShaderTriple.target = "spv";
    outCaps->requestedShaderTriple.version = 110;
    outCaps->requestedShaderTriple.option = "";
}

void VulkanDevice::onEnterMainThread()
{
}

void VulkanDevice::onLeaveMainThread() 
{
}

void VulkanDevice::onSaveExternalRenderState()
{
}

void VulkanDevice::onRestoreExternalRenderState()
{
}

Ref<ISwapChain> VulkanDevice::onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize)
{
	auto ptr = makeRef<VulkanSwapChain>();
    if (!ptr->init(this, window, backbufferSize)) {
        return nullptr;
    }

    //if (!m_mainSwapchain) {
    //    m_mainSwapchain = ptr;

    //    GLFWPlatformWindow* glfwWindow = dynamic_cast<GLFWPlatformWindow*>(window);
    //    SizeI size;
    //    glfwWindow->getSize(&size);
    //    g_app.device = m_device;
    //    g_app.initWindow(glfwWindow->glfwWindow(), size.width, size.height);
    //    g_app.initVulkan();
    //}
	return ptr;
}

Ref<IVertexDeclaration> VulkanDevice::onCreateVertexDeclaration(const VertexElement* elements, int elementsCount)
{
    auto ptr = makeRef<VulkanVertexDeclaration>();
    if (!ptr->init(elements, elementsCount)) {
        return nullptr;
    }
    return ptr;
}

Ref<IVertexBuffer> VulkanDevice::onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData)
{
    auto ptr = makeRef<VulkanVertexBuffer>();
    if (!ptr->init(this, usage, bufferSize, initialData)) {
        return nullptr;
    }
	return ptr;
}

Ref<IIndexBuffer> VulkanDevice::onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData)
{
    auto ptr = makeRef<VulkanIndexBuffer>();
    if (!ptr->init(this, usage, format, indexCount, initialData)) {
        return nullptr;
    }
	return ptr;
}

Ref<ITexture> VulkanDevice::onCreateTexture2D(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
    auto ptr = makeRef<VulkanTexture2D>();
    if (!ptr->init(this, width, height, requestFormat, mipmap, initialData)) {
        return nullptr;
    }
	return ptr;
}

Ref<ITexture> VulkanDevice::onCreateTexture3D(uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
	LN_NOTIMPLEMENTED();
	return nullptr;
}

Ref<ITexture> VulkanDevice::onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap)
{
    auto ptr = makeRef<VulkanRenderTarget>();
    if (!ptr->init(this, width, height, requestFormat, mipmap)) {
        return nullptr;
    }
    return ptr;
}

Ref<IDepthBuffer> VulkanDevice::onCreateDepthBuffer(uint32_t width, uint32_t height)
{
	auto ptr = makeRef<VulkanDepthBuffer>();
    if (!ptr->init(this, width, height)) {
        return nullptr;
    }
	return ptr;
}

Ref<ISamplerState> VulkanDevice::onCreateSamplerState(const SamplerStateData& desc)
{
	auto ptr = makeRef<VulkanSamplerState>();
	if (!ptr->init(this, desc)) {
		return nullptr;
	}
	return ptr;
}

Ref<IShaderPass> VulkanDevice::onCreateShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag)
{
    auto ptr = makeRef<VulkanShaderPass>();
    if (!ptr->init(this, createInfo, diag)) {
        return nullptr;
    }
    return ptr;
}

Result VulkanDevice::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, uint32_t* outType)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            *outType = i;
            return false;
        }
    }

    LN_LOG_ERROR << "failed to find suitable memory type!";
    return false;
}

//static std::vector<const char*> getRequiredExtensions()
//{
//    return instanceExtension;
//
//    uint32_t glfwExtensionCount = 0;
//    const char** glfwExtensions;
//    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
//
//    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
//
//    if (enableValidationLayers) {
//        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
//    }
//
//    return extensions;
//}
//
Result VulkanDevice::createInstance()
{
    if (enableValidationLayers && !VulkanHelper::checkValidationLayerSupport()) {
        LN_LOG_ERROR << "validation layers requested, but not available!";
        return false;
    }

    //uint32_t apiVersion;
    //VkResult r = vkEnumerateInstanceVersion(&apiVersion);

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

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(VulkanHelper::validationLayers.size());
        createInfo.ppEnabledLayerNames = VulkanHelper::validationLayers.data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    LN_VK_CHECK(vkCreateInstance(&createInfo, vulkanAllocator(), &m_instance));

    return true;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

Result VulkanDevice::setupDebugMessenger()
{
    if (enableValidationLayers)
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;

        LN_VK_CHECK(CreateDebugUtilsMessengerEXT(vulkanInstance(), &createInfo, vulkanAllocator(), &m_debugMessenger));
    }

	return true;
}

Result VulkanDevice::pickPhysicalDevice()
{
    // Get physical devices
    {
        uint32_t count = 0;
        auto ret = vkEnumeratePhysicalDevices(m_instance, &count, nullptr);
        if (ret != VK_SUCCESS || count < 1) {
            LN_LOG_ERROR << "Failed vkEnumeratePhysicalDevices";
            return false;
        }

        m_physicalDeviceInfos.resize(count);

        std::vector<VkPhysicalDevice> gpuDevices(count);
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
    // TODO:
    const PhysicalDeviceInfo& info = m_physicalDeviceInfos[0];
    m_physicalDevice = info.device;

    //LN_LOG_VERBOSE << VK_VERSION_MAJOR(info.deviceProperty.apiVersion);
    //LN_LOG_VERBOSE << VK_VERSION_MINOR(info.deviceProperty.apiVersion);
    //LN_LOG_VERBOSE << VK_VERSION_PATCH(info.deviceProperty.apiVersion);

    return true;
}

Result VulkanDevice::createLogicalDevice()
{
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
        std::vector<VkQueueFamilyProperties> queueFamilyPros(propCount);
        vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &propCount, queueFamilyPros.data());
        queueCreateInfos.resize(propCount);

        int queueIndex = 0;
        int totalQueueCount = 0;
        float queuePriority = 1.0f;
        for (int i = 0; i < propCount; ++i) {
            queueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfos[i].pNext = nullptr;
            queueCreateInfos[i].flags = 0;
            queueCreateInfos[i].queueCount = queueFamilyPros[i].queueCount;
            queueCreateInfos[i].queueFamilyIndex = i;

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

        uint32_t offset = 0u;
        queuePriorities.resize(totalQueueCount);
        for (uint32_t i = 0u; i < propCount; ++i) {
            queueCreateInfos[i].pQueuePriorities = &queuePriorities[offset];
            offset += queueCreateInfos[i].queueCount;
        }

    }

	//std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	//std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value()/*, indices.presentFamily.value()*/ };

	//for (uint32_t queueFamily : uniqueQueueFamilies) {
	//	VkDeviceQueueCreateInfo queueCreateInfo = {};
	//	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	//	queueCreateInfo.queueFamilyIndex = queueFamily;
	//	queueCreateInfo.queueCount = 1;
	//	queueCreateInfo.pQueuePriorities = &queuePriority;
	//	queueCreateInfos.push_back(queueCreateInfo);
	//}

	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(VulkanHelper::validationLayers.size());
		createInfo.ppEnabledLayerNames = VulkanHelper::validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(vulkanPhysicalDevice(), &createInfo, nullptr, &m_device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

    m_graphicsQueueFamilyIndex = graphicsFamilyIndex;
	vkGetDeviceQueue(m_device, graphicsFamilyIndex, graphicsQueueIndex, &m_graphicsQueue);

	return true;
}

Result VulkanDevice::createCommandPool()
{
    //QueueFamilyIndices queueFamilyIndices = findQueueFamilies(vulkanPhysicalDevice());

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = m_graphicsQueueFamilyIndex;

    // 1つのコマンドバッファを繰り返し使うようにマークする
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    LN_VK_CHECK(vkCreateCommandPool(m_device, &poolInfo, vulkanAllocator(), &m_commandPool));

    return true;
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

bool VulkanDevice::findPresentQueueFamily(VkSurfaceKHR surface, uint32_t* outIndex)
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilies.data());

    for (uint32_t i = 0; i < queueFamilies.size(); i++) {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, surface, &presentSupport);
        if (queueFamilies[i].queueCount > 0 && presentSupport) {
            *outIndex = i;
            return true;
        }
    }
    return false;
}

VkFormat VulkanDevice::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(vulkanPhysicalDevice(), format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    LN_UNREACHABLE();
    return VK_FORMAT_UNDEFINED;
}

VkFormat VulkanDevice::findDepthFormat()
{
    // Stencil 要素を含むフォーマットでないと、そもそもステンシルテスト自体が行われない (ので、Never を指定しても描画される)
	// VK_FORMAT_D24_UNORM_S8_UINT が一般的そうだけど、Radeon Vega 8 Mobile では使えなかった。
    return findSupportedFormat(
        {/*VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, */ VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D16_UNORM_S8_UINT},
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

VkCommandBuffer VulkanDevice::beginSingleTimeCommands()
{
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

    return commandBuffer;
}

Result VulkanDevice::endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    LN_VK_CHECK(vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE));
    LN_VK_CHECK(vkQueueWaitIdle(m_graphicsQueue));

    vkFreeCommandBuffers(m_device, vulkanCommandPool(), 1, &commandBuffer);

    return true;
}

void VulkanDevice::copyBufferImmediately(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferCopy copyRegion = {};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(commandBuffer);
}

void VulkanDevice::copyBufferToImageImmediately(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
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

Result VulkanDevice::transitionImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
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
    barrier.subresourceRange.levelCount = 1;
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
    //else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {
    //    barrier.srcAccessMask = 0;
    //    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    //    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    //    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    //}
    else {
        LN_LOG_ERROR << "unsupported layout transition!";
        return false;
    }


    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );


    return true;
}

Result VulkanDevice::transitionImageLayoutImmediately(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();
    Result result = transitionImageLayout(commandBuffer, image, format, oldLayout, newLayout);
    endSingleTimeCommands(commandBuffer);
    return result;
}



//==============================================================================
// VulkanGraphicsContext

VulkanGraphicsContext::VulkanGraphicsContext()
	: m_device(nullptr)
	, m_recodingCommandBuffer(nullptr)
{
}

Result VulkanGraphicsContext::init(VulkanDevice* owner)
{
	LN_CHECK(owner);
	IGraphicsContext::init(owner);
	m_device = owner;

	m_recodingCommandBuffer = makeRef<VulkanCommandBuffer>();
	if (!m_recodingCommandBuffer->init(m_device)) {
		return false;
	}

	return true;
}

void VulkanGraphicsContext::dispose()
{
	if (m_recodingCommandBuffer) {
		m_recodingCommandBuffer->dispose();
		m_recodingCommandBuffer = nullptr;
	}
}

void VulkanGraphicsContext::onBeginCommandRecoding()
{
	m_recodingCommandBuffer->beginRecording();
}

void VulkanGraphicsContext::onEndCommandRecoding()
{
	m_recodingCommandBuffer->endRecording();
}

void VulkanGraphicsContext::onUpdatePipelineState(const BlendStateDesc& blendState, const RasterizerStateDesc& rasterizerState, const DepthStencilStateDesc& depthStencilState)
{
}

void VulkanGraphicsContext::onUpdateFrameBuffers(ITexture** renderTargets, int renderTargetsCount, IDepthBuffer* depthBuffer)
{
}

void VulkanGraphicsContext::onUpdateRegionRects(const RectI& viewportRect, const RectI& scissorRect, const SizeI& targetSize)
{
}

void VulkanGraphicsContext::onUpdatePrimitiveData(IVertexDeclaration* decls, IVertexBuffer** vertexBuufers, int vertexBuffersCount, IIndexBuffer* indexBuffer)
{
}

void VulkanGraphicsContext::onUpdateShaderPass(IShaderPass* newPass)
{
}

void VulkanGraphicsContext::onSubmitStatus(const GraphicsContextState& state, uint32_t stateDirtyFlags, GraphicsContextSubmitSource submitSource)
{
}

void* VulkanGraphicsContext::onMapResource(IGraphicsResource* resource)
{
	// データ転送に使う vkCmdCopyBuffer() は RenderPass inside では使えないので、開いていればここで End しておく。次の onSubmitState() で再開される。
	m_recodingCommandBuffer->endRenderPassInRecordingIfNeeded();

	switch (resource->resourceType())
	{
	case DeviceResourceType::VertexBuffer:
	{
		VulkanVertexBuffer* vertexBuffer = static_cast<VulkanVertexBuffer*>(resource);
		vertexBuffer->m_mappedResource = recodingCommandBuffer()->cmdCopyBuffer(vertexBuffer->buffer()->size(), vertexBuffer->buffer());
		return vertexBuffer->m_mappedResource->map();
	}
	case DeviceResourceType::IndexBuffer:
	{
		VulkanIndexBuffer* indexBuffer = static_cast<VulkanIndexBuffer*>(resource);
		indexBuffer->m_mappedResource = recodingCommandBuffer()->cmdCopyBuffer(indexBuffer->buffer()->size(), indexBuffer->buffer());
		return indexBuffer->m_mappedResource->map();
	}
	default:
		LN_NOTIMPLEMENTED();
		return nullptr;
	}
}

void VulkanGraphicsContext::onUnmapResource(IGraphicsResource* resource)
{
	switch (resource->resourceType())
	{
	case DeviceResourceType::VertexBuffer:
		static_cast<VulkanVertexBuffer*>(resource)->m_mappedResource->unmap();
		break;
	case DeviceResourceType::IndexBuffer:
		static_cast<VulkanIndexBuffer*>(resource)->m_mappedResource->unmap();
		break;
	default:
		LN_NOTIMPLEMENTED();
		break;
	}
}

void VulkanGraphicsContext::onSetSubData(IGraphicsResource* resource, size_t offset, const void* data, size_t length)
{
	switch (resource->resourceType())
	{
	case DeviceResourceType::VertexBuffer:
		static_cast<VulkanVertexBuffer*>(resource)->setSubData(offset, data, length);
		break;
	case DeviceResourceType::IndexBuffer:
		static_cast<VulkanIndexBuffer*>(resource)->setSubData(offset, data, length);
		break;
	default:
		LN_NOTIMPLEMENTED();
		break;
	}
}

void VulkanGraphicsContext::onSetSubData2D(ITexture* resource, int x, int y, int width, int height, const void* data, size_t dataSize)
{
	// データ転送に使う vkCmdCopyBufferToImage() は RenderPass inside では使えないので、開いていればここで End しておく。次の onSubmitState() で再開される。
	m_recodingCommandBuffer->endRenderPassInRecordingIfNeeded();

	static_cast<VulkanTexture*>(resource)->setSubData(x, y, width, height, data, dataSize);
}

void VulkanGraphicsContext::onSetSubData3D(ITexture* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize)
{
	// データ転送に使う vkCmdCopyBufferToImage() は RenderPass inside では使えないので、開いていればここで End しておく。次の onSubmitState() で再開される。
	m_recodingCommandBuffer->endRenderPassInRecordingIfNeeded();

	static_cast<VulkanTexture*>(resource)->setSubData3D(x, y, z, width, height, depth, data, dataSize);
}

void VulkanGraphicsContext::onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
	bool skipClear;
	submitStatusInternal(GraphicsContextSubmitSource_Clear, flags, color, z, stencil, &skipClear);
	if (skipClear) return;

	const GraphicsContextState& state = stagingState();

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
		//uint32_t rtCount = 1;

		if (testFlag(flags, ClearFlags::Color))
		{
			float frgba[4] = { color.r, color.g, color.b, color.a, };

			//for (uint32_t ii = 0; ii < state.framebufferState.renderTargets.size(); ++ii)
            for (uint32_t ii = 0; ii < 1; ++ii)
			{
				if (state.framebufferState.renderTargets[ii])
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
		}

		if ((testFlag(flags, ClearFlags::Depth) || testFlag(flags, ClearFlags::Stencil)) &&
			state.framebufferState.depthBuffer != nullptr)
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
			m_recodingCommandBuffer->vulkanCommandBuffer()
			, count
			, attachments
			, LN_ARRAY_SIZE_OF(rect)
			, rect
		);

	}
}

void VulkanGraphicsContext::onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount)
{
	submitStatusInternal(GraphicsContextSubmitSource_Draw, ClearFlags::None, Color::White, 0, 0, nullptr);
	vkCmdDraw(m_recodingCommandBuffer->vulkanCommandBuffer(), VulkanHelper::getPrimitiveVertexCount(primitive, primitiveCount), 1, startVertex, 0);
	m_recodingCommandBuffer->m_priorToAnyDrawCmds = false;
}

void VulkanGraphicsContext::onDrawPrimitiveIndexed(PrimitiveTopology primitive, int startIndex, int primitiveCount)
{
	submitStatusInternal(GraphicsContextSubmitSource_Draw, ClearFlags::None, Color::White, 0, 0, nullptr);
	vkCmdDrawIndexed(m_recodingCommandBuffer->vulkanCommandBuffer(), VulkanHelper::getPrimitiveVertexCount(primitive, primitiveCount), 1, startIndex, 0, 0);
	m_recodingCommandBuffer->m_priorToAnyDrawCmds = false;
}

// TODO: もし複数 swapchain へのレンダリングを1つの CommandBuffer でやる場合、flush 時には描画するすべての swapchain の image 準備を待たなければならない。
// CommandBuffer 単位で、setRenderTarget された SwapChain の RenderTarget をすべて覚えておく仕組みが必要だろう。
void VulkanGraphicsContext::onFlushCommandBuffer(ITexture* affectRendreTarget)
{
	auto* t = static_cast<VulkanRenderTarget*>(affectRendreTarget);
	recodingCommandBuffer()->submit(
		t->imageAvailableSemaphore(),
		t->renderFinishedSemaphore());

	// [GeForce GTX 1060] 既にシグナル状態のセマフォを vkQueueSubmit で待つように指定すると、vkQueueSubmit が VK_ERROR_DEVICE_LOST を返す。
	// 通常は vkAcquireNextImageKHR と vkQueueSubmit が交互に実行されるので問題ないが、
	// RenderTarget の Read で一度 CommnadBuffer を vkQueueSubmit し、続けて CommnadBuffer の記録を再開 → vkQueueSubmit したときに問題になる。
	// vkQueueSubmit で待ちたいのは vkAcquireNextImageKHR で準備開始された RenderTarget が本当に準備終わったかどうかなので、一度待てば十分。
	// ということで、一度でも submit したら、↓ は null をセットして、次回の submit では何も待たないようにしておく。
	t->setImageAvailableSemaphoreRef(nullptr);
}

void VulkanGraphicsContext::onPresent(ISwapChain* swapChain)
{
	static_cast<VulkanSwapChain*>(swapChain)->present();

	// TODO: あったほうがいい？
	//vkDeviceWaitIdle(m_device);
	//g_app.mainLoop();
}

Result VulkanGraphicsContext::submitStatusInternal(GraphicsContextSubmitSource submitSource, ClearFlags flags, const Color& color, float z, uint8_t stencil, bool* outSkipClear)
{
	const GraphicsContextState& state = stagingState();
	uint32_t stateDirtyFlags = stagingStateDirtyFlags();

	bool clearBuffersOnBeginRenderPass = false;//(submitSource == SubmitSource_Clear && m_recodingCommandBuffer->m_priorToAnyDrawCmds);
	if (outSkipClear) *outSkipClear = clearBuffersOnBeginRenderPass;

	//m_recodingCommandBuffer->beginRecording();

	//if (!m_recodingCommandBuffer->m_lastFoundFramebuffer)
	if (stateDirtyFlags & GraphicsContextStateDirtyFlags_FrameBuffers)
	{
		// 前回開始した RenderPass があればクローズしておく
		m_recodingCommandBuffer->endRenderPassInRecordingIfNeeded();

		m_recodingCommandBuffer->m_lastFoundFramebuffer = m_device->framebufferCache()->findOrCreate(state.framebufferState);

	}

	// ↑の Framebuffer 変更や、mapResource などで RenderPass が End されていることがあるので、その場合はここで開始
	if (!m_recodingCommandBuffer->m_insideRendarPass)
	{
		VulkanFramebuffer* framebuffer = m_device->framebufferCache()->findOrCreate(state.framebufferState);
		m_recodingCommandBuffer->m_lastFoundFramebuffer = framebuffer;
		{
			VkRenderPassBeginInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = framebuffer->vulkanRenderPass();//renderPass;
			renderPassInfo.framebuffer = framebuffer->vulkanFramebuffer();
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent.width = state.framebufferState.renderTargets[0]->realSize().width; //m_mainSwapchain->vulkanSwapchainExtent();
			renderPassInfo.renderArea.extent.height = state.framebufferState.renderTargets[0]->realSize().height;

			if (clearBuffersOnBeginRenderPass) {
				VkClearValue clearValues[MaxMultiRenderTargets + 1] = {};
				uint32_t count = 0;

				if (testFlag(flags, ClearFlags::Color))
				{
					float frgba[4] = { color.r, color.g, color.b, color.a, };

					for (uint32_t ii = 0; ii < state.framebufferState.renderTargets.size(); ii++) {
						if (state.framebufferState.renderTargets[ii]) {
							clearValues[count].color = { color.r, color.g, color.b, color.a };
							count++;
						}
					}
				}

				if ((testFlag(flags, ClearFlags::Depth) || testFlag(flags, ClearFlags::Stencil)) &&
					state.framebufferState.depthBuffer != nullptr) {
					clearValues[count].depthStencil = { z, stencil };
					count++;
				}

				renderPassInfo.clearValueCount = count;
				renderPassInfo.pClearValues = clearValues;

				vkCmdBeginRenderPass(m_recodingCommandBuffer->vulkanCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			}
			else {
				renderPassInfo.clearValueCount = 0;
				renderPassInfo.pClearValues = nullptr;

				vkCmdBeginRenderPass(m_recodingCommandBuffer->vulkanCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			}

			m_recodingCommandBuffer->m_insideRendarPass = true;
		}
	}

	// TODO: modify チェック (以下の CmdSet や Bind は inside RenderPass である必要はない)
	{
		VkViewport viewport;
		viewport.x = state.regionRects.viewportRect.x;
		viewport.y = state.regionRects.viewportRect.height + state.regionRects.viewportRect.y;
		viewport.width = state.regionRects.viewportRect.width;
		viewport.height = -state.regionRects.viewportRect.height;   // height マイナスで、DirectX や OpenGL と同じ座標系になる
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(m_recodingCommandBuffer->vulkanCommandBuffer(), 0, 1, &viewport);

		VkRect2D scissor;
		scissor.offset.x = state.regionRects.scissorRect.x;
		scissor.offset.y = state.regionRects.scissorRect.y;
		scissor.extent.width = state.regionRects.scissorRect.width;
		scissor.extent.height = state.regionRects.scissorRect.height;
		vkCmdSetScissor(m_recodingCommandBuffer->vulkanCommandBuffer(), 0, 1, &scissor);
	}

	if (submitSource == GraphicsContextSubmitSource_Draw) {

		//IGraphicsDevice::State state;
		//state.pipelineState.shaderPass = m_shaderPass;
		//state.pipelineState.vertexDeclaration = m_vertexDeclaration;
		//state.framebufferState.renderTargets[0] = m_deviceContext->m_mainSwapchain->swapchainRenderTargets()[0];
		//state.framebufferState.depthBuffer = m_depthImage;
		//state.viewportRect.width = m_deviceContext->m_mainSwapchain->vulkanSwapchainExtent().width;
		//state.viewportRect.height = m_deviceContext->m_mainSwapchain->vulkanSwapchainExtent().height;
		VulkanPipeline* graphicsPipeline = m_device->pipelineCache()->findOrCreate(state, m_recodingCommandBuffer->m_lastFoundFramebuffer->vulkanRenderPass());


		vkCmdBindPipeline(m_recodingCommandBuffer->vulkanCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->vulkanPipeline());//graphicsPipeline);


		{
			std::array<VkBuffer, MaxVertexStreams> vertexBuffers;
			int vbCount = 0;
			for (int i = 0; i < state.primitive.vertexBuffers.size(); i++) {
				if (state.primitive.vertexBuffers[i]) {
					auto* vertexBuffer = static_cast<VulkanVertexBuffer*>(state.primitive.vertexBuffers[i]);
					VkBuffer buffer = vertexBuffer->vulkanBuffer();//[] = { vertexBuffer->vulkanBuffer() };
					VkDeviceSize offset = 0;//[] = { 0 };
					vkCmdBindVertexBuffers(m_recodingCommandBuffer->vulkanCommandBuffer(), i, 1, &buffer, &offset);
				}
				//else {
				//    VkBuffer buffer = VK_NULL_HANDLE;
				//    VkDeviceSize offset = 0;
				//    vkCmdBindVertexBuffers(m_recodingCommandBuffer->vulkanCommandBuffer(), i, 0, &buffer, &offset);
				//}
			}

			auto* indexBuffer = static_cast<VulkanIndexBuffer*>(state.primitive.indexBuffer);


			if (indexBuffer) {
				vkCmdBindIndexBuffer(m_recodingCommandBuffer->vulkanCommandBuffer(), indexBuffer->vulkanBuffer(), 0, indexBuffer->indexType());
			}
		}

		{
			auto* shaderPass = static_cast<VulkanShaderPass*>(state.shaderPass);

			// UniformBuffer は copy コマンドを使って更新できる。
			// TODO: ただし、texture や sampler は vkUpdateDescriptorSets でしか更新できないのでこれもキャッシュしたりする仕組みがほしいところ。
			//VulkanBuffer* buffer = m_recodingCommandBuffer->cmdCopyBuffer(sizeof(ubo), &m_uniformBuffer);
			//VulkanShaderUniformBuffer* uniformBuffer = static_cast<VulkanShaderUniformBuffer*>(shaderPass->getUniformBuffer(0));
			//uniformBuffer->setData(&ubo, sizeof(ubo));


			std::array<VkDescriptorSet, DescriptorType_Count> sets;
			m_recodingCommandBuffer->allocateDescriptorSets(shaderPass, &sets);
			vkCmdBindDescriptorSets(m_recodingCommandBuffer->vulkanCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, shaderPass->vulkanPipelineLayout(), 0, sets.size(), sets.data(), 0, nullptr);

		}

#if 0
		//
		// test
		//vertices[0].pos.x = 0;
		//vertices[0].pos.y = 0;
		//VulkanBuffer* buffer = commandBuffer->cmdCopyBuffer(sizeof(vertices[0]) * vertices.size(), m_vertexBuffer->buffer());
		//buffer->setData(0, vertices.data(), sizeof(vertices[0]) * vertices.size());

	//#if 1
	//        m_uniformBuffers[imageIndex]->setData(0, &ubo, sizeof(ubo));
	//#else
	//        // CombindSamper ではなく、個別設定のレイアウトを使っていると、実行中キューと記録中キューの間で共有できないようだ。
	//        // SubmitQueue が失敗する。
	//        VulkanBuffer* buffer = commandBuffer->cmdCopyBuffer(sizeof(ubo), m_uniformBuffers[imageIndex].get());//&m_uniformBuffer);
	//        buffer->setData(0, &ubo, sizeof(ubo));
	//#endif

		vkCmdDrawIndexed(commandBuffer->vulkanCommandBuffer(), static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

		// test
		//vertices[0].pos.x = 0;
		//vertices[0].pos.y = 0;
		//m_vertexBuffer->setSubData(0, vertices.data(), sizeof(vertices[0]) * vertices.size());
#endif


	//
	}

	return true;
}

//Result VulkanDevice::submitStatus(const State& state)
//{
//
//}

//==============================================================================
// VulkanSwapChain

VulkanSwapChain::VulkanSwapChain()
{
}

Result VulkanSwapChain::init(VulkanDevice* deviceContext, PlatformWindow* window, const SizeI& backbufferSize)
{
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
    createInfo.surface = m_surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;	// readData できるようにするため、VK_IMAGE_USAGE_TRANSFER_SRC_BIT も指定しておく

    ///QueueFamilyIndices indices = m_deviceContext->findQueueFamilies(m_deviceContext->vulkanPhysicalDevice());
    uint32_t queueFamilyIndices[] = { m_deviceContext->graphicsQueueFamilyIndex(), presentQueueFamily };

    if (m_deviceContext->graphicsQueueFamilyIndex() != presentQueueFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;


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

    //// 初期状態ではバックバッファのレイアウトは VK_IMAGE_LAYOUT_UNDEFINED となっている。
    //// Vulkan-Tutorial では、初回の VkAttachmentDescription::initialLayout
    //for (uint32_t i = 0; i < swapChainImages.size(); i++) {
    //    if (!m_deviceContext->transitionImageLayoutImmediately(swapChainImages[i], m_swapchainImageFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)) {
    //        return false;
    //    }
    //}

    {
        m_swapChainImageViews.resize(swapChainImages.size());

        for (uint32_t i = 0; i < swapChainImages.size(); i++) {
            if (!VulkanHelper::createImageView(m_deviceContext, swapChainImages[i], m_swapchainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, &m_swapChainImageViews[i])) {
                return false;
            }
        }
    }

    m_swapchainRenderTargets.resize(swapChainImages.size());
    for (uint32_t i = 0; i < swapChainImages.size(); i++) {
        auto target = makeRef<VulkanRenderTarget>();
        target->init(m_deviceContext, m_swapchainExtent.width, m_swapchainExtent.height, m_swapchainImageFormat, swapChainImages[i], m_swapChainImageViews[i]);
        m_swapchainRenderTargets[i] = target;
    }



    //m_swapchainRenderTarget = makeRef<VulkanSwapchainRenderTargetTexture>();
    //m_swapchainRenderTarget->init(m_deviceContext);
    //m_swapchainRenderTarget->reset(swapChainExtent.width, swapChainExtent.height, swapChainImageFormat, swapChainImages, swapChainImageViews);



    m_imageAvailableSemaphores.resize(maxFrameCount());
    //m_renderFinishedSemaphores.resize(maxFrameCount());
    ////inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    ////VkFenceCreateInfo fenceInfo = {};
    ////fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    ////fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < maxFrameCount(); i++) {
        LN_VK_CHECK(vkCreateSemaphore(device, &semaphoreInfo, m_deviceContext->vulkanAllocator(), &m_imageAvailableSemaphores[i]));
        //LN_VK_CHECK(vkCreateSemaphore(device, &semaphoreInfo, m_deviceContext->vulkanAllocator(), &m_renderFinishedSemaphores[i]));

        //if ( != VK_SUCCESS ||
        //    vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS/* ||
        //    vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS*/) {
        //    throw std::runtime_error("failed to create synchronization objects for a frame!");
        //}
    }
    m_currentFrame = 0;


    m_inFlightCommandBuffers.resize(m_swapchainRenderTargets.size());
    for (size_t i = 0; i < m_inFlightCommandBuffers.size(); i++) {
        m_inFlightCommandBuffers[i] = makeRef<VulkanCommandBuffer>();
        if (!m_inFlightCommandBuffers[i]->init(m_deviceContext)) {
            return false;
        }
    }

	//m_colorBuffer = makeRef<VulkanRenderTarget>();
	return true;
}

void VulkanSwapChain::dispose()
{
    VkDevice device = m_deviceContext->vulkanDevice();

    cleanup();

    for (auto c : m_inFlightCommandBuffers) {
        if (c) {
            c->dispose();
        }
    }
    m_inFlightCommandBuffers.clear();

    for (auto& x : m_imageAvailableSemaphores) {
        vkDestroySemaphore(device, x, m_deviceContext->vulkanAllocator());
    }
    m_imageAvailableSemaphores.clear();

    //for (auto& x : m_renderFinishedSemaphores) {
    //    vkDestroySemaphore(device, x, m_deviceContext->vulkanAllocator());
    //}
    //m_renderFinishedSemaphores.clear();

    for (auto& x : m_swapchainRenderTargets) {
        x->dispose();
    }
    m_swapchainRenderTargets.clear();

    if (m_swapchain) {
        vkDestroySwapchainKHR(device, m_swapchain, m_deviceContext->vulkanAllocator());
        m_swapchain = VK_NULL_HANDLE;
    }

    if (m_surface) {
        vkDestroySurfaceKHR(m_deviceContext->vulkanInstance(), m_surface, m_deviceContext->vulkanAllocator());
        m_surface = VK_NULL_HANDLE;
    }

    ISwapChain::dispose();
}

void VulkanSwapChain::cleanup()
{
    for (auto& iv : m_swapChainImageViews) {
        vkDestroyImageView(m_deviceContext->vulkanDevice(), iv, m_deviceContext->vulkanAllocator());
    }
    m_swapChainImageViews.clear();
}

void VulkanSwapChain::acquireNextImage(int* outIndex)
{
    //static bool tt = false;

    //if (tt) return;
    //tt = true;

    VkResult result = vkAcquireNextImageKHR(
        m_deviceContext->vulkanDevice(), vulkanSwapchain(),
        std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &m_imageIndex);

    // 次に present や readData など、この RenderTarget への書き込みコマンドを実行するとき、これを待たなければならない
    m_swapchainRenderTargets[m_imageIndex]->setImageAvailableSemaphoreRef(&m_imageAvailableSemaphores[m_currentFrame]);

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

ITexture* VulkanSwapChain::getRenderTarget(int imageIndex) const
{
	return m_swapchainRenderTargets[imageIndex];
}

Result VulkanSwapChain::resizeBackbuffer(uint32_t width, uint32_t height)
{
	LN_NOTIMPLEMENTED();
	return true;
}

void VulkanSwapChain::present()
{
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

    // Swap command buffer
    {
        auto t = m_deviceContext->graphicsContext()->recodingCommandBuffer();
        m_deviceContext->graphicsContext()->setRecodingCommandBuffer(m_inFlightCommandBuffers[m_currentFrame]);
        m_inFlightCommandBuffers[m_currentFrame] = t;
    }

    m_currentFrame = (m_currentFrame + 1) % maxFrameCount();


    // TODO: 必要？
    //vkDeviceWaitIdle(m_deviceContext->vulkanDevice());
}

VkSurfaceFormatKHR VulkanSwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
        return{ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
    }

    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR VulkanSwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
{
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

VkExtent2D VulkanSwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t requiredWidth, uint32_t requiredHeight)
{
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

SwapChainSupportDetails VulkanSwapChain::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
{
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

    return details;
}

//==============================================================================
// VulkanVertexDeclaration

VulkanVertexDeclaration::VulkanVertexDeclaration()
{
}

// https://gist.github.com/SaschaWillems/428d15ed4b5d71ead462bc63adffa93a
Result VulkanVertexDeclaration::init(const VertexElement* elements, int elementsCount)
{
    LN_DCHECK(elements);

    // 事前に binding がどれだけあるのか調べる
    m_maxStreamCount = 0;
    for (int i = 0; i < elementsCount; i++) {
        m_maxStreamCount = std::max(m_maxStreamCount, elements[i].StreamIndex);
        m_elements.push_back(elements[i]);
    }
    m_maxStreamCount++;
    m_bindings.resize(m_maxStreamCount);

    for (int i = 0; i < m_maxStreamCount; i++)
    {
        m_bindings[i].binding = i;  // VkVertexInputAttributeDescription が参照する binding 番号。ひとまず連番
        m_bindings[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    }

    // 実際に値を計算する
    for (int i = 0; i < elementsCount; i++) {
        auto& element = m_elements[i];

        AttributeDescriptionSource attr;
        attr.binding = elements[i].StreamIndex;
        attr.format = VulkanHelper::LNVertexElementTypeToVkFormat(elements[i].Type);
        attr.offset = m_bindings[attr.binding].stride;
        m_attributeSources.push_back(attr);

        m_bindings[element.StreamIndex].stride += GraphicsHelper::getVertexElementTypeSize(elements[i].Type);
    }

    m_hash = computeHash(m_elements);

    return true;
}

void VulkanVertexDeclaration::dispose()
{
    IVertexDeclaration::dispose();
}

uint64_t VulkanVertexDeclaration::computeHash(const std::vector<VertexElement>& elements)
{
    MixHash hash;
    hash.add(elements.size());
    for (auto& e : elements) {
        hash.add(e.StreamIndex);
        hash.add(e.Type);
        hash.add(e.Usage);
        hash.add(e.UsageIndex);
    }
    return hash.value();
}

//==============================================================================
// VulkanVertexBuffer

VulkanVertexBuffer::VulkanVertexBuffer()
    : m_buffer()
    , m_usage(GraphicsResourceUsage::Static)
{
}

Result VulkanVertexBuffer::init(VulkanDevice* deviceContext, GraphicsResourceUsage usage, size_t bufferSize, const void* initialData)
{
    LN_DCHECK(deviceContext);
    m_deviceContext = deviceContext;

    m_usage = usage;

#if 1   // TODO: Dynamic という特別な状態を持たせる必要があるか、まだわからない。
        // VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT で十分なのか、VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT を使った方がいいのか、
        // 実際にパフォーマンス測定できる段になったら改めて調査する。
    if (!m_buffer.init(deviceContext, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, nullptr)) {
        return false;
    }

    if (initialData)
    {
        VulkanBuffer stagingBuffer;
        if (!stagingBuffer.init(m_buffer.deviceContext(), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, nullptr)) {
            return false;
        }
        stagingBuffer.setData(0, initialData, bufferSize);
        m_buffer.deviceContext()->copyBufferImmediately(stagingBuffer.vulkanBuffer(), vulkanBuffer(), bufferSize);
        stagingBuffer.dispose();
    }
#else
    VkMemoryPropertyFlags properties = 0;
    if (usage == GraphicsResourceUsage::Dynamic) {
        properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    }
    else {
        properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    }

    if (!m_buffer.init(deviceContext, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, properties)) {
        return false;
    }

    if (initialData)
    {
        if (usage == GraphicsResourceUsage::Dynamic) {
            m_buffer.setData(0, initialData, bufferSize);
        }
        else {
            VulkanBuffer stagingBuffer;
            if (!stagingBuffer.init(m_buffer.deviceContext(), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
                return false;
            }
            stagingBuffer.setData(0, initialData, bufferSize);
            m_buffer.deviceContext()->copyBufferImmediately(stagingBuffer.vulkanBuffer(), vulkanBuffer(), bufferSize);
            stagingBuffer.dispose();
        }
    }
#endif

    return true;
}

void VulkanVertexBuffer::dispose()
{
    m_buffer.dispose();
    IVertexBuffer::dispose();
}

// TODO: これは廃止して、CommandList 側に持って行った方がいいと思う。
void VulkanVertexBuffer::setSubData(size_t offset, const void* data, size_t length)
{
    // static/dynamic にかかわらず、コマンド経由で転送しなければ整合性が取れなくなる
    VulkanBuffer* buffer = m_deviceContext->graphicsContext()->recodingCommandBuffer()->cmdCopyBuffer(length, &m_buffer);
    buffer->setData(offset, data, length);
}

//==============================================================================
// VulkanIndexBuffer

VulkanIndexBuffer::VulkanIndexBuffer()
    : m_buffer()
    , m_usage(GraphicsResourceUsage::Static)
{
}

Result VulkanIndexBuffer::init(VulkanDevice* deviceContext, GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData)
{
    LN_DCHECK(deviceContext);
    m_deviceContext = deviceContext;

    m_usage = usage;
    int stride = 0;
    if (format == IndexBufferFormat::UInt16) {
        m_indexType = VK_INDEX_TYPE_UINT16;
        stride = 2;
    }
    else {
        m_indexType = VK_INDEX_TYPE_UINT32;
        stride = 4;
    }
    size_t bufferSize = stride * indexCount;

    if (!m_buffer.init(deviceContext, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, nullptr)) {
        return false;
    }

    if (initialData)
    {
        VulkanBuffer stagingBuffer;
        if (!stagingBuffer.init(m_buffer.deviceContext(), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, nullptr)) {
            return false;
        }
        stagingBuffer.setData(0, initialData, bufferSize);
        m_buffer.deviceContext()->copyBufferImmediately(stagingBuffer.vulkanBuffer(), vulkanBuffer(), bufferSize);
        stagingBuffer.dispose();
    }

    return true;
}

void VulkanIndexBuffer::dispose()
{
    m_buffer.dispose();
    IIndexBuffer::dispose();
}

// TODO: これは廃止して、CommandList 側に持って行った方がいいと思う。
void VulkanIndexBuffer::setSubData(size_t offset, const void* data, size_t length)
{
    // static/dynamic にかかわらず、コマンド経由で転送しなければ整合性が取れなくなる
    VulkanBuffer* buffer = m_deviceContext->graphicsContext()->recodingCommandBuffer()->cmdCopyBuffer(length, &m_buffer);
    buffer->setData(offset, data, length);
}

//==============================================================================
// VulkanTexture2D

VulkanTexture2D::VulkanTexture2D()
{
}

Result VulkanTexture2D::init(VulkanDevice* deviceContext, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
	LN_DCHECK(deviceContext);
	m_deviceContext = deviceContext;
    m_size.width = width;
    m_size.height = height;
    m_format = requestFormat;

    m_nativeFormat = VulkanHelper::LNFormatToVkFormat(requestFormat);
    VkDeviceSize imageSize = width * height * GraphicsHelper::getPixelSize(requestFormat);

    if (!initialData) {
        LN_NOTIMPLEMENTED();
    }
    else {
        VulkanBuffer stagingBuffer;
        if (!stagingBuffer.init(m_deviceContext, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, nullptr)) {
            return false;
        }
        stagingBuffer.setData(0, initialData, imageSize);

        // VK_FORMAT_R8G8B8A8_UNORM
        m_image.init(m_deviceContext, width, height, m_nativeFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT);

        if (!m_deviceContext->transitionImageLayoutImmediately(m_image.vulkanImage(), m_nativeFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)) {
            return false;
        }

        m_deviceContext->copyBufferToImageImmediately(stagingBuffer.vulkanBuffer(), m_image.vulkanImage(), width, height);

        if (!m_deviceContext->transitionImageLayoutImmediately(m_image.vulkanImage(), m_nativeFormat, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)) {
            return false;
        }

        stagingBuffer.dispose();
    }

	return true;
}

void VulkanTexture2D::dispose()
{
    m_image.dispose();
    VulkanTexture::dispose();
}

void VulkanTexture2D::setSubData(int x, int y, int width, int height, const void* data, size_t dataSize)
{
    // TODO:
    assert(x == 0);
    assert(y == 0);


    // vkCmdCopyBufferToImage() の dstImageLayout は VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR の
    // いずれかでなければならない。https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/vkCmdCopyBufferToImage.html
    // 転送前にレイアウトを変更しておく。
    if (!m_deviceContext->transitionImageLayout(m_deviceContext->graphicsContext()->recodingCommandBuffer()->vulkanCommandBuffer(),
        m_image.vulkanImage(), m_nativeFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)) {
        LN_ERROR();
        return;
    }

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
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height),
        1
    };
    VulkanBuffer* buffer = m_deviceContext->graphicsContext()->recodingCommandBuffer()->cmdCopyBufferToImage(dataSize, region, &m_image);
    buffer->setData(0, data, dataSize);

    // レイアウトを元に戻す
    if (!m_deviceContext->transitionImageLayout(m_deviceContext->graphicsContext()->recodingCommandBuffer()->vulkanCommandBuffer(),
        m_image.vulkanImage(), m_nativeFormat, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)) {
        LN_ERROR();
        return;
    }
}

//==============================================================================
// VulkanSwapchainRenderTargetTexture

VulkanRenderTarget::VulkanRenderTarget()
    : m_deviceContext(nullptr)
    , m_imageAvailableSemaphoreRef(nullptr)
{
}

Result VulkanRenderTarget::init(VulkanDevice* deviceContext, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap)
{
    LN_DCHECK(deviceContext);
    m_deviceContext = deviceContext;
    m_size.width = width;
    m_size.height = height;
    m_format = requestFormat;

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    LN_VK_CHECK(vkCreateSemaphore(m_deviceContext->vulkanDevice(), &semaphoreInfo, m_deviceContext->vulkanAllocator(), &m_renderFinishedSemaphore));

    {
        VkFormat nativeFormat = VulkanHelper::LNFormatToVkFormat(requestFormat);
        VkDeviceSize imageSize = width * height * GraphicsHelper::getPixelSize(requestFormat);

        m_image = std::make_unique<VulkanImage>();
        if (!m_image->init(m_deviceContext, width, height, nativeFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT)) {
            return false;
        }

        if (!m_deviceContext->transitionImageLayoutImmediately(m_image->vulkanImage(), nativeFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)) {
            return false;
        }

        //// Color attachment
        //VkImageCreateInfo image = vks::initializers::imageCreateInfo();
        //image.imageType = VK_IMAGE_TYPE_2D;
        //image.format = FB_COLOR_FORMAT;
        //image.extent.width = offscreenPass.width;
        //image.extent.height = offscreenPass.height;
        //image.extent.depth = 1;
        //image.mipLevels = 1;
        //image.arrayLayers = 1;
        //image.samples = VK_SAMPLE_COUNT_1_BIT;
        //image.tiling = VK_IMAGE_TILING_OPTIMAL;
        //// We will sample directly from the color attachment
        //image.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

        //VkMemoryAllocateInfo memAlloc = vks::initializers::memoryAllocateInfo();
        //VkMemoryRequirements memReqs;

        //VK_CHECK_RESULT(vkCreateImage(device, &image, nullptr, &offscreenPass.color.image));
        //vkGetImageMemoryRequirements(device, offscreenPass.color.image, &memReqs);
        //memAlloc.allocationSize = memReqs.size;
        //memAlloc.memoryTypeIndex = vulkanDevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        //VK_CHECK_RESULT(vkAllocateMemory(device, &memAlloc, nullptr, &offscreenPass.color.mem));
        //VK_CHECK_RESULT(vkBindImageMemory(device, offscreenPass.color.image, offscreenPass.color.mem, 0));

        //VkImageViewCreateInfo colorImageView = vks::initializers::imageViewCreateInfo();
        //colorImageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
        //colorImageView.format = FB_COLOR_FORMAT;
        //colorImageView.subresourceRange = {};
        //colorImageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        //colorImageView.subresourceRange.baseMipLevel = 0;
        //colorImageView.subresourceRange.levelCount = 1;
        //colorImageView.subresourceRange.baseArrayLayer = 0;
        //colorImageView.subresourceRange.layerCount = 1;
        //colorImageView.image = offscreenPass.color.image;
        //VK_CHECK_RESULT(vkCreateImageView(device, &colorImageView, nullptr, &offscreenPass.color.view));
    }

    return true;
}

Result VulkanRenderTarget::init(VulkanDevice* deviceContext, uint32_t width, uint32_t height, VkFormat format, VkImage image, VkImageView imageView)
{
	LN_DCHECK(deviceContext);
	m_deviceContext = deviceContext;

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    LN_VK_CHECK(vkCreateSemaphore(m_deviceContext->vulkanDevice(), &semaphoreInfo, m_deviceContext->vulkanAllocator(), &m_renderFinishedSemaphore));

	return reset(width, height, format, image, imageView);
}

void VulkanRenderTarget::dispose()
{
    if (m_image) {
        m_image->dispose();
        m_image = nullptr;
    }

    if (m_deviceContext) {
        m_deviceContext->framebufferCache()->invalidateRenderTarget(this);

        //if (m_imageAvailableSemaphore) {
        //    vkDestroySemaphore(m_deviceContext->vulkanDevice(), m_imageAvailableSemaphore, m_deviceContext->vulkanAllocator());
        //    m_imageAvailableSemaphore = VK_NULL_HANDLE;
        //}

        if (m_renderFinishedSemaphore) {
            vkDestroySemaphore(m_deviceContext->vulkanDevice(), m_renderFinishedSemaphore, m_deviceContext->vulkanAllocator());
            m_renderFinishedSemaphore = VK_NULL_HANDLE;
        }

        m_deviceContext = nullptr;
    }
    VulkanTexture::dispose();
}

Result VulkanRenderTarget::reset(uint32_t width, uint32_t height, VkFormat format, VkImage image, VkImageView imageView)
{
	m_size.width = width;
	m_size.height = height;
	m_format = VulkanHelper::VkFormatToLNFormat(format);

	m_image = std::make_unique<VulkanImage>();
	if (!m_image->init(m_deviceContext, format, image, imageView)) {
		return false;
	}
	return true;
}

void VulkanRenderTarget::readData(void* outData)
{
    // Flush
    //m_deviceContext->recodingCommandBuffer()->submit(imageAvailableSemaphore(), renderFinishedSemaphore());

    vkDeviceWaitIdle(m_deviceContext->vulkanDevice());

	uint32_t width = m_size.width;
    uint32_t height = m_size.height;
    VkDeviceSize size = width * height * 4; // TODO

    // 転送先として作成
    VulkanBuffer destBuffer;
    if (!destBuffer.init(m_deviceContext, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, nullptr)) {
        LN_ERROR();
        goto Exit;
    }

    {
        VkCommandBuffer commandBuffer = m_deviceContext->beginSingleTimeCommands();

        // Swapchain の Backbuffer (VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) から、転送ソース (VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) へレイアウト変換
        {
            VkImageMemoryBarrier imageMemoryBarrier = {};
            imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            imageMemoryBarrier.pNext = nullptr;
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            imageMemoryBarrier.image = m_image->vulkanImage();
            imageMemoryBarrier.subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
            vkCmdPipelineBarrier(
                commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                0,
                0, nullptr,
                0, nullptr,
                1, &imageMemoryBarrier);
        }

        // destBuffer へコピー
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
            region.imageExtent = { width, height, 1 };
            vkCmdCopyImageToBuffer(
                commandBuffer,
                m_image->vulkanImage(),
                VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                destBuffer.vulkanBuffer(),
                1, &region);
        }

        // レイアウトを元に戻す
        {
            VkImageMemoryBarrier imageMemoryBarrier = {};
            imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            imageMemoryBarrier.pNext = nullptr;
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
            imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            imageMemoryBarrier.image = m_image->vulkanImage();
            imageMemoryBarrier.subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
            vkCmdPipelineBarrier(
                commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                0,
                0, nullptr,
                0, nullptr,
                1, &imageMemoryBarrier);
        }

        // Submit and Wait
        if (!m_deviceContext->endSingleTimeCommands(commandBuffer)) {
            LN_ERROR();
            goto Exit;
        }
    }

    // Blit
    {
        void* rawData = destBuffer.map();

        // TODO: まだ Bitmap クラス側が BGRA の save に対応していないのでここで変換してしまう。
        if (m_image->vulkanFormat() == VK_FORMAT_B8G8R8A8_UNORM ||
            m_image->vulkanFormat() == VK_FORMAT_R8G8B8A8_UNORM) {

            if (m_image->vulkanFormat() == VK_FORMAT_B8G8R8A8_UNORM) {
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

            // V flip
            for (uint32_t y = 0; y < height; y++)
            {
                unsigned char* sr = static_cast<unsigned char*>(rawData) + ((y)* width) * 4;
                unsigned char* dr = static_cast<unsigned char*>(outData) + ((height - y - 1)* width * 4);
                memcpy(dr, sr, width * 4);
            }
        }
        else {
            LN_NOTIMPLEMENTED();
        }

        destBuffer.unmap();
    }

Exit:
    destBuffer.dispose();
}

//==============================================================================
// VulkanSwapchainRenderTargetTexture

//VulkanSwapchainRenderTargetTexture::VulkanSwapchainRenderTargetTexture()
//{
//}
//
//Result VulkanSwapchainRenderTargetTexture::init(VulkanDevice* deviceContext)
//{
//    LN_DCHECK(deviceContext);
//    m_deviceContext = deviceContext;
//    m_currentBufferIndex = 0;
//    return true;
//}
//
//void VulkanSwapchainRenderTargetTexture::dispose()
//{
//    if (m_deviceContext) {
//        m_deviceContext->framebufferCache()->invalidateRenderTarget(this);
//        m_deviceContext = nullptr;
//    }
//
//    clear();
//    VulkanTexture::dispose();
//}
//
//Result VulkanSwapchainRenderTargetTexture::reset(uint32_t width, uint32_t height, VkFormat format, const std::vector<VkImage>& images, const std::vector<VkImageView>& imageViews)
//{
//    LN_DCHECK(images.size() == imageViews.size());
//    clear();
//
//    m_size.width = width;
//    m_size.height = height;
//    m_format = VulkanHelper::VkFormatToLNFormat(format);
//
//    m_images.resize(images.size());
//    for (int i = 0; i < images.size(); i++) {
//        m_images[i] = std::make_shared<VulkanImage>();
//        if (!m_images[i]->init(m_deviceContext, format, images[i], imageViews[i])) {
//            return false;
//        }
//    }
//
//    return true;
//}
//
//void VulkanSwapchainRenderTargetTexture::clear()
//{
//    for (auto& image : m_images) {
//        image->dispose();
//    }
//    m_images.clear();
//}

//==============================================================================
// VulkanDepthBuffer

VulkanDepthBuffer::VulkanDepthBuffer()
{
}

Result VulkanDepthBuffer::init(VulkanDevice* deviceContext, uint32_t width, uint32_t height)
{
    LN_DCHECK(deviceContext);
    if (LN_REQUIRE(width > 0)) return false;
    if (LN_REQUIRE(height > 0)) return false;
    m_deviceContext = deviceContext;

    VkFormat depthFormat = m_deviceContext->findDepthFormat();

    if (!m_image.init(m_deviceContext, width, height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_DEPTH_BIT)) {
        return false;
    }

    if (!m_deviceContext->transitionImageLayoutImmediately(m_image.vulkanImage(), depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)) {
        return false;
    }

    return true;
}

void VulkanDepthBuffer::dispose()
{
    if (m_deviceContext) {
        m_deviceContext->framebufferCache()->invalidateDepthBuffer(this);
        m_deviceContext = nullptr;
    }

    m_image.dispose();
    IDepthBuffer::dispose();
}

//==============================================================================
// VulkanSamplerState

VulkanSamplerState::VulkanSamplerState()
	: m_deviceContext(nullptr)
	, m_sampler(VK_NULL_HANDLE)
{
}

Result VulkanSamplerState::init(VulkanDevice* deviceContext, const SamplerStateData& desc)
{
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
    samplerInfo.anisotropyEnable = VK_FALSE;//VK_TRUE;		// TODO:
    samplerInfo.maxAnisotropy = 0;// 16;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;

	LN_VK_CHECK(vkCreateSampler(m_deviceContext->vulkanDevice(), &samplerInfo, m_deviceContext->vulkanAllocator(), &m_sampler));

	return true;
}

void VulkanSamplerState::dispose()
{
	if (m_sampler) {
		vkDestroySampler(m_deviceContext->vulkanDevice(), m_sampler, m_deviceContext->vulkanAllocator());
		m_sampler = VK_NULL_HANDLE;
	}

	m_deviceContext = nullptr;
	ISamplerState::dispose();
}

//==============================================================================
// VulkanShaderPass

VulkanShaderPass::VulkanShaderPass()
{
}

Result VulkanShaderPass::init(VulkanDevice* deviceContext, const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag)
{
    LN_DCHECK(deviceContext);
    m_deviceContext = deviceContext;

    VkDevice device = m_deviceContext->vulkanDevice();

    // vert
    {
        m_vertEntryPointName = createInfo.vsEntryPointName;

        VkShaderModuleCreateInfo shaderCreateInfo = {};
        shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderCreateInfo.codeSize = createInfo.vsCodeLen;
        shaderCreateInfo.pCode = reinterpret_cast<const uint32_t*>(createInfo.vsCode);

        LN_VK_CHECK(vkCreateShaderModule(device, &shaderCreateInfo, m_deviceContext->vulkanAllocator(), &m_vertShaderModule));
    }

    // frag
    {
        m_fragEntryPointName = createInfo.psEntryPointName;

        VkShaderModuleCreateInfo shaderCreateInfo = {};
        shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderCreateInfo.codeSize = createInfo.psCodeLen;
        shaderCreateInfo.pCode = reinterpret_cast<const uint32_t*>(createInfo.psCode);

        LN_VK_CHECK(vkCreateShaderModule(device, &shaderCreateInfo, m_deviceContext->vulkanAllocator(), &m_fragShaderModule));
    }

	// SamplerBuffer
	m_localShaderSamplerBuffer = makeRef<VulkanLocalShaderSamplerBuffer>();
	if (!m_localShaderSamplerBuffer->init()) {
		return false;
	}

    // DescriptorSetLayout
    {
        // https://docs.microsoft.com/ja-jp/windows/desktop/direct3dhlsl/dx-graphics-hlsl-variable-register

        // 'b' register in HLSL
        {
            std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
            layoutBindings.reserve(createInfo.descriptorLayout->uniformBufferRegister.size());
            m_bufferDescriptorBufferInfo.reserve(createInfo.descriptorLayout->uniformBufferRegister.size());
            for (auto& item : createInfo.descriptorLayout->uniformBufferRegister) {
                VkDescriptorSetLayoutBinding layoutBinding = {};
                layoutBinding.binding = item.binding;
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                layoutBinding.descriptorCount = 1;
                layoutBinding.stageFlags |= (createInfo.descriptorLayout->isReferenceFromVertexStage(DescriptorType_UniformBuffer)) ? VK_SHADER_STAGE_VERTEX_BIT : 0;
                layoutBinding.stageFlags |= (createInfo.descriptorLayout->isReferenceFromPixelStage(DescriptorType_UniformBuffer)) ? VK_SHADER_STAGE_FRAGMENT_BIT : 0;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBindings.push_back(layoutBinding);

                VkDescriptorBufferInfo info;
                info.buffer = VK_NULL_HANDLE;   // set from submitDescriptorWriteInfo
                info.offset = 0;
                info.range = item.size;
                m_bufferDescriptorBufferInfo.push_back(info);
                
                VkWriteDescriptorSet set;
                set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                set.pNext = nullptr;
                set.dstSet = VK_NULL_HANDLE;   // set from submitDescriptorWriteInfo
                set.dstBinding = item.binding;
                set.dstArrayElement = 0;
                set.descriptorCount = 1;
                set.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                set.pImageInfo = nullptr;
                set.pBufferInfo = &m_bufferDescriptorBufferInfo.back();
                set.pTexelBufferView = nullptr;
                m_descriptorWriteInfo.push_back(set);
            }

            VkDescriptorSetLayoutCreateInfo layoutInfo = {};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = layoutBindings.size();    // 0 で空のインスタンスだけ作ることは可能
            layoutInfo.pBindings = layoutBindings.data();
            LN_VK_CHECK(vkCreateDescriptorSetLayout(device, &layoutInfo, m_deviceContext->vulkanAllocator(), &m_descriptorSetLayouts[0]));
        }

        // 't' register in HLSL
        {
            std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
            layoutBindings.reserve(createInfo.descriptorLayout->textureRegister.size());
            m_textureDescripterImageInfo.reserve(createInfo.descriptorLayout->textureRegister.size());
            for (auto& item : createInfo.descriptorLayout->textureRegister) {
                m_localShaderSamplerBuffer->addDescriptor(DescriptorType_Texture, item.binding, item.name, layoutBindings.size(), m_descriptorWriteInfo.size());

                VkDescriptorSetLayoutBinding layoutBinding = {};
                layoutBinding.binding = item.binding;
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;//VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
                layoutBinding.descriptorCount = 1;
                layoutBinding.stageFlags |= (createInfo.descriptorLayout->isReferenceFromVertexStage(DescriptorType_Texture)) ? VK_SHADER_STAGE_VERTEX_BIT : 0;
                layoutBinding.stageFlags |= (createInfo.descriptorLayout->isReferenceFromPixelStage(DescriptorType_Texture)) ? VK_SHADER_STAGE_FRAGMENT_BIT : 0;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBindings.push_back(layoutBinding);

                VkDescriptorImageInfo info;
                info.sampler = VK_NULL_HANDLE;
                info.imageView = VK_NULL_HANDLE;    // set from submitDescriptorWriteInfo
                info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                m_textureDescripterImageInfo.push_back(info);

                VkWriteDescriptorSet set;
                set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                set.pNext = nullptr;
                set.dstSet = VK_NULL_HANDLE;   // set from submitDescriptorWriteInfo
                set.dstBinding = item.binding;
                set.dstArrayElement = 0;
                set.descriptorCount = 1;
                set.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;//VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                set.pImageInfo = &m_textureDescripterImageInfo.back();
                set.pBufferInfo = nullptr;
                set.pTexelBufferView = nullptr;
                m_descriptorWriteInfo.push_back(set);
            }

            VkDescriptorSetLayoutCreateInfo layoutInfo = {};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = layoutBindings.size();    // 0 で空のインスタンスだけ作ることは可能
            layoutInfo.pBindings = layoutBindings.data();
            LN_VK_CHECK(vkCreateDescriptorSetLayout(device, &layoutInfo, m_deviceContext->vulkanAllocator(), &m_descriptorSetLayouts[1]));
        }

        // 's' register in HLSL (SamplerState and CombinedSampler)
        {
            std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
            layoutBindings.reserve(createInfo.descriptorLayout->samplerRegister.size());
            m_samplerDescripterImageInfo.reserve(createInfo.descriptorLayout->samplerRegister.size());
            for (auto& item : createInfo.descriptorLayout->samplerRegister) {
                m_localShaderSamplerBuffer->addDescriptor(DescriptorType_SamplerState, item.binding, item.name, layoutBindings.size(), m_descriptorWriteInfo.size());

                VkDescriptorSetLayoutBinding layoutBinding = {};
                layoutBinding.binding = item.binding;
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER; //VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;//   // VK_DESCRIPTOR_TYPE_SAMPLER としても使える。ただし、ImageView をセットしておく必要がある。
                layoutBinding.descriptorCount = 1;
                layoutBinding.stageFlags |= (createInfo.descriptorLayout->isReferenceFromVertexStage(DescriptorType_SamplerState)) ? VK_SHADER_STAGE_VERTEX_BIT : 0;
                layoutBinding.stageFlags |= (createInfo.descriptorLayout->isReferenceFromPixelStage(DescriptorType_SamplerState)) ? VK_SHADER_STAGE_FRAGMENT_BIT : 0;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBindings.push_back(layoutBinding);

                VkDescriptorImageInfo info;
                info.sampler = VK_NULL_HANDLE;      // set from submitDescriptorWriteInfo
                info.imageView = VK_NULL_HANDLE;    // set from submitDescriptorWriteInfo
                info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                m_samplerDescripterImageInfo.push_back(info);

                VkWriteDescriptorSet set;
                set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                set.pNext = nullptr;
                set.dstSet = VK_NULL_HANDLE;   // set from submitDescriptorWriteInfo
                set.dstBinding = item.binding;
                set.dstArrayElement = 0;
                set.descriptorCount = 1;
                set.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;//VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;//VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                set.pImageInfo = &m_samplerDescripterImageInfo.back();
                set.pBufferInfo = nullptr;
                set.pTexelBufferView = nullptr;
                m_descriptorWriteInfo.push_back(set);
            }

            VkDescriptorSetLayoutCreateInfo layoutInfo = {};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = layoutBindings.size();    // 0 で空のインスタンスだけ作ることは可能
            layoutInfo.pBindings = layoutBindings.data();
            LN_VK_CHECK(vkCreateDescriptorSetLayout(device, &layoutInfo, m_deviceContext->vulkanAllocator(), &m_descriptorSetLayouts[2]));
        }
    }

    // PipelineLayout
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = m_descriptorSetLayouts.size();
        pipelineLayoutInfo.pSetLayouts = m_descriptorSetLayouts.data();
        LN_VK_CHECK(vkCreatePipelineLayout(device, &pipelineLayoutInfo, m_deviceContext->vulkanAllocator(), &m_pipelineLayout));
    }

	// UniformBuffers
	{
        uint32_t writeIndex = 0;
		for (auto& item : createInfo.descriptorLayout->uniformBufferRegister)
		{
			auto buf = makeRef<VulkanShaderUniformBuffer>();
			if (!buf->init(m_deviceContext, item.name, item.size, item.members)) {
				return false;
			}
            buf->descriptorWriteInfoIndex = writeIndex;
            buf->bindingIndex = item.binding;
            writeIndex++;

			m_uniformBuffers.push_back(buf);
		}
	}

    return true;
}

void VulkanShaderPass::dispose()
{
    if (m_deviceContext) {
        VkDevice device = m_deviceContext->vulkanDevice();

        for (auto& buf : m_uniformBuffers) {
            buf->dispose();
        }
        m_uniformBuffers.clear();
    
        if (m_localShaderSamplerBuffer) {
            m_localShaderSamplerBuffer->dispose();
            m_localShaderSamplerBuffer = nullptr;
        }

        for (auto& pool : m_descriptorSetsPools) {
            pool->dispose();
        }
        m_descriptorSetsPools.clear();

        if (m_localShaderSamplerBuffer) {
            m_localShaderSamplerBuffer->dispose();
            m_localShaderSamplerBuffer = nullptr;
        }

        if (m_pipelineLayout) {
            vkDestroyPipelineLayout(device, m_pipelineLayout, m_deviceContext->vulkanAllocator());
            m_pipelineLayout = VK_NULL_HANDLE;
        }

        for (auto& layout : m_descriptorSetLayouts) {
            if (layout) {
                vkDestroyDescriptorSetLayout(device, layout, m_deviceContext->vulkanAllocator());
                layout = VK_NULL_HANDLE;
            }
        }

        if (m_vertShaderModule) {
            vkDestroyShaderModule(device, m_vertShaderModule, m_deviceContext->vulkanAllocator());
            m_vertShaderModule = VK_NULL_HANDLE;
        }

        if (m_fragShaderModule) {
            vkDestroyShaderModule(device, m_fragShaderModule, m_deviceContext->vulkanAllocator());
            m_fragShaderModule = VK_NULL_HANDLE;
        }

        m_deviceContext->pipelineCache()->invalidateFromShaderPass(this);
        m_deviceContext = nullptr;
    }

    IShaderPass::dispose();
}

IShaderUniformBuffer* VulkanShaderPass::getUniformBuffer(int index) const
{
    return m_uniformBuffers[index];
}

IShaderSamplerBuffer* VulkanShaderPass::samplerBuffer() const
{
    return m_localShaderSamplerBuffer;
}

const std::vector<VkWriteDescriptorSet>& VulkanShaderPass::submitDescriptorWriteInfo(VulkanCommandBuffer* commandBuffer, const std::array<VkDescriptorSet, DescriptorType_Count>& descriptorSets)
{
    for (size_t i = 0; i < m_uniformBuffers.size(); i++) {
        auto& uniformBuffer = m_uniformBuffers[i];

        // UniformBuffer の内容を CopyCommand に乗せる。
        // Inside RenderPass では vkCmdCopyBuffer が禁止されているので、DeviceLocal に置いたメモリを使うのではなく、
        // 毎回新しい HostVisible な Buffer を作ってそれを使う。
        VulkanBuffer* buffer = commandBuffer->allocateBuffer(uniformBuffer->buffer()->size(), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        buffer->setData(0, uniformBuffer->data().data(), uniformBuffer->data().size());

        VkDescriptorBufferInfo& info = m_bufferDescriptorBufferInfo[i];
        info.buffer = info.buffer = buffer->vulkanBuffer();

        VkWriteDescriptorSet& writeInfo = m_descriptorWriteInfo[i];
        writeInfo.dstSet = descriptorSets[DescriptorType_UniformBuffer];
    }

    int count = m_localShaderSamplerBuffer->registerCount();
    for (int i = 0; i < count; i++) {
        DescriptorType type = m_localShaderSamplerBuffer->descriptorType(i);
        uint32_t imageIndex = m_localShaderSamplerBuffer->descriptorImageInfoIndex(i);
        uint32_t writeIndex = m_localShaderSamplerBuffer->descriptorWriteInfoIndex(i);
        VkWriteDescriptorSet& writeInfo = m_descriptorWriteInfo[writeIndex];

        auto& texture = m_localShaderSamplerBuffer->texture(i);
        auto& samplerState = m_localShaderSamplerBuffer->samplerState(i);

        if (type == DescriptorType_Texture) {
            VkDescriptorImageInfo& imageInfo = m_textureDescripterImageInfo[imageIndex];
            imageInfo.imageView = (texture) ? texture->image()->vulkanImageView() : 0;
            imageInfo.sampler = (samplerState) ? samplerState->vulkanSampler() : 0;
            writeInfo.dstSet = descriptorSets[DescriptorType_Texture];
        }
        else if (type == DescriptorType_SamplerState) {
            VkDescriptorImageInfo& imageInfo = m_samplerDescripterImageInfo[imageIndex];
            imageInfo.imageView = (texture) ? texture->image()->vulkanImageView() : 0;
            imageInfo.sampler = (samplerState) ? samplerState->vulkanSampler() : 0;
            writeInfo.dstSet = descriptorSets[DescriptorType_SamplerState];
        }
        else {
            LN_UNREACHABLE();
        }
    }

    return m_descriptorWriteInfo;
}

Ref<VulkanDescriptorSetsPool> VulkanShaderPass::getDescriptorSetsPool()
{
    if (m_descriptorSetsPools.empty()) {
        auto ptr = makeRef<VulkanDescriptorSetsPool>();
        if (!ptr->init(m_deviceContext, this)) {
            return nullptr;
        }
        return ptr;
    }
    else {
        auto ptr = m_descriptorSetsPools.back();
        m_descriptorSetsPools.pop_back();
        return ptr;
    }
}

void VulkanShaderPass::releaseDescriptorSetsPool(VulkanDescriptorSetsPool* pool)
{
    LN_DCHECK(pool);
    pool->reset();
    m_descriptorSetsPools.push_back(pool);
}

//==============================================================================
// VulkanShaderUniformBuffer

VulkanShaderUniformBuffer::VulkanShaderUniformBuffer()
{
}

Result VulkanShaderUniformBuffer::init(VulkanDevice* deviceContext, const std::string& name, size_t size, const std::vector<ShaderUniformInfo>& members)
{
	m_name = name;
	if (m_name == "$Global") {
		m_name = "_Global";
	}

	m_data.resize(size);

    // TRANSFER_DST に最適化
    if (!m_uniformBuffer.init(deviceContext, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, nullptr)) {
        return false;
    }

    for (auto& member : members) {

        //size_t memberByteSize = 0;
        //{
        //    uint16_t nextOffset = UINT16_MAX;
        //    for (size_t i = 0; i < members.size(); i++) {
        //        if (member.offset < members[i].offset) {
        //            nextOffset = std::min(nextOffset, members[i].offset);
        //        }
        //    }
        //    if (nextOffset == UINT16_MAX) {
        //        nextOffset = size;
        //    }
        //    memberByteSize = nextOffset - member.offset;
        //}

        auto buf = makeRef<VulkanShaderUniform>();
        if (!buf->init(member)) {
            return false;
        }
        m_members.push_back(buf);
    }

	return true;
}

void VulkanShaderUniformBuffer::dispose()
{
    for (auto& member : m_members) {
        member->dispose();
    }
    m_members.clear();

    m_uniformBuffer.dispose();
	IShaderUniformBuffer::dispose();
}

IShaderUniform* VulkanShaderUniformBuffer::getUniform(int index) const
{
    return m_members[index];
}

void VulkanShaderUniformBuffer::setData(const void* data, size_t size)
{
	LN_DCHECK(size <= m_data.size());
	memcpy(m_data.data(), data, size);
}

//=============================================================================
// VulkanShaderUniform

VulkanShaderUniform::VulkanShaderUniform()
{
}

Result VulkanShaderUniform::init(const ShaderUniformInfo& info/*, size_t memberByteSize*/)
{
    m_name = info.name;
    m_desc.type2 = (ShaderUniformType)info.type;
    m_desc.rows = info.matrixRows;
    m_desc.columns = info.matrixColumns;
    m_desc.elements = info.arrayElements;

    if (m_desc.columns == 0) { // OpenGL Dirver の動作に合わせる
        m_desc.columns = info.vectorElements;// *sizeof(float);
    }

    m_desc.offset = info.offset;
    //m_desc.size = 0;
    if (info.arrayElements > 0) {
        VulkanHelper::resolveStd140Layout(info, &m_desc.arrayStride);
    }
    m_desc.matrixStride = info.matrixColumns *sizeof(float);

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

Result VulkanLocalShaderSamplerBuffer::init(/*const DescriptorLayout* descriptorLayout*/)
{
    //uint32_t writeIndex = descriptorLayout->uniformBufferRegister.size();

    //// 't' register in HLSL
    //for (auto& item : descriptorLayout->textureRegister) {
    //    Entry e;
    //    e.textureRegisterName = item.name;
    //    e.descriptorWriteInfoIndex = writeIndex;
    //    e.bindingIndex = item.binding;
    //    m_table.push_back(e);
    //    writeIndex++;
    //}

    //// 's' register in HLSL (SamplerState and CombinedSampler)
    //for (auto& item : descriptorLayout->samplerRegister) {
    //    Entry e;
    //    e.samplerRegisterName = item.name;
    //    e.descriptorWriteInfoIndex = writeIndex;
    //    e.bindingIndex = item.binding;
    //    m_table.push_back(e);
    //    writeIndex++;
    //}

    return true;
}

void VulkanLocalShaderSamplerBuffer::dispose()
{
    IShaderSamplerBuffer::dispose();
}

void VulkanLocalShaderSamplerBuffer::setTexture(int registerIndex, ITexture* texture)
{
    m_table[registerIndex].texture = static_cast<VulkanTexture*>(texture);
}

void VulkanLocalShaderSamplerBuffer::setSamplerState(int registerIndex, ISamplerState* state)
{
    m_table[registerIndex].samplerState = static_cast<VulkanSamplerState*>(state);
}

void VulkanLocalShaderSamplerBuffer::addDescriptor(DescriptorType type, uint32_t bindingIndex, const std::string& name, uint32_t imageInfoIndex, uint32_t writeInfoIndex)
{
	Entry e;
    e.type = type;
	e.name = name;
    e.descriptorImageInfoIndex = imageInfoIndex;
	e.descriptorWriteInfoIndex = writeInfoIndex;
	e.bindingIndex = bindingIndex;
	m_table.push_back(std::move(e));
}

} // namespace detail
} // namespace ln
