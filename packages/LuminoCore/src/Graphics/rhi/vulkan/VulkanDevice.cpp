#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <numeric>
#include <unordered_set>

#include "VulkanHelpers.hpp"
#include "VulkanBackend.hpp"
#include "VulkanDevice.hpp"


namespace ln::rhi::vulkan {

// ------ Debug messenger helpers -------------------------------------------------------------------------------------------------

static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    std::cerr << "Vulkan validation: [ " << pCallbackData->pMessageIdName << " ] |  MessageID = " << std::hex<< std::dec
              << pCallbackData->messageIdNumber << std::dec << std::endl;
    std::cerr << pCallbackData->pMessage << std::endl;

    if (pCallbackData->objectCount > 0) {
        std::cerr << "Objects: " << pCallbackData->objectCount << std::endl;
        for (int i = 0; i < pCallbackData->objectCount; i++) {
            const auto& obj = pCallbackData->pObjects[i];
            // TODO: type は 次のようにして文字列化できるようにしたい。
            // #include <vulkan/vk_enum_string_helper.h>
            // const char* typeName = string_VkObjectType(VK_OBJECT_TYPE_DEVICE);
            // Returns "VK_OBJECT_TYPE_DEVICE"
            std::cerr << "  [" << i << "] Type: " << obj.objectType
                      << ", Handle: " << std::hex << obj.objectHandle << std::dec << ", Name: " << (obj.pObjectName ? obj.pObjectName : "N/A") << std::endl;
        }
    }

#if defined(_MSC_VER) && defined(_DEBUG)
    //if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
    __debugbreak();
    //}
#endif

    return VK_FALSE;
}

// ------ VulkanDevice ----------------------------------------------------------------------------------------------------------------

VulkanDevice::VulkanDevice() = default;

VoidResult VulkanDevice::init(const DeviceDesc& desc) {
    // Create instance
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Lumino";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Lumino";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_1;

    std::vector<const char*> instanceExtensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
    };

    uint32_t glfwExtCount = 0;
    const char** glfwExts = glfwGetRequiredInstanceExtensions(&glfwExtCount);
    if (glfwExts) {
        instanceExtensions.assign(glfwExts, glfwExts + glfwExtCount);
    }

    if (desc.enableValidation) {
        instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    std::vector<const char*> layers;
    if (desc.enableValidation) {
        layers.push_back("VK_LAYER_KHRONOS_validation");
    }

    VkInstanceCreateInfo instInfo{};
    instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instInfo.pApplicationInfo = &appInfo;
    instInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
    instInfo.ppEnabledExtensionNames = instanceExtensions.data();
    instInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
    instInfo.ppEnabledLayerNames = layers.data();

    if (vkCreateInstance(&instInfo, nullptr, &m_instance) != VK_SUCCESS) {
        return LN_MAKE_ERROR("vkCreateInstance failed.");
    }

#if 0
    // Setup debug messenger
    if (desc.enableValidation) {
        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
        if (CreateDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS) {
            std::cerr << "Warning: Failed to set up Vulkan debug messenger." << std::endl;
        }
    }
#endif

    // Select physical device
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
    if (deviceCount == 0) return LN_MAKE_ERROR("No Vulkan-capable GPU found.");
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());
    m_physicalDevice = devices[0];

    VkPhysicalDeviceProperties deviceProperties = {};
    vkGetPhysicalDeviceProperties(m_physicalDevice, &deviceProperties);
    VulkanHelpers::logDeviceProperties(deviceProperties);

    
    {
        auto result = lookupQueueFamilies(m_physicalDevice, &m_graphicsQueuFamily);
        if (!result) {
            return LN_BOX_ERROR(result);
        }
    }


    // Create logical device
    float priority = 1.0f;
    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = m_graphicsQueuFamily;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &priority;

    const char* devExtensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkDeviceCreateInfo devInfo{};
    devInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    devInfo.queueCreateInfoCount = 1;
    devInfo.pQueueCreateInfos = &queueInfo;
    devInfo.enabledExtensionCount = 1;
    devInfo.ppEnabledExtensionNames = devExtensions;

    if (vkCreateDevice(m_physicalDevice, &devInfo, nullptr, &m_device) != VK_SUCCESS) {
        return LN_MAKE_ERROR("vkCreateDevice failed.");
    }

    vkGetDeviceQueue(m_device, m_graphicsQueuFamily, 0, &m_graphicsQueue);

    // Command pool
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = m_graphicsQueuFamily;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_commandPool);

    // Resource management subsystems
    m_descriptorPoolManager.init(m_device);
    m_stagingPool.init(m_device, m_physicalDevice);
    return LN_MAKE_SUCCESS();
}

