// GLFW より先に取り込むこと (理由は VulkanLoader.hpp の注意書きを参照)。
#include "VulkanLoader.hpp"
#include <GLFW/glfw3.h>

#include <cstring>
#include <LuminoBase/Logger.hpp>

#include <LuminoBase/SmallVector.hpp>
#include "VulkanHelpers.hpp"
#include "VulkanBuffer.hpp"
#include "VulkanTexture.hpp"
#include "VulkanTextureView.hpp"
#include "VulkanCommandBuffer.hpp"
#include "VulkanSwapChain.hpp"
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
    LN_LOG_ERROR("Vulkan validation: [ %s ] |  MessageID = %d", pCallbackData->pMessageIdName, pCallbackData->messageIdNumber);
    LN_LOG_ERROR("%s", pCallbackData->pMessage);

    if (pCallbackData->objectCount > 0) {
        LN_LOG_ERROR("Objects: %u", pCallbackData->objectCount);
        for (int i = 0; i < pCallbackData->objectCount; i++) {
            const auto& obj = pCallbackData->pObjects[i];
            // TODO: type は 次のようにして文字列化できるようにしたい。
            // #include <vulkan/vk_enum_string_helper.h>
            // const char* typeName = string_VkObjectType(VK_OBJECT_TYPE_DEVICE);
            // "VK_OBJECT_TYPE_DEVICE" が返る
            LN_LOG_ERROR("  [%d] Type: %d, Handle: 0x%llx, Name: %s", i, (int)obj.objectType, (unsigned long long)obj.objectHandle, (obj.pObjectName ? obj.pObjectName : "N/A"));
        }
    }

#if defined(_MSC_VER) && defined(_DEBUG)
    __debugbreak();
#endif

    return VK_FALSE;
}

// ------ VulkanDevice ----------------------------------------------------------------------------------------------------------------

VulkanDevice::VulkanDevice() = default;

VoidResult VulkanDevice::init(const DeviceDesc& desc) {
    // Vulkan ローダーを動的にロードする。ここで失敗するのは Vulkan が使えない
    // 環境というだけなので、呼び出し側は他のバックエンドへフォールバックできる。
    if (!loadVulkanLoader()) {
        return LN_MAKE_ERROR("Vulkan loader is not available on this system.");
    }

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

    if (const VkResult r = vkCreateInstance(&instInfo, nullptr, &m_instance); r != VK_SUCCESS) {
        // VK_ERROR_INCOMPATIBLE_DRIVER (-9): Vulkan ドライバ (ICD) が未登録。
        // VK_ERROR_LAYER_NOT_PRESENT (-6): 要求した検証レイヤーが見つからない。
        return LN_MAKE_ERROR("vkCreateInstance failed. (VkResult: %d)", static_cast<int>(r));
    }

    // インスタンスレベルの関数をグローバルへロードする。デバイスレベルの関数は
    // vkCreateDevice の後に volkLoadDevice() でロードする。
    volkLoadInstanceOnly(m_instance);

    if (desc.enableValidation) {
        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
        if (CreateDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS) {
            LN_LOG_ERROR("Warning: Failed to set up Vulkan debug messenger.");
        }
    }

    // 物理デバイスを選択
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
            return LN_FORWARD_ERROR(result);
        }
    }


    // 論理デバイスを作成
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

    // デバイスレベルの関数を vkGetDeviceProcAddr で直接取得し、ローダーの
    // ディスパッチを 1 段省く。
    // ponytail: グローバルの関数ポインタへロードするため VkDevice は 1 プロセスに
    // 1 つまで。複数デバイスを扱うなら volkLoadDeviceTable() + VolkDeviceTable へ。
    volkLoadDevice(m_device);

    vkGetDeviceQueue(m_device, m_graphicsQueuFamily, 0, &m_graphicsQueue);

    // コマンドプール
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = m_graphicsQueuFamily;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_commandPool);

    // リソース管理サブシステム
    m_descriptorPoolManager.init(m_device);
    m_stagingPool.init(this, m_physicalDevice);
    return LN_MAKE_SUCCESS();
}

