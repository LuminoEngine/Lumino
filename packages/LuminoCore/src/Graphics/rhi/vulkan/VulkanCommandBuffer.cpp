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
    if (vkCreateFence(
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
        vkWaitForFences(m_device->vkDevice(), 1, &m_inFlightFences, VK_TRUE, UINT64_MAX);
    }

    // CommandBuffer must be released before vkResetDescriptorPool.
    if (m_cmd) {
        VkCommandPool pool = m_device->commandPool();
        vkFreeCommandBuffers(m_device->vkDevice(), pool, 1, &m_cmd);
        m_cmd = VK_NULL_HANDLE;
    }
    // Queue vkFreeCommandBuffers to run once the GPU finishes this frame.
    // If the command buffer was never submitted, free immediately.
    //VkDevice dev = m_device->vkDevice();
    //VkCommandPool pool = m_device->commandPool();
    //VkCommandBuffer cmd = m_cmd;

    //if (m_submitted) {
    //    m_device->frameResources().queueDelete(m_submittedFrame, [dev, pool, cmd]() {
    //        vkFreeCommandBuffers(dev, pool, 1, &cmd);
    //    });
    //}
    //else {
    //    vkFreeCommandBuffers(dev, pool, 1, &cmd);
    //}

    if (m_inFlightFences) {
        vkDestroyFence(m_device->vkDevice(), m_inFlightFences, m_device->vulkanAllocator());
        m_inFlightFences = VK_NULL_HANDLE;
    }
}

VoidResult VulkanCommandBuffer::begin() {
    // もし前回 vkQueueSubmit したコマンドバッファが完了していなければ待つ
    //vkWaitForFences(m_device->vulkanDevice(), 1, &m_inFlightFence, VK_TRUE, std::numeric_limits<uint64_t>::max());
    VkDevice vkDevice = m_device->vkDevice();
    vkWaitForFences(vkDevice, 1, &m_inFlightFences, VK_TRUE, UINT64_MAX);
    vkResetFences(vkDevice, 1, &m_inFlightFences);

    vkResetCommandBuffer(m_cmd, 0);
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    if (VkResult r = vkBeginCommandBuffer(m_cmd, &beginInfo); r != VK_SUCCESS) {
        return LN_MAKE_VULKAN_ERROR(r, "vkBeginCommandBuffer");
    }
    return LN_MAKE_SUCCESS();
}

RenderPass* VulkanCommandBuffer::beginRenderPass(const RenderPassDesc& desc) {
    // Build RenderPassKey
    RenderPassKey rpKey;
    u32 fbWidth = 0, fbHeight = 0;
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

        auto rp = Ref<VulkanRenderPass>::adopt(new VulkanRenderPass(vkRenderPass, layoutDesc));
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
    vkEndCommandBuffer(m_cmd);

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

    vkQueueSubmit(m_device->graphicsQueue(), 1, &submitInfo, m_inFlightFences);

    m_encoder = nullptr;
}

} // namespace ln::rhi::vulkan