void VulkanDevice::finalize() {
    if (m_device) vkDeviceWaitIdle(m_device);

    // Flush any deferred cleanups (e.g., command buffers) before destroying the pool.
    m_frameResources.flushAll();

    // Destroy caches
    for (auto& [key, fb] : m_framebufferCache) vkDestroyFramebuffer(m_device, fb, nullptr);
    for (auto& [key, rp] : m_renderPassCache) vkDestroyRenderPass(m_device, rp, nullptr);

    m_stagingPool.destroy();
    m_descriptorPoolManager.destroy();
    if (m_commandPool) vkDestroyCommandPool(m_device, m_commandPool, nullptr);
    if (m_device) vkDestroyDevice(m_device, nullptr);
    if (m_debugMessenger) DestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
    if (m_instance) vkDestroyInstance(m_instance, nullptr);
    Device::finalize();
}

VkRenderPass VulkanDevice::getOrCreateRenderPass(const RenderPassKey& key) {
    std::lock_guard lock(m_cacheMutex);
    auto it = m_renderPassCache.find(key);
    if (it != m_renderPassCache.end()) return it->second;

    std::vector<VkAttachmentDescription> attachments;
    std::vector<VkAttachmentReference> colorRefs;

    for (size_t i = 0; i < key.colorAttachments.size(); ++i) {
        VkAttachmentDescription att{};
        att.format = key.colorAttachments[i].format;
        att.samples = VK_SAMPLE_COUNT_1_BIT;
        att.loadOp = key.colorAttachments[i].loadOp;
        att.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        att.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        att.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        att.initialLayout = att.loadOp == VK_ATTACHMENT_LOAD_OP_LOAD
                                ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                                : VK_IMAGE_LAYOUT_UNDEFINED;
        att.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        attachments.push_back(att);

        VkAttachmentReference ref{};
        ref.attachment = static_cast<uint32_t>(i);
        ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        colorRefs.push_back(ref);
    }

    VkAttachmentReference depthRef{};
    if (key.depthFormat != VK_FORMAT_UNDEFINED) {
        VkAttachmentDescription att{};
        att.format = key.depthFormat;
        att.samples = VK_SAMPLE_COUNT_1_BIT;
        att.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        att.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        att.stencilLoadOp = key.stencilLoadOp;
        att.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
        att.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        att.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        attachments.push_back(att);

        depthRef.attachment = static_cast<uint32_t>(attachments.size() - 1);
        depthRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    }

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = static_cast<uint32_t>(colorRefs.size());
    subpass.pColorAttachments = colorRefs.data();
    if (key.depthFormat != VK_FORMAT_UNDEFINED) {
        subpass.pDepthStencilAttachment = &depthRef;
    }

    VkSubpassDependency dep{};
    dep.srcSubpass = VK_SUBPASS_EXTERNAL;
    dep.dstSubpass = 0;
    dep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dep.srcAccessMask = 0;
    dep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo rpInfo{};
    rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rpInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    rpInfo.pAttachments = attachments.data();
    rpInfo.subpassCount = 1;
    rpInfo.pSubpasses = &subpass;
    rpInfo.dependencyCount = 1;
    rpInfo.pDependencies = &dep;

    VkRenderPass rp = VK_NULL_HANDLE;
    vkCreateRenderPass(m_device, &rpInfo, nullptr, &rp);
    m_renderPassCache[key] = rp;
    return rp;
}

VkFramebuffer VulkanDevice::getOrCreateFramebuffer(const FramebufferKey& key) {
    std::lock_guard lock(m_cacheMutex);
    auto it = m_framebufferCache.find(key);
    if (it != m_framebufferCache.end()) return it->second;

    VkFramebufferCreateInfo fbInfo{};
    fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fbInfo.renderPass = key.renderPass;
    fbInfo.attachmentCount = static_cast<uint32_t>(key.attachments.size());
    fbInfo.pAttachments = key.attachments.data();
    fbInfo.width = key.width;
    fbInfo.height = key.height;
    fbInfo.layers = 1;

    VkFramebuffer fb = VK_NULL_HANDLE;
    vkCreateFramebuffer(m_device, &fbInfo, nullptr, &fb);
    m_framebufferCache[key] = fb;
    return fb;
}