void VulkanDevice::finalize() {
    if (m_device) vkDeviceWaitIdle(m_device);

    // プールを破棄する前に、遅延破棄 (コマンドバッファなど) をすべて実行する。
    m_frameResources.flushAll();

    // キャッシュを破棄
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

    SmallVector<VkAttachmentDescription, kMaxMultiRenderTargets + 1> attachments;
    SmallVector<VkAttachmentReference, kMaxMultiRenderTargets> colorRefs;

    for (size_t i = 0; i < key.colorAttachments.size(); ++i) {
        VkAttachmentDescription att{};
        att.format = key.colorAttachments[i].format;
        att.samples = VK_SAMPLE_COUNT_1_BIT;
        att.loadOp = key.colorAttachments[i].loadOp;
        att.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        att.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        att.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        if (key.colorAttachments[i].isSwapchainBackbuffer) {
            // NOTE: スワップチェーンのバックバッファは present 前後どちらも PRESENT_SRC_KHR とする。
            //   present 後や再 acquire 後のレイアウトは PRESENT_SRC_KHR のまま残るため、initialLayout も PRESENT_SRC_KHR を指定する。
            //   各イメージの初回 acquire 時のみ UNDEFINED から PRESENT_SRC_KHR へ遷移させる処理を
            //   VulkanSwapChain::acquireNextTexture() で行っている (presentable image は acquire 後でないと遷移できないため)。
            //   ※ Vulkan Tutorial ではシングルパスでフレーム開始時にクリアするため UNDEFINED を指定しているが、
            //   Lumino ではマルチパスでフレーム開始時にクリアしないこともあるため、適切なレイアウトを指定する。
            //   see: [ VUID-VkPresentInfoKHR-pImageIndices-01430 ]
            //
            att.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            att.finalLayout   = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        } else {
            // パス終了後はシェーダ入力(テクスチャ)として使用できるように VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL に遷移する。
            // https://qiita.com/Pctg-x8/items/a1a39678e9ca95c59d19
            att.initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;  // もし DONT_CARE と併用する場合は UNDEFINED にしておくとよい
            att.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        }
        attachments.push_back(att);

        VkAttachmentReference ref{};
        ref.attachment = static_cast<uint32_t>(i);
        // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL まはた GENERAL でなければならない。
        // (https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-VkRenderPassCreateInfo-pAttachments-00836)
        // VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ではダメ。
        // 要するに、描画中のレンダーターゲットは書き込み可能なレイアウトにしておきなさい、ということ。
        ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        colorRefs.push_back(ref);
    }

    VkAttachmentReference depthRef{};
    if (key.depthFormat != VK_FORMAT_UNDEFINED) {
        VkAttachmentDescription att{};
        att.format = key.depthFormat;
        att.samples = VK_SAMPLE_COUNT_1_BIT;
        att.loadOp = key.depthLoadOp;
        att.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        att.stencilLoadOp = key.stencilLoadOp;
        att.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
        // depth または stencil のいずれかが LOAD の場合、既存の内容を読み込むため
        // initialLayout を UNDEFINED にはできない (VUID-VkAttachmentDescription-format-06699/06700)。
        // 直前のパスの finalLayout (= DEPTH_STENCIL_ATTACHMENT_OPTIMAL) を引き継ぐ。
        // ※ Load を指定する場合、呼び出し側は事前にこのデプスバッファを描画済みである必要がある。
        const bool depthOrStencilLoad = (key.depthLoadOp == VK_ATTACHMENT_LOAD_OP_LOAD) ||
                                        (key.stencilLoadOp == VK_ATTACHMENT_LOAD_OP_LOAD);
        att.initialLayout = depthOrStencilLoad
            ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
            : VK_IMAGE_LAYOUT_UNDEFINED;
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
    if (key.depthFormat != VK_FORMAT_UNDEFINED) {
        // デプスステンシルを Load する場合、直前パスのデプス書き込みと
        // 本パスのデプス読み書きを正しく同期させる。
        dep.srcStageMask |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        dep.dstStageMask |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        dep.srcAccessMask |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        dep.dstAccessMask |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    }

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
    limits.minUniformBufferOffsetAlignment = static_cast<uint32_t>(props.limits.minUniformBufferOffsetAlignment);
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
    // Vertex と Index のバッファはデバイスローカル (GPU に最適な) メモリの方が有利。
    // ただし呼び出し側が明示的にマップ可能 (ホスト可視) なバッファを要求した場合は除く。
    bool useDeviceLocal =
        !desc.mappable &&
        ((desc.usage & BufferUsage::Vertex) || (desc.usage & BufferUsage::Index));

    auto buf = Ref<VulkanBuffer>::adopt(new VulkanBuffer());
    if (!buf->init(this, m_physicalDevice, desc, useDeviceLocal)) {
        return LN_MAKE_ERROR("Failed to create buffer.");
    }

    if (useDeviceLocal && desc.initialData && desc.size > 0) {
        m_stagingPool.uploadImmediate(buf->handle(), desc.initialData, desc.size);
    }

    return Ref<Buffer>(buf);
}

Result<Ref<Texture>> VulkanDevice::createTexture(const TextureDesc& desc) {
    auto tex = Ref<VulkanTexture>::adopt(new VulkanTexture());
    if (!tex->init(this, m_physicalDevice, desc)) {
        return LN_MAKE_ERROR("Failed to create texture.");
    }

    // 初期データがあればステージングバッファ経由でアップロードする。
    if (desc.initialData) {
        uint32_t bpp = 4; // 一般的なフォーマットは 1 ピクセル 4 バイトとみなす。
        if (desc.format == TextureFormat::R8Unorm) bpp = 1;
        else if (desc.format == TextureFormat::RG8Unorm) bpp = 2;
        else if (desc.format == TextureFormat::RGBA16Float) bpp = 8;
        else if (desc.format == TextureFormat::RGBA32Float) bpp = 16;
        VkDeviceSize imageSize = static_cast<VkDeviceSize>(desc.width) * desc.height * bpp;

        m_stagingPool.uploadTextureImmediate(
            tex->handle(), desc.initialData, imageSize, desc.width, desc.height);
    }

    return Ref<Texture>(tex);
}

Result<Ref<TextureView>> VulkanDevice::createTextureView(Texture* texture) {
    auto* vtex = static_cast<VulkanTexture*>(texture);
    VkFormat fmt = VulkanHelpers::toVkFormat(vtex->format());
    VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT;
    if (vtex->format() == TextureFormat::Depth24Stencil8) {
        aspect = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    } else if (vtex->format() == TextureFormat::Depth32Float) {
        aspect = VK_IMAGE_ASPECT_DEPTH_BIT;
    }
    auto view = Ref<VulkanTextureView>::adopt(new VulkanTextureView());
    if (!view->init(this, vtex->handle(), fmt, aspect, vtex->width(), vtex->height())) {
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
    if (!sm->init(this, desc)) {
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

Result<Ref<RenderPipeline>> VulkanDevice::createRenderPipeline(const RenderPipelineDesc& desc) {
    // パイプラインの互換性はアタッチメントのフォーマットとサンプル数だけで決まる
    // (load op や初期レイアウトは影響しない) ため、レイアウトから互換なレンダーパスを引く。
    RenderPassKey rpKey;
    for (auto fmt : desc.renderPassLayout.colorFormats) {
        RenderPassKey::ColorAttachment attachment = {};
        attachment.format = VulkanHelpers::toVkFormat(fmt);
        attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        rpKey.colorAttachments.push_back(attachment);
    }
    rpKey.depthFormat = VulkanHelpers::toVkFormat(desc.renderPassLayout.depthStencilFormat);
    VkRenderPass renderPass = getOrCreateRenderPass(rpKey);

    auto rp = Ref<VulkanRenderPipeline>::adopt(new VulkanRenderPipeline());
    if (!rp->init(this, renderPass, desc)) {
        return LN_MAKE_ERROR("Failed to create render pipeline.");
    }
    return Ref<RenderPipeline>(rp);
}

uint32_t VulkanDevice::currentFrameIndex() const {
    // アクティブな SwapChain が無いのは、まだ 1 フレームも回していないか、
    // SwapChain を破棄した後 (デバイスロスト復旧など) のどちらか。
    // どちらも GPU は待機済みなので、どのキューに積んでも解放の安全性は変わらない。
    return m_activeSwapChain ? m_activeSwapChain->currentFrame() : 0u;
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

VoidResult VulkanDevice::writeBuffer(Buffer* dst, uint64_t dstOffset, const void* data, uint64_t size) {
    auto* vkBuffer = static_cast<vulkan::VulkanBuffer*>(dst);
    if (vkBuffer->isDeviceLocal()) {
        m_stagingPool.uploadImmediate(vkBuffer->handle(), data, size, dstOffset);
    } else {
        void* mapped = vkBuffer->mappedMemory();
        if (!mapped) {
            return LN_MAKE_ERROR("Failed to map buffer for writeBuffer.");
        }
        std::memcpy(static_cast<uint8_t*>(mapped) + dstOffset, data, size);
    }
    return LN_MAKE_SUCCESS();
}

Result<std::vector<uint8_t>> VulkanDevice::readbackTexture(TextureView* view) {
    auto* vkView = static_cast<VulkanTextureView*>(view);
    if (!vkView || vkView->image() == VK_NULL_HANDLE) {
        return LN_MAKE_ERROR("Invalid TextureView for readback.");
    }

    // スワップチェーンのイメージは present() 後は PRESENT_SRC_KHR になっている。
    const VkImageLayout currentLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    auto pixels = m_stagingPool.downloadTextureImmediate(
        vkView->image(), currentLayout, vkView->width(), vkView->height());

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
    VkResult result = checkDeviceLost(vkEndCommandBuffer(commandBuffer), "vkEndCommandBuffer");
    if (result != VK_SUCCESS) {
        LN_LOG_ERROR("endSingleTimeCommands: vkEndCommandBuffer failed (%d)", static_cast<int>(result));
        // return せず続行する。
    }

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    result = checkDeviceLost(
        vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE), "vkQueueSubmit");
    if (result != VK_SUCCESS) {
        LN_LOG_ERROR("endSingleTimeCommands: vkQueueSubmit failed (%d)", static_cast<int>(result));
        // return せず続行する。
    }

    result = checkDeviceLost(vkQueueWaitIdle(m_graphicsQueue), "vkQueueWaitIdle");
    if (result != VK_SUCCESS) {
        LN_LOG_ERROR("endSingleTimeCommands: vkQueueWaitIdle failed (%d)", static_cast<int>(result));
        // return せず続行する。
    }

    vkFreeCommandBuffers(m_device, m_commandPool, 1, &commandBuffer);
}

VkResult VulkanDevice::checkDeviceLost(VkResult r, const char* what) {
    if (r == VK_ERROR_DEVICE_LOST) {
        LN_LOG_ERROR("[Vulkan] VK_ERROR_DEVICE_LOST detected: %s", what ? what : "?");
        markDeviceLost(what);
    }
    return r;
}

// Graphics と Compute を両方サポートするキューファミリを探す。
// Vulkan はそのようなファミリが少なくとも 1 つ存在することを保証している。
// 現状 Lumino が実際に使うのは Graphics キューだけ。
VoidResult VulkanDevice::lookupQueueFamilies(
    VkPhysicalDevice physicalDevice,
    uint32_t* outGraphicsQueuFamily) {
    uint32_t propCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propCount, nullptr);
    std::vector<VkQueueFamilyProperties> props(propCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &propCount, props.data());

    constexpr VkQueueFlags required = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT;
    for (uint32_t i = 0; i < propCount; ++i) {
        if ((props[i].queueFlags & required) == required) {
            *outGraphicsQueuFamily = i;
            return LN_MAKE_SUCCESS();
        }
    }
    return LN_MAKE_ERROR("No queue family supporting graphics was found.");
}

} // namespace ln::rhi::vulkan

// ------ Device::create factory --------------------------------------------------------------------------------------------

#ifdef LN_ENABLE_WEBGPU
#include "../webgpu/WebGPUDevice.hpp"
#endif
#ifdef LN_ENABLE_WEBGL2
#include "../webgl2/WebGL2Device.hpp"
#endif

namespace ln::rhi {

Result<Ref<Device>> Device::create(const DeviceDesc& desc) {
    if (desc.backend == Backend::Vulkan) {
        auto dev = Ref<vulkan::VulkanDevice>::adopt(new vulkan::VulkanDevice());
        if (!dev->init(desc)) {
            return LN_MAKE_ERROR("Failed to initialize Vulkan device");
        }
        return Ref<Device>(dev);
    }
#ifdef LN_ENABLE_WEBGPU
    if (desc.backend == Backend::WebGPU) {
        auto dev = Ref<webgpu::WebGPUDevice>::adopt(new webgpu::WebGPUDevice());
        if (!dev->init(desc)) {
            return LN_MAKE_ERROR("Failed to initialize WebGPU device");
        }
        return Ref<Device>(dev);
    }
#endif
#ifdef LN_ENABLE_WEBGL2
    // デスクトップでも ANGLE 上で ES 3.0 として動かし、Web の主経路を再現する。
    if (desc.backend == Backend::WebGL2) {
        auto dev = Ref<webgl2::WebGL2Device>::adopt(new webgl2::WebGL2Device());
        auto r = dev->init(desc);
        if (!r) {
            return LN_FORWARD_ERROR(r);
        }
        return Ref<Device>(dev);
    }
#endif
    return LN_MAKE_ERROR("Unsupported backend");
}

Result<Ref<Device>> Device::beginCreateAsync(const DeviceDesc& desc) {
    // デスクトップのバックエンドは同期的に初期化できるため、ブロッキング作成に委譲する。
    // (Vulkan は同期 API、ネイティブ Dawn のコールバックは AllowSpontaneous で同期的に呼ばれる)
    return create(desc);
}

} // namespace ln::rhi
