#include <LuminoBase/Logger.hpp>
#include "VulkanHelpers.hpp"
#include "VulkanDevice.hpp"
#include "VulkanBackend.hpp"
#include "VulkanTextureView.hpp"
#include "VulkanSwapChain.hpp"
#include "VulkanCommandBuffer.hpp"

namespace ln::rhi::vulkan {

VulkanCommandBuffer::VulkanCommandBuffer() = default;

VoidResult VulkanCommandBuffer::init(VulkanDevice* device, VkCommandBuffer cmd) {
    m_device = device;
    m_cmd = cmd;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    if (m_device->vk().vkCreateFence(
            m_device->vkDevice(),
            &fenceInfo,
            m_device->vulkanAllocator(),
            &m_inFlightFences) != VK_SUCCESS) {
        return LN_MAKE_ERROR("vkCreateFence failed.");
    }
    return LN_MAKE_SUCCESS();
}

void VulkanCommandBuffer::finalize() {
    dispose();
    CommandBuffer::finalize();
}

void VulkanCommandBuffer::dispose() {
    if (m_inFlightFences) {
        // Wait for execution to complete as it may be pending.
        m_device->vk().vkWaitForFences(m_device->vkDevice(), 1, &m_inFlightFences, VK_TRUE, UINT64_MAX);
    }

    // CommandBuffer must be released before vkResetDescriptorPool.
    if (m_cmd) {
        VkCommandPool pool = m_device->commandPool();
        m_device->vk().vkFreeCommandBuffers(m_device->vkDevice(), pool, 1, &m_cmd);
        m_cmd = VK_NULL_HANDLE;
    }
    // Queue vkFreeCommandBuffers to run once the GPU finishes this frame.
    // If the command buffer was never submitted, free immediately.
    //const VolkDeviceTable* vk = &m_device->vk();
    //VkDevice dev = m_device->vkDevice();
    //VkCommandPool pool = m_device->commandPool();
    //VkCommandBuffer cmd = m_cmd;

    //if (m_submitted) {
    //    m_device->frameResources().queueDelete(m_submittedFrame, [vk, dev, pool, cmd]() {
    //        vk->vkFreeCommandBuffers(dev, pool, 1, &cmd);
    //    });
    //}
    //else {
    //    vk->vkFreeCommandBuffers(dev, pool, 1, &cmd);
    //}

    if (m_inFlightFences) {
        m_device->vk().vkDestroyFence(m_device->vkDevice(), m_inFlightFences, m_device->vulkanAllocator());
        m_inFlightFences = VK_NULL_HANDLE;
    }
}

VoidResult VulkanCommandBuffer::begin() {
    VkDevice vkDevice = m_device->vkDevice();

    // もし前回 vkQueueSubmit したコマンドバッファが完了していなければ待つ。
    // デバイスロスト時はフェンスが永遠にシグナルされない環境があるため、
    // 無限待ちではなく有限タイムアウトで待ちながらロスト状態を確認する。
    constexpr uint64_t kFenceTimeoutNs = 2'000'000'000ull; // 2秒
    for (;;) {
        VkResult r = m_device->checkDeviceLost(
            m_device->vk().vkWaitForFences(vkDevice, 1, &m_inFlightFences, VK_TRUE, kFenceTimeoutNs),
            "vkWaitForFences");
        if (r == VK_SUCCESS) break;
        if (m_device->isDeviceLost()) {
            return LN_MAKE_ERROR_WITH_CODE(
                ln::ErrorCode::DeviceLost, "Device lost while waiting for in-flight fence.");
        }
        if (r != VK_TIMEOUT) {
            return LN_MAKE_VULKAN_ERROR(r, "vkWaitForFences");
        }
        LN_LOG_WARNING("VulkanCommandBuffer::begin: in-flight fence wait timed out, retrying.");
    }
    m_device->vk().vkResetFences(vkDevice, 1, &m_inFlightFences);

    m_device->vk().vkResetCommandBuffer(m_cmd, 0);
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    if (VkResult r = m_device->vk().vkBeginCommandBuffer(m_cmd, &beginInfo); r != VK_SUCCESS) {
        return LN_MAKE_VULKAN_ERROR(r, "vkBeginCommandBuffer");
    }
    return LN_MAKE_SUCCESS();
}

RenderPass* VulkanCommandBuffer::beginRenderPass(const RenderPassDesc& desc) {
    // Build RenderPassKey
    RenderPassKey rpKey;
    uint32_t fbWidth = 0, fbHeight = 0;
    for (size_t i = 0; i < desc.colorAttachments.size(); ++i) {
        auto& ca = desc.colorAttachments[i];
        auto* view = static_cast<VulkanTextureView*>(ca.view);

        RenderPassKey::ColorAttachment attachment = {};
        attachment.format = view->vkFormat();
        attachment.loadOp = ca.loadOp == LoadOp::Clear ? VK_ATTACHMENT_LOAD_OP_CLEAR
            : ca.loadOp == LoadOp::Load                ? VK_ATTACHMENT_LOAD_OP_LOAD
                                                       : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment.isSwapchainBackbuffer = view->isSwapchainBackbuffer();
        rpKey.colorAttachments.push_back(attachment);

        if (fbWidth == 0) {
            fbWidth = view->width();
            fbHeight = view->height();
        }
    }
    if (desc.depthStencilAttachment) {
        rpKey.depthFormat = static_cast<VulkanTextureView*>(desc.depthStencilAttachment->view)
                                ->vkFormat();
        rpKey.depthLoadOp = desc.depthStencilAttachment->depthLoadOp == LoadOp::Clear
            ? VK_ATTACHMENT_LOAD_OP_CLEAR
            : desc.depthStencilAttachment->depthLoadOp == LoadOp::Load
            ? VK_ATTACHMENT_LOAD_OP_LOAD
            : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        rpKey.stencilLoadOp = desc.depthStencilAttachment->stencilLoadOp == LoadOp::Clear
            ? VK_ATTACHMENT_LOAD_OP_CLEAR
            : desc.depthStencilAttachment->stencilLoadOp == LoadOp::Load
            ? VK_ATTACHMENT_LOAD_OP_LOAD
            : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    }

    VkRenderPass vkRenderPass = m_device->getOrCreateRenderPass(rpKey);

    // Get or create cached VulkanRenderPass
    auto it = m_renderPassCache.find(rpKey);
    if (it == m_renderPassCache.end()) {
        // Build RenderPassLayoutDesc from the actual attachments
        RenderPassLayoutDesc layoutDesc;
        for (auto& ca : desc.colorAttachments) {
            auto* view = static_cast<VulkanTextureView*>(ca.view);
            layoutDesc.colorFormats.push_back(VulkanHelpers::fromVkFormat(view->vkFormat()));
        }
        if (desc.depthStencilAttachment) {
            layoutDesc.depthStencilFormat = VulkanHelpers::fromVkFormat(
                static_cast<VulkanTextureView*>(desc.depthStencilAttachment->view)->vkFormat());
        }

        auto rp = Ref<VulkanRenderPass>::adopt(new VulkanRenderPass(m_device, vkRenderPass, layoutDesc));
        it = m_renderPassCache.emplace(rpKey, std::move(rp)).first;
    }

    // Build FramebufferKey
    FramebufferKey fbKey;
    fbKey.renderPass = vkRenderPass;
    fbKey.width = fbWidth;
    fbKey.height = fbHeight;
    for (auto& ca : desc.colorAttachments) {
        fbKey.attachments.push_back(static_cast<VulkanTextureView*>(ca.view)->handle());
    }
    if (desc.depthStencilAttachment) {
        fbKey.attachments.push_back(
            static_cast<VulkanTextureView*>(desc.depthStencilAttachment->view)->handle());
    }

    VkFramebuffer framebuffer = m_device->getOrCreateFramebuffer(fbKey);
    VkExtent2D extent{fbKey.width, fbKey.height};

    m_encoder = it->second.get();
    m_encoder->beginEncoding(m_cmd, framebuffer, extent, desc);
    return m_encoder;
}

void VulkanCommandBuffer::submit() {
    m_device->vk().vkEndCommandBuffer(m_cmd);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_cmd;

    VkSemaphore waitSemaphore = VK_NULL_HANDLE;
    VkSemaphore signalSemaphore = VK_NULL_HANDLE;
    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkFence fence = VK_NULL_HANDLE;

    auto* sc = m_device->activeSwapChain();
    if (sc) {
        m_submittedFrame = sc->currentFrame();
        m_submitted = true;
        waitSemaphore = sc->imageAvailableSemaphore();
        signalSemaphore = sc->renderFinishedSemaphore();
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &waitSemaphore; // 実行を開始する前に待機するセマフォ
        submitInfo.pWaitDstStageMask = &waitStage;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &signalSemaphore; // 実行を完了したときに通知されるセマフォ
    }

    m_device->checkDeviceLost(
        m_device->vk().vkQueueSubmit(m_device->graphicsQueue(), 1, &submitInfo, m_inFlightFences),
        "vkQueueSubmit");

    m_encoder = nullptr;
}

} // namespace ln::rhi::vulkan