DeviceLimits VulkanDevice::deviceLimits() const {
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(m_physicalDevice, &props);
    DeviceLimits limits;
    limits.minUniformBufferOffsetAlignment = static_cast<u32>(props.limits.minUniformBufferOffsetAlignment);
    limits.maxUniformBufferRange = props.limits.maxUniformBufferRange;
    return limits;
}

Result<Ref<SwapChain>> VulkanDevice::createSwapChain(const SwapChainDesc& desc) {
    auto sc = Ref<VulkanSwapChain>::adopt(new VulkanSwapChain());
    if (!sc->init(this, desc)) {
        return LN_MAKE_ERROR("Failed to create swap chain.");
    }
    return Ref<SwapChain>(sc);
}

Result<Ref<Buffer>> VulkanDevice::createBuffer(const BufferDesc& desc) {
    // Vertex and Index buffers benefit from device-local (GPU-optimal) memory,
    // unless the caller explicitly requests mappable (host-visible) buffers.
    bool useDeviceLocal =
        !desc.mappable &&
        ((desc.usage & BufferUsage::Vertex) || (desc.usage & BufferUsage::Index));

    auto buf = Ref<VulkanBuffer>::adopt(new VulkanBuffer());
    if (!buf->init(this, m_physicalDevice, desc, useDeviceLocal)) {
        return LN_MAKE_ERROR("Failed to create buffer.");
    }

    if (useDeviceLocal && desc.initialData && desc.size > 0) {
        m_stagingPool.uploadImmediate(
            m_graphicsQueue, m_commandPool, buf->handle(), desc.initialData, desc.size);
    }

    return Ref<Buffer>(buf);
}

Result<Ref<Texture>> VulkanDevice::createTexture(const TextureDesc& desc) {
    auto tex = Ref<VulkanTexture>::adopt(new VulkanTexture());
    if (!tex->init(this, m_physicalDevice, desc)) {
        return LN_MAKE_ERROR("Failed to create texture.");
    }

    // Upload initial data via staging buffer if provided.
    if (desc.initialData) {
        u32 bpp = 4; // Assume 4 bytes per pixel for common formats.
        if (desc.format == TextureFormat::R8Unorm) bpp = 1;
        else if (desc.format == TextureFormat::RG8Unorm) bpp = 2;
        else if (desc.format == TextureFormat::RGBA16Float) bpp = 8;
        else if (desc.format == TextureFormat::RGBA32Float) bpp = 16;
        VkDeviceSize imageSize = static_cast<VkDeviceSize>(desc.width) * desc.height * bpp;

        m_stagingPool.uploadTextureImmediate(
            m_graphicsQueue, m_commandPool,
            tex->handle(), desc.initialData, imageSize,
            desc.width, desc.height);
    }

    return Ref<Texture>(tex);
}

Result<Ref<TextureView>> VulkanDevice::createTextureView(Texture* texture) {
    auto* vtex = static_cast<VulkanTexture*>(texture);
    VkFormat fmt = toVkFormat(vtex->format());
    VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT;
    if (vtex->format() == TextureFormat::Depth24Stencil8) {
        aspect = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    } else if (vtex->format() == TextureFormat::Depth32Float) {
        aspect = VK_IMAGE_ASPECT_DEPTH_BIT;
    }
    auto view = Ref<VulkanTextureView>::adopt(new VulkanTextureView());
    if (!view->init(m_device, vtex->handle(), fmt, aspect, vtex->width(), vtex->height())) {
        return LN_MAKE_ERROR("Failed to create texture view.");
    }
    return Ref<TextureView>(view);
}

Result<Ref<Sampler>> VulkanDevice::createSampler(const SamplerDesc& desc) {
    auto s = Ref<VulkanSampler>::adopt(new VulkanSampler());
    if (!s->init(this, desc)) {
        return LN_MAKE_ERROR("Failed to create sampler.");
    }
    return Ref<Sampler>(s);
}

Result<Ref<ShaderModule>> VulkanDevice::createShaderModule(const ShaderModuleDesc& desc) {
    auto sm = Ref<VulkanShaderModule>::adopt(new VulkanShaderModule());
    if (!sm->init(m_device, desc)) {
        return LN_MAKE_ERROR("Failed to create shader module.");
    }
    return Ref<ShaderModule>(sm);
}

Result<Ref<PipelineLayout>> VulkanDevice::createPipelineLayout(const PipelineLayoutDesc& desc) {
    auto pl = Ref<VulkanPipelineLayout>::adopt(new VulkanPipelineLayout());
    if (!pl->init(this, desc)) {
        return LN_MAKE_ERROR("Failed to create pipeline layout.");
    }
    return Ref<PipelineLayout>(pl);
}

Result<Ref<RenderPass>> VulkanDevice::createRenderPass(const RenderPassLayoutDesc& desc) {
    RenderPassKey rpKey;
    for (auto& fmt : desc.colorFormats) {
        RenderPassKey::ColorAttachment attachment = {};
        attachment.format = toVkFormat(fmt);
        attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        rpKey.colorAttachments.push_back(attachment);
    }
    if (desc.depthStencilFormat != TextureFormat::Undefined) {
        rpKey.depthFormat = toVkFormat(desc.depthStencilFormat);
    }
    VkRenderPass vkRenderPass = getOrCreateRenderPass(rpKey);

    auto rp = Ref<VulkanRenderPass>::adopt(new VulkanRenderPass(vkRenderPass, desc));
    return Ref<RenderPass>(rp);
}

Result<Ref<RenderPipeline>> VulkanDevice::createRenderPipeline(const RenderPipelineDesc& desc) {
    auto* vulkanRP = static_cast<vulkan::VulkanRenderPass*>(desc.renderPass);
    VkRenderPass renderPass = vulkanRP->handle();

    auto rp = Ref<VulkanRenderPipeline>::adopt(new VulkanRenderPipeline());
    if (!rp->init(this, renderPass, desc)) {
        return LN_MAKE_ERROR("Failed to create render pipeline.");
    }
    return Ref<RenderPipeline>(rp);
}

Result<Ref<VulkanCommandBuffer>> VulkanDevice::createCommandBuffer() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer cmd;
    if (vkAllocateCommandBuffers(m_device, &allocInfo, &cmd) != VK_SUCCESS) {
        return LN_MAKE_ERROR("vkAllocateCommandBuffers failed.");
    }
    auto cb = Ref<VulkanCommandBuffer>::adopt(new VulkanCommandBuffer());
    if (!cb->init(this, cmd)) {
        return LN_MAKE_ERROR("Failed to initialize command buffer.");
    }
    return cb;
}

Result<std::vector<uint8_t>> VulkanDevice::readbackTexture(TextureView* view) {
    auto* vkView = static_cast<VulkanTextureView*>(view);
    if (!vkView || vkView->image() == VK_NULL_HANDLE) {
        return tl::make_unexpected(Error{ErrorCode::InvalidArgument, "Invalid TextureView for readback."});
    }

    // Swapchain images are in PRESENT_SRC_KHR after present().
    const VkImageLayout currentLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    auto pixels = m_stagingPool.downloadTextureImmediate(
        m_graphicsQueue, m_commandPool,
        vkView->image(), currentLayout,
        vkView->width(), vkView->height(),
        vkView->vkFormat());

    return pixels;
}

void VulkanDevice::waitIdle() {
    if (m_device) vkDeviceWaitIdle(m_device);
}

Result<VkCommandBuffer> VulkanDevice::beginSingleTimeCommands() {
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_commandPool;
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

void VulkanDevice::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    VkResult result = vkEndCommandBuffer(commandBuffer);
    if (result != VK_SUCCESS) {
        LN_MAKE_VULKAN_ERROR(result, "vkEndCommandBuffer");
        // no return, continue.
    }

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    result = vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    if (result != VK_SUCCESS) {
        LN_MAKE_VULKAN_ERROR(result, "vkQueueSubmit");
        // no return, continue.
    }

    result = vkQueueWaitIdle(m_graphicsQueue);
    if (result != VK_SUCCESS) {
        LN_MAKE_VULKAN_ERROR(result, "vkQueueWaitIdle");
        // no return, continue.
    }

    vkFreeCommandBuffers(m_device, m_commandPool, 1, &commandBuffer);
}

//VoidResult VulkanDevice::transitionImageLayoutImmediately(
//    VkImage image,
//    VkFormat format,
//    uint32_t mipLevel,
//    VkImageLayout oldLayout,
//    VkImageLayout newLayout) {
//    VkCommandBuffer commandBuffer = beginSingleTimeCommands();
//    Result_deprecated<> result = transitionImageLayout(commandBuffer, image, format, mipLevel, oldLayout, newLayout);
//    endSingleTimeCommands(commandBuffer);
//    return result;
//}



// Vulkan Tutorial(https://vulkan-tutorial.com/Compute_Shader) では
// VK_QUEUE_GRAPHICS_BIT と VK_QUEUE_COMPUTE_BIT が両方サポートされている QueueFamily を使っており、
// Vulkan 自体も そのような QueueFamily が最低1つあることをサポートしているとのこと。
//
// ただここでは、 Graphics と Compute、 そして Transfer を別々にサポートしている QueueFamily を優先して探すようにする
// …というのを昔作ったので、そのまま移植してみている。
// 現状、実際に使っているのは Graphics Queue だけなので注意。
//
VoidResult VulkanDevice::lookupQueueFamilies(
    VkPhysicalDevice physicalDevice,
    uint32_t* outGraphicsQueuFamily) {
    auto graphicsFamilyIndex = UINT32_MAX;
    auto computeFamilyIndex = UINT32_MAX;
    auto transferFamilyIndex = UINT32_MAX;
    auto graphicsQueueIndex = UINT32_MAX;
    auto computeQueueIndex = UINT32_MAX;
    auto transferQueueindex = UINT32_MAX;
    std::vector<VkQueueFamilyProperties> queueFamilyProps;
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::vector<float> queuePriorities;
    {
        uint32_t propCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propCount, nullptr);
        queueFamilyProps.resize(propCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propCount, queueFamilyProps.data());
        queueCreateInfos.resize(propCount);

        // Family[0]: Graphics|Compute|Transfer, QueueCount=16
        // 

        int queueIndex = 0;
        int totalQueueCount = 0;
        float queuePriority = 1.0f;
        for (int i = 0; i < propCount; ++i) {
            queueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfos[i].pNext = nullptr;
            queueCreateInfos[i].flags = 0;
            queueCreateInfos[i].queueCount = queueFamilyProps[i].queueCount;
            queueCreateInfos[i].queueFamilyIndex = i;

            totalQueueCount += queueFamilyProps[i].queueCount;

            // Graphics queue
            //if (m_queueFamilyProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            if ((queueFamilyProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
                (queueFamilyProps[i].queueFlags & VK_QUEUE_COMPUTE_BIT)) {
                if (graphicsFamilyIndex == UINT32_MAX) {
                    graphicsFamilyIndex = i;
                    graphicsQueueIndex = queueIndex;
                    queueIndex++;
                }
            }

            // Compute queue
            if ((queueFamilyProps[i].queueFlags & VK_QUEUE_COMPUTE_BIT) &&
                ((queueFamilyProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != VK_QUEUE_GRAPHICS_BIT)) {
                if (computeFamilyIndex == UINT32_MAX) {
                    computeFamilyIndex = i;
                    computeQueueIndex = queueIndex;
                    queueIndex++;
                }
            }

            // Transfer queue
            if ((queueFamilyProps[i].queueFlags & VK_QUEUE_TRANSFER_BIT) &&
                ((queueFamilyProps[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != VK_QUEUE_GRAPHICS_BIT)) {
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
                if (queueFamilyProps[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
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
                if (queueFamilyProps[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
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

    *outGraphicsQueuFamily = graphicsFamilyIndex;
    return LN_MAKE_SUCCESS();
}

} // namespace ln::rhi::vulkan

// ------ Device::create factory --------------------------------------------------------------------------------------------

namespace ln::rhi {

Result<Ref<Device>> Device::create(const DeviceDesc& desc) {
    if (desc.backend == Backend::Vulkan) {
        auto dev = Ref<vulkan::VulkanDevice>::adopt(new vulkan::VulkanDevice());
        if (!dev->init(desc)) {
            return tl::unexpected(Error{ErrorCode::NotInitialized, "Failed to initialize Vulkan device"});
        }
        return Ref<Device>(dev);
    }
    return tl::unexpected(Error{ErrorCode::NotSupported, "Unsupported backend"});
}

} // namespace ln::rhi
